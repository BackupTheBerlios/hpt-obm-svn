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

$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
//load contact management class
require($GO_MODULES->path."classes/addressbook.class.inc");
$ab = new addressbook();

$ab_settings = $ab->get_settings($GO_SECURITY->user_id);

if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
	switch($post_action)
	{
		case 'delete_addressbook':
			$delete_ab = $ab->get_addressbook($_POST['delete_addressbook_id']);

			if($GO_SECURITY->has_permission($GO_SECURITY->user_id, $delete_ab['acl_write']))
			{
				$default_id = $ab->get_default_addressbook($GO_SECURITY->user_id);
				if ($ab->delete_addressbook($_POST['delete_addressbook_id']))
				{
					$GO_SECURITY->delete_acl($delete_ab['acl_write']);
					$GO_SECURITY->delete_acl($delete_ab['acl_read']);
				}

				$ab->get_subscribed_addressbooks($GO_SECURITY->user_id);
				if ($ab->next_record())
				{
					$next_id = $ab->f('id');
					if ($_POST['delete_addressbook_id'] == $default_id)
					{
						$ab->set_default_addressbook($GO_SECURITY->user_id, $next_id);
					}
					if ($_POST['addressbook_id'] = $_POST['delete_addressbook_id'])
					{
						$_POST['addressbook_id'] = $next_id;
					}
				}else
				{
					unset($addressbook_id);
				}

			}
			$post_action = 'addressbooks';
		break;

		case 'subscribe':
			$ab->unsubscribe_all($GO_SECURITY->user_id);
			if(isset($_POST['subscribed']))
			{
				for ($i=0;$i<sizeof($_POST['subscribed']);$i++)
				{
					$ab->subscribe($GO_SECURITY->user_id, $_POST['subscribed'][$i]);
				}
			}

			if (!$ab->is_subscribed($GO_SECURITY->user_id, $_POST['default_addressbook_id']))
			{
				$ab->subscribe($GO_SECURITY->user_id, $_POST['default_addressbook_id']);
			}
			$ab->set_default_addressbook($GO_SECURITY->user_id, $_POST['default_addressbook_id']);
			$post_action = 'addressbooks';
		break;

	}
}

$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : $ab->get_default_addressbook($GO_SECURITY->user_id);
$is_treeview = isset($_REQUEST['treeview']) ? $_REQUEST['treeview'] : '0';

if (!$addressbook_id)
{
	$ab_name = $_SESSION['GO_SESSION']['name'];
	$new_ab_name = $ab_name;
	$x = 1;
	while($ab->get_addressbook_by_name($new_ab_name))
	{
		$new_ab_name = $ab_name.' ('.$x.')';
		$x++;
	}
	$addressbook_id = $ab->add_addressbook($GO_SECURITY->user_id, addslashes($new_ab_name));
}

$addressbook = $ab->get_addressbook($addressbook_id);

$page_title = $lang_modules['contacts'];
require($GO_THEME->theme_path."header.inc");
?>
<table border="0" cellspacing="0" cellpadding="0">
<tr>
	<td class="ModuleIcons">
	<a class="small" href="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=search&addressbook_id=<?php echo $addressbook_id; ?>"><img src="<?php echo $GO_THEME->images['ab_search']; ?>" border="0" height="32" width="32" /><br /><?php echo $contacts_search; ?></a></td>
	</td>
	<td class="ModuleIcons">
	<a class="small" href="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=browse&addressbook_id=<?php echo $addressbook_id; ?>"><img src="<?php echo $GO_THEME->images['ab_browse']; ?>" border="0" height="32" width="32" /><br /><?php echo $contacts_contacts; ?></a></td>
	</td>
	<td class="ModuleIcons">
	<a class="small" href="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=companies&addressbook_id=<?php echo $addressbook_id; ?>"><img src="<?php echo $GO_THEME->images['ab_companies']; ?>" border="0" height="32" width="32" /><br /><?php echo $ab_companies; ?></a></td>
	</td>
	<td class="ModuleIcons">
	<a class="small" href="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=members&addressbook_id=<?php echo $addressbook_id; ?>"><img src="<?php echo $GO_THEME->images['users']; ?>" border="0" height="32" width="32" /><br /><?php echo $contacts_members; ?></a></td>
	</td>
	<td class="ModuleIcons">
	<a class="small" href="contact.php?addressbook_id=<?php echo $addressbook_id; ?>&return_to=<?php echo urlencode($link_back); ?>"><img src="<?php echo $GO_THEME->images['add_contact']; ?>" border="0" height="32" width="32" /><br /><?php echo $ab_new_contact; ?></a></td>
	</td>
	<td class="ModuleIcons">
	<a class="small" href="company.php?addressbook_id=<?php echo $addressbook_id; ?>&return_to=<?php echo urlencode($link_back); ?>"><img src="<?php echo $GO_THEME->images['ab_add_company']; ?>" border="0" height="32" width="32" /><br /><?php echo $ab_new_company; ?></a></td>
	</td>
	<td class="ModuleIcons">
	<a class="small" href="addressbooks.php?return_to=<?php echo urlencode($link_back); ?>"><img src="<?php echo $GO_THEME->images['ab_addressbooks']; ?>" border="0" height="32" width="32" /><br /><?php echo $ab_addressbooks; ?></a></td>
	</td>
	<?php
	$tp_plugin = $GO_MODULES->get_plugin('templates');
	if ($tp_plugin )
	{
		echo '<td class="ModuleIcons">';
		echo '<a class="small" href="'.$tp_plugin['url'].'"><img src="'.$GO_THEME->images['new_slide'].'" border="0" height="32" width="32" /><br />'.$ab_templates.'</a></td>';

		echo '<td class="ModuleIcons">';
		echo '<a class="small" href="'.$tp_plugin['url'].'mailings.php"><img src="'.$GO_THEME->images['mailings'].'" border="0" height="32" width="32" /><br />'.$ab_mailings.'</a></td>';

	}
	$custom_fields_plugin = $GO_MODULES->get_plugin('custom_fields');
	if ($custom_fields_plugin && $GO_MODULES->write_permissions)
	{
		echo '<td class="ModuleIcons">';
		echo '<a class="small" href="custom_fields/"><img src="'.$GO_THEME->images['ab_custom_fields'].'" border="0" height="32" width="32" /><br />'.$ab_custom_fields.'</a></td>';
	}
	if ($post_action != 'members' && $post_action != 'addressbooks')
	{
		echo '<td class="ModuleIcons">';
		echo '<a class="small" href="javascript:confirm_delete()"><img src="'.$GO_THEME->images['delete_big'].'" border="0" height="32" width="32" /><br />'.$contacts_delete.'</a></td>';
	}

	?>
</tr>
</table>
<form name="contacts" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=<?php echo $post_action; ?>&addressbook_id=<?php echo $addressbook_id; ?>">
<?php
switch($post_action)
{
	case 'members':
		require('members.inc');
	break;

	case 'browse':
		require("contacts.inc");
	break;

	case 'companies':
		if ($is_treeview)
			require('treeview.inc');
		else
			require('companies.inc');
	break;

	default:
		require('search.inc');
	break;
}
?>
</form>
<?php require($GO_THEME->theme_path."footer.inc"); ?>
