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

class color_selector
{
  var $colors = array();

  function add_color($color_code)
  {
    $this->colors[] = $color_code;
  }

  function print_color_selector($name, $selected_color, $form_name ='forms[0]')
  {
    global $GO_THEME;

    if (!in_array($selected_color, $this->colors))
    {
      $selected_color = $this->colors[0];
    }
    echo '<input type="hidden" name="'.$name.'" value="'.$selected_color.'" />';


    echo '<table border="0"><tr>';

    while ($color_code = array_shift($this->colors))
    {
      echo '<td id="'.$name.'_'.$color_code.'" style="background-color: #'.$color_code.'; border: 1px solid #aaa;"><a href="javascript:select_background_'.$name.'(\''.$color_code.'\');"><img border="0" height="16" width="16" src="'.$GO_THEME->images['blank'].'" /></a></td>';
    }
    echo '</tr></table>';

    echo "
      <script type=\"text/javascript\" language=\"javascript\">
      function select_background_".$name."(color)
      {
	active_table_cell = get_object('".$name."_'+document.".$form_name.".".$name.".value);
	table_cell = get_object('".$name."_'+color);
	active_table_cell.style.border = '1px solid #aaa';
	table_cell.style.border = '2px solid black';
	document.".$form_name.".".$name.".value = color;

      }
    select_background_".$name."('".$selected_color."');
    </script>";
  }
}
?>
