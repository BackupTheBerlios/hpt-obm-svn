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

require("../../Group-Office.php");
$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('calendar');
require($GO_LANGUAGE->get_language_file('calendar'));

$max_columns = 7;
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$merged_view = isset($_REQUEST['merged_view']) ? $_REQUEST['merged_view'] : true;
//are we in print view?
$print = isset($_REQUEST['print']) ? true : false;	

require_once($GO_MODULES->class_path.'calendar.class.inc');
require_once($GO_MODULES->class_path.'todos.class.inc');
$cal = new calendar();

$cal_settings = $cal->get_settings($GO_SECURITY->user_id);

//get the local times
$local_time = get_time();
$year = isset($_REQUEST['year']) ? $_REQUEST['year'] : date("Y", $local_time);
$month = isset($_REQUEST['month']) ? $_REQUEST['month'] : date("m", $local_time);
$day = isset($_REQUEST['day']) ? $_REQUEST['day'] : date("j", $local_time);
$hour = isset($_REQUEST['hour']) ? $_REQUEST['hour'] : date("H", $local_time);
$min = isset($_REQUEST['min']) ? $_REQUEST['min'] : date("i", $local_time);
$local_browse_time = mktime($hour, $min, 0, $month, $day, $year);

//recalculate date
$year = date("Y", $local_browse_time);
$month = date("m", $local_browse_time);
$day = date("j", $local_browse_time);
$hour = date("H", $local_browse_time);
$min = date("i", $local_browse_time);

//get the current date properties
$current_year = date("Y", $local_time);
$current_month = date("m", $local_time);
$current_day = date("j", $local_time);
$current_hour = date("H", $local_time);
$current_min = date("i", $local_time);
$current_date = date(DB_DATE_FORMAT, $local_time);

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = isset($_POST['return_to']) ? $_POST['return_to'] : '';
$show_days = isset($_REQUEST['show_days']) ? $_REQUEST['show_days'] : '';

if(isset($_REQUEST['calendar_view_id']))
{
	$tmp = explode(':', $_REQUEST['calendar_view_id']);
	switch($tmp[0])
	{
		case 'calendar':
			$calendar_id = $tmp[1];	
			$view_id=0;	
		break;
		
		case 'view':
			$view_id = $tmp[1];
			$calendar_id = 0;	
		break;
	}
}else
{
	$view_id = $cal_settings['default_view_id'];
	$calendar_id = $cal_settings['default_cal_id'];
}

//if a view is given then display view. Otherwise open a calendar
if($view_id > 0)
{
  $view = $cal->get_view($view_id);
  if ($view)
  {	
    $title = $view['name'];
    $calendar_id = 0;
    $cal_start_hour = $view['start_hour'];
    $cal_end_hour = $view['end_hour'];  
  }
}
if(!isset($view) || !$view)
{
  //get the calendar properties and check permissions
  if ($calendar_id != 0 && $calendar = $cal->get_calendar($calendar_id))
  {
	$calendar['read_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_read']);
	$calendar['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_write']);
	if (!$calendar['read_permission'] && !$calendar['write_permission'] )
	{

	  //hmm no calendar_id given and default calendar is set
	  //Does this user even have calendars?
	  $cal->get_user_calendars($GO_SECURITY->user_id);
	  if ($cal->next_record()) {
	    //yes he does so set it default
	    $calendar_id = $cal->f('id');
	  } else {
	    $calendar_name = $_SESSION['GO_SESSION']['name'];
	    $new_calendar_name = $calendar_name;
	    $x = 1;
	    while($cal->get_calendar_by_name($new_calendar_name)) {
	      $new_calendar_name = $calendar_name.' ('.$x.')';
	      $x++;
	    }
	    if (!$calendar_id = $cal->add_calendar($GO_SECURITY->user_id, addslashes($new_calendar_name), 7, 20)) {
	      $feedback = '<p class="Error">'.$strSaveError.'</p>';
	    }
	  }

	  $calendar = $cal->get_calendar($calendar_id);
	  if ($calendar) {
	    $calendar['read_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_read']);
	    $calendar['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_write']);
	  }
	  if (!$calendar['read_permission'] && !$calendar['write_permission'] ) {
	    header('Location: '.$GO_CONFIG->host.'error_docs/401.php');
	    exit();
	  }
	}
	$title = $calendar['name'];
	$cal_start_hour = $calendar['start_hour'];
	$cal_end_hour = $calendar['end_hour'];
  }else
  {
    //hmm no calendar_id given and no default calendar is set
    //Does this user even have calendars?
    $cal->get_user_calendars($GO_SECURITY->user_id);
    if ($cal->next_record())
    {
      //yes he does so set it default
      $calendar_id = $cal->f('id');
    }else
		{
		 		$calendar_name =$_SESSION['GO_SESSION']['name'];
	      $new_calendar_name = $calendar_name;
	      $x = 1;
	      while($cal->get_calendar_by_name($new_calendar_name))
	      {
					$new_calendar_name = $calendar_name.' ('.$x.')';
	        $x++;
	      }
	      if (!$calendar_id = $cal->add_calendar($GO_SECURITY->user_id, addslashes($new_calendar_name), 7, 20))
	      {
					$feedback = '<p class="Error">'.$strSaveError.'</p>';
	      }
		}
	
	if($calendar_id == 0 || !$calendar = $cal->get_calendar($calendar_id))
	{
	  die($strDataError);
	}else
	{
		
		
	  $calendar['read_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_read']);
	  $calendar['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_write']);
	  if (!$calendar['read_permission'] && !$calendar['write_permission'] )
	  {
		header('Location: '.$GO_CONFIG->host.'error_docs/401.php');
		exit();
	  }
	  $title = $calendar['name'];
	  $cal_start_hour = $calendar['start_hour'];
	  $cal_end_hour = $calendar['end_hour'];
	}
  }
}

if($calendar_id > 0)
{
	$cal->set_default_view($GO_SECURITY->user_id, $calendar_id, 0);
	$calendar_view_id = 'calendar:'.$calendar_id;
}else
{
	$cal->set_default_view($GO_SECURITY->user_id, 0, $view_id);
	$calendar_view_id = 'view:'.$view_id;
}

$overlib = new overlib();
$GO_HEADER['head'] = $overlib->get_header();

if (!$print) {
  $datepicker = new date_picker();
  $GO_HEADER['head'] .= $datepicker->get_header();

  $GO_HEADER['auto_refresh']['interval'] = '300';
  $GO_HEADER['auto_refresh']['url'] = $GO_MODULES->full_url;

  require($GO_THEME->theme_path."header.inc");
  ?>
    <script type="text/javascript">
    function date_picker(calendar) {
      // Beware that this function is called even if the end-user only
      // changed the month/year.  In order to determine if a date was
      // clicked you can use the dateClicked property of the calendar:
      if (calendar.dateClicked) {
	// OK, a date was clicked, redirect to /yyyy/mm/dd/index.php
	var y = calendar.date.getFullYear();
	var m = calendar.date.getMonth()+1;     // integer, 0..11
	var d = calendar.date.getDate();      // integer, 1..31
	// redirect...
	window.location = "index.php?calendar_id=<?php echo $calendar_id; ?>&view_id=<?php echo $view_id; ?>&day="+d+"&month="+m+"&year="+y+"&return_to=<?php echo rawurlencode($return_to); ?>";
      }
    };

  function goto_date(day, month, year)
  {
    document.forms[0].method='get';
    document.forms[0].day.value = day;
    document.forms[0].month.value = month;
    document.forms[0].year.value = year;
    document.forms[0].submit();
  }

  function change_calendar()
  {
    document.forms[0].method='get';
    document.forms[0].submit();
  }
  </script>
  	
    <table border="0" cellpadding="0" cellspacing="0">
    <tr>
    <td class="ModuleIcons"><a href="event.php?return_to=<?php echo rawurlencode($link_back);?><?php if (strpos($calendar_view_id,"view:") === 0 && $merged_view) echo '&merged_view='.substr($calendar_view_id,strlen("view:"));?>" class="small"><img src="<?php echo $GO_THEME->images['cal_compose']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_new_app; ?></a></td>
    <td class="ModuleIcons">
		<a href="<?php echo $_SERVER['PHP_SELF']; ?>?show_days=1&calendar_id=<?php echo $calendar_id; ?>&view_id=<?php echo $view_id; ?>&day=<?php echo $day; ?>&month=<?php echo $month; ?>&year=<?php echo $year; ?>&merged_view=<?php echo $merged_view;?>" 
		class=
			<?php if ((!isset($found) && ($show_days == 1)) ||
					  (($cal_settings['show_days'] == 1) && (strpos($link_back, "?") === false))){
				echo '"bold_small" ';
				$found=true;
			}else{
				echo '"small" ';
			} ?>
		>
		<img src="<?php echo $GO_THEME->images['cal_day']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_day_view; ?></a></td>
		
    <td class="ModuleIcons">
		<a href="<?php echo $_SERVER['PHP_SELF']; ?>?show_days=7&calendar_id=<?php echo $calendar_id; ?>&view_id=<?php echo $view_id; ?>&day=<?php echo $day; ?>&month=<?php echo $month; ?>&year=<?php echo $year; ?>&merged_view=<?php echo $merged_view;?>" 
		class=
			<?php if ((!isset($found) && ($show_days == 7))||
					  (($cal_settings['show_days'] == 7) && (strpos($link_back, "?") === false))){
				echo '"bold_small" ';
				$found=true;
			}else{
				echo '"small" ';
			} ?>
		>
		<img src="<?php echo $GO_THEME->images['cal_week']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_week_view; ?></a></td>
    <td class="ModuleIcons">
		<a href="<?php echo $_SERVER['PHP_SELF']; ?>?show_days=35&calendar_id=<?php echo $calendar_id; ?>&view_id=<?php echo $view_id; ?>&day=<?php echo $day; ?>&month=<?php echo $month; ?>&year=<?php echo $year; ?>&merged_view=<?php echo $merged_view;?>" 
		class=
			<?php if ((!isset($found) && ($show_days == 35))||
					  (($cal_settings['show_days'] == 35) && (strpos($link_back, "?") === false))){
				echo '"bold_small" ';
				$found=true;
			}else{
				echo '"small" ';
			} ?>			
		>
		<img src="<?php echo $GO_THEME->images['cal_month']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_month_view; ?></a></td>
    <td class="ModuleIcons">
		<a href="<?php echo $_SERVER['PHP_SELF']; ?>?calendar_id=<?php echo $calendar_id; ?>&view_id=<?php echo $view_id; ?>&day=<?php echo $day; ?>&month=<?php echo $month; ?>&year=<?php echo $year; ?>&task=list_view&merged_view=<?php echo $merged_view;?>" 
		class=
			<?php if (!isset($found) && ($task == 'list_view') && ($show_days == '')){
				echo '"bold_small" ';
				$found=true;
			}else{
				echo '"small" ';
			} ?>
		>
		<img src="<?php echo $GO_THEME->images['cal_list']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_list_view; ?></a></td>
    <td class="ModuleIcons"><a href="views.php?return_to=<?php echo rawurlencode($link_back); ?>" class="small"><img src="<?php echo $GO_THEME->images['cal_view']; ?>" border="0" width="32" height="32" /><br /><?php echo $cal_views; ?></a></td>
    <td class="ModuleIcons"><a href="calendars.php?return_to=<?php echo rawurlencode($link_back); ?>" class="small"><img src="<?php echo $GO_THEME->images['cal_calendar']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_calendars; ?></a></td>
    <td class="ModuleIcons"><a href="<?php echo $_SERVER['REQUEST_URI']; ?>" class="small"><img src="<?php echo $GO_THEME->images['cal_refresh']; ?>" border="0" width="32" height="32" /><br /><?php echo $sc_refresh; ?></a></td>
    <td class="ModuleIcons"><a href="javascript:popup('<?php echo $GO_MODULES->url; ?>index.php?print=true&calendar_id=<?php echo $calendar_id; ?>&view_id=<?php echo $view_id; ?>&merged_view=<?php echo $merged_view; ?>&day=<?php echo $day; ?>&month=<?php echo $month; ?>&year=<?php echo $year; ?>','','')" class="small"><img src="<?php echo $GO_THEME->images['cal_print']; ?>" border="0" width="32" height="32" /><br /><?php echo $cmdPrint; ?></a></td>
    </tr>
    </table>

    <form name="calendar_form" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
    <input type="hidden" name="task" value="<?php echo $task; ?>" />
    <input type="hidden" name="year" value="<?php echo $year; ?>" />
    <input type="hidden" name="month" value="<?php echo $month; ?>" />
    <input type="hidden" name="day" value="<?php echo $day; ?>" />
    <input type="hidden" name="link_back" value="<?php echo $link_back; ?>" />
    <input type="hidden" name="calendar_id" value="<?php echo $calendar_id; ?>" />
    <input type="hidden" name="view_id" value="<?php echo $view_id; ?>" />
    <table width="1" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td>
    <?php
//Chinh dong LICH va XEM		
    if ($cal->get_authorised_calendars($GO_SECURITY->user_id))
    {
	  $db = new db();
	  $db->query('SELECT calendar_id FROM cal_config WHERE user_id="'.$GO_SECURITY->user_id.'"');		
	  while ($db->next_record())
	  	$subscribed[] = $db->f('calendar_id');
		
      echo '<table border="0"><tr><td><h3>'.$sc_calendar.':</h3></td><td>';
      $dropbox = new dropbox();
      $dropbox->add_optgroup($sc_calendars);
      #$dropbox->add_sql_data("cal","id","name");
      while($cal->next_record())
      {
      	if (!isset($subscribed))
			$dropbox->add_value('calendar:'.$cal->f('id'), $cal->f('name'));
		else
			if (in_array($cal->f('id'),$subscribed))
				$dropbox->add_value('calendar:'.$cal->f('id'), ($cal->f('name')));
      }
      
    }
    
    if($cal->get_authorised_views($GO_SECURITY->user_id))
    {
      $db = new db();
      $db->query('SELECT view_id FROM cal_view_subscriptions WHERE user_id="'.$GO_SECURITY->user_id.'"');		
      while ($db->next_record())
	$views_subscribed[] = $db->f('view_id');

      $dropbox->add_optgroup($cal_views);
      #$dropbox->add_value('','----- '.$cal_views.' -----');
      while($cal->next_record())
      {
	if (!isset($views_subscribed))
	  $dropbox->add_value('view:'.$cal->f('id'), $cal->f('name'));
	else
	  if (in_array($cal->f('id'),$views_subscribed))
	    $dropbox->add_value('view:'.$cal->f('id'), $cal->f('name'));
      }		    
    }
    $dropbox->print_dropbox("calendar_view_id", $calendar_view_id, 'onchange="javascript:change_calendar()"');
    echo '</td></tr>';
			
    if(isset($view) && $view)
    {
//      echo '<tr><td><h3>'.$title.'</h3></td><td>';
      echo '<tr><td><h3></h3></td><td>';	  
	  $calendars = $cal->get_view_calendars($view_id);
	  if(count($calendars) > 1)
	  {
        $dropbox = new dropbox();
        $dropbox->add_value(true, $cal_view_merged);
        $dropbox->add_value(false, $cal_view_emerged);
        $dropbox->print_dropbox("merged_view", $merged_view, 'onchange="javascript:document.forms[0].submit()"');
	  }
      echo '</td></tr>';
    }
    echo '</table>';
    ?>    
		</td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
        <td valign="bottom">
<?php		
//Chinh dong LICH va XEM	

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

if(!$print)
{
  echo '<table border="0" valign="bottom" cellpadding="0" cellspacing="0"><tr height="28"><td><h3>';
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

  echo '</td><td nowrap><h3><a href="javascript:goto_date('.($day-$interval).', '.$month.', '.$year.');">&lt;&lt; </a>&nbsp;';
  echo date($_SESSION['GO_SESSION']['date_format'], $interval_start_time2+
      ($timezone_offset*3600)).' - '.
      date($_SESSION['GO_SESSION']['date_format'], $interval_end_time2+
      ($timezone_offset*3600));
  echo '&nbsp;<a href="javascript:goto_date('.($day+$interval).', '.$month.', '.$year.');">&gt;&gt;</a>';
  echo '</h3></td></tr></table>';
}
?>		
		</td>
      </tr>
    </table>

    <table border="0" cellpadding="0" cellspacing="0" width="100%">
    <tr>
    <td valign="top">
    <table border="0" cellpadding="0" cellspacing="0" style="margin-right: 10px;">

<!--    
//Chinh dong LICH va XEM	

<tr height="28">
    <td>
    <?php
/*    if ($cal->get_authorised_calendars($GO_SECURITY->user_id))
    {
      echo '<table border="0"><tr><td><h3>'.$sc_calendar.':</h3></td><td>';
      $dropbox = new dropbox();
      $dropbox->add_optgroup($sc_calendars);
      #$dropbox->add_sql_data("cal","id","name");
      while($cal->next_record())
      {
      	$dropbox->add_value('calendar:'.$cal->f('id'), cut_string($cal->f('name'),20));
      }
      
      if($cal->get_views($GO_SECURITY->user_id))
      {
	      $dropbox->add_optgroup($cal_views);
	      #$dropbox->add_value('','----- '.$cal_views.' -----');
	      while($cal->next_record())
	      {
	      	$dropbox->add_value('view:'.$cal->f('id'), cut_string($cal->f('name'),20));
	      }		    
	    }
      $dropbox->print_dropbox("calendar_view_id", $calendar_view_id, 'onchange="javascript:change_calendar()"');
      echo '</td></tr>';
			
    }
    
    if(isset($view) && $view)
    {
      echo '<tr><td><h3>'.$title.'</h3></td><td>';
	  $calendars = $cal->get_view_calendars($view_id);
	  if(count($calendars) > 1)
	  {
        $dropbox = new dropbox();
        $dropbox->add_value(true, $cal_view_merged);
        $dropbox->add_value(false, $cal_view_emerged);
        $dropbox->print_dropbox("merged_view", $merged_view, 'onchange="javascript:document.forms[0].submit()"');
	  }
      echo '</td></tr>';
    }
    echo '</table>';*/
    ?>
    </td>
    </tr>-->

    <tr>
    <td class="cal_todos">
	<div id="date_picker1_container"></div><br>
    <?php

    $_REQUEST['max_rows'] = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : 5;
    require('todos.inc');
    ?>
    </td>
    </tr>
    </table>
    </td>
    <td valign="top" width="100%">
    <?php
}

require_once($GO_MODULES->path.'classes/cal_holidays.class.inc');
$holidays = new holidays();

if ($task == 'list_view')
{
  require('list_view.inc');
}elseif($print)
{
  require('print.inc');
}else
{
  require('calendar.inc');
}

if (!$print)
{
  ?>
    </td>
    </tr>
    </table>
    </form>
    <?php
    $next_month = $month+1;
  $datepicker->print_date_picker('date_picker1', '',$month.'/'.$day.'/'.$year, 'date_picker1_container', 'date_picker');
  //$datepicker->print_date_picker('date_picker2', '',$next_month.'/'.$day.'/'.$year, 'date_picker2_container', 'date_picker');
  require($GO_THEME->theme_path."footer.inc");
}
?>
