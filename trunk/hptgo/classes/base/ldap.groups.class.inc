<?php
/**
*   Copyright Intermesh 2003
*   @author		Merijn Schering <mschering@intermesh.nl>
*   @author		Michael Borko <michael.borko@tgm.ac.at>
*   @author		Markus Schabel <markus.schabel@tgm.ac.at>
*   @version	1.0 Release date: 08 July 2003
*   @version	1.5 Release date: 27 February 2004
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at your
*   option) any later version.
*/

require_once ($GO_CONFIG->class_path.'base/base.groups.class.inc');

class GO_GROUPS extends base_groups
{
  var $ldap;
  var $grouplist;
  var $is_in_group;
  var $grouplist_index;

	/**
	* Constructor
	*/
  function GO_GROUPS()
  {
    global $GO_CONFIG, $auth_sources;

    // TODO do we really need the database here?!?
    $this->db();
  }

  // TODO (this function deletes a group)
  function delete_group($group_id)
  {
    global $GO_SECURITY;
    $GO_SECURITY->delete_group($group_id);
  }

  // TODO (this function removes all users from a group)
  function clear_group($group_id)
  {
    return false;
  }

  // TODO
  function add_user_to_group($user_id, $group_id)
  {
    if ( $user_id )
      return $this->query("INSERT INTO users_groups (user_id,group_id)".
	  " VALUES ('$user_id', '$group_id')");
    else
      return false;
  }

	/**
	* Delete a user
	* @param	int	$user_id
	*/
  function delete_user($user_id)
  {
    $sql = "DELETE FROM users_groups WHERE user_id='$user_id'";
    $this->query($sql);
    // We delete all groups which are owned by the user.
    $sql = "SELECT id FROM groups WHERE user_id='$user_id'";
    $this->query($sql);
    $del = new GO_GROUPS();
    while ( $this->next_record() ) {
      $del->delete_group($this->f("id"));
    }
  }

	/**
	* Delete user from group
	* @param	int		$user_id
	* @param	int		$group_id
	* @return	int		Handle of mysql_query
	*/
  function delete_user_from_group($user_id, $group_id)
  {
    return $this->query("DELETE FROM users_groups WHERE".
	" user_id='$user_id' AND group_id='$group_id'");
  }

	/**
	* Get a group
	* @param	int		$group_id
	* @return	mixed	array if success, otherwise : FALSE
	*/
  function get_group($group_id)
  {
    global $GO_LDAP;
    $GO_LDAP->search( "gidNumber=$group_id", $GO_LDAP->GroupsDN );
    if ( $GO_LDAP->num_entries() ) {
      $entry = $GO_LDAP->get_entries();
      $profile = new profiles();
      return $profile->convert_group_profile_ldap( $entry[0] );
    }
    return false;
  }

	/**
	* Update a group
	* @param	int		$group_id
	* @param	string	$name
	* @return	int		Handle of mysql_query	
	*/
  function update_group($group_id, $name)
  {
    return $this->query("UPDATE groups SET name='$name' WHERE id='$group_id'");
  }
  
  /**
  * Get a group by name
  * @param	string	$name
  * @return	mixed	array if success, otherwise : FALSE
  */
  function get_group_by_name($name)
  {
    global $GO_LDAP;
    $name = htmlentities($name);
    $GO_LDAP->search( "(|(cn=$name)(uid=$name))", $GO_LDAP->GroupsDN );
    if ( $GO_LDAP->num_entries() ) {
      $entry = $GO_LDAP->get_entries();
      $profile = new profiles();
      return $profile->convert_group_profile_ldap( $entry[0] );
    }
    return false;
  }

  /**
  * Add a new group
  * @param	int		$user_id
  * @param	string	$name
  * @return mixed	int (group ID) if success, otherwise : FALSE
  */
  function add_group($user_id, $name)
  {
    $name = htmlentities($name);
    $group_id = $this->nextid("groups");
    if ($group_id > 0) {
      $this->query("INSERT INTO groups (id, user_id, name) VALUES".
	  		" ('$group_id','$user_id','$name')");
      return $group_id;
    } else {
      return false;
    }
  }

	/**
	* Check a user in a group
	* @param	int		$user_id
	* @param	int		$group_id
	* @return	bool
	*/
  function user_owns_group($user_id, $group_id)
  {
    $this->query("SELECT user_id FROM groups WHERE user_id='$user_id' AND".
	" id='$group_id'");
    if ($this->num_rows() > 0) {
      return true;
    } else {
      return false;
    }
  }

  // updated by CSV - 2004-03-31: removed old code, since we can say for sure
  // that there are only uid's that exist in LDAP.
	/**
	* Check a user in a group
	* @param	int		$user_id
	* @param	int		$group_id
	* @return	bool
	*/
  function is_in_group($user_id, $group_id)
  {
    global $GO_CONFIG, $GO_USERS, $GO_LDAP;

    $GO_LDAP->search( "uidNumber=$user_id", $GO_LDAP->BaseDN, array("uid") );
    if ( $GO_LDAP->num_entries() == 0 ) {
      return false;
    } else {
      $GO_LDAP->next_entry();
      $uid = $GO_LDAP->first_value( "uid" );
    }
    $GO_LDAP->search( "(&(gidNumber=$group_id)(memberUid=$uid))",
                      $GO_LDAP->BaseDN );
    $GO_LDAP->next_entry();
    if ( $GO_LDAP->get_values("gidNumber") ) {
      return true;
    }
    return false;
  }

	/**
	* Get users in a group (LDAP)
	* @param	int		$group_id
	* @param	string	$sort
	* @param	string	$direction
	* @return	int		Number users in this group
	*/
  function get_users_in_group($group_id, $sort="name", $direction="ASC")
  {
    global $GO_CONFIG, $GO_LDAP, $GO_USERS;

    $GO_LDAP->search( "(gidNumber=$group_id)", $GO_LDAP->BaseDN );

    $GO_LDAP->next_entry();
    $this->is_in_group = $GO_LDAP->get_values("memberUid");
    
    //TODO Its not nice, but necessary to get the users from the group ...
    // ... maybe its possible to join the two things?!
    for ( $i=0; $i<$this->is_in_group["count"]; $i++ ) {
      $this->grouplist[] = $GO_USERS->get_profile_by_username( 
	$this->is_in_group[$i] );
    }
    
    $this->grouplist_index = 0;

    return $this->is_in_group["count"];
  }

	/**
	* Check a user in a group (both My SQL and LDAP)
	* @param	int		$user_id
	* @param	int		$group_id
	* @return	bool
	*/
  function group_is_visible($user_id, $group_id)
  {
    if ($this->user_owns_group($user_id, $group_id)
				|| $this->is_in_group($user_id, $group_id))
      return true;
    else
      return false;
  }

  /**
  * Gets all groups. Not for user display. Use get_authorised groups.
  */
  function get_all_groups()
  {
    $this->query("SELECT groups.*,users.username FROM groups, users WHERE".
	" groups.user_id = users.id ORDER BY groups.id ASC");
  }

  /**
  * Users can only see groups when they own it or are in it. Therefore this
  * complicated query.
  * @param	int		$user_id
  * @return	int		Number group where user be in
  */
  function get_authorised_groups($user_id)
  {
    global $GO_USERS, $GO_LDAP;
##Use the groups where the user is a member:
    $user = $GO_USERS->get_user($user_id);

    $GO_LDAP->search( "(memberUid=".$user["username"].")", $GO_LDAP->GroupsDN,
	  array( "gidnumber", "sn" ) );
    $GO_LDAP->sort( "sn" );
    $entries = $GO_LDAP->get_entries();

    $profile = new profiles();

    for ( $i = 0; $i<$entries["count"]; $i++ ) {
      $this->grouplist[] = $profile->convert_group_profile_ldap(
	    $entries[$i] );
    }
    $this->grouplist_index = 0;
    return count( $this->grouplist );
  }

	/**
	* Get next group
	* @return	mixed	array if success, otherwise : FALSE
	*/
  function next_record()
  {
    if ( count( $this->grouplist ) > $this->grouplist_index )
    {
      $this->Record = $this->grouplist[$this->grouplist_index++];
      return $this->Record;
    } else
      return false;
  }
	/**
	* Search a groupd
	* @param	string	$query
	* @param	string 	$field
	* @param	int		$user_id
	* @param	int		$start
	* @param	int		$offset
	* @return	int		Number found group
	*/
  function search($query, $field, $user_id, $start=0, $offset=0) {
##TODO: DONT SEARCH FOR GROUPS UNDER 1000 !!!
    $query = utf8_encode(substr( $query, 1, strlen( $query ) - 2 ));
    switch( $field ) {
      case "lehrer":
        if ( $query == "*" ) $filter="(&(cn=lehrer*)".
	  "(gidNumber=*)(sn=*))";
        else $filter="(&(cn=lehrer*$query*)".
          "(gidNumber=*)(sn=*))";
        $path="ou=Groups,ou=edu,dc=tgm,dc=ac,dc=at";
        break;
      case "schueler":
        if ( $query == "*" ) $filter="(&(cn=schueler*)".
	  "(gidNumber=*)(sn=*))";
        else $filter="(&(cn=schueler*$query*)".
  	  "(gidNumber=*)(sn=*))";
        $path="ou=Groups,ou=edu,dc=tgm,dc=ac,dc=at";
        break;
      case "admins":
        if ( $query == "*" ) $filter="(&(cn=*)".
	  "(gidNumber=*)(sn=*))";
        else $filter="(&(cn=*$query*)".
	  "(gidNumber=*)(sn=*))";
        $path="ou=Groups,ou=admin,dc=tgm,dc=ac,dc=at";
        break;
      default:
    }
    global $GO_LDAP;
    
    $GO_LDAP->search($filter, $path );
    $GO_LDAP->sort( "cn" );
    $ldapentries = $GO_LDAP->num_entries();
    $entries = $GO_LDAP->get_entries();

    $profile = new profiles();

    for ( $i=0; $i<$entries["count"]; $i++ )
      $this->grouplist[] = $profile->convert_group_profile_ldap(
	$entries[$i] );

    $this->grouplist_index = 0;
    return count($this->grouplist);
  }
}
?>
