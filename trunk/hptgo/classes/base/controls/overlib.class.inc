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

class overlib
{
  function get_header()
  {
    global $GO_CONFIG, $GO_THEME;

    return '<script type="text/javascript" src="'.$GO_CONFIG->control_url.'overlib/overlib.js"></script>'.
      "<script type=\"text/javascript\">overlib_pagedefaults(CSSCLASS, ".
      "TEXTFONTCLASS,'olFont',FGCLASS,'olForeground', BGCLASS,'olBackground',".
      "CAPTIONFONTCLASS,'olCaptionFont', CLOSEFONTCLASS, 'olCaptionFont');</script>".
      '<link href="'.$GO_THEME->theme_url.'overlib.css" type="text/css" rel="stylesheet" />';
  }

  function print_overlib($text, $caption='', $options='')
  {
    if ($text != '')
    {
      $overlib = 'onmouseover="return overlib(\''.addslashes($text).'\'';

      if ($caption != '')
      {
	$overlib .= 'CAPTION, \''.addslashes($caption).'\'';
      }
      if ($options != '')
      {
	$overlib .= ','.$options;
      }
      $overlib .= ');" onmouseout="return nd();"';
      return $overlib;
    }
  }
}
?>
