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

class checkbox
{
  /**
   * Constructor. Echo the checkbox.
   *
   * @param string $name the checkbox id.
   * @param string $value the checkbox value
   * @param string $text the checkbox caption
   * @param boolean $checked checked status
   * @param boolean $disabled
   * @param string $attributes additional attributes, default is empty
   */
  function checkbox($name, $value, $text, $checked=false, $disabled=false, $attributes='')
  {
    $id = uniqid(time());
    $checked = $checked ? ' checked' : '';

    $disabled_str = $disabled ? ' disabled' : '';
    $checkbox = '<input id="'.$id.'" type="checkbox" name="'.$name.'" value="'.$value.'"'.$checked.$disabled_str.' '.$attributes.' />';

    if ($text != '')
    {
      if ($disabled)
      {
	echo $checkbox.'<span style="color: #7d7d7d">'.$text.'</span>';
      }else
      {
	echo $checkbox.'<a href="javascript:check_checkbox(\''.$id.'\')">'.$text.'</a>';
      }
    }else
    {
      echo $checkbox;
    }
  }
}
?>
