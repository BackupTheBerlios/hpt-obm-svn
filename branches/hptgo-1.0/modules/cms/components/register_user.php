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

require('../../../Group-Office.php');
require($GO_LANGUAGE->get_base_language_file('users'));

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();

$page_title = $registration_title;
require($GO_THEME->theme_path."header.inc");
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
  $first_name = smart_addslashes(trim($_POST['first_name']));
  $middle_name = smart_addslashes(trim($_POST['middle_name']));
  $last_name = smart_addslashes(trim($_POST['last_name']));
  $initials = smart_addslashes($_POST["initials"]);
  $birthday = smart_addslashes($_POST["birthday"]);
  $email = smart_addslashes($_POST["email"]);
  $home_phone = smart_addslashes($_POST["home_phone"]);
  $fax = smart_addslashes($_POST["fax"]);
  $cellular = smart_addslashes($_POST["cellular"]);
  $country = smart_addslashes($_POST["country"]);
  $state = smart_addslashes($_POST["state"]);
  $city = smart_addslashes($_POST["city"]);
  $zip = smart_addslashes($_POST["zip"]);
  $address = smart_addslashes($_POST["address"]);
  $company = smart_addslashes($_POST["company"]);
  
  if ($first_name == '' || $last_name == '' || $email == '')
  {
    $feedback = '<p class="Error">'.$error_missing_field.'</p>';
  }elseif(!eregi("^([a-z0-9]+)([._-]([a-z0-9]+))*[@]([a-z0-9]+)([._-]([a-z0-9]+))*[.]([a-z0-9]){2}([a-z0-9])?$", $email))
  {
    $feedback = '<p class="Error">'.$error_email.'</p>';
  }else
  {	  
    $mailbody = '
      <html>
      <head>
      <title>'.$GO_CONFIG->title.'</title>
      </head>
      <body>
      <table border="0" class="normal" cellpadding="0" cellspacing="3" width="100%">
      <tr>
      <td align="right" nowrap>'.$strFirstName.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($first_name).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strMiddleName.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($middle_name).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strLastName.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($last_name).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strSex.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($strSexes[$_POST['sex']]).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strCompany.':&nbsp;</td>
      <td>'.empty_to_stripe($company).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strAddress.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($address).'</td>
      </tr>

      <tr>
      <td align="right" nowrap>'.$strZip.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($zip).'</td>
      </tr>

      <tr>
      <td align="right" nowrap>'.$strCity.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($city).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strState.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($state).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strCountry.':&nbsp;</td>
      <td width="100%">'.empty_to_stripe($country).'</td>
      </tr>
      <tr>
      <td colspan="2">&nbsp;</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strPhone.':&nbsp;</td>
      <td>'.empty_to_stripe($home_phone).'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strFax.':&nbsp;</td>
      <td>'.empty_to_stripe($fax).'</td>
      </tr

      <tr>
      <td align="right" nowrap>'.$strEmail.':&nbsp;</td>
      <td>'.$email.'</td>
      </tr>
      <tr>
      <td align="right" nowrap>'.$strCellular.':&nbsp;</td>
      <td>'.empty_to_stripe($cellular).'</td>
      </tr>
      <tr>
      <td colspan="2" nowrap>';

    $mailbody .= '<a class="normal" href="'.$GO_CONFIG->full_url.'administrator/users/register.php'.
      '?first_name='.urlencode($first_name).
      '&middle_name='.urlencode($middle_name).
      '&last_name='.urlencode($last_name).
      '&initials='.urlencode($initials).
      '&birthday='.urlencode($birthday).
      '&username='.urlencode($username).
      '&email='.urlencode($email).
      '&address='.urlencode($address).
      '&zip='.urlencode($zip).
      '&city='.urlencode($city).
      '&state='.urlencode($state).
      '&country='.urlencode($country).
      '&home_phone='.urlencode($home_phone).
      '&fax='.urlencode($fax).
      '&cellular='.urlencode($cellular).
      '&company='.urlencode($company).
      '">'.$register_accept.'</a>&nbsp;&nbsp;';

    $mailbody .= '<a class="normal" href="'.$GO_CONFIG->full_url.'modules/addressbook/contact.php'.
      '?first_name='.urlencode($first_name).
      '&middle_name='.urlencode($middle_name).
      '&last_name='.urlencode($last_name).
      '&sex='.urlencode($sex).
      '&initials='.urlencode($initials).
      '&birthday='.urlencode($birthday).
      '&email='.urlencode($email).
      '&address='.urlencode($address).
      '&zip='.urlencode($zip).
      '&city='.urlencode($city).
      '&state='.urlencode($state).
      '&country='.urlencode($country).
      '&home_phone='.urlencode($home_phone).
      '&fax='.urlencode($fax).
      '&cellular='.urlencode($cellular).
      '&company='.urlencode($company).
      '">'.$register_addressbook.'</a>'.
      '</td></tr></table></body></html>';
 
	$middle_name = ($middle_name == '') ? '' : ' '.$middle_name;
	$name = $_POST['last_name'].$middle_name.' '.$_POST['first_name'];

	if(!sendmail($GO_CONFIG->webmaster_email, $email, $name, $register_new_user, $mailbody, '3', 'text/HTML'))
	{
	  $feedback = '<p class="Error">'.$cms_sendmail_error.'</p>';
	}else
	{
	  echo $register_thanks;
	  require($GO_THEME->theme_path."footer.inc");
	  exit();
	}  
  }
}
?>
<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST" name="register">
<?php if (isset($feedback)) echo $feedback; ?>
<table border="0" cellpadding="0" cellspacing="3">
<tr>
<td align="right" nowrap>
<?php echo $strUsername; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="username" size="40" value="<?php if(isset($_POST['username'])) echo $_POST['username']; ?>" maxlength="50">
</td>
</tr>
<tr><td colspan="2">&nbsp;</td></tr>
<tr>
<td align="right" nowrap>
<?php echo $strCompany; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="company" size="40" value="<?php if(isset($_POST['company'])) echo $_POST['company']; ?>" maxlength="50">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strFirstName; ?>*:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="first_name" size="40" value="<?php if(isset($_POST['first_name'])) echo $_POST['first_name']; ?>" maxlength="50">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strMiddleName; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="middle_name" size="40" value="<?php if(isset($_POST['middle_name'])) echo $_POST['middle_name']; ?>" maxlength="50">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strLastName; ?>*:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="last_name" size="40" value="<?php if(isset($_POST['last_name'])) echo $_POST['last_name']; ?>" maxlength="50">
</td>
</tr>
<tr heigth="25">
<td align="right" nowrap><?php echo $strInitials; ?>:&nbsp;</td>
<td width="100%"><input type="text" class="textbox"  name="initials" size="40" maxlength="50" value="<?php if(isset($_REQUEST['initials'])) echo $_REQUEST['initials']; ?>"></td>
</tr>
<tr>
<td align="right" nowrap><?php echo $strSex; ?>:</td>
<td>
<?php
$sex = isset($_REQUEST['sex']) ? $_REQUEST['sex'] : 'M';
$radiolist = new radio_list('sex', $sex);
$radiolist->add_option('M', $strSexes['M']);
echo '&nbsp;';
$radiolist->add_option('F', $strSexes['F']);
?>
</td>
</tr>
<tr>
<td align="right" nowrap><?php echo $strBirthday; ?>:</td>
<td>
<?php
$birthday = isset($_REQUEST['birthday']) ? $_REQUEST['birthday'] : '';
$datepicker->print_date_picker('birthday', $GO_CONFIG->date_formats[0], $birthday);
?>
</td>
</tr>
<tr><td colspan="2">&nbsp;</td></tr>

<tr>
<td align="right" nowrap>
<?php echo $strAddress; ?>*:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="address" size="40" value="<?php  if(isset($_POST['address'])) echo $_POST['address']; ?>" maxlength="100">
</td>
</tr>

<tr>
<td align="right" nowrap>
<?php echo $strZip; ?>*:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="zip" size="40" value="<?php if(isset($_POST['zip'])) echo $_POST['zip']; ?>" maxlength="20">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strCity; ?>*:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="city" size="40" value="<?php if(isset($_POST['city'])) echo $_POST['city']; ?>" maxlength="50">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strState; ?>*:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="state" size="40" value="<?php if(isset($_POST['state'])) echo $_POST['state']; ?>" maxlength="50">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strCountry; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="country" size="40" value="<?php if(isset($_POST['country'])) echo $_POST['country']; ?>" maxlength="50">
</td>
</tr>
<tr><td colspan="2">&nbsp;</td></tr>

<tr>
<td align="right" nowrap>
<?php echo $strPhone; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="home_phone" size="40" value="<?php if(isset($_POST['home_phone'])) echo $_POST['home_phone']; ?>" maxlength="20">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strCellular; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="cellular" size="40" value="<?php if(isset($_POST['cellular'])) echo $_POST['cellular']; ?>" maxlength="20">
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $strFax; ?>:&nbsp;
</td>
<td>
<input type="text" class="textbox"  name="fax" size="40" value="<?php if(isset($_POST['fax'])) echo $_POST['fax']; ?>" maxlength="20">
</td>
</tr>
<tr>
<td align="right" nowrap valign="top">
<?php echo $strEmail; ?>*:&nbsp;
</td>
<td class="small">
<input type="text" class="textbox"  name="email" size="40" value="<?php if(isset($_POST['email'])) echo $_POST['email']; ?>" maxlength="75"><br />
</td>
</tr>
<tr>
<td colspan="2">
<?php	
$button = new button($cmdOk, 'javascript:document.forms[0].submit()');	
echo '&nbsp;&nbsp;';
$button = new button($cmdReset, 'javascript:document.forms[0].reset()');
?>
</td>
</tr>
</table>
<?php
require($GO_THEME->theme_path."footer.inc");
?>

