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

$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || 
    !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || 
      $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
  $ab_module = false;
}else
{
  require_once ($ab_module['path'].'classes/addressbook.class.inc');
  $ab = new addressbook();	
}

//get the local times
$local_time = get_time();
$year = isset($_REQUEST['year']) ? $_REQUEST['year'] : date("Y", $local_time);
$month = isset($_REQUEST['month']) ? $_REQUEST['month'] : date("m", $local_time);
$day = isset($_REQUEST['day']) ? $_REQUEST['day'] : date("j", $local_time);
$hour = isset($_REQUEST['hour']) ? $_REQUEST['hour'] : date("H", $local_time);
$min = isset($_REQUEST['min']) ? $_REQUEST['min'] : date("i", $local_time);


if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
{
	$dst_offset = $_SESSION['GO_SESSION']['DST'];
}else
{
	$dst_offset = 0;
}

$timezone_offset = $_SESSION['GO_SESSION']['timezone']+$dst_offset;
		  
$hours = array("00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23");
$mins = array("00","05","10","15","20","25","30","35","40","45","50","55");

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('calendar');
require($GO_LANGUAGE->get_language_file('calendar'));

require($GO_MODULES->path.'classes/calendar.class.inc');
$cal = new calendar();

$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$settings = $cal->get_settings($GO_SECURITY->user_id);

$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : $settings['default_cal_id'];
$event_id = isset($_REQUEST['event_id']) ? $_REQUEST['event_id'] : 0;
$send_invitation = isset($_POST['send_invitation']) ? true : false;

if ($task == 'save_event')
{
  $name = smart_addslashes(trim($_POST['name']));
  if ($name == '')
  {
    $feedback = '<p class="Error">'.$error_missing_field.'</p>';
  }elseif(!isset($_POST['calendars']) || count($_POST['calendars']) == 0)
  {
    $feedback = '<p class="Error">'.$sc_select_calendar_please.'</p>';
  }else
  {
    $repeat_forever = isset($_POST['repeat_forever']) ? '1' : '0';
    $repeat_every = isset($_POST['repeat_every']) ? $_POST['repeat_every'] : '0';
    $month_time = isset($_POST['month_time']) ? $_POST['month_time'] : '';

    if (isset($_POST['all_day_event']))
    {
      $all_day_event = '1';
      $start_hour = '0';
      $start_min = '0';
      $end_hour = '0';
      $end_min = '0';

      $start_time = date_to_unixtime($_POST['start_date'].' '.$start_hour.':'.$start_min);
      $end_time= date_to_unixtime($_POST['end_date'].' '.$end_hour.':'.$end_min);
    }else
    {
      $all_day_event = '0';
      $start_min = $_POST['start_min'];
      $start_hour = $_POST['start_hour'];
      $end_hour = $_POST['end_hour'];
      $end_min = $_POST['end_min'];

			$start_time = get_gmt_time(date_to_unixtime($_POST['start_date'].' '.$start_hour.':'.$start_min));
			$end_time = get_gmt_time(date_to_unixtime($_POST['end_date'].' '.$end_hour.':'.$end_min));

    }		

    if ($_POST['repeat_type'] != REPEAT_NONE)
    {
      $repeat_end_time = isset($_POST['repeat_forever']) ? '0' : date_to_unixtime($_POST['repeat_end_date']);
    }else
    {
      $repeat_end_time=0;
    }

    $shift_day=0;
    //shift the selected weekdays to GMT time
    if (!isset($_POST['all_day_event']))
    {
      $shifted_start_hour = $start_hour - $timezone_offset;
      if ($shifted_start_hour > 23)
      {
	$shifted_start_hour = $shifted_start_hour - 24;
	$shift_day = 1;
      }elseif($shifted_start_hour < 0)
      {
	$shifted_start_hour = 24 + $shifted_start_hour;
	$shift_day = -1;
      }
    }
	
    switch($shift_day)
    {
      case 0:
	$mon = isset($_POST['repeat_days_1']) ? '1' : '0';
	$tue = isset($_POST['repeat_days_2']) ? '1' : '0';
	$wed = isset($_POST['repeat_days_3']) ? '1' : '0';
	$thu = isset($_POST['repeat_days_4']) ? '1' : '0';
	$fri = isset($_POST['repeat_days_5']) ? '1' : '0';
	$sat = isset($_POST['repeat_days_6']) ? '1' : '0';
	$sun = isset($_POST['repeat_days_0']) ? '1' : '0';
	break;

      case 1:
	$mon = isset($_POST['repeat_days_0']) ? '1' : '0';
	$tue = isset($_POST['repeat_days_1']) ? '1' : '0';
	$wed = isset($_POST['repeat_days_2']) ? '1' : '0';
	$thu = isset($_POST['repeat_days_3']) ? '1' : '0';
	$fri = isset($_POST['repeat_days_4']) ? '1' : '0';
	$sat = isset($_POST['repeat_days_5']) ? '1' : '0';
	$sun = isset($_POST['repeat_days_6']) ? '1' : '0';

	break;

      case -1:
	$mon = isset($_POST['repeat_days_2']) ? '1' : '0';
	$tue = isset($_POST['repeat_days_3']) ? '1' : '0';
	$wed = isset($_POST['repeat_days_4']) ? '1' : '0';
	$thu = isset($_POST['repeat_days_5']) ? '1' : '0';
	$fri = isset($_POST['repeat_days_6']) ? '1' : '0';
	$sat = isset($_POST['repeat_days_0']) ? '1' : '0';
	$sun = isset($_POST['repeat_days_1']) ? '1' : '0';
	break;
    }

    if ($event_id > 0)
    {
      if (!$cal->update_event($event_id, $start_time,
	    $end_time, $all_day_event, $name,
	    smart_addslashes($_POST['description']),
	    $_POST['contact_id'], $_POST['reminder'],
	    smart_addslashes($_POST['location']),
	    $_POST['background'], $_POST['repeat_type'],
	    $repeat_end_time, $month_time, $repeat_forever,
	    $repeat_every,  $mon, $tue, $wed, $thu, $fri,
	    $sat, $sun))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
	

	if($event = $cal->get_event($event_id))
	{
	  switch($_POST['permissions'])
	  {
	    case 'everybody_read':
	      $GO_SECURITY->delete_group_from_acl($GO_CONFIG->group_everyone, $event['acl_write']);
	      if(!$GO_SECURITY->group_in_acl($GO_CONFIG->group_everyone, $event['acl_read']))
	      {
		$GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $event['acl_read']);
	      }
	      break;

	    case 'everybody_write':
	      $GO_SECURITY->delete_group_from_acl($GO_CONFIG->group_everyone, $event['acl_read']);
	      if(!$GO_SECURITY->group_in_acl($GO_CONFIG->group_everyone, $event['acl_write']))
	      {
		$GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $event['acl_write']);
	      }
	      break;

	    case 'private':
	      $GO_SECURITY->delete_group_from_acl($GO_CONFIG->group_everyone, $event['acl_write']);
	      $GO_SECURITY->delete_group_from_acl($GO_CONFIG->group_everyone, $event['acl_read']);
	      break;			
	  }
	}
      }

    }else
    {
      $acl_read = $GO_SECURITY->get_new_acl('Event read: '.$event_id);
      $acl_write = $GO_SECURITY->get_new_acl('Event read: '.$event_id);

      if (!$acl_read || !$acl_write ||
	  !$event_id = $cal->add_event($GO_SECURITY->user_id, $start_time,
	    $end_time, $all_day_event, $name,
	    smart_addslashes($_POST['description']),
	    $_POST['contact_id'],
	    $_POST['reminder'],
	    smart_addslashes($_POST['location']),
	    $_POST['background'],
	    $_POST['repeat_type'], $repeat_end_time,
	    $month_time, $repeat_forever,
	    $repeat_every, $mon, $tue, $wed, $thu,
	    $fri, $sat, $sun,
	    $acl_read, $acl_write))
      {
	$GO_SECURITY->delete_acl($acl_read);
	$GO_SECURITY->delete_acl($acl_write);
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
	$GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write);	
	switch($_POST['permissions'])
	{
	  case 'everybody_read':
	    $GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $acl_read);
	    break;

	  case 'everybody_write':
	    $GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $acl_write);
	    break;
	}				
      }
    }
    if (!isset($feedback))
    {				
      //enter the event in all selected calendars
      $cal2 = new calendar();
      $cal->get_authorised_calendars($GO_SECURITY->user_id);
      while ($cal->next_record())
      {
	if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $cal->f('acl_write')))
	{
	  if (in_array($cal->f('id'), $_POST['calendars']))
	  {							
	    if (!$cal2->event_is_subscribed($event_id, $cal->f('id')))
	    {
		$cal2->subscribe_event($event_id, $cal->f('id'));
	    }
	  }else
	  {
	    if ($cal2->event_is_subscribed($event_id, $cal->f('id')))
	    {
	      $cal2->unsubscribe_event($event_id, $cal->f('id'));
	    }
	  }
	}
      }

      //set the reminder
      if ($_POST['reminder'] > 0)
      {
	$next_recurrence_time = $cal->get_next_recurrence_time($event_id);

	$remind_time = $next_recurrence_time - $_POST['reminder'];
	$cal->insert_reminder($GO_SECURITY->user_id, $event_id, $remind_time);
      }

      if (trim($_POST['to']) != '' && $send_invitation)
      {
		//remove participants and add them if invitation is sent succesfully  
		$cal->remove_participants($event_id);  
	$send_invitation = false;
	//send an invitation mail to all participants
	$participants = cut_address($_POST['to'], $charset);

	$mail_body  = '<html><body>'.$sc_invited.'<br /><br />';
	$mail_body  .= '<table border="0"><tr><td>'.$sc_title.':</td><td>'.$name.'</td></tr>';

	if ($_POST['contact_id'] > 0)
	{
	  $mail_body  .= '<tr><td>'.$sc_client.':</td>';
	  $mail_body  .= '<td>'.show_contact($_POST['contact_id']).'</td></tr>';
	}
	if ($_POST['description'] != '')
	{
	  $mail_body  .= '<tr><td valign="top">'.$strDescription.':</td>';
	  $mail_body  .='<td>'.text_to_html($_POST['description']).'</td></tr>';
	}

	if ($_POST['location'] != '')
	{
	  $mail_body  .= '<tr><td>'.$sc_location.':</td>';
	  $mail_body  .= '<td>'.text_to_html($_POST['location']).'</td></tr>';
	}

	$mail_body  .= '<tr><td>'.$sc_type.':</td>';
	$mail_body  .= '<td>'.$sc_types[$_POST['repeat_type']].'</td></tr>';

	//don't calculate timezone offset for all day events
	$timezone_offset_string = isset($_POST['all_day_event']) ? 0 : $timezone_offset;
	if ($timezone_offset > 0)
	{
	  $gmt_string = '\G\M\T +'.$timezone_offset_string;
	}elseif($timezone_offset < 0)
	{
	  $gmt_string = '\G\M\T -'.$timezone_offset_string;
	}else
	{
	  $gmt_string = '\G\M\T';
	}

	if (isset($_POST['all_day_event']))
	{
	  $event_datetime_format = $_SESSION['GO_SESSION']['date_format'];
	}else
	{
	  $event_datetime_format = $_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'].' '.$gmt_string;
	}
	$event_time_format = $_SESSION['GO_SESSION']['time_format'].' '.$gmt_string;

	switch($_POST['repeat_type'])
	{
	  case REPEAT_NONE:

	    $mail_body  .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	    if ($end_time != $start_time)
	    {
	      $mail_body  .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	    }
	    break;

	  case REPEAT_WEEKLY:
	    if(!isset($_POST['all_day_event']))
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_time_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      if ($end_time != $start_time)
	      {
		$mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_time_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	      }
	    }

	    $mail_body .= '<tr><td>'.$sc_at_days.':</td><td>';

	    $local_start_hour = date('H',$start_time-$timezone_offset) + ($timezone_offset/3600);
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

	    $event['days'] = array();
	    if (isset($_POST['repeat_days_0']))
	    {
	      $event['days'][] = $full_days[0+$shift_day];
	    }
	    if (isset($_POST['repeat_days_1']))
	    {
	      $event['days'][] = $full_days[1+$shift_day];
	    }

	    if (isset($_POST['repeat_days_2']))
	    {
	      $event['days'][] = $full_days[2+$shift_day];
	    }

	    if (isset($_POST['repeat_days_3']))
	    {
	      $event['days'][] = $full_days[3+$shift_day];
	    }

	    if (isset($_POST['repeat_days_4']))
	    {
	      $event['days'][] = $full_days[4+$shift_day];
	    }

	    if (isset($_POST['repeat_days_5']))
	    {
	      $event['days'][] = $full_days[5+$shift_day];
	    }

	    if (isset($_POST['repeat_days_6']))
	    {
	      $event['days'][] = $full_days[6]+$shift_day;
	    }
	    $mail_body .= implode(', ', $event['days']);

	    $mail_body .= '</td></tr>';

	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if ($repeat_forever == '1')
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }
	    $mail_body .= '</td></tr>';

	    break;

	  case REPEAT_DAILY:
	    if(!isset($_POST['all_day_event']))
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      $mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	    }
	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if (isset($repeat_forever))
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }
	    $mail_body .= '</td></tr>';
	    break;

	  case REPEAT_MONTH_DATE:	
	    if(!isset($_POST['all_day_event']))
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      if ($end_time != $start_time)
	      {
		$mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	      }
	    }else
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.$sc_day.' '.date('d', $start_time).'</td></tr>';
	      $mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.$sc_day.' '.date('d', $start_time).'</td></tr>';
	    }

	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if ($repeat_forever == '1')
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }	
	    break;

	  case REPEAT_MONTH_DAY:
	    if(!isset($_POST['all_day_event']))
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      if (isset($repeat_forever))
	      {
		$mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	      }
	    }

	    $local_start_hour = date('H',$start_time-$timezone_offset) + ($timezone_offset/3600);
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

	    if (isset($_POST['repeat_days_0']))
	    {
	      $event['days'][] = $full_days[0+$shift_day];
	    }
	    if (isset($_POST['repeat_days_1']))
	    {
	      $event['days'][] = $full_days[1+$shift_day];
	    }

	    if (isset($_POST['repeat_days_2']))
	    {
	      $event['days'][] = $full_days[2+$shift_day];
	    }

	    if (isset($_POST['repeat_days_3']))
	    {
	      $event['days'][] = $full_days[3+$shift_day];
	    }

	    if (isset($_POST['repeat_days_4']))
	    {
	      $event['days'][] = $full_days[4+$shift_day];
	    }

	    if (isset($_POST['repeat_days_5']))
	    {
	      $event['days'][] = $full_days[5+$shift_day];
	    }

	    if (isset($_POST['repeat_days_6']))
	    {
	      $event['days'][] = $full_days[6]+$shift_day;
	    }
	    $mail_body .= implode(', ', $event['days']);

	    $mail_body .= '</td></tr>';

	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if ($repeat_forever == '1')
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }


	    break;

	    case REPEAT_YEARLY;
	    if(!isset($_POST['all_day_event']))
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      if ($end_time != $start_time)
	      {
		$mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	      }
	    }else
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.$sc_day.' '.date('d', $start_time).'</td></tr>';
	      $mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.$sc_day.' '.date('d', $start_time).'</td></tr>';
	    }

	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if ($repeat_forever == '1')
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }

	    break;
	}

	$mail_body .= '</table><br /><br />'.$sc_accept_question.'<br /><br />';	
	
	require($GO_CONFIG->class_path."phpmailer/class.phpmailer.php");
	require($GO_CONFIG->class_path."phpmailer/class.smtp.php");
	$mail = new PHPMailer();
  $mail->PluginDir = $GO_CONFIG->class_path.'phpmailer/';
  $mail->SetLanguage($php_mailer_lang, $GO_CONFIG->class_path.'phpmailer/language/');

  switch($GO_CONFIG->mailer)
  {
	  case 'smtp':
			$mail->Host = $GO_CONFIG->smtp_server;
			$mail->Port = $GO_CONFIG->smtp_port;
			$mail->IsSMTP();			  
		break;			
	  case 'qmail':
			$mail->IsQmail();
		break;			
	  case 'sendmail':
			$mail->IsSendmail();
		break;
	  case 'mail':
			$mail->IsMail();
		break;
  }

  $mail->Sender     = $_SESSION['GO_SESSION']["email"];    
  $mail->From     = $_SESSION['GO_SESSION']["email"];
//  $mail->FromName = $name;
  $mail->FromName = $_SESSION['GO_SESSION']["name"];
  $mail->AddReplyTo($_SESSION['GO_SESSION']["email"],$_SESSION['GO_SESSION']["name"]);
  $mail->WordWrap = 50;
  $mail->IsHTML(true);
  $mail->Subject = $name;
  
  require_once($GO_MODULES->class_path.'go_ical.class.inc');
	$ical = new go_ical();

	$ics_string = $ical->export_event($event_id);
	if($ics_string)
	{  
  	$mail->AddStringAttachment($ics_string, $name.'.ics', 'base64','text/calendar');
  }

	for ($i=0;$i<sizeof($participants);$i++)
	{
	  $id = 0;
	  if ($ab_module)
	  {
	    $user_profile = $ab->get_contact_profile_by_email(smart_addslashes($participants[$i]), $GO_SECURITY->user_id);
	    $id = $user_profile["source_id"];
	  }else
	  {
	    $user_profile = false;
	  }
	  if (!$user_profile)
	  {
	    $user_profile = $GO_USERS->get_profile_by_email(smart_addslashes($participants[$i]));
	    $id = $user_profile["id"];
	  }

	  if ($user_profile)
	  {
	    $middle_name = $user_profile['middle_name'] == '' ? '' : $user_profile['middle_name'].' ';
	    $profile_name = $user_profile['first_name'].' '.$middle_name.$user_profile['last_name'];
	  }else
	  {
	    $profile_name = $participants[$i];
	  }

	  if ($id == 0)
	  {
/*	    $nouser_link = '<p><a href="'.$GO_MODULES->full_url.
	      'accept.php?event_id='.$event_id.
	      '&member=false&email='.$participants[$i].
	      '" class="blue">'.$sc_accept.'</a>&nbsp|&nbsp;<a href="'.
	      $GO_MODULES->url.'decline.php?event_id='.
	      $event_id.'&member=false&email='.$participants[$i].
	      '" class="blue">'.$sc_decline.'</a></p>';
*/	     
		$nouser_link = '<p><a href="'.$GO_MODULES->full_url.
	      'accept.php?event_id='.$event_id.
	      '&member=false&email='.$participants[$i].
	      '" class="blue">'.$sc_accept.'</a>&nbsp|&nbsp;<a href="'.
	      $GO_MODULES->full_url.'decline.php?event_id='.
	      $event_id.'&member=false&email='.$participants[$i].
	      '" class="blue">'.$sc_decline.'</a></p>';
			 $mail->Body = $mail_body.$nouser_link;

	     $mail->ClearAllRecipients();
	     $mail->AddAddress($participants[$i]);

	    if ($mail->Send())
	    {
	      $cal->add_participant($event_id, smart_addslashes($participants[$i]));
	    }
	  }else
	  {
/*	    $user_link = '<p class="cmd"><a href="'.$GO_CONFIG->full_url.
	      'index.php?return_to='.	urlencode($GO_MODULES->url.
		  'accept.php?event_id='.$event_id.'&member=true&email='.
		  $participants[$i]).'" class="blue">'.$sc_accept.
	      '</a>&nbsp|&nbsp;<a href="'.$GO_CONFIG->full_url.
	      'index.php?return_to='.urlencode($GO_MODULES->url.
		  'decline.php?event_id='.$event_id.'&member=true&email='.
		  $participants[$i]).'" class="blue">'.$sc_decline.'</a></p>';
*/
		$user_link = '<p class="cmd"><a href="'.$GO_CONFIG->full_url.
	      'index.php?return_to='.	urlencode($GO_MODULES->full_url.
		  'accept.php?event_id='.$event_id.'&member=true&email='.
		  $participants[$i]).'" class="blue">'.$sc_accept.
	      '</a>&nbsp|&nbsp;<a href="'.$GO_CONFIG->full_url.
	      'index.php?return_to='.urlencode($GO_MODULES->full_url.
		  'decline.php?event_id='.$event_id.'&member=true&email='.
		  $participants[$i]).'" class="blue">'.$sc_decline.'</a></p>';
		  
	    if ($GO_SECURITY->user_id != $id)
	    {
	    	$mail->Body = $mail_body.$user_link;
	     	$mail->ClearAllRecipients();
	     	$mail->AddAddress($participants[$i]);
	      if ($mail->Send())
	      {
		$cal->add_participant($event_id, addslashes($user_profile["email"]), $id);
	      }
	    }else
	    {
	      $cal->add_participant($event_id,  addslashes($user_profile["email"]), $id);
	      $cal->set_event_status($event_id, '1', $user_profile["email"]);
	    }
	  }
	}
      }
      if ($_POST['close'] == 'true')
      {
	header('Location: '.$return_to);
	exit();
      }else
      {
	$task = '';
      }
    }
  }
}

if ($event_id > 0 && $task != 'save_event')
{
  //get the event
  $event = $cal->get_event($event_id);

  if (!$event['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $event['acl_write']))
  {
    header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
    exit();
  }

  //populate an address string of the participants
  $event['to'] = '';
  $cal->get_participants($event_id);
  while ($cal->next_record())
  {
    if ($event['to'] == '')
    {
      $event['to'] = $cal->f("email");
    }else
    {
      $event['to'] .= ', '.$cal->f("email");
    }
  }

  //don't calculate timezone offset for all day events
  $timezone_offset = ($event['all_day_event'] == '0') ? ($timezone_offset*3600) : 0;

  $gmt_start_time = $event['start_time'];
  $event['start_time'] += $timezone_offset;
  $event['start_hour'] = date('G', $event['start_time']);
  $event['start_min'] = date('i', $event['start_time']);

  $event['end_time'] += $timezone_offset;
  $event['end_hour'] = date('G', $event['end_time']);
  $event['end_min'] = date('i', $event['end_time']);

  $event['start_date'] = date($_SESSION['GO_SESSION']['date_format'], $event['start_time']);
  $event['end_date'] = date($_SESSION['GO_SESSION']['date_format'], $event['end_time']);

  $event['repeat_end_date'] = date($_SESSION['GO_SESSION']['date_format'], $event['repeat_end_time']);

  if ($event['repeat_type'] != REPEAT_NONE)
  {
    if ($event['repeat_forever'] == '0')
    {
      $event['repeat_end_date'] = date($_SESSION['GO_SESSION']['date_format'], $event['repeat_end_time']);
    }else
    {
      $event['repeat_end_date'] = date($_SESSION['GO_SESSION']['date_format'], $event['end_time']);
    }
  }else
  {
    $event['repeat_end_date'] = date($_SESSION['GO_SESSION']['date_format'], $event['start_time']);
  }

  //to what calendars is this event subscribed?
  $event['calendars'] = array();
  $cal->get_event_subscribtions($event_id);
  while($cal->next_record())
  {
    $event['calendars'][] = $cal->f('calendar_id');
  }

  //shift the selected weekdays to local time
  $local_start_hour = date("G", $gmt_start_time) + ($timezone_offset/3600);
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
    case 1:
      $mon = $event['sun'] == '1' ? '1' : '0';
      $tue = $event['mon'] == '1' ? '1' : '0';
      $wed = $event['tue'] == '1' ? '1' : '0';
      $thu = $event['wed'] == '1' ? '1' : '0';
      $fri = $event['thu'] == '1' ? '1' : '0';
      $sat = $event['fri'] == '1' ? '1' : '0';
      $sun = $event['sat'] == '1' ? '1' : '0';
      break;

    case -1:
      $mon = $event['tue'] == '1' ? '1' : '0';
      $tue = $event['wed'] == '1' ? '1' : '0';
      $wed = $event['thu'] == '1' ? '1' : '0';
      $thu = $event['fri'] == '1' ? '1' : '0';
      $fri = $event['sat'] == '1' ? '1' : '0';
      $sat = $event['sun'] == '1' ? '1' : '0';
      $sun = $event['mon'] == '1' ? '1' : '0';
      break;

  }

  if ($shift_day != 0)
  {
    $event['sun'] = $sun;
    $event['mon'] =	$mon;
    $event['tue'] =	$tue;
    $event['wed'] =	$wed;
    $event['thu'] =	$thu;
    $event['fri'] =	$fri;
    $event['sat'] =	$sat;
  }

  $title = $event['name'];

  if($GO_SECURITY->group_in_acl($GO_CONFIG->group_everyone, $event['acl_read']))
  {
    $event['permissions'] = 'everybody_read';
  }elseif($GO_SECURITY->group_in_acl($GO_CONFIG->group_everyone, $event['acl_write']))
  {
    $event['permissions'] = 'everybody_write';
  }else
  {
    $event['permissions'] = 'private';
  }	
}else
{
  $title = $sc_new_app;
  $requested_time=mktime($hour,0,0,$month, $day, $year);
  $requested_date=date($_SESSION['GO_SESSION']['date_format'], $requested_time);
  //new event declare all vars
  $event['calendars'] = isset($_POST['calendars']) ? $_POST['calendars'] : array();
  $event['description'] = isset($_POST['description']) ? smartstrip($_POST['description']) : '';
  $event['name'] = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
  $event['to'] = isset($_POST['to']) ? smartstrip($_POST['to']) : '';
  $event['contact_id'] = isset($_REQUEST['contact_id']) ? $_REQUEST['contact_id'] : '';

  $event['start_date'] = isset($_POST['start_date']) ? smartstrip($_POST['start_date']) : $requested_date;
  $tmp = (strlen($hour) == 1) ? '0'.$hour : $hour;

  $event['start_hour'] = isset($_POST['start_hour']) ? $_POST['start_hour'] : $tmp;
  $event['start_min'] = isset($_POST['start_min']) ? $_POST['start_min'] : '00';

  $event['end_date'] = isset($_POST['end_date']) ? $_POST['end_date'] : $requested_date;
  $tmp = (strlen($hour+1) == 1) ? '0'.$hour+1 : $hour+1;
  $event['end_hour'] = isset($_POST['end_hour']) ? $_POST['end_hour'] : $tmp;
  $event['end_min'] = isset($_POST['end_min']) ? $_POST['end_min'] : '00';

  $event['repeat_end_date'] = isset($_POST['repeat_end_date']) ? $_POST['repeat_end_date'] : $requested_date;

  $event['repeat_type'] = isset($_POST['repeat_type']) ? $_POST['repeat_type'] : REPEAT_NONE;
  $event['all_day_event'] = isset($_POST['all_day_event']) ? $_POST['all_day_event'] :'0';
  $event['repeat_forever'] = isset($_POST['repeat_forever']) ? $_POST['repeat_forever'] :'0';
  $event['repeat_every'] = isset($_POST['repeat_every']) ? $_POST['repeat_every'] :'0';
  $event['month_time'] = isset($_POST['month_time']) ? $_POST['month_time'] : '0';

  $event['sun'] = isset($_POST['repeat_days_0']) ? true : false;
  $event['mon'] = isset($_POST['repeat_days_1']) ? true : false;
  $event['tue'] = isset($_POST['repeat_days_2']) ? true : false;
  $event['wed'] = isset($_POST['repeat_days_3']) ? true : false;
  $event['thu'] = isset($_POST['repeat_days_4']) ? true : false;
  $event['fri'] = isset($_POST['repeat_days_5']) ? true : false;
  $event['sat'] = isset($_POST['repeat_days_6']) ? true : false;
  $event['reminder'] = isset($_POST['reminder']) ? $_POST['reminder'] :'0';
  $event['background'] = isset($_POST['background']) ? $_POST['background'] :'FFFFCC';
  $event['location'] = isset($_POST['location']) ? smartstrip($_POST['location']) :'';
  $event['permissions'] = isset($_POST['permissions']) ? $_POST['permissions'] : 'everybody_read';
}

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();
require($GO_THEME->theme_path.'header.inc');

if ($ab_module)
{
  $ab->enable_contact_selector();
}

echo '<form name="event_form" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="calendar_id" value="'.$calendar_id.'" />';
echo '<input type="hidden" name="event_id" value="'.$event_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

//address_string used by the addressbok selector
echo '<input type="hidden" name="address_string" value="" />';

$tabtable = new tabtable('event_table', htmlspecialchars($title), '600', '400', '120', '', true, 'left', 'top', 'event_form');
if ($event_id > 0)
{

  if ($cal->get_participants($event_id))
  {
    $tabtable->add_tab('properties', $strProperties);
    $tabtable->add_tab('participants', $sc_participants);
  }
}
$tabtable->print_head();
echo '<br />';
switch($tabtable->get_active_tab_id())
{
  case 'participants':
    echo '<input type="hidden" name="status" />';
    echo '<table border="0">';
    echo '<tr><td><h3>'.$strName.'</td>';
	echo '<td><h3>'.$strEmail.'</td>';
    echo '<td><h3>'.$sc_status.'</td></tr>';

    while ($cal->next_record())
    {
      echo '<tr><td nowrap>';
      if($cal->f('user_id') > 0)
      {
	echo show_profile($cal->f('user_id'),'','normal',$link_back);
      }else
      {
	echo show_profile_by_email($cal->f('email'),'',$link_back).'&nbsp;</td>';
      }
      echo '<td nowrap>'.mail_to($cal->f('email')).'&nbsp;</td><td>';
      switch($cal->f('status'))
      {
	case '0':
	  echo $sc_not_responded;
	  break;

	case '1':
	  echo $sc_accepted;
	  break;

	case '2':
	  echo $sc_declined;
	  break;

      }
      echo '</td></tr>';
    }
    echo '</table>';
    $status = $cal->get_event_status($event_id, $_SESSION['GO_SESSION']['email']);
    if($status !== false)
    {
      echo '<br />';
      switch ($status)
      {
	case '0';
	$button = new button($sc_accept, "javascript:document.location='".$_SERVER['REQUEST_URI']."&status=1'");
	echo '&nbsp;&nbsp;';
	$button = new button($sc_decline, "javascript:document.location='".$_SERVER['REQUEST_URI']."&status=2'");
	break;

	case '1';
	$button = new button($sc_decline, "javascript:document.location='".$_SERVER['REQUEST_URI']."&status=2'");
	break;

	case '2';
	$button = new button($sc_accept, "javascript:document.location='".$_SERVER['REQUEST_URI']."&status=1'");
	break;
      }
    }
    echo '<br /><br />';
    $button = new button($cmdClose,"javascript:document.location='".$return_to."'");
    break;
  case 'read_permissions':
    $read_only = ($event['user_id'] == $GO_SECURITY->user_id) ? false : true;
    print_acl($event['acl_read'], $read_only);
    echo '<br /><br />';
    $button = new button($cmdCancel,"javascript:document.location='".$return_to."'");
    break;

  case 'write_permissions':
    $read_only = ($event['user_id'] == $GO_SECURITY->user_id) ? false : true;
    print_acl($event['acl_write'], $read_only);
    echo '<br /><br />';
    $button = new button($cmdCancel,"javascript:document.location='".$return_to."'");
    break;

  default:

    echo '<table border="0" cellpadding="2" cellspacing="0">';
    if (isset($feedback))
      echo '<tr><td colspan="2" class="Error">'.$feedback.'</td></tr>';
    echo '<tr><td>'.$strName.':&nbsp;</td><td><input type="text" class="textbox" maxlength="50" name="name" style="width: 300px;" value="'.htmlspecialchars($event['name']).'" /></td></tr>';
    echo '<tr><td>';

    if ($ab_module)
    {
      $link = $ab->select_contacts('document.event_form.to', $GO_MODULES->url.'add_contacts.php');

      echo 	"<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr><td>".
	"<a class=\"normal\" href=\"".$link."\"><img src=\"".
	$GO_THEME->images['addressbook_small']."\" width=\"16\" ".
	"height=\"16\" border=\"0\" /></a>&nbsp;</td><td>".
	'<a class="normal" href="'.$link.'">'.$sc_participants.'</a>:&nbsp;</td></tr></table></td>';
    }else
    {
      echo $sc_participants.":&nbsp;";
    }
    echo '<td nowrap><input type="text" class="textbox" name="to" value="'.htmlspecialchars($event['to']).'" style="width: 300px;" />';

    if ($event_id > 0)
    {
      $checkbox = new checkbox('send_invitation', 'true', $cal_resend_invitation, $send_invitation);
    }else
    {
      echo '<input type="hidden" name="send_invitation" value="true" />';
    }

    echo '</td></tr>';

    if($ab_module)
    {
      $select = new select('contact', 'event_form', 'contact_id', $event['contact_id']);
      echo '<tr><td>';
      $select->print_link($sc_client);
      echo ':</td><td>';
      $select->print_field();
      echo '</td></tr>';

    }else
    {
      echo '<input type="hidden" value="0" name="contact_id" />';
    }
    echo '<tr><td>'.$sc_location.':</td>'.
      '<td><input type="text" class="textbox" style="width: 300px;"'.
      'name="location" value="'.htmlspecialchars($event['location']).
      '" /></td></tr>'.
      '<tr><td valign="top">'.$sc_description.':&nbsp;</td><td>'.
      '<textarea class="textbox" name="description" cols="60" rows="4">'.
      htmlspecialchars($event['description']).'</textarea></td></tr>'.
      '<tr><td colspan="2">&nbsp;</td></tr>';

    echo '<tr><td>'.$sc_start_at.':&nbsp;</td><td>';
    echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td>';

    $datepicker->print_date_picker('start_date', $_SESSION['GO_SESSION']['date_format'], $event['start_date'], '', '', 'onchange="javascript:document.event_form.end_date.value=this.value;document.event_form.repeat_end_date.value=this.value;"');

    echo '</td><td>&nbsp;&nbsp;';

    $dropbox = new dropbox();
    $dropbox->add_arrays($hours, $hours);
    $dropbox->print_dropbox("start_hour", $event['start_hour'], 'onchange="javascript:update_end_hour(this.value);"');
    echo '&nbsp;:&nbsp;';
    $dropbox = new dropbox();
    $dropbox->add_arrays($mins, $mins);
    $dropbox->print_dropbox("start_min", $event['start_min'], 'onchange="javascript:document.event_form.end_min.value=this.value;"');

    echo '</td></tr></table>';
    echo '</td></tr>';

    echo '<tr><td>'.$sc_end_at.':&nbsp;</td><td>';

    echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td>';
    $datepicker->print_date_picker('end_date', $_SESSION['GO_SESSION']['date_format'], $event['end_date']);
    echo '</td><td>&nbsp;&nbsp;';
    $dropbox = new dropbox();
    $dropbox->add_arrays($hours, $hours);
    $dropbox->print_dropbox("end_hour", $event['end_hour']);
    echo '&nbsp;:&nbsp;';
    $dropbox = new dropbox();
    $dropbox->add_arrays($mins, $mins);
    $dropbox->print_dropbox("end_min", $event['end_min']);
    echo '</td></tr></table>';
    echo '</td></tr>';

    $all_day_event = ($event['all_day_event'] == '1') ? true : false;

    echo '<tr><td>&nbsp;</td><td>';
    $checkbox = new checkbox('all_day_event', '1', $sc_notime, $all_day_event, false, 'onclick="javascript:disable_time()"');
    echo '</td></tr>';

    echo '<tr><td colspan="2">&nbsp;</td></tr>';

    echo '<tr><td>'.$sc_recur_every.':</td><td>';

    $dropbox = new dropbox();
    for ($i=1;$i<11;$i++)
    {
      $dropbox->add_value($i, $i);
    }
    $dropbox->print_dropbox('repeat_every', $event['repeat_every']);
    $dropbox = new dropbox();
    $dropbox->add_value('0', $sc_types1[REPEAT_NONE]);
    $dropbox->add_value('1', $sc_types1[REPEAT_DAILY]);
    $dropbox->add_value('2', $sc_types1[REPEAT_WEEKLY]);
    $dropbox->add_value('3', $sc_types1[REPEAT_MONTH_DATE]);
    $dropbox->add_value('4', $sc_types1[REPEAT_MONTH_DAY]);
    $dropbox->add_value('5', $sc_types1[REPEAT_YEARLY]);
    $dropbox->print_dropbox('repeat_type', $event['repeat_type'], 'onclick="javascript:toggle_repeat(this.value);"');

    echo '</td></tr>';

    echo '<tr><td>'.$sc_at_days.':</td><td>';
    echo '<table border="0" cellpadding="0" cellspacing="0"><tr>';
    echo '<td>';
    $dropbox = new dropbox();
    $dropbox->add_arrays(array(1,2,3,4), $month_times);
    $dropbox->print_dropbox("month_time", $event['month_time']);
    echo '</td>';


    $day_data_field[0] = 'sun';
    $day_data_field[1] = 'mon';
    $day_data_field[2] = 'tue';
    $day_data_field[3] = 'wed';
    $day_data_field[4] = 'thu';
    $day_data_field[5] = 'fri';
    $day_data_field[6] = 'sat';

    $day_number = $_SESSION['GO_SESSION']['first_weekday'];

    for ($i=0;$i<7;$i++)
    {
      if ($day_number == 7) $day_number = 0;
      echo '<td>';
      $checkbox = new checkbox('repeat_days_'.$day_number, '1', $days[$day_number], $event[$day_data_field[$day_number]]);
      echo '</td>';
      $day_number++;
    }
    echo '</tr></table></td></tr>';
    echo '<tr><td>'.$sc_cycle_end.':&nbsp;</td><td>';
    $datepicker->print_date_picker('repeat_end_date', $_SESSION['GO_SESSION']['date_format'], $event['repeat_end_date']);

    $repeat_forever = $event['repeat_forever'] == '1' ? true : false;
    $checkbox = new checkbox('repeat_forever', '1', $sc_noend, $repeat_forever, false, 'onclick="javascript:toggle_repeat_end_info()"');
    echo '</td></tr>';

    echo '<tr><td colspan="2">&nbsp;</td></tr>';

    echo '<tr><td>'.$sc_reminder.':</td><td>';

    $dropbox=new dropbox();
    $dropbox->add_value('0', ' ');
    $dropbox->add_value('300', '5 '.$sc_mins);
    $dropbox->add_value('900', '15 '.$sc_mins);
    $dropbox->add_value('1800', '30 '.$sc_mins);
    $dropbox->add_value('2700', '45 '.$sc_mins);
    $dropbox->add_value('3600', '1 '.$sc_hour);
    $dropbox->add_value('7200', '2 '.$sc_hours);
    $dropbox->add_value('10800', '3 '.$sc_hours);
    $dropbox->add_value('14400', '4 '.$sc_hours);
    $dropbox->add_value('18000', '5 '.$sc_hours);
    $dropbox->add_value('21600', '6 '.$sc_hours);
    $dropbox->add_value('25200', '7 '.$sc_hours);
    $dropbox->add_value('28800', '8 '.$sc_hours);
    $dropbox->add_value('32400', '9 '.$sc_hours);
    $dropbox->add_value('36000', '10 '.$sc_hours);
    $dropbox->add_value('39600', '11 '.$sc_hours);
    $dropbox->add_value('43200', '12 '.$sc_hours);
    $dropbox->add_value('86400', '1 '.$sc_day);
    $dropbox->add_value('172800', '2 '.$sc_days);
    $dropbox->add_value('259200', '3 '.$sc_days);
    $dropbox->add_value('345600', '4 '.$sc_days);
    $dropbox->add_value('432000', '5 '.$sc_days);
    $dropbox->add_value('518400', '6 '.$sc_days);
    $dropbox->add_value('604800', '1 '.$sc_week);
    $dropbox->add_value('1209600', '2 '.$sc_weeks);
    $dropbox->add_value('1814400', '3 '.$sc_weeks);
    $dropbox->print_dropbox('reminder', $event['reminder']);

    echo '</td></tr>';
    echo '<tr><td colspan="2">&nbsp;</td></tr>';

    echo '<tr><td nowrap>'.$sc_background.':&nbsp;</td><td>';
    $color_selector = new color_selector();
    $color_selector->add_color('FFFFCC');
    $color_selector->add_color('FF6666');
    $color_selector->add_color('CCFFCC');
    $color_selector->add_color('99CCFF');
    $color_selector->add_color('FF99FF');
    $color_selector->add_color('FFCC66');
    $color_selector->add_color('CCCC66');
    $color_selector->add_color('F1F1F1');
    $color_selector->add_color('FFCCFF');
    $color_selector->print_color_selector('background', $event['background'], 'event_form');
    echo '</td></tr>';

    $permissions = isset($_POST['permissions']) ? $_POST['permissions'] : 'everybody_read';
    echo '<tr><td valign="top">'.$strPermissions.':</td><td>';

    $radio_list = new radio_list('permissions', $event['permissions']);
    $radio_list->add_option('everybody_read', $cal_everybody_read);
    echo '<br />';
    $radio_list->add_option('everybody_write', $cal_everybody_write);
    echo '<br />';
    $radio_list->add_option('private', $sc_private_event);

    echo '</td></tr>';


    $calendar_count = $cal->get_authorised_calendars($GO_SECURITY->user_id);
    $dropbox= new dropbox();
    $count = 0;
    while ($cal->next_record())
    {
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $cal->f('acl_write')))
      {
	//remember the first ab that is writable
	if(!isset($first_writable_cal))
	{
	  $first_writable_cal = $cal->f('id');
	}
	$dropbox->add_value($cal->f('id'), $cal->f('name'));
	$count++;
      }
    }

    //get the given calendar
    if ($calendar_id > 0)
    {
      $calendar = $cal->get_calendar($calendar_id);
    }else
    {
      $calendar = false;
    }

    //if there was no or a read only addressbook given then change to the first writable
    if (!$calendar || !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_write']))
    {
      //there is no writable addressbook so add one
      if (!isset($first_writable_cal))
      {
	$cal_name = $_SESSION['GO_SESSION']['name'];
	$new_cal_name = $cal_name;
	$x = 1;
	while($cal->get_calendar_by_name($new_cal_name))
	{
	  $new_cal_name = $cal_name.' ('.$x.')';
	      $x++;
	      }
	      $calendar_id = $cal->add_calendar($GO_SECURITY->user_id, addslashes($new_cal_name), 7, 20);
	      $dropbox->add_value($calendar_id, $new_cal_name);
	      }else
	      {
	      $calendar_id = $first_writable_cal;
	      }
	      }

	      if (count($event['calendars']) == 0)
	      {
	      $event['calendars'][] = $calendar_id;
	      }

	      for($i=0;$i<count($event['calendars']);$i++)
	      {
	      if (!$dropbox->is_in_dropbox($event['calendars'][$i]))
	      {
	      echo '<input type="hidden" name="calendars[]" value="'.$event['calendars'][$i].'" />';
	      }
	      }

	      echo '<tr><td valign="top">'.$sc_put_in.':</td>';
	      echo '<td><table border="0">';
	      $dropbox->print_dropbox('calendars[]', $event['calendars'], '', true, '5', '200');
	      echo '</table></td></tr>';

	      echo '<tr><td colspan="2">';
	      $button = new button($cmdOk, "javascript:save_event('true');");
	      echo '&nbsp;&nbsp;';
	      $button = new button($cmdApply, "javascript:save_event('false');");
	      echo '&nbsp;&nbsp;';
	      if ($event_id > 0)
	      {
		$button = new button($cal_export, "document.location='export.php?event_id=$event_id';");
		echo '&nbsp;&nbsp;';
	      }
	      $button = new button($cmdCancel, "javascript:document.location='$return_to'");
	      echo '</td></tr>';
	      echo '</table>';
	      ?>
		<script type="text/javascript" language="javascript">
		toggle_repeat('<?php echo $event['repeat_type']; ?>');
	      <?php

		if ($event['all_day_event'] == '1')
		{
		  echo 'disable_time();';
		}

	      if ($event['repeat_forever'] == '1')
	      {
		echo 'toggle_repeat_end_info();';
	      }
	      ?>



		function update_end_hour(start_hour)
		{
		  if (start_hour == 24)
		  {
		    document.event_form.end_hour.value='01';
		  }else
		  {
		    start_hour = parseInt(start_hour)+1
		      if (start_hour < 10)
		      {
			start_hour = "0"+start_hour;
		      }
		    document.event_form.end_hour.value= start_hour;
		  }
		}

	      function get_date(dateString)
	      {
		<?php
		  if ($_SESSION['GO_SESSION']['date_format'] == "d-m-Y")
		  {
		    echo "
		      var date = new Date(dateString.substring(6,10),
			  dateString.substring(3,5)-1,
			  dateString.substring(0,2),
			  dateString.substring(11,13),
			  dateString.substring(14,16)
			  );";
		  }else
		  {
		    echo "
		      var date = new Date(dateString.substring(6,10),
			  dateString.substring(0,2),
			  dateString.substring(3,5)-1,
			  dateString.substring(11,13),
			  dateString.substring(14,16)
			  );";
		  }
		  ?>

		    return date;
	      }

	      function save_event(close)
	      {
		start_date = get_date(document.event_form.start_date.value.replace(/-/g,'/')+' '+document.event_form.start_hour.value+':'+document.event_form.start_min.value+':00');
		end_date = get_date(document.event_form.end_date.value.replace(/-/g,'/')+' '+document.event_form.end_hour.value+':'+document.event_form.end_min.value+':00');
		repeat_end_date = get_date(document.event_form.repeat_end_date.value.replace(/-/g,'/')+' 00:00:00');

		if (start_date > end_date)
		{
		  alert("<?php echo $sc_start_later; ?>");
		  return;
		}
		if (document.event_form.repeat_type.value != '0')
		{
		  if ((start_date >= repeat_end_date) && document.event_form.repeat_forever.checked == false)
		  {
		    alert("<?php echo $sc_cycle_start_later; ?>");
		    return;
		  }
		}

		if (document.event_form.repeat_type.value == '1' && document.event_form.reminder.value > 43200)
		{
		  alert("<?php echo $sc_reminder_set_to_early; ?>");
		  return;
		}

		if (document.event_form.repeat_type.value == '2' && document.event_form.reminder.value > 518400)
		{
		  alert("<?php echo $sc_reminder_set_to_early; ?>");
		  return;
		}

		if (document.event_form.repeat_type.value == '2' || document.event_form.repeat_type.value == '4')
		{
		  if (document.event_form.repeat_days_0.checked == false && document.event_form.repeat_days_1.checked == false && document.event_form.repeat_days_2.checked == false && document.event_form.repeat_days_3.checked == false && document.event_form.repeat_days_4.checked == false && document.event_form.repeat_days_5.checked == false && document.event_form.repeat_days_6.checked == false)
		  {
		    alert("<?php echo $sc_never_happens; ?>");
		    return;
		  }
		}
		document.event_form.task.value = 'save_event';
		document.event_form.close.value = close;

		document.event_form.submit();

	      }

	      function remove_client()
	      {
		document.event_form.contact_id.value = 0;
		document.event_form.contact_name.value = '';
		document.event_form.contact_name_text.value = '';
	      }

	      function toggle_repeat_end_info()
	      {
		document.event_form.repeat_end_date.disabled = !document.event_form.repeat_end_date.disabled;
	      }

	      function disable_time()
	      {
		if (document.event_form.start_hour.disabled==false)
		{
		  document.event_form.start_hour.disabled=true;
		  document.event_form.start_min.disabled=true;
		  document.event_form.end_hour.disabled=true;
		  document.event_form.end_min.disabled=true;
		}else
		{
		  document.event_form.start_hour.disabled=false;
		  document.event_form.start_min.disabled=false;
		  document.event_form.end_hour.disabled=false;
		  document.event_form.end_min.disabled=false;
		}
	      }

	      function toggle_repeat(repeat)
	      {

		document.event_form.repeat_type.value = repeat;
		switch(repeat)
		{
		  case '0':
		    disable_days(true);
		    document.event_form.month_time.disabled = true;
		    disable_repeat_end_date(true);
		    document.event_form.repeat_every.disabled = true;
		    break;

		  case '1':
		    disable_days(true);
		    document.event_form.month_time.disabled = true;
		    disable_repeat_end_date(false);
		    document.event_form.repeat_every.disabled = false;
		    break;

		  case '2':
		    disable_days(false);
		    document.event_form.month_time.disabled = true;
		    disable_repeat_end_date(false);
		    document.event_form.repeat_every.disabled = false;
		    break;

		  case '3':
		    disable_days(true);
		    disable_repeat_end_date(false);
		    break;

		  case '4':
		    disable_days(false);
		    document.event_form.month_time.disabled = false;
		    disable_repeat_end_date(false);
		    document.event_form.repeat_every.disabled = false;
		    break;

		  case '5':
		    disable_days(true);
		    document.event_form.month_time.disabled = true;
		    disable_repeat_end_date(false);
		    document.event_form.repeat_every.disabled = false;
		    break;
		}
	      }

	      function disable_days(disable)
	      {
		document.event_form.repeat_days_0.disabled=disable;
		document.event_form.repeat_days_1.disabled=disable;
		document.event_form.repeat_days_2.disabled=disable;
		document.event_form.repeat_days_3.disabled=disable;
		document.event_form.repeat_days_4.disabled=disable;
		document.event_form.repeat_days_5.disabled=disable;
		document.event_form.repeat_days_6.disabled=disable;

	      }

	      function disable_repeat_end_date(disable)
	      {
		document.event_form.repeat_forever.disabled=disable;
		if (disable == true || (disable==false && document.event_form.repeat_forever.checked == false))
		{
		  document.event_form.repeat_end_date.disabled=disable;
		}
	      }

	      document.event_form.name.focus();
	      </script>
		<?php
		break;
}
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path.'footer.inc');
?>
