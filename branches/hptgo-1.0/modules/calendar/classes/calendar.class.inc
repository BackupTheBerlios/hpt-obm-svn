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

define('REPEAT_NONE', 0);
define('REPEAT_DAILY', 1);
define('REPEAT_WEEKLY', 2);
define('REPEAT_MONTH_DATE', 3);
define('REPEAT_MONTH_DAY', 4);
define('REPEAT_YEARLY', 5);

define('DB_DATETIME_FORMAT', 'Y-m-d H:i:00');
define('DB_DATE_FORMAT', 'Y-m-d');
define('DB_TIME_FORMAT', 'H:i:00');

class calendar extends db
{
	var $events = array();
	var $all_day_events = array();
	var $backgrounds = array();
	
  function calendar()
  {
    $this->db();
  }
  /*
     takes a sting YYYY-MM-DD HH:MM in GMT time and converts it to an array with
     hour, min etc. with	a timezone offset. If 0000 or 00 is set in a date
     (not time) then it will be replaced with current locale	date.
   */
  function explode_datetime($datetime_stamp, $timezone_offset)
  {
    $local_time = get_time();

    $datetime_array = explode(' ', $datetime_stamp);
    $date_stamp = $datetime_array[0];
    $time_stamp = isset($datetime_array[1]) ? $datetime_array[1] : '00:00:00';

    $date_array = explode('-',$date_stamp);

    $year = $date_array[0] == '0000' ? date('Y', $local_time) : $date_array[0];
    $month = $date_array[1] == '00' ? date('n', $local_time) : $date_array[1];
    $day = $date_array[2] == '00' ? date('j', $local_time) : $date_array[2];;

    $time_array = explode(':',$time_stamp);
    $hour = $time_array[0];
    $min = $time_array[1];

    $unix_time = mktime($hour, $min, 0, $month, $day, $year);

    $unix_time = $unix_time+($timezone_offset*3600);

    $result['year'] = date('Y', $unix_time);
    $result['month'] = date('n', $unix_time);
    $result['day'] = date('j', $unix_time);
    $result['hour'] = date('G', $unix_time);
    $result['min'] = date('i', $unix_time);

    return $result;
  }

  function add_view($user_id, $name, $start_hour, $end_hour, $type)
  {
    $view_id = $this->nextid("cal_views");

    if ($view_id > 0)
    {
      $sql = "INSERT INTO cal_views (id, user_id, name, start_hour, end_hour, type) ".
	"VALUES ('$view_id', '$user_id', '$name', '$start_hour', '$end_hour', '$type')";
      $this->query($sql);
      return $view_id;
    }	
    return false;
  }

  function update_view($view_id, $name, $start_hour, $end_hour, $type)
  {
    $sql = "UPDATE cal_views SET name='$name', start_hour='$start_hour', ".
      "end_hour='$end_hour', type='$type' WHERE id='$view_id'";
    return $this->query($sql);
  }

  function delete_view($view_id)
  {
    if($this->query("DELETE FROM cal_views_calendars WHERE view_id='$view_id'"))
    {
      return $this->query("DELETE FROM cal_views WHERE id='$view_id'");
    }
  }

  function get_views($user_id)
  {
    $sql = "SELECT * FROM cal_views WHERE user_id='$user_id'";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_view($view_id)
  {
    $sql = "SELECT * FROM cal_views WHERE id='$view_id'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function get_view_calendars($view_id)
  {
    $cal = new calendar;

    $sql = "SELECT * FROM cal_views_calendars WHERE view_id='$view_id'";
    $this->query($sql);

    while($this->next_record())
    {
      $calendars[] = $cal->get_calendar($this->f("calendar_id"));
    }

    if(isset($calendars))
    {
      return $calendars;
    }
    return false;
  }

  function get_view_calendar_ids($view_id)
  {
    $cal = new calendar;

    $sql = "SELECT * FROM cal_views_calendars WHERE view_id='$view_id'";
    $this->query($sql);

    while($this->next_record())
    {
      $calendars[] = $this->f("calendar_id");
    }

    if(isset($calendars))
    {
      return $calendars;
    }
    return false;
  }

  function add_calendar_to_view($calendar_id, $view_id)
  {
    $sql = "INSERT INTO cal_views_calendars (view_id, calendar_id) ".
      "VALUES ('$view_id', '$calendar_id')";
    return $this->query($sql);
  }

  function remove_calendar_from_view($calendar_id, $view_id)
  {
    $sql = "DELETE FROM cal_views_calendars WHERE calendar_id='$calendar_id' AND view_id='$view_id'";
    return $this->query($sql);
  }

  function calendar_is_in_view($calendar_id, $view_id)
  {
    $sql = "SELECT * FROM cal_views_calendars WHERE calendar_id='$calendar_id' AND view_id='$view_id'";
    $this->query($sql);
    return $this->next_record();
  }

  function get_view_by_name($user_id, $name)
  {
    $sql = "SELECT * FROM cal_views WHERE user_id='$user_id' AND name='$name'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function user_has_calendar($user_id)
  {
    $sql = "SELECT id FROM cal_calendars WHERE user_id='$user_id'";
    $this->query($sql);
    return $this->next_record();
  }

  function add_participant($event_id, $email, $user_id = 0)
  {
    $id = $this->nextid("cal_participants");

    if ($id > 0)
    {
      $sql = 	"INSERT INTO cal_participants (id, event_id, user_id, email) ".
	"VALUES ('$id', '$event_id', '$user_id', '$email')";
      if ($this->query($sql))
      {
	return $id;
      }else
      {
	return -1;
      }
    }else
    {
      return -1;
    }
  }

  function remove_participants($event_id)
  {
    $sql = "DELETE FROM cal_participants WHERE event_id='$event_id'";
    return $this->query($sql);
  }

  function is_participant($event_id, $email)
  {
    $sql = "SELECT id FROM cal_participants WHERE event_id='$event_id' AND email='$email'";
    $this->query($sql);
    return $this->next_record();
  }

  function get_participants($event_id)
  {
    $sql = "SELECT * FROM cal_participants WHERE event_id='$event_id' ORDER BY email ASC" ;
    $this->query($sql);
    return $this->num_rows();
  }

  function set_default_calendar($user_id, $calendar_id)
  {
    $sql = "UPDATE cal_settings SET default_cal_id='$calendar_id' WHERE user_id='$user_id'";
    return $this->query($sql);
  }
  
  function set_default_view($user_id, $calendar_id, $view_id)
  {
  	$sql = "UPDATE cal_settings SET default_cal_id='$calendar_id', default_view_id='$view_id' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function get_settings($user_id)
  {
    $this->query("SELECT * FROM cal_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO cal_settings (user_id) VALUES ('$user_id')");
      return $this->get_settings($user_id);
    }		
  }

  function set_show_days($user_id, $show_days)
  {
    $sql = "UPDATE cal_settings SET show_days='$show_days' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function set_todo_view($user_id, $hide_completed_todos, $show_todos)
  {
    $sql = "UPDATE cal_settings SET hide_completed_todos='$hide_completed_todos',".
      "show_todos='$show_todos' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function add_calendar($user_id, $name, $start_hour, $end_hour)
  {
    global $GO_SECURITY;
    $acl_read = $GO_SECURITY->get_new_acl('calendar read: '.$name);
    $acl_write = $GO_SECURITY->get_new_acl('calendar write: '.$name);
    if ($acl_read > 0 && $acl_write > 0)
    {
      $calendar_id = $this->nextid("cal_calendars");
      if ($calendar_id > 0)
      {
	$sql  = "INSERT INTO cal_calendars (id, user_id, name, acl_read, acl_write, start_hour, end_hour) ";
	$sql .= "VALUES ('$calendar_id', '$user_id', '$name', '$acl_read', '$acl_write', '$start_hour', '$end_hour')";
	if ($this->query($sql))
	{
	  $GO_SECURITY->add_user_to_acl($user_id,$acl_write);
	  return $calendar_id;
	}
      }else
      {
	$GO_SECURITY->delete_acl($acl_read);
	$GO_SECURITY->delete_acl($acl_write);
      }

    }
    return false;
  }

  function delete_calendar($calendar_id)
  {
    global $GO_SECURITY;
    $delete = new calendar;

    $sql = "SELECT * FROM cal_events_calendars WHERE calendar_id='$calendar_id'";
    $this->query($sql);

    while ($this->next_record())
    {
      $sql = "SELECT * FROM cal_events_calendars WHERE event_id='".$this->f("event_id")." '";
      $delete->query($sql);
      if ($delete->num_rows() < 2)
      {
	$event = $delete->get_event($this->f("event_id"));
	$GO_SECURITY->delete_acl($event["acl_read"]);
	$GO_SECURITY->delete_acl($event["acl_write"]);
	$sql = "DELETE FROM cal_events WHERE id='".$this->f("event_id")."'";
	$delete->query($sql);
	$sql = "DELETE FROM cal_participants WHERE event_id='".$this->f("event_id")."'";
	$delete->query($sql);
	$sql = "DELETE FROM cal_reminders WHERE event_id='".$this->f("event_id")."'";
	$delete->query($sql);
      }
    }
    $sql = "DELETE FROM cal_events_calendars WHERE calendar_id='$calendar_id'";
    $this->query($sql);

    $sql= "DELETE FROM cal_calendars WHERE id='$calendar_id'";
    $this->query($sql);
    $sql = "DELETE FROM cal_views_calendars WHERE calendar_id='$calendar_id'";
    $this->query($sql);
  }

  function update_calendar($calendar_id, $name, $start_hour, $end_hour)
  {
    $sql = "UPDATE cal_calendars SET name='$name', start_hour='$start_hour', end_hour='$end_hour' WHERE id='$calendar_id'";
    return $this->query($sql);
  }


  function get_calendar($calendar_id)
  {
    $sql = "SELECT * FROM cal_calendars WHERE id='$calendar_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function get_calendar_by_name($name)
  {
    $sql = "SELECT * FROM cal_calendars WHERE name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function get_user_calendars($user_id)
  {
    $sql = "SELECT * FROM cal_calendars WHERE user_id='$user_id' ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_calendars()
  {
    $sql = "SELECT * FROM cal_calendars ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_authorised_calendars($user_id)
  {
    $sql = "SELECT DISTINCT cal_calendars.* FROM cal_calendars, acl, users_groups WHERE (".
      "cal_calendars.acl_read = acl.acl_id OR cal_calendars.acl_write = acl.acl_id".
      ") AND ( ( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id." ) ) ORDER BY cal_calendars.name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function set_event_status($event_id, $status, $email)
  {
    $sql = "UPDATE cal_participants SET status='$status' WHERE email='$email' AND event_id='$event_id'";
    return $this->query($sql);
  }

  function get_event_status($event_id, $email)
  {
    $sql = "SELECT status FROM cal_participants WHERE email='$email' AND event_id='$event_id'";
    if($this->query($sql))
    {
      if($this->next_record())
      {
	return $this->f('status');
      }
    }
    return false;
  }


  /*
     Times in GMT!
   */
  function add_event($user_id, $start_time, $end_time, $all_day_event, $name,
      $description, $contact_id, $reminder, $location,
      $background, $repeat_type, $repeat_end_time, $month_time,
      $repeat_forever, $repeat_every, $mon, $tue, $wed, $thu,
      $fri, $sat, $sun, $acl_read, $acl_write)
  {
    $event_id = $this->nextid("cal_events");
    if ($event_id > 0)
    {
      $gmt_time = get_gmt_time();
      $sql = "INSERT INTO cal_events (id, user_id, start_time, end_time, ".
	"all_day_event, acl_read, acl_write, name, description, ".
	"contact_id, reminder, location, background, repeat_type,".
	" repeat_end_time, month_time, repeat_forever, repeat_every, mon,".
	" tue, wed, thu, fri, sat, sun, ctime, mtime) VALUES ".
	"('$event_id', '$user_id', '$start_time', '$end_time', ".
	"'$all_day_event', '$acl_read', '$acl_write', '$name', ".
	"'$description', '$contact_id', '$reminder', '$location',".
	" '$background', '$repeat_type', '$repeat_end_time',".
	" '$month_time', '$repeat_forever', '$repeat_every',  '$mon', ".
	"'$tue', '$wed', '$thu', '$fri', '$sat', '$sun', '$gmt_time',".
	" '$gmt_time')";

      if ($this->query($sql))
      {
	return $event_id;
      }
    }
    return false;
  }

  function update_event($event_id, $start_time, $end_time,
      $all_day_event, $name, $description, $contact_id,
      $reminder, $location, $background, $repeat_type,
      $repeat_end_time, $month_time, $repeat_forever,
      $repeat_every, $mon, $tue, $wed, $thu, $fri,
      $sat, $sun)
  {
    $sql = "UPDATE cal_events SET contact_id='$contact_id', ".
      "start_time='$start_time', end_time='$end_time', ".
      "all_day_event='$all_day_event', name='$name', ".
      "description='$description', reminder='$reminder',".
      " location='$location', background='$background', ".
      "repeat_type='$repeat_type', ".
      "repeat_end_time='$repeat_end_time', month_time='$month_time',".
      " repeat_forever='$repeat_forever', repeat_every='$repeat_every',".
      " mon='$mon', tue='$tue', wed='$wed', thu='$thu', fri='$fri', ".
      "sat='$sat', sun='$sun', mtime='".get_gmt_time()."' ".
      "WHERE id='$event_id'";

    return $this->query($sql);
  }

  function subscribe_event($event_id, $calendar_id)
  {
    $sql = "INSERT INTO cal_events_calendars (calendar_id, event_id) VALUES ('$calendar_id','$event_id')";
    return $this->query($sql);
  }

  function unsubscribe_event($event_id, $calendar_id)
  {
    $sql = "DELETE FROM cal_events_calendars WHERE calendar_id='$calendar_id' AND event_id='$event_id'";
    return $this->query($sql);
  }

  function event_is_subscribed($event_id, $calendar_id)
  {
    $sql = "SELECT * FROM cal_events_calendars WHERE event_id='$event_id' AND calendar_id='$calendar_id'";
    $this->query($sql);
    return $this->next_record();
  }

  function get_event_subscribtions($event_id)
  {
    $sql = "SELECT * FROM cal_events_calendars WHERE event_id='$event_id'";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_calendars_from_event($event_id)
  {
    $sql = "SELECT name FROM cal_calendars INNER JOIN cal_events_calendars ".
      "ON cal_events_calendars.calendar_id=cal_calendars.id ".
      "WHERE cal_events_calendars.event_id='$event_id'";
    $this->query($sql);
    return $this->num_rows();
  }

  function insert_reminder($user_id, $event_id, $remind_time)
  {
    return $this->query("REPLACE cal_reminders (user_id, event_id, remind_time) VALUES ('$user_id', '$event_id', '$remind_time')");
  }

  function update_reminder($user_id, $event_id, $remind_time)
  {
    return $this->query("UPDATE cal_reminders SET remind_time='$remind_time' WHERE user_id='$user_id' AND event_id='$event_id'");
  }

  function delete_reminder($user_id, $event_id)
  {
    return $this->query("DELETE FROM cal_reminders WHERE user_id='$user_id' AND event_id='$event_id'");
  }
  /*
     returns next starting time of an event in GMT timezone
   */

  function get_next_recurrence_time($event_id, $gmt_time=0, $event='')
  {
    if ($event == '')
    {
      $event = $this->get_event($event_id);
    }

    if ($gmt_time == 0)
    {
      $gmt_time = get_gmt_time();
    }

    if ($event['repeat_forever'] == '0' && ($event['repeat_end_time'] < $gmt_time && $event['repeat_type'] != REPEAT_NONE))
    {
      return 0;
    }

    if ($event['start_time'] > $gmt_time && $event['repeat_type'] != REPEAT_NONE)
    {
      $start_time = $event['start_time'];
    }else
    {
      $start_time = $gmt_time;
    }
    $year = date('Y', $start_time);
    $month = date('n', $start_time);
    $day = date('j', $start_time);

    $event['start_year'] = date('Y', $event['start_time']);
    $event['start_month'] = date('n', $event['start_time']);
    $event['start_day'] = date('j', $event['start_time']);
    $event['start_hour'] = date('G', $event['start_time']);
    $event['start_min'] = date('i', $event['start_time']);

    $day_db_field[0] = 'sun';
    $day_db_field[1] = 'mon';
    $day_db_field[2] = 'tue';
    $day_db_field[3] = 'wed';
    $day_db_field[4] = 'thu';
    $day_db_field[5] = 'fri';
    $day_db_field[6] = 'sat';

    switch($event['repeat_type'])
    {
      case REPEAT_NONE:
	return $event['start_time'];
	break;

      case REPEAT_WEEKLY:
	if ($event['repeat_every'] > 1)
	{
	  $interval = $start_time - $event['start_time'];
	  $interval_weeks = floor($interval/604800);
	  $devided = $interval_weeks/$event['repeat_every'];
	  $rounded = ceil($devided);
	  $last_repeat_week = $event['repeat_every']*$rounded*604800;

	  $last_occurence_week = $event['start_time']+$last_repeat_week;
	  $year = date('Y', $last_occurence_week);
	  $month = date('n', $last_occurence_week);
	  $day = date('j', $last_occurence_week);
	}else
	{
	  $last_occurence_week= $start_time;
	}

	$weekday = date("w", $last_occurence_week);
	//loop one day more then one week for occurence that happen exactly
	//one week later but earlier then this time
	$loops = $event['repeat_every'] > 1 ? 2 : 1;

	for ($loop=0;$loop<$loops;$loop++)
	{
	  $day_add=$loop*$event['repeat_every']*7;

	  for ($i=0;$i<7;$i++)
	  {
	    if ($weekday>6)
	    {
	      $weekday -= 7;
	    }

	    if ($event[$day_db_field[$weekday]] == '1')
	    {
	      $occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $month, $day+$i+$day_add, $year);

	      if ($event['repeat_forever'] == '0' && ($occurence_time > $event['repeat_end_time']))
	      {
		return 0;
	      }elseif ($occurence_time > $start_time)
	      {
		return $occurence_time;
	      }
	    }
	    $weekday++;
	  }
	}

	break;

      case REPEAT_DAILY:

	$occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $month, $day, $year);

	if ($occurence_time <= $start_time)
	{
	  $occurence_time =  mktime($event['start_hour'], $event['start_min'], 0, $month, $day+$event['repeat_every'], $year);
	}

	if ($event['repeat_forever'] == '0' && $occurence_time > $event['repeat_end_time'])
	{
	  return 0;
	}else
	{
	  return $occurence_time;
	}


	break;

      case REPEAT_MONTH_DATE:
	$occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $month, $event['start_day'], $year);
	if ($occurence_time < $start_time)
	{
	  $occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $month+$event['repeat_every'], $event['start_day'], $year);
	}

	if ($event['repeat_forever'] == '0' && $occurence_time > $event['repeat_end_time'])
	{
	  return 0;
	}else
	{
	  return $occurence_time;
	}
	break;

      case REPEAT_MONTH_DAY:

	if ($event['repeat_every'] > 1)
	{
	  $interval_years = $year-date('Y', $event['start_time']);
	  $interval_months = $month - date('n', $event['start_time']);
	  $interval_months = 12*$interval_years+$interval_months;
	  $devided = $interval_months/$event['repeat_every'];
	  $rounded = (int) $devided;
	  $last_repeat_month = $event['repeat_every']*$rounded;

	  $repeat_year = date('Y', $event['start_time']);
	  $repeat_month = date('n', $event['start_time']);
	  $repeat_day = date('j', $event['start_time']);

	  $last_occurence_month = mktime(0,0,0, $repeat_month+$last_repeat_month, $repeat_day, $repeat_year);
	  $year = date('Y', $last_occurence_month);
	  $month = date('n', $last_occurence_month);
	  $day = date('j', $last_occurence_month);
	}else
	{
	  $last_occurence_month = $start_time;
	}

	$loops = $event['repeat_every'] > 1 ? 2 : 1;
	for ($loop=0;$loop<$loops;$loop++)
	{
	  if ($loop > 0)
	  {
	    $month += ($loop*$event['repeat_every'])-1;
	  }

	  //loop one day more then one week for occurence that happen exactly
	  //one week later but earlier then this time
	  for ($i=0;$i<32;$i++)
	  {
	    $occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $month, $i+1, $year);
	    $year = date('Y', $occurence_time);
	    $month = date('n', $occurence_time);
	    $day = date('j', $occurence_time);

	    $weekday = date("w", $occurence_time);

	    if ($event[$day_db_field[$weekday]] == '1')
	    {
	      if (ceil(date('j',$occurence_time)/7) == $event['month_time'])
	      {
		if ($event['repeat_forever'] == '0' && $occurence_time > $event['repeat_end_time'])
		{
		  return 0;
		}elseif ($occurence_time > $start_time)
		{
		  return $occurence_time;
		}
	      }
	    }
	  }
	}

	break;

	case REPEAT_YEARLY;
	$occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $event['start_month'], $event['start_day'], $year);
	if ($occurence_time < $start_time)
	{
	  $occurence_time = mktime($event['start_hour'], $event['start_min'], 0, $event['start_month'], $event['start_day'], $year+$event['repeat_every']);
	}
	if ($event['repeat_forever'] == '0' && $occurence_time > $event['repeat_end_time'])
	{
	  return 0;
	}else
	{
	  return $occurence_time;
	}
	break;
    }
  }

  /*
     Times in GMT!
   */

  function get_events($calendar_id=0, $view_id=0, $user_id=0, $contact_id=0, $interval_start=0, $interval_end=0)
  {
    $sql  = "SELECT DISTINCT cal_events.* FROM cal_events";

    if ($view_id > 0)
    {
      $sql .= " INNER JOIN cal_events_calendars ON (cal_events.id=cal_events_calendars.event_id)";
      $sql .= " INNER JOIN cal_views_calendars ON (cal_views_calendars.calendar_id=cal_events_calendars.calendar_id)";
    }elseif ($calendar_id > 0)
    {
      $sql .= " INNER JOIN cal_events_calendars ON (cal_events.id=cal_events_calendars.event_id)";
    }elseif($user_id > 0)
    {
      $sql .= " INNER JOIN cal_events_calendars ON (cal_events.id=cal_events_calendars.event_id)";
      $sql .= " INNER JOIN cal_calendars ON (cal_events_calendars.calendar_id=cal_calendars.id)";
    }

    $sql .= " WHERE";

    if ($view_id > 0)
    {
      $sql .= " cal_views_calendars.view_id='$view_id'";
    }elseif($user_id > 0)
    {
      $sql .= " cal_calendars.user_id='$user_id'";
    }elseif ($calendar_id > 0)
    {
      $sql .= " cal_events_calendars.calendar_id='$calendar_id'";
    }elseif ($contact_id > 0)
    {
      $sql .= "  cal_events.contact_id='$contact_id'";
    }

    if ($interval_start > 0)
    {
      $sql .= " AND ((cal_events.repeat_type='".REPEAT_NONE."' AND ".
	"cal_events.start_time<='$interval_end' AND cal_events.end_time>='$interval_start') OR ".
	"(cal_events.repeat_type!='".REPEAT_NONE."' AND ".
	"cal_events.start_time<='$interval_end' AND ".
	"(cal_events.repeat_end_time>='$interval_start' OR cal_events.repeat_forever='1')))";
    }
    $sql .=	" ORDER BY start_time";
    $this->query($sql);
    return $this->num_rows();
  }

  function calculate_repeat_once($event, $timezone_offset)
  {
    //determine permissions
    if (!$event['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $event['acl_write']))
    {
      $event['read_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $event['acl_read']);
    }
    if (!$event['write_permission'] && !$event['read_permission'])
    {
      return false;
    }else
    {
      //calculate stored GMT unix times to local timezone dates
      //don't calculate timezone offset for all day events
      $timezone_offset = ($event['all_day_event'] == '0') ? $timezone_offset : 0;

      //the start time of the first occurence localized
      $initial_start_time = $event['start_time']+($_timezone_offset*3600);		
    }

  }

  function get_events_in_array($calendar_id, $view_id, $user_id, $contact_id, 
					$interval_start_time, $interval_end_time, $day, $month, $year, 
					$timezone_offset, $display_start_hour=0, $index_date='Ymd', 
					$index_hour=true)
  {
	$this->events = array();
	$this->all_day_events = array();
	$this->backgrounds = array();
	
    if($count = $this->get_events($calendar_id,
																	$view_id,
																	$user_id,
																	$contact_id,
																	$interval_start_time,
																	$interval_end_time))
    {
	  while($this->next_record())
      {
				$this->calculate_event($this->Record, 
															$interval_start_time, 
															$interval_end_time, 
															$day, 
															$month, 
															$year, 
															$timezone_offset, 
															$display_start_hour, 
															$index_date, 
															$index_hour);
      }
    }
    if ($index_date)
    {
      $all_events['timed'] = $this->events;
      $all_events['all_day'] = $this->all_day_events;
      $all_events['backgrounds'] = $this->backgrounds;
      return $all_events;
    }else
    {
	  return $events;
    }
  }
	
	function add_event_to_array($event, $interval_start_time, $interval_end_time, $timezone_offset, $index_date, $index_hour, $display_start_hour)
	{
		if ($index_date)
	  {
	    if ($event['all_day_event'] == '1')
	    {
	      //if event starts before the time to display skip the
	      //time not displayed
	      $time_before = $interval_start_time+($timezone_offset*3600) - $event['start_time'];
	      if($time_before > 0)
	      {
					$event_start_day = floor($time_before/86400);
	      }else
	      {
					$event_start_day = 0;
	      }
	      if($index_hour)
	      {
	      	$duration_days = ceil(($event['end_time'] - $event['start_time'])/86400)+1;
	      	
					for ($event_day=$event_start_day;$event_day<$duration_days;$event_day++)
					{
					  $process_time = mktime(0,0,0,$event['start_month'], $event['start_day']+$event_day, $event['start_year']);
					  $process_date = date($index_date, $process_time);
					  $this->all_day_events[$process_date][] = $event;
					}
	      }else
	      {
					$process_time = mktime(0,0,0,$event['start_month'], $event['start_day'], $event['start_year']);
					$process_date = date($index_date, $process_time);
					$this->all_day_events[$process_date][] = $event;
	      }
	    }else
	    {
	      //if event starts before the time to display skip the
	      //time not displayed
	      $time_before = $interval_start_time+($timezone_offset*3600) - $event['start_time'];
	      if($time_before > 0)
	      {
					$event_start_hour = floor($time_before/3600);
	      }else
	      {
					$event_start_hour = 0;
	      }
	      
	      //how many hours does this event occur?
	      $real_duration_hours = ($event['end_time'] - $event['start_time'])/3600;
	      $duration_hours = ceil($real_duration_hours+($event['start_min']/60));
	      //zero hours should still be displayed in one hour
	      if ($duration_hours == 0)
	      {
					$duration_hours = 1;
	      }
	      
	      for($event_hour=$event_start_hour;$event_hour<$duration_hours;$event_hour++)
	      {
					//the time we are now processing
					$process_time = mktime($event['start_hour']+$event_hour,
																$event['start_min'],0,$event['start_month'], 
																$event['start_day'], $event['start_year']);
					
					//if we are processing time that happens later then the end of the 
					//display time interval then exit the loop			
					if (($process_time-($_SESSION['GO_SESSION']['timezone']*3600)) > $interval_end_time)
					{
					  break;
					}
					$process_date = date($index_date, $process_time);
					$process_hour = date('G', $process_time);

					if ($event_hour == 0 || ($process_hour == $display_start_hour && $index_hour))
					{
					  if ($index_hour)
					  {
					    $this->events[$process_date][$process_hour][] = $event;
					  }else
					  {
					    $this->events[$process_date][] = $event;
					    if ($process_hour == $display_start_hour)
					    {
					      $event_hour += 23-$display_start_hour;
					    }
					  }			
					}else
					{
					  $this->backgrounds[$process_date][$process_hour] = $event['background'];
					}
	      }
	    }
	  }else
	  {
	    $this->events[] = $event;
	  }	
	}


  function calculate_event($event, $interval_start_time, $interval_end_time, 
						$day, $month, $year, $timezone_offset, $display_start_hour=0, 
						$index_date='Ymd', $index_hour=true)
  {
  	global $GO_SECURITY;
  	
  	$show_days = ceil(($interval_end_time - $interval_start_time)/86400);
  	
    if (!$event['write_permission'] = 
			$GO_SECURITY->has_permission($GO_SECURITY->user_id, $event['acl_write']))
    {
      $event['read_permission'] = 
				$GO_SECURITY->has_permission($GO_SECURITY->user_id, $event['acl_read']);
    }

    if ($event['write_permission'] || $event['read_permission'])
    {
      //calculate stored GMT unix times to local timezone dates
      //don't calculate timezone offset for all day events
      $_timezone_offset = ($event['all_day_event'] == '0') ? $timezone_offset : 0;

      //the start time of the first occurence localized
      $initial_start_time = $event['start_time']+($_timezone_offset*3600);

      $event['repeat_start_time'] = $initial_start_time;
      $event['repeat_end_time'] += ($_timezone_offset*3600);

      $event['start_hour'] = date('G', $initial_start_time);
      $event['start_min'] = date('i', $initial_start_time);

      //the end time of the first occurence localized
      $initial_end_time = $event['end_time']+($_timezone_offset*3600);

      $event['end_hour'] = date('G', $initial_end_time);
      $event['end_min'] = date('i', $initial_end_time);

      $event['start_date'] = date('Y-m-d', $initial_start_time);

      if ($event['reminder'] > 0)
      {
				$event['remind_time'] = $event['start_time'] - $event['reminder'];
      }
      

      switch($event['repeat_type'])
      {
				case REPEAT_NONE:
				  $event['start_time'] += ($_timezone_offset*3600);
				  $event['end_time'] += ($_timezone_offset*3600);
				  $event['start_year'] = date('Y', $event['start_time']);
				  $event['start_month'] = date('n', $event['start_time']);
				  $event['start_day'] = date('j', $event['start_time']);	  
					
					$this->add_event_to_array($event, $interval_start_time, 
						$interval_end_time, $timezone_offset, $index_date, 
						$index_hour, $display_start_hour);
				  break;

				case REPEAT_DAILY:
				  //loop through all days that are displayed + the last day for occurences
					//that started outside but end in the display interval
				  for($show_day=-1;$show_day<$show_days;$show_day++)
				  {
				    $day_start_time = mktime(0,0,0,$month, $day+$show_day, $year);
				    $day_end_time = mktime(0,0,0,$month, $day+$show_day+1, $year);
			
				    //does the event occur at the current day to display?
				    if ($event['repeat_start_time'] < $day_end_time && 
							($event['repeat_end_time'] >= $day_start_time || 
							$event['repeat_forever'] == '1'))
				    {
				      $event['start_time'] = mktime($event['start_hour'], $event['start_min'], 0, $month, $day+$show_day, $year);
				      $event['end_time'] = $event['start_time'] + $initial_end_time - $initial_start_time;
			
				      if ($event['repeat_every'] > 1)
				      {
								$interval = $event['start_time'] - $event['repeat_start_time'];
								$interval_days = ceil($interval/86400);
								$devided = $interval_days/$event['repeat_every'];
								$match = (int) $devided;
				      }
			
				      if ($event['repeat_every'] < 2 || $devided == $match)
				      {
							  $event['start_year'] = $year;
							  $event['start_month'] = $month;
							  $event['start_day'] = $day+$show_day;
							 
								$this->add_event_to_array($event, $interval_start_time, 
																	$interval_end_time, $timezone_offset, $index_date, 
																	$index_hour, $display_start_hour);
				      }
				    }
				  }
				break;

				case REPEAT_WEEKLY:
				  //loop through all days that are displayed + the last day for 
					//occurences that started outside but end in the display interval
				  for($show_day=-1;$show_day<$show_days;$show_day++)
				  {
				    $day_start_time = mktime(0,0,0,$month, $day+$show_day, $year);
				    $day_end_time = mktime(0,0,0,$month, $day+$show_day+1, $year);
			
				    //does the event occur at the current day to display?
				    if ($event['repeat_start_time'] < $day_end_time && 
								($event['repeat_end_time'] >= $day_start_time || 
								$event['repeat_forever'] == '1'))
				    {
				      $event['start_time'] = mktime($event['start_hour'], $event['start_min'], 0, $month, $day+$show_day, $year);
				      $event['end_time'] = $event['start_time'] + $initial_end_time - $initial_start_time;
			
				      if ($event['repeat_every'] > 1)
				      {
								$interval = $event['start_time'] - $event['repeat_start_time'];
								$interval_weeks = floor($interval/604800);
								$devided = $interval_weeks/$event['repeat_every'];
								$match = (int) $devided;
				      }
			
				      if ($event['repeat_every'] < 2 || $devided == $match)
				      {
								$weekday = date('w', $day_start_time);
						
								//shift the weekdays to local time
								$local_start_hour = date('H',$initial_start_time-($_timezone_offset*3600)) + $_timezone_offset;
								if ($local_start_hour > 23)
								{
								  $local_start_hour = $local_start_hour - 24;
								  $shift_day = 1;
								}elseif($local_start_hour < 0)
								{
								  $local_start_hour = 24 + $local_start_hour;
								  $shift_day = -1;
								}else
								{
								  $shift_day = 0;
								}
						
								switch($shift_day)
								{
								  case 0:
								    $day_db_field[0] = 'sun';
								    $day_db_field[1] = 'mon';
								    $day_db_field[2] = 'tue';
								    $day_db_field[3] = 'wed';
								    $day_db_field[4] = 'thu';
								    $day_db_field[5] = 'fri';
								    $day_db_field[6] = 'sat';
								    break;
						
								  case 1:
								    $day_db_field[0] = 'sat';
								    $day_db_field[1] = 'sun';
								    $day_db_field[2] = 'mon';
								    $day_db_field[3] = 'tue';
								    $day_db_field[4] = 'wed';
								    $day_db_field[5] = 'thu';
								    $day_db_field[6] = 'fri';
						
								    break;
						
								  case -1:
								    $day_db_field[0] = 'mon';
								    $day_db_field[1] = 'tue';
								    $day_db_field[2] = 'wed';
								    $day_db_field[3] = 'thu';
								    $day_db_field[4] = 'fri';
								    $day_db_field[5] = 'sat';
								    $day_db_field[6] = 'sun';
								    break;
								}
								//does it repeat on this weekday?
								if ($event[$day_db_field[$weekday]] == '1')
								{
									$event['start_year'] = $year;
									$event['start_month'] = $month;
									$event['start_day'] = $day+$show_day;
									
									$this->add_event_to_array($event, $interval_start_time, 
																							$interval_end_time, $timezone_offset, 
																							$index_date, $index_hour, 
																							$display_start_hour);
								}				 
				      }
				    }
				  }
				  break;

				case REPEAT_MONTH_DATE:
			
				  //how many months are to be displayed?
				  $first_month = date('m', $interval_start_time+($timezone_offset*3600));
				  $interval_years = date('Y', $interval_end_time)-date('Y', $interval_start_time+($timezone_offset*3600));
				  $interval_months = date('m', $interval_end_time) - date('m', $interval_start_time+($timezone_offset*3600));
				  $show_months = (12*$interval_years)+$interval_months+1;
			
				  //check for an occurence that started last year
				  $process_years = array($year);
				  
				  $real_duration_hours = ($initial_end_time - $initial_start_time)/3600;
		      $duration_hours = ceil($real_duration_hours+($event['start_min']/60));
		      //zero hours should still be displayed in one hour
		      if ($duration_hours == 0)
		      {
						$duration_hours = 1;
		      }				  
				  
				  if (date('Y', mktime(0,0,0,$month, $day, $year)) > date('Y', mktime(0,0,0,$first_month, $day, $year)-($duration_hours*3600)))
				  {
				    $process_years[] = $year-1;
				  }
				  for ($process_year_index=0;$process_year_index<count($process_years);$process_year_index++)
				  {
				    for ($show_month=0;$show_month<$show_months;$show_month++)
				    {
				      //does the event occur at this day?
				      $event['start_day'] = date('j', $initial_start_time);
			
				      $event['start_time'] = mktime($event['start_hour'], $event['start_min'], 0, $first_month+$show_month, $event['start_day'], $process_years[$process_year_index]);
				      $event['end_time'] = $event['start_time'] + $initial_end_time - $initial_start_time;
			
				      if($event['start_time'] < $interval_end_time && $event['end_time'] > $interval_start_time)
				      {
								if ($event['repeat_every'] > 1)
								{
								  $interval_years = $year-date('Y', $event['repeat_start_time']);
								  $interval_months = $first_month+$show_month - date('m', $event['repeat_start_time']);
								  $interval_months = 12*$interval_years+$interval_months;
								  $devided = $interval_months/$event['repeat_every'];
								  $match = (int) $devided;
								}
			
								if ($event['repeat_every'] < 2 || $devided == $match)
								{
									$event['start_year'] = $process_years[$process_year_index];
									$event['start_month'] = $month;
									
									$this->add_event_to_array($event, $interval_start_time, 
																							$interval_end_time, $timezone_offset, 
																							$index_date, $index_hour, 
																							$display_start_hour);
								}
				      }
				    }
				  }			
					break;

				case REPEAT_MONTH_DAY:
				  //loop through all days that are displayed + the last day for 
					//occurences that started outside but end in the display interval
			
				  for($show_day=-1;$show_day<$show_days;$show_day++)
				  {
				    $day_start_time = mktime(0,0,0,$month, $day+$show_day, $year);
				    $day_end_time = mktime(0,0,0,$month, $day+$show_day+1, $year);
			
				    if ($event['repeat_every'] > 1)
				    {
				      $new_year = date('Y', $day_start_time);
				      $new_month = date('n', $day_start_time);
			
				      $interval_years = $new_year-date('Y', $event['repeat_start_time']);
				      $interval_months = $new_month - date('m', $event['repeat_start_time']);
			
				      $interval_months = 12*$interval_years+$interval_months;
			
				      $devided = $interval_months/$event['repeat_every'];
				      $match = (int) $devided;						
				    }
			
				    if ($event['repeat_every'] < 2 || $devided == $match)
				    {
				      $new_day = date('j', $day_start_time);
			
				      $month_time = ceil($new_day/7);
			
				      if ($event['month_time'] == $month_time)
				      {
								$day_start_time = mktime(0,0,0,$month, $day+$show_day, $year);
								$day_end_time = mktime(0,0,0,$month, $day+$show_day+1, $year);
						
								//does the event occur at the current day to display?
								if ($event['repeat_start_time'] < $day_end_time && ($event['repeat_end_time'] >= $day_start_time || $event['repeat_forever'] == '1'))
								{
								  $weekday = date('w', $day_start_time);
								  //shift the weekdays to local time
								  $local_start_hour = date('H',$initial_start_time-($_timezone_offset*3600)) + $_timezone_offset;
								  if ($local_start_hour > 23)
								  {
								    $local_start_hour = $local_start_hour - 24;
								    $shift_day = 1;
								  }elseif($local_start_hour < 0)
								  {
								    $local_start_hour = 24 + $local_start_hour;
								    $shift_day = -1;
								  }else
								  {
								    $shift_day = 0;
								  }
						
								  switch($shift_day)
								  {
								    case 0:
								      $day_db_field[0] = 'sun';
								      $day_db_field[1] = 'mon';
								      $day_db_field[2] = 'tue';
								      $day_db_field[3] = 'wed';
								      $day_db_field[4] = 'thu';
								      $day_db_field[5] = 'fri';
								      $day_db_field[6] = 'sat';
								      break;
						
								    case 1:
								      $day_db_field[0] = 'sat';
								      $day_db_field[1] = 'sun';
								      $day_db_field[2] = 'mon';
								      $day_db_field[3] = 'tue';
								      $day_db_field[4] = 'wed';
								      $day_db_field[5] = 'thu';
								      $day_db_field[6] = 'fri';
						
								      break;
						
								    case -1:
								      $day_db_field[0] = 'mon';
								      $day_db_field[1] = 'tue';
								      $day_db_field[2] = 'wed';
								      $day_db_field[3] = 'thu';
								      $day_db_field[4] = 'fri';
								      $day_db_field[5] = 'sat';
								      $day_db_field[6] = 'sun';
								      break;
						
								  }
						
								  $event['start_time'] = mktime($event['start_hour'], $event['start_min'], 0, $month, $day+$show_day, $year);
								  $event['end_time'] = $event['start_time'] + $initial_end_time - $initial_start_time;
						
								  //does it repeat on this weekday?
								  if ($event[$day_db_field[$weekday]] == '1')
								  {
								  	$event['start_year'] = $year;
										$event['start_month'] = $month;
										$event['start_day'] = $day+$show_day;
										
										$this->add_event_to_array($event, $interval_start_time, 
																								$interval_end_time, $timezone_offset, 
																								$index_date, $index_hour, 
																								$display_start_hour);
									}
								}
							}
				    }
				  }
				  break;

				case REPEAT_YEARLY:
				  //check for an occurence that started last year
				  $process_years = array($year);
				  
				  $real_duration_hours = ($initial_end_time - $initial_start_time)/3600;
		      $duration_hours = ceil($real_duration_hours+($event['start_min']/60));
		      //zero hours should still be displayed in one hour
		      if ($duration_hours == 0)
		      {
						$duration_hours = 1;
		      }
				  
				  
				  if (date('Y', mktime(0,0,0,$month, $day, $year)) > date('Y', mktime(0,0,0,$month, $day, $year)-($duration_hours*3600)))
				  {
				    $process_years[] = $year-1;
				  }
				  for ($process_year_index=0;$process_year_index<count($process_years);$process_year_index++)
				  {
				    if ($event['repeat_every'] > 1)
				    {
				      $interval_years = $process_years[$process_year_index]-date('Y', $event['repeat_start_time']);
			
				      $devided = $interval_years/$event['repeat_every'];
				      $match = (int) $devided;
				    }
			
				    if ($event['repeat_every'] < 2 || $devided == $match)
				    {
				      //does the event occur at this day?
				      $event['start_month'] = date('n', $event['start_time']);
				      $event['start_day'] = date('j', $event['start_time']);
			
				      $event['start_time'] = mktime($event['start_hour'], $event['start_min'], 0, $event['start_month'], $event['start_day'], $process_years[$process_year_index]);
				      $event['end_time'] = $event['start_time'] + $initial_end_time - $initial_start_time;
			
				      if($event['start_time'] < $interval_end_time && $event['end_time'] > $interval_start_time)
				      {
				      	$event['start_year'] = $process_years[$process_year_index];
								
								$this->add_event_to_array($event, $interval_start_time, 
																						$interval_end_time, $timezone_offset, 
																						$index_date, $index_hour, 
																						$display_start_hour);	      
					     }
				    }
				  }
				  break;
      }
    }
  }


  function get_events_to_remind($user_id)
  {
    $gmt_time = get_gmt_time();

    $sql = "SELECT * FROM cal_reminders WHERE user_id='$user_id' AND remind_time<='$gmt_time'";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_event($event_id)
  {
    $sql = "SELECT * FROM cal_events WHERE id='$event_id'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function delete_event($event_id)
  {
    $event = $this->get_event($event_id);

    $sql = "DELETE FROM cal_events WHERE id='$event_id'";
    $this->query($sql);
    $sql = "DELETE FROM cal_events_calendars WHERE event_id='$event_id'";
    $this->query($sql);
    $sql = "DELETE FROM cal_reminders WHERE event_id='$event_id'";
    $this->query($sql);
    $sql = "DELETE FROM cal_participants WHERE event_id='$event_id'";
    return $this->query($sql);
  }

  function delete_user($user_id)
  {
    $delete = new calendar();
    $sql = "SELECT * FROM cal_calendars WHERE user_id='$user_id'";
    $this->query($sql);
    while($this->next_record())
    {
      $delete->delete_calendar($this->f('id'));
    }

    $sql = "SELECT * FROM cal_events WHERE user_id='$user_id'";
    $this->query($sql);

    while($this->next_record())
    {
      $delete->delete_event($this->f('id'));
    }

    $this->get_views($user_id);

    while($this->next_record())
    {
      $delete->delete_view($this->f('id'));
    }    
  }

  
  
  function get_timezones($vcalendar)
  {

  }
  
//TODO: VTODO and attendee support
  function import_ical_file($user_id, $ical_file, $calendar_id)
  {
    global $GO_CONFIG, $GO_SECURITY, $GO_MODULES;
    $count = 0;
    
    $cal_module = $GO_MODULES->get_module('calendar');
    
    if ($calendar = $this->get_calendar($calendar_id) && $cal_module)
    {
		    	
      require_once($cal_module['class_path'].'ical2array.class.inc');
      $ical2array = new ical2array();

      $vcalendar = $ical2array->parse_file($ical_file);
      
      $ical2array->get_timezones($vcalendar);

      while($object = array_shift($vcalendar[0]['objects']))
      {
      	if($object['type'] == 'VEVENT')
      	{
	$name = isset($object['SUMMARY']['value']) ? trim($object['SUMMARY']['value']) : '';
	$description = isset($event['DESCRIPTION']['value']) ? trim($object['DESCRIPTION']['value']) : '';
	$location = isset($object['LOCATION']['value']) ? trim($object['LOCATION']['value']) : '';
	
	$timezone_id = isset($object['DTEND']['params']['TZID']) ? $object['DTEND']['params']['TZID'] : '';
	$start_time = $ical2array->parse_date($object['DTSTART']['value'], $timezone_id);
	
	if(isset($object['DTEND']['value']))
	{		
		$timezone_id = isset($object['DTEND']['params']['TZID']) ? $object['DTEND']['params']['TZID'] : '';
		$end_time = $ical2array->parse_date($object['DTEND']['value'],  $timezone_id);
	}elseif(isset($object['DURATION']['value']))
	{
		$duration = $ical2array->parse_date($object['DURATION']['value']);
		$end_time = $start_time+$duration;
	}	

	if ($name != '' && $start_time > 0 && $end_time > 0)
	{
	  $acl_read = $GO_SECURITY->get_new_acl('calendar_read');
	  $acl_write = $GO_SECURITY->get_new_acl('calendar_write');

	  if ($acl_read > 0 && $acl_write > 0)
	  {
	    $all_day_event = (isset($object['DTSTART']['params']['VALUE']) &&
		strtoupper($object['DTSTART']['params']['VALUE']) == 'DATE') ? true : false;

	    $sun = 0;
	    $mon = 0;
	    $tue = 0;
	    $wed = 0;
	    $thu = 0;
	    $fri = 0;
	    $sat = 0;

	    $repeat_every = 0;
	    $repeat_forever = 0;
	    $repeat_type = REPEAT_NONE;
	    $repeat_end_time = 0;
	    $month_time = 0;

	    if (isset($object['RRULE']['value']))
	    {
	      $rrule = $ical2array->parse_rrule($object['RRULE']['value']);

	      if (isset($rrule['UNTIL']))
	      {
		$repeat_end_time = $ical2array->parse_date($rrule['UNTIL']);
	      }else
	      {
		$repeat_forever = 1;
	      }

	      $repeat_every = $rrule['INTERVAL'];
	      switch($rrule['FREQ'])
	      {
		case 'DAILY':
		  $repeat_type = REPEAT_DAILY;
		  break;

		case 'WEEKLY':
		  $repeat_type = REPEAT_WEEKLY;

		  $days = explode(',', $rrule['BYDAY']);

		  $sun = in_array('SU', $days) ? '1' : '0';
		  $mon = in_array('MO', $days) ? '1' : '0';
		  $tue = in_array('TU', $days) ? '1' : '0';
		  $wed = in_array('WE', $days) ? '1' : '0';
		  $thu = in_array('TH', $days) ? '1' : '0';
		  $fri = in_array('FR', $days) ? '1' : '0';
		  $sat = in_array('SA', $days) ? '1' : '0';
		  break;

		case 'MONTHLY':
		  if (isset($rrule['BYDAY']))
		  {
		    $repeat_type = REPEAT_MONTH_DAY;

		    $month_time = $rrule['BYDAY'][0];
		    $day = substr($rrule['BYDAY'], 1);

		    switch($day)
		    {
		      case 'MO':
			$mon = 1;
			break;

		      case 'TU':
			$tue = 1;
			break;

		      case 'WE':
			$wed = 1;
			break;

		      case 'TH':
			$thu = 1;
			break;

		      case 'FR':
			$fri = 1;
			break;

		      case 'SA':
			$sat = 1;
			break;

		      case 'SUN':
			$sun = 1;
			break;
		    }
		  }else
		  {
		    $repeat_type = REPEAT_MONTH_DATE;
		  }
		  break;

		case 'YEARLY':
		  $repeat_type = REPEAT_YEARLY;
		  break;
	      }
	    }

	    if (isset($object['X-GO-BGCOLOR']['value']))
	    {
	      $bgcolor = $object['X-GO-BGCOLOR']['value'];
	    }else
	    {
	      $bgcolor = 'FFFFCC';
	    }

	    if ($event_id = $this->add_event($user_id, $start_time,
		  $end_time, $all_day_event,
		  addslashes($name), addslashes($description), 0, 0, addslashes($location),
		  $bgcolor, $repeat_type, $repeat_end_time, $month_time, $repeat_forever,
		  $repeat_every, $mon, $tue, $wed, $thu, $fri, $sat, $sun,
		  $acl_read, $acl_write))
	    {
	      $count++;
	      $this->subscribe_event($event_id, $calendar_id);
	      $GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $acl_read);
	      $GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write);
	    }else
	    {
	      $GO_SECURITY->delete_acl($acl_read);
	      $GO_SECURITY->delete_acl($acl_write);
	    }
	  }
	}
				}
      }
    }
    return $count;
  }
}
?>
