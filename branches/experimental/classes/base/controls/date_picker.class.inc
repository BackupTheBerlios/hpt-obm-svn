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

class date_picker
{
  /**
   * return date picker's header
   */
  function get_header()
  {
    global $GO_CONFIG, $jscalendar_language_name;

    $jscalendar_language_name = $jscalendar_language_name != '' ? $jscalendar_language_name : 'calendar-en.js';
    $headers  = '<script type="text/javascript" src="'.$GO_CONFIG->control_url.'jscalendar/calendar.js"></script>';
    $headers .= '<script type="text/javascript" src="'.$GO_CONFIG->control_url.'jscalendar/lang/'.$jscalendar_language_name.'"></script>';
    $headers .= '<script type="text/javascript" src="'.$GO_CONFIG->control_url.'jscalendar/calendar-setup.js"></script>';

    return $headers;
  }

  /**
   * Print the date picker.
   *
   * @param string $name
   * @param string $date_format
   * @param string $set_date default is empty
   * @param string $flat_div_id default is empty
   * @param string $flat_callbackfunction default is empty
   * @param string $arguments default is empty
   * @param boolean $disabled default is false
   * @see get_date_picker
   */
  function print_date_picker($name, $date_format, $set_date='', $flat_div_id='', $flat_callbackfunction='', $arguments='', $disabled=false)
  {
	echo  $this->get_date_picker($name, $date_format, $set_date, $flat_div_id, $flat_callbackfunction, $arguments, $disabled);
  }

  /**
   * Get the date picker.
   *
   * @param string $name
   * @param string $date_format
   * @param string $set_date default is empty
   * @param string $flat_div_id default is empty
   * @param string $flat_callbackfunction default is empty
   * @param stringg $arguments default is empty
   * @param boolean $disabled default is false
   * @return string html code of the date picker
   */
  function get_date_picker($name, $date_format, $set_date='', $flat_div_id='', $flat_callbackfunction='', $arguments='', $disabled=false)
  {
    $str_return='';
    //dirty hack for mysql default date
    if ($set_date == '00-00-0000')
    {
      $set_date = '';
    }
    if ($disabled)
    {
      $arguments .= ' disabled';
    }
    $date_format = $this->convert_date_format($date_format);

    if ($flat_div_id == '')
    {
      $h = $m = '';
      $browser = detect_browser();
      if ($browser['name'] == 'MSIE' || $browser['name'] == 'OPERA') {
        $h = '19px';
        $m = '0px';
      }
      elseif ($browser['name'] == 'NETSCAPE' || $browser['subversion'] <= '20030624') {
        $h = '18px';
        $m = '1px';
      }
      else {
        $h = '18px';
        $m = '-1px';
      }
      $str_return.='<input class="textbox" type="text" id="'.$name.'_input" name="'.$name.'" value="'.$set_date.'" '.$arguments.' />'.
	'<input type="button" name="'.$name.'_button" id="'.$name.'_button" value="..." class="button" style="width: 18px; height: '.$h.'; margin-bottom: '.$m.';"';
      if ($disabled)
      {
	$str_return.=' disabled';
      }
      $str_return.='/>';
    }

    $first_weekday = $_SESSION['GO_SESSION']['first_weekday']==""?0:$_SESSION['GO_SESSION']['first_weekday'];

    $str_return.='<script type="text/javascript">
      var calendar = Calendar.setup(
	  {
firstDay  : '.$first_weekday.',
inputField  : "'.$name.'_input",
ifFormat    : "'.$date_format.'",';

    if ($set_date != '')
      {
	$str_return.='date :  "'.str_replace('-','/',$set_date).'",';
      }

    if ($flat_div_id != '')
    {
      $str_return.='flat : "'.$flat_div_id.'", flatCallback : '.$flat_callbackfunction;
    }else
    {
      $str_return.='button      : "'.$name.'_button"';
    }
    $str_return.='});</script>';
    return $str_return;
  }

  /**
   * Convert date format.
   *
   * @param string $php_date_format
   * @return string converted format
   */
  function convert_date_format($php_date_format)
  {
    /*
     %a 	abbreviated weekday name
     %A 	full weekday name
     %b 	abbreviated month name
     %B 	full month name
     %C 	century number
     %d 	the day of the month ( 00 .. 31 )
     %e 	the day of the month ( 0 .. 31 )
     %H 	hour ( 00 .. 23 )
     %I 	hour ( 01 .. 12 )
     %j 	day of the year ( 000 .. 366 )
     %k 	hour ( 0 .. 23 )
     %l 	hour ( 1 .. 12 )
     %m 	month ( 01 .. 12 )
     %M 	minute ( 00 .. 59 )
     %n 	a newline character
     %p 	\x{201C}PM\x{201D} or \x{201C}AM\x{201D}
     %P 	\x{201C}pm\x{201D} or \x{201C}am\x{201D}
     %S 	second ( 00 .. 59 )
     %s 	number of seconds since Epoch (since Jan 01 1970 00:00:00 UTC)
     %t 	a tab character
     %U, %W, %V 	the week number
     %u 	the day of the week ( 1 .. 7, 1 = MON )
     %w 	the day of the week ( 0 .. 6, 0 = SUN )
     %y 	year without the century ( 00 .. 99 )
     %Y 	year including the century ( ex. 1979 )
     %% 	a literal % character
    */


    $cal_format[] = '%e';
    $php_format[] = 'j';

    $cal_format[] = '%d';
    $php_format[] = 'd';

    $cal_format[] = '%H';
    $php_format[] = 'H';

    $cal_format[] = '%I';
    $php_format[] = 'G';

    $cal_format[] = '%k';
    $php_format[] = 'h';

    $cal_format[] = '%l';
    $php_format[] = 'g';

    $cal_format[] = '%P';
    $php_format[] = 'a';

    $cal_format[] = '%Y';
    $php_format[] = 'Y';

    $cal_format[] = '%y';
    $php_format[] = 'y';

    $cal_format[] = '%m';
    $php_format[] = 'm';

    for ($i=0;$i<count($php_format);$i++)
    {
      $php_date_format = str_replace($php_format[$i], $cal_format[$i], $php_date_format);
    }
    return $php_date_format;
  }


}
?>
