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

if(!file_exists("Group-Office.php"))
{
  header("Location: install.php");
  exit();
}elseif(is_writable("Group-Office.php"))
{
  echo '<font color="red"><b>Group-Office.php is writable please chmod 755
    Group-Office.php and change the ownership to any other user then the
    webserver user.</b></font>';
  exit();
}
header('Content-Type: text/html; charset='.$charset);

$_COOKIE['GO_AUTH_SOURCE_KEY'] = isset($_COOKIE['GO_AUTH_SOURCE_KEY']) ? $_COOKIE['GO_AUTH_SOURCE_KEY'] : '0';
$_REQUEST['auth_source_key'] = isset($_REQUEST['auth_source_key']) ? $_REQUEST['auth_source_key'] : $_COOKIE['GO_AUTH_SOURCE_KEY'];

require("Group-Office.php");

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

require($GO_LANGUAGE->get_base_language_file('login'));

if ($task == "logout")
{
  SetCookie("GO_UN","",time()-3600,"/","",0);
  SetCookie("GO_PW","",time()-3600,"/","",0);
  unset($_SESSION);
  unset($_COOKIE);
  $GO_SECURITY->logout();
}

//when the user is logged in redirect him.
if ($GO_SECURITY->logged_in == true)
{
  $start_module = $GO_MODULES->get_module(
      $_SESSION['GO_SESSION']['start_module']);

  if (isset($_REQUEST['return_to']))
  {
    $link = $_REQUEST['return_to'];
  }elseif ( $start_module && ( $GO_SECURITY->has_permission(
	  $GO_SECURITY->user_id, $start_module['acl_read']) || 
	$GO_SECURITY->has_permission($GO_SECURITY->user_id, 
	  $start_module['acl_write']) ) )
  {
    $link = $start_module['url'];
  }else
  {
    $link = $GO_CONFIG->host.'configuration/preferences/';
  }
  require($GO_THEME->theme_path."frames.inc");
  exit();
}

//if form was posted user wants to login
//set cookies to remember login before headers are sent
if ( $_SERVER['REQUEST_METHOD'] == "POST" || (isset($_COOKIE['GO_UN']) 
      && isset($_COOKIE['GO_PW'])) )
{
  if ($_SERVER['REQUEST_METHOD'] != "POST")
  {
    $remind = true;
    $password = smart_addslashes($_COOKIE['GO_PW']);
    $username = smart_addslashes($_COOKIE['GO_UN']);
    $auth_source_key = isset($_COOKIE['GO_AUTH_SOURCE_KEY']) ? 
      $_COOKIE['GO_AUTH_SOURCE_KEY'] : 0;
  } else {
    $remind = isset($_POST['remind']) ? true : false;
    $username = smart_addslashes($_POST['username']);
    $password = smart_addslashes($_POST['password']);
    $auth_source_key= isset($_POST['auth_source_key']) ? 
      $_POST['auth_source_key'] : 0;
  }
  //check if both fields were filled
  if (!$username || !$password)
  {
    $feedback = "<p class=\"Error\">".$login_missing_field."</p>";
  } else {
    SetCookie("GO_AUTH_SOURCE_KEY",
	$auth_source_key, time()+3600*24*30,"/",'',0);
    $_COOKIE['GO_AUTH_SOURCE_KEY'] = $auth_source_key;

    //attempt login using security class inherited from index.php
    if ($GO_AUTH->login($username, $password, $auth_sources[$auth_source_key]))
    {
      //login is correct final check if login registration was ok
      if ($GO_SECURITY->logged_in == true)
      {
	if ($remind)
	{
	  SetCookie("GO_UN",$username,time()+3600*24*30,"/",'',0);
	  SetCookie("GO_PW",$password,time()+3600*24*30,"/",'',0);
	}

	if ($_SESSION['GO_SESSION']['first_name'] == '' || 
	    $_SESSION['GO_SESSION']['last_name'] == '' || 
	    $_SESSION['GO_SESSION']['email'] == '')
	{
	  header("Location: ".$GO_CONFIG->host.
	      "configuration/account/index.php");
	  exit();
	}else
	{
	  $start_module = $GO_MODULES->get_module(
	      $_SESSION['GO_SESSION']['start_module']);

	  if (isset($_REQUEST['return_to']))
	  {
	    $link = $_REQUEST['return_to'];
	  } elseif ($start_module && (
		$GO_SECURITY->has_permission($GO_SECURITY->user_id, 
		  $start_module['acl_read']) || 
		$GO_SECURITY->has_permission($GO_SECURITY->user_id, 
		  $start_module['acl_write'])))
	  {
	    $link = $start_module['url'];
	  } else
	  {
	    $link = $GO_CONFIG->host.'configuration/preferences/';
	  }
	  //redefine theme
	  $GO_THEME = new GO_THEME();
	  require($GO_THEME->theme_path."frames.inc");
	  exit();
	}
      }else
      {
	$feedback = "<p class=\"Error\">".$login_registration_fail."</p>";
      }
    }else
    {
      $feedback = "<p class=\"Error\">".$login_bad_login."</p>";
    }
  }
}
require('login.inc');
?>
