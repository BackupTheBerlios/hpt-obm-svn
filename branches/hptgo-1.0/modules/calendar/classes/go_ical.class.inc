<?php
/*
Copyright Intermesh 2003
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 14 March 2004

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

TODO: Attendee and VTODO support
*/
class go_ical extends calendar
{
	var $calendar_properties = '';
	var $events = array();
	var $datetime_format = 'Ymd\THis';
	var $date_format = 'Ymd';
	
	var $timezone_id = '';

	function go_ical()
	{
		$this->db();

		$this->calendar_properties =
			"VERSION:2.0\r\n".
			"PRODID:-//Intermesh//NONSGML Group-Office//EN\r\n".
			"CALSCALE:GREGORIAN\r\n".
			"METHOD:PUBLISH\r\n";
	}
	
	function export_calendar_header()
	{
		return "BEGIN:VCALENDAR\r\n".
				$this->calendar_properties.
				$this->export_timezone();
	}
	
	function export_calendar_footer()
	{
		return "END:VCALENDAR\r\n";
	}

	function export()
	{
	
	}
	
	function format_timezone($timezone)
	{
		if($timezone < 10 && $timezone > -10)
		{
			if($timezone > 0)
			{
				$timezone = '+0'.$timezone.'00';
			}else
			{
				$timezone = '-0'.$timezone.'00';				
			}
		}else
		{
			if($timezone > 0)
			{
				$timezone = '+'.$timezone.'00';
			}else
			{
				$timezone = '-'.$timezone.'00';				
			}
		}
		
		return $timezone;
	}

	function export_timezone()
	{
		global $GO_LANGUAGE;
		
		$this->timezone_id = $GO_LANGUAGE->language['description'];
		
		$timezone =	"BEGIN:VTIMEZONE\r\n".
								"TZID:".$this->timezone_id."\r\n";
								"LAST-MODIFIED:".date($this->date_format, get_time())."\r\n";
							
		$dst_tz_offset_from = $this->format_timezone($_SESSION['GO_SESSION']['timezone']);			
		$dst_tz_offset_to = $this->format_timezone($_SESSION['GO_SESSION']['DST']+$_SESSION['GO_SESSION']['timezone']);		
							
		if($_SESSION['GO_SESSION']['DST'] > 0)
		{		
			$timezone .= 	"BEGIN:DAYLIGHT\r\n".
										"DTSTART:19700329T030000\r\n".										
										"TZOFFSETTO:".$dst_tz_offset_to."\r\n".
										"TZOFFSETFROM:".$dst_tz_offset_from."\r\n".
										"TZNAME;language=".$_SESSION['GO_SESSION']['language']['code'].":GMT".$dst_tz_offset_to."\r\n".
										"RRULE:FREQ=YEARLY;INTERVAL=1;BYDAY=-1SU;BYMONTH=3\r\n".
										"END:DAYLIGHT\r\n";
		}
		
		$timezone .=  "BEGIN:STANDARD\r\n".
									"TZOFFSETTO:".$dst_tz_offset_from."\r\n".
									"TZOFFSETFROM:".$dst_tz_offset_to."\r\n".
									"TZNAME;language=".$_SESSION['GO_SESSION']['language']['code'].":GMT".$dst_tz_offset_from."\r\n";
			
		if($_SESSION['GO_SESSION']['DST'] > 0)
		{
				$timezone .= 	"DTSTART:19701025T040000\r\n".
											"RRULE:FREQ=YEARLY;INTERVAL=1;BYDAY=-1SU;BYMONTH=10\r\n";
		}else
		{
				$timezone .= "DTSTART:19700101T000000\r\n";
		}
		
		$timezone .= 	"END:STANDARD\r\n".
									"END:VTIMEZONE\r\n";
		return $timezone;
	}

	function export_event($event_id)
	{
		$ics = $this->export_calendar_header();
		$ics .= $this->convert_event($this->get_event($event_id));
		$ics .= $this->export_calendar_footer();
		
		return $ics;
	}

	function export_calendar($calendar_id)
	{		
		$ics = $this->export_calendar_header();
		
		$this->get_events($calendar_id);
		while($this->next_record())
		{
			$ics .= $this->convert_event($this->Record);
		}
		
		$ics .= $this->export_calendar_footer();
		return $ics;
	}

	function rfc2445_convert($string)
	{
		//TODO: is there a better way? preg_replace?
		$string = str_replace("\r", '', $string);
		$string = str_replace("\n", '\n', $string);
		$string = str_replace(',', '\,', $string);
		$string = str_replace(';', '\;', $string);
		$string = wordwrap($string, 72, " \r\n ");
		return $string;
	}

	function convert_event($event)
	{
		if($this->timezone_id != '')
		{
			if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
			{
				$dst_offset = $_SESSION['GO_SESSION']['DST'];
			}else
			{
				$dst_offset = 0;
			}			  
			$timezone_offset = ($_SESSION['GO_SESSION']['timezone']+$dst_offset)*3600;
		}
	
		$ical_event = "BEGIN:VEVENT\r\n".
			"UID:go".date($this->datetime_format, $event['ctime'])."-".
				$event['id']."@".$_SERVER['HTTP_HOST']."\r\n".
			"SUMMARY:".$this->rfc2445_convert($event['name'])."\r\n";

		if ($event['description'] != '')
		{
			$ical_event .= "DESCRIPTION:".$this->rfc2445_convert($event['description'])."\r\n";
		}
		if ($event['location'] != '')
		{
			$ical_event .= "LOCATION:".$this->rfc2445_convert($event['location'])."\r\n";
		}
		//TODO: Implement these values in Group-Office:
		$ical_event .=
			"STATUS:CONFIRMED\r\n".
			"CLASS:PRIVATE\r\n";

		switch($event['repeat_type'])
		{
			case REPEAT_DAILY:
 				$ical_event .= "RRULE:FREQ=DAILY;";

				if ($event['repeat_forever'] == '0')
				{
					$ical_event .= "UNTIL=".date($this->date_format, $event['repeat_end_time']).";";
				}

				$ical_event .= "INTERVAL=".$event['repeat_every']."\r\n";
			break;

			case REPEAT_WEEKLY:
				$ical_event .= "RRULE:FREQ=WEEKLY;";

				if ($event['repeat_forever'] == '0')
				{
					$ical_event .= "UNTIL=".date($this->date_format, $event['repeat_end_time']).";";
				}

				$ical_event .= "INTERVAL=".$event['repeat_every'].";BYDAY=";

				$event_days = array();

				if ($event['sun'] == '1')
				{
					$event_days[] = "SU";
				}
				if ($event['mon'] == '1')
				{
					$event_days[] = "MO";
				}
				if ($event['tue'] == '1')
				{
					$event_days[] = "TU";
				}
				if ($event['wed'] == '1')
				{
					$event_days[] = "WE";
				}
				if ($event['thu'] == '1')
				{
					$event_days[] = "TH";
				}
				if ($event['fri'] == '1')
				{
					$event_days[] = "FR";
				}
				if ($event['sat'] == '1')
				{
					$event_days[] = "SA";
				}

				$ical_event .= implode(',', $event_days)."\r\n";
			break;

			case REPEAT_MONTH_DATE:
					$ical_event .= "RRULE:FREQ=MONTHLY;";
					if ($event['repeat_forever'] == '0')
					{
						$ical_event .= "UNTIL=".date($this->date_format, $event['repeat_end_time']).";";
					}
					$ical_event .= "INTERVAL=".$event['repeat_every']."\r\n";
			break;

			case REPEAT_MONTH_DAY:
				$ical_event .= "RRULE:FREQ=MONTHLY;";
				if ($event['repeat_forever'] == '0')
				{
					$ical_event .= "UNTIL=".date($this->date_format, $event['repeat_end_time']).";";
				}
				$ical_event .= "INTERVAL=".$event['repeat_every'].";BYDAY=";

				$event_days = array();

				if ($event['sun'] == '1')
				{
					$event_days[] = $event['month_time']."SU";
				}
				if ($event['mon'] == '1')
				{
					$event_days[] = $event['month_time']."MO";
				}
				if ($event['tue'] == '1')
				{
					$event_days[] = $event['month_time']."TU";
				}
				if ($event['wed'] == '1')
				{
					$event_days[] = $event['month_time']."WE";
				}
				if ($event['thu'] == '1')
				{
					$event_days[] = $event['month_time']."TH";
				}
				if ($event['fri'] == '1')
				{
					$event_days[] = $event['month_time']."FR";
				}
				if ($event['sat'] == '1')
				{
					$event_days[] = $event['month_time']."SA";
				}

				$ical_event .= implode(',', $event_days)."\r\n";

			break;

			case REPEAT_YEARLY:
				$ical_event .= "RRULE:FREQ=YEARLY;";
				if ($event['repeat_forever'] == '0')
				{
					$ical_event .= "UNTIL=".date($this->date_format, $event['repeat_end_time']).";";
				}
				$ical_event .= "INTERVAL=".$event['repeat_every'].";BYMONTH=".date('n', $event['start_time'])."\r\n";
			break;
		}

		$DT_format = $event['all_day_event'] == '1' ? $this->date_format : $this->datetime_format;
		
		$ical_event .= "DTSTART";
		
		if ($event['all_day_event'] == '1')
		{
			$ical_event .= ";VALUE=DATE";
		}			
		if($this->timezone_id != '') 
		{
			$ical_event .= ";TZID=".$this->timezone_id.":".date($DT_format, $event['start_time']+$timezone_offset);
		}else
		{
			$ical_event .= ":".date($DT_format, $event['start_time'])."Z";
		}	

		$ical_event .= "\r\nDTEND";
		
		if ($event['all_day_event'] == '1')
		{
			$ical_event .= ";VALUE=DATE";
		}			
		if($this->timezone_id != '') 
		{
			$ical_event .= ";TZID=".$this->timezone_id.":".date($DT_format, $event['end_time']+$timezone_offset);
		}else
		{
			$ical_event .= ";".date($DT_format, $event['end_time'])."Z";
		}	


			$ical_event .= "\r\nDTSTAMP:".date($this->datetime_format, $event['ctime'])."\r\n".
			"X-GO-BGCOLOR:".$event['background']."\r\n".
			"END:VEVENT\r\n";

		return $ical_event;

		return true;
	}
}
?>
