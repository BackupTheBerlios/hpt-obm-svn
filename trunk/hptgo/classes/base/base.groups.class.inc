<?php
/**
*  Copyright Intermesh 2003
* Author: Merijn Schering <mschering@intermesh.nl>
* Version: 1.0 Release date: 27 March 2004
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
*  option) any later version.
*
* 
*   Only add functions here that apply to all user managemers.
*/

/**
* This is the base class of the user group management files.
* Don't use this class directly it should be extended by a group manager.
* For example sql.groups.class.inc.
*
* @package  base.groups.class.inc
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 2.05
*/

class base_groups extends db
{
	/**
	* Constructor. Does nothing
	*
	* @access public
	* @return void
	*/
  function base_groups()
  {
    $this->db();    
  }
}
?>
