<?php
/*
Copyright Intermesh 2004
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 14 March 2004

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

builds an array from an ics file:

Example:

SUMMARY:Event summary
$this->events[$n]['SUMMARY']['value'] = 'Event summary';

RRULE:FREQ=YEARLY;INTERVAL=1;BYMONTH=3
$this->events[$n]['RRULE']['value'] = 'FREQ=YEARLY;INTERVAL=1;BYMONTH=3';

You can parse the rrule with parse_rrule() into:

$rrule = $this->parse_rrule('FREQ=YEARLY;INTERVAL=1;BYMONTH=3');
$rrule['FREQ'] = 'YEARLY';
$rrule['INTERVAL'] = '1';
$rrule['BYMONTH'] = '3';

DTSTART;VALUE=DATE:20040303
$this->events[$n]['DTSTART']['params']['VALUE'] = 'DATE';
$this->events[$n]['DTSTART']['value'] = '20040303';
*/
class ical2array
{
	var $events = array();
	var $todos = array();

	var $event = array();
	var $object_name;
	var $param_name;
	var $buffer;

	function parse_file($ical_file)
	{
		$in_event = false;

		if (!$handle = fopen ($ical_file, "r"))
		{
			return false;
		}
		while (!feof($handle))
		{
		  $line = fgets($handle, 4096);

		  //don't process empty lines
		  if (strlen($line) > 0)
		  {
				switch(trim($line))
				{
					case 'BEGIN:VEVENT':
						$in_event = true;
						$in_value = false;
						$this->buffer_type = 'object_name';
						$this->buffer = '';
					break;

					case 'END:VEVENT':
						$in_event = false;

						$this->add_buffer();
						$this->add_event();
					break;

					default:
						if ($in_event)
						{
							/*
							If we are processing a value and the first value is a tab (chr(9)) or
							a space (chr(32)) then cut it. if it's not one of these chars
							then we are done processing a value.
							*/
							if ($this->buffer_type == 'value')
							{
								if(isset($line[0]) && $line[0] != chr(32) && $line[0] != chr(9))
								{
									$this->add_buffer();
									$this->buffer_type = 'object_name';
								}else
								{
									$line = substr($line, 1);
								}
							}

							$line_length = strlen($line);
							for($i=0;$i<$line_length;$i++)
							{
								$char = $line[$i];

								switch($char)
								{
									case ':':
										if (strlen($this->buffer) > 0)
										{
											//after an : comes an:
											if ($this->buffer_type == 'value')
											{
												$this->buffer .= $char;
											}else
											{
												$this->add_buffer();
												$this->buffer_type = 'value';
											}
										}
									break;

									case ';':
										if (strlen($this->buffer) > 0)
										{
											//after an ; comes an:
											if ($this->buffer_type == 'value')
											{
												$this->buffer .= $char;
											}else
											{
												$this->add_buffer();
												$this->buffer_type = 'param_name';
											}
										}
									break;

									case '=':
										if (strlen($this->buffer) > 0)
										{
											//after an ; comes an:
											if ($this->buffer_type == 'value')
											{
												$this->buffer .= $char;
											}else
											{
												$this->add_buffer();
												$this->buffer_type = 'param_value';
											}
										}
									break;

									default:
										//default is to create the buffer
										$this->buffer .= $char;
									break;
								}
							}
						}
					break;
				}
			}
		}
		fclose ($handle);

		return $this->events;
	}

	function parse_rrule($rrule)
	{
		$rrule_arr = array();

		$rrule = str_replace('RRULE:', '', $rrule);
		$params = explode(';', $rrule);

		while($param = array_shift($params))
		{
			$param_arr = explode('=', $param);

			if (isset($param_arr[0]) && isset($param_arr[1]))
			{
				$rrule_arr[strtoupper(trim($param_arr[0]))] = strtoupper(trim($param_arr[1]));
			}
		}
		return $rrule_arr;
	}

	function parse_date($date)
	{
		$year = substr($date,0,4);
		$month = substr($date,4,2);
		$day = substr($date,6,2);

		if (strpos($date, 'T') !== false)
		{
			$hour = substr($date,9,2);
			$min = substr($date,11,2);
			$sec = substr($date,13,2);
		}else
		{
			$hour = 0;
			$min = 0;
			$sec = 0;
		}
		return gmmktime($hour, $min, $sec, $month, $day , $year);
	}

	function add_buffer()
	{
		$this->buffer = str_replace('\r\n', "\r\n", $this->buffer);
		$this->buffer = str_replace('\n', "\r\n", $this->buffer);
		$this->buffer = stripslashes($this->buffer);
		$this->buffer = trim($this->buffer);

		switch($this->buffer_type)
		{
			case 'object_name':
				$this->event[$this->buffer] = array();
				$this->object_name = $this->buffer;
			break;

			case 'param_name':
				$this->event[$this->object_name]['params'][$this->buffer] = '';
				$this->param_name = $this->buffer;
			break;

			case 'param_value':
				$this->event[$this->object_name]['params'][$this->param_name] = $this->buffer;
			break;

			case 'value':
				$this->event[$this->object_name]['value'] = $this->buffer;
				unset($this->object_name,	$this->param_name);
			break;
		}
		$this->buffer = '';
	}

	function add_event()
	{
		$this->events[] = $this->event;
		$this->event = array();
	}
}
?>