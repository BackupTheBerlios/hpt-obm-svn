<?php
/*
   Copyright Intermesh 2003

   Author: Markus Schabel <markus.schabel@tgm.ac.at>
   Version: 1.0 Release date: 22 August 2003

   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.1 Release date: 29 March 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

class GO_AUTH
{
  //attempts to login a user and registers user_id in a session.
  //returns true on success. Stores general preferences in sessions
  //////////////////////////////////////////////////////////////
  function login($username, $password, $params) {
    global $GO_CONFIG, $GO_SECURITY, $GO_LANGUAGE, $GO_USERS, $GO_GROUPS,
           $GO_MODULES, $GO_LDAP;

    $GO_SECURITY->logged_in = false;
    $GO_SECURITY->user_id = 0;

    $GO_LDAP->search( "uid=$username", $GO_LDAP->PeopleDN );
    if ( $GO_LDAP->next_entry() ) {
      $dn = $GO_LDAP->dn();
      $username = substr( $dn, 0, strpos( $dn, "," ) );
      $username = substr( $username, strpos( $username, "=" )+1 );
    }

    if ( @$GO_LDAP->bind($dn, $password)) {
      // TODO put the profiles.class.inc into the dap.users.class.inc
      require_once($GO_CONFIG->class_path.'profiles.class.inc');
      $profiles = new profiles();

      // Search directory for user information...
      $user_id = 0;
      $GO_LDAP->search("uid=$username", $GO_LDAP->PeopleDN);

      if ($GO_LDAP->next_entry()) {
	$user_id = $GO_LDAP->first_value("uidNumber");
      }

      if ($user_id < 1) {
	// login failed
	return false;
      } else {
        // Login was successfully - that means username and password are
	// valid.
	// So we have to check if the user exists in our user management
	// database. If it does not exist we have to create him.
	// TODO couldn't be this get_user call be replaced with get_profile?
	if ( !$GO_USERS->get_user( $user_id ) ) {
	  // user doesn't exist in SQL => get profile from ldap
	  if ( !$profile = $profiles->get_profile( $user_id ) ) {
	    die('FATAL ERROR: Couldn\'t get profile from LDAP user');
	  }

	  if (!$user_id = $GO_USERS->add_user(
		$username, 
		$password, 
		$profile['first_name'],
		$profile['middle_name'],
		$profile['last_name'],
		$profile['initials'], 
		$profile['title'], 
		$profile['sex'], 
		$profile['birthday'], 
		$profile['email'], 
		'', //work_phone
		'', //home_phone
		'', //fax
		'', //cellular
		'', //country
		'', //state
		'', //city
		'', //zip
		'', //address
		'', //company
		'', //work_country
		'', //work_state
		'', //work_city
		'', //work_zip
		'', //work_address
		'', //work_fax
		'', //homepage
		'', //department
		'', //function
		'', //language
		'', //theme
		'', // start module
		true, //visible
		$user_id ) )
	  {
	    die('FATAL ERROR: Failed adding LDAP user to Group-Office');
	  }
	  
	  while ( $group_name = array_shift( $params['groups'] ) ) {
	    if ( $group = $GO_GROUPS->get_group_by_name( $group_name ) ) {
	      $GO_GROUPS->add_user_to_group($user_id, $group['id']);
	    }
	  }

	  while( $module_name = array_shift( $params['modules_read'] ) ) {
	    if( $module = $GO_MODULES->get_module( $module_name ) ) {
	      $GO_SECURITY->add_user_to_acl($user_id, $module['acl_read']);
	    }
	  }

	  while( $module_name = array_shift( $params['modules_write'] ) ) {
	    if( $module = $GO_MODULES->get_module( $module_name ) ) {
	      $GO_SECURITY->add_user_to_acl($user_id, $module['acl_write']);
	    }
	  }

	  $old_umask = umask(000);
	  @mkdir($GO_CONFIG->file_storage_path.$username, $GO_CONFIG->create_mode );
	  umask($old_umask);
	  // end of user creation part
	}

	$GO_USERS->update_session( $user_id );
	//Added by Meir Michanie
	//Used for two way email password encryption
	$_SESSION['GO_SESSION']['password'] = $password;
	$_SESSION['GO_SESSION']['old_password'] = $password;

	$GO_SECURITY->logged_in( $user_id );

	if($GO_CONFIG->dav_switch) {
	  global $GO_DAV;
	  $GO_DAV->check_login($username);
	}

	$GO_USERS->increment_logins( $user_id );

	return true;
      }      
    }
    return false;
  }
}
?>
