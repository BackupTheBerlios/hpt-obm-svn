<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require_once($GO_MODULES->path.'classes/pdf.class.inc');

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

if ($cal_settings['show_days'] > 4)
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

if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
{
	$dst_offset = $_SESSION['GO_SESSION']['DST'];
}else
{
	$dst_offset = 0;
}

$interval_start_time = mktime(0,0,0,$month, $day-1, $year);
$interval_end_time = mktime(0,0,0,$month, $day+$cal_settings['show_days']+1, $year);
$timezone_offset = $_SESSION['GO_SESSION']['timezone']+$dst_offset;
$view_id = isset($view_id) ? $view_id : 0;

$weeknumber = date("W", mktime(0,0,0,$month,$day,$year));
$rows = $cal_settings['show_days']/$max_columns;
$merged_view = isset($merged_view) ? $merged_view : true;

$get_view = false;
if($view_id == 0)
{
  $calendars[] = $calendar;
}else
{
  if($merged_view)
  {
    $get_view = true;
	$calendar_id = 0;
  }else
  {
    $calendars = $cal->get_view_calendars($view_id);
	$view_id = 0;
  }
}

$week_title = $sc_week.' '.$weeknumber;

$pdf = new pdf();
$pdf->AddFont('Arial','','arial.php');
$pdf->AddFont('Arial','B','arialbd.php');
$pdf->Open();


if($get_view)
{
  $title = $sc_view.": ".$title;
  
  $pdf->AddPage();
  require('print_view.inc');
}else
{
  for($cal_index = 0; $cal_index<count($calendars); $cal_index++)
  {
	$calendar=$calendars[$cal_index];
        $calendar_id = $calendar["id"];
	$title = $sc_calendar.": ".$calendar["name"];

	// test here

	$pdf->AddPage();
    require('print_view.inc');
  }
}
$pdf->auto_print(true);
$pdf->Output();
?>
