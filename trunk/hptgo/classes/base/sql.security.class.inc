<?php
/**
*   Copyright Intermesh 2003
*   @author		Merijn Schering <mschering@intermesh.nl>
*   @version	1.0 Release date: 08 July 2003
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at your
*   option) any later version.
*/

class GO_SECURITY extends base_security
{
	/**
	* Constructor
	*/
  function GO_SECURITY()
  {   
    $this->base_security();
  }

  /**
  * Checks if a user has permission for an acl
  * @param	int		$user_id
  * @param	int		$acl_id
  * @return	bool	true if success, false otherwise
  */
  function has_permission($user_id, $acl_id)
  {
    global $GO_CONFIG;

    if ($user_id > 0 && $acl_id > 0)
    {
      $sql = "SELECT {$this->acl_table}.acl_id FROM {$this->acl_table}, users_groups WHERE".
	" {$this->acl_table}.acl_id='$acl_id' AND ({$this->acl_table}.user_id='$user_id' OR".
	" ({$this->acl_table}.group_id=users_groups.group_id AND".
	" users_groups.user_id='$user_id')) GROUP BY {$this->acl_table}.acl_id";
      $this->query($sql);

      if ($this->num_rows() > 0)
      {
	return true;
      }
    }
    return false;
  }

}
?>
