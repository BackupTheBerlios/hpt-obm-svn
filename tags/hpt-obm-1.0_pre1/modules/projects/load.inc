<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

$show_period = isset($_GET['show_period']) ? $_GET['show_period'] : 'all';
$load_user_id = isset($_GET['load_user_id']) ? $_GET['load_user_id'] : $GO_SECURITY->user_id;
$load_type = isset($_GET['load_type']) ? $_GET['load_type'] : 'project';
$print = (isset($_GET['print']) && $_GET['print'] == 'true') ? true : false;
$project_id = isset($_GET['project_id']) ? $_GET['project_id'] : '0';

$time = get_time();
$this_day = date("j", $time);
$this_year = date("Y", $time);
$this_month = date("m", $time);

$month = isset($_GET['month']) ? $_GET['month'] : $this_month;
$year = isset($_GET['year']) ? $_GET['year'] : $this_year;

$module_url = isset($projects_module) ? $projects_module['url'] : $GO_MODULES->url;

if (isset($_REQUEST['delete_hours']))
{
  if ($working_hours = $projects->get_working_hours($_REQUEST['delete_hours']))
  {
    if($_project = $projects->get_project($working_hours['project_id']))
    {
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $_project['acl_write']))
      {
	$projects->delete_hours($_REQUEST['delete_hours']);
      }else
      {
	echo '<p class="Error">'.$strAccessDenied.'</p>';
      }
    }
  }
}

if (!$print)
{
  echo '<h3>'.$pm_load_type.':</h3>';
  echo '<table border="0" cellspacing="0" cellpadding="4">';

  if (!isset($fixed_project_id) && $ab_module = $GO_MODULES->get_module('addressbook'))
  {
    if($has_contacts_module = ($ab_module &&
	  ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) ||
	   $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write']))))
    {
    	require_once($ab_module['class_path'].'addressbook.class.inc');
      $load_contact_id = isset($_GET['load_contact_id']) ? $_GET['load_contact_id'] : '0';

      $select = new select('contact', 'projects_form', 'load_contact_id', $load_contact_id);
      echo '<tr><td>';
      $select->print_link($pm_client);
      echo ':</td><td>';
      $select->print_field();
      echo '</td></tr>';
    }
  }?>
  <tr>
    <td>
    <?php
    $load_user_id = isset($_REQUEST['load_user_id']) ? $_REQUEST['load_user_id'] : 0;
  if ($GO_MODULES->write_permissions)
  {
    $select = new select('user', 'projects_form', 'load_user_id', $load_user_id);
    echo '<tr><td>';
    $select->print_link($pm_employee);
    echo ':</td><td>';
    $select->print_field();
    echo '</td></tr>';
  }else
  {
    $checkbox = new checkbox('load_user_id', $GO_SECURITY->user_id, $pm_yours_only, $load_user_id);
  }
  ?>
    </td>
    </tr>
    <?php

    echo '</table>';
  echo '<h3>'.$pm_load_period.':</h3>';
  echo '<table border="0" cellspacing="0" cellpadding="4">';

  switch ($show_period)
  {
    case 'all':
      $all_check='checked';
      $period_check = '';
      $month_check = '';
      break;

    case 'period':
      $all_check='';
      $period_check = 'checked';
      $month_check = '';
      break;

    case 'month':
      $all_check='';
      $period_check = '';
      $month_check = 'checked';
      break;
  }
  echo '<input type="hidden" name="delete_hours" />';
  ?>
    <tr>
    <td height="25" colspan="2">
    <?php
    echo '<input type="radio" name="show_period" value="all" '.$all_check.' />'.$pm_show_all;
  echo '</td></tr><tr><td height="25">';

  echo '<input type="radio" name="show_period" value="period" '.$period_check.' />'.$pm_show_period.'</td><td>';

  $date = date($_SESSION['GO_SESSION']['date_format'], $time);

  $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : $date;
  $datepicker->print_date_picker('start_date', $_SESSION['GO_SESSION']['date_format'], $start_date);

  echo '&nbsp;&nbsp;'.$pm_to.'&nbsp;&nbsp;';

  $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : $date;
  $datepicker->print_date_picker('end_date', $_SESSION['GO_SESSION']['date_format'], $end_date);
  ?>
    </td>
    </tr>
    <tr>
    <td height="25">
    <input type="radio" name="show_period" value="month" <?php echo $month_check; ?> /><?php echo $pm_show_month; ?>
    </td>
    <td>
    <?php
    for ($i=0;$i<10;$i++)
    {
      $num_years[] = $this_year - $i;
    }

  for ($i=1;$i<32;$i++)
  {
    $num_days[] = $i;
  }

  for ($i=1;$i<13;$i++)
  {
    $num_months[] = $i;
  }

  $dropbox = new dropbox();
  $dropbox->add_arrays($num_months, $months);
  $dropbox->print_dropbox("month", $month);

  $dropbox = new dropbox();
  $dropbox->add_arrays($num_years, $num_years);
  $dropbox->print_dropbox("year", $year);
  ?>
    </td>
    </tr>
    <tr>
    <td colspan="2">
    <?php
    $button = new button($cmdOk, "javascript:_load();");
  echo '&nbsp;&nbsp;';
  $button = new button($cmdPrint, "javascript:_print();");
  if (isset($fixed_project_id))
  {
    echo '&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
  }
  ?>
    </td>
    </tr>
    </table>
    <?php
}
if ($task=='load')
{
  $project_id = isset($_GET['project_id']) ? $_GET['project_id'] : 0;
  $load_user_id = isset($_GET['load_user_id']) ? $_GET['load_user_id'] : 0;
  $load_contact_id = isset($_GET['load_contact_id']) ? $_GET['load_contact_id'] : 0;

  switch($show_period)
  {
    case 'all':
      $start_time=0;
      $end_time=0;
      break;

    case 'period':
      $end_time = date_to_unixtime($_GET['end_date']);
      $start_time = date_to_unixtime($_GET['start_date']);
      break;

    case 'month':
      $start_time = mktime(0,0,0,$month, 1, $year);
      $end_time = mktime(0,0,0,$month+1, 1, $year);
      break;
  }

  $projects->get_hours($start_time, $end_time, $project_id, $load_user_id, $load_contact_id);

  echo '<table border="0" cellpadding="4" cellspacing="0">';
  echo '<tr>';
  echo '<td><h3>'.$pm_starttime.'</h3></td>';
  echo '<td><h3>'.$pm_endtime.'</h3></td>';
  if ($load_user_id == 0)
  {
    echo '<td><h3>'.$pm_employee.'</h3></td>';
  }
  if ($project_id == 0)
  {
    echo '<td><h3>'.$pm_project.'</h3></td>';
    if ($load_contact_id == 0)
    {
      echo '<td><h3>'.$pm_client.'</h3></td>';
    }
  }
  echo '<td align="center"><h3>'.$pm_hours.'</h3></td>';
  echo '<td align="left"><h3>'.$pm_fee.'</h3></td>';
  echo '<td align="right"><h3>'.$pm_breaktime.'</h3></td>';
  echo '<td align="right"><h3>'.$pm_unit_value.'</h3></td>';
  echo '<td align="center"><h3>'.$pm_units.'</h3></td>';
  echo '<td></td></tr>';

  $last_date = '';
  $total_days = 0;
  $total_seconds = 0;
  $total_fee = 0;
  $class = 'TableInside1';

  while($projects->next_record())
  {
    $local_start_time = date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $projects->f('start_time')+($_SESSION['GO_SESSION']['timezone']*3600));
    $local_end_time = date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $projects->f('end_time')+($_SESSION['GO_SESSION']['timezone']*3600));

    if ($projects->f('unit_value') > 0)
    {
      $unit_value = $projects->f('unit_value');
      $break_time = '-';
      $units = ($projects->f('end_time') - $projects->f('start_time'))*60/$unit_value;
    }else
    {
      $unit_value = '-';
      $units = '-';
      $break_time = gmdate('G:i', gmmktime(0,0,$projects->f('break_time')));
    }
    $seconds = ($projects->f('end_time') - $projects->f('start_time') - $projects->f('break_time'));

    if ($projects->f('fee_value') > 0)
    {
      $fee_string = htmlentities($_SESSION['GO_SESSION']['currency']).'&nbsp;'.
      	number_format($projects->f('fee_value'), 2, $_SESSION['GO_SESSION']['decimal_seperator'],$_SESSION['GO_SESSION']['thousands_seperator']).
	'&nbsp;/&nbsp;'.$projects->f('fee_time').'&nbsp;'.$pm_mins;
	
      $fee_per_hour = ($projects->f('fee_value')/$projects->f('fee_time'))*60;
      $fee = $fee_per_hour*$seconds/3600;
      $total_fee += $fee;
    }else
    {
      $fee_string = '0';
    }

    $date = date($_SESSION['GO_SESSION']['date_format'], $projects->f('start_time'));
    if ($date != $last_date) $total_days++;
    $last_date = $date;

    $total_seconds += $seconds;
    echo '<tr class="'.$class.'">';
    echo '<td valign="top" nowrap>'.$local_start_time.'</td>';
    echo '<td valign="top" nowrap>'.$local_end_time.'</td>';
    if ($load_user_id == 0)
    {
      echo '<td valign="top" nowrap>'.show_profile($projects->f('user_id')).'</td>';
    }
    if ($project_id == 0)
    {
    	$description = ($projects->f('description') == '') ? '' : ' ('.htmlspecialchars($projects->f('description')).')';
	
      echo '<td valign="top" nowrap><a href="project.php?project_id='.$projects->f('project_id').'" class="normal">'.
      		htmlspecialchars($projects->f('name')).$description.'</a></td>';
		
      if ($load_contact_id == 0)
      {
	echo '<td valign="top" nowrap>'.show_contact($projects->f('contact_id')).'</td>';
      }
    }

    echo '<td align="center" valign="top">'.gmdate('G:i', $seconds).'</td>';
    echo '<td align="left" valign="top">'.$fee_string.'</td>';
    echo '<td align="right" valign="top">'.$break_time.'</td>';
    echo '<td align="right" valign="top">'.$unit_value.'</td>';
    echo '<td align="center" valign="top">'.$units.'</td>';

    echo "<td valign=\"top\">";
    if (!$print)
    {
      echo "<a href='javascript:div_confirm_action(\"javascript:delete_hours(".$projects->f("id").")\",\"".div_confirm_id($strDeletePrefix.' '.$pm_this_entry.' '.$strDeleteSuffix)."\")' title=\"".$strDeleteItem." ".$pm_entry."\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a>";
    }
    echo "</td></tr>\n";

    if ($projects->f('comments') != '')
    {
      echo '<tr class="'.$class.'"><td valign="top" colspan="99">'.text_to_html($projects->f('comments')).'</td></tr>';
    }

    if ($class != 'TableInside2') $class='TableInside2'; else $class='TableInside1';
  }

  echo '</table><br /><h3>'.$pm_summary .'</h3>';
  echo '<table border="0">';

$total_hours = floor($total_seconds/3600);
  
  echo'<tr><td><i>'.$pm_total_hours.':</td><td>'.$total_hours.':'.gmdate('i', $total_seconds).'</i></td></tr>';
  echo'<tr><td><i>'.$pm_total_days.':</td><td>'.$total_days.'</i></td></tr>';
  echo'<tr><td><i>'.$pm_total_fee.':</td><td>'.htmlentities($_SESSION['GO_SESSION']['currency']).'&nbsp;'.number_format($total_fee, 2, $_SESSION['GO_SESSION']['decimal_seperator'],$_SESSION['GO_SESSION']['thousands_seperator']).'</i></td></tr>';
  echo '</table>';
}
?>
<input type="hidden" name="print" />

  <script type="text/javascript" langauge="javascript">
function delete_hours(id)
{
  document.projects_form.task.value='load';
  document.projects_form.delete_hours.value=id;
  document.projects_form.submit();
}

function remove_load_client()
{
  document.projects_form.load_contact_id.value = 0;
  document.projects_form.contact_name.value = '';
  document.projects_form.contact_name_text.value = '';
}

function remove_load_user()
{
  document.projects_form.load_user_id.value = 0;
  document.projects_form.user_name.value = '';
  document.projects_form.user_name_text.value = '';
}

function _print()
{
  document.projects_form.task.value='load';
  document.projects_form.print.value = 'true';
  document.projects_form.target = '_blank';
  document.projects_form.action = '<?php echo $module_url; ?>print.php';
  document.projects_form.submit();
  document.projects_form.task.value='load';
  document.projects_form.print.value = 'false';
  document.projects_form.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
  document.projects_form.target = '_self';

}

function _load()
{
  document.projects_form.task.value='load';
  document.projects_form.print.value = 'false';
  document.projects_form.target = '_self';
  document.projects_form.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
  document.projects_form.submit();
}
</script>
