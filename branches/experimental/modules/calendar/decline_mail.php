<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

	$_SESSION['GO_SESSION']['DST'] = $db->f('DST');
	$_SESSION['GO_SESSION']['date_format'] = $db->f('date_format');
	$_SESSION['GO_SESSION']['time_format'] = $db->f('time_format');
	
	$mail_body = '<b><i>'. $name . ' - '.$email.'</i></b> '.$sc_declined_event.'<br><br>';

	if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
	{
		$dst_offset = $_SESSION['GO_SESSION']['DST'];
	}else
	{
		$dst_offset = 0;
	}
	$timezone_offset = $_SESSION['GO_SESSION']['timezone']+$dst_offset;
	
/*	if ($db->f('all_day_event')==1)
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

    }		*/
	$start_time = $db->f('start_time');

	$end_time = $db->f('end_time');

	$mail_body  .= '<table border="0"><tr><td>'.$sc_title.':</td><td><b><i>'.$title.'</i></b></td></tr>';

/*	if ($_POST['contact_id'] > 0)
	{
	  $mail_body  .= '<tr><td>'.$sc_client.':</td>';
	  $mail_body  .= '<td>'.show_contact($_POST['contact_id']).'</td></tr>';
	}*/
//	if ($_POST['description'] != '')
	if ($db->f('description') !='')
	{
	  $mail_body  .= '<tr><td valign="top">'.$strDescription.':</td>';
	  $mail_body  .='<td>'.text_to_html($db->f('description')).'</td></tr>';
	}

//	if ($_POST['location'] != '')
	if ($db->f('location') !='')
	{
	  $mail_body  .= '<tr><td>'.$sc_location.':</td>';
	  $mail_body  .= '<td>'.text_to_html($db->f('location')).'</td></tr>';
	}

	$mail_body  .= '<tr><td>'.$sc_type.':</td>';
	$mail_body  .= '<td>'.$sc_types[$db->f('repeat_type')].'</td></tr>';
	
	//don't calculate timezone offset for all day events
	$timezone_offset_string = $db->f('all_day_event')==0 ? 0 : $timezone_offset;
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
	if ($db->f('all_day_event')==0)
	{
	  $event_datetime_format = $_SESSION['GO_SESSION']['date_format'];
	}else
	{
	  $event_datetime_format = $_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'].' '.$gmt_string;
	}
	$event_time_format = $_SESSION['GO_SESSION']['time_format'].' '.$gmt_string;
//echo 'datetim_format='.$_SESSION['GO_SESSION']['date_format'];
//exit();
	switch($db->f('repeat_type'))
	{
	  case REPEAT_NONE:
	    $mail_body  .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
//	    if ($end_time != $start_time)
//	    {
	      $mail_body  .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
//	    }
	    break;

	  case REPEAT_WEEKLY:
	    if($db->f('all_day_event')==0)
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_time_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      if ($end_time != $start_time)
	      {
		$mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_time_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	      }
	    }

/*	    $mail_body .= '<tr><td>'.$sc_at_days.':</td><td>';

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

/*	    $event['days'] = array();
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
	    }*/
//	    $mail_body .= implode(', ', $event['days']);

/*	    $mail_body .= '</td></tr>';

	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
    	
		$repeat_forever = $db->f('repeat_forever')=='1' ? '1' : '0';		
	    if ($repeat_forever == '1')
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
    	  $repeat_forever = $db-f('repeat_end_time')=='1' ? '1' : '0';		
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }*/
//	    $mail_body .= '</td></tr>';

	    break;

	  case REPEAT_DAILY:
	    if($db->f('all_day_event')==0)
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      $mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	    }
/*	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if (isset($repeat_forever))
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }
	    $mail_body .= '</td></tr>';*/
	    break;

	  case REPEAT_MONTH_DATE:	
	    if ($db->f('all_day_event')==0)
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
	    if ($db->f('all_day_event')==0)
	    {
	      $mail_body .= '<tr><td>'.$sc_start_at.':</td><td>'.date($event_datetime_format, $start_time+($timezone_offset*3600)).'</td></tr>';
	      if (isset($repeat_forever))
	      {
		$mail_body .= '<tr><td>'.$sc_end_at.':</td><td>'.date($event_datetime_format, $end_time+($timezone_offset*3600)).'</td></tr>';
	      }
	    }

/*	    $local_start_hour = date('H',$start_time-$timezone_offset) + ($timezone_offset/3600);
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
	    }*/


	    break;

	    case REPEAT_YEARLY;
	    if ($db->f('all_day_event')==0)
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

/*	    $mail_body .= '<tr><td>'.$sc_cycle_end.':</td><td>';
	    if ($repeat_forever == '1')
	    {
	      $mail_body .= $sc_noend;
	    }else
	    {
	      $mail_body .= date($_SESSION['GO_SESSION']['date_format'], $repeat_end_time);
	    }*/

	    break;
	}

	$mail_body .= '<tr><td>&nbsp;'.$sc_reason.': </td><td><b><i>'.$_REQUEST['reason'].'</i></b></td></tr>';
	$mail_body .= '</table>';
?>
