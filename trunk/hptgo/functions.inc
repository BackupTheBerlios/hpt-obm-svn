<?php
/**
* Copyright Intermesh 2003
* Author: Merijn Schering <mschering@intermesh.nl>
* Version: 1.0 Release date: 08 July 2003
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*/

/**
* This file holds global functions for use inside Group-Office
*
* @package  Functions
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 1.0
*/

// New function, use only when timezone problem is gone
function get_timezone_offset($time)
{
  if($_SESSION['GO_SESSION']['DST'] > 0 && date('I', $time) == 0)
	{
		return $_SESSION['GO_SESSION']['timezone']-$_SESSION['GO_SESSION']['DST'];
	}else
	{
		return $_SESSION['GO_SESSION']['timezone'];
	}
}

/**
* Adds paramaters to an URL
*
* @param	string $url
* @param	string $params
* @access public
* @return string
*/

// New function, use only when timezone problem is gone
function add_params_to_url($url, $params)
{
	if(strpos($url, '?') === false)
	{
		$url .= '?'.$params;
	}else
	{
		$url .= '&'.$params;
	}
	return $url;
}

/**
* Get's all queries from an SQL dump file in an array
*
* @param	string $file The absolute path to the SQL file
* @access public
* @return array An array of SQL strings
*/

function get_sql_queries($file)
{
  $sql ='';
  $queries=array();
  if($handle = fopen ($file, "r"))
  {
    while (!feof ($handle))
    {
      $buffer = trim(fgets($handle, 4096));
      if($buffer != '' && strpos($buffer, '#') === false)
      {
	$sql .= $buffer;
      }
    }
    fclose ($handle);
  }else
  {
    die("Could not read SQL dump file $file!");
  }
  $length = strlen($sql);
  $in_string=false;
  $start = 0;
  $escaped = false;
  for ($i=0;$i<$length;$i++)
  {
    $char = $sql[$i];
    if ($char == '\'' && !$escaped)
    {
      $in_string = !$in_string;
    }
    if ($char == ';' && !$in_string)
    {
      $offset = $i - $start;
      $queries[] = substr($sql, $start, $offset);

      $start = $i+1;
    }
    if ($char == '\\')
    {
      $escaped = true;
    }else
    {
      $escaped = false;
    }
  }
  return $queries;
}

/**
* Converts a Group-Office date to unix time.
*
* A Group-Office date is formated by user preference.
*
* @param	string $date_string The date string formated in the user's preference
* @access public
* @return int unix timestamp
*/

function date_to_unixtime($date_string)
{
  $date_string = trim($date_string);
  if ($date_string != '')
  {
    $date_string = str_replace('/', '-', $date_string);

    $datetime_array = explode(' ', $date_string);

    $date = isset($datetime_array[0]) ? $datetime_array[0] : '0000-00-00';

    $date_array = explode('-',$datetime_array[0]);
    $year = isset($date_array[2]) ? $date_array[2] : 0;

    if ($_SESSION['GO_SESSION']['date_format'] == "m-d-Y")
    {
      $month = isset($date_array[0]) ? $date_array[0] : 0;
      $day = isset($date_array[1]) ? $date_array[1] : 0;
    }else
    {
      $month = isset($date_array[1]) ? $date_array[1] : 0;
      $day = isset($date_array[0]) ? $date_array[0] : 0;
    }

    $time = isset($datetime_array[1]) ? $datetime_array[1] : '00:00:00';
    $time_array = explode(':', $time);

    $hour = isset($time_array[0]) ? $time_array[0] : 0;
    $min = isset($time_array[1]) ? $time_array[1] : 0;
    $sec = isset($time_array[2]) ? $time_array[2] : 0;

    return mktime($hour,$min,$sec,$month, $day, $year);
  }
  return false;
}

/**
* Convert a Group-Office date to MySQL date format
*
* A Group-Office date is formated by user preference.
*
* @param	string $date_string The Group-Office date string
* @param	bool $with_time The output sting should contain time too
* @access public
* @return int unix timestamp
*/

function date_to_db_date($date_string, $with_time=false)
{
  $date_string = str_replace('/', '-', $date_string);

  $datetime_array = explode(' ', $date_string);

  $date = isset($datetime_array[0]) ? $datetime_array[0] : '0000-00-00';

  $date_array = explode('-',$datetime_array[0]);
  $year = isset($date_array[2]) ? $date_array[2] : 0;

  if ($_SESSION['GO_SESSION']['date_format'] == "m-d-Y")
  {
    $month = isset($date_array[0]) ? $date_array[0] : 0;
    $day = isset($date_array[1]) ? $date_array[1] : 0;
  }else
  {
    $month = isset($date_array[1]) ? $date_array[1] : 0;
    $day = isset($date_array[0]) ? $date_array[0] : 0;
  }

  $time = isset($datetime_array[1]) ? $datetime_array[1] : '00:00:00';
  $time_array = explode(':', $time);

  $hour = isset($time_array[0]) ? $time_array[0] : 0;
  $min = isset($time_array[1]) ? $time_array[1] : 0;
  $sec = isset($time_array[2]) ? $time_array[2] : 0;

  $date_format = $with_time ? 'Y-m-d H:i' : 'Y-m-d';

  $db_date = "$year-$month-$day";
  if ($with_time)
  {
    $db_date .= ' '.date($_SESSION['GO_SESSION']['timeformat'], mktime($hour, $min, $sec));
  }

  return $db_date;
}

/**
* Convert a MySQL date to Group-Office date.
*
* A Group-Office date is formated by user preference.
*
* @param	string 	$date_string The MySQL date string
* @param	bool 		$with_time The output sting should contain time too
* @access public
* @return string Group-Office date string
*/

function db_date_to_date($date_string, $with_time=false)
{
  $datetime_array = explode(' ', $date_string);

  $date = isset($datetime_array[0]) ? $datetime_array[0] : '0000-00-00';

  $date_array = explode('-',$datetime_array[0]);

  $year = isset($date_array[0]) ? $date_array[0] : 0;
  $month = isset($date_array[1]) ? $date_array[1] : 0;
  $day = isset($date_array[2]) ? $date_array[2] : 0;

  $time = isset($datetime_array[1]) ? $datetime_array[1] : '00:00:00';
  $time_array = explode(':', $time);

  $hour = isset($time_array[0]) ? $time_array[0] : 0;
  $min = isset($time_array[1]) ? $time_array[1] : 0;
  $sec = isset($time_array[2]) ? $time_array[2] : 0;

  if ($_SESSION['GO_SESSION']['date_format'] == "m-d-Y")
  {
    $date = "$month-$day-$year";
  }else
  {
    $date = "$day-$month-$year";
  }

  if ($with_time)
  {
    $date .= ' '.date($_SESSION['GO_SESSION']['timeformat'], mktime($hour, $min, $sec));
  }

  return $date;

}

/**
* Convert a MySQL date to unix time format
*
* @param	int			$date_string The MySQL date string
* @access public
* @return int unix timestamp
*/

function db_date_to_unixtime($date_string)
{
  $datetime_array = explode(' ', $date_string);

  $date = isset($datetime_array[0]) ? $datetime_array[0] : '0000-00-00';

  $date_array = explode('-',$datetime_array[0]);

  $year = isset($date_array[0]) ? $date_array[0] : 0;
  $month = isset($date_array[1]) ? $date_array[1] : 0;
  $day = isset($date_array[2]) ? $date_array[2] : 0;

  $time = isset($datetime_array[1]) ? $datetime_array[1] : '00:00:00';
  $time_array = explode(':', $time);

  $hour = isset($time_array[0]) ? $time_array[0] : 0;
  $min = isset($time_array[1]) ? $time_array[1] : 0;
  $sec = isset($time_array[2]) ? $time_array[2] : 0;

  return mktime($hour,$min,$sec,$month, $day, $year);
}

/**
* Convert a unix time stamp to MySQL date format
*
* @param	int 		$date_string The MySQL date string
* @access public
* @return string MySQL timestamp
*/

function unixtime_to_db_date($unixtime)
{
	return date('Y-m-d');
}

/**
* Return only the contents of the body tag from a HTML page
*
* @param	string $html A HTML formatted string
* @access public
* @return string HTML formated string
*/

function get_html_body($html)
{
  $to_removed_array = array (
      "'<html[^>]*>'si",
      "'</html>'si",
      "'<body[^>]*>'si",
      "'</body>'si",
      "'<head[^>]*>.*?</head>'si",
      "'<style[^>]*>.*?</style>'si",
      "'<object[^>]*>.*?</object>'si",
      );

  //$html = str_replace("\r", "", $html);
  //$html = str_replace("\n", "", $html);

  $html = preg_replace($to_removed_array, '', $html);
  return $html;

}

/**
* Get the user's local time using the timezone offset preference.
*
* @param	string $server_time The time according to the server. 
*					If empty time() is used.
* @access public
* @return int unix timestamp
*/

function get_time($server_time=0)
{
  if ($server_time == '0')
  {
    $server_time = time();
  }

  $server_timezone_offset = date('Z');

  if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
  {
	$dst_offset = 3600 * $_SESSION['GO_SESSION']['DST'];
  }else
  {
	$dst_offset = 0;
  }
  
  $time = $server_time+(($_SESSION['GO_SESSION']['timezone']*3600)-$server_timezone_offset)+$dst_offset;
  return $time;
}

/**
* Get current GMT time.
*
* @param	string $local_time The local time of the logged in user.
*					If empty the current time in GMT will be returned.
* @access public
* @return int unix timestamp
*/
function get_gmt_time($local_time=0)
{
	if($local_time == 0)
	{
	  $server_timezone_offset = date('Z');
  	$time = time()-$server_timezone_offset;
  }else
  {
  	if($_SESSION['GO_SESSION']['DST'] > 0 && date('I') > 0)
	  {
		$dst_offset = 3600 * $_SESSION['GO_SESSION']['DST'];
	  }else
	  {
		$dst_offset = 0;
	  }
  	$time = $local_time-($_SESSION['GO_SESSION']['timezone']*3600)-$dst_offset;
  }
  return $time;
}

// New function, use only when timezone problem is gone
function gmt_to_local_time($gmt_time)
{
	$timezone_offset = get_timezone_offset($gmt_time);
  $local_time = $gmt_time+($timezone_offset*3600);

  return $local_time;
}

/**
* Give it a full name and it tries to determine the First, Middle and Lastname
*
* @param	string $full_name A full name
* @access public
* @return array array with keys first, middle and last
*/

function split_name($full_name)
{
  $name_arr = explode(' ', $full_name);

  $name['first'] = $full_name;
  $name['middle'] = '';
  $name['last'] = $full_name;
  $count = count($name_arr);
  $last_index = $count-1;
  for($i=0;$i<$count;$i++)
  {
    switch ($i)
    {
      case 0:
	$name['first'] = $name_arr[$i];
	break;

      case $last_index:
	$name['last'] = $name_arr[$i];
	break;

      default:
	$name['middle'] .= $name_arr[$i].' ';
	break;
    }
  }
  $name['middle'] = trim($name['middle']);
  return $name;
}

/**
* Prints an iframe containing a dialog to modify an Access Control List
*
* @param	int $acl_id The acl_id to modify
* @access public
* @return void
*/
function print_acl($acl_id)
{
  global $GO_CONFIG;
  echo '<iframe width="550"; height="300" border="0" frameborder="0" '.
				'scrolling="no" src="'.$GO_CONFIG->control_url.'acl.php?acl_id='.
				$acl_id.'"></iframe>';
}

/**
* Get the current server time in microseconds
*
* @access public
* @return int
*/
function getmicrotime(){
  list($usec, $sec) = explode(" ",microtime());
  return ((float)$usec + (float)$sec);
}

/**
* Returns an array with browser information
*
* @access public
* @return array Array contains keys name, version and subversion
*/
function detect_browser()
{
  if(eregi('msie ([0-9].[0-9]{1,2})',$_SERVER['HTTP_USER_AGENT'],$log_version))
  {
    $browser['version']=$log_version[1];
    $browser['name']='MSIE';
  }elseif(eregi('opera/([0-9].[0-9]{1,2})',$_SERVER['HTTP_USER_AGENT'],$log_version))
  {
    $browser['version']=$log_version[1];
    $browser['name']='OPERA';
  }elseif(eregi('mozilla/([0-9].[0-9]{1,2}).*gecko/([0-9]+)',$_SERVER['HTTP_USER_AGENT'],$log_version))
  {
    $browser['version']=$log_version[1];
    $browser['name']='MOZILLA';
    $browser['subversion']=$log_version[2];
  }elseif(eregi('netscape/([0-9].[0-9]{1,2})',$_SERVER['HTTP_USER_AGENT'],$log_version))
  {
    $browser['version']=$log_version[1];
    $browser['name']='NETSCAPE';
  }else
  {
    $browser['version']=0;
    $browser['name']='OTHER';
  }
  return $browser;
}

/**
* Check if an email adress is in a valid format
*
* @param	string $email E-mail address
* @access public
* @return bool
*/
function validate_email($email)
{
  return eregi("^([a-z0-9]+)([._-]([a-z0-9]+))*[@]([a-z0-9]+)([._-]([a-z0-9]+))*[.]([a-z0-9]){2}([a-z0-9])?([a-z0-9])?$",$email);
}

/**
* Checks for empty string and returns stripe when empty
*
* @param	string $input Any string
* @access public
* @return string
*/
function empty_to_stripe($input)
{
  if ($input == "")
  {
    return "-";
  }else
  {
    return $input;
  }
}

/**
* Build mailto link based on user's preference
* 
* creates a mail to link based on the users settings to use 
* his own mail client or the Group-Office mail client
*
* @param	string $email The reciever's address
* @param	string $name The reciever's name
* @param	string $class The CSS class of the link
* @param	bool $full_link Return a full link or just the href attribute
* @param	int $contact_id The contact id of the reciever
* @access public
* @return string URL to open e-mail client
*/
function mail_to($email, $name="", $class="normal", $full_link=true, $contact_id=0, $company_id=0)
{
  if (validate_email($email))
  {
    global $GO_CONFIG, $GO_SECURITY, $GO_MODULES, $strEmailTo;

    $module = $GO_MODULES->get_module('email');

    if ($name == "")
      $name = $email;

    if ($_SESSION['GO_SESSION']['mail_client'] == 1 && $module = $GO_MODULES->get_module('email'))
    {
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']))
      {
	if ($full_link == true)
	{
	  return "<a class=\"".$class."\" title=\"".$strEmailTo.$email."\" href='javascript:popup(\"".$module['url']."send.php?mail_to=".$email."&contact_id=".$contact_id."&company_id=".$company_id."\",\"".$GO_CONFIG->composer_width."\",\"".$GO_CONFIG->composer_height."\")'>".$name."</a>";
	}else
	{
	  return 'javascript:popup("'.$module['url'].'send.php?mail_to='.$email.'&contact_id='.$contact_id.'&company_id='.$company_id.'","'.$GO_CONFIG->composer_width.'","'.$GO_CONFIG->composer_height.'")';
	}
      }
    }

    if ($full_link == true)
    {
      return "<a class=\"".$class."\" href=\"mailto:".$email."\" title=\"".$strEmailTo.$email."\">".$name."</a>";
    }else
    {
      return 'mailto:'.$email;
    }
  }
  else
  {
    return '';
  }
}

/**
* Return an URL to a user profile page
* 
* @param	int $user_id The user_id
* @param	string $linktext The text to display
* @param	string $class The CSS class of the link
* @param	string $return_to The URL to return to when Back is clicked
* @access public
* @return string URL to a user profile page
*/
function show_profile($user_id, $linktext='', $class='normal', $return_to='')
{
  global $strShowProfile,  $strProtectedUser,  $GO_CONFIG, $GO_USERS;

  if ($return_to == '')
  {
    $return_to = $_SERVER['REQUEST_URI'];
  }

  $GO_SECURITY = new GO_SECURITY();
  $GO_MODULES = new GO_MODULES();

  if ($linktext == '')
  {
    $profile = $GO_USERS->get_user($user_id);
    $middle_name = $profile['middle_name'] == '' ? '' : $profile['middle_name'].' ';
    $linktext = $profile['last_name'].' '.$middle_name.$profile['first_name'];
  }
  $ab_module = $GO_MODULES->get_module('addressbook');
  if ($ab_module)
  {
    require_once($ab_module['class_path'].'addressbook.class.inc');
    $ab = new addressbook();
  }
  if ($ab_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) ||
	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])) &&
      $contact = $ab->user_is_contact($GO_SECURITY->user_id, $user_id))
  {

    if($contact['color'] != '')
    {
      $style = ' style="color: #'.$contact['color'].';"';
    }else
    {
      $style = '';
    }
    $link = "<a ".$style." href=\"".$ab_module['url']."contact.php?contact_id=".$contact['id']."&return_to=".urlencode($return_to)."\" class=\"".$class."\" title=\"".$strShowProfile."\">";
    $link .= $linktext."</a>\n";
  }else
  {
    if ($GO_SECURITY->user_is_visible($user_id))
    {
      $link = '<a href="'.$GO_CONFIG->control_url.'user.php?id='.$user_id.'&return_to='.urlencode($return_to).'" class="'.$class.'" title="'.$strShowProfile.'">';
      $link .= $linktext."</a>\n";
    }else
    {
      $link = $linktext;
    }

  }
  return $link;
}

/**
* Return an URL to a user profile page
* 
* @param	int $contact_id The contact_id
* @param	string $linktext The text to display
* @param	string $return_to The URL to return to when Back is clicked
* @access public
* @return string URL to Contact profile page
*/
function show_contact($contact_id, $linktext='', $return_to='')
{
  global $strShowProfile, $strProtectedUser, $GO_CONFIG, $GO_SECURITY, $GO_MODULES;

  if ($return_to == '')
  {
    $return_to = $_SERVER['REQUEST_URI'];
  }

  $ab_module = $GO_MODULES->get_module('addressbook');

  if ($ab_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) ||
	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
  {
    require_once($ab_module['class_path'].'addressbook.class.inc');
    $ab = new addressbook();

    $contact = $ab->get_contact($contact_id);

    if($ab->f('color') != '')
    {
      $style = ' style="color: #'.$ab->f('color').';"';
    }else
    {
      $style = '';
    }

    if ($linktext == '')
    {
      $middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
      $linktext = $contact['last_name'].' '.$middle_name.$contact['first_name'];
    }
    $contact_id=$contact['id'];

    $link = "<a ".$style." href=\"".$ab_module['url'].
      "contact.php?contact_id=".$contact_id."&return_to=".
      urlencode($return_to)."\" class=\"normal\" title=\"".$strShowProfile."\">";
    $link .= $linktext."</a>\n";
  }else
  {
    $link = '';
  }
  return $link;
}

/**
* Return an URL to a user or contact profile page
* 
* @param	string $email The e-mail address to search
* @param	string $linktext The text to display
* @param	string $return_to The URL to return to when Back is clicked
* @access public
* @return string URL to a user profile page
*/
function show_profile_by_email($email, $linktext = '', $return_to='')
{
  global $GO_USERS;
  if ($return_to == '')
  {
    $return_to = $_SERVER['REQUEST_URI'];
  }
  if ($linktext == '') $linktext = $email;

  global $GO_CONFIG, $GO_SECURITY, $strShowProfile, $strAddContact, $GO_MODULES, $GO_USERS;

  $ab_module = $GO_MODULES->get_module('addressbook');

  $has_contacts_module = ($ab_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])));

  if ($has_contacts_module)
  {
    require_once($ab_module['class_path'].'addressbook.class.inc');
    $ab = new addressbook();
  }

  if ($has_contacts_module && $contact = $ab->get_contact_profile_by_email($email, $GO_SECURITY->user_id))
  {
    if($contact['color'] != '')
    {
      $style = ' style="color: #'.$contact['color'].';"';
    }else
    {
      $style = '';
    }
    $link = "<a ".$style." href=\"".$ab_module['url']."contact.php?contact_id=".$contact['id']."&return_to=".urlencode($return_to)."\" class=\"normal\" title=\"".$strShowProfile."\">";
    $m = $contact['middle_name'];
    $m = isset($m) ? ' '.$m.' ' : ' ';
    $link .= $contact['last_name'].$m.$contact['first_name']."</a>\n";
  }else
  {
    $users_id = $GO_USERS->get_user_id_by_email($email);
    if ($users_id && $GO_SECURITY->user_is_visible($users_id))
    {
      $link = '<a href="'.$GO_CONFIG->control_url.'user.php?id='.$users_id.'&return_to='.urlencode($return_to).'" title="'.$strShowProfile.'">';
      $u = $GO_USERS->get_user($users_id);
      $m = $u['middle_name'];
      $m = isset($m) ? ' '.$m.' ' : ' ';
      $link .= $u['last_name'].$m.$u['first_name']."</a>\n";
    }elseif($has_contacts_module)
    {
      $name = split_name($linktext);
      $link = "<a href=\"".$ab_module['url']."contact.php?email=".$email."&first_name=".urlencode($name['first'])."&middle_name=".urlencode($name['middle'])."&last_name=".urlencode($name['last'])."&return_to=".urlencode($return_to)."\" class=\"normal\" title=\"".$strShowProfile."\">";
      $link .= $linktext."</a>\n";
    }else
    {
      $link = $linktext;
    }
  }

  return $link;
}

/**
* Check precence of invalid characters
* 
* @param	string $input The string to check
* @param	array $invalid_chars An array of invalid charcters
* @access public
* @return bool
*/
function validate_input($input, $invalid_chars = "")
{
  if ($invalid_chars == "")
  {
    $invalid_chars[] = "\"";
    $invalid_chars[] = "/";
    $invalid_chars[] = "?";
    $invalid_chars[] = "&";
  }

  for ($i=0; $i<count($invalid_chars);$i++)
  {
    if (strchr($input,$invalid_chars[$i]))
    {
      return false;
    }
  }
  return true;
}

/**
* Format a size to a human readable format.
* 
* @param	int $size The size in bytes
* @param	int $decimals Number of decimals to display
* @access public
* @return string
*/

function format_size($size, $decimals = 1)
{
  switch ($size)
  {
    case ($size > 1073741824):
      $size = number_format($size / 1073741824, $decimals, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']);
      $size .= " GB";
      break;

    case ($size > 1048576):
      $size = number_format($size / 1048576, $decimals, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']);
      $size .= " MB";
      break;

    case ($size > 1024):
      $size = number_format($size / 1024, $decimals, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']);
      $size .= " KB";
      break;

    default:
      number_format($size, $decimals, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']);
      $size .= " bytes";
      break;
  }
  return $size;
}

$utf8_skip_data = array(
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
);

function utf8_strlen($string)
{
  global $utf8_skip_data;
  $len = 0;
  $len2 = 0;
  $len3 = strlen($string);
  while ($len < $len3)
  {
    $len += $utf8_skip_data[ord($string[$len])];
    $len2 ++;
  }
  return $len2;
}

/**
* Chop long strings with 3 dots
*
* Chops of the string after a given length and puts three dots behind it
* function editted by Tyler Gee to make it chop at whole words
* 
* @param	string $string The string to chop
* @param	int $maxlength The maximum number of characters in the string
* @access public
* @return string
*/

function cut_string($string, $maxlength)
{
  global $utf8_skip_data;

  if (true) // use utf-8 encoding
  {
    if (utf8_strlen($string) <= $maxlength)
      return $string;
    $len = 0;
    $len2 = 0;
    while ($len2 < $maxlength-3)
    {
      $len += $utf8_skip_data[ord($string[$len])];
      $len2 ++;
    }
    $temp = substr($string,0,$len);
    if ($pos = strrpos($temp, ' '))
    {
      return substr($temp,0,$pos).'...';
    }else
    {
      return $temp.'...';
    }
  }
  else
  {
    if (strlen($string) > $maxlength)
    {
      $temp = substr($string,0,$maxlength-3);
      if ($pos = strrpos($temp, ' '))
      {
        return substr($temp,0,$pos).'...';
      }else
      {
        return $temp = substr($string,0,$maxlength-3).'...';
      }
 
    }else
    {
      return $string;
    }
  }
}

/**
* Convert an enriched formated string to HTML format
*
* @param	string $enriched Enriched formatted string
* @access public
* @return string HTML formated string
*/
function enriched_to_html($enriched)
{
  global $GO_CONFIG, $GO_MODULES;

  // We add space at the beginning and end of the string as it will
  // make some regular expression checks later much easier (so we
  // don't have to worry about start/end of line characters)
  $enriched = ' ' . $enriched . ' ';

  // Get color parameters into a more useable format.
  $enriched = preg_replace('/<color><param>([\da-fA-F]+),([\da-fA-F]+),([\da-fA-F]+)<\/param>/Uis', '<color r=\1 g=\2 b=\3>', $enriched);
  $enriched = preg_replace('/<color><param>(red|blue|green|yellow|cyan|magenta|black|white)<\/param>/Uis', '<color n=\1>', $enriched);

  // Get font family parameters into a more useable format.
  $enriched = preg_replace('/<fontfamily><param>(\w+)<\/param>/Uis', '<fontfamily f=\1>', $enriched);

  // Single line breaks become spaces, double line breaks are a
  // real break. This needs to do <nofill> tracking to be
  // compliant but we don't want to deal with state at this
  // time, so we fake it some day we should rewrite this to
  // handle <nofill> correctly.
  $enriched = preg_replace('/([^\n])\r\n([^\r])/', '\1 \2', $enriched);
  $enriched = preg_replace('/(\r\n)\r\n/', '\1', $enriched);

  // We try to protect against bad stuff here.
  $enriched = @htmlspecialchars($enriched, ENT_QUOTES, $charset);

  // Now convert the known tags to html. Try to remove any tag
  // parameters to stop people from trying to pull a fast one
  $enriched = preg_replace('/(?<!&lt;)&lt;bold.*&gt;(.*)&lt;\/bold&gt;/Uis', '<span style="font-weight: bold">\1</span>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;italic.*&gt;(.*)&lt;\/italic&gt;/Uis', '<span style="font-style: italic">\1</span>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;underline.*&gt;(.*)&lt;\/underline&gt;/Uis', '<span style="text-decoration: underline">\1</span>', $enriched);
  $enriched = preg_replace_callback('/(?<!&lt;)&lt;color r=([\da-fA-F]+) g=([\da-fA-F]+) b=([\da-fA-F]+)&gt;(.*)&lt;\/color&gt;/Uis', 'colorize', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;color n=(red|blue|green|yellow|cyan|magenta|black|white)&gt;(.*)&lt;\/color&gt;/Uis', '<span style="color: \1">\2</span>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;fontfamily&gt;(.*)&lt;\/fontfamily&gt;/Uis', '\1', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;fontfamily f=(\w+)&gt;(.*)&lt;\/fontfamily&gt;/Uis', '<span style="font-family: \1">\2</span>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;smaller.*&gt;/Uis', '<span style="font-size: smaller">', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;\/smaller&gt;/Uis', '</span>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;bigger.*&gt;/Uis', '<span style="font-size: larger">', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;\/bigger&gt;/Uis', '</span>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;fixed.*&gt;(.*)&lt;\/fixed&gt;/Uis', '<font face="fixed">\1</font>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;center.*&gt;(.*)&lt;\/center&gt;/Uis', '<div align="center">\1</div>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;flushleft.*&gt;(.*)&lt;\/flushleft&gt;/Uis', '<div align="left">\1</div>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;flushright.*&gt;(.*)&lt;\/flushright&gt;/Uis', '<div align="right">\1</div>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;flushboth.*&gt;(.*)&lt;\/flushboth&gt;/Uis', '<div align="justify">\1</div>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;paraindent.*&gt;(.*)&lt;\/paraindent&gt;/Uis', '<blockquote>\1</blockquote>', $enriched);
  $enriched = preg_replace('/(?<!&lt;)&lt;excerpt.*&gt;(.*)&lt;\/excerpt&gt;/Uis', '<blockquote>\1</blockquote>', $enriched);

  // Now we remove the leading/trailing space we added at the
  // start.
  $enriched = preg_replace('/^ (.*) $/s', '\1', $enriched);

  $module = $GO_MODULES->get_module('email');

  $enriched = preg_replace("/(?:^|\b)(((http(s?):\/\/)|(www\.-))([\w\.-]+)([,:;%#&\/?=\w+\.\-@]+))(?:\b|$)/is", "<a href=\"http$4://$5$6$7\" target=\"_blank\" class=\"blue\">$1</a>", $enriched);
  if ($_SESSION['GO_SESSION']['mail_client'] == 1)
  {
    $enriched = preg_replace("/(\A|\s)([\w\.\-]+)(@)([\w\.-]+)([A-Za-z]{2,3})\b/i","\\1<a href=\"javascript:popup('".$module['url']."send.php?mail_to=\\2\\3\\4\\5','650','500')\" class=\"blue\">\\2\\3\\4\\5</a>",$enriched);
  }else
  {
    $enriched = preg_replace("/(\A|\s)([\w\.\-]+)(@)([\w\.-]+)([A-Za-z]{2,3})\b/i","\\1<a href=\"mailto:\\2\\3\\4\\5\" class=\"blue\">\\2\\3\\4\\5</a>",$enriched);
  }

  $enriched = nl2br($enriched);
  $enriched = str_replace("\r", "", $enriched);
  $enriched = str_replace("\n", "", $enriched);

  return $enriched;

}

/**
* Used by enriched_to_html() to create colors
*
* @param	array $colors
* @access private
* @return string HTML formatted string
*/
function colorize($colors)
{
  for ($i = 1; $i < 4; $i++) {
    $colors[$i] = sprintf('%02X', round(hexdec($colors[$i]) / 255));
  }
  return '<span style="color: #' . $colors[1] . $colors[2] . $colors[3] . '">' . $colors[4] . '</span>';
}


/**
* Convert plain text to HTML
*
* @param	string $text Plain text string
* @access public
* @return string HTML formatted string
*/
function text_to_html($text)
{
  global $GO_CONFIG, $GO_MODULES;

  $module = $GO_MODULES->get_module('email');

  $text = htmlspecialchars($text);
  $text = preg_replace("/(?:^|\b)(((http(s?):\/\/)|(www\.-))([\w\.-]+)([,:;%#&\/?=\w+\.\-@]+))(?:\b|$)/is", "<a href=\"http$4://$5$6$7\" target=\"_blank\" class=\"blue\">$1</a>", $text);
  if ($_SESSION['GO_SESSION']['mail_client'] == 1)
  {
    $text = preg_replace("/(\A|\s)([\w\.\-]+)(@)([\w\.-]+)([A-Za-z]{2,3})\b/i","\\1<a href=\"javascript:popup('".$module['url']."send.php?mail_to=\\2\\3\\4\\5','650','500')\" class=\"blue\">\\2\\3\\4\\5</a>",$text);
  }else
  {
    $text = preg_replace("/(\A|\s)([\w\.\-]+)(@)([\w\.-]+)([A-Za-z]{2,3})\b/i","\\1<a href=\"mailto:\\2\\3\\4\\5\" class=\"blue\">\\2\\3\\4\\5</a>",$text);
  }

  $text = nl2br($text);
  $text = str_replace("\r", "", $text);
  $text = str_replace("\n", "", $text);

  return ($text);

}

/**
* Convert Dangerous HTML to safe HTML for display inside of Group-Office
*
* This also removes everything outside the body and replaces mailto links
*
* @param	string $text Plain text string
* @access public
* @return string HTML formatted string
*/
function convert_html($html, $remove = true)
{
  global $GO_CONFIG, $GO_MODULES;

  //$html = str_replace("\r", "", $html);
  //$html = str_replace("\n", "", $html);

  $module = $GO_MODULES->get_module('email');

  if ($remove)
  {
    $to_removed_array = array (
	"'<html>'si",
	"'</html>'si",
	"'<body[^>]*>'si",
	"'</body>'si",
	"'<head[^>]*>.*?</head>'si",
	"'<style[^>]*>.*?</style>'si",
	"'<script[^>]*>.*?</script>'si",
	"'<object[^>]*>.*?</object>'si",
	"'<embed[^>]*>.*?</embed>'si",
	"'<applet[^>]*>.*?</applet>'si",
	"'<form[^>]*>'si",
	"'<input[^>]*>'si",
	"'<select[^>]*>.*?</select>'si",
	"'<textarea[^>]*>.*?</textarea>'si",
	"'</form>'si"
	);
    $html= preg_replace($to_removed_array, '', $html);
    //$html = preg_replace("|href=\"(.*)script:|i", 'href="removed_script:', $html);
    //$html = preg_replace("|<([^>]*)java|i", '<removed_java_tag', $html);
    //$html = preg_replace("|<([^>]*)&{.*}([^>]*)>|i", "<&{;}\\3>", $html);
    //$html = preg_replace("|<([^>]*)mocha:([^>]*)>|i", "<removed_mocha:\\2>",$html);
  }

  if ($_SESSION['GO_SESSION']['mail_client'] == 1)
  {
    $html = preg_replace("/(href=\"mailto:)([\w\.\-]+)(@)([\w\.\-\"]+)\b/i","href=\"javascript:popup('".$module['url']."send.php?mail_to=$2$3$4','650','500')\" class=\"blue\"",$html);
  }

  $html = preg_replace("/\b((href=\"http(s?):\/\/))([\w\.\?\=\&]+)([\/\w+\.\?\=\&\:\~]+)\b/i"," target=\"_blank\" class=\"blue\" href=\"http$3://$4$5$6", $html);
  $html = preg_replace("/\b((href='http(s?):\/\/))([\w\.\?\=\&]+)([\/\w+\.\?\=\&\:\~]+)\b/i"," target=\"_blank\" class=\"blue\" href='http$3://$4$5$6", $html);

  return ($html);
}

/**
* Parse a comma seperated string of e-mail addresses into an array of e-mail addresses
*
* @param	string $addr The comma seperated string
* @param	string $charset The charset used
* @access public
* @return array Array of e-mail addresses
*/
function cut_address($addr, $charset)
{
  $addr = smartstrip($addr);

  $addresses = array();
  $token = '';
  $quote_esc = false;
  for ($i = 0; $i < strlen($addr); $i++) {
    $c = substr($addr, $i, 1);

    if($c == '"') {
      $quote_esc = !$quote_esc;
    }

    if($c == ',' || $c == ';') {
      if(!$quote_esc) {
	$token = trim($token);
	if($token != '') {
	  $addresses[] = $token;
	}
	$token = '';
	continue;
      }
    }

    $token .= $c;
  }
  if(!$quote_esc) {
    $token = trim($token);
    if($token != '') {
      $addresses[] = $token;
    }
  }
  return ($addresses);
}

/**
* Get all e-mail addresses inside a string
*
* @param	string $addr The comma seperated string
* @access public
* @return array Array of e-mail addresses
*/
function get_addresses_from_string($address_string)
{
  $in_address = false;
  $address = '';
  $addresses = array();
  $length = strlen($address_string);
  for($i=0;$i<$length;$i++)
  {
    $char = $address_string[$i];
    if ($char == '>')
    {
      if ($in_address)
      {
	$in_address = false;
	$addresses[] = trim($address);
	$address = '';
      }
    }elseif($char == '<')
    {
      $in_address=true;
    }elseif($in_address)
    {
      $address .= $char;
    }
  }
  return $addresses;
}

/**
* Get CRLF based on OS (Deprecated
*
* @param	string $smtp
* @access public
* @return string CRLF
*/
function get_crlf($smtp = "")
{
  $crlf = stristr(PHP_OS, 'Windows') ? "\r\n" : "\n";

  if ($smtp != "")
    $crlf = $smtp ? "\r\n" : $crlf;

  //return ($crlf);
  return "\r\n";
}

/**
* Deprecated use smart_stripslashes()
*
* @param	string $string
* @access public
* @return string Stripped of slashes
*/
function smartstrip($string)
{
  return smart_stripslashes($string);
}

/**
* Unescapes a slashed string if magic_quotes_gpc is on
*
* @param	string $string
* @access public
* @return string Stripped of slashes
*/
function smart_stripslashes($string)
{
	if(get_magic_quotes_gpc())
  {
    $string = stripslashes($string);
  }
  return $string;
}

/**
* Escapes a string with slashes if magic_quotes_gpc is off
*
* @param	string $string
* @access public
* @return string Stripped of slashes
*/
function smart_addslashes($string)
{
  if(!get_magic_quotes_gpc()) {
    $string = addslashes($string);
  }
  return $string;
}

/**
* Quotes a string with >
*
* @param	string $text
* @access public
* @return string A string quoted with >
*/
function quote($text)
{
  $text = "> " . ereg_replace("\n", "\n> ", trim($text));
  return($text);
}

/**
* Sends an e-mail using the global mail settings from Group-Office.php
*
* @param	string $email_to The e-mail reciepent
* @param	string $email_from The e-mail sender
* @param	string $subject The e-mail subject
* @param	string $body The e-mail body
* @param	string $priority The priotity of the e-mail message
* @param	string $body_ctype The the mime type of the body
				 (text/PLAIN or text/HTML)
* @access public
* @return bool Returns true if the message was successfully sent
*/
function sendmail($email_to, $email_from, $name_from, $subject, $body, 
									$priority = '3', $body_ctype = 'text/PLAIN')
{
  global $GO_CONFIG, $charset, $php_mailer_lang;

  require_once($GO_CONFIG->class_path."phpmailer/class.phpmailer.php");
  require_once($GO_CONFIG->class_path."phpmailer/class.smtp.php");
  require_once($GO_CONFIG->class_path."html2text.class.inc");

  $mail = new PHPMailer();
  $mail->PluginDir = $GO_CONFIG->class_path.'phpmailer/';
  $mail->SetLanguage($php_mailer_lang, 
												$GO_CONFIG->class_path.'phpmailer/language/');

  switch($GO_CONFIG->mailer)
  {
    case 'smtp':
      $mail->Host = $GO_CONFIG->smtp_server;
      $mail->Port = $GO_CONFIG->smtp_port;
      $mail->IsSMTP();
      break;
    case 'qmail':
      $mail->IsQmail();
      break;
    case 'sendmail':
      $mail->IsSendmail();
      break;
    case 'mail':
      $mail->IsMail();
      break;
  }
  $mail->Priority = $priority;
  $mail->Sender     = $email_from;    
  $mail->From     = $email_from;
  $mail->FromName = $name_from;
  $mail->AddReplyTo($email_from, $name_from);
  $mail->WordWrap = 50;
  $html_message = strtolower($body_ctype) == 'text/html' ? true : false;
  $mail->IsHTML($html_message);
  $mail->Subject = smartstrip(trim($subject));

  $mail_to_array = cut_address(trim($email_to), $charset);
  while ($to_address = array_shift($mail_to_array))
  {
    $mail->AddAddress($to_address);
  }

  if ($html_message)
  {
    $mail->Body = smartstrip($body);
	$h2t =& new html2text($body);
    	$mail->AltBody  = $h2t->get_text();

  }else
  {
    $mail->Body = smartstrip($body);
  }

  return $mail->Send();
}

/**
* returns a link poping up a open file dialog
*
* A handler file needs to be created to do something with the file to open
*
* @param	string $GO_HANDLER The handler file to submit to
* @param	string $GO_FILTER Filter certain file types to display
* @param	string $link_text The text to display in the link
* @param	string $class CSS class file to use in the link
* @param	bool $full_link Return a full link or retrun only an URL.
* @access public
* @return string URL or link
*/
function show_open($GO_HANDLER, $GO_FILTER, $link_text="", $class="normal", $fulllink=true)
{
  global $GO_CONFIG, $GO_MODULES;

  $module = $GO_MODULES->get_module('filesystem');

  if ($fulllink)
  {
    $link = "<a class=\"".$class."\" href=\"javascript:popup('".$module['url']."index.php?GO_HANDLER=".urlencode($GO_HANDLER)."&GO_FILTER=".$GO_FILTER."&mode=popup','600','400')\">".$link_text."</a>";
  }else
  {
    $link = "javascript:popup('".$module['url']."index.php?GO_HANDLER=".urlencode($GO_HANDLER)."&GO_FILTER=".$GO_FILTER."&mode=popup','600','400')";
  }
  return $link;
}

/**
* returns a link poping up a save file dialog
*
* A temporarely file to save must be passed
*
* @param	string $tmp_file The temporary file location
* @param	string $link_text The text to display in the link
* @param	string $class CSS class file to use in the link
* @param	bool $full_link Return a full link or retrun only an URL.
* @access public
* @return string URL or link
*/

function show_save($tmpfile, $link_text="", $class="normal", $fulllink=true)
{
  global $GO_CONFIG, $GO_MODULES;

  $module = $GO_MODULES->get_module('files');

  if ($fulllink)
  {
    $link = "<a class=\"".$class."\" href=\"javascript:popup('".$module['url']."save.php?SET_HANDLER=".urlencode($tmpfile)."','600','400')\">".$link_text."</a>";
  }else
  {
    $link = "javascript:popup('".$module['url']."save.php?GO_HANDLER=".urlencode($tmpfile)."','400','450')";
  }
  return $link;
}

/**
* Return a filename without extension
*
* @param	string $filename The complete filename
* @access public
* @return string A filename without the extension
*/
function strip_extension($filename)
{
  $pos = strrpos($filename,'.');
  if ($pos)
  {
    $filename = substr($filename,0,$pos);
  }
  return $filename;
}

/**
* Returns the extension of a filename
*
* @param	string $filename The complete filename
* @access public
* @return string  The extension of a filename
*/
function get_extension($filename)
{
  $extension = '';
  $pos = strrpos($filename,'.');
  if ($pos)
  {
    $extension= substr($filename,$pos+1,strlen($filename));
  }
  return $extension;
}

/**
* Returns the mime type of a file using Group-Office internal filetype system
*
* @param	string $path The full path to the file
* @access public
* @return string  The mime type of the file
*/
function filemime($path)
{
  global $GO_CONFIG;
  require_once($GO_CONFIG->class_path.'filetypes.class.inc');
  $filetypes = new filetypes();

  if(!$type = $filetypes->get_type($filetypes->get_extension(basename($path))))
  {
    $type = $filetypes->add_type($filetypes->get_extension(basename($path)));
  }

  if ($type)
  {
    return $type['mime'];
  }else
  {
    return false;
  }
}

/**
* Returns the file type n a friendly way. eg. text/HTML will be HTML document
*
* @param	string $path The full path to the file
* @access public
* @return string  The file type of the file
*/
function filemimefriendly($path)
{
  global $GO_CONFIG;
  require_once($GO_CONFIG->class_path.'filetypes.class.inc');
  $filetypes = new filetypes();

  if(!$type = $filetypes->get_type(get_extension(basename($path))))
  {
    $type = $filetypes->add_type(get_extension(basename($path)));
  }

  if ($type)
  {
    return $type['friendly'];
  }else
  {
    return false;
  }
}

/**
function path_to_url($path)
{
  global $GO_CONFIG;
  if ($path = str_replace($GO_CONFIG->root_path, $GO_CONFIG->host, $path))
  {
    return $path;
  }
  return false;
}*/


/**
* Finds a system program
*
* @param	string $program	The name of the program
* @access public
* @return string The full path to the program
*/
function find_program ($program) {
  $path = array('/bin', '/sbin', '/usr/bin', '/usr/sbin', '/usr/local/bin', '/usr/local/sbin');
  while ($this_path = current($path)) {
    if (is_executable("$this_path/$program")) {
      return "$this_path/$program";
    }
    next($path);
  }
  return;
}


// Execute a system program. return a trim()'d result.
// does very crude pipe checking.  you need ' | ' for it to work
// ie $program = execute_program('netstat', '-anp | grep LIST');
// NOT $program = execute_program('netstat', '-anp|grep LIST');

/**
* Execute a system program.
*
* does very crude pipe checking.  you need ' | ' for it to work
* ie $program = execute_program('netstat', '-anp | grep LIST');
* NOT $program = execute_program('netstat', '-anp|grep LIST');
*
* @param	string $program	The name of the program
* @access public
* @return string trim()'d result
*/
function execute_program ($program, $args = '') {
  $buffer = '';
  $program = find_program($program);

  if (!$program) { return; }

  // see if we've gotten a |, if we have we need to do patch checking on the cmd
  if ($args) {
    $args_list = split(' ', $args);
    for ($i = 0; $i < count($args_list); $i++) {
      if ($args_list[$i] == '|') {
	$cmd = $args_list[$i + 1];
	$new_cmd = find_program($cmd);
	$args = ereg_replace("\| $cmd", "| $new_cmd", $args);
      }
    }
  }

  // we've finally got a good cmd line.. execute it
  if ($fp = popen("$program $args", 'r')) {
    while (!feof($fp)) {
      $buffer .= fgets($fp, 4096);
    }
    return trim($buffer);
  }
}

function array_csort($marray, $column) {
  foreach ($marray as $row) {
    $sortarr[] = $row[$column];
  }
  array_multisort($sortarr, $marray);
  return $marray;
}

/**
* Returns an array with RGB color (e.g. R128 G126 B124) from a hex html color code (e.g. #F7F7F7)
*
* @param  string $html_color HTML color code: e.g. #FFFFFF
* @access public
* @return array RGB color code: e.g. R:102 G:102 B:102
*/
function hex2dec($html_color)
{
  $R = hexdec(substr($html_color,1,2));
  $G = hexdec(substr($html_color,3,2));
  $B = hexdec(substr($html_color,5,2));
  $color = array();
  $color['R']=$R;
  $color['G']=$G;
  $color['B']=$B;
  return $color;
}
?>
