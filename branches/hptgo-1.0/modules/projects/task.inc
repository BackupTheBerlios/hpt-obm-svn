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

$tasks = load_project_task($projects, $project_id);

if ($task != 'show_task_gantt') {
  $task_status = $_GET['task_status'];
  $save_type = isset($task_status) ? "status" : "list";

  echo "<input type=\"hidden\" name=\"max_task_id\" value=\"$max_task_id\">";
  echo "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
  show_planned_eslapsed_times();
  echo "<tr><td class=\"TableHead2\" width=\"25\" align=\"center\" nowrap>$pm_task_order</td>";
  echo     "<td class=\"TableHead2\" width=\"200\" nowrap>$pm_task_name</td>";
  echo     "<td class=\"TableHead2\" width=\"190\" nowrap>$pm_task_person</td>";
  echo     "<td class=\"TableHead2\" width=\"100\" nowrap>$pm_task_predecessors</td>";
  echo     "<td class=\"TableHead2\" width=\"45\" nowrap>$pm_task_days</td>";
  echo     "<td class=\"TableHead2\" width=\"80\" nowrap>$pm_task_status</td>";
  echo     "<td class=\"TableHead2\" width=\"50\" align=\"center\" nowrap>$pm_task_approved</td>";
  echo     "<td class=\"TableHead2\" width=\"50\" align=\"center\" nowrap>$pm_task_document</td></tr>";

  // Display Task List
  if (!isset($task_status)) {
    foreach ($tasks as $id => $t)
      $t->print_info($member_ids, $member_names, $member_list);
    $stask = $tasks[1];
  }
  // Display Task Status
  else {
    $task_id = isset($_GET['task_id']) ? $_GET['task_id'] : '0';
    $stask = $tasks[$task_id];
    if ($task_status == 'show_doc')
      $stask->show_doc_folder();
    else
      $stask->print_status($member_list);
  }
}
else {
  echo "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
  show_planned_eslapsed_times();
  echo "<tr><td class=\"TableHead2\" width=\"25\" align=\"center\" nowrap>$pm_task_order</td>";
  echo "<td class=\"TableHead2\" width=\"715\" align=\"center\" colspan=\"2\">$cmdGantt</td></tr>";

  $date = strftime("%d/%m", $pstart_date);
  echo "<tr class=\"TableInside2\"><td width=\"25\" align=\"center\" nowrap>&nbsp;</td>";
  echo "<td width=\"150\">&nbsp;</font></td>";
  echo "<td width=\"565\" class=\"GanttDate\" nowrap><b>$pm_start_date ($date)<br>v</b></td></tr>";

  $today = getdate();
  $today = gmmktime(0, 0, 0, $today['mon'], $today['mday'], $today['year']);
  $duration = round((($today < $pstop_date ? $pstop_date : $today) - $pstart_date) / (24 * 3600));
  if ($duration == 0) $duration = 1;
  $day_space = round(480 / $duration);

  $n = 1;
  foreach ($tasks as $id => $t) {
    list($start_date, $real_start_date) = $t->find_start_date();
    $display_date = $start_date * $day_space;
    $display_rdate = $real_start_date * $day_space;
    $ndays = $t->days * $day_space;
    $date = strftime("%d/%m", $pstart_date + ($start_date + $t->days) * 24 * 3600);
    if ($t->status == TASK_ONGOING || ($t->status == TASK_DONE && !$t->approved)) {
      $d = round(($today - $pstart_date) / (24 * 3600)) - $real_start_date;
      $odate = strftime("%d/%m", $pstart_date + ($real_start_date + $d) * 24 * 3600);
      $cwidth = $d * $day_space;
      if (!$cwidth) $cwidth = 2;
      $current = "<img src=\"".$GO_THEME->images['task_on']."\" width=\"".$cwidth."\" height=\"7\">&nbsp;<font color=\"green\">$odate</font>";
      $cspace = $real_start_date * $day_space;
      $current_space = "<img src=\"".$GO_THEME->images['task_space']."\" width=\"$cspace\" height=\"7\">";
    }
    elseif ($t->approved) {
      $d = (round((strtotime($t->approved_date) - $pstart_date) / (24 * 3600)) - $real_start_date);
      $adate = strftime("%d/%m", $pstart_date + ($real_start_date + $d) * 24 * 3600);
      $cwidth = $d * $day_space;
      if (!$cwidth) $cwidth = 2;
      $current = "<img src=\"".$GO_THEME->images['task_done']."\" width=\"".$cwidth."\" height=\"7\">&nbsp;<font color=\"red\">$adate</font>";
      $cspace = $real_start_date * $day_space;
      $current_space = "<img src=\"".$GO_THEME->images['task_space']."\" width=\"$cspace\" height=\"7\">";
    }
    else {
      $current = "&nbsp;";
      $current_space = "";
    }

    if ($ndays == 0) $ndays = 2;
    $space = "<img src=\"".$GO_THEME->images['task_space']."\" width=\"$display_date\" height=\"7\">";
    $image = "<img src=\"".$GO_THEME->images['task_wait']."\" width=\"$ndays\" height=\"7\">";
    echo "<tr class=\"TableInside$n\"><td width=\"25\" align=\"center\" nowrap>".$t->id."</td>";
    echo "<td width=\"150\"><font size=\"0\">".$t->name."</font></td>";
    echo "<td width=\"565\" class=\"GanttDate\" nowrap>$space$image&nbsp;<font color=\"blue\"><i>$date<br>$current_space$current</i></font></td></tr>";
    $n = $n == 1 ? 2 : 1;
  }

  $date = strftime("%d/%m", $pstop_date);
  $ndays = round(($pstop_date - $pstart_date) / (24 * 3600));
  $display_date = $ndays * $day_space;
  $space = "<img src=\"".$GO_THEME->images['task_space']."\" width=\"".$display_date."\" height=\"7\">";
  echo "<tr class=\"TableInside$n\"><td width=\"25\">&nbsp;</td><td width=\"150\">&nbsp;</td>";
  echo "<td class=\"GanttDate\" nowrap>$space<b>^<br>$space$pm_end_date ($date)</b></td></tr>";

  $return_to = $_SERVER['PHP_SELF'] . "?project_id=$project_id&task=show_task_list";
}

echo "</table><table border=\"0\" cellspacing=\"0\" cellpadding=\"4\">";
echo "<tr><td colspan=\"2\">&nbsp;</td></tr><tr><td>";

if ($task != 'show_task_gantt') {
  if ($stask->project_status!=STATUS_DROP) {
    if ($task_status != 'show_doc') {
      if ((isset($task_status) && !$stask->approved) ||
         ($stask->is_manager && (!isset($task_status) || ($task_status != TASK_DONE && !$stask->approved))))
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

$_SESSION['GO_FILESYSTEM_PATH'] = $GO_CONFIG->file_storage_path.'users/'.$_SESSION['GO_SESSION']['username'];
if ($task != 'show_task_gantt')
{
  if (!$is_new_project)
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
}
else {
  $button = new button($cmdClose, "javascript:change_tab(".TASK_LIST_TAB.")");
  echo "</td><td align=\"right\" width=\"500\">";
  echo "<img src=\"".$GO_THEME->images['task_wait']."\" width=\"14\" height=\"7\"> $pm_legend_planning&nbsp;&nbsp;";
  echo "<img src=\"".$GO_THEME->images['task_on']."\" width=\"14\" height=\"7\"> $pm_legend_ongoing&nbsp;&nbsp;";
  echo "<img src=\"".$GO_THEME->images['task_done']."\" width=\"14\" height=\"7\"> $pm_legend_done&nbsp;";
}
echo "</td></tr></table>";

function show_planned_eslapsed_times() {
  global $tasks, $pstop_date, $pstart_date, $pm_delay, $pm_days, $pm_remain,
  		 $pm_legend_planning, $pm_legend_ongoing, $today;

  $tasks[1]->get_elapsed();
  $plan_days = round(($pstop_date - $pstart_date) / (24 * 3600));
  $done_days = round(($today - $pstart_date) / (24 * 3600));
  if ($done_days > $plan_days)
	$delay_days = ",&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$pm_delay: ".($done_days - $plan_days)." $pm_days";
  else
	$delay_days = ",&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$pm_remain: ".($plan_days - $done_days)." $pm_days";

  echo "<tr><td colspan=\"8\">$pm_legend_planning: $plan_days $pm_days,".
       "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;$pm_legend_ongoing: $done_days $pm_days$delay_days</td></tr>";
  echo "<tr><td colspan=\"8\">&nbsp;</td></tr>";
}

?>

<script type="text/javascript" language="javascript">
function _save_task(action_type)
{
  if (action_type != 'status' ||
    (document.projects_form.task_status.value == document.projects_form.saved_task_status.value ||
      (document.projects_form.task_status.value != <?php echo TASK_DONE; ?> || confirm("<?php echo smart_addslashes($strStatusChangeConfirm); ?>"))))
  {
    document.projects_form.task.value='save_task_'+action_type;
    document.projects_form.target = '_self';
    document.projects_form.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
    document.projects_form.submit();
  }
}

function _set_approve() {
  document.projects_form.task.value='save_task_approve';
  document.projects_form.target = '_self';
  document.projects_form.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
  document.projects_form.submit();
}

function _gantt()
{
  document.projects_form.task.value='show_task_gantt';
  document.projects_form.target = '_self';
  document.projects_form.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
  document.projects_form.submit();
}

function _reset()
{
  document.projects_form.reset();
}
</script>