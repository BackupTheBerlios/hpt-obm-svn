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

$GO_SECURITY->authenticate();
//if the user is authorising but it's logged in under another user log him out first.
if(isset($_REQUEST['requested_user_id']) && $_REQUEST['requested_user_id'] != $GO_SECURITY->user_id)
{
  SetCookie("GO_UN","",time()-3600,"/","",0);
  SetCookie("GO_PW","",time()-3600,"/","",0);
  unset($_SESSION);
  unset($_COOKIES);
  $GO_SECURITY->logout();
  $GO_SECURITY->authenticate();
}

$return_to = $GO_CONFIG->host.'configuration/';

require($GO_LANGUAGE->get_base_language_file('account'));

$page_title = $acTitle;

$tabtable = new tabtable('account', $acManager, '100%', '300');
$tabtable->add_tab('profile.inc', $acProfile);
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

switch($task)
{
  case 'accept':
    if (isset($_REQUEST['requested_user_id']) && isset($_REQUEST['authcode']))
    {
      if ($user = $GO_USERS->get_user($_REQUEST['requesting_user_id']))
      {
				$middle_name = $user['middle_name'] == '' ? '' : $user['middle_name'].' ';
				$user_name = $middle_name.$user['last_name'];

				if($GO_USERS->authorize($_REQUEST['requesting_user_id'], $_REQUEST['authcode'], $GO_SECURITY->user_id))
				{
					$feedback = $ac_auth_success.'<br /><br />';

					$mail_body = $ac_salutation." ".$sir_madam[$user['sex']]." ".$user_name.",\r\n\r\n";
					$mail_body .= $_SESSION['GO_SESSION']['name']." ".$ac_auth_accept_mail_body;

					sendmail($user['email'], $GO_CONFIG->webmaster_email,
								$GO_CONFIG->title, $ac_auth_accept_mail_title,
								$mail_body,'3 (Normal)', 'text/plain');
				}

      }else
      {
				$feedback = '<p class="Error">'.$ac_auth_error.'</p>';
      }
      $task = 'privacy';
      $tabtable->set_active_tab(2);
    }
    break;

  case 'decline':
    if (isset($_REQUEST['requested_user_id']) && isset($_REQUEST['authcode']))
    {
      if ($user = $GO_USERS->get_user($_REQUEST['requesting_user_id']))
      {
				$middle_name = $user['middle_name'] == '' ? '' : $user['middle_name'].' ';
				$user_name = $middle_name.$user['last_name'];

				$feedback = $ac_auth_decline.'<br /><br />';
				$mail_body = $ac_salutation." ".$sir_madam[$user['sex']]." ".$user_name.",\r\n\r\n";
				$mail_body .= $_SESSION['GO_SESSION']['name']." ".$ac_auth_decline_mail_body;
				sendmail($user['email'], $GO_CONFIG->webmaster_email, $GO_CONFIG->title,
								$ac_auth_decline_mail_title, $mail_body,'3 (Normal)', 'text/plain');

      }else
      {
				$feedback = '<p class="Error">'.$ac_auth_error.'</p>';
      }
      $task = 'privacy';
      $tabtable->set_active_tab(2);
    }
    break;

  case 'save_profile':

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
		
    require($GO_CONFIG->class_path."/validate.class.inc");
    $val = new validate();
    //translate the given birthdayto gmt unix time
    $birthday = date_to_db_date($_POST['birthday']);

    $val->error_required = $error_required;
    $val->error_min_length = $error_min_length;
    $val->error_max_length = $error_max_length;
    $val->error_expression = $error_email;
    $val->error_match = $error_match_auth;

    $val->name="first_name";
    $val->input=$_POST['first_name'];
    $val->max_length=50;
    $val->required=true;
    $val->validate_input();

    $val->name="last_name";
    $val->input=$_POST['first_name'];
    $val->max_length=50;
    $val->required=true;
    $val->validate_input();


    $val->name="email";
    $val->input=$_POST['email'];
    $val->max_length=75;
    $val->required=true;
    $val->expression="^([a-z0-9]+)([._-]([a-z0-9]+))*[@]([a-z0-9]+)([._-]([a-z0-9]+))*[.]([a-z0-9]){2}([a-z0-9])?([a-z0-9])?$";
    $val->validate_input();
    if ($val->validated == true)
    {
      if (!$GO_USERS->update_profile($GO_SECURITY->user_id, $first_name,
      					$middle_name, $last_name, $initials, $title, $_POST["sex"], $birthday,
      					$email, $work_phone, $home_phone, $fax, $cellular, $country,
      					$state, $city, $zip, $address, $company, $work_country,
      					$work_state, $work_city, $work_zip, $work_address, $work_fax,
      					$homepage,  $department, $function))
      {
				$feedback = "<p class=\"Error\">".$strSaveError."</p>";
      }elseif (isset($_POST['load_frames']))
      {
				header('Location: '.$GO_CONFIG->host);
				exit();
      }elseif ($_POST['close'] == 'true')
      {
				header('Location: '.$return_to);
				exit();
      }
    }else
    {
      $feedback ="<p class='Error'>".$errors_in_form."</p>";
    }


    break;

  case 'change_password':
    require($GO_CONFIG->class_path."/validate.class.inc");
    $val = new validate;
    $val->error_required = $error_required;
    $val->error_min_length = $error_min_length;
    $val->error_max_length = $error_max_length;
    $val->error_expression = $error_email;
    $val->error_match = $error_match_auth;

    $val->name="currentpassword";
    $val->input=$_POST['currentpassword'];
    $val->max_length=20;
    $val->required=true;
    $val->validate_input();

    $val->name="newpass1";
    $val->input=$_POST['newpass1'];
    $val->min_length=3;
    $val->max_length=20;
    $val->required=true;
    $val->validate_input();

    $val->name="newpass2";
    $val->input=$_POST['newpass2'];
    $val->min_length=3;
    $val->max_length=20;
    $val->required=true;
    $val->validate_input();

    $val->name="newpass1";
    $val->match1=$_POST['newpass1'];
    $val->match2=$_POST['newpass2'];
    $val->validate_input();

    if ($val->validated == true)
    {
      if (!$GO_USERS->check_password(smartstrip($_POST['currentpassword'])))
      {
				$feedback = "<p class=\"Error\">".$security_wrong_password."</p>";
      }else
      {
				if ($_POST['newpass1'] != "")
				{
					if ($GO_USERS->update_password($GO_SECURITY->user_id,
									smartstrip($_POST['newpass1']),
									smartstrip($_POST['currentpassword'])))
					{
						$email_module = $GO_MODULES->get_module('email');						
						if ($email_module)
						{
							require_once($email_module['class_path'].'email.class.inc');
							$email = new email();
							$email->re_encrypt_email($GO_SECURITY->user_id, smartstrip($_POST['currentpassword']), smartstrip($_POST['newpass1']));
						}
						$feedback = "<p class=\"Success\">".$security_password_update."</p>";

						if ($_POST['close'] == 'true')
						{
							header('Location: '.$return_to);
							exit();
						}
					}else
					{
						$feedback = "<p class=\"Error\">".$strSaveError."</p>";
					}
				}
      }
    }
    break;
}

$profile = $GO_USERS->get_user($GO_SECURITY->user_id);

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();

require($GO_THEME->theme_path."header.inc");
if ($_SESSION['GO_SESSION']['first_name'] != '' && $_SESSION['GO_SESSION']['last_name'] != '' && $_SESSION['GO_SESSION']['email'] != '')
{
	/*
	If the user manager of the authentication source is not equal
	then GO can't change the user's password.
	*/
  if (($auth_sources[$_SESSION['auth_source']]['type'] ==
		  $auth_sources[$_SESSION['auth_source']]['user_manager'])
  		 && $GO_CONFIG->allow_password_change)
  {
    $tabtable->add_tab('security.inc', $acSecurity);
  }
  $tabtable->add_tab('privacy.inc', $acPrivacy);
  $tabtable->add_tab('statistics.inc', $acStatistics);
}
?>
<form action="<?php echo $_SERVER['PHP_SELF']; ?>" name="account_form" method="post">
<input type="hidden" name="task" />
<input type="hidden" name="close" value="false" />
<?php
$tabtable->print_head();
require($tabtable->get_active_tab_id());
$tabtable->print_foot();
?>
</form>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
