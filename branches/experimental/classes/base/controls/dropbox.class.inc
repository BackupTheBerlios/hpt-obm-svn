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

class dropbox
{
  var $value=array();
  var $text=array();
  var $optgroup;

  /**
   * Add new (text/value) to the dropbox.
   *
   * @param string $value
   * @param string $text
   * @return boolean
   */
  function add_value($value, $text)
  {
    if ($text != '')
    {
      $this->value[] = $value;
      $this->text[] = $text;
      return true;
    }else
    {
      return false;
    }
  }

  /**
   * Check if value $value (or text if $is_text is true) is in the dropbox.
   *
   * @param string $value
   * @param string $is_text
   * @return boolean
   */
  function is_in_dropbox($value, $is_text=false)
  {
    if ($is_text)
    {
      return in_array($value, $this->text);
    }else
    {
      return in_array($value, $this->value);
    }
  }

  /**
   * Add optgroup $name.
   *
   * @param string $name
   */
  function add_optgroup($name)
  {
    $this->optgroup[count($this->value)] = $name;
  }

  /**
   * Add all data from sql object named $sql_object to the dropbox.
   *
   * @param mixed $sql_object object name
   * @param string $value sql field to get value
   * @param string $text sql field to get text
   */
  function add_sql_data($sql_object, $value, $text)
  {
    global $$sql_object;

    while ($$sql_object->next_record())
    {
      $this->value[] = $$sql_object->f($value);
      $this->text[] = $$sql_object->f($text);
    }
  }

  /**
   * Add an array of values.
   *
   * @param string $value array of values
   * @param string $text the text
   * @return boolean
   */
  function add_arrays($value, $text)
  {
    if (is_array($this->value))
    {
      $this->value = array_merge($this->value, $value);
      $this->text = array_merge($this->text, $text);
    }else
    {
      $this->value = $value;
      $this->text = $text;
    }
    return true;
  }

  /**
   * Count options of the dropbox.
   *
   * @return int the count.
   */
  function count_options()
  {
    return count($this->value);
  }

  /**
   * Print the dropbox.
   *
   * @param string $name field name of the dropbox in form.
   * @param string $selected_field selected field(s) (default is empty)
   * @param string $attributes additional attributes (default is empty)
   * @param boolean $multiple allow multiple choice or not (default is false)
   * @param int $size default is 10
   * @param int $width default is 0
   */
  function print_dropbox($name, $selected_field='', $attributes='', $multiple=false, $size='10', $width='0')
  {
	echo $this->get_dropbox($name, $selected_field, $attributes, $multiple, $size, $width);	
  }

  /**
   * Return html code of  the dropbox.
   *
   * @param string $name field name of the dropbox in form.
   * @param string $selected_field selected field(s) (default is empty). this is an array if multiple selections
   * @param string $attributes additional attributes (default is empty)
   * @param boolean $multiple allow multiple choice or not (default is false)
   * @param int $size default is 10
   * @param int $width default is 0
   */
  function get_dropbox($name, $selected_field='', $attributes='', $multiple=false, $size='10', $width='0')
  {
    $return_str='';
    $multiple_str = $multiple ? ' multiple="true" size="'.$size.'"' : '';

    $optgroup_open = false;
    $return_str.= '<select name="'.$name.'" class="textbox"'.$multiple_str.' '.$attributes;

    if ($width > 0)
    {
      $return_str.=  'style="width: '.$width.'"';
    }
    $return_str.=  '>';


    for ($i=0;$i<count($this->value);$i++)
    {
      if(isset($this->optgroup[$i]))
      {
	if ($optgroup_open == true)
	{
	  $return_str.=  '</optgroup>';
	}else
	{
	  $optgroup_open = true;
	}
	$return_str.=  '<optgroup label="'.$this->optgroup[$i].'">';
      }

      if ($this->text[$i] != '')
      {
	$return_str.=  '<option value="'.$this->value[$i].'"';

	if ($multiple)
	{
	  if (in_array($this->value[$i], $selected_field))
	  {
	    $return_str.=  ' selected';
	  }
	}else
	{
	  if ($this->value[$i] == $selected_field)
	  {
	    $return_str.=  ' selected';
	  }
	}
	$return_str.=  '>';
	$return_str.=  htmlspecialchars($this->text[$i]);
	$return_str.=  '</option>';
      }
    }
    if ($optgroup_open == true)
    {
      $return_str.=  '</optgroup>';
    }
    $return_str.=  '</select>';
  return  $return_str;
  }
}
?>
