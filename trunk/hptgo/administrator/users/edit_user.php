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
require($GO_LANGUAGE->get_base_language_file('users'));
require($GO_LANGUAGE->get_base_language_file('common'));

$GO_SECURITY->authenticate(true);


require($GO_CONFIG->class_path."validate.class.inc");
$val = new validate();

$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
  if ($_REQUEST['pass1'] != '')
  {
    $val->error_min_length = $error_min_length;
    $val->error_max_length = $error_max_length;
    $val->error_match = $admin_pass_match;

    $val->name="pass1";
    $val->input=$_POST['pass1'];
    $val->min_length=3;
    $val->max_length=20;
    $val->validate_input();

    $val->name="pass2";
    $val->input=$_POST['pass2'];
    $val->min_length=3;
    $val->max_length=20;
    $val->validate_input();

    $val->name="pass";
    $val->match1=$_POST['pass1'];
    $val->match2=$_POST['pass2'];
    $val->validate_input();

    if ($val->validated)
    {
      if(!$GO_USERS->update_password($_POST['id'], $_POST['pass1']))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p><br />';
      }
    }else
    {
      $feedback = '<p class="Error">'.$errors_in_form.'</p><br />';;
    }
  }

  $GO_MODULES->get_modules();
  while ($GO_MODULES->next_record())
  {
    $could_read = $GO_SECURITY->has_permission($_POST['id'], $GO_MODULES->f('acl_read'));
    $can_read = isset($_POST['module_acl_read']) ? in_array($GO_MODULES->f('acl_read'), $_POST['module_acl_read']) : false;

    if ($could_read && !$can_read)
    {
      $GO_SECURITY->delete_user_from_acl($_POST['id'], $GO_MODULES->f('acl_read'));
    }

    if ($can_read && !$could_read)
    {
      $GO_SECURITY->add_user_to_acl($_POST['id'], $GO_MODULES->f('acl_read'));
    }

    $could_write = $GO_SECURITY->has_permission($_POST['id'], $GO_MODULES->f('acl_write'));
    $can_write = isset($_POST['module_acl_write']) ? in_array($GO_MODULES->f('acl_write'), $_POST['module_acl_write']) : false;

    if ($could_write && !$can_write)
    {
      $GO_SECURITY->delete_user_from_acl($_POST['id'], $GO_MODULES->f('acl_write'));
    }

    if ($can_write && !$could_write)
    {
      $GO_SECURITY->add_user_to_acl($_POST['id'], $GO_MODULES->f('acl_write'));
    }
  }

  $GO_GROUPS->get_all_groups();
  $groups2 = new GO_GROUPS();
  while($GO_GROUPS->next_record())
  {
    $is_in_group = $groups2->is_in_group($_REQUEST['id'], $GO_GROUPS->f('id'));
    $should_be_in_group = isset($_POST['user_groups']) ? in_array($GO_GROUPS->f('id'), $_POST['user_groups']) : false;

    if ($is_in_group && !$should_be_in_group)
    {
      $groups2->delete_user_from_group($_REQUEST['id'], $GO_GROUPS->f('id'));
    }

    if (!$is_in_group && $should_be_in_group)
    {
      $groups2->add_user_to_group($_REQUEST['id'], $GO_GROUPS->f('id'));
    }
  }

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

    if (!$GO_USERS->update_profile($_REQUEST['id'], $first_name,
	  $middle_name, $last_name, $initials, $title, $_POST["sex"], $birthday,
	  $email, $work_phone, $home_phone, $fax, $cellular, $country,
	  $state, $city, $zip, $address, $company, $work_country,
	  $work_state, $work_city, $work_zip, $work_address, $work_fax,
	  $homepage,  $department, $function))
    {
      $feedback = "<p class=\"Error\">".$strSaveError."</p>";
    }elseif ($_POST['close'] == 'true' && !isset($feedback))
    {
      header('Location: '.$return_to);
      exit();
    }
  }else
  {
    $feedback ="<p class='Error'>".$errors_in_form."</p>";
  }
}

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();

$page_title = $menu_users;
require($GO_THEME->theme_path."header.inc");

$profile = $GO_USERS->get_user($_REQUEST['id']);
if (!$profile)
{
  $feedback = '<p class="Error">'.$strDataError.'</p>';
}

?>
<form method="post" name="user" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="id" value="<?php echo $_REQUEST['id']; ?>" />
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>" />
<input type="hidden" name="close" value="false" />
<input type="hidden" name="task" value="" />

  <?php
if (isset($feedback))
{
  echo $feedback.'<br />';;
}
?>
<table border="0" cellpadding="10" cellspacing="3" width="600">
<tr>
<td colspan="2"><h1><?php echo $user_profile; ?> <?php echo $profile['username']; ?></h1></td>
</tr>
<tr>
<td valign="top">
<table border="0" class="normal" cellpadding="2" cellspacing="0">

<?php
if (isset($val->error["name"]))
{
  ?>
    <tr>
    <td class="Error" colspan="2">
    <?php echo $val->error["name"]; ?>
    </td>
    </tr>
    <?php } ?>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strLastName; ?>*:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="last_name" size="40" maxlength="50" value="<?php echo $profile["last_name"]; ?>"></td>
    </tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strMiddleName; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="middle_name" size="40" maxlength="50" value="<?php echo $profile["middle_name"]; ?>"></td>
    </tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strFirstName; ?>*:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="first_name" size="40" maxlength="50" value="<?php echo $profile["first_name"]; ?>"></td>
    </tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strTitle; ?> / <?php echo $strInitials; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="title" size="11" maxlength="12" value="<?php echo $profile['title']; ?>">&nbsp;/&nbsp;<input type="text" class="textbox"  name="initials" size="11" maxlength="50" value="<?php echo $profile['initials']; ?>"></td>
    </tr>
    <tr>
    <td align="right" nowrap><?php echo $strSex; ?>:</td>
    <td>
    <?php
    $radiolist = new radio_list('sex', $profile['sex']);
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
    $birthday = db_date_to_date($profile['birthday']);
    $datepicker->print_date_picker('birthday', $_SESSION['GO_SESSION']['date_format'], $birthday);
    ?>
    </td>
    </tr>
    <tr><td colspan="2">&nbsp;</td></tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strAddress; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="address" size="40" maxlength="50" value="<?php echo $profile["address"]; ?>"></td>
    </tr>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strZip; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="zip" size="40" maxlength="20" value="<?php echo $profile["zip"]; ?>"></td>
    </tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strCity; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="city" size="40" maxlength="50" value="<?php echo $profile["city"]; ?>">		</td>
    </tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strState; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="state" size="40" maxlength="30" value="<?php echo $profile["state"]; ?>"></td>
    </tr>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strCountry; ?>:&nbsp;</td>
    <td width="100%"><input type="text" class="textbox"  name="country" size="40" maxlength="30" value="<?php echo $profile["country"]; ?>"></td>
    </tr>
    <tr><td colspan="2">&nbsp;</td></tr>
    <?php
    if (isset($val->error["email"]))
{
  ?>
    <tr>
    <td class="Error" colspan="2">
    <?php echo $val->error["email"]; ?>
    </td>
    </tr>
    <?php } ?>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strEmail; ?>*:&nbsp;</td>
    <td><input type="text" class="textbox"  name="email" size="40" value="<?php echo $profile["email"]; ?>" maxlength="50"></td>
    </tr>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strPhone; ?>:&nbsp;</td>
    <td><input type="text" class="textbox"  name="home_phone" size="40" value="<?php echo $profile["home_phone"]; ?>" maxlength="20"></td>
    </tr>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strFax; ?>:&nbsp;</td>
    <td><input type="text" class="textbox"  name="fax" size="40" value="<?php echo $profile["fax"]; ?>" maxlength="20"></td>
    </tr>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strCellular; ?>:&nbsp;</td>
    <td><input type="text" class="textbox"  name="cellular" size="40" value="<?php echo $profile["cellular"]; ?>" maxlength="20"></td>
    </tr>


    </table>
    </td>
    <td valign="top">
    <table border="0" class="normal" cellpadding="2" cellspacing="0">

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strCompany; ?>:&nbsp;</td>
    <td><input type="text" class="textbox"  name="company" size="40" value="<?php echo $profile["company"]; ?>" maxlength="50"></td>
    </tr>
    <tr heigth="25">
    <td align="right" nowrap><?php echo $strDepartment; ?>:&nbsp;</td>
    <td><input type="text" class="textbox"  name="department" size="40" value="<?php echo $profile["department"]; ?>" maxlength="50"></td>
    </tr>

    <tr heigth="25">
    <td align="right" nowrap><?php echo $strFunction; ?>:&nbsp;</td>
    <td><input type="text" class="textbox" name="function" size="40" value="<?php echo $profile["function"]; ?>" maxlength="50"></td>
    </tr>
    <tr><td colspan="2">&nbsp;</td></tr>
    <tr>
    <td align="right" nowrap>
    <?php echo $strAddress; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_address" size="40" value="<?php echo $profile["work_address"]; ?>" maxlength="100">
    </td>
    </tr>

    <tr>
    <td align="right" nowrap>
    <?php echo $strZip; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_zip" size="40" value="<?php echo $profile["work_zip"]; ?>" maxlength="20">
    </td>
    </tr>
    <tr>
    <td align="right" nowrap>
    <?php echo $strCity; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_city" size="40" value="<?php echo $profile["work_city"]; ?>" maxlength="50">
    </td>
    </tr>


    <tr>
    <td align="right" nowrap>
    <?php echo $strState; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_state" size="40" value="<?php echo $profile["work_state"]; ?>" maxlength="50">
    </td>
    </tr>

    <tr>
    <td align="right" nowrap>
    <?php echo $strCountry; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_country" size="40" value="<?php echo $profile["work_country"]; ?>" maxlength="50">
    </td>
    </tr>
    <tr><td colspan="2">&nbsp;</td></tr>
    <tr>
    <td align="right" nowrap>
    <?php echo $strPhone; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_phone" size="40" value="<?php echo $profile["work_phone"]; ?>" maxlength="20">
    </td>
    </tr>
    <tr>
    <td align="right" nowrap>
    <?php echo $strFax; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="work_fax" size="40" value="<?php echo $profile["work_fax"]; ?>" maxlength="20">
    </td>
    </tr>
    <tr>
    <td align="right" nowrap>
    <?php echo $strHomepage; ?>:&nbsp;
    </td>
    <td>
    <input type="text" class="textbox"  name="homepage" size="40" value="<?php echo $profile["homepage"] ?>" maxlength="100">
    </td>
    </tr>

    </table>
    </td>
    </tr>
    <tr>
    <td colspan="2">
    <h2><?php echo $ac_login_info; ?></h2>
    <table border="0" cellpadding="0" cellspacing="3">
    <tr>
    <td><?php echo $ac_registration_time; ?>:</td>
    <td><?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $profile["registration_time"]+($_SESSION['GO_SESSION']['timezone']*3600)); ?></td>
    </tr>
    <tr>
    <td><?php echo $ac_lastlogin; ?>:</td>
    <td><?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $profile["lastlogin"]+($_SESSION['GO_SESSION']['timezone']*3600)); ?></td>
    </tr>
    <tr>
    <td><?php echo $ac_logins; ?>:</td>
    <td><?php echo number_format($profile["logins"], 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']); ?></td>
    </tr>
    </table>
    </td>
    </tr>
    <tr>
    <td valign="top">
    <h2><?php echo $admin_modules; ?></h2>
    <?php 
    echo $admin_module_access;
    echo '<br /><br />';
    echo '<table border="0" cellpadding="4" cellspacing="0">';
    echo '<tr><td><h3>'.$admin_module.'</h3>';
    echo '<td><h3>'.$admin_use.'</h3>';
    echo '<td><h3>'.$admin_manage.'</h3>';

    $module_count = $GO_MODULES->get_modules();
while($GO_MODULES->next_record())
{
  $lang_var = isset($lang_modules[$GO_MODULES->f('id')]) ? $lang_modules[$GO_MODULES->f('id')] : $GO_MODULES->f('id');

  echo '<tr><td>'.$lang_var.'</td><td align="center">';
  $checkbox = new checkbox('module_acl_read[]', $GO_MODULES->f('acl_read'), '', $GO_SECURITY->has_permission($_REQUEST['id'], $GO_MODULES->f('acl_read')));
  echo '</td><td align="center">';
  $checkbox = new checkbox('module_acl_write[]', $GO_MODULES->f('acl_write'), '', $GO_SECURITY->has_permission($_REQUEST['id'], $GO_MODULES->f('acl_write')));
  echo '</td></tr>';			
}	
echo '</table>';
?>
</td>
<td valign="top">
<h2><?php echo $admin_groups; ?></h2>
<?php echo $admin_groups_user; ?>:<br /><br />
<?php 
echo '<input type="hidden" value="'.$GO_CONFIG->group_everyone.'" name="user_groups[]" />';
$dropbox = new dropbox();
$GO_GROUPS->get_all_groups();
$groups2 = new GO_GROUPS();
$user_groups = array();
while($GO_GROUPS->next_record())
{
  if ($GO_GROUPS->f('id') != $GO_CONFIG->group_everyone)
  {
    if ($groups2->is_in_group($_REQUEST['id'], $GO_GROUPS->f('id')))
    {
      $user_groups[] = $GO_GROUPS->f('id');
    }
    $dropbox->add_value($GO_GROUPS->f('id'), $GO_GROUPS->f('name'));
  }
}
$dropbox->print_dropbox('user_groups[]', $user_groups, '',true, 10, 200);

?>
</td>
</tr>
<tr>
<td colspan="2">
<h2><?php echo $admin_change_password; ?></h2>
<table border="0">
<?php
if (isset($val->error['pass']))
{
  echo '<tr><td colspan="2" class="Error">'.$val->error['pass'].'</td></tr>';

}

if (isset($val->error['pass1']))
{
  echo '<tr><td colspan="2" class="Error">'.$val->error['pass1'].'</td></tr>';

}
?>
<tr>
<td><?php echo $admin_password; ?>:</td>
<td><input class="textbox" name="pass1" type="password" size="40" /></td>
</tr>
<?php
if (isset($val->error['pass2']))
{
  echo '<tr><td colspan="2" class="Error">'.$val->error['pass2'].'</td></tr>';

}
?>
<tr>
<td><?php echo $admin_confirm_password; ?>:</td>
<td><input class="textbox" name="pass2" type="password" size="40" /></td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan="2">
<?php
$button = new button($cmdOk, "javascript:_save('save', 'true')");
echo '&nbsp;&nbsp;';
$button = new button($cmdApply, "javascript:_save('save', 'false')");
echo '&nbsp;&nbsp;';
$button = new button($cmdClose, 'javascript:document.location=\''.$return_to.'\';');
?>
</td>
</tr>
</table>

</form>
  <script type="text/javascript">
function _save(task, close)
{
  document.forms[0].task.value = task;
  document.forms[0].close.value = close;
  document.forms[0].submit();
}
</script>

<?php
require($GO_THEME->theme_path."footer.inc");
?>
