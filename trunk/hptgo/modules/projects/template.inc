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

$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$editor = '<img src="'.$GO_THEME->images['edit'].'" border="0">';
$cat_id = $_REQUEST['cat_id'];
if (!isset($cat_id)) $cat_id = 0;
$template_id = $_REQUEST['template_id'];
if (!isset($template_id)) $template_id = 0;

$task = $_REQUEST['task'];
$db = new db();

switch ($task) {
  case 'add_template':
    $template_name = $_REQUEST['template_name'];
    if (isset($template_name) && $template_name != '') {
      $db->query("SELECT * FROM task_templates WHERE cat_id=$cat_id AND name='$template_name'");
      if ($db->num_rows() > 0)
        echo sprintf($pm_TemplateExists, $catalog_name);
      else {
        $db->query("INSERT INTO task_templates(id, cat_id, name) VALUES('', $cat_id, '$template_name')");
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
              "task_level tinyint(4) default '1',".
              "task_approved_date datetime default NULL)");
        $db->query("SELECT * FROM pmStatus WHERE cat_id=$cat_id");
        $pstatus = array();
        while ($db->next_record()) {
          $pstatus[$db->f('value')] = $db->f('name');
        }
        $n = 1;
        foreach ($pstatus as $value => $name) {
          $db->query("INSERT INTO $task_table(task_id, task_name, task_level) ".
              "VALUES($n, '$name', $value)");
          $n = $n + 1;
        }
        $db->query("INSERT INTO $task_table(task_id, task_name, task_level) ".
              "VALUES($n, '$pm_status_done', ".STATUS_DONE.")");
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
    $template_name = $_REQUEST['template_name'];
    if (isset($template_name) && $template_name != '')
      $db->query("UPDATE task_templates SET name='$template_name' WHERE id=$template_id");
    break;

  case 'change_task_order':
    $task_value = $_REQUEST['task_value'];
    if ($task_value > 1) {
      $task_change = $_REQUEST['task_change'];
      $related_task = $task_value + $task_change;
      $tt_table = "task_template_$template_id";
      $db->query("UPDATE $tt_table SET task_id=999 WHERE task_id=$task_value");
      $db->query("UPDATE $tt_table SET task_id=$task_value WHERE task_id=$related_task");
      $db->query("UPDATE $tt_table SET task_id=$related_task WHERE task_id=999");
    }
    break;
  case 'delete_task':
    $task_value = $_REQUEST['task_value'];
    if ($task_value > 1) {
      $db->query("DELETE FROM task_template_$template_id ".
                 "WHERE task_id=$task_value");
      $task_value = $task_value + 1;
      $db->query("UPDATE task_template_$template_id SET task_id=task_id-1 ".
                 "WHERE task_id=$task_value");
    }
    break;
  case 'update_task':
    $task_value = $_REQUEST['task_value'];
    if ($task_value > 1) {
      $task_name = $_REQUEST['task_name'];
      $task_predecessors = $_REQUEST['task_predecessors'];
      $task_time = $_REQUEST['task_time'];
      if ($task_predecessors == '0')
        $task_status = TASK_ONGOING;
      else
        $task_status = TASK_WAITING;
      $db->query("UPDATE task_template_$template_id ".
                 "SET task_name='$task_name', ".
                     "task_time='$task_time', ".
                     "task_predecessors='$task_predecessors', ".
                     "task_status=$task_status ".
                 "WHERE task_id=$task_value");
    }
    break;
  case 'add_task':
    $last_value = $_REQUEST['last_value'];
    $task_name = $_REQUEST['task_name'];
    $task_predecessors = $_REQUEST['task_predecessors'];
    $task_time = $_REQUEST['task_time'];
    if (!isset($task_time)) $task_time = 0;
    if (isset($task_name) && $task_name != '') {
      $db->query("UPDATE task_template_$template_id ".
                 "SET task_id=$last_value+1 WHERE task_id=$last_value");
      if ($task_predecessors == '0')
        $task_status = TASK_ONGOING;
      else
        $task_status = TASK_WAITING;
      $sql = "INSERT INTO task_template_$template_id".
          "(task_id, task_name, task_predecessors, task_time, task_status, task_level) ".
          "VALUES($last_value, '$task_name', '$task_predecessors', $task_time, $task_status, -1)";
      $db->query($sql);
    }
    break;
}

echo '<input type="hidden" name="post_action" value="template" />';
echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<tr><td><table border="0" cellpadding="2" cellspacing="0"><tr><td colspan="2">'.$pm_category.':</td><td colspan="5">';

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
echo '</td></tr>';

$db->query('SELECT * FROM task_templates WHERE cat_id='.$cat_id);
$count = $db->num_rows();
$template = new dropbox();
while ($db->next_record()) {
  if ($template_id == 0) $template_id = $db->f('id');
  $template->add_value($db->f('id'), $db->f('name'));
  if ($template_id == $db->f('id'))
    $template_name = $db->f('name');
}
echo '<tr><td colspan="2">&nbsp;</td><td><a href="javascript:add_template()" class="normal">Tạo quy trình mới</a></tr>';
echo '<tr><td colspan="2">'.$pm_process_template.':</td><td colspan="5">';
echo '<input type="hidden" name="template_name" value="'.$template_name.'" />';
$template->print_dropbox('template_id', $template_id,
    'onchange="javascript:document.forms[0].submit()"', false, '10', '200');
echo '</td>';
echo '<td><a href="javascript:edit_template()" title="'.$cmdEdit.'">'.$editor.'</a></td>';
echo "<td><a href=\"javascript:delete_template('$pm_ConfirmDeleteTemplate')\" title=\"$cmdDelete\">$trash</a></td>";
echo '</tr></table>';
echo '<table border="0" cellpadding="2" cellspacing="0">';

if ($count > 0) {
  $db->query('SELECT * FROM task_template_'.$template_id.' ORDER BY task_id');
  $count = $db->num_rows();
}

$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="12" height="12">';
$spliter = '<tr><td colspan="102" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

echo '<tr class="TableHead2" height="20">';
echo '<td align="center" width="20">#</td>';
echo '<td width="320" colspan="30" nowrap>'.$pm_task_name.'</td>';
echo '<td nowrap colspan="30">'.$pm_task_predecessors.'</td>';
echo '<td nowrap colspan="9">'.$pm_task_days.'</td>';
echo '<td nowrap colspan="30">'.$pm_task_level.'</td>';
echo '<td nowrap colspan="2">&nbsp;</td>';
echo '</tr>';

$value = 1;
if ($count > 0) {
  echo '<tr>';
  echo '<td align="center">0</td>';
  echo "<td colspan=\"30\" nowrap>$pm_status_begin</td>";
  echo '<td colspan="30">&nbsp;</td>';
  echo '<td colspan="9" align="center">&nbsp;</td>';
  echo '<td colspan="30" align="center">0</td>';
  echo '<td nowrap colspan="2">&nbsp;</td>';
  echo '</tr>';
  echo $spliter;
  while ($db->next_record()) {
    $id = $db->f('task_id');
    $name = $db->f('task_name');
    $delitem = sprintf($pm_ConfirmDeleteStatus, $name);
    $delitem_hint = "$strDeleteItem '$name'";
    $level = $db->f('task_level');
    $level = $level >= 0 ? 0 : -$level;
    $time = $db->f('task_time');
    $pre = $db->f('task_predecessors');
    echo '<tr>';
    echo '<td align="center">'.$id.'</td>';
    if ($level == 0)
      echo "<td colspan=\"30\" nowrap><b><a href='javascript:set_edit(\"$name\", \"$pre\", \"$time\", $id)'>$name</a></b></td>";
    else
      echo "<td colspan=\"30\" nowrap><a href='javascript:set_edit(\"$name\", \"$pre\", \"$time\", $id)'>$name</a></td>";
    echo '<td colspan="30">'.$db->f('task_predecessors').'</td>';
    echo '<td colspan="9" align="center">'.$db->f('task_time').'</td>';
    echo '<td colspan="30" align="center">'.$level.'</td>';
    if ($level != 0) {
      if ($id == 1)
        echo '<td align="center">'.$move_space.'<a href="javascript:move_task('.$id.', 1)" title="'.$delitem_hint.'">'.$move_task_dn.'</a></td>';
      elseif ($id == $count-1)
        echo '<td align="center"><a href="javascript:move_task('.$id.', -1)">'.$move_task_up.'</a>'.$move_space.'</td>';
      else
        echo '<td align="center"><a href="javascript:move_task('.$id.', -1)">'.$move_task_up.' <a href="javascript:move_task('.$id.', 1)">'.$move_task_dn.'</td>';
      echo "<td><a href='javascript:delete_task(\"$delitem\",$id)'>$trash</a></td>";
    }
    echo '</tr>';

    echo $spliter;
  }
}
else {
  echo '<tr><td colspan="102">&nbsp;</td></tr>';
  echo '<tr><td colspan="102" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '<input type="hidden" name="last_value" value="'.$id.'" />';
echo '<input type="hidden" name="task_value" value="0" />';
echo '<input type="hidden" name="task_change" value="0" />';
echo '<br />';
echo '<tr><td colspan="100">'.$pm_task_name.':</td></tr>';
echo '<tr><td colspan="31">';
echo '<input type="text" class="textbox" style="width: 350px;" name="task_name" value="" maxlength="50" /></td>';
echo '<td colspan="30">';
echo '<input type="text" class="textbox" style="width: 50px;" name="task_predecessors" value="" maxlength="20" /></td>';
echo '<td colspan="9">';
echo '<input type="text" class="textbox" style="width: 35px;" name="task_time" value="" maxlength="3" /></td><td></td></tr>';
echo '</table></td></tr></table>';
$button = new button('Add', 'javascript:add_task()');
echo '&nbsp;&nbsp;';
$button = new button('Update', 'javascript:update_task()');
?>

<script type="text/javascript">

function move_task(task_value, action)
{
  document.forms[0].task.value = 'change_task_order';
  document.forms[0].task_value.value = task_value;
  document.forms[0].task_change.value = action;
  document.forms[0].submit();
}

function delete_task(task_name, task_value)
{
  if (confirm(task_name)) {
    document.forms[0].task.value = 'delete_task';
    document.forms[0].task_value.value = task_value;
    document.forms[0].submit();
  }
}

function set_edit(name, pre, days, task_value)
{
  document.forms[0].task_name.value = name;
  document.forms[0].task_time.value = days;
  document.forms[0].task_predecessors.value = pre;
  document.forms[0].task_value.value = task_value;
}

function add_task()
{
  if (document.forms[0].task_name.value != '') {
    document.forms[0].task.value = 'add_task';
    document.forms[0].submit();
  }
}

function update_task()
{
  if (document.forms[0].task_name.value != '') {
    document.forms[0].task.value = 'update_task';
    document.forms[0].submit();
  }
}

function edit_template()
{
  name = prompt("<?php echo $pm_EditTemplate; ?>", document.forms[0].template_name.value);
  if (name != '') {
    document.forms[0].task.value = 'update_template';
    document.forms[0].catalog_name.value = name;
    document.forms[0].submit();
  }
}

function add_template()
{
  name = prompt("<?php echo $pm_AddTemplate; ?>", "");
  if (name != '') {
    document.forms[0].task.value = 'add_template';
    document.forms[0].template_name.value = name;
    document.forms[0].submit();
  }
}

function delete_template(msg)
{
  if (confirm(msg + "'" + document.forms[0].template_name.value + "' ?")) {
    document.forms[0].task.value = 'delete_template';
    document.forms[0].submit();
  }
}

function change_category()
{
  document.forms[0].template_name.value = '';
  document.forms[0].template_id.value = 0;
  document.forms[0].submit();
}
</script>
