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
$post_action = isset($post_action) ? $post_action : '';

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('addressbook');
require($GO_LANGUAGE->get_language_file('addressbook'));

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : '0';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];;

//load contact management class
require($GO_MODULES->path."classes/addressbook.class.inc");
$ab = new addressbook();

require($GO_THEME->theme_path."header.inc");
?>
<script type="text/javascript" langugae="javascript">
<!--

function import_data()
{
	document.forms[0].task.value='import';
	document.forms[0].submit();
}

-->
</script>

<form name="import" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="task" value="import" />
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>" />

<?php
$tabtable = new tabtable('export_tab', $contacts_import, '460', '400', '120', '', true);
$tabtable->print_head();
echo '<table border="0" cellpadding="5"><tr><td>';
require_once($GO_CONFIG->class_path."/phpvnconv/phpvnconv.class.inc");
$vnconv = new phpVnconv();
$vnconv->set_to('utf8');
$vnconv->set_from($_POST['encoding'] == "none" ? '' : $_POST['encoding']);

if ($task == 'import')
{

	$contact_groups[''] = 0;
	$group_mode = isset($_POST['group_mode']) ? $_POST['group_mode'] : 'group_name';
	$group_id = isset($_POST['group_id']) ? $_POST['group_id'] : 'group_id';
	if ($group_mode == 'file')
	{
		$ab->get_groups($_POST['addressbook_id']);
		while ($ab->next_record())
		{
			$contact_groups[$ab->f('name')] = $ab->f('id');
		}
	}

	switch($_POST['file_type'])
	{
		case 'vcf':
			require_once($GO_MODULES->path."classes/vcard.class.inc");
			$vcard = new vcard();
			$success = $vcard->import($_POST['import_file'], $GO_SECURITY->user_id, $_POST['addressbook_id'],$vnconv);

			unlink($_POST['import_file']);
			if($success)
			{
				echo  $contacts_import_success;
			}else
			{
				echo $ab_import_failed;
			}
			echo '<br /><br />';
			$button = new button($cmdOk, "javascript:document.location='".$return_to."'");
			break;
		case 'csv':
			$seperator = isset($_POST['seperator']) ? $_POST['seperator'] : ';';

			$fp = fopen($_POST['import_file'], "r");

			if (!$fp || !$addressbook = $ab->get_addressbook($_POST['addressbook_id']))
			{
				unlink($_POST['import_file']);
				$feedback = "<p class=\"Error\">".$strDataError."</p>";

			}else
			{
				fgets($fp, 4096);
				while (!feof($fp))
				{
					$record = fgetcsv($fp, 4096, ',', '"');

					foreach ($record as $i => $j) {
					  $record[$i] = $vnconv->vnconv($record[$i]);
					}


					if ($_POST['import_type'] == 'contacts')
					{
					if((isset($record[$_POST['first_name']]) && $record[$_POST['first_name']] != "") || (isset($record[$_POST['last_name']]) && $record[$_POST['last_name']] != ''))
						{
							if ($group_mode == 'file')
							{
								$group_name = trim($record[$_POST['group_record']]);
								if (isset($contact_groups[stripslashes($group_name)]))
								{
									$group_id = $contact_groups[$group_name];
								}else
								{
									$ab2= new addressbook();
									$group_id = $ab2->add_group($_POST['addressbook_id'], $group_name);
									$contact_groups[$group_name] = $group_id;
								}
							}
							$title = isset($record[$_POST['title']]) ? addslashes(trim($record[$_POST['title']])) : '';
							$first_name = isset($record[$_POST['first_name']]) ? addslashes(trim($record[$_POST['first_name']])) : '';
							$middle_name = isset($record[$_POST['middle_name']]) ? addslashes(trim($record[$_POST['middle_name']])) : '';
							$last_name = isset($record[$_POST['last_name']]) ? addslashes(trim($record[$_POST['last_name']])) : '';
							$initials = isset($record[$_POST['initials']]) ? addslashes(trim($record[$_POST['initials']])) : '';
							$sex = isset($record[$_POST['sex']]) ? addslashes(trim($record[$_POST['sex']])) : 'M';
							$birthday = isset($record[$_POST['birthday']]) ? addslashes(trim($record[$_POST['birthday']])) : '';
							$email = isset($record[$_POST['email']]) ? addslashes(trim($record[$_POST['email']])) : '';
							if(preg_match("/(\b)([\w\.\-]+)(@)([\w\.-]+)([A-Za-z]{2,4})\b/i", $email, $matches))
							{
								$email = $matches[0];
								
							}							
							
							$work_phone = isset($record[$_POST['work_phone']]) ? addslashes(trim($record[$_POST['work_phone']])) : '';
							$home_phone = isset($record[$_POST['home_phone']]) ? addslashes(trim($record[$_POST['home_phone']])) : '';
							$fax = isset($record[$_POST['fax']]) ? addslashes(trim($record[$_POST['fax']])) : '';
							$work_fax = isset($record[$_POST['work_fax']]) ? addslashes(trim($record[$_POST['work_fax']])) : '';
							$cellular = isset($record[$_POST['cellular']]) ? addslashes(trim($record[$_POST['cellular']])) : '';
							$country = isset($record[$_POST['country']]) ? addslashes(trim($record[$_POST['country']])) : '';
							$state = isset($record[$_POST['state']]) ? addslashes(trim($record[$_POST['state']])) : '';
							$city = isset($record[$_POST['city']]) ? addslashes(trim($record[$_POST['city']])) : '';
							$zip = isset($record[$_POST['zip']]) ? addslashes(trim($record[$_POST['zip']])) : '';
							$address = isset($record[$_POST['address']]) ? addslashes(trim($record[$_POST['address']])) : '';
							$company_name = isset($record[$_POST['company_name']]) ? addslashes(trim($record[$_POST['company_name']])) : '';
							$department = isset($record[$_POST['department']]) ? addslashes(trim($record[$_POST['department']])) : '';
							$function = isset($record[$_POST['function']]) ? addslashes(trim($record[$_POST['function']])) : '';

							$acl_read = $GO_SECURITY->get_new_acl('contact read');
							$acl_write = $GO_SECURITY->get_new_acl('contact write');

							if ($acl_read > 0 && $acl_write > 0)
							{
								if ($company_name != '')
								{
									$company_id = $ab->get_company_id_by_name($company_name, $_POST['addressbook_id']);
								}else
								{
									$company_id = 0;
								}
								if($ab->add_contact("", $GO_SECURITY->user_id, $_POST['addressbook_id'], $first_name, $middle_name, $last_name, $initials, $title, $sex, $birthday, $email, $work_phone, $home_phone, $fax, $cellular, $country, $state, $city, $zip, $address, $company_id, $work_fax, $department, $function,'', $group_id, '', $acl_read, $acl_write))
								{
									$GO_SECURITY->copy_acl($addressbook['acl_read'], $acl_read);
									$GO_SECURITY->copy_acl($addressbook['acl_write'], $acl_write);
								}else
								{
									$GO_SECURITY->delete_acl($acl_read);
									$GO_SECURITY->delete_acl($acl_write);
								}
							}
						}
					}else
					{
						if(isset($record[$_POST['name']]) && $record[$_POST['name']] != '')
						{
							$name = addslashes(trim($record[$_POST['name']]));
					
							if (!$ab->get_company_by_name($_POST['addressbook_id'], $name))
							{
								$email = isset($record[$_POST['email']]) ? addslashes(trim($record[$_POST['email']])) : '';
								if(preg_match("/(\b)([\w\.\-]+)(@)([\w\.-]+)([A-Za-z]{2,4})\b/i", $email, $matches))
								{
									$email = $matches[0];
									
								}
								
								$phone = isset($record[$_POST['phone']]) ? addslashes(trim($record[$_POST['phone']])) : '';
								$fax = isset($record[$_POST['fax']]) ? addslashes(trim($record[$_POST['fax']])) : '';
								$country = isset($record[$_POST['country']]) ? addslashes(trim($record[$_POST['country']])) : '';
								$state = isset($record[$_POST['state']]) ? addslashes(trim($record[$_POST['state']])) : '';
								$city = isset($record[$_POST['city']]) ? addslashes(trim($record[$_POST['city']])) : '';
								$zip = isset($record[$_POST['zip']]) ? addslashes(trim($record[$_POST['zip']])) : '';
								$address = isset($record[$_POST['address']]) ? addslashes(trim($record[$_POST['address']])) : '';
								$homepage = isset($record[$_POST['homepage']]) ? addslashes(trim($record[$_POST['homepage']])) : '';
								$bank_no = isset($record[$_POST['bank_no']]) ? addslashes(trim($record[$_POST['bank_no']])) : '';
								$vat_no = isset($record[$_POST['vat_no']]) ? addslashes(trim($record[$_POST['vat_no']])) : '';
	
								$acl_read = $GO_SECURITY->get_new_acl('contact read');
								$acl_write = $GO_SECURITY->get_new_acl('contact write');
	
								if ($acl_read > 0 && $acl_write > 0)
								{
									if ($ab->add_company($_POST['addressbook_id'],
											$GO_SECURITY->user_id, $name, $address, $zip, 
											$city, $state, $country, $email, $phone, $fax, 
											$homepage, $bank_no, $vat_no, $acl_read, $acl_write))
									{
										$GO_SECURITY->copy_acl($addressbook['acl_read'], $acl_read);
										$GO_SECURITY->copy_acl($addressbook['acl_write'], $acl_write);
									}else
									{
										$GO_SECURITY->delete_acl($acl_read);
										$GO_SECURITY->delete_acl($acl_write);
									}
								}
							}
						}
					}
				}
				fclose($fp);
				unlink($_POST['import_file']);
				echo  $contacts_import_success;
				echo '<br /><br />';
				$button = new button($cmdOk, "javascript:document.location='".$return_to."'");
			}
			break;
	}
}

if ($task == 'upload')
{

	$import_file = $GO_CONFIG->tmpdir.'contacts_import_'.$GO_SECURITY->user_id.'.'.$_POST['file_type'];

	if (isset($_FILES['import_file']) && is_uploaded_file($_FILES['import_file']['tmp_name']))
	{
		if(!copy($_FILES['import_file']['tmp_name'], $import_file))
		{
			unset($import_file);
			echo '<p class="Error">'.$fbNoFile.'</p>';
			echo '<br /><br />';
			$button = new button($cmdOk, "javascript:document.location='".$return_to."'");

		}
	}elseif(!file_exists($import_file))
	{
		unset($import_file);
		echo '<p class="Error">'.$fbNoFile.'</p>';
		echo '<br /><br />';
		$button = new button($cmdOk, "javascript:document.location='".$return_to."'");
	}

	if (isset($import_file))
	{
		echo '<input type="hidden" name="addressbook_id" value="'.$addressbook_id.'">';
		echo '<input type="hidden" value="'.$import_file.'" name="import_file" />';
		echo '<input type="hidden" value="'.$_POST['file_type'].'" name="file_type" />';
		
		switch($_POST['file_type'])
		{
			case 'vcf':
				echo '<table border="0" cellpadding="4" cellspacing="0">';
				echo '<tr><td><h3>Group-Office</h3></td>';
				echo '<td><h3>VCF</h3></td></tr>';
				echo '</table>';
				echo $ab_import_vcf_file_ok.'<br /><br />';
				echo '<table border="0" cellpadding="4" cellspacing="0">';
				echo '<tr><td colspan="2"><br />';
				$button = new button($cmdOk, 'javascript:import_data()');
				echo '&nbsp;&nbsp;';
				$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
				echo '</td></tr>';
				echo '</table>';
				$tabtable->print_foot();
				require($GO_THEME->theme_path.'footer.inc');
				exit();
				break;
			case 'csv':
				$fp = fopen($import_file, 'r');

				if ($fp)
				{					
					//when it's still not exploded then the file is not compatible.
					if (!$record = fgetcsv($fp, 4096, ',', '"'))
					{
						echo '<p class="Error">'.$contacts_import_incompatible.'</p>';
						$button = new button($cmdOk, "javascript:document.location='".$return_to."'");
					}else
					{
						fclose($fp);
						foreach ($record as $i => $j) {
						  $record[$i] = $vnconv->vnconv($record[$i]);
						}
						//echo '<input type="hidden" name="seperator" value="'.$seperator.'">';
						echo '<input type="hidden" name="import_type" value="'.$_POST['import_type'].'">';
						if (isset($feedback))
						{
							echo $feedback;
						}
						echo $contacts_import_feedback.'<br /><br />';

						if ($_POST['import_type'] == 'contacts')
						{
							echo '<table border="0" cellpadding="2" cellspacing="0">';
							$group_mode = isset($_POST['group_mode']) ? $_POST['group_mode'] : 'group_name';

							if ($ab->get_groups($addressbook_id) > 0)
							{
								$check = $group_mode == 'group_name' ? 'checked' : '';
								echo '<tr><td><input type="radio" name="group_mode" value="group_name" '.$check.' />'.$contacts_import_to_group.': </td><td>';
								$group_id = isset($_POST['group_id']) ? $_POST['group_id'] : 0;
								$dropbox = new dropbox();
								$dropbox->add_value('0',$contacts_other);
								$dropbox->add_sql_data('ab','id','name');
								$dropbox->print_dropbox('group_id',$group_id);
								echo '</td></tr>';
								$check = $group_mode == 'file' ? 'checked' : '';
								echo '<tr><td><input type="radio" name="group_mode" value="file" '.$check.'  />'.$contacts_auto_group.': </td><td>';
							}else
							{
								$check = $group_mode == 'file' ? true : false;
								echo '<tr><td><input type="hidden" name="group_id" value="0" />';
								$checkbox = new checkbox('group_mode', 'file', $ab_group_on_file, $check);
								echo ':</td><td>';
							}
							$dropbox = new dropbox();
							for ($n=0;$n<sizeof($record);$n++)
							{
								$dropbox->add_value($n,$record[$n]);
							}
							$group_record = isset($_POST['group_record']) ? $_POST['group_record'] : 0;
							$dropbox->print_dropbox('group_record', $group_record);
							echo '</td></tr></table><br />';

							$dropbox = new dropbox();
							$required_dropbox = new dropbox();
							$dropbox->add_value('-1',$strNotIncluded);
							for ($n=0;$n<sizeof($record);$n++)
							{
								$required_dropbox->add_value($n,$record[$n]);
								$dropbox->add_value($n,$record[$n]);
							}


							echo '<table border="0" cellpadding="4" cellspacing="0">';
							echo '<tr><td><h3>Group-Office</h3></td>';
							echo '<td><h3>CSV</h3></td></tr>';

							$title = isset($_POST['title']) ? $_POST['title'] : -1;
							echo '<tr><td>'.$strTitle.':</td><td>';
							$dropbox->print_dropbox('title', $title);
							echo '</td></tr>';

							$first_name = isset($_POST['first_name']) ? $_POST['first_name'] : -1;
							echo '<tr><td>'.$strFirstName.':</td><td>';
							$dropbox->print_dropbox('first_name', $first_name);
							echo '</td></tr>';

							$middle_name = isset($_POST['middle_name']) ? $_POST['middle_name'] : -1;
							echo '<tr><td>'.$strMiddleName.':</td><td>';
							$dropbox->print_dropbox('middle_name', $middle_name);
							echo '</td></tr>';

							$last_name = isset($_POST['last_name']) ? $_POST['last_name'] : 0;
							echo '<tr><td>'.$strLastName.':</td><td>';
							$required_dropbox->print_dropbox('last_name', $last_name);
							echo '</td></tr>';

							$initials = isset($_POST['initials']) ? $_POST['initials'] : -1;
							echo '<tr><td>'.$strInitials.':</td><td>';
							$dropbox->print_dropbox('initials', $initials);
							echo '</td></tr>';

							$sex = isset($_POST['sex']) ? $_POST['sex'] : -1;
							echo '<tr><td>'.$strSex.':</td><td>';
							$dropbox->print_dropbox('sex', $sex);
							echo '</td></tr>';

							$birthday = isset($_POST['birthday']) ? $_POST['birthday'] : -1;
							echo '<tr><td>'.$strBirthday.':</td><td>';
							$dropbox->print_dropbox('birthday', $birthday);
							echo '</td></tr>';

							$address = isset($_POST['address']) ? $_POST['address'] : -1;
							echo '<tr><td>'.$strAddress.':</td><td>';
							$dropbox->print_dropbox('address', $address);
							echo '</td></tr>';

							$zip = isset($_POST['zip']) ? $_POST['zip'] : -1;
							echo '<tr><td>'.$strZip.':</td><td>';
							$dropbox->print_dropbox('zip', $zip);
							echo '</td></tr>';

							$city = isset($_POST['city']) ? $_POST['city'] : -1;
							echo '<tr><td>'.$strCity.':</td><td>';
							$dropbox->print_dropbox('city', $city);
							echo '</td></tr>';

							$state = isset($_POST['state']) ? $_POST['state'] : -1;
							echo '<tr><td>'.$strState.':</td><td>';
							$dropbox->print_dropbox('state', $state);
							echo '</td></tr>';

							$country = isset($_POST['country']) ? $_POST['country'] : -1;
							echo '<tr><td>'.$strCountry.':</td><td>';
							$dropbox->print_dropbox('country', $country);
							echo '</td></tr>';

							$email = isset($_POST['email']) ? $_POST['email'] : -1;
							echo '<tr><td>'.$strEmail.':</td><td>';
							$dropbox->print_dropbox('email', $email);
							echo '</td></tr>';

							$home_phone = isset($_POST['home_phone']) ? $_POST['home_phone'] : -1;
							echo '<tr><td>'.$strPhone.':</td><td>';
							$dropbox->print_dropbox('home_phone', $home_phone);
							echo '</td></tr>';

							$fax = isset($_POST['fax']) ? $_POST['fax'] : -1;
							echo '<tr><td>'.$strFax.':</td><td>';
							$dropbox->print_dropbox('fax', $fax);
							echo '</td></tr>';

							$work_phone = isset($_POST['work_phone']) ? $_POST['work_phone'] : -1;
							echo '<tr><td>'.$strWorkphone.':</td><td>';
							$dropbox->print_dropbox('work_phone', $work_phone);
							echo '</td></tr>';

							$work_fax = isset($_POST['work_fax']) ? $_POST['work_fax'] : -1;
							echo '<tr><td>'.$strWorkFax.':</td><td>';
							$dropbox->print_dropbox('work_fax', $work_fax);
							echo '</td></tr>';

							$cellular = isset($_POST['cellular']) ? $_POST['cellular'] : -1;
							echo '<tr><td>'.$strCellular.':</td><td>';
							$dropbox->print_dropbox('cellular', $cellular);
							echo '</td></tr>';

							$company_name = isset($_POST['company_name']) ? $_POST['company_name'] : -1;
							echo '<tr><td>'.$strCompany.':</td><td>';
							$dropbox->print_dropbox('company_name', $company_name);
							echo '</td></tr>';

							$department = isset($_POST['department']) ? $_POST['department'] : -1;
							echo '<tr><td>'.$strDepartment.':</td><td>';
							$dropbox->print_dropbox('department', $department);
							echo '</td></tr>';

							$function = isset($_POST['function']) ? $_POST['function'] : -1;
							echo '<tr><td>'.$strFunction.':</td><td>';
							$dropbox->print_dropbox('function', $function);
							echo '</td></tr>';
							echo '</table>';
						}else
						{
							$dropbox = new dropbox();
							$required_dropbox = new dropbox();
							$dropbox->add_value('',$strNotIncluded);
							for ($n=0;$n<sizeof($record);$n++)
							{
								$dropbox->add_value($n,$record[$n]);
								$required_dropbox->add_value($n,$record[$n]);
							}
							echo '<table border="0" cellpadding="4" cellspacing="0">';
							echo '<tr><td><h3>Group-Office</h3></td>';
							echo '<td><h3>CSV</h3></td></tr>';
							$name = isset($_POST['name']) ? $_POST['name'] : 0;
							echo '<tr><td>'.$strName.':</td><td>';
							$required_dropbox->print_dropbox('name', $name);
							echo '</td></tr>';

							$address = isset($_POST['address']) ? $_POST['address'] : -1;
							echo '<tr><td>'.$strAddress.':</td><td>';
							$dropbox->print_dropbox('address', $address);
							echo '</td></tr>';

							$zip = isset($_POST['zip']) ? $_POST['zip'] : -1;
							echo '<tr><td>'.$strZip.':</td><td>';
							$dropbox->print_dropbox('zip', $zip);
							echo '</td></tr>';

							$city = isset($_POST['city']) ? $_POST['city'] : -1;
							echo '<tr><td>'.$strCity.':</td><td>';
							$dropbox->print_dropbox('city', $city);
							echo '</td></tr>';

							$state = isset($_POST['state']) ? $_POST['state'] : -1;
							echo '<tr><td>'.$strState.':</td><td>';
							$dropbox->print_dropbox('state', $state);
							echo '</td></tr>';

							$country = isset($_POST['country']) ? $_POST['country'] : -1;
							echo '<tr><td>'.$strCountry.':</td><td>';
							$dropbox->print_dropbox('country', $country);
							echo '</td></tr>';

							$email = isset($_POST['email']) ? $_POST['email'] : -1;
							echo '<tr><td>'.$strEmail.':</td><td>';
							$dropbox->print_dropbox('email', $email);
							echo '</td></tr>';

							$phone = isset($_POST['phone']) ? $_POST['phone'] : -1;
							echo '<tr><td>'.$strPhone.':</td><td>';
							$dropbox->print_dropbox('phone', $phone);
							echo '</td></tr>';

							$fax = isset($_POST['fax']) ? $_POST['fax'] : -1;
							echo '<tr><td>'.$strFax.':</td><td>';
							$dropbox->print_dropbox('fax', $fax);
							echo '</td></tr>';

							$homepage = isset($_POST['homepage']) ? $_POST['homepage'] :-1;
							echo '<tr><td>'.$strHomepage.':</td><td>';
							$dropbox->print_dropbox('homepage', $homepage);
							echo '</td></tr>';

							$bank_no = isset($_POST['bank_no']) ? $_POST['bank_no'] : -1;
							echo '<tr><td>'.$ab_bank_no.':</td><td>';
							$dropbox->print_dropbox('bank_no', $bank_no);
							echo '</td></tr>';

							$vat_no = isset($_POST['vat_no']) ? $_POST['vat_no'] : -1;
							echo '<tr><td>'.$ab_vat_no.':</td><td>';
							$dropbox->print_dropbox('vat_no', $vat_no);
							echo '</td></tr>';
							echo '</table>';
						}

						echo "<tr><td colspan=\"2\"><br />";
						$button = new button($cmdOk, 'javascript:import_data()');
						echo '&nbsp;&nbsp;';
						$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
						echo "</td></tr>";
						echo "</table>";
						echo "</td></tr>";
						echo "</table>";
						$tabtable->print_foot();
						echo "</td></tr>";
						echo "</table>";
						require($GO_THEME->theme_path.'footer.inc');
						exit();
					}
				}
				break;
		}
	}
}
echo '</td></tr></table>';
$tabtable->print_foot();
?>
</form>
<?php
require($GO_THEME->theme_path.'footer.inc');
?>
