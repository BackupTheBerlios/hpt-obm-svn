<?php
$calendar_module_url = isset($calendar_module_url) ? $calendar_module_url : $GO_MODULES->url;

if(!isset($cal_settings))
{
  require_once($calendar_module['class_path'].'calendar.class.inc');
  $cal = new calendar();
  $cal_settings = $cal->get_settings($GO_SECURITY->user_id);
}

if ($cal_settings['show_days'] == 0)
{
  $_REQUEST['show_days'] = 7;
}
if (isset($_REQUEST['show_days']) && ($_REQUEST['show_days'] != $cal_settings['show_days']))
{
  $cal->set_show_days($GO_SECURITY->user_id, $_REQUEST['show_days']);
  $cal_settings['show_days'] = $_REQUEST['show_days'];  
}

//show_days is the number of days to show (defined in index.php)
//max_columns (defined in index.php)
//day_start is the first day of the week (defined in index.php)

$link_back = isset($link_back) ? $link_back : $_SERVER['REQUEST_URI'];

if ($cal_settings['show_days'] > 29)
{
  //display month
  $interval_start_time = mktime(0,0,0,$month, 1, $year)-($_SESSION['GO_SESSION']['timezone']*3600);
  $interval_end_time = mktime(0,0,0,$month+1, 0, $year)-($_SESSION['GO_SESSION']['timezone']*3600);
}else
{
  if ($cal_settings['show_days'] > 4)
  {
    //calculate the first day to display in the week view.
    $weekday = date("w", $local_browse_time);
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

  $interval_start_time = mktime(0,0,0,$month, $day, $year)-($_SESSION['GO_SESSION']['timezone']*3600);
  $interval_end_time = mktime(0,0,0,$month, $day+$cal_settings['show_days'], $year)-($_SESSION['GO_SESSION']['timezone']*3600)-1;
}

$contact_id = isset($contact_id) ? $contact_id : 0;
$view_id = isset($view_id) ? $view_id : 0;

echo '<input type="hidden" name="contact_id" value="'.$contact_id.'" />';
$events = $cal->get_events_in_array($calendar_id, $view_id, 0, $contact_id, $interval_start_time, $interval_end_time, $day, $month, $year, $_SESSION['GO_SESSION']['timezone'], 0, 'Ymd', false);

$events = array_merge($events['all_day'], $events['timed']);
krsort($events);

echo '<table border="0"><tr><td><h3>';
echo $sc_view.':&nbsp;</h3></td><td>';
$dropbox = new dropbox();
$dropbox->add_value('1', '1 '.$sc_day);
$dropbox->add_value('2', '2 '.$sc_days);
$dropbox->add_value('5', '5 '.$sc_days);
$dropbox->add_value('7', '1 '.$sc_week);
$dropbox->add_value('14', '2 '.$sc_weeks);
$dropbox->add_value('21', '3 '.$sc_weeks);
$dropbox->add_value('35', '1 '.$sc_month);
$dropbox->print_dropbox("show_days", $cal_settings['show_days'], 'onchange="javascript:document.forms[0].submit()"');

echo '</td><td><h3><a href="javascript:goto_date('.($day-$cal_settings['show_days']).', '.$month.', '.$year.');">&lt;&lt;</a>&nbsp;';
echo date($_SESSION['GO_SESSION']['date_format'], $interval_start_time+($_SESSION['GO_SESSION']['timezone']*3600)).' - '.date($_SESSION['GO_SESSION']['date_format'], $interval_end_time+($_SESSION['GO_SESSION']['timezone']*3600));
echo '&nbsp;<a href="javascript:goto_date('.($day+$cal_settings['show_days']).', '.$month.', '.$year.');">&gt;&gt;</a>';
echo '</h3></td></tr></table>';

echo '<table cellspacing="0" cellpadding="0" width="100%">';
echo '<tr height="20">';
echo '<td class="TableHead2" width="100" nowrap>'.$sc_start_time.'</td>';
echo '<td class="TableHead2" width="100" nowrap>'.$sc_end_time.'</td>';
echo '<td class="TableHead2" width="100" nowrap>'.$strName.'</td>';
echo '</tr>';
if (count($events) > 0)
{
  while($event_date = array_shift($events))
  {
    while($event = array_shift($event_date))
    {
      if (date($_SESSION['GO_SESSION']['date_format'], $event['start_time']) != date($_SESSION['GO_SESSION']['date_format'], $event['end_time']))
      {
	$date_format = $_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'];
      }else
      {
	$date_format = $_SESSION['GO_SESSION']['time_format'];
      }
      $title = $sc_start_at.': '.date($date_format, $event['start_time']).'<br />'.
	$sc_end_at.': '.date($date_format, $event['end_time']);

      if ($ab_module && $event['contact_id'] > 0 && $contact = $ab->get_contact($event['contact_id']))
      {
	$middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
	$contact_name = $contact['last_name'].' '.$middle_name.$contact['first_name'];
	$title .= "<br />".$sc_client.": ".htmlspecialchars($contact_name);
	if ($contact['company_name'] != '')
	{
	  $title .= " (".htmlspecialchars($contact['company_name']).")";
	}
      }
      if ($event['location'] != '')
      {
	$title .= "<br />".$sc_location.": ".htmlspecialchars($event['location']);
      }
      $event_cal_count = $cal->get_calendars_from_event($event['id']);
      if($view_id > 0 || $event_cal_count > 1)
      {
	$title .= "<br />$cal_event_calendars: ";
	$first = true;
	while($cal->next_record())
	{
	  if($first)
	  {
	    $first = false;
	  }else
	  {
	    $title .= ' ,';
	  }
	  $title .= htmlspecialchars($cal->f('name'));
	}
      }

      $link = $event['write_permission'] ? $calendar_module_url.'event.php' : $calendar_module_url.'show_event.php';

      $date_format = $event['all_day_event'] == '1' ? $_SESSION['GO_SESSION']['date_format'] : $_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'];
      echo '<tr style="background-color: #'.$event['background'].'" height="16"><td>'.date($date_format, $event['start_time']).'&nbsp;</td>';
      echo '<td>'.date($date_format, $event['end_time']).'&nbsp;</td>';
      echo '<td><a class="normal" href="'.$link.'?event_id='.$event['id'].'&return_to='.rawurlencode($link_back).'" '.$overlib->print_overlib($title).'>'.$event['name'].'</a>&nbsp;</td></tr>';
      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
    }
  }
}else
{
  echo '<tr><td colspan="99">'.$sc_no_events.'</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table>';
