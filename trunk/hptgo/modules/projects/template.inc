<?php
/*
   Copyright HPT Commerce 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 08 August 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require_once($GO_MODULES->class_path.'task.class.inc');

if (!$GO_MODULES->write_permissions) {
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}
$showasgraph = isset($_REQUEST['show_as']) && (int)$_REQUEST['show_as'] != 0 ? 1 : 0;

$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$editor = '<img src="'.$GO_THEME->images['edit'].'" border="0">';
$cat_id = $_REQUEST['cat_id'];
if (!isset($cat_id)) $cat_id = 0;
$template_id = $_REQUEST['template_id'];
if (!isset($template_id)) $template_id = 0;

$task = $_REQUEST['task'];
$db = new db();

function reformat_predecessors($task_predecessors)
{
  if (!isset($task_predecessors) || $task_predecessors=='')
    $task_predecessors = '0';
  else {
    $t = array();
    $pre = explode(",", $task_predecessors);
    foreach ($pre as $p)
      $t[] = trim($p) + 0;
    $t = array_unique($t);
    if (count($t) <= 0)
      $task_predecessors = '0';
    else {
      sort($t);
      $task_predecessors = implode(",", $t);
    }
  }
  return $task_predecessors;
}

switch ($task) {
  case 'add_template':
    $template_name = $_REQUEST['template_name'];
    if (isset($template_name) && $template_name != '') {
      $db->query("SELECT * FROM task_templates WHERE cat_id=$cat_id AND name='$template_name'");
      if ($db->num_rows() > 0)
        echo sprintf($pm_TemplateExists, $catalog_name);
      else {
        $db->query("INSERT INTO task_templates(id, cat_id, name) ".
                   "VALUES('', $cat_id, '$template_name')");
        $db->query("SELECT * FROM task_templates WHERE cat_id=$cat_id AND name='$template_name'");
        $db->next_record();
        $id = $template_id = $db->f('id');
        $task_table = "task_template_$id";
        $db->query("CREATE TABLE $task_table (".
              "task_id int(8) NOT NULL default '0',".
              "task_name varchar(255) default NULL,".
              "task_predecessors varchar(255) default NULL,".
              "task_time int(4) default NULL,".
              "task_person_id tinyint(4) default NULL,".
              "task_status tinyint(4) default NULL,".
              "task_comment blob,".
              "task_approved char(1) default NULL,".
              "task_level tinyint(4) default '-1',".
              "task_approved_date datetime default NULL,".
              "task_order tinyint(4) default '0')");
        $db->query("SELECT * FROM pmStatus WHERE cat_id=$cat_id");
        $pstatus = array();
        while ($db->next_record()) {
          $pstatus[$db->f('value')] = $db->f('name');
        }
        $n = 1;
        foreach ($pstatus as $value => $name) {
          $p = $n - 1;
          $db->query("INSERT INTO $task_table(task_id, task_name, task_time, task_level, ".
                                             "task_predecessors, task_order) ".
                     "VALUES($n, '$name', 1, $value, '$p', $n)");
          $n = $n + 1;
        }
        $p = $n - 1;
        $db->query("INSERT INTO $task_table(task_id, task_name, task_time, task_level, ".
                                           "task_predecessors, task_order) ".
                   "VALUES($n, '$pm_status_done', 1, ".STATUS_DONE.", '$p', $n)");
      }
    }
    break;
  case 'delete_template':
    $db->query("SELECT * FROM task_templates WHERE cat_id=$cat_id AND id=$template_id");
    if ($db->num_rows() > 0) {
      $db->query("DROP TABLE task_template_$template_id");
      $db->query("DELETE FROM task_templates WHERE id=$template_id");
      $template_id = 0;
    }
    else {
      echo '<p class="Error">'.$pm_CannotDelete.'</p>';
    }
    break;
  case 'update_template':
    $template_name = $_REQUEST['catalog_name'];
    if (isset($template_name)) {
      $template_name = trim($template_name);
      if ($template_name != '')
        $db->query("UPDATE task_templates SET name='$template_name' WHERE id=$template_id");
    }
    break;

  case 'change_task_order':
    $task_value = $_REQUEST['task_value'];
    if ($task_value >= 1) {
      $task_change = $_REQUEST['task_change'];
      $related_task = $task_value + $task_change;
      if ($related_task >= 1) {
        error_log("$task_value <=> $related_task: $task_change");
        $tt_table = "task_template_$template_id";
        $db->query("SELECT * FROM $tt_table WHERE task_order IN ($task_value, $related_task)");
        $new_order = array($task_value => $related_task, $related_task => $task_value);
        while ($db->next_record())
          $cur_order[$db->f('task_id')] = $db->f('task_order');
        foreach ($cur_order as $id => $order)
          $db->query("UPDATE $tt_table SET task_order=".$new_order[$order]." WHERE task_id=$id");
      }
    }
    break;
  case 'delete_task':
    $task_value = $_REQUEST['task_value'];
    if ($task_value >= 1) {
      $task_id = $_REQUEST['task_change'];
      $tt_table = 'task_template_'.$template_id;
      $db->query("SELECT * FROM $tt_table ORDER BY task_order");
      $tp = array();
      while ($db->next_record()) {
        $no = $db->f('task_order');
        $p = explode(",", $db->f('task_predecessors'));
        if ($no == $task_value)
          $pre = $p;
        elseif (($n = array_search($task_id, $p)) !== false) {
          unset($p[$n]);
          $tp[$no] = $p;
        }
      }
      $db->query("DELETE FROM $tt_table ".
                 "WHERE task_order=$task_value");
      foreach ($tp as $no => $p) {
        $p = array_unique(array_merge($p, $pre));
        sort($p);
        $p = implode(",", $p);
        $db->query("UPDATE $tt_table SET task_predecessors='$p' WHERE task_order=$no");
      }
      $db->query("UPDATE $tt_table ".
                 "SET task_order=task_order-1 ".
                 "WHERE task_order>$task_value");
    }
    break;
  case 'update_task':
    $task_value = $_REQUEST['task_value'];
    if ($task_value >= 1) {
      $task_name = $_REQUEST['task_name'];
      $task_predecessors = $_REQUEST['task_predecessors'];
      if (isset($task_predecessors) && $task_predecessors != '') {
        $task_predecessors = reformat_predecessors($task_predecessors);
        $task_status = $task_predecessors == '0' ? TASK_ONGOING : TASK_WAITING;
        $extra = ", task_predecessors='$task_predecessors', task_status=$task_status ";
      }
      $db->query("UPDATE task_template_$template_id ".
                 "SET task_name='$task_name' ".$extra.
                 "WHERE task_order=$task_value");
    }
    break;
  case 'update_task_days':
    $task_value = $_REQUEST['task_value'];
    $task_time = $_REQUEST['task_time'] + 0;
    if ($task_value >= 1 && $task_time > 0) {
      $db->query("UPDATE task_template_$template_id ".
                 "SET task_time='$task_time' WHERE task_order=$task_value");
    }
    break;
  case 'update_task_predecessors':
    $task_value = $_REQUEST['task_value'];
    $task_predecessors = reformat_predecessors($_REQUEST['task_predecessors']);
    if ($task_value >= 1) {
      $task_status = $task_predecessors == '0' ? TASK_ONGOING : TASK_WAITING;
      $db->query("UPDATE task_template_$template_id ".
                 "SET task_predecessors='$task_predecessors',".
                     "task_status=$task_status ".
                 "WHERE task_id=$task_value");
    }
    break;
  case 'add_task':
    $task_value = $_REQUEST['task_value'];
    $last_value = $_REQUEST['last_value'];
    $task_name = $_REQUEST['task_name'];
    $task_predecessors = reformat_predecessors($_REQUEST['task_predecessors']);
    $task_time = $_REQUEST['task_time'] + 0;
    if ($task_time <= 0) $task_time = 1;
    if (isset($task_name) && $task_name != '') {
      $db->query("UPDATE task_template_$template_id ".
                 "SET task_order=task_order+1 WHERE task_order>=$task_value");
      $task_status = $task_predecessors == '0' ? TASK_ONGOING : TASK_WAITING;
      $sql = "INSERT INTO task_template_$template_id".
                          "(task_id, task_name, task_predecessors, ".
                          "task_time, task_status, task_level, task_order) ".
                          "VALUES($last_value, '$task_name', '$task_predecessors', ".
                          "$task_time, $task_status, -1, $task_value)";
      $db->query($sql);
    }
    break;
}

echo '<input type="hidden" name="post_action" value="template" />';
echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<tr><td>'.
       '<table border="0" cellpadding="2" cellspacing="0" width="550">'.
         '<tr><td align="right">'.$pm_category.':</td><td colspan="2">';

$db->query('SELECT * FROM pmCatalog');
$count = $db->num_rows();
$catalog = new dropbox();
$catalog_name = '';
while ($db->next_record()) {
  if ($cat_id == 0) $cat_id = $db->f('id');
  $catalog->add_value($db->f('id'), $db->f('name'));
  if ($cat_id == $db->f('id'))
    $catalog_name = $db->f('name');
}
echo '<input type="hidden" name="catalog_name" value="'.$catalog_name.'" />';
$catalog->print_dropbox('cat_id', $cat_id,
  'onchange="javascript:change_category()"', false, '10', '200');
$legend = '<table border="0" cellspacing="1" cellpadding="0" width="180" bgcolor="black">'.
          '<tr><td>'.
            '<table border="0" cellspacing="0" cellpadding="2" width="100%" bgcolor="#FFF7EB">'.
            '<tr>'.
              '<td align="center">'.
                '<img src="'.$GO_THEME->images['add_project_task'].'" border="0" /></td>'.
              '<td>'.$pm_legend_addtask.'</td>'.
            '</tr>'.
            '<tr>'.
              '<td align="center"><img src="'.$GO_THEME->images['task_up'].'" border="0" /><br>'.
              '<img src="'.$GO_THEME->images['task_dn'].'" border="0" /></td>'.
              '<td>'.$pm_legend_movetask.'</td>'.
            '</tr>'.
            '<tr>'.
              '<td align="center">'.$trash.'</td>'.
              '<td>'.$pm_legend_deltask.'</td>'.
            '</tr>'.
            '</table>'.
          '</td></tr>'.
          '</table>';
echo '</td><td rowspan="3" align="right">'.$legend.'</td></tr>';

$db->query('SELECT * FROM task_templates WHERE cat_id='.$cat_id);
$count = $db->num_rows();
$template = new dropbox();
while ($db->next_record()) {
  if ($template_id == 0) $template_id = $db->f('id');
  $template->add_value($db->f('id'), $db->f('name'));
  if ($template_id == $db->f('id'))
    $template_name = $db->f('name');
}
echo '<tr><td>&nbsp;</td>'.
     '<td colspan="2">'.
     '<a href="javascript:add_template()" class="normal">'.$pm_new_template.'</a></td>'.
     '</tr>';
echo '<tr><td align="right" nowrap>'.$pm_process_template.':</td><td>';
echo '<input type="hidden" name="template_name" value="'.$template_name.'" />';
$template->print_dropbox('template_id', $template_id,
    'onchange="javascript:frm.submit()"', false, '10', '200');
echo '</td><td width="60">'.
     '<a href="javascript:edit_template()" title="'.$cmdEdit.'">'.$editor.'</a>'.
     '<a href="javascript:delete_template()" title="'.$cmdDelete.'">'.$trash.'</a></td>';
echo '</tr>';
$showas_dropbox = new dropbox();
$showas_dropbox->add_value('0',$pm_list);
$showas_dropbox->add_value('1',$pm_graph);
echo '<tr><td align="right">'.$pm_show_as.'</td><td colspan="5">';
$showas_dropbox->print_dropbox('show_as',$showasgraph,'onchange="javascript:document.forms[0].submit()"',false,'10','200');
echo '</td></tr>';
echo '</table>';
echo '<table border="0" cellpadding="2" cellspacing="0" width="550">';

if ($count > 0) {
  $db->query('SELECT * FROM task_template_'.$template_id.' ORDER BY task_order');
  $count = $db->num_rows();
}

$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="12" height="12">';
$task_insert_btn = '<td align="center"><span id="task_insert_btn_[id]"><a href="javascript:insert_task([id])"><img src="'.$GO_THEME->images['add_project_task'].'" border="0" height="8" width="8" title="'.$pm_insert_task.'"></a></span></td>';
$spliter_cell = '<td align="center"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td>';
$spliter_line = '<td colspan="91" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td>';
$task_insert =
  '<tr id="task_row_[id]" style="visibility: hidden; display: none;">'.
  '<td><a href="javascript:save_task()">'.
    '<img src="'.$GO_THEME->images['task_pre_save'].'" border="0" title="Save" />'.
  '</a></td>'.
  '<td colspan="30"><span id="task_insert_[id]">&nbsp;</span></td>'.
  '<td colspan="30"><a href="javascript:show_task_list(-[id], '.$template_id.')">'.
    '<span id="task_prenew_[id]">[&nbsp;...&nbsp;]</span>'.
  '</a></td>'.
  '<td colspan="9" align="center">[&nbsp;<span id="task_days_new_[id]">1</span>&nbsp;]</td>'.
  '<td align="center" colspan="2">'.
  '<a href="javascript:save_task()">'.
    '<img src="'.$GO_THEME->images['task_pre_save'].'" border="0" title="Save" />'.
  '</a>&nbsp;&nbsp;'.
  '<a href="javascript:hide_editor(),hide_task_list()">'.
    '<img src="'.$GO_THEME->images['task_edit_cancel'].'" border="0" title="Cancel" />'.
  '</a></td>'.
  '</tr>'.
  '<tr id="task_row_spliter_[id]" style="visibility: hidden; display: none;">'.
  '<td>&nbsp;</td>'.
  '<td colspan="91" height="1">'.
    '<img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" />'.
  '</td>'.
  '</tr>';

if ($showasgraph) {
  require_once($GO_MODULES->class_path.'graph.class.inc');
?>
<map name="template_map">
<?php build_template_image($template_id,'cmap'); ?>
</map>
<img src="<?php echo $GO_MODULES->url.'template_graph.php?template_id='.$template_id; ?>" usemap="#template_map" border="0"/>
<?php
  // get last id
  while ($db->next_record()) {
    $id = $db->f('task_id');
  }
} else {
echo '<tr class="TableHead2" height="20">';
echo '<td align="center" width="20">#</td>';
echo '<td width="320" colspan="30" nowrap>'.$pm_task_name.'</td>';
echo '<td nowrap colspan="30">'.$pm_task_predecessors.'</td>';
echo '<td nowrap colspan="9">&nbsp;'.$pm_task_time.'&nbsp;</td>';
echo '<td nowrap colspan="2">&nbsp;</td>';
echo '</tr>';

if ($count > 0) {
  $tasks = array();
  while ($db->next_record()) {
    $n = $db->f('task_order');
    $id = $db->f('task_id');
    $name = $db->f('task_name');
    $level = $db->f('task_level');
    $level = $level >= 0 ? 0 : -$level;
    $time = $db->f('task_time');
    $pre = $db->f('task_predecessors');
    $tasks[$id] = array($n, $name, $level, $time, $pre);
  }

  echo '<tr>';
  echo '<td align="center">0</td>';
  echo '<td colspan="30" nowrap><b>'.$pm_status_begin.'</b></td>';
  echo '<td colspan="30">&nbsp;</td>';
  echo '<td colspan="9" align="center">&nbsp;</td>';
  echo '<td nowrap colspan="2">&nbsp;</td>';
  echo '</tr>';
  echo '<tr>';
  echo str_replace("[id]", 1, $task_insert_btn);
  echo $spliter_line.
       '</tr>';
  echo str_replace("[id]", 1, $task_insert);

  $max_id = 0;
  $nolist = $idlist = array();
  foreach ($tasks as $id => $data) {
    list($n, $name, $level, $time, $pre) = $data;

    $idlist[] = $id;
    $nolist[] = $n;

    $delitem = sprintf($pm_ConfirmDeleteStatus, $name);
    $dellink = "<a href='javascript:delete_task($id,\"$delitem\",$n)' ".
               "title=\"$strDeleteItem '$name'\">$trash</a>";
    $timelink = '<a href="javascript:set_edit_days('.$n.','.$time.')">'.$time.'</a>';
    $timespan = '[&nbsp;<span id="task_days_'.$n.'">'.$timelink.'</span>&nbsp;]';

    $tips = '<span id="task_tips_'.$n.'" '.
              'style="position: absolute; visibility:hidden; display: none;">'.
            '<table border="0" cellspacing="1" cellpadding="0" bgcolor="black">'.
            '<tr><td>'.
              '<table border="0" cellspacing="0" cellpadding="2" bgcolor="#FFF7EB">';
    $preorder = array();
    foreach (explode(',', $pre) as $p) {
      if ($p == 0) {
        $to = 0;
        $tn = $pm_status_begin;
      } else {
        $to = $tasks[$p][0];
        $tn = $tasks[$p][1];
      }
      $tips .= '<tr><td align="right">'.$to.'.</td><td>&nbsp;'.$tn.'</td></tr>';
      $preorder[] = $to;
    }
    $prelink = '<a href="javascript:show_task_list('.$id.','.$template_id.')">'.
               '[&nbsp;'.implode(",", $preorder).'&nbsp;]</a>';

    $tips .=  '</table></td></tr></table></span>';
    $tips_action = 'onmouseover="return show_tips('.$n.');" onmouseout="return hide_tips();';

    $name_link = "<a href='javascript:set_edit_name($n, \"$name\")'>$name</a>";
    if ($level == 0) $name_link = '<b>'.$name_link.'</b>';

    echo '<tr>';
    echo '<td align="center">'.$n.'</td>';
    echo '<td colspan="30" nowrap><span id="task_name_'.$n.'">'.$name_link.'</span></td>';
    echo '<td colspan="30">'.$tips.
           '<span id="task_pre_'.$n.'" '.$tips_action.'">'.$prelink.'</span>'.
         '</td>';
    echo '<td colspan="9" align="center">'.$timespan.'</td>';
    if ($level != 0) {
      $move_up_link = $move_dn_link = $move_space;
      if ($n >= 1 && $n < $count-1)
        $move_dn_link = '<a href="javascript:move_task('.$n.', 1)" title="'.$pm_move_task_dn.'">'.
                        $move_task_dn.'</a>';
      if ($n > 1 && $n <= $count-1)
        $move_up_link = '<a href="javascript:move_task('.$n.', -1)" title="'.$pm_move_task_up.'">'.
                        $move_task_up.'</a>';
      echo '<td align="center">'.$move_up_link.'&nbsp;'.$move_dn_link.'</td>';
      echo '<td>'.$dellink.'</td>';
    }
    echo '</tr>';

    echo '<tr>';
    if ($n < $count)
      echo str_replace("[id]", $n+1, $task_insert_btn);
    else
      echo $spliter_cell;
    echo $spliter_line.
         '</tr>';

    if ($n < $count)
      echo str_replace("[id]", $n+1, $task_insert);

    if ($id > $max_id) $max_id = $id;
  }

// Create Predecessor List
  $task_list = '<span id="task_list" '.
                 'style="position: absolute; visibility: hidden; display: none; z-index: 9999">'.
                '<table border="0" cellpadding="1" cellspacing="0" bgcolor="black">'.
                '<tr class="TableHead2">'.
                '<td width="18"><a href="javascript:save_task_prelist()">'.
                '<img src="'.$GO_THEME->images['task_pre_save'].'" border="0" title="Save"/>'.
                '</a></td>'.
                '<td nowrap>'.$pm_task_list_predecessors.'</td>'.
                '<td align="right"><a href="javascript:hide_task_list()">'.
                '<img src="'.$GO_THEME->images['task_list_close'].'" border="0" title="Close"/>'.
                '</a></td></tr>'.
                '<tr><td colspan="3">'.
                  '<iframe id="pre_frame" src="predecessors.php" '.
                  'width="220" height="80" class="textbox">'.
                  '</iframe>'.
                '</td></tr>'.
                '</table></span>';
  $idlist = count($idlist) > 0 ? "'".implode("','", $idlist)."'" : '';
  $nolist = count($nolist) > 0 ? "'".implode("','", $nolist)."'" : '';
}
else {
  echo '<tr><td colspan="92">&nbsp;</td></tr>';
  echo '<tr><td>&nbsp;</td>'.$spliter_line.'</tr>';
}
}
echo '<input type="hidden" name="last_value" value="'.($max_id+1).'" />';
echo '<input type="hidden" name="task_value" value="0" />';
echo '<input type="hidden" name="task_change" value="0" />';
echo '</table></td></tr></table>';
echo $task_list;
echo '<input id="task_edit" type="text" class="textbox" '.
     'style="position: absolute; visibility: hidden; display: none; width: 280px;" '.
     'name="task_name" value="" maxlength="255" />';
echo '<input id="task_days" type="text" class="textbox" '.
     'style="position: absolute; visibility: hidden; display: none; width: 30px;" '.
     'name="task_time" value="" maxlength="3" style="text-align: right;" align="right" />';
echo '<input type="hidden" name="task_predecessors" value="" />';
?>
<script type="text/javascript">
var agent = navigator.userAgent.toLowerCase();
var is_ie = ((agent.indexOf("msie") != -1) && (agent.indexOf("opera") == -1));
var frm = document.forms[0];

var task_edit = document.getElementById("task_edit");
var task_days = document.getElementById("task_days");
var task_pre_container = null;
var task_insert_btn = null;
var task_row = null;
var task_row_spliter = null;
var task_action = null;
var task_list = null;

var idlist = new Array(<?php echo $idlist;?>);
var nolist = new Array(<?php echo $nolist;?>);

// Set Netscape up to run the "captureMousePosition" function whenever
// the mouse is moved. For Internet Explorer and Netscape 6, you can capture
// the movement a little easier.
if (document.layers) { // Netscape
  document.captureEvents(Event.MOUSEMOVE);
  document.onmousemove = captureMousePosition;
} else if (document.all) { // Internet Explorer
  document.onmousemove = captureMousePosition;
} else if (document.getElementById) { // Netcsape 6
  document.onmousemove = captureMousePosition;
}
// Global variables
var xMousePos = 0; // Horizontal position of the mouse on the screen
var yMousePos = 0; // Vertical position of the mouse on the screen
var xMousePosMax = 0; // Width of the page
var yMousePosMax = 0; // Height of the page

function captureMousePosition(e) {
  if (document.layers) {
    // When the page scrolls in Netscape, the event's mouse position
    // reflects the absolute position on the screen. innerHight/Width
    // is the position from the top/left of the screen that the user is
    // looking at. pageX/YOffset is the amount that the user has
    // scrolled into the page. So the values will be in relation to
    // each other as the total offsets into the page, no matter if
    // the user has scrolled or not.
    xMousePos = e.pageX;
    yMousePos = e.pageY;
    xMousePosMax = window.innerWidth+window.pageXOffset;
    yMousePosMax = window.innerHeight+window.pageYOffset;
  } else if (document.all) {
    // When the page scrolls in IE, the event's mouse position
    // reflects the position from the top/left of the screen the
    // user is looking at. scrollLeft/Top is the amount the user
    // has scrolled into the page. clientWidth/Height is the height/
    // width of the current page the user is looking at. So, to be
    // consistent with Netscape (above), add the scroll offsets to
    // both so we end up with an absolute value on the page, no
    // matter if the user has scrolled or not.
    xMousePos = window.event.x+document.body.scrollLeft;
    yMousePos = window.event.y+document.body.scrollTop;
    xMousePosMax = document.body.clientWidth+document.body.scrollLeft;
    yMousePosMax = document.body.clientHeight+document.body.scrollTop;
  } else if (document.getElementById) {
    // Netscape 6 behaves the same as Netscape 4 in this regard
    xMousePos = e.pageX;
    yMousePos = e.pageY;
    xMousePosMax = window.innerWidth+window.pageXOffset;
    yMousePosMax = window.innerHeight+window.pageYOffset;
  }
//  window.status = "xMousePos=" + xMousePos + ", yMousePos=" + yMousePos + ", xMousePosMax=" + xMousePosMax + ", yMousePosMax=" + yMousePosMax;
}

function submit_task_info()
{
  if (task_action && (task_edit.value != '' || task_days.value != '')) {
    frm.task.value = task_action;
    frm.submit();
  }
}

function save_task()
{
  if (task_edit.value != '' && parseInt(task_days.value) > 0) {
    frm.task.value = task_action;
    frm.submit();
  }
}

function task_edit_keypressed(e)
{
  if (is_ie) e = window.event;
  switch (e.keyCode) {
    case 13:
      submit_task_info();
      break;
    case 27:
      hide_editor();
      hide_task_list();
      frm.task_predecessors.value = '';
      break;
  }
  return true;
}

document.onkeypress = task_edit_keypressed;

function task_days_keypressed(e)
{
  if (is_ie) e = window.event;
  switch (e.keyCode) {
    case 0:
      if (e.which >= 48 && e.which <= 57)
        return true;
    case 13:
      submit_task_info();
      break;
    case 38:
      n = parseInt(task_days.value);
      task_days.value = (n + 1);
      break;
    case 40:
      n = parseInt(task_days.value);
      if (n > 1)
        task_days.value = (n - 1);
      break;
  }
  return false;
}

function get_position(element)
{
  var elem = element, tagname = "", x = 0, y = 0;

  while ((typeof(elem) == "object") && (typeof(elem.tagName) != "undefined"))
  {
    y += elem.offsetTop;
    x += elem.offsetLeft;
    tagname = elem.tagName.toUpperCase();

    if (tagname == "BODY") elem = 0;
    if (typeof(elem) == "object")
      if (typeof(elem.offsetParent) == "object")
        elem=elem.offsetParent;
  }

  position = new Object();
  position.x = x;
  position.y = y;
  return position;
}

var task_tips = null;
function show_tips(id)
{
  if (task_list || task_tips) return false;

  task_tips = document.getElementById("task_tips_"+id);
  task_tips.style.display = '';
  task_tips.style.visibility = 'visible';
  p = get_position(document.getElementById("task_pre_"+id));
  x = p.x, y = p.y - task_tips.offsetHeight;
  task_tips.style.top = y + "px";
  task_tips.style.left = x + "px";
  return false;
}

function hide_tips()
{
  if (task_tips) {
    task_tips.style.display = 'none';
    task_tips.style.visibility = 'hidden';
    task_tips = null;
  }
  return false;
}

function show_task_list(id, template_id)
{
  extra = id <= 0 ? "&predecessors="+frm.task_predecessors.value : "";
  pre_frame = document.getElementById("pre_frame");
  src = "predecessors.php?template_id="+template_id+"&task_id="+id+extra;
  if (is_ie)
    pre_frame.location.href = src;
  else
    pre_frame.src = src;
  task_list = document.getElementById("task_list");
  if (yMousePos + 85 >= yMousePosMax)
    yMousePos -= 90;
  task_list.style.top = yMousePos + "px";
  task_list.style.left = xMousePos + "px";
  task_list.style.display = '';
  task_list.style.visibility = 'visible';
  if (id > 0) frm.task_value.value = id;
}

function hide_task_list()
{
  if (task_list) {
    task_list.style.display = 'none';
    task_list.style.visibility = 'hidden';
    task_list = null;
    if (task_action) task_edit.focus();
  }
}

function save_task_prelist()
{
  pre_frame = document.getElementById("pre_frame");
  if (pre_frame.contentDocument)
    pdoc = pre_frame.contentDocument;
  else if (pre_frame.contentWindow)
    pdoc = pre_frame.contentWindow.document;
  else if (pre_frame.document)
    pdoc = pre_frame.document;
  p = pdoc.getElementById("predecessors");
  frm.task_predecessors.value = p.value;

  if (task_pre_container) {
    no = new Array();
    id = p.value.split(',');
    for (i = c = 0; i < id.length; i++) {
      for (n = 0; n < idlist.length; n++)
        if (id[i] == parseInt(idlist[n])) {
          no[c++] = nolist[n];
          break;
        }
    }
    id = '';
    for (i = 0; i < no.length - 1; i++)
      id += no[i] + ',';
    id += no[i];

    task_pre_container.innerHTML = '[&nbsp;'+id+'&nbsp;]';
  }
  else {
    frm.task.value = 'update_task_predecessors';
    frm.submit();
  }
  hide_task_list();
}

function move_task(n, action)
{
  frm.task.value = 'change_task_order';
  frm.task_value.value = n;
  frm.task_change.value = action;
  frm.submit();
}

function delete_task(task_id, task_name, task_no)
{
  if (confirm(task_name)) {
    frm.task.value = 'delete_task';
    frm.task_value.value = task_no;
    frm.task_change.value = task_id;
    frm.submit();
  }
}

function show_editor(container, editor, value, handler, offset)
{
  p = get_position(container);
  editor.style.top = p.y + "px";
  editor.style.left = (offset + p.x) + "px";
  editor.style.display = '';
  editor.style.visibility = 'visible';
  editor.value = value == null ? container.innerHTML : value;
  editor.focus();
  if (value != null && value != '')
    editor.select();
  if (handler) {
    if (is_ie)
      editor.attachEvent("onkeypress", handler);
    else
      editor.addEventListener("keypress", handler, false);
  }
  document.location.hash='edit';
}

function hide_editor()
{
  task_action = null;
  task_edit.blur();
  task_edit.style.display = 'none';
  task_edit.style.visibility = 'hidden';
  task_days.blur();
  task_days.style.display = 'none';
  task_days.style.visibility = 'hidden';
  if (task_insert_btn) {
    task_insert_btn.style.visibility = 'visible';
    task_insert_btn = null;
    task_row.style.display = 'none';
    task_row.style.visibility = 'hidden';
    task_row = null;
    task_row_spliter.style.display = 'none';
    task_row_spliter.style.visibility = 'hidden';
    task_row_spliter = null;
  }
  task_pre_container = null;
}

function set_edit_name(n, name)
{
  hide_editor();

  task_action = 'update_task';
  frm.task_value.value = n;
  show_editor(document.getElementById("task_name_"+n),
//      task_edit, name, task_edit_keypressed, 0);
      task_edit, name, null, 0);
}

function set_edit_days(n, days)
{
  hide_editor();

  task_action = 'update_task_days';
  frm.task_value.value = n;
  show_editor(document.getElementById("task_days_"+n),
      task_days, days, task_days_keypressed, -12);
}

function insert_task(p)
{
  hide_editor();

  frm.task_value.value = p;

  task_action = 'add_task';
  task_insert_btn = document.getElementById("task_insert_btn_"+p);
  task_insert_btn.style.visibility = 'hidden';

  task_row = document.getElementById("task_row_"+p);
  task_row.style.display = '';
  task_row.style.visibility = 'visible';

  task_row_spliter = document.getElementById("task_row_spliter_"+p);
  task_row_spliter.style.display = '';
  task_row_spliter.style.visibility = 'visible';

  task_pre_container = document.getElementById("task_prenew_"+p);
  task_pre_container.innerHTML = '[&nbsp;...&nbsp;]';

  show_editor(document.getElementById("task_days_new_"+p),
      task_days, null, task_days_keypressed, -12);
//  show_editor(document.getElementById("task_insert_"+p), task_edit, "", task_edit_keypressed, 0);
  show_editor(document.getElementById("task_insert_"+p),
      task_edit, "", null, 0);
}

function edit_template()
{
  name = prompt("<?php echo $pm_EditTemplate; ?>", frm.template_name.value);
  if (name == null) return;
  name = name.replace(/^\s+|\s+$/g, '');
  if (name != '') {
    frm.task.value = 'update_template';
    frm.catalog_name.value = name;
    frm.submit();
  }
}

function add_template()
{
  name = prompt("<?php echo $pm_AddTemplate; ?>", "");
  if (name != null && name != '') {
    frm.task.value = 'add_template';
    frm.template_name.value = name;
    frm.submit();
  }
}

function delete_template()
{
  if (confirm("<?php echo $pm_ConfirmDeleteTemplate; ?>" + "'" + frm.template_name.value + "' ?")) {
    frm.task.value = 'delete_template';
    frm.submit();
  }
}

function change_category()
{
  frm.template_name.value = '';
  frm.template_id.selectedIndex = -1;
  frm.submit();
}
</script>
