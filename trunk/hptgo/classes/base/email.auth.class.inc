<?php
/**
*  Copyright Intermesh 2003
*  Author: Merijn Schering <mschering@intermesh.nl>
*  Version: 1.0 Release date: 08 July 2003
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at your
*  option) any later version.
*/

/**
* This package can authenticate a user using an e-mail server supporting 
*	IMAP or POP-3
*
* @package  E-mail authentication class
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 2.05
*/

class GO_AUTH
{
  /**
   * This function logs a user in using a mail server
   * 
   * @access public
   * 
   * @param string $username
   * @param string $password
   * @param array $params	The authentication source specified in auth_sources.inc
   * 
   * @return bool
   */
  function login($username, $password, $params)
  {
    global $GO_CONFIG, $GO_SECURITY, $GO_LANGUAGE, $GO_USERS, $GO_GROUPS,
	   $GO_MODULES;

    $GO_SECURITY->logged_in = false;
    $GO_SECURITY->user_id = 0;

    require_once($GO_CONFIG->class_path.'imap.class.inc');
    $imap = new imap();

    $email_address = $username.'@'.$params['domain'];
    if ($params['add_domain_to_username']) {
      $username = $email_address;
    }

    if ($imap->open($params['host'], $params['proto'], $params['port'], $username, $password))
    {
      $imap->close();

      $_SESSION['GO_SESSION']['password'] = $password;//needed for encryption

      if ( $profile = $GO_USERS->get_profile_by_email( $email_address ) ) {
	$user_id = $profile['id'];
      } else {
	// the user does not exist, so we have to add him.
	if ( !$user_id = $GO_USERS->add_user(
	      $email_address, 
	      $password, 
	      '', 
	      '', 
	      '',
	      '',
	      '', 
	      'M',
	      '',
	      $email_address,
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '', 
	      '',
	      '', 
	      '', 
	      '',
	      '', 
	      '', 
	      ''))
	      {
		die('FATAL ERROR: Failed adding mail user to Group-Office');
	      } else {
		while($group_name = array_shift($params['groups']))
		{
		  if ($group = $GO_GROUPS->get_group_by_name($group_name))
		  {
		    $GO_GROUPS->add_user_to_group($user_id, $group['id']);
		  }
		}

		while($module_name = array_shift($params['modules_read']))
		{
		  if($module = $GO_MODULES->get_module($module_name))
		  {
		    $GO_SECURITY->add_user_to_acl($user_id, $module['acl_read']);
		  }
		}

		while($module_name = array_shift($params['modules_write']))
		{
		  if($module = $GO_MODULES->get_module($module_name))
		  {
		    $GO_SECURITY->add_user_to_acl($user_id, $module['acl_write']);
		  }
		}				

		$old_umask = umask( 000 );
		@mkdir( $GO_CONFIG->file_storage_path.$email_address, $GO_CONFIG->create_mode );
		umask($old_umask);

		if ($params['create_email_account'])
		{
		  $email_module = $GO_MODULES->get_module('email');

		  if($email_module)
		  {						
		    require_once($email_module['class_path']."email.class.inc");
		    require($GO_LANGUAGE->get_language_file('email'));
		    $email_client = new email();
		    if (!$account_id = $email_client->add_account($user_id, 
			  $params['proto'],
			  $params['host'], 
			  $params['port'], 
			  $params['mbroot'], 
			  $username, 
			  $password, 
			  $username, 
			  $email_address, 
			  "", 
			  $ml_sent_items, 
			  $ml_spam, 
			  $ml_trash))
		    {
		      require($GO_LANGUAGE->get_language_file('email'));
		      echo "<p class=\"Error\">".$registration_email_error."</p>";
		      echo "<p class=\"Error\">".$email_client->last_error."</p>";
		    }
		  }		
		}
		$profile = $GO_USERS->get_profile($user_id);	      
	      }
      }	
      $username = $email_address;
      //if local password is different from external the update it
      if(md5($password) != $profile['password'])
      {
	$GO_USERS->update_password($user_id, $password);

	$email_module = $GO_MODULES->get_module('email');

	if($email_module)
	{						
	  require_once($email_module['class_path']."email.class.inc");
	  $email_client = new email();
	  $email_client->update_password($params['host'], $username, $password);
	}  
      }

      $GO_USERS->update_session( $user_id );

      $GO_SECURITY->logged_in( $user_id );

      $GO_USERS->increment_logins( $user_id );

      return true;
    }
    return false;
  }
}
?>
