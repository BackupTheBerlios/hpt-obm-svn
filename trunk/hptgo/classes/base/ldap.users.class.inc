<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Author: Markus Schabel <markus.schabel@tgm.ac.at>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   TODO when we fetch all users [get_users()], most times we only need
        the following attributes: name and uid, and a bit more information
	in the administration/users
 */

require_once($GO_CONFIG->class_path.'profiles.class.inc');

require_once($GO_CONFIG->class_path.'base/base.users.class.inc');

class GO_USERS extends base_users {
  var $user_id;
  var $profile;
  var $userlist;
  var $userlist_index;
  
  function GO_USERS() {
    global $GO_CONFIG;
    $this->db();

    /*
      TODO: Probably we could connect with our own ldap-uid and not as admin
      (or whatever is configured in GO as rootdn), so that we definitely can
      only see what we are allowed by LDAP access rights. So we cannot change
      attributes of other users. Probably administrator should bind with
      rootdn.
    */
    //$this->ldap->bind($_SESSION['GO_SESSION']['user'],
    //  $_SESSION['GO_SESSION']['password']);
  }

  function get_search_fields() {
    $searchfields[] = array( 'name', "Name" );
    return $searchfields;
  }

  function search($query, $field, $user_id, $start=0, $offset=0) {
    global $GO_LDAP;
    $query = substr( $query, 1, strlen( $query ) - 2 );
    switch( $field )
    {
      default:
	case "name":
	  $filter="(&(cn=*".utf8_encode($query)."*)(mail=*))";
	break;
    }
    $GO_LDAP->search($filter, $GO_LDAP->PeopleDN );
    //, array( "uidNumber", "uid", "cn"));
    $GO_LDAP->sort( "sn" );
    $ldapentries = $GO_LDAP->num_entries();
    $entries = $GO_LDAP->get_entries();
    $profile = new profiles();
    for ( $i=0; $i<$entries["count"]; $i++ ) {
      $this->userlist[] = $profile->convert_profile_ldap( $entries[$i] );
    }

    $this->userlist_index = 0;
    return count($this->userlist);
  }

  function get_users($sort="name",$direction="ASC", $start=0, $offset=0) {
    global $GO_LDAP;
    $GO_LDAP->search("(&(uid=*)(mail=*))", $GO_LDAP->PeopleDN ); //, array( "uidNumber", "uid", "cn"));
    $GO_LDAP->sort( "sn" );
    $ldapentries = $GO_LDAP->num_entries();
    $entries = $GO_LDAP->get_entries();
    $profile = new profiles();
//  if ( $offset == 0 ) { $offset = $entries["count"]; }
//  for ( $i=$start; ( $i<$entries["count"] ) && ( $i<$start+$offset ); $i++ ) {
    for ( $i=0; $i<$entries["count"]; $i++ ) {
      $this->userlist[] = $profile->convert_profile_ldap( $entries[$i] );
    }

//  sort( $this->userlist );
    $this->userlist_index = 0;
    return $entries["count"];
  }

  // TODO
  function get_authorized_users($user_id, $sort="name",$direction="ASC") {
    if ($sort == 'users.name' || $sort=='name') {
      $sort = 'users.first_name AND users.last_name';
    }
    $sql = "SELECT DISTINCT users.* FROM users, users_groups INNER JOIN acl ON users.acl_id= acl.acl_id WHERE ".
      "((acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id.") OR (".
      "acl.user_id = ".$user_id." )) ORDER BY ".$sort." ".$direction;

    $this->query($sql);
    return $this->num_rows();
  }

  function next_record() {
    if ( count( $this->userlist ) > $this->userlist_index ) {
      $this->Record = $this->userlist[$this->userlist_index++];
      return $this->Record;
    } else {
      return false;
    }
  }

  function get_profile_by_email($email) {
    if ( $uid = $this->get_user_id_by_email( $email ) ) {
      $profile = new profiles();
      $this->Record = $profile->get_profile( $uid );
      return $this->Record;
    }
    return false;
  }

  function get_user_id_by_email($email) {
    global $GO_LDAP;
    // I'm not sure if we really need this, because each LDAP user should be in
    // SQL too. But in LDAP you have the possibility to specify more than one
    // email address.
    $GO_LDAP->search("mail=$email", $GO_LDAP->PeopleDN);
    if ( $GO_LDAP->num_entries() > 0 ) {
      $GO_LDAP->next_entry();
      return $GO_LDAP->first_value("uidnumber");
    }
    return false;
  }

  function check_password($password) {
    global $GO_LDAP;
    // rebinding is not an optimal solution. hints for doing better are welcome...
    $ok = false;
    if ($GO_LDAP->bind("uid=".$_SESSION['GO_SESSION']['user_id'].",".$GO_LDAP->PeopleDN, $password)) {
      $ok = true;
    }
    $GO_LDAP->bind();
    return $ok;
  }

  function get_user($user_id) {
    $profile = new profiles();
    $this->Record = $profile->get_profile( $user_id );
    return $this->Record;
  }

  function update_profile($user_id, $first_name, $middle_name, $last_name,
      $initials, $title, $sex, $birthday, $email,
      $work_phone, $home_phone, $fax, $cellular,
      $country, $state, $city, $zip, $address, $company,
      $work_country, $work_state, $work_city, $work_zip,
      $work_address, $work_fax, $homepage, $department,
      $function) {
    global $GO_LDAP;
    $middle_name = trim($middle_name);

    $GO_LDAP->search("uidNumber=".$_SESSION['GO_SESSION']['user_id'], $GO_LDAP->PeopleDN);
    if ($GO_LDAP->num_entries() > 0) {
      $GO_LDAP->next_entry();
      // TODO: update ldap attributes if we are able to write. This needs
      // to be intelligent code because LDAP structure is mostly different.

      // Update session if update was ok.
/*    if ($user_id == $_SESSION['GO_SESSION']['user_id']) {
	$middle_name = $middle_name == '' ? '' : $middle_name.' ';
	$_SESSION['GO_SESSION']['name']  = $first_name.' '.$middle_name.$last_name;
	$_SESSION['GO_SESSION']['first_name']  = $first_name;
	$_SESSION['GO_SESSION']['middle_name']  = $middle_name;
	$_SESSION['GO_SESSION']['last_name']  = $last_name;
	$_SESSION['GO_SESSION']['email'] = $email;
      }*/
      // If the update was ok, then we can return true.
      // return true;
    }
    return false;
  }

  function update_password($user_id, $password,$old_password=null) {
    global $GO_CONFIG,$GO_CRYPTO,$GO_SESSION;
    if($profile = $this->get_user($user_id)) {
      // If we were able to find the user, we can change his password.
    }
    return false;
  }

  // TODO
  function update_authcode($authcode) {
    $sql = "UPDATE users SET authcode='$authcode' WHERE id='".$_SESSION['GO_SESSION']['user_id']."'";
    if ($this->query($sql)) {
      return true;
    } else {
      return false;
    }
  }


  function get_profile_by_username($username) {
    global $GO_LDAP;
    $GO_LDAP->search( "uid=$username", $GO_LDAP->PeopleDN );
    if ( $GO_LDAP->num_entries() > 0 ) {
      $GO_LDAP->next_entry();
      $uid = $GO_LDAP->first_value( "uidNumber" );
      $profile = new profiles();
      $this->Record = $profile->get_profile( $uid );
    }

    return $this->Record;
  }

  function email_exists($email) {
    global $GO_LDAP;
    $GO_LDAP->search("mail=$email", $GO_LDAP->PeopleDN);
    if ($GO_LDAP->num_entries() > 0) {
      return true;
    }
    return false;
  }
  
  
  /*
  MKS: Some behaviour has changed.
  Regional settings like language, currency decimal sep etc.
  are stored in language/locale.inc
  They should be retrieved with the given locale_code.
  See sql.users.class.inc for an example.
  */

  function add_user($username, $password, $first_name, $middle_name,
      $last_name, $initials, $title, $sex, $birthday,
      $email, $work_phone, $home_phone, $fax,
      $cellular, $country, $state, $city, $zip, $address,
      $company, $work_country, $work_state, $work_city,
      $work_zip, $work_address, $work_fax, $homepage,
      $department, $function, $locale_code='', $theme='', $start_module='',
      $visible=true, $user_id=-1) {
    global $GO_CONFIG;

    //don't forget to get preferences with new method.
    //See sql.user.class add_user()
    if ($language == '') {
      $language=$GO_CONFIG->language;
    }

    if ($theme == '') {
      $theme=$GO_CONFIG->theme;
    }

    if ($user_id < 0) {
      $user_id = $this->nextid("users");
    }
    if ($user_id > 0) {
      // TODO we need to add this user to LDAP.
      return -1;
    }
  }

  function max_users_reached() {
    global $GO_CONFIG;

    if( ( $this->get_users() < $GO_CONFIG->max_users ) ||
        ( $GO_CONFIG->max_users == 0 ) ) {
      return false;
    } else {
      return true;
    }
  }

  // TODO
  function set_preferences($user_id, $date_format, $time_format, $thousands_seperator, $decimal_seperator, $currency, $mail_client, $max_rows_list, $timezone_offset, $start_module, $language, $theme, $first_weekday)
  {
  	global $GO_LANGUAGE;
  	
      $_SESSION['GO_SESSION']['thousands_seperator'] = $thousands_seperator;
      $_SESSION['GO_SESSION']['decimal_seperator'] = $decimal_seperator;
      $_SESSION['GO_SESSION']['date_format']= $date_format;
      $_SESSION['GO_SESSION']['time_format']= $time_format;
      $_SESSION['GO_SESSION']['currency'] = $currency;
      $_SESSION['GO_SESSION']['mail_client'] = $mail_client;
      $_SESSION['GO_SESSION']['max_rows_list'] = $max_rows_list;
      $_SESSION['GO_SESSION']['timezone'] = $timezone_offset;
      $_SESSION['GO_SESSION']['start_module'] = $start_module;
      $_SESSION['GO_SESSION']['theme'] = $theme;
      $GO_LANGUAGE->set_language($profile['language']);
      $_SESSION['GO_SESSION']['first_weekday'] = $first_weekday;
  }

  function delete_user($user_id)
  {
    global $GO_CONFIG,$GO_SECURITY, $GO_MODULES, $GO_GROUPS;

    if($user = $this->get_user($user_id))
    {
      $acl_id = $this->f("acl_id");
      $username = $this->f("username");

      // TODO user should be deleted from LDAP (only!)
      $sql = "DELETE FROM users WHERE id='$user_id'";
      if ($this->query($sql))
      {
	$GO_SECURITY->delete_acl($acl_id);
	$GO_SECURITY->delete_user($acl_id);

	// TODO these module->delete_user should work via an enumeration of
	// all modules.

	if ($GO_MODULES->get_module('email'))
	{
	  require_once($GO_CONFIG->class_path."email.class.inc");
	  $email = new email();
	  $email->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('addressbook'))
	{
	  require_once($GO_CONFIG->class_path."addressbook.class.inc");
	  $ab = new addressbook();
	  $ab->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('scheduler'))
	{
	  require_once($GO_CONFIG->class_path."scheduler.class.inc");
	  $scheduler = new scheduler();
	  $scheduler->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('calendar'))
	{
	  require_once($GO_CONFIG->class_path."calendar.class.inc");
	  $calendar = new calendar();
	  $calendar->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('filesystem'))
	{
	  require_once($GO_CONFIG->class_path."filesystem.class.inc");
	  $filesystem = new filesystem();
	  $filesystem->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('projects'))
	{
	  require_once($GO_CONFIG->class_path."projects.class.inc");
	  $projects = new projects();
	  $projects->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('cms'))
	{
	  require_once($GO_CONFIG->class_path."cms.class.inc");
	  $cms = new cms();
	  $cms->delete_user($user_id);
	}

	if ($GO_MODULES->get_module('notes'))
	{
	  require_once($GO_CONFIG->class_path."notes.class.inc");
	  $notes = new notes();
	  $notes->delete_user($user_id);
	}

	require_once($GO_CONFIG->class_path."bookmarks.class.inc");
	$bookmarks = new bookmarks();
	$bookmarks->delete_user($user_id);

	$GO_GROUPS->delete_user($user_id);

	$sql = "SELECT * FROM acl_items WHERE user_id='$user_id'";
	$this->query($sql);
	while($this->next_record())
	{
	  $GO_SECURITY->delete_acl($this->f('id'));
	}

	system('rm -Rf '.$GO_CONFIG->file_storage_path.$username);
	return true;
      }
    }
    return false;
  }

  function increment_logins( $user_id ) {
    // TODO
  }
}
?>
