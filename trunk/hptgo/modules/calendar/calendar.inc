<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || 
    !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || 
      $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
  $ab_module = false;
}else
{
  require_once($ab_module['path'].'classes/addressbook.class.inc');
  $ab = new addressbook();
}

//show_days is the number of days to show (defined in index.php)
//max_columns (defined in index.php)
if ($cal_settings['show_days'] == 0)
{
  $_REQUEST['show_days'] = 7;
}
if (isset($_REQUEST['show_days']) && ($_REQUEST['show_days'] != $cal_settings['show_days']))
{
  $cal->set_show_days($GO_SECURITY->user_id, $_REQUEST['show_days']);
  $cal_settings['show_days'] = $_REQUEST['show_days'];  
}
if ($cal_settings['show_days'] > 29)
{
  $day = 1;
}
//else if ($cal_settings['show_days'] > 4)
else if ($cal_settings['show_days'] >= 7)
{
  //calculate the first day to display in the week view.
  $weekday = date("w", mktime(0,0,0,$month, $day, $year));
  $tmpday = $day - $weekday + $_SESSION['GO_SESSION']['first_weekday'];
  if ($tmpday > $day)
  {
    $tmpday = $tmpday -7;
  }
  $new_time = mktime(0,0,0,$month, $tmpday, $year);
  $day = date("j", $new_time);
  $month = date('m', $new_time);
  $year = date('Y', $new_time);
}

// 	Quoc sua cach tinh NgayBatDau va NgayKetThuc
$interval_start_time = mktime(0,0,0,$month, $day-1, $year); // old calculation
$interval_end_time = mktime(0,0,0,$month, $day+$cal_settings['show_days']+1, $year);

$interval_start_time2 = mktime(0,0,0,$month, $day, $year);

//$interval_end_time = mktime(0,0,0,$month, $day+$cal_settings['show_days']+1, $year);
if ($cal_settings['show_days'] > 29){ // Hien thi mot thang
	// Ngay cuoi thang = ngay dau thang sau - 1
	if ($month == 12)
		$interval_end_time2 = mktime(0,0,0,12, 31, $year);
	else{
		$interval_end_time2 = mktime(0,0,0,$month+1, 0, $year);
	}	
}else if ($cal_settings['show_days'] >= 2){ // hien thi theo nhieu ngay
	$interval_end_time2 = mktime(0,0,0,$month, $day+$cal_settings['show_days']-1, $year);
}
else{ // hien thi mot ngay
	$interval_end_time2 = $interval_start_time2;
}

if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
{
	$dst_offset = $_SESSION['GO_SESSION']['DST'];
}else
{
	$dst_offset = 0;
}
	  
$timezone_offset = $_SESSION['GO_SESSION']['timezone']+$dst_offset;

//Chinh dong LICH va XEM
/*
if(!$print)
{
  echo '<table border="0" cellpadding="0" cellspacing="0"><tr height="28"><td><h3>';
  echo $sc_view.':&nbsp;</h3></td><td>';
  $dropbox = new dropbox();
  $dropbox->add_value('1', '1 '.$sc_day);
  $dropbox->add_value('2', '2 '.$sc_days);
  $dropbox->add_value('5', '5 '.$sc_days);
  $dropbox->add_value('7', '1 '.$sc_week);
  $dropbox->add_value('14', '2 '.$sc_weeks);
  $dropbox->add_value('21', '3 '.$sc_weeks);
  $dropbox->add_value('35', '1 '.$sc_month);
  $dropbox->print_dropbox("show_days", $cal_settings['show_days'],
      'onchange="javascript:document.forms[0].submit()"');
  if ($cal_settings['show_days'] == 5)
  {
    $interval= 7;
  }else
  {
    $interval = $cal_settings['show_days'];
  }

  echo '</td><td><h3><a href="javascript:goto_date('.($day-$interval).', '.$month.', '.$year.');">&lt;&lt; </a>&nbsp;';
  echo date($_SESSION['GO_SESSION']['date_format'], $interval_start_time2+
      ($timezone_offset*3600)).' - '.
      date($_SESSION['GO_SESSION']['date_format'], $interval_end_time2+
      ($timezone_offset*3600));
  echo '&nbsp;<a href="javascript:goto_date('.($day+$interval).', '.$month.', '.$year.');">&gt;&gt;</a>';
  echo '</h3></td></tr></table>';
}
*/

$weeknumber = date("W", mktime(0,0,0,$month,$day,$year));
echo '<table id="cal" width="100%" height="100%">';
echo '<tr><td colspan="8" class="cal_title">'.$sc_week.' '.$weeknumber.'</td></tr>';
$rows = $cal_settings['show_days']/$max_columns;


$view_id = isset($view_id) ? $view_id : 0;
$merged_view = isset($merged_view) ? $merged_view : true;

if($merged_view || $view_id == 0)
{
  require('calendar_view.inc');
}else
{
  $view_id = 0;
  for($cal_index = 0; $cal_index<count($calendars); $cal_index++)
  {
    $calendar=$calendars[$cal_index];
    $calendar_id = $calendar["id"];
  
    if(count($calendars) > 1)
    {
      echo '<tr><td height="100%" colspan="8" style="padding:3px;font-weight:bold;">'.$calendar["name"].'</td></tr>';
    }
 
    require('calendar_view.inc');
  }
}

echo '</table>';
?>
<script type="text/javascript">

function glow_row(hour)
{
  table_row = get_object(hour);
  table_row.style.backgroundColor = '#FFFFCC';
}

function unglow_row(hour)
{
  table_row = get_object(hour);
  table_row.style.backgroundColor = '';
}
</script>
