<?php

$cal_index = isset($cal_index) ? $cal_index : 0;

$events = $cal->get_events_in_array($calendar_id, $view_id, 0, 0, 
  $interval_start_time, $interval_end_time, 
  $day, $month, $year, 
  $timezone_offset, 
  $cal_start_hour);

for($row=0;$row<$rows;$row++)
{
  $start_day = $row*$max_columns;
  $end_day = $start_day+$max_columns;
  $current_row = $row + $cal_index;
  
  if ($end_day > $cal_settings['show_days'])
  {
    $end_day = $cal_settings['show_days'];
  }

  //This screwed up month view in mozilla 
  /*
     $weeknumber = date("W", mktime(0,0,0,$month,$day+$start_day,$year));

     echo '<tr><td colspan="'.$max_columns.'" class="cal_title">';
     echo $sc_week.' '.$weeknumber;
     echo '</td><td>&nbsp;</td></tr>';*/

  //create table headings
  echo '<tr>';
  echo '<th width="40" nowrap>';
  //echo $sc_week.' '.$weeknumber;
  echo '</th>';

  for ($day_offset=$start_day;$day_offset<$end_day;$day_offset++)
  {
    $day_to_display = $day+$day_offset;
    $new_time = mktime(0,0, 0, $month, $day_to_display, $year);
    $new_date = date(DB_DATE_FORMAT, $new_time);
    $new_day = date('j', $new_time);
    $new_month = date('m', $new_time);
    $new_year = date('Y', $new_time);

    //get holiday name if ever
    $holiday_name = "";
    if($holidays->get_holiday($GO_SECURITY->user_id, $calendar_id, $new_time))
    {
      $holiday_name = $holidays->f("name");
    }

    if ($new_date == $current_date)
    {
      $class="HoverLinkSelected";
    }else
    {
      $class="HoverLink";
    }
    $weekday = date('w', $new_time);
    echo '<th><a href="index.php?calendar_id='.$calendar_id.'&year='.$new_year.
      '&month='.$new_month.'&day='.$new_day.'&show_days=1" class="'.
      $class.'">'.$full_days[$weekday].'<br />'.
      date($_SESSION['GO_SESSION']['date_format'],$new_time).
      '<br />'.$holiday_name.'</a>';

    echo '</th>';
  }
  echo '</tr>';

  //process the all day events

  if (count($events['all_day']))
  {
    echo '<tr id="cal_all_day_events">';
    echo '<td class="cal_hour"></td>';

    for ($day_offset=$start_day;$day_offset<$end_day;$day_offset++)
    {
      echo '<td>';
      $day_to_display = $day+$day_offset;
      $new_time = mktime(0,0, 0, $month, $day_to_display, $year);
      $new_date = date('Ymd', $new_time);
      if (isset($events['all_day'][$new_date]))
      {
	echo '<table id="cal_event" width="100%" height="100%">';
	while ($event = array_shift($events['all_day'][$new_date]))
	{
	  echo '<tr style="background-color: #'.$event['background'].'"><td width="100%">';
	  $link = $event['write_permission'] ? 'event.php' : 'show_event.php';

	  $title = '';
	  if (date($_SESSION['GO_SESSION']['date_format'], $event['start_time']) != date($_SESSION['GO_SESSION']['date_format'], $event['end_time']))
	  {
	    $title .= date($_SESSION['GO_SESSION']['date_format'], $event['start_time']).' - '.date($_SESSION['GO_SESSION']['date_format'], $event['end_time']);
	  }
	  if ($ab_module && $event['contact_id'] > 0 && $contact = $ab->get_contact($event['contact_id']))
	  {
	    if ($title != '')
	    {
	      $title .= "<br />";
	    }
	    $middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
	    $contact_name = $contact['last_name'].' '.$middle_name.$contact['first_name'];
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
	      $title .= "<br />";
	    }
	    $title .= $sc_location.": ".htmlspecialchars($event['location']);
	    //$title .= $sc_description.": ".htmlspecialchars($event['description']);
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
		$title .= ', ';
	      }						  
	      $title .= htmlspecialchars($cal->f('name'));  
	    }
	  }

	  echo '<a class="calendar_event_link" href="'.$link.'?event_id='.$event['id'].'" '.$overlib->print_overlib($title).'>'.htmlspecialchars($event['name']).'</a>';
	  if (($event['write_permission'] || ($calendar_id > 0 && $calendar['write_permission'])) && !$print)
	  {
	    echo '</td><td valign="top" width="16" style="background-color: #'.$event['background'].'">';
	    echo '<a href="delete_event.php?event_id='.$event['id'].'&calendar_id='.$calendar_id.'&return_to='.rawurlencode($_SERVER['REQUEST_URI']).'" title="'.$strDeleteItem.' "\''.htmlspecialchars($cal->f("name")).'\'"><img src="'.$GO_THEME->images['delete'].'" border="0"></a>';
	  }
	  echo '</td></tr>';
	}
	echo '</table></td>';
      }
    }
    echo "</tr>\r\n";
  }

  for ($hour_of_day=$cal_start_hour;$hour_of_day<=$cal_end_hour;$hour_of_day++)
  {
    echo '<tr id="'.$current_row.'_'.$hour_of_day.'"><td width="40" align="right" class="cal_hour">'.$hour_of_day.':00</td>';

    //display the days row by row
    for ($day_offset=$start_day;$day_offset<$end_day;$day_offset++)
    {
      $day_to_display = $day+$day_offset;

      //make a valid date
      $new_time = mktime(0,0, 0, $month, $day_to_display, $year);
      $new_date = date('Ymd', $new_time);
      $new_day = date("j", $new_time);
      $new_month = date('m', $new_time);
      $new_year = date('Y', $new_time);

      if(isset($events['timed'][$new_date][$hour_of_day]))
      {
	echo '<td>';
	echo '<table height="100%" id="cal_event">';

	while($event = array_shift($events['timed'][$new_date][$hour_of_day]))
	{
	  echo '<tr style="background-color: #'.$event['background'].';"><td width="100%">';
	  $link = $event['write_permission'] ? 'event.php' : 'show_event.php';
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
		$title .= ', ';
	      }						  
	      $title .= htmlspecialchars($cal->f('name'));  
	    }
	  }
	  if ($event['description'] != '')
	  {
            $description = str_replace("\r\n", "<br />", $event['description']);
            $description = str_replace("\r", "<br />", $description);
            $description = str_replace("\n", "<br />", $description);
	    $title .= "<br />".$sc_description.": ".htmlspecialchars($description);
	  }

	  echo '<a class="calendar_event_link" href="'.$link.'?event_id='.$event['id'].'&return_to='.rawurlencode($_SERVER['REQUEST_URI']).'" '.$overlib->print_overlib($title).'>'.date($_SESSION['GO_SESSION']['time_format'], $event['start_time']).' '.htmlspecialchars($event['name']).'</a>';

	  if (($event['write_permission'] || ($calendar_id > 0 && $calendar['write_permission'])) && !$print)
	  {						
	    echo '</td><td valign="top" width="16">';
	    echo '<a href="delete_event.php?event_id='.$event['id'].'&calendar_id='.$calendar_id.'&return_to='.rawurlencode($_SERVER['REQUEST_URI']).'" title="'.$strDeleteItem.' \''.htmlspecialchars($event["name"]).'\'"><img src="'.$GO_THEME->images['delete'].'" border="0"></a>';
	  }else
	  {
	    echo '<td>&nbsp;</td>';
	  }
	  echo '</td></tr>';
	}
	echo '</table>';
      }else
      {
	if (isset($events['backgrounds'][$new_date][$hour_of_day]))
	{
	  $style = ' style="background-color: #'.$events['backgrounds'][$new_date][$hour_of_day].'"';
	}else
	{
	  $style = '';
	}
	echo '<td height="100%"'.$style.'>';
	if(!$print)
	{
	  if ($new_date >= date('Ymd', time()))
	  echo '<a onmouseover="javascript:glow_row(\''.$current_row.'_'.$hour_of_day.'\');" onmouseout="javascript:unglow_row(\''.$current_row.'_'.$hour_of_day.'\');" href="event.php?calendar_id='.$calendar_id.'&day='.$new_day.'&month='.$new_month.'&year='.$new_year.'&hour='.$hour_of_day.'"><img border="0" width="100%" height="12" src="'.$GO_THEME->images['blank'].'" /></a>';
	  else
	  echo '<a onmouseover="javascript:glow_row(\''.$current_row.'_'.$hour_of_day.'\');" onmouseout="javascript:unglow_row(\''.$current_row.'_'.$hour_of_day.'\');" href="javascript:alert(\''.$strNoPastEvent.'\')"><img border="0" width="100%" height="12" src="'.$GO_THEME->images['blank'].'" /></a>';
	}

      }
      echo '</td>';
    }
    echo "</tr>\r\n";
  }
}
?>
