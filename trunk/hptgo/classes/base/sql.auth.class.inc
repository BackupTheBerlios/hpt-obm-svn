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

class GO_AUTH extends db
{
  function login($username, $password, $params) {
    global $GO_CONFIG, $GO_SECURITY, $GO_LANGUAGE, $GO_USERS;
    $GO_SECURITY->logged_in = false;
    $GO_SECURITY->user_id = 0;

    //maybe deprecated now with sepearte security classes?
    //$_SESSION['GO_SESSION']['auth_src'] = 'sql';
    
    $sql = "SELECT id FROM users WHERE username='$username' AND password='".md5($password)."'";
    $this->query($sql);
    if ($this->next_record()) {
      $user_id = $this->f('id');

      if($user_id > 0) {
	$GO_USERS->update_session( $user_id );

	$GO_SECURITY->logged_in( $user_id );

	if($GO_CONFIG->dav_switch) {
	  global $GO_DAV;
	  $GO_DAV->check_login($username);
	}
	//Who added this?
	//Added by Meir Michanie (OpenTTS developer)
	//Used for email password encryption.
	$_SESSION['GO_SESSION']['password'] = $password;
	$_SESSION['GO_SESSION']['old_password'] = $password;

	$GO_USERS->increment_logins( $user_id );

	return true;
      }
    }
    return false;
  }
}
?>
