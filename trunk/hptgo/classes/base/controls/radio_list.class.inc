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

class radio_list
{
  var $group;
  var $selected;

  /**
   * Constructor.
   *
   * @param string $group
   * @param string $selected default is empty
   */
  function radio_list($group, $selected='')
  {
    $this->group = $group;
    $this->selected = $selected;
  }

  /**
   * Add new option, print it too.
   *
   * @param string $value
   * @param string $text
   * @param boolean $disabled default is empty
   * @param string $attributes additional attributes (default is empty)
   */
  function add_option($value, $text, $disabled=false, $attributes='')
  {
    if ($this->selected == $value)
    {
      $selected = ' checked';
    }else
    {
      $selected = '';
    }

    $id = uniqid(time());

    $disabled = $disabled ? ' disabled' : '';
    $radio = '<input id="'.$id.'" type="radio" name="'.$this->group.'" value="'.$value.'"'.$selected.$disabled.' '.$attributes.' />';

    if ($text != '')
    {
      if ($disabled == ' disabled')
      {
	echo $radio.'<span style="color: #7d7d7d">'.$text.'</span>';
      }else
      {
	echo $radio.'<a href="javascript:select_radio(\''.$id.'\')">'.$text.'</a>';
      }
    }else
    {
      echo $radio;
    }
  }
}
?>
