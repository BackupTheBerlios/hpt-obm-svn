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

if (!$profile)
{
	echo "<p class=\"Error\">".$strDataError."</p>";
}
?>
<table border="0" cellpadding="2" cellspacing="0">
<tr heigth="25">
	<td colspan="2"><?php if (isset($feedback)) echo $feedback; ?>
	<?php echo $acProfileText; ?><br /><br />
	</td>
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
		if ($profile['sex'] != 'M' && $profile['sex'] != 'F')
		{
			$profile['sex'] = 'M';
		}
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
<tr heigth="25">
	<td colspan="2">
	<?php
	/*$button = new button($cmdOk, "javascript:_save('save_profile', 'true')");
	if ($_SESSION['GO_SESSION']['first_name'] != '' && $_SESSION['GO_SESSION']['last_name'] != '' && $_SESSION['GO_SESSION']['email'] != '')
	{
		echo '&nbsp;&nbsp;';
		$button = new button($cmdApply, "javascript:_save('save_profile', 'false')");
		echo '&nbsp;&nbsp;';
		$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
	}else
	{
		echo '<input type="hidden" name="load_frames" value="true" />';
	}*/
	if ($_SESSION['GO_SESSION']['first_name'] == '' || $_SESSION['GO_SESSION']['last_name'] == '' || $_SESSION['GO_SESSION']['email'] == '')
	{
		echo '<input type="hidden" name="load_frames" value="true" />';
	}
	$button = new button($cmdSave, "javascript:_save('save_profile', 'false')");
	
	?></td>
</tr>
</table>
<script type="text/javascript">
function _save(task, close)
{
	document.forms[0].task.value = task;
	document.forms[0].close.value = close;
	document.forms[0].submit();
}
</script>
