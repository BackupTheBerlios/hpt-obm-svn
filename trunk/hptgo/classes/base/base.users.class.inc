<?php
/**
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   MS TODO: Remove profiles.class.inc and integrate it into this class
   Also remove all uneened sql code since that is handled by sql.users.class.inc
   I think you should extend that class with this one and only redefine
   funtions that require different handling for LDAP.

   CSV TODO: Wouldn't it be better if the functions update_profile and
   add_user need only one parameter, which is the same array as we get as
   result from the get_* functions?

   CSV TODO: There are some inconsistencies regarding the function-names. What
   is the difference between get_user and get_profile? Or get_user_by_* and
   get_profile_by_*? We should define a naming scheme for these functions.

   CSV TODO: To minimize double-writen code we should think about simplifying
   the add_user function: This function only creates a basic user-entry, and
   calls update_profile and update_preferences to set the additional info.

   This file is supposed to be empty. Only if common user functions for all
   user managers come in the future they should be put in this file.
 */

/**
* This is the base class of the user management files.
* Don't use this class directly it should be extended by a user manager.
* For example sql.users.class.inc.
*
* @package  Users Base Class
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 2.05
*/

class base_users extends db
{
  /**
   * Initializes the SQL database.
   *
   * @access void
   * 
   * @param void
   * 
   * @return void
   */
  function users()
  {
    $this->db();  
  }

  /**
   * This function returns an array of the fields that can be used as search
   * criterias for users.
   * 
   * @access public
   * 
   * @param void
   * 
   * @return array
   */
  function get_search_fields() {
    return false;
  }

  /**
   * This function searches for users with the given search field.
   * 
   * @access public
   * 
   * @param string $query
   * @param string $field
   * @param int $user_id
   * @param int $start
   * @param int $offest
   * 
   * @return array
   */
  function search( $query, $field, $user_id, $start=0, $offset=0 ) {
    return false;
  }

  /**
   * This function ....
   * 
   * @access public
   * 
   * @param int $requesting_user_id
   * @param string $authcode
   * @param int $accepting_user_id
   * 
   * @return bool
   */
  function authorize( $requesting_user_id, $authcode, $accepting_user_id ) {
    return false;
  }

  /**
   * This function retrieves all users from the database and returns their
   * number. After that you are able to process each user via next_record.
   * 
   * @access public
   * 
   * @param string $sort
   * @param string $direction
   * @param int $start
   * @param int $offset
   * 
   * @return int
   */
  function get_users( $sort="name", $direction="ASC", $start=0, $offset=0 ) {
    return 0;
  }

  /**
   * This function retrieves all users ...
   * 
   * @access public
   * 
   * @param string $sort
   * @param string $direction
   * @param int $start
   * @param int $offset
   * 
   * @return int
   */
  function get_authorized_users($user_id, $sort="name",$direction="ASC") {
    return 0;
  }

  /**
   * This function retrieves all userdata based on the users email address.
   * 
   * @access public
   * 
   * @param string $email
   * 
   * @return array
   */
  function get_profile_by_email( $email ) {
    return false;
  }

  /**
   * This function retrieves the ID of a user based on his email address.
   * 
   * @access public
   * 
   * @param string $email
   * 
   * @return int
   */
  function get_user_id_by_email( $email ) {
    return false;
  }

  /**
   * This function checks if the password the user supplied is valid.
   * 
   * @access public
   * 
   * @param string $password
   * 
   * @return bool
   */
  function check_password($password) {
    return false;
  }

  /**
   * This function returns all userdata based on the users ID.
   * 
   * @access public
   * 
   * @param int $user_id
   * 
   * @return array
   */
  function get_user( $user_id ) {
    return false;
  }

  // TODO added by CSV:
  // Can anybody explain if there is any difference between get_user and
  // get_profile?? Or is it possible to drop get_profile (in sql.users.class
  // both implementations are exactly the same, and in ldap.users.class the
  // function get_profile does not exist)?

  /**
   * This function returns all userdata based on the users ID.
   * 
   * @access public
   * 
   * @param int $user_id
   * 
   * @return array
   */
  function get_profile( $user_id ) {
    return false;
  }

  /**
   * This function updates all userdata based on the given parameters.
   * 
   * @access public
   * 
   * @param int $user_id
   * @param string $first_name
   * @param string $middle_name
   * @param string $last_name
   * @param string $initials
   * @param string $title
   * @param string $sex
   * @param string $birthday
   * @param string $email
   * @param string $work_phone
   * @param string $home_phone
   * @param string $fax
   * @param string $cellular
   * @param string $country
   * @param string $state
   * @param string $city
   * @param string $zip
   * @param string $address
   * @param string $company
   * @param string $work_country
   * @param string $work_state
   * @param string $work_city
   * @param string $work_zip
   * @param string $work_address
   * @param string $work_fax
   * @param string $homepage
   * @param string $department
   * @param $function
   * 
   * @return bool
   */
  function update_profile( $user_id, $first_name, $middle_name, $last_name,
      $initials, $title, $sex, $birthday, $email, $work_phone, $home_phone,
      $fax, $cellular, $country, $state, $city, $zip, $address, $company,
      $work_country, $work_state, $work_city, $work_zip, $work_address,
      $work_fax, $homepage, $department, $function ) {
    return false;
  }

  /**
   * This function updates the user's password.
   * 
   * @access public
   * 
   * @param int $user_id
   * @param string $password
   * @param string $old_password
   * 
   * @return bool
   */
  function update_password( $user_id, $password,$old_password=null ) {
    return false;
  }

  /**
   * This function updates the user's authcode.
   * 
   * @access public
   * 
   * @param string %authcode
   * 
   * @return bool
   */
  function update_authcode( $authcode ) {
    return false;
  }

  /**
   * This function returns all userdata based on the user's name.
   * 
   * @access public
   * 
   * @param string $username
   * 
   * @return array
   */
  function get_profile_by_username( $username ) {
    return false;
  }

  /**
   * This function checks, if there is already a user with the given email
   * address.
   * 
   * @access public
   * 
   * @param string $email
   * 
   * @return bool
   */
  function email_exists( $email ) {
  }

  /**
   * This function adds a new user to the database.
   * 
   * @access public
   * 
   * @param string $username
   * @param string $password
   * @param string $first_name
   * @param string $middle_name
   * @param string $last_name
   * @param string $initials
   * @param string $title
   * @param string $sex
   * @param string $birthday
   * @param string $email
   * @param string $work_phone
   * @param string $home_phone
   * @param string $fax
   * @param string $cellular
   * @param string $country
   * @param string $state
   * @param string $city
   * @param string $zip
   * @param string $address
   * @param string $company
   * @param string $work_country
   * @param string $work_state
   * @param string $work_city
   * @param string $work_zip
   * @param string $work_address
   * @param string $work_fax
   * @param string $homepage
   * @param string $department
   * @param string $function
   * @param string $language
   * @param string $theme
   * @param bool $visible
   * @param int $user_id
   * 
   * @return bool
   */ 
  function add_user( $username, $password, $first_name, $middle_name,
      $last_name, $initials, $title, $sex, $birthday, $email, $work_phone,
      $home_phone, $fax, $cellular, $country, $state, $city, $zip, $address,
      $company, $work_country, $work_state, $work_city, $work_zip,
      $work_address, $work_fax, $homepage, $department, $function,
      $language='', $theme='', $start_module='', $visible=true, $user_id=-1 ) {
    return false;
  }

  /**
   * This function tells us if we exceeded the maximum number of users.
   * 
   * @access public
   * 
   * @param void
   * 
   * @return bool
   */
  function max_users_reached() {
  }

  /**
   * This function set's the preferences of the user.
   * 
   * @access public
   * 
   * @param int $user_id
   * @param string $date_format
   * @param string $time_format
   * @param char $thousands_seperator
   * @param char $decimal_seperator
   * @param char $currency
   * @param bool $mail_client
   * @param int $max_rows_list
   * @param string $timezone_offset
   * @param string $DST
   * @param string $start_module
   * @param string $language
   * @param string $theme
   * @param string $first_weekday
   * 
   * @return bool
   */
  function set_preferences( $user_id, $date_format, $time_format,
      $thousands_seperator, $decimal_seperator, $currency, $mail_client,
      $max_rows_list, $timezone_offset, $DST, $start_module, $language,
      $theme, $first_weekday ) {
    return false;
  }

  /**
   * This function deletes a user from the database.
   * 
   * @access public
   * 
   * @param int $user_id
   * 
   * @return bool
   */
  function delete_user( $user_id ) {
    return false;
  }

  /**
   * This function updates the number of logins of the user in the database.
   * 
   * @access public
   * 
   * @param int $user_id
   * 
   * @return bool
   */
  function increment_logins( $user_id ) {
    return false;
  }

  /**
   * Updates the session data corresponding to the user_id.
   * 
   * @access public
   * 
   * @param int $user_id
   * 
   * @return bool
   */
  function update_session( $user_id ) {
    global $GO_LANGUAGE;
    if ( $userdata = $this->get_user( $user_id ) ) {
      $middle_name = $userdata['middle_name'] ==  '' ?
	'' : $userdata['middle_name'].' ';
      $GO_LANGUAGE->set_language($userdata['language']);

      $_SESSION['GO_SESSION']['user_id']  = $user_id;
      $_SESSION['GO_SESSION']['username'] = $userdata['username'];
      $_SESSION['GO_SESSION']['authcode'] = $userdata['authcode'];
      $_SESSION['GO_SESSION']['name'] =
//	$userdata['first_name'].' '.$middle_name.$userdata['last_name'];
	$userdata['last_name'].' '.$middle_name.$userdata['first_name'];
      $_SESSION['GO_SESSION']['first_name'] = $userdata['first_name'];
      $_SESSION['GO_SESSION']['middle_name'] = $userdata['middle_name'];
      $_SESSION['GO_SESSION']['last_name'] = $userdata['last_name'];

      $_SESSION['GO_SESSION']['email'] = $userdata['email'];

      $_SESSION['GO_SESSION']['thousands_seperator'] =
	$userdata['thousands_seperator'];
      $_SESSION['GO_SESSION']['decimal_seperator']   =
	$userdata['decimal_seperator'];
      $_SESSION['GO_SESSION']['date_format'] = $userdata['date_format'];
      $_SESSION['GO_SESSION']['time_format'] = $userdata['time_format'];
      $_SESSION['GO_SESSION']['currency'] = $userdata['currency'];
      $_SESSION['GO_SESSION']['mail_client'] = $userdata['mail_client'];
      $_SESSION['GO_SESSION']['lastlogin'] = isset($userdata['lastlogin']) ? $userdata['lastlogin'] : get_gmt_time();
      $_SESSION['GO_SESSION']['max_rows_list'] = $userdata['max_rows_list'];
      $_SESSION['GO_SESSION']['timezone'] = $userdata['timezone'];
      $_SESSION['GO_SESSION']['start_module'] = isset($userdata['start_module']) ? $userdata['start_module'] : 'summary';
      $_SESSION['GO_SESSION']['DST'] = $userdata['DST'];
      /*--- Take care of $_SESSION['GO_SESSION']['timezone'] before uncommenting this
      if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
			{
				$_SESSION['GO_SESSION']['timezone'] = $_SESSION['GO_SESSION']['timezone']+$_SESSION['GO_SESSION']['DST'];
			}
*/
      $_SESSION['GO_SESSION']['theme'] = $userdata['theme'];
      $_SESSION['GO_SESSION']['first_weekday'] = $userdata['first_weekday'];
      return true;
    }
    return false;
  }

  /**
   * This function generates a randomized password.
   * 
   * @access public
   * 
   * @param string $characters_allow
   * @param string $characters_disallow
   * @param int $password_length
   * @param int $repeat
   * 
   * @return string
   */
  function random_password($characters_allow = 'a-z,1-9',$characters_disallow = 'i,o',$password_length = 8,$repeat = 0) {
    // Generate array of allowable characters.
    $characters_allow = explode( ',', $characters_allow );

    for ( $i = 0; $i < count( $characters_allow ); $i++ ) {
      if ( substr_count( $characters_allow[$i], '-' ) > 0 ) {
	$character_range = explode( '-', $characters_allow[$i] );

	for ($j=ord($character_range[0]);$j <= ord( $character_range[1] ); $j++) {
	  $array_allow[] = chr( $j );
	}
      } else {
	$array_allow[] = $characters_allow[$i];
      }
    }

    // Generate array of disallowed characters.

    $characters_disallow = explode( ',', $characters_disallow );

    for ( $i = 0; $i < count( $characters_disallow ); $i++ ) {
      if ( substr_count( $characters_disallow[$i], '-' ) > 0 ) {
	$character_range = explode( '-', $characters_disallow[$i] );

	for ( $j = ord( $character_range[0] );
	    $j <= ord( $character_range[1] ); $j++ ) {
	  $array_disallow[] = chr( $j );
	}
      } else {
	$array_disallow[] = $characters_disallow[$i];
      }
    }

    mt_srand( ( double ) microtime() * 1000000 );

    // Generate array of allowed characters by removing disallowed
    // characters from array.
    $array_allow = array_diff( $array_allow, $array_disallow );

    // Resets the keys since they won't be consecutive after
    // removing the disallowed characters.
    reset( $array_allow );
    $new_key = 0;
    while( list( $key, $val ) = each( $array_allow ) ) {
      $array_allow_tmp[$new_key] = $val;
      $new_key++;
    }

    $array_allow = $array_allow_tmp;
    $password = '';
    while ( strlen( $password ) < $password_length ) {
      $character = mt_rand( 0, count( $array_allow ) - 1 );

      // If characters are not allowed to repeat,
      // only add character if not found in partial password string.
      if ( $repeat == 0 ) {
	if (substr_count($password, $array_allow[$character])== 0) {
	  $password .= $array_allow[$character];
	}
      } else {
	$password .= $array_allow[$character];
      }
    }
    return $password;
  }
}
?>
