<?php
/*
   Copyright HPT Corporation 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 30 June 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$pstate = $projects['status'];
$project_acl_write = $project['acl_write'];
$pstart_date = $project['start_date'];
$pstop_date = $project['end_date'];
$project_id = isset($_GET['project_id']) ? $_GET['project_id'] : '0';
$projects->query("SELECT user_id, last_name, middle_name, first_name FROM acl, users ".
                 "WHERE acl.acl_id=$project_acl_write AND acl.user_id=users.id");
while ($projects->next_record()) {
  $middle_name = $projects->f('middle_name');
  if (isset($middle_name))
    $middle_name = " ".$middle_name." ";
  $member_ids[] = $projects->f('user_id');
  $member_names[] = $projects->f('last_name') . $middle_name . $projects->f('first_name');
  $member_list[$projects->f('user_id')] = 
  $projects->f('last_name') . $middle_name . $projects->f('first_name');
}

$monitor_ids = array();
$project_acl_read = $project['acl_read'];
$projects->query("SELECT DISTINCT id as user_id, last_name, middle_name, first_name".
                 " FROM acl, users, users_groups ".
                 "WHERE acl.acl_id=$project_acl_read".
                 " AND ((acl.user_id=users.id AND acl.group_id=0) OR (acl.user_id=0 AND users.id=users_groups.user_id AND users_groups.group_id=acl.group_id))");
$monitors = array();
while ($projects->next_record()) {
  $middle_name = $projects->f('middle_name');
  if (isset($middle_name))
    $middle_name = ' '.$middle_name.' ';
  $monitors[$projects->f('user_id')] =
    $projects->f('last_name') . $middle_name . $projects->f('first_name');
}

$today = getdate();
$today = gmmktime(0, 0, 0, $today['mon'], $today['mday'], $today['year']);

$tasks = load_project_task($projects, $project_id);

if ($task != 'show_task_gantt') {
  if (isset($_GET['task_status']))
    $task_status = $_GET['task_status'];
  $save_type = isset($task_status) ? "status" : "list";

  echo '<input type="hidden" name="max_task_id" value="'.$max_task_id.'">';
  echo '<table border="0" cellpadding="0" cellspacing="0">';
  show_planned_eslapsed_times();
  echo '<tr><td class="TableHead2" width="25" align="center" nowrap>'.$pm_task_order.'</td>';
  echo     '<td class="TableHead2" width="200" nowrap>'.$pm_task_name.'</td>';
  echo     '<td class="TableHead2" width="190" nowrap>'.$pm_task_person.'</td>';
  echo     '<td class="TableHead2" width="100" nowrap>'.$pm_task_predecessors.'</td>';
  echo     '<td class="TableHead2" width="45" nowrap>'.$pm_task_time.'</td>';
  echo     '<td class="TableHead2" width="80" nowrap>'.$pm_task_status.'</td>';
  echo     '<td class="TableHead2" width="50" align="center" nowrap>'.$pm_task_approved.'</td>';
  echo     '<td class="TableHead2" width="50" align="center" nowrap>'.$pm_task_document.'</td></tr>';

  // Display Task List
  if (!isset($task_status) || isset($_REQUEST['task_status_close'])) {
    foreach ($tasks as $id => $t)
      $t->print_info($member_ids, $member_names, $member_list);
    $stask = $tasks[1];
  }
  // Display Task Status
  else {
    $task_id = isset($_GET['task_id']) ? $_GET['task_id'] : '0';
    $stask = $tasks[$task_id];
    if ($task_status == 'show_doc') {
      $stask->show_doc_folder();
    }
    else {
      echo '<input type="hidden" name="task_status_close" value="1"/>';
      $stask->print_status($member_list);
    }
  }
}
else {
  echo '<table border="0" cellpadding="0" cellspacing="0">';
  show_planned_eslapsed_times();
  echo '<tr><td class="TableHead2" width="25" align="center" nowrap>'.$pm_task_order.'</td>';
  echo '<td class="TableHead2" width="715" align="center" colspan="2">'.$cmdGantt.'</td></tr>';

  $date = strftime("%d/%m", $pstart_date);
  echo '<tr class="TableInside2"><td width="25" align="center" nowrap>&nbsp;</td>';
  echo '<td width="150">&nbsp;</font></td>';
  echo '<td width="565" class="GanttDate" nowrap><b>'."$pm_start_date ($date)<br>v</b></td></tr>";

  $day_sec = 24 * 3600;
  if ($today == 0) {
    $today = getdate();
    $today = gmmktime(0, 0, 0, $today['mon'], $today['mday'], $today['year']);
  }
  $duration = round((($today < $pstop_date ? $pstop_date : $today) - $pstart_date) / $day_sec);
  if ($duration == 0) $duration = 1;
  $day_space = round(480 / $duration);

  $n = 1;
  foreach ($tasks as $id => $t) {
    list($start_date, $real_start_date) = $t->find_start_date();
    $display_date = $start_date * $day_space;
    $display_rdate = $real_start_date * $day_space;
    $ndays = $t->days * $day_space;
    $date = strftime("%d/%m", $pstart_date + ($start_date + $t->days) * $day_sec);
    if ($t->status == TASK_ONGOING || ($t->status == TASK_DONE && !$t->approved)) {
      $d = round(($today - $pstart_date) / $day_sec) - $real_start_date;
      $odate = strftime("%d/%m", $pstart_date + ($real_start_date + $d) * $day_sec);
      $cwidth = $d * $day_space;
      if (!$cwidth) $cwidth = 2;
      $current = '<img src="'.$GO_THEME->images['task_on'].'" width="'.$cwidth.'" height="7">&nbsp;<font color="green">'.$odate.'</font>';
      $cspace = $real_start_date * $day_space;
      $current_space = '<img src="'.$GO_THEME->images['task_space'].'" width="'.$cspace.'" height="7">';
    }
    elseif ($t->approved) {
      $d = $t->approved_date == '' ? 0 :
           (round((strtotime($t->approved_date) - $pstart_date) / $day_sec) - $real_start_date);
      $adate = strftime("%d/%m", $pstart_date + ($real_start_date + $d) * $day_sec);
      $cwidth = $d * $day_space;
      if (!$cwidth) $cwidth = 2;
      $current = '<img src="'.$GO_THEME->images['task_done'].'" width="'.$cwidth.'" height="7">&nbsp;<font color="red">'.$adate.'</font>';
      $cspace = $real_start_date * $day_space;
      $current_space = '<img src="'.$GO_THEME->images['task_space'].'" width="'.$cspace.'" height="7">';
    }
    else {
      $current = '&nbsp;';
      $current_space = "";
    }

    if ($ndays == 0) $ndays = 2;
    $space = '<img src="'.$GO_THEME->images['task_space'].'" width="'.$display_date.'" height="7">';
    $image = '<img src="'.$GO_THEME->images['task_wait'].'" width="'.$ndays.'" height="7">';
    echo '<tr class="TableInside'.$n.'"><td width="25" align="center" nowrap>'.$t->order.'</td>';
    echo '<td nowrap>'.$t->name.'</td>';
    echo '<td width="565" class="GanttDate" nowrap>'.$space.$image.'&nbsp;<font color="blue"><i>'.$date.'<br>'.$current_space.$current.'</i></font></td></tr>';
    $n = $n == 1 ? 2 : 1;
  }

  $date = strftime("%d/%m", $pstop_date);
  $ndays = round(($pstop_date - $pstart_date) / $day_sec);
  $display_date = $ndays * $day_space;
  $space = '<img src="'.$GO_THEME->images['task_space'].'" width="'.$display_date.'" height="7">';
  echo '<tr class="TableInside'.$n.'"><td width="25">&nbsp;</td><td width="150">&nbsp;</td>';
  echo '<td class="GanttDate" nowrap>'."$space<b>^<br>$space$pm_end_date ($date)</b></td></tr>";
}

echo '</table><table border="0" cellspacing="0" cellpadding="4">';
echo '<tr><td colspan="2">&nbsp;</td></tr><tr><td>';

if ($task != 'show_task_gantt') {
  if ($stask->project_status!=STATUS_DROP) {
    if (!isset($task_status) || $task_status != 'show_doc') {
      if ((isset($task_status) && !$stask->approved) ||
         ($stask->is_manager && (!isset($task_status) ||
         ($task_status != TASK_DONE && !$stask->approved))))
      {
        $button = new button($cmdSave, "javascript:_save_task('$save_type');");
        echo '&nbsp;&nbsp;';
      }
    }

    if (!isset($task_status) && !$is_new_project) {
      $button = new button($cmdGantt, "javascript:_gantt();");
      echo '&nbsp;&nbsp;';
    }
    else
    if ($task_status >= TASK_ONGOING) {
      $button = new button($cmdReset, "javascript:_reset();");
      echo '&nbsp;&nbsp;';
    }
  }
}

$_SESSION['GO_FILESYSTEM_PATH'] = $GO_CONFIG->file_storage_path.'users/'.
                                  $_SESSION['GO_SESSION']['username'];
if ($task != 'show_task_gantt')
{
  if (!$is_new_project) {
    if ($task == 'show_task_status' || $task == 'show_task_doc') {
      if ($_GET['task_status'] == 'show_doc') {
	$return_to2 = $_REQUEST['return_to2'];
	$button = new button($cmdClose, "javascript:document.location='$return_to2';");
      } else {
        $button = new button($cmdClose, "javascript:change_tab(".TASK_LIST_TAB.");");
      }
    } else
      $button = new button($cmdClose, "javascript:document.location='$return_to';");
  }
}
else {
//  $button = new button($cmdClose, "javascript:document.location='$return_to';");
  $button = new button($cmdClose, 'javascript:change_tab('.TASK_LIST_TAB.')');
  echo '</td><td align="right" width="100%">'.
       '<table border="0" cellpadding="1" cellspacing="0" bgcolor="black">'.
	   '<tr><td>'.
	   '<table border="0" cellspacing="5" cellpadding="0" bgcolor="#FFF7EB">'.
	   '<tr><td>';
  echo '<img src="'.$GO_THEME->images['task_wait'].'" width="14" height="7"> '.$pm_legend_planning.'&nbsp;&nbsp;';
  echo '<img src="'.$GO_THEME->images['task_on'].'" width="14" height="7"> '.$pm_legend_ongoing.'&nbsp;&nbsp;';
  echo '<img src="'.$GO_THEME->images['task_done'].'" width="14" height="7"> '.$pm_legend_done.'&nbsp;';
  echo '</td></tr>'.
       '</table>'.
       '</td></tr>'.
	   '</table>';
}
echo '</td></tr></table>';

function show_planned_eslapsed_times() {
  global $tasks, $pstop_date, $pstart_date,
    $pm_delay, $pm_days, $pm_remain, $pm_advance, $pm_predict_delay,
    $pm_legend_planning, $pm_legend_ongoing, $pm_legend_done, $today, $last_task_id;

  $tasks[1]->get_elapsed();
  $pm_legend_current = $pm_legend_ongoing;
  $project_done = $tasks[$last_task_id]->approved;
  if ($project_done) {
    $today = strtotime($tasks[$last_task_id]->approved_date);
    $pm_legend_current = $pm_legend_done;
  }
  $plan_days = round(($pstop_date - $pstart_date) / (24 * 3600));
  $done_days = round(($today - $pstart_date) / (24 * 3600));

  if ($done_days > $plan_days)
    $delay_days = "$pm_delay: ".($done_days - $plan_days)." $pm_days";
  else {
    $d = ($plan_days - $done_days);
    if ($project_done && $d > 0)
      $delay_days = "$pm_advance: ".$d." $pm_days";
    else
      $delay_days = "$pm_remain: ".$d." $pm_days";
  }

  // NOTE: i haven't seen $predict_delay_days defined anywhere,
  // so set it empty to avoid php notices
  $predict_delay_days = '';

  
  if (!$project_done) {
    $real_stop_date = $tasks[$last_task_id]->find_stop_date();
    if ($real_stop_date > $plan_days) {
      $predict_delay_days .= ', &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.
        "$pm_predict_delay: ".($real_stop_date - $plan_days)." $pm_days";
    }
  }

  if ($done_days >= 0)
  {
	  echo '<tr><td colspan="8">'.
    	   "$pm_legend_planning: $plan_days $pm_days,".
	       '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.
    	   "$pm_legend_current: $done_days $pm_days,".
	       '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.
    	   "$delay_days$predict_delay_days</td></tr>";
	  echo '<tr><td colspan="8">&nbsp;</td></tr>';
  }
}

?>

<script type="text/javascript" language="javascript">
var task_tips = null;
var frm = document.projects_form;

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

function show_tips(id)
{
  if (task_tips) return false;

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

function _save_task(action_type)
{
  if (action_type != 'status' || (frm.task_status.value == frm.saved_task_status.value ||
     (frm.task_status.value != <?php echo TASK_DONE; ?> ||
	  confirm("<?php echo smart_addslashes($strStatusChangeConfirm); ?>"))))
  {
    frm.task.value='save_task_'+action_type;
    frm.target = '_self';
    frm.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
    frm.submit();
  }
}

function _set_approve() {
  frm.task.value='save_task_approve';
  frm.target = '_self';
  frm.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
  frm.submit();
}

function _gantt()
{
  frm.task.value='show_task_gantt';
  frm.target = '_self';
  frm.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
  frm.submit();
}

function _reset()
{
  frm.reset();
}
</script>
