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

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
	if (isset($_POST['user']) && $addressbook = $ab->get_addressbook($_POST['addressbook_id']))
	{
		for ($i=0;$i<sizeof($_POST['user']);$i++)
		{
			if (!$ab->user_is_in_addressbook($_POST['user'][$i], $_POST['addressbook_id']))
			{
				$profile = $GO_USERS->get_user($_POST['user'][$i]);

				$first_name = addslashes($profile["first_name"]);
				$middle_name = addslashes($profile["middle_name"]);
				$last_name = addslashes($profile["last_name"]);
				$initials = addslashes($profile["initials"]);
				$title = addslashes($profile["title"]);
				$birthday = addslashes($profile["birthday"]);
				$email = addslashes($profile["email"]);
				$work_phone = addslashes($profile["work_phone"]);
				$home_phone = addslashes($profile["home_phone"]);
				$fax = addslashes($profile["fax"]);
				$cellular = addslashes($profile["cellular"]);
				$country = addslashes($profile["country"]);
				$state = addslashes($profile["state"]);
				$city = addslashes($profile["city"]);
				$zip = addslashes($profile["zip"]);
				$address = addslashes($profile["address"]);
				$company = addslashes($profile["company"]);
				$department = addslashes($profile["department"]);
				$function = addslashes($profile["function"]);
				$work_fax = addslashes($profile["work_fax"]);
				$work_country = addslashes($profile["work_country"]);
				$work_state = addslashes($profile["work_state"]);
				$work_city = addslashes($profile["work_city"]);
				$work_zip = addslashes($profile["work_zip"]);
				$work_address = addslashes($profile["work_address"]);
				$homepage = addslashes($profile["homepage"]);

				$acl_read = $GO_SECURITY->get_new_acl('contact read');
				$acl_write = $GO_SECURITY->get_new_acl('contact write');
				if ($acl_read > 0 && $acl_write > 0)
				{
					$company_id = $ab->get_company_id_by_name($profile['company'], $_POST['addressbook_id']);

					if ($ab->add_contact($_POST['user'][$i], $GO_SECURITY->user_id,
						$_POST['addressbook_id'], $first_name, $middle_name, $last_name, 
						$initials, $title, $profile['sex'], $birthday, $email, $work_phone, 
						$home_phone, $fax, $cellular, $country, $state, $city, $zip, 
						$address, $company_id, $work_fax, $department, $function,'',
						$_POST['group'], '', $acl_read, $acl_write))
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
if(isset($_REQUEST['new_sort_order']) && $_REQUEST['new_sort_order'] != $ab_settings['sort_users_order'])
{
  $ab->set_users_sorting($GO_SECURITY->user_id, 
						  $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
  $ab_settings['sort_users_order'] = $_REQUEST['new_sort_order'];
  $ab_settings['sort_users_field'] = $_REQUEST['new_sort_field'];
}

if ($ab_settings['sort_users_order'] == "DESC")
{
	$image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
	$new_sort_order = "ASC";
}else
{
	$image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
	$new_sort_order = "DESC";
}

echo '<input type="hidden" name="post_action" value="'.$post_action.'" />';
echo '<table border="0" width="700"><tr>';
if ($ab->get_subscribed_addressbooks($GO_SECURITY->user_id) > 1)
{
	echo '<td nowrap>'.$ab_addressbook.':</td><td>';
	$subscribed_addressbooks = new dropbox();
	while ($ab->next_record())
	{
		$subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));
	}
	$subscribed_addressbooks->print_dropbox('addressbook_id', $addressbook_id, 'onchange="javascript:document.forms[0].submit()"');
	echo '</td>';
}else
{
	echo '<input type="hidden" name="addressbook_id" value="'.$addressbook_id.'" />';
}

echo '<td align="right" width="100%">';
$group_count = $ab->get_groups($addressbook_id);
if ($ab->get_groups($addressbook_id) > 0)
{
	$dropbox = new dropbox();
	$dropbox->add_value('',$contacts_add_to_group);
	$dropbox->add_value('0',$contacts_other);
	$dropbox->add_sql_data('ab','id','name');
	$dropbox->print_dropbox('group','','onchange="javascript:document.forms[0].submit()"');
}else
{
	echo '<a href="javascript:document.forms[0].submit()" class="normal">'.$contacts_add_to.'</a><br /><br />';
	echo '<input type="hidden" name="group" value="0" />';
}
echo '</td></tr></table>';


$count = $GO_USERS->get_authorized_users($GO_SECURITY->user_id, $ab_settings['sort_users_field'], $ab_settings['sort_users_order']);
echo "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"700\">\n";
echo "<tr>";
echo '<td class="TableHead2" width="16"><input type="checkbox" value="dummy" name="dummy" onclick="javascript:invert_selection()" /></td>';
echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?post_action=members&new_sort_field=name&new_sort_order=".$new_sort_order."\">".$strName;
if ($ab_settings['sort_users_field'] == "name")
        echo $image_string;
echo "</a></td>\n";

require('../../lib/tkdlib.php');
$order = print_config_title($constMembersPage);

for ($i=0; $i<count($order); $i++)
{
	echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?post_action=members&new_sort_field=company&new_sort_order=".$new_sort_order."\">".$strCom[$order[$i]];
	if ($ab_settings['sort_users_field'] == $order[$i])
        echo $image_string;
	echo "</a></td>\n";
}

if (count($order) == 0)
{
echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?post_action=members&new_sort_field=email&new_sort_order=".$new_sort_order."\">".$strEmail;
if ($ab_settings['sort_users_field'] == "email")
        echo $image_string;
echo "</a></td>\n";
echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?post_action=members&new_sort_field=company&new_sort_order=".$new_sort_order."\">".$strCompany;
if ($ab_settings['sort_users_field'] == "company")
        echo $image_string;
echo "</a></td>\n";
}


echo "<td class=\"TableHead2\">&nbsp;</td>\n";
echo "</tr>\n";


while ($GO_USERS->next_record())
{
	$middle_name = $GO_USERS->f('middle_name') == '' ? '' : $GO_USERS->f('middle_name').' ';
	$name = $GO_USERS->f('last_name').' '.$middle_name.$GO_USERS->f('first_name');

	echo "<tr id=\"".$GO_USERS->f("id")."\" height=\"18\" class=\"Table1\">\n";
	echo "<td width=\"16\"><input type=\"checkbox\" name=\"user[]\" value=\"".$GO_USERS->f("id")."\" onclick=\"item_click(this)\" /></td>";
	echo "<td nowrap> ".show_profile($GO_USERS->f("id"), $name)."</td>\n";

if (count($order) == 0)
{
	echo "<td>".mail_to(empty_to_stripe($GO_USERS->f("email")))."&nbsp;</td>\n";
	echo "<td>".empty_to_stripe($GO_USERS->f("company"))."&nbsp;</td>\n";
}

	print_config_content($order,$ab,$constMembersPage);

	echo "<td><a href=\"contact.php?user_id=".$GO_USERS->f("id")."&return_to=".urlencode($link_back)."\" title=\"".$contacts_add_member."\"><img src=\"".$GO_THEME->images['save']."\" border=\"0\" />";
	echo "</tr>\n";
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}


if($count > 0)
{
	echo '<tr><td colspan="99" height="18">'.$count.' '.$contacts_members.'</td></tr>';
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}else
{
        echo '<tr><td colspan="99" height="18">'.$contacts_no_contacts.'</td></tr>';
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo "</table>";
?>
<script language="javascript" type="text/javascript">
function item_click(check_box)
{
	var item = get_object(check_box.value);
	if (check_box.checked)
	{
		item.className = 'Table2';
	}else
	{
		item.className = 'Table1';
	}
}

function invert_selection()
{
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
		{
			document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
			item_click(document.forms[0].elements[i]);
		}
	}
}

</script>