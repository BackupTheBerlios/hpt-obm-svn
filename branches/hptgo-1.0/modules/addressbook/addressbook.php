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

//load contact management class
require($GO_MODULES->path."classes/addressbook.class.inc");
$ab = new addressbook();

$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : 0;

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

switch($task)
{
	case 'copy_read_acl':
		if($addressbook = $ab->get_addressbook($addressbook_id))
		{
			if ($ab->get_user_contacts($GO_SECURITY->user_id, $addressbook_id) > 0)
			{
				while($ab->next_record())
				{
					$GO_SECURITY->copy_acl($addressbook['acl_read'], $ab->f('acl_read'));
				}
			}
			if ($ab->get_user_companies($GO_SECURITY->user_id, $addressbook_id) > 0)
			{
				while($ab->next_record())
				{
					$GO_SECURITY->copy_acl($addressbook['acl_read'], $ab->f('acl_read'));
				}
			}
		}
	break;

	case 'copy_write_acl':
		if($addressbook = $ab->get_addressbook($addressbook_id))
		{
			if ($ab->get_user_contacts($GO_SECURITY->user_id, $addressbook_id) > 0)
			{
				while($ab->next_record())
				{
					$GO_SECURITY->copy_acl($addressbook['acl_write'], $ab->f('acl_write'));
				}
			}

			if ($ab->get_user_companies($GO_SECURITY->user_id, $addressbook_id) > 0)
			{
				while($ab->next_record())
				{
					$GO_SECURITY->copy_acl($addressbook['acl_write'], $ab->f('acl_write'));
				}
			}
		}
	break;

	case 'save':
		$name = smart_addslashes(trim($_POST['name']));
		if ($name == '')
		{
			$feedback = '<p class="Error">'.$error_missing_field.'</p>';
		}else
		{
			if ($addressbook_id > 0)
			{
				$existing_addressbook = $ab->get_addressbook_by_name($name);
				if($existing_addressbook && $existing_addressbook['id'] != $addressbook_id)
				{
					$feedback = '<p class="Error">'.$ab_addressbook_exists.'</p>';
				}elseif(!$ab->update_addressbook($_POST['addressbook_id'], $name))
				{
					$feedback = '<p class="Error">'.$strSaveError.'</p>';
				}elseif($_POST['close'] == 'true')
				{
					header('Location: '.$return_to);
					exit();
				}
			}else
			{
				if($ab->get_addressbook_by_name($name))
				{
					$feedback = '<p class="Error">'.$ab_addressbook_exists.'</p>';
				}elseif(!$addressbook_id = $ab->add_addressbook($GO_SECURITY->user_id, $name))
				{
					$feedback = '<p class="Error">'.$strSaveError.'</p>';
				}elseif($_POST['close'] == 'true')
				{
					header('Location: '.$return_to);
					exit();
				}
			}
		}
	break;

	case 'export':
		$addressbook = $ab->get_addressbook($addressbook_id);
		require($GO_CONFIG->class_path."/phpvnconv/phpvnconv.class.inc");
		$vnconv = new phpVnconv();

		$vnconv->set_to("ascii");
		$browser = detect_browser();
		header("Content-type: text/x-csv");
		header('Expires: '.gmdate('D, d M Y H:i:s') . ' GMT');
		if ($browser['name'] == 'MSIE')
		{
			header("Content-Disposition: inline; filename=\"".$vnconv->vnconv($addressbook['name'])."-".$_POST['export_type'].".csv\"");
			header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
			header('Pragma: public');
		}else
		{
			header('Pragma: no-cache');
			header("Content-Disposition: attachment; filename=\"".$vnconv->vnconv($addressbook['name']).".csv\"");
		}

		$vnconv->set_to($_POST['encoding'] == "none" || $_POST['encoding'] == "utf16" ? '' : $_POST['encoding']);
		$utf16 = $_POST['encoding'] == 'utf16';
		$quote = smartstrip($_POST['quote']);
		$crlf = smartstrip($_POST['crlf']);
		$crlf = str_replace('\\r', "\015", $crlf);
		$crlf = str_replace('\\n', "\012", $crlf);
		$crlf = str_replace('\\t', "\011", $crlf);
		switch ($_POST['seperator']) {
			case 'comma': $seperator = ','; break;
			case 'semicolon': $seperator = ';'; break;
			case 'colon': $seperator = ':'; break;
			case 'tab': $seperator = "\t"; break;
			default: $seperator = '';
		}
		if ($utf16) // Windows hack
			echo "\xFF\xFE";

		if ($_POST['export_type'] == 'contacts')
		{
			$headings = array($strTitle, $strFirstName, $strMiddleName, $strLastName, $strInitials, $strSex, $strBirthday, $strEmail, $strCountry, $strState, $strCity, $strZip, $strAddress, $strPhone, $strWorkphone, $strFax, $strWorkFax, $strCellular, $strCompany, $strDepartment, $strFunction, $ab_comment, $contacts_group);
			$headings = $quote.implode($quote.$seperator.$quote, $headings).$quote;
			echo $utf16 ? mb_convert_encoding($headings,"UTF-16LE","UTF-8") : $vnconv->VnConv($headings);
			echo $utf16 ? mb_convert_encoding($crlf,"UTF-16LE","UTF-8") : $crlf;

			$ab->get_contacts_for_export($_POST['addressbook_id']);
			while ($ab->next_record())
			{
				$record = array($ab->f("title"), $ab->f("first_name"),$ab->f("middle_name"), $ab->f("last_name"), $ab->f("initials"), $ab->f("sex"), $ab->f('birthday'), $ab->f("email"), $ab->f("country"), $ab->f("state"), $ab->f("city"), $ab->f("zip"), $ab->f("address"), $ab->f("home_phone"), $ab->f("work_phone"), $ab->f("fax"), $ab->f("work_fax"), $ab->f("cellular"), $ab->f("company"), $ab->f("department"), $ab->f("function"), $ab->f("comment"), $ab->f("group_name"));
				$record = $quote.implode($quote.$seperator.$quote, $record).$quote;
				echo $utf16 ? mb_convert_encoding($record,"UTF-16LE","UTF-8") : $vnconv->VnConv($record);
				echo $utf16 ? mb_convert_encoding($crlf,"UTF-16LE","UTF-8") : $crlf;
			}
		}else
		{
			$headings = array($strName, $strCountry, $strState, $strCity, $strZip, $strAddress, $strEmail, $strPhone, $strFax, $strHomepage, $ab_bank_no, $ab_vat_no);
			$headings = $quote.implode($quote.$seperator.$quote, $headings).$quote;
			echo $utf16 ? mb_convert_encoding($headings,"UTF-16LE","UTF-8") : $vnconv->VnConv($headings);
			echo $utf16 ? mb_convert_encoding($crlf,"UTF-16LE","UTF-8") : $crlf;

			$ab->get_companies($_POST['addressbook_id']);

			while($ab->next_record())
			{
				$record = array($ab->f("name"), $ab->f("country"), $ab->f("state"), $ab->f("city"), $ab->f("zip"), $ab->f("address"), $ab->f("email"), $ab->f("phone"), $ab->f("fax"), $ab->f("homepage"), $ab->f("bank_no"), $ab->f('vat_no'));
				$record = $quote.implode($quote.$seperator.$quote, $record).$quote;
				echo $utf16 ? mb_convert_encoding($record,"UTF-16LE","UTF-8") : $vnconv->VnConv($record);
				echo $utf16 ? mb_convert_encoding($crlf,"UTF-16LE","UTF-8") : $crlf;
			}
		}
		exit();
	break;
}

if ($addressbook_id > 0 && $addressbook = $ab->get_addressbook($addressbook_id))
{
	if (!$write_permission = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $addressbook['acl_write']))
	{
		$read_permission = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $addressbook['acl_read']);
	}
	$name = isset($name) ? $name : $addressbook['name'];

	$tabtable = new tabtable('addressbook', $name, '460', '400', '120', '', true);
	$tabtable->add_tab('name', $strProperties);
	if ($write_permission)
	{
		$tabtable->add_tab('groups', $contacts_groups);
		$tabtable->add_tab('import', $contacts_import);
	}
	$tabtable->add_tab('export', $contacts_export);
	$tabtable->add_tab('read_permissions', $strReadRights);
	$tabtable->add_tab('write_permissions', $strWriteRights);
}else
{
	$tabtable = new tabtable('addressbook', $ab_new_ab, '460', '400', '120', '', true);
	$write_permission = true;
}

if (!$write_permission && !$read_permission)
{
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}

if (isset($_REQUEST['active_tab']))
{
	$tabtable->set_active_tab($_REQUEST['active_tab']);
}
$link_back .= '&active_tab='.$tabtable->active_tab;

require($GO_THEME->theme_path."header.inc");

echo '<form name="addressbook" method="post" action="'.$_SERVER['PHP_SELF'].'" enctype="multipart/form-data">';
echo '<input type="hidden" name="task" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="addressbook_id" value="'.$addressbook_id.'" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

$tabtable->print_head();

echo '<br />';
switch($tabtable->get_active_tab_id())
{
	case 'read_permissions':
		if ($addressbook['user_id'] == $GO_SECURITY->user_id)
		{
			echo '<a class="normal" href="javascript:copy_acl(\'copy_read_acl\');">'.$ab_copy_read_acl.'</a><br />';
		}
		print_acl($addressbook["acl_read"]);
		echo '<br />';
    		echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
		$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	break;

	case 'write_permissions':
		if ($addressbook['user_id'] == $GO_SECURITY->user_id)
		{
			echo '<a class="normal" href="javascript:copy_acl(\'copy_write_acl\');">'.$ab_copy_write_acl.'</a><br />';
		}
		print_acl($addressbook["acl_write"]);
		echo '<br />';
    		echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
		$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	break;

	case 'groups':
		require('groups.inc');
		echo '<br />';
		$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	break;

	case 'import':
		require('import.inc');
	break;

	case 'export':
		require('export.inc');
	break;

	default:
		$name = isset($name) ? htmlspecialchars(stripslashes($name)) : '';
		echo '<table border="0">';
		if (isset($feedback))
		{
			echo '<tr><td colspan="2" class="Error">'.$feedback.'</td></tr>';
		}
		if ($write_permission)
			echo '<tr><td>'.$strName.':</td><td><input type="text" class="textbox" name="name" value="'.$name.'" size="40" /></td></tr></table>';
		else
			echo '<tr><td>'.$strName.':</td><td>'.htmlspecialchars($name).'</td></tr></table>';
		echo '<br />';
		if ($write_permission)
		{
			$button = new button($cmdOk, "javascript:ok_addressbook()");
			echo '&nbsp;&nbsp;';
			$button = new button($cmdApply, "javascript:apply_addressbook()");
			echo '&nbsp;&nbsp;';
		}
		$button = new button($cmdClose, "javascript:document.location='".$return_to."';");

		echo'<script type="text/javascript">document.forms[0].name.focus();</script>';
	break;
}

$tabtable->print_foot();

echo '</form>';
?>
<script type="text/javascript" language="javascript">

function ok_addressbook()
{
	document.forms[0].close.value = 'true';
	document.forms[0].task.value = 'save';
	document.forms[0].submit();
}
function apply_addressbook()
{
	document.forms[0].task.value = 'save';
	document.forms[0].submit();
}

function copy_acl(task)
{
	document.forms[0].task.value = task;
	document.forms[0].submit();
}
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
