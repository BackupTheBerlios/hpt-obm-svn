<?php
echo '<input type="hidden" name="completed_todo_id" />';
$calendar_module = $GO_MODULES->get_module('calendar');
if ($calendar_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_read']) ||
      $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_write'])))
{
  require($GO_LANGUAGE->get_language_file('calendar'));
  $ab_module = $GO_MODULES->get_module('addressbook');
  if (!$ab_module || !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
  {
    $ab_module = false;
  }else
  {
    require_once($ab_module['class_path'].'addressbook.class.inc');
    $ab = new addressbook();
  }

  echo 	'<table border="0" style="margin-top: 10px"><tr><td valign="top">'.
    '<img style="margin-right: 10px;" width="32" height="32" src="'.$GO_THEME->images['sum_calendar'].
    '" border="0" /></td><td>'.
    '<h2><a href="'.$calendar_module['url'].'">'.
    $lang_modules['calendar'].'</a></h2></td></tr><tr><td></td><td>';

  require_once($calendar_module['class_path'].'calendar.class.inc');
  $cal = new calendar();
  $show_days=1;

  $interval_start_time = mktime(0,0,0,$month, $day, $year)-($_SESSION['GO_SESSION']['timezone']*3600);
  $interval_end_time = mktime(0,0,0,$month, $day+1, $year)-($_SESSION['GO_SESSION']['timezone']*3600);

if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
{
	$dst_offset = $_SESSION['GO_SESSION']['DST'];
}else
{
	$dst_offset = 0;
}
	  
$timezone_offset = $_SESSION['GO_SESSION']['timezone']+$dst_offset;

  $events = $cal->get_events_in_array(0, 0, $GO_SECURITY->user_id, 0, $interval_start_time, $interval_end_time, $day, $month, $year, $timezone_offset , 0, 'Ymd', false);
  //$events = array_merge($events['all_day'], $events['timed']);
  echo '<table cellspacing="0" cellpadding="0">';
  $index_date = date('Ymd', get_time());
  $all_day_events = isset($events['all_day'][$index_date]) ? $events['all_day'][$index_date] : array(); 
  if (count($events['timed']) > 0 || count($all_day_events) > 0)
  {
      while($event = array_shift($all_day_events))
      {
	$link = $event['write_permission'] ? $calendar_module['url'].'event.php' : $calendar_module['url'].'show_event.php';

	$title = '';
	$date_format = '';

	if ($ab_module && $event['contact_id'] > 0 && $contact = $ab->get_contact($event['contact_id']))
	{
	  $middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
	  $contact_name = $contact['last_name'].' '.$middle_name.$contact['first_name'];
	  if ($title != '')
	  {
	    $title .= '<br />';
	  }
	  $title .= $sc_client.": ".htmlspecialchars($contact_name);

	  if ($contact['company_name'] != '')
	  {
	    $title .= " (".htmlspecialchars($contact['company_name']).")";
	  }
	}
	if ($event['location'] != '')
	{
	  if ($title != '')
	  {
	    $title .= '<br />';
	  }
	  $title .= $sc_location.": ".htmlspecialchars($event['location']);
	}

	$event_cal_count = $cal->get_calendars_from_event($event['id']);
		if ($title != '')
	  {
	    $title .= '<br />';
	  }
	$title .= "$cal_event_calendars: ";
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

	echo '<tr height="16"><td>'.date($date_format, $event['start_time']).'&nbsp;</td>';
	echo '<td><a class="normal" href="'.$link.'?event_id='.$event['id'].'&return_to='.
	  rawurlencode($link_back).'" '.$overlib->print_overlib($title).'>'.$event['name'].'</a>&nbsp;</td></tr>';
      }
      while($event_date = array_shift($events['timed']))
      {
	while($event = array_shift($event_date))
	{
	  $link = $event['write_permission'] ? $calendar_module['url'].'event.php' : $calendar_module['url'].'show_event.php';

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
	    $title .= '<br />'.$sc_client.": ".htmlspecialchars($contact_name);

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

	  echo '<tr height="16"><td>'.date($date_format, $event['start_time']).'&nbsp;</td>';
	  echo '<td><a class="normal" href="'.$link.'?event_id='.$event['id'].'&return_to='.
	    rawurlencode($link_back).'" '.$overlib->print_overlib($title).'>'.$event['name'].'</a>&nbsp;</td></tr>';
	}
      }
  }else
  {
    echo '<tr><td colspan="99">'.$sc_no_events.'</td></tr>';
  }
  echo 	'</table>'.
    '</td></tr></table>';


  echo 	'<table border="0" style="margin-top: 10px"><tr><td valign="top">'.
    '<img style="margin-right: 10px;" width="32" height="32" src="'.$GO_THEME->images['todos'].
    '" border="0" /></td><td>'.
    '<h2><a href="'.$calendar_module['url'].'">'.
    $lang_modules['todos'].'</a></h2></td></tr><tr><td></td><td>';

  require_once($calendar_module['class_path'].'todos.class.inc');
  $todos = new todos();

  if (isset($_POST['completed_todo_id']) && $_POST['completed_todo_id'] > 0)
  {
    if ($todo = $todos->get_todo($_POST['completed_todo_id']))
    {
      if ($todos->f('completion_time') > 0)
      {
	$todos->set_uncompleted($_POST['completed_todo_id']);
      }else
      {
	$todos->set_completed($_POST['completed_todo_id']);
      }
    }
  }

  if($todos->get_todos_for_today() == 0)
  {
    echo '<p align="center">'.$cal_no_todos.'</p>';
  }else
  {
    echo '<table cellspacing="0" cellpadding="0">';

    while($todos->next_record())
    {
      $start_time = $todos->f('start_time')+($_SESSION['GO_SESSION']['timezone']*3600);
      $due_time = $todos->f('due_time')+($_SESSION['GO_SESSION']['timezone']*3600);
      $check = '';
      if ($todos->f('completion_time') > 0)
      {
	$class = 'todo_completed';
	$check = 'checked';
      }elseif($due_time < $local_time)
      {
	$class = 'todo_late';
      }else
      {
	$class= 'todo';
      }

      switch ($todos->f("priority"))
      {
	case "0":
	  $priority = '<img src="'.$GO_THEME->images['low_priority'].'" width="8" height="16" />';
	break;

	case "2":
	  $priority = '<img src="'.$GO_THEME->images['high_priority'].'" width="8" height="16" />';
	break;

	default:
	$priority = '';
	break;
      }

      $title = 	$sc_start_at.': '.date($_SESSION['GO_SESSION']['date_format'].' '.
	  $_SESSION['GO_SESSION']['time_format'], $start_time).'<br />'.
	$cal_due_at.': '.date($_SESSION['GO_SESSION']['date_format'].' '.
	    $_SESSION['GO_SESSION']['time_format'], $due_time);
      if ($todos->f('completion_time') > 0)
      {
	$title .= 	'<br />'.$cal_completed.': '.
	  date($_SESSION['GO_SESSION']['date_format'].' '.
	      $_SESSION['GO_SESSION']['time_format'],
	      $todos->f('completion_time')+
	      ($_SESSION['GO_SESSION']['timezone']*3600));
      }

      if ($ab_module && $todos->f('contact_id') > 0 &&
	  $contact = $ab->get_contact($todos->f('contact_id')))
      {
	if ($title != '')
	{
	  $title .= '<br />';
	}
	$middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
	$contact_name = $contact['last_name'].' '.$middle_name.$contact['first_name'];
	$title .= $sc_client.": ".htmlspecialchars($contact_name);
	if ($contact['company_name'] != '')
	{
	  $title .= " (".htmlspecialchars($contact['company_name']).")";
	}
      }
      if ($todos->f('location') != '')
      {
	if ($title != '')
	{
	  $title .= '<br />';
	}
	$title .= $sc_location.": ".htmlspecialchars($todos->f('location'));
      }

      $title .= '<br />'.$cal_status.': '.$todos->f('status').'%';


      echo 	'<tr><td width="20"><input type="checkbox" name="todo[]" value="'.
	$todos->f('id').'" onclick="javascript:'.
	'set_completed(this.value);" '.$check.'></td>'.
	'<td width="8">'.$priority.'</td>'.
	'<td width="100%"><a class="'.$class.'" href="'.$calendar_module['url'].'todo.php?todo_id='.$todos->f('id').
	'&return_to='.urlencode($link_back).'" '.$overlib->print_overlib($title).'>'.$todos->f('name').'</a></td>'.
	'</tr>';
    }
    echo 	'</table>';
  }
  echo '</td></tr></table>';
}
?>
<script type="text/javascript">

function set_completed(todo_id)
{
  document.forms[0].completed_todo_id.value=todo_id;
  document.forms[0].submit();
}

</script>
