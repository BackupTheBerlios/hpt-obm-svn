<?php
/**
*   Copyright Intermesh 2003
*
*   @author		Markus Schabel <markus.schabel@tgm.ac.at>
*   @version	1.0 Release date: 22 August 2003
*
*   @author		Merijn Schering <mschering@intermesh.nl>
*   @version	1.1 Release date: 29 March 2004
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at your
*   option) any later version.
*/

class GO_SECURITY extends base_security {

	/**
	* Constructor
	*/
  function GO_SECURITY() {
    global $GO_CONFIG;
    $this->base_security();
  }

	/**
	* Logged in
	* @param	int		$user ID
	*/
  function logged_in( $user_id ) {
    global $GO_LDAP;
    parent::logged_in( $user_id );

    // create temporary table
    $sql = "DELETE FROM users_groups WHERE user_id='$user_id'";
    $this->query( $sql );

    // Get the UserID Entries from LDAP for checking
    $GO_LDAP->search( "(uidNumber=$user_id)", $GO_LDAP->BaseDN );
    $GO_LDAP->next_entry();
    $uid = $GO_LDAP->first_value( "uid" );

    $GO_LDAP->search( "(&(gidNumber=*)(memberUid=$uid))",
      $GO_LDAP->BaseDN, array("gidNumber") );
    $groups = $GO_LDAP->get_entries();
    for ( $i=0; $i<$groups["count"]; $i++ ) {
      $sql = "INSERT INTO users_groups VALUES ( '".$groups[$i]["gidnumber"][0]."', '$user_id' )";
      $this->query( $sql );
    }
  }

  /**
  * Checks if a user has permission for an acl
  * @param	int		$user_id
  * @param	int		$acl_id
  * @return	bool	
  */
  function has_permission( $user_id, $acl_id ) {
    global $GO_CONFIG, $auth_sources, $GO_LDAP;

    if ($user_id > 0 && $acl_id > 0) {
      // First we check if the user directly has access to this ACL-ID.
      $sql = "SELECT acl_id FROM acl WHERE ".
	  "acl_id='$acl_id' AND user_id='$user_id'";
      $this->query($sql);
      if ($this->num_rows() > 0)
	return true;

      // Get the UserID Entries from LDAP for checking
      $GO_LDAP->search( "(uidNumber=$user_id)", $GO_LDAP->BaseDN );
      $GO_LDAP->next_entry();
      $uid = $GO_LDAP->first_value( "uid" );

      // We find out all groups that are associated with this ACL-ID, and
      // check if the user is member of any of them.
      $sql = "SELECT acl.group_id FROM acl WHERE acl.acl_id='".
	$acl_id."' AND acl.user_id='0' ORDER BY group_id ASC";
      $this->query($sql);
      $this->next_record();

      while ( $this->Record != "" ) {
	$result = $this->Record;
	$group_id = $result["group_id"];

	$GO_LDAP->search( "(&(gidNumber=$group_id)(memberUid=$uid))",
	    $GO_LDAP->BaseDN, array("sn") );
	if ( $GO_LDAP->num_entries() )  {
	  return true;
	}

	$this->next_record();
      }
    }
    return false;
  }

	/**
	* Get groups in acl
	* @param	int		$acl_id
	* @return	int		Number groups
	*/
  function get_groups_in_acl($acl_id)
  {
    global $GO_CONFIG, $auth_sources;
      $sql = "SELECT * FROM acl WHERE acl_id='$acl_id' AND user_id='0'";
      $this->query($sql);
      return $this->num_rows();
/*
  This is old code (non-ldap-um). Still in there since the code above isn't
  finished yet.

      $sql = "SELECT groups.* FROM groups INNER JOIN acl ON".
	" acl.group_id=groups.id WHERE acl.acl_id='$acl_id'".
	" ORDER BY groups.name";
      $this->query( $sql );
      return $this->num_rows();
*/
  }
}
?>
