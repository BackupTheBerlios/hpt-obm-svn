<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST" name="register">
<table border="0" cellpadding="10" cellspacing="3">
<tr>
	<td colspan="2"><h1><?php echo $admin_new_user; ?></h1>
	<?php if (isset($error)) echo $error; ?>
	<?php echo $registration_text; ?><br /><br />
	<h2><?php echo $registration_personal_info; ?></h2>
</tr>
<tr>
	<td valign="top">
	<table border="0" cellpadding="0" cellspacing="3">
	<?php
	if (isset($val->error["first_name"]))
	{
	?>
	<tr>
		<td class="Error" colspan="2">
			<?php echo $val->error["first_name"]; ?>
		</td>
	</tr>
	<?php
	}
	?>
		<?php
		if (isset($val->error["last_name"]))
		{
		?>
		<tr>
			<td class="Error" colspan="2">
				<?php echo $val->error["last_name"]; ?>
			</td>
		</tr>
		<?php
		}
	?>
	<tr heigth="25">
		<td align="right" nowrap><?php echo $strLastName; ?>*:&nbsp;</td>
		<td width="100%"><input type="text" class="textbox"  name="last_name" size="30" maxlength="50" value="<?php if(isset($last_name)) echo stripslashes($last_name); ?>"></td>
	</tr>
	<tr heigth="25">
		<td align="right" nowrap><?php echo $strMiddleName; ?>:&nbsp;</td>
		<td width="100%"><input type="text" class="textbox"  name="middle_name" size="30" maxlength="50" value="<?php if(isset($middle_name)) echo stripslashes($middle_name); ?>"></td>
	</tr>
	<tr heigth="25">
		<td align="right" nowrap><?php echo $strFirstName; ?>*:&nbsp;</td>
		<td width="100%"><input type="text" class="textbox"  name="first_name" size="30" maxlength="50" value="<?php if(isset($first_name)) echo stripslashes($first_name); ?>"></td>
	</tr>
	<tr heigth="25">
		<td align="right" nowrap><?php echo $strTitle; ?> / <?php echo $strInitials; ?>:&nbsp;</td>
		<td width="100%"><input type="text" class="textbox"  name="title" size="10" maxlength="12" value="<?php if(isset($title)) echo stripslashes($title); ?>">&nbsp;/&nbsp;<input type="text" class="textbox"  name="initials" size="12" maxlength="50" value="<?php if(isset($initials)) echo stripslashes($initials); ?>"></td>
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
		$datepicker->print_date_picker('birthday', $_SESSION['GO_SESSION']['date_format'], $birthday);
		?>
		</td>
	</tr>
	<tr><td colspan="2">&nbsp;</td></tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strAddress; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="address" size="30" value="<?php  if(isset($address)) echo stripslashes($address); ?>" maxlength="100">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strZip; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="zip" size="30" value="<?php if(isset($zip)) echo stripslashes($zip); ?>" maxlength="20">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strCity; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="city" size="30" value="<?php if(isset($city)) echo stripslashes($city); ?>" maxlength="50">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strState; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="state" size="30" value="<?php if(isset($state)) echo stripslashes($state); ?>" maxlength="50">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strCountry; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="country" size="30" value="<?php if(isset($country)) echo stripslashes($country); ?>" maxlength="50">
		</td>
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strPhone; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="home_phone" size="30" value="<?php if(isset($home_phone)) echo stripslashes($home_phone); ?>" maxlength="20">
		</td>
	</tr>



	<tr>
		<td align="right" nowrap>
		<?php echo $strFax; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="fax" size="30" value="<?php if(isset($fax)) echo stripslashes($fax); ?>" maxlength="20">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strCellular; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="cellular" size="30" value="<?php if(isset($cellular)) echo stripslashes($cellular); ?>" maxlength="20">
		</td>
	</tr>
	<?php
	if (isset($val->error["email"]))
	{
	?>
	<tr>
		<td colspan="2" class="Error">
			<?php echo $val->error["email"]; ?>
		</td>
	</tr>
	<?php } ?>
	<tr>
		<td align="right" nowrap valign="top">
		<?php echo $strEmail; ?>*:&nbsp;
		</td>
		<td class="small">
		<input type="text" class="textbox"  name="email" size="30" value="<?php if(isset($email)) echo stripslashes($email); ?>" maxlength="75"><br />
		</td>
	</tr>


	</table>
	</td>
	<td valign="top">
		<table border="0" cellpadding="0" cellspacing="3">
		<tr>
			<td align="right" nowrap>
			<?php echo $strCompany; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="company" size="30" value="<?php if(isset($company)) echo stripslashes($company); ?>" maxlength="50">
			</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strDepartment; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="department" size="30" value="<?php if(isset($department)) echo stripslashes($department); ?>" maxlength="50">
			</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strFunction; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="function" size="30" value="<?php if(isset($function)) echo stripslashes($function); ?>" maxlength="50">
			</td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strAddress; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_address" size="30" value="<?php if(isset($work_address)) echo stripslashes($work_address); ?>" maxlength="100">
			</td>
		</tr>

		<tr>
			<td align="right" nowrap>
			<?php echo $strZip; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_zip" size="30" value="<?php if(isset($work_zip)) echo stripslashes($work_zip); ?>" maxlength="20">
			</td>
		</tr>

		<tr>
			<td align="right" nowrap>
			<?php echo $strCity; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_city" size="30" value="<?php if(isset($work_city)) echo stripslashes($work_city); ?>" maxlength="50">
			</td>
		</tr>


		<tr>
			<td align="right" nowrap>
			<?php echo $strState; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_state" size="30" value="<?php if(isset($work_state)) echo stripslashes($work_state); ?>" maxlength="50">
			</td>
		</tr>

		<tr>
			<td align="right" nowrap>
			<?php echo $strCountry; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_country" size="30" value="<?php if(isset($work_country)) echo stripslashes($work_country); ?>" maxlength="50">
			</td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>

		<tr>
			<td align="right" nowrap>
			<?php echo $strWorkphone; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_phone" size="30" value="<?php if(isset($work_phone)) echo stripslashes($work_phone); ?>" maxlength="20">
			</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strWorkFax; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="work_fax" size="30" value="<?php if(isset($work_fax)) echo stripslashes($work_fax); ?>" maxlength="20">
			</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strHomepage; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="homepage" size="30" value="<?php if(isset($homepage)) echo stripslashes($homepage); ?>" maxlength="100">
			</td>
		</tr>
		</table>
	</td>
</tr>
<tr>
	<td colspan="2">
	<h2><?php echo $registration_account_info; ?></h2>
   <?php echo $registartion_username_text; ?><br><br>
	<table border="0">
	<tr>
	<?php
	if (isset($val->error["username"]))
	{
	?>
	<tr>
	        <td colspan="2" class="Error">
	                <?php echo $val->error["username"]; ?>
	        </td>
	</tr>
	<?php } ?>
	
	<tr>
		<td align="right" nowrap>
	        <?php echo $strUsername; ?>*:&nbsp;
	      	</td>
	      	<td width="100%">
	        <input type="text" class="textbox"  name="username" size="30" value="<?php if(isset($username)) echo stripslashes($username); ?>" maxlength="20">
	       </td>
	</tr>
	<?php
	if (isset($val->error["pass1"]))
	{
	?>
	<tr>
	        <td colspan="2" class="Error">
	                <?php echo $val->error["pass1"]; ?>
	        </td>
	</tr>
	<?php } ?>
	
	<tr>
		<td align="right" nowrap>
	        <?php echo $strPassword; ?>*:&nbsp;
	      	</td>
	      	<td>
	        <input type=password class="textbox" name=pass1 size="30" maxlength="20" value="<?php if(isset($pass1)) echo stripslashes($pass1); ?>">
	        </td>
	</tr>
	<?php
	if (isset($val->error["pass2"]))
	{
	?>
	<tr>
	        <td colspan="2" class="Error">
	                <?php echo $val->error["pass2"]; ?>
	        </td>
	</tr>
	<?php } ?>
	<tr>
		<td align="right" nowrap>
	        <?php echo $strPasswordConfirm; ?>*:&nbsp;
	      	</td>
	      	<td>
	        <input type="password" class="textbox" name="pass2" size="30" maxlength="20" value="<?php if(isset($pass2)) echo stripslashes($pass2); ?>">
	        </td>
	</tr>
	<tr><td>&nbsp;</td></tr>
	<tr>
		<td nowrap align="right">
			<?php echo $reg_language; ?>:&nbsp;
		</td>
		<td>
			<?php
			$dropbox= new dropbox();
			$languages = $GO_LANGUAGE->get_languages();
			while($language = array_shift($languages))
			{
				$dropbox->add_value($language['code'], $language['description']);
			}
			$dropbox->print_dropbox("language", $GO_CONFIG->language);
			?>
		</td>
	</tr>
	<tr>
		<td nowrap align="right">
			<?php echo $reg_theme; ?>:&nbsp;
		</td>
		<td>
			<?php
			$theme = isset($_REQUEST['theme']) ? $_REQUEST['theme'] : $GO_CONFIG->theme;
			$themes = $GO_THEME->get_themes();
			$dropbox = new dropbox();
			$dropbox->add_arrays($themes, $themes);
			$dropbox->print_dropbox("theme", $theme);
			?>
		</td>
	</tr>
	<tr><td>&nbsp;</td></tr>
	<tr>
	  <td colspan="2">
	  <?php
	  $checkbox = new checkbox('visible', 'true', $registration_visibility, isset($_POST['visible'])); 
		?>
		</td>
	</tr>		
	</table>
	</td>
</tr>
<tr>
	<td>		
	<?php
	echo '<h2>'.$admin_modules.'</h2>';
	echo $admin_module_access.'<br /><br />';

	echo '<table border="0" cellpadding="4" cellspacing="0">';
	echo '<tr><td><h3>'.$admin_module.'</h3>';
	echo '<td><h3>'.$admin_use.'</h3>';
	echo '<td><h3>'.$admin_manage.'</h3>';

	$module_count = $GO_MODULES->get_modules();
	while($GO_MODULES->next_record())
	{
		$lang_var = isset($lang_modules[$GO_MODULES->f('id')]) ? $lang_modules[$GO_MODULES->f('id')] : $GO_MODULES->f('id');
		
		echo '<tr><td>'.$lang_var.'</td><td align="center">';
		$checkbox = new checkbox('module_acl[]', $GO_MODULES->f('acl_read'), '', in_array($GO_MODULES->f('acl_read'), $module_acl));
		echo '</td><td align="center">';
		$checkbox = new checkbox('module_acl[]', $GO_MODULES->f('acl_write'), '', in_array($GO_MODULES->f('acl_write'), $module_acl));
		echo '</td></tr>';			
	}	
	echo '</table>';
  ?>
  </td>
  <td valign="top">
	<h2><?php echo $admin_groups; ?></h2>
	<?php echo $admin_groups_user; ?>:<br /><br />
	<?php 
	$dropbox = new dropbox();
	$GO_GROUPS->get_all_groups();
	$groups2 = new GO_GROUPS();
	$user_groups = array();
	while($GO_GROUPS->next_record())
	{
		if ($GO_GROUPS->f('id') != $GO_CONFIG->group_everyone)
		{
			if (isset($_POST['user_groups']) && in_array($GO_GROUPS->f('id'), $_POST['user_groups']))
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
  <?php
	$button = new button($cmdOk, 'javascript:document.forms[0].submit()');
	echo '&nbsp;&nbsp;';
	$button = new button($cmdCancel, 'javascript:document.location=\'index.php\';');
	?>
  </td>
</tr>
</table>
