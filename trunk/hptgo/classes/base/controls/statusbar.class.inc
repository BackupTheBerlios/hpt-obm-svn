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

class statusbar
{
  var $usage_color = "#0000CC";
  var $background_color = "#f1f1f1";
  var $border_color = "#000000";
  var $width = "250";
  var $height = "10";
  var $info_text;
  var $turn_red_point = 0;

  /**
   * Print the progress bar.
   *
   * @param $usage
   * @param $limit
   * @return none
   */
  function print_bar($usage, $limit)
  {
    if ($limit != 0)
    {
      $percentage = ($usage/$limit)*100;
      $usage_width = number_format(($usage/$limit)*$this->width);
      $remaining_width = $this->width - $usage_width;
    }else
    {
      $usage_width= 0;
      $remaining_width = $this->width;
    }
    if ($this->turn_red_point > 0 && $percentage >= $this->turn_red_point)
    {
      $this->usage_color = '#FF0000';
    }

    echo '<table border="0" cellpadding="1" cellspacing="0">';
    echo '<tr><td bgcolor="'.$this->border_color.'">';
    echo '<table border="0" cellpadding="0" cellspacing="0" width="250">';
    echo '<tr><td height="'.$this->height.'" bgcolor="'.$this->usage_color.'" width="'.$usage_width.'"></td>';
    echo '<td height="'.$this->height.'" bgcolor="'.$this->background_color.'" width="'.$remaining_width.'"></td></tr>';
    echo '</table></td></tr>';
    echo '</table><table border="0" cellpadding="1" cellspacing="0">';
    echo '<tr><td class="small" height="20">'.number_format($percentage).'% '.$this->info_text.'</td></tr>';
    echo '</table>';
  }
}
?>
