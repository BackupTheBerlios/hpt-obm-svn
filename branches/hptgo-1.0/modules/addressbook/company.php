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
$GO_MODULES->authenticate('addressbook');
require($GO_LANGUAGE->get_language_file('addressbook'));

$page_title=$contact_profile;
require($GO_MODULES->class_path."addressbook.class.inc");
$ab = new addressbook();

$custom_fields_plugin = $GO_MODULES->get_plugin('custom_fields');

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : null;
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$company_id = isset($_REQUEST['company_id']) ? $_REQUEST['company_id'] : '0';

$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : '0';

//remember sorting of the projects list in a cookie
if (isset($_REQUEST['new_sort_field']))
{
	SetCookie("contact_sort",$_REQUEST['new_sort_field'],time()+3600*24*365,"/","",0);
	$_COOKIE['contact_sort'] = $_REQUEST['new_sort_field'];
}

if (isset($_REQUEST['new_sort_direction']))
{
	SetCookie("contact_direction",$_REQUEST['new_sort_direction'],time()+3600*24*365,"/","",0);
	$_COOKIE['contact_direction'] = $_REQUEST['new_sort_direction'];
}

	//get all writable user addressbooks and add them to a dropdownbox
	$ab->get_subscribed_addressbooks($GO_SECURITY->user_id);
	$subscribed_addressbooks = new dropbox();
	while ($ab->next_record())
	{
		if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_write')))
		{
			//remember the first ab that is writable
			if(!isset($first_writable_ab))
			{
				$first_writable_ab = $ab->f('id');
			}
			$subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));

		}
	}

	//get the given addressbook_id
	if ($addressbook_id > 0)
	{
		 $addressbook = $ab->get_addressbook($addressbook_id);
	}

	//if there was no or a read only addressbook given then change to the first writable
	if (!isset($addressbook) || !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $addressbook['acl_write']))
	{
		//there is no writable addressbook so add one
		if (!isset($first_writable_ab))
		{
			$ab_name = $_SESSION['GO_SESSION']['name'];
			$new_ab_name = $ab_name;
			$x = 1;
			while($ab->get_addressbook_by_name($new_ab_name))
			{
				$new_ab_name = $ab_name.' ('.$x.')';
				$x++;
			}
			$subscribed_addressbook_id = $ab->add_addressbook($GO_SECURITY->user_id, $new_ab_name);
			$subscribed_addressbooks->add_value($subscribed_addressbook_id, $new_ab_name);
		}
	}

	if ($addressbook_id > 0)
		$subscribed_addressbook_id = $addressbook_id;
	$old_subscribed_addressbook_id = $subscribed_addressbook_id;
	if (isset($first_writable_ab) && (!isset($subscribed_addressbook_id) || $subscribed_addressbook_id == 0))
		$subscribed_addressbook_id = $first_writable_ab;

	$datepicker = new date_picker();
	$GO_HEADER['head'] = $datepicker->get_header();
//save
switch($task)
{
	case 'save_company':
		$name = trim(smart_addslashes($_POST['name']));
		$shortname = trim(smart_addslashes($_POST['shortname']));
		$engname = trim(smart_addslashes($_POST['engname']));
		$relation_date = smart_addslashes($_POST["relation_date"]);
		$address = smart_addslashes($_POST["address"]);
		$zip = smart_addslashes($_POST["zip"]);
		$city = smart_addslashes($_POST["city"]);
		$state = smart_addslashes($_POST["state"]);
		$country = smart_addslashes($_POST["country"]);
		$email = smart_addslashes($_POST["email"]);
		$phone = smart_addslashes($_POST["phone"]);
		$fax = smart_addslashes($_POST["fax"]);
		$homepage = smart_addslashes($_POST["homepage"]);
		$bank_no = smart_addslashes($_POST["bank_no"]);
		$vat_no = smart_addslashes($_POST["vat_no"]);
		$parent_id = $_POST['parent_id'];
		
		$relation_date = date_to_db_date($_POST['relation_date']);
		if ($name == '')
		{
			$feedback = "<p class=\"Error\">".$error_missing_field."</p>";
		}else
		{
			if ($_POST['company_id'] > 0)
			{
				$seen = array();
				$seen[$_POST['company_id']] = 1;
				$parent = $parent_id;
				while ($parent != 0 && !isset($seen[$parent]))
				{
					$seen[$parent] = 1;
					$sql = "SELECT parent FROM ab_companies WHERE id='$parent'";
					if ($ab->query($sql) && $ab->next_record())
						$parent = $ab->f('parent');
				}
				if ($parent != 0)
					$feedback = "<p class=\"Error\">".$strParentCompanyError."</p>";
				else
				{
					if ($ab->update_company($_POST['company_id'], $addressbook_id, $name, $shortname, $engname,
									$relation_date, $address, $zip, $city, $state, $country, $email, $phone, $fax,
									$homepage, $bank_no, $vat_no, $parent_id))
					{
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
			}else
			{
				$acl_read = $GO_SECURITY->get_new_acl('company read');
				$acl_write = $GO_SECURITY->get_new_acl('company write');

				if ($company_id = $ab->add_company($addressbook_id, 
							$GO_SECURITY->user_id, $name, $shortname, $engname, $relation_date,
							$address, $zip, $city, $state, $country, $email,
							$phone, $fax, $homepage, $bank_no, $vat_no,
							$acl_read, $acl_write,$parent_id))
				{
					if($addressbook = $ab->get_addressbook($addressbook_id))
					{
						$GO_SECURITY->copy_acl($addressbook['acl_read'], $acl_read);
						$GO_SECURITY->copy_acl($addressbook['acl_write'], $acl_write);
					}

					/*
					if ($_POST['close'] == 'true')
					{
						header('Location: '.$return_to);
						exit();
					}
					*/
				}else
				{
					$GO_SECURITY->delete_acl($acl_read);
					$GO_SECURITY->delete_acl($acl_write);
					$feedback = "<p class=\"Error\">".$strSaveError."</p>";
				}
			}
		}
	break;

	case 'save_custom_fields':
	if (isset($_POST['fields']))
	{
		require_once($custom_fields_plugin['class_path'].'custom_fields.class.inc');
		$cf = new custom_fields('ab_custom_company_fields');

		$cf->update_record($company_id, $_POST['fields'], $_POST['values']);
		if ($_POST['close'] == 'true')
		{
			header('Location: '.$return_to);
			exit();
		}
	}
	break;
}

//check permissions
if ($company_id > 0 && $company = $ab->get_company($company_id))
{
	$myab = new addressbook();
	$myab->query("SELECT * FROM ab_addressbooks WHERE id='".$ab->f('addressbook_id')."'");
	$myab->next_record();

	$tabtable= new tabtable('company_table', $company['name'], '100%', '400',
						'120', '', true, 'left', 'top', 'company_form', 'vertical',$myab->f('name'));

	
	$tabtable->add_tab('profile', $ab_company_properties);
	
	if ($custom_fields_plugin)
	{
		require_once($custom_fields_plugin['path'].'classes/custom_fields.class.inc');
		$cf = new custom_fields('ab_custom_company_fields');

		if ($cf->get_catagories($GO_SECURITY->user_id) > 0)
		{
			while($cf->next_record())
			{
				$tabtable->add_tab($cf->f('id'), $cf->f('name'));
			}
		}
	}
//------------------------	
	$tabtable->add_tab('categories', $ab_category);
//----------------	
	$tabtable->add_tab('contacts', $ab_employees);
	$tabtable->add_tab('read_permissions', $strReadRights);
	$tabtable->add_tab('write_permissions', $strWriteRights);

	$addressbook_id = $company['addressbook_id'];
	if (!$write_permission =
		$GO_SECURITY->has_permission($GO_SECURITY->user_id, $company['acl_write']))
	{
		$read_permission =
			$GO_SECURITY->has_permission($GO_SECURITY->user_id, $company['acl_read']);
	}

}else
{
	$tabtable= new tabtable('company_table', $ab_new_company, '100%', '400',
									'120', '', true, 'left', 'top', 'company_form');
	$write_permission = true;
	$read_permission = true;
}

if (!$write_permission && !$read_permission)
{
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}

require($GO_THEME->theme_path."header.inc");

$active_tab = isset($_REQUEST['active_tab']) ? $_REQUEST['active_tab'] : null;
	
if (isset($active_tab))
{
	$tabtable->set_active_tab($active_tab);
}

if ($tabtable->get_active_tab_id() == 'contacts')
{
	$ab->enable_contact_selector();
}

echo '<form method="post" name="company_form" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
echo '<input type="hidden" name="company_id" value="'.$company_id.'" />';

if ($company_id == 0 || $task == 'save_company')
{
	$company['name'] = isset($_REQUEST['name']) ? smartstrip($_REQUEST['name']) : '';
	$company['shortname'] = isset($_REQUEST['shortname']) ? smartstrip($_REQUEST['shortname']) : '';
	$company['engname'] = isset($_REQUEST['engname']) ? smartstrip($_REQUEST['engname']) : '';
	$company['relation_date'] = isset($_REQUEST['relation_date']) ? smartstrip($_REQUEST['relation_date']) : '0';
	$company['parent_id'] = isset($_REQUEST['parent_id']) ? smartstrip($_REQUEST['parent_id']) : '0';
	$company['address'] = isset($_REQUEST['address']) ? smartstrip($_REQUEST['address']) : '';
	$company['zip'] = isset($_REQUEST['zip']) ? smartstrip($_REQUEST['zip']) : '';
	$company['city'] = isset($_REQUEST['city']) ? smartstrip($_REQUEST['city']) : '';
	$company['state'] = isset($_REQUEST['state']) ? smartstrip($_REQUEST['state']) : '';
	$company['email'] = isset($_REQUEST['email']) ? smartstrip($_REQUEST['email']) : '';
	$company['country'] = isset($_REQUEST['country']) ? smartstrip($_REQUEST['country']) : '';
	$company['phone'] = isset($_REQUEST['phone']) ? smartstrip($_REQUEST['phone']) : '';
	$company['fax'] = isset($_REQUEST['fax']) ? smartstrip($_REQUEST['fax']) : '';
	$company['homepage'] = isset($_REQUEST['homepage']) ? smartstrip($_REQUEST['homepage']) : 'http://';
	$company['bank_no'] = isset($_REQUEST['bank_no']) ? smartstrip($_REQUEST['bank_no']) : '';
	$company['vat_no'] = isset($_REQUEST['vat_no']) ? smartstrip($_REQUEST['vat_no']) : '';

}

	$addressbook_id = isset($old_subscribed_addressbook_id) && $old_subscribed_addressbook_id > 0 ? $subscribed_addressbook_id : $company['addressbook_id'];
	$subscribed_addressbook_id = $addressbook_id;
	$cp = new addressbook();
	$parent_dropbox = new dropbox();
	$company['parent_id'] = 0;
	if ($company_id)
	{
		$cp->get_company($company_id);
		$company['parent_id'] = $cp->f('parent');
	}
	$count = $cp->get_companies($addressbook_id);
	$parent_dropbox->add_value(0,$strNone);
	if ($count)
	while ($cp->next_record())
	{
		$id = $cp->f('id');
		if ($id == $company_id)
			continue;
		$name = $cp->f('name');
		$parent_dropbox->add_value($id,$name);
	}


$tabtable->print_head();
if ($tabtable->get_active_tab_id() > 0)
{
	$catagory_id = $tabtable->get_active_tab_id();
	$active_tab_id = 'custom_fields';
}else
{
	$active_tab_id =$tabtable->get_active_tab_id();
}

switch($active_tab_id)
{
	case 'read_permissions':
		print_acl($company['acl_read']);
		echo '<br />';
    		echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
		if (isset($return_to))
		{
			$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
		}
	break;

	case 'write_permissions':
		print_acl($company['acl_write']);
		echo '<br />';
    		echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
		if (isset($return_to))
		{
			$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
		}
	break;
	
	case 'categories':
		require('company_categories.php');
	break;

	case 'custom_fields':
		require('custom_fields/custom_fields.inc');
	break;

	case 'contacts':
		require('company_contacts.inc');
	break;

	default:
		require('../../lib/tkdlib.php');
		if ($write_permission)
		{
			require('edit_company.inc');
		}else
		{
			require('show_company.inc');
		}
	break;
}

$tabtable->print_foot();

echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
