<?php
/*
Copyright Intermesh 2003
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 14 March 2004

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

/**
* TODO: Attendee and VTODO support
* @author	Merijn Schering <mschering@intermesh.nl>
* @version	1.0 Release date: 14 March 2004
* @package	go_ical
*/
class go_ical extends calendar
{
	/**
	* Calendar properties
	* @var string
	*/
	var $calendar_properties = '';
	/**
	* Events
	* @var array
	*/
	var $events = array();
	/**
	* DateTime format
	* @var string
	*/
	var $datetime_format = 'Ymd\THis\Z';
	/**
	* Date format
	* @var string
	*/
	var $date_format = 'Ymd';

	/**
	* Constructor
	*/
	function go_ical()
	{
		$this->db();

		$this->calendar_properties =
			"VERSION\r\n :2.0\r\n".
			"PRODID\r\n :-//Intermesh//NONSGML Group-Office//EN\r\n".
			"METHOD\r\n :PUBLISH\r\n";
	}

	/**
	* Convert events array to string
	* @return	string
	*/
	function export()
	{
		$ics = '';
		while($event = array_shift($this->events))
		{
			$ics .= $event;
		}
		return $ics;
	}

	/**
	* Export a event by event ID
	* @param	int	$event_id	Event ID
	* @return	string
	*/
	function export_event($event_id)
	{
		$this->add_event($this->get_event($event_id));
		return $this->export();
	}

	/**
	* Export all event of a calendar by calendar ID
	* @param	int	$calendar_id	Calendar ID
	* @return	string
	*/
	function export_calendar($calendar_id)
	{
		$this->get_events($calendar_id);
		while($this->next_record())
		{
			$this->add_event($this->Record);
		}

		return $this->export();
	}

	/**
	* Convert a string to RFC2445 format string
	* @param	string	$string		Source string
	* @return	string	RFC2445 format string
	*/
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

	/**
	* Add a event
	* @param	array	$event	String array
	* @return	bool	true if success, otherwise false
	*/
	function add_event($event)
	{
		$ical_event = "BEGIN:VCALENDAR\r\n".
			$this->calendar_properties.
			"BEGIN:VEVENT\r\n".
			"UID\r\n :go".date($this->datetime_format, $event['ctime'])."-".
				$event['id']."@".$_SERVER['HTTP_HOST']."\r\n".
			"SUMMARY\r\n ".$this->rfc2445_convert(':'.$event['name'])."\r\n";

		if ($event['description'] != '')
		{
			$ical_event .= "DESCRIPTION\r\n ".$this->rfc2445_convert(':'.$event['description'])."\r\n";
		}
		if ($event['location'] != '')
		{
			$ical_event .= "LOCATION\r\n ".$this->rfc2445_convert(':'.$event['location'])."\r\n";
		}
		//TODO: Implement these values in Group-Office:
		$ical_event .=
			"STATUS\r\n :CONFIRMED\r\n".
			"CLASS\r\n :PRIVATE\r\n";

		switch($event['repeat_type'])
		{
			case REPEAT_DAILY:
 				$ical_event .= "RRULE\r\n :FREQ=DAILY;";

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
				$ical_event .= "INTERVAL=".$event['repeat_every'].";BYDAY=\r\n";

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

		$ical_event .=
			"\r\n :".date($DT_format, $event['start_time'])."\r\n".
			"DTEND";

			if ($event['all_day_event'] == '1')
			{
				$ical_event .= ";VALUE=DATE";
			}
		$ical_event .=
			"\r\n :".date($DT_format, $event['end_time'])."\r\n".

			"DTSTAMP\r\n :".date($this->datetime_format, $event['ctime'])."\r\n".
			"X-GO-BGCOLOR\r\n :".$event['background']."\r\n".
			"END:VEVENT\r\n".
			"END:VCALENDAR\r\n";

		$this->events[] = $ical_event;

		return true;
	}
}
?>