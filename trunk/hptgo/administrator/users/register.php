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

require("../../Group-Office.php");
$GO_SECURITY->authenticate(true);
require($GO_LANGUAGE->get_base_language_file('users'));
require($GO_LANGUAGE->get_base_language_file('common'));

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();

$page_title = $registration_title;
require($GO_THEME->theme_path."header.inc");

require($GO_CONFIG->class_path."/validate.class.inc");
$val = new validate;

$module_acl = isset($_POST['module_acl']) ? $_POST['module_acl'] : array();

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
  $first_name = smart_addslashes(trim($_POST['first_name']));
  $middle_name = smart_addslashes(trim($_POST['middle_name']));
  $last_name = smart_addslashes(trim($_POST['last_name']));
  $initials = smart_addslashes($_POST["initials"]);
  $title = smart_addslashes($_POST["title"]);
  $birthday = smart_addslashes($_POST["birthday"]);
  $email = smart_addslashes($_POST["email"]);
  $work_phone = smart_addslashes($_POST["work_phone"]);
  $home_phone = smart_addslashes($_POST["home_phone"]);
  $fax = smart_addslashes($_POST["fax"]);
  $cellular = smart_addslashes($_POST["cellular"]);
  $country = smart_addslashes($_POST["country"]);
  $state = smart_addslashes($_POST["state"]);
  $city = smart_addslashes($_POST["city"]);
  $zip = smart_addslashes($_POST["zip"]);
  $address = smart_addslashes($_POST["address"]);
  $department = smart_addslashes($_POST["department"]);
  $function = smart_addslashes($_POST["function"]);
  $company = smart_addslashes($_POST["company"]);
  $work_country = smart_addslashes($_POST["work_country"]);
  $work_state = smart_addslashes($_POST["work_state"]);
  $work_city = smart_addslashes($_POST["work_city"]);
  $work_zip = smart_addslashes($_POST["work_zip"]);
  $work_address = smart_addslashes($_POST["work_address"]);
  $work_fax = smart_addslashes($_POST["work_fax"]);
  $homepage = smart_addslashes($_POST["homepage"]);

  $pass1 = smartstrip($_POST["pass1"]);
  $pass2 = smartstrip($_POST["pass2"]);
  $username = smart_addslashes($_POST['username']);

  $val->error_required = $error_required;
  $val->error_min_length = $error_min_length;
  $val->error_max_length = $error_max_length;
  $val->error_expression = $error_email;

  $val->name="first_name";
  $val->input=$first_name;
  $val->max_length=50;
  $val->required=true;
  $val->validate_input();

  $val->name="last_name";
  $val->input=$last_name;
  $val->max_length=50;
  $val->required=true;
  $val->validate_input();

  $val->name="username";
  $val->input=$username;
  $val->min_length=3;
  $val->max_length=20;
  $val->required=true;
  $val->validate_input();

  $val->name="pass1";
  $val->input=$pass1;
  $val->min_length=3;
  $val->max_length=20;
  $val->required=true;
  $val->validate_input();

  $val->name="pass2";
  $val->input=$pass2;
  $val->min_length=3;
  $val->max_length=20;
  $val->required=true;
  $val->validate_input();


  $val->name="email";
  $val->input=$_POST['email'];
  $val->max_length=75;

  if (!isset($_POST['create_email']))
  {
    $val->required=true;
  }

  $val->expression="^([a-z0-9]+)([._-]([a-z0-9]+))*[@]([a-z0-9]+)([._-]([a-z0-9]+))*[.]([a-z0-9]){2}([a-z0-9])?([a-z0-9])?$";
  $val->validate_input();

  $val->error_match = $error_match_pass;
  $val->name="pass1";
  $val->match1=$_POST['pass1'];
  $val->match2=$_POST['pass2'];
  $val->validate_input();

  if (!$val->validated)
  {
    $error ="<p class='Error'>".$errors_in_form."</p>";
    //check if username already exists
  }elseif($GO_USERS->get_profile_by_username($_POST['username']))
  {
    $error = "<p class='Error'>".$error_username_exists."</p>";
    //check if email is already registered
  }elseif($GO_USERS->email_exists($_POST['email']))
  {
    $error =  "<p class='Error'>".$error_email_exists."</p>";
  }else
  {
    $birthday = date_to_db_date($_POST['birthday']);

    $email = ($_POST['email'] == '') ? $_POST['username'].'@'.$GO_CONFIG->inmail_host : $_POST['email'];

    //register the new user. function returns new user_id or -1 on failure.
    if ($new_user_id = $GO_USERS->add_user($username,$pass1, $first_name,
	  $middle_name, $last_name, $initials, $title, $_POST['sex'],
	  $birthday, $email, $work_phone,
	  $home_phone, $fax, $cellular, $country, $state,
	  $city, $zip, $address, $company, $work_country,
	  $work_state, $work_city, $work_zip, $work_address,
	  $work_fax, $homepage, $department, $function,
	  $_POST['language'], $_POST['theme'], '',isset($_POST['visible'])
	  ))
    {
      if (isset($_POST['create_email']))
      {
	require_once($GO_CONFIG->class_path."email.class.inc");
	$email_client = new email();

	$middle_name = $middle_name ==  '' ? '' : $middle_name.' ';
	$name = $last_name.' '.$middle_name.$first_name;

	require($GO_LANGUAGE->get_language_file('email'));
	if (!$account_id = $email_client->add_account($new_user_id,
	      $GO_CONFIG->inmail_type,$GO_CONFIG->local_email_host,
	      $GO_CONFIG->inmail_port, $GO_CONFIG->inmail_root,
	      $username, $pass1, $name,
	      $username."@".$GO_CONFIG->inmail_host, "",
	      $ml_sent_items, $ml_spam, $ml_trash))
	{
	  echo "<p class=\"Error\">".$registration_email_error."</p>";
	  echo "<p class=\"Error\">".$email_client->last_error."</p>";
	}
      }

      //send email to the user with password
      $registration_mail_body = str_replace("%sex%", $sir_madam[$_POST['sex']], $registration_mail_body);
      $registration_mail_body = str_replace("%last_name%", $_POST['last_name'], $registration_mail_body);
      $registration_mail_body = str_replace("%middle_name%", $middle_name, $registration_mail_body);
      $registration_mail_body = str_replace("%first_name%", $_POST['first_name'], $registration_mail_body);
      $registration_mail_body = str_replace("%username%",$_POST['username'], $registration_mail_body);
      $registration_mail_body = str_replace("%password%",$_POST['pass1'], $registration_mail_body);
      $registration_mail_body .= "\n\n".$GO_CONFIG->full_url;
      sendmail($email,  $GO_CONFIG->webmaster_email, $GO_CONFIG->title, $registration_mail_subject, $registration_mail_body);

      //used for professional version
      //$user_count = $GO_USERS->get_users();
      //sendmail('info@intermesh.nl',  $GO_CONFIG->webmaster_email, $GO_CONFIG->title, 'User count for '.$GO_CONFIG->full_url.': '.$user_count, '');

			if(isset($_POST['user_groups']))
			{
	      while($group_id = array_shift($_POST['user_groups']))
	      {
		$GO_GROUPS->add_user_to_group($new_user_id, $group_id);
	      }
	    }

      //set module permissions
      for ($i=0;$i<count($module_acl);$i++)
      {
	$GO_SECURITY->add_user_to_acl($new_user_id, $_POST['module_acl'][$i]);
      }

      //create Group-Office home directory
      $old_umask = umask(000);
      mkdir($GO_CONFIG->file_storage_path.$username, $GO_CONFIG->create_mode);
      umask($old_umask);

      //confirm registration to the user and exit the script so the form won't load
      echo $registration_success." <b>".$email."</b>";
      echo '<br /><br />';
      $button = new button($cmdContinue, "javascript:document.location='index.php';");
      echo '</td></tr></table>';
      require($GO_THEME->theme_path."footer.inc");
      exit;
    }else
    {
      $error = "<p class=\"Error\">".$registration_failure."</p>";
    }
  }
}
if (!$GO_USERS->max_users_reached())
{
  require("register_form.inc");
}else
{
  echo '<h1>'.$max_user_limit.'</h1>'.$max_users_text;
}
require($GO_THEME->theme_path."footer.inc");
?>
