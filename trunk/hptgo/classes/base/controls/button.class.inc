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

class button
{
  /**
   * Constructor.
   *
   * @param string $text the text button. default is null. If $text is not null, then echo the button.
   * @param string $action default is null
   * @param int $size default is 100
   */
  function button($text=null, $action=null, $size='100')
  {
       if ($text){
       echo $this->get_button($text, $action, $size);
       }
  }

  /**
   * Get button.
   *
   * @param string $text the text button. default is null
   * @param string $action default is null
   * @param int $size default is 100
   * @return string the html string of the button.
   */
  function get_button($text, $action, $size='100')
  {
    return '<input type="button" class="button" style="width: '.$size.'px;" value="'.$text.'" onclick="'.$action.'" onmouseover="javascript:this.className=\'button_mo\';" onmouseout="javascript:this.className=\'button\';" />';
  }
}
?>
