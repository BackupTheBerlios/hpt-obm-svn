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

require_once($GO_CONFIG->class_path.'base/base.users.class.inc');

class GO_USERS extends base_users
{
  function GO_USERS()
  {
    $this->base_users();   
  }

  function get_search_fields() {
    global $strFirstName, $strLastName, $strEmail,
    $strCompany, $strDepartment, $strFunction,
    $strAddress, $strCity, $strZip, $strCountry, $strState,
    $strWorkAddress, $strWorkZip, $strWorkCity, $strWorkState,
    $strWorkCountry;
    $searchfields[] = array( 'first_name',  $strFirstName );
    $searchfields[] = array( 'last_name',   $strLastName );
    $searchfields[] = array( 'email',	    $strEmail );
    $searchfields[] = array( 'company',	    $strCompany );
    $searchfields[] = array( 'department',  $strDepartment );
    $searchfields[] = array( 'function',    $strFunction );
    $searchfields[] = array( 'address',	    $strAddress );
    $searchfields[] = array( 'city',	    $strCity );
    $searchfields[] = array( 'zip',	    $strZip );
    $searchfields[] = array( 'state',	    $strState );
    $searchfields[] = array( 'country',	    $strCountry );
    $searchfields[] = array( 'work_address',$strWorkAddress );
    $searchfields[] = array( 'work_cip',    $strWorkZip );
    $searchfields[] = array( 'work_city',   $strWorkCity );
    $searchfields[] = array( 'work_state',  $strWorkState );
    $searchfields[] = array( 'work_country',$strWorkCountry );
    return $searchfields;
  }

  function search($query, $field, $user_id, $start=0, $offset=0)
  {    
    $sql = "SELECT DISTINCT users.* FROM users, users_groups INNER ".
      "JOIN acl ON users.acl_id= acl.acl_id WHERE ".
      "((acl.group_id = users_groups.group_id AND users_groups.user_id = ".
      $user_id.") OR (acl.user_id = ".$user_id." )) AND ($field LIKE '".smart_addslashes($query)."' ";
    // Additional Vietnamese glyphs
    if ($query == 'Đ%')
      $sql .= " OR $field LIKE 'đ%' ";
    $sql .= ") ORDER BY first_name ASC, last_name ASC";
    if ($offset != 0) {
      $this->query($sql);
      $count = $this->num_rows();
      $sql .= " LIMIT $start, $offset";
    }

    $this->query($sql);
    return isset($count) ? $count : $this->num_rows();
  }

  function authorize($requesting_user_id, $authcode, $accepting_user_id)
  {
    global $GO_SECURITY;

    $this->query("SELECT acl_id, id FROM users WHERE authcode='".
	smart_addslashes($authcode)."' AND id='$requesting_user_id'");
    if ($this->next_record())
    {
      $acl_id = $this->f("acl_id");
      if (!$GO_SECURITY->user_in_acl($accepting_user_id, $acl_id))
      {
	if (!$GO_SECURITY->add_user_to_acl($accepting_user_id,$acl_id))
	{
	  return false;
	}
      }
      $this->query("SELECT acl_id FROM users WHERE id='$accepting_user_id'");
      if ($this->next_record())
      {
	$acl_id = $this->f("acl_id");
	if (!$GO_SECURITY->user_in_acl($requesting_user_id, $acl_id))
	{
	  if (!$GO_SECURITY->add_user_to_acl($requesting_user_id,$acl_id))
	  {
	    return false;
	  }
	}
      }
      return true;
    }else
    {
      return false;
    }
  }

  function get_users($sort="name",$direction="ASC", $start=0, $offset=0)
  {
    if ($sort == 'name')
    {
      $sort = 'first_name '.$direction.', last_name';
    }
    $count=0;
    $this->query("SELECT id FROM users");
    if ($this->next_record())
    {
      $count = $this->num_rows();
    }

    if ($count > 0)
    {
      $sql = "SELECT * FROM users ORDER BY ".$sort." ".$direction;

      if ($offset != 0)
      {
	$sql .= " LIMIT $start,$offset";
      }
      $this->query($sql);
    }
    return $count;
  }

  function get_authorized_users($user_id, $sort="name",$direction="ASC",$first=0,$max_rows=0)
  {
    if ($sort == 'users.name' || $sort=='name')
    {
      $sort = 'users.first_name '.$direction.', users.last_name';
    }
    $sql = "SELECT DISTINCT users.* FROM users, users_groups ".
      "INNER JOIN acl ON users.acl_id= acl.acl_id WHERE ".
      "((acl.group_id = users_groups.group_id AND ".
      " users_groups.user_id = ".$user_id.") OR (".
      "acl.user_id = ".$user_id." )) ORDER BY ".$sort." ".$direction;

    if ($max_rows != 0) {
      $sql .= " LIMIT $first, $max_rows";

      $sql2 = "SELECT DISTINCT users.* FROM users, users_groups ".
	"INNER JOIN acl ON users.acl_id= acl.acl_id WHERE ".
	"((acl.group_id = users_groups.group_id AND ".
	" users_groups.user_id = ".$user_id.") OR (".
	"acl.user_id = ".$user_id." )) ORDER BY ".$sort." ".$direction;

      $this->query($sql2);
      $count = $this->num_rows();
      if ($count > 0) {
	$this->query($sql);
	return $count;
      }
      return 0;
    } else {
      $this->query($sql);
      return $this->num_rows();
    }
  }

  function get_profile_by_email($email)
  {
    $sql = "SELECT * FROM users WHERE email='$email'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function get_user_id_by_email($email)
  {
    $sql = "SELECT id FROM users WHERE email='$email'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->f("id");
    }
    return false;
  }

  function check_password($password)
  {
    $this->query("SELECT id FROM users WHERE password='".md5($password).
	"' AND id='".$_SESSION['GO_SESSION']['user_id']."'");
    if ($this->num_rows() > 0)
    {
      return true;
    }
    return false;
  }

  function get_user($user_id)
  {
    $sql = "SELECT * FROM users WHERE id='$user_id'";
    $this->query( $sql );
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function get_profile($user_id)
  {
    $sql = "SELECT * FROM users WHERE id='$user_id'";
    $this->query( $sql );
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function update_profile($user_id, $first_name, $middle_name, $last_name,
      $initials, $title, $sex, $birthday, $email,
      $work_phone, $home_phone, $fax, $cellular,
      $country, $state, $city, $zip, $address, $company,
      $work_country, $work_state, $work_city, $work_zip,
      $work_address, $work_fax, $homepage, $department,
      $function)
  {
    $middle_name = trim($middle_name);

    $sql = "UPDATE users SET first_name='$first_name', ".
      "middle_name='$middle_name', last_name='$last_name', ".
      "initials='$initials', title='$title', sex='$sex', ".
      "birthday='$birthday', email='$email', work_phone='$work_phone',".
      "home_phone='$home_phone', fax='$fax', cellular='$cellular',".
      "country='$country', state='$state', city='$city', zip='$zip', ".
      "address='$address',company='$company', department='$department',".
      "function='$function', work_country='$work_country', ".
      "work_state='$work_state', work_city='$work_city',".
      "work_zip='$work_zip', work_address='$work_address', ".
      "work_fax='$work_fax', homepage='$homepage' WHERE id='$user_id'";
    if ($this->query($sql))
    {
      if ($user_id == $_SESSION['GO_SESSION']['user_id'])
      {
	$middle_name = $middle_name == '' ? '' : $middle_name.' ';

//	$_SESSION['GO_SESSION']['name']  = $first_name.' '.$middle_name.$last_name;
	$_SESSION['GO_SESSION']['name']  = $last_name.' '.$middle_name.$first_name;
	$_SESSION['GO_SESSION']['first_name']  = $first_name;
	$_SESSION['GO_SESSION']['middle_name']  = $middle_name;
	$_SESSION['GO_SESSION']['last_name']  = $last_name;
	$_SESSION['GO_SESSION']['email'] = $email;
      }      
      return true;
    }
    return false;
  }

  function update_password($user_id, $password,$old_password=null)
  {
    global $GO_CONFIG;

    if($profile = $this->get_user($user_id))
    {
      $sql = "UPDATE users SET password='".md5($password)."' WHERE id='$user_id'";
      if ($this->query($sql))
      {
	$_SESSION['GO_SESSION']['password'] = $password;
	return true;
      }

    }
    return false;
  }

  function update_authcode($authcode)
  {
    $sql = "UPDATE users SET authcode='$authcode' WHERE id='".$_SESSION['GO_SESSION']['user_id']."'";
    if ($this->query($sql))
    {
      return true;
    }else
    {
      return false;
    }
  }


  function get_profile_by_username($username)
  {
    $sql = "SELECT * FROM users WHERE username='$username'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function email_exists($email)
  {
    $sql = "SELECT id FROM users WHERE email='$email'";
    $this->query($sql);
    if ($this->num_rows() > 0)
    {
      return true;
    }
    return false;
  }

  // the following fields are removed now: acl_id, authcode
  function add_user($username, $password, $first_name, $middle_name,
      $last_name, $initials, $title, $sex, $birthday,
      $email, $work_phone, $home_phone, $fax,
      $cellular, $country, $state, $city, $zip, $address,
      $company, $work_country, $work_state, $work_city,
      $work_zip, $work_address, $work_fax, $homepage,
      $department, $function, $language='', $theme='',
      $visible=true, $user_id=-1)
  {
    global $GO_CONFIG, $GO_LANGUAGE, $GO_SECURITY, $GO_GROUPS;
    require($GO_LANGUAGE->language_path.'languages.inc');

    // We check if we are able to add a new user. If we already have too much
    // of them we do not want new ones ;)
    if ( $this->max_users_reached() ) {
      return false;
    }
    // We check if a user with this email address already exists. Since the
    // email address is used as key for the acl_id, no two users may have the
    // same address. It also should not be possible to have multiple users
    // with the same name...
    $this->query( "SELECT id FROM users WHERE
	email='$email' OR username='$username'" );
    if ( $this->num_rows() > 0 ) {
      return false;
    }

    //if local_code is empty try HTTP_ACCEPT_LANGUAGE
    //if that failes use default from GO_CONFIG
    $hal = $_SERVER['HTTP_ACCEPT_LANGUAGE'] ? $_SERVER['HTTP_ACCEPT_LANGUAGE'] : $GO_CONFIG->language;
    $language = ($language == '') ? $hal : $language;

    if (isset($languages[$language])) {
      $language_info = $languages[$language];
    } else {
      $language_info = $languages[$GO_LANGUAGE->default_language];
    }

    $currency = isset($language_info['currency']) ?
      $language_info['currency'] : 'EUR';
    $decimal_seperator = isset($language_info['decimal_seperator']) ?
      $language_info['decimal_seperator'] : ',';
    $thousands_seperator = isset($language_info['thousands_seperator']) ?
      $language_info['thousands_seperator'] : '.';
    $time_format = isset($language_info['time_format']) ?
      $language_info['time_format'] : $GO_CONFIG->time_formats[0];
    $date_format = isset($language_info['date_format']) ?
      $language_info['date_format'] : $GO_CONFIG->date_formats[0];
    $first_weekday = isset($language_info['first_weekday']) ?
      $language_info['first_weekday'] : '1';
    $timezone = isset($language_info['timezone']) ?
      $language_info['timezone'] : '1';
    $DST = isset($language_info['DST']) ?
      $language_info['DST'] : '0';


    if ($theme == '') {
      $theme=$GO_CONFIG->theme;
    }
    if ($user_id < 0) {
      $user_id = $this->nextid("users");
    }
    $acl_id = $GO_SECURITY->get_new_acl( $email );
    $authcode = $this->random_password();

    if ($user_id > 0)
    {
      $sql = "INSERT INTO users (id, username, password, first_name, ".
	"middle_name, last_name, initials, title, sex, birthday, ".
	"email, authcode, work_phone, home_phone, fax, cellular, ".
	"country, state, city, zip, address, company, work_country,".
	"work_state, work_city, work_zip, work_address, work_fax,".
	"homepage, department, function, acl_id, registration_time, ".
	"currency, max_rows_list, timezone, DST, date_format, ".
	"time_format, language, theme, first_weekday, decimal_seperator, thousands_seperator) ".
	"VALUES ('$user_id', '$username','".md5($password)."', ".
	"'$first_name', '$middle_name', '$last_name', '$initials', ".
	"'$title', '$sex', '$birthday', '$email', '$authcode', ".
	"'$work_phone', '$home_phone', '$fax', '$cellular', '$country',".
	"'$state', '$city', '$zip', '$address', '$company', ".
	"'$work_country', '$work_state', '$work_city', '$work_zip', ".
	"'$work_address', '$work_fax', '$homepage', '$department',".
	"'$function', '$acl_id', '".get_gmt_time()."', '$currency', '15', ".
	"'$timezone', '$DST', '".
	$date_format."', '".$time_format.
	"', '$language', '$theme', '".$first_weekday."', '$decimal_seperator', '$thousands_seperator')";

      if ($this->query($sql)) {
	$GO_SECURITY->set_acl_owner( $acl_id, $user_id );
	$GO_GROUPS->add_user_to_group( $user_id, $GO_CONFIG->group_everyone );
	if ( $visible ) {
	  $GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $acl_id);
	}
	$GO_SECURITY->add_group_to_acl($GO_CONFIG->group_root, $acl_id);
	return $user_id;
      } else {
	$GO_SECURITY->delete_acl( $acl_id );
      }
    }
    return false;
  }

  function max_users_reached()
  {
    global $GO_CONFIG;

    if($this->get_users() < $GO_CONFIG->max_users || $GO_CONFIG->max_users == 0)
    {
      return false;
    }else
    {
      return true;
    }
  }

  function set_preferences($user_id, $date_format, $time_format, 
      $thousands_seperator, $decimal_seperator, 
      $currency, $mail_client, $max_rows_list, 
      $timezone_offset,$DST, $start_module, 
      $language, $theme, $first_weekday)
  {
    global $GO_LANGUAGE; 

    if($this->query("UPDATE users SET time_format='$time_format', ".
	  "date_format='$date_format', ".
	  "thousands_seperator='$thousands_seperator', ".
	  "decimal_seperator='$decimal_seperator', ".
	  "currency='$currency', ".
	  "mail_client='$mail_client', max_rows_list='$max_rows_list', ".
	  "timezone='$timezone_offset', ".
	  "DST='$DST', ".
	  "start_module='$start_module', ".
	  "theme='$theme', ".
	  "language='$language', ".
	  "first_weekday='$first_weekday' ".

	  "WHERE id='$user_id'"))
    {
      $GO_LANGUAGE->set_language($language);      
      $this->update_session($user_id);
    }
  }

  function set_notations($user_id, $date_format, $time_format, 
      $thousands_seperator, $decimal_seperator, 
      $currency,  $timezone_offset,$DST, $first_weekday)
  {
    global $GO_LANGUAGE; 

    if($this->query("UPDATE users SET time_format='$time_format', ".
	  "date_format='$date_format', ".
	  "thousands_seperator='$thousands_seperator', ".
	  "decimal_seperator='$decimal_seperator', ".
	  "currency='$currency', ".
	  "timezone='$timezone_offset', ".
	  "DST='$DST', ".
	  "first_weekday='$first_weekday', ".

	  "WHERE id='$user_id'"))
    {
      $this->update_session($user_id);
    }
  }
  
  function set_look_and_feel($user_id,
      $max_rows_list, 
      $start_module, 
      $language, 
			$theme)
  {
    global $GO_LANGUAGE; 

    if($this->query("UPDATE users SET max_rows_list='$max_rows_list', ".	  
	  "start_module='$start_module', ".
	  "theme='$theme', ".
	  "language='$language' ".
	  "WHERE id='$user_id'"))
    {
      $GO_LANGUAGE->set_language($language);      
      $this->update_session($user_id);

    }
  }
  

  function delete_user($user_id)
  {
    global $GO_CONFIG,$GO_SECURITY, $GO_MODULES, $GO_GROUPS;

    if($user = $this->get_user($user_id))
    {
      $acl_id = $this->f("acl_id");
      $username = $this->f("username");
      $sql = "DELETE FROM users WHERE id='$user_id'";
      if ($this->query($sql))
      {
	$GO_SECURITY->delete_acl($acl_id);
	$GO_SECURITY->delete_user($acl_id);
	$email_module = $GO_MODULES->get_module('email');
	if ($email_module)
	{
	  require_once($email_module['class_path']."email.class.inc");
	  $email = new email();
	  $email->delete_user($user_id);
	}

	$ab_module = $GO_MODULES->get_module('addressbook');
	if ($ab_module)
	{
	  require_once($ab_module['class_path']."addressbook.class.inc");
	  $ab = new addressbook();
	  $ab->delete_user($user_id);
	}

	$cal_module = $GO_MODULES->get_module('calendar');
	if ($cal_module)
	{
	  require_once($cal_module['class_path']."calendar.class.inc");
	  $calendar = new calendar();
	  $calendar->delete_user($user_id);
	}

	$fs_module =$GO_MODULES->get_module('filesystem'); 
	if ($fs_module)
	{
	  require_once($GO_CONFIG->class_path."filesystem.class.inc");
	  $filesystem = new filesystem();
	  $filesystem->delete_user($user_id);
	}

	$pr_module = $GO_MODULES->get_module('projects');
	if ($pr_module)
	{
	  require_once($pr_module['class_path']."projects.class.inc");
	  $projects = new projects();
	  $projects->delete_user($user_id);
	}

	$cms_module = $GO_MODULES->get_module('cms');
	if ($cms_module)
	{
	  require_once($cms_module['class_path']."cms.class.inc");
	  $cms = new cms();
	  $cms->delete_user($user_id);
	}

	$notes_module = $GO_MODULES->get_module('notes');
	if ($notes_module)
	{
	  require_once($notes_module['class_path']."notes.class.inc");
	  $notes = new notes();
	  $notes->delete_user($user_id);
	}

	$bm_module = $GO_MODULES->get_module('bookmarks');
	if ($bm_module)
	{
	  require_once($bm_module['class_path']."bookmarks.class.inc");
	  $bookmarks = new bookmarks();
	  $bookmarks->delete_user($user_id);
	}

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
    $sql =  "UPDATE users SET logins=logins+1, lastlogin='".get_gmt_time().
      "' WHERE id='$user_id'";
    $this->query( $sql );
  }
}
?>
