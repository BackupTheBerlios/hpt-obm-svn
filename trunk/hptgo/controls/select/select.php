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

if (isset($_REQUEST['handler_base64_encoded']))
{
	$_REQUEST['GO_HANDLER'] = base64_decode($_REQUEST['GO_HANDLER']);
}
require("../../Group-Office.php");
$GO_SECURITY->authenticate();

//check for the addressbook module$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || 
	!($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) ||
 	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
	$ab_module = false;
}

require($GO_LANGUAGE->get_language_file('addressbook'));

$GO_FIELD = isset($_REQUEST['GO_FIELD']) ? $_REQUEST['GO_FIELD'] : '';
$GO_HANDLER = isset($_REQUEST['GO_HANDLER']) ? $_REQUEST['GO_HANDLER'] : '';

$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : 'search';
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

$pass_value = isset($_REQUEST['pass_value']) ? $_REQUEST['pass_value'] : 'email';
$multiselect = (isset($_REQUEST['multiselect']) && $_REQUEST['multiselect'] == 'true') ? true : false;
$require_email_address = (isset($_REQUEST['require_email_address']) && $_REQUEST['require_email_address'] == 'true') ? true : false;
$show_users = (isset($_REQUEST['show_users']) && $_REQUEST['show_users'] == 'true') ? true : false;
$show_contacts = (isset($_REQUEST['show_contacts']) && $_REQUEST['show_contacts'] == 'true') ? true : false;
$show_companies = (isset($_REQUEST['show_companies']) && $_REQUEST['show_companies'] == 'true') ? true : false;

if ($show_contacts || $show_companies)
{
	$GO_MODULES->authenticate('addressbook');
	
	require_once($ab_module['class_path'].'addressbook.class.inc');
	$ab1 = new addressbook();
	$ab2 = new addressbook();
	$custom_fields_plugin = $GO_MODULES->get_plugin('custom_fields');
	
	$ab_settings = $ab1->get_settings($GO_SECURITY->user_id);
	$search_type = $ab_settings['search_type'];
}else
{
  $search_type = 'users';
}

$page_title = $contacts_select;
require($GO_THEME->theme_path."header.inc");
?>
<script type="text/javascript" language="javascript">
var nav4 = window.Event ? true : false;
function processkeypress(e)
{
	if(nav4)
    {
		var whichCode = e.which;
	}else
 	{
		var whichCode = event.keyCode;
	}

	if (whichCode == 13)
	{
		search();
		return true;
    }
}
if (window.Event) //if Navigator 4.X
{
	document.captureEvents(Event.KEYPRESS)
}
document.onkeypress = processkeypress;


function search()
{
	document.select.action = "<?php echo $_SERVER['PHP_SELF']; ?>";
	document.select.task.value = 'search';
	document.select.submit();
}

function item_click(id, check_box)
{
	var item = get_object(id);
	if (item)
	{
		if (check_box.checked)
		{
			item.style.backgroundColor = '#FFFFCC';
		}else
		{
			item.style.backgroundColor = '#FFFFFF';
		}
	}
}

function invert_selection()
{
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
		{
			document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
			document.forms[0].elements[i].onclick();
		}
	}
}

function expand_group(group_id)
{
	document.select.action = "<?php echo $_SERVER['PHP_SELF']; ?>";
	document.forms[0].new_sort_order.value = '';
	document.forms[0].expand_id.value = group_id;
	document.forms[0].task.value = "expand";
	document.forms[0].submit();
}

function select_group(group_id)
{
	var add = false;

	for (var i = 0; i < document.forms[0].elements.length; i++)
	{
		if (document.forms[0].elements[i].name == 'group_start_'+group_id)
		{
			add = true;
		}

		if (document.forms[0].elements[i].name == 'group_end_'+group_id)
		{
			add = false;
		}

		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy' && add==true)
		{
			document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
			document.forms[0].elements[i].onclick();
		}
	}
}

function change_addressbook()
{
	document.select.action = "<?php echo $_SERVER['PHP_SELF']; ?>";
	document.select.submit();
}

function change_mode(mode)
{
	document.select.action = "<?php echo $_SERVER['PHP_SELF']; ?>";
	document.select.post_action.value = mode;
	document.select.submit();
}

function _click(clicked_value, clicked_type)
{
	document.select.clicked_type.value=clicked_type;
	document.select.clicked_value.value=clicked_value;
	document.select.submit();
}

function letter_click(letter)
{
	document.select.action = "<?php echo $_SERVER['PHP_SELF']; ?>";
	document.select.post_action.value = 'search';
	document.select.task.value='show_letter';
	document.select.query.value=letter;
	document.select.submit();
}
function sort(column)
{
	document.select.action = "<?php echo $_SERVER['PHP_SELF']; ?>";
	document.forms[0].task.value = 'sort';
	document.forms[0].new_sort_field.value = column;
	document.forms[0].submit();
}
</script>
<form method="post" name="select" action="<?php echo $GO_HANDLER; ?>">
<?php
if ($multiselect)
{
	echo '<input type="hidden" value="true" name="multiselect" />';
}
if($require_email_address)
{
	echo '<input type="hidden" value="true" name="require_email_address" />';
}
if($show_users)
{
	echo '<input type="hidden" value="true" name="show_users" />';
	$types_used[] = 'users';
}
if($show_contacts)
{
	echo '<input type="hidden" value="true" name="show_contacts" />';
	$types_used[] = 'contacts';
}
if($show_companies)
{
	$types_used[]='companies';
	echo '<input type="hidden" value="true" name="show_companies" />';
}

if(isset($_REQUEST['search_type']) && isset($ab_settings))
{
  $search_type = $_REQUEST['search_type'];
  switch($_REQUEST['search_type'])
  {
	case 'contacts':
	  $ab_settings['search_contacts_field'] = $_REQUEST['search_field'];
	  $ab_settings['search_addressbook_id'] = $_REQUEST['search_addressbook_id'];
	break;
	
	case 'companies':
	  $ab_settings['search_addressbook_id'] = $_REQUEST['search_addressbook_id'];
	  $ab_settings['search_companies_field'] = $_REQUEST['search_field'];
	break;
	
	case 'users':
	  $ab_settings['search_users_field '] = $_REQUEST['search_field'];
	break;
  }
  
  $ab1->set_search($GO_SECURITY->user_id, 
				  $search_type, 
				  $ab_settings['search_contacts_field'], 
				  $ab_settings['search_companies_field'], 
				  $ab_settings['search_users_field'],
				  $ab_settings['search_addressbook_id']);

}

$search_type = in_array($search_type, $types_used) ? $search_type : $types_used[0];
	
/*if ($search_type != 'users')
{
	$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : $ab1->get_default_addressbook($GO_SECURITY->user_id);
	if (!$addressbook_id)
	{
		$addressbook_id = $ab1->add_addressbook($GO_SECURITY->user_id, $_SESSION['GO_SESSION']['name']);
	}
	$addressbook = $ab1->get_addressbook($addressbook_id);
}*/
?>
<input type="hidden" name="pass_value" value="<?php echo $pass_value; ?>" />
<input type="hidden" name="post_action" value="<?php echo $post_action; ?>" />
<input type="hidden" name="task" />
<input type="hidden" name="GO_FIELD" value="<?php echo $_REQUEST['GO_FIELD']; ?>" />
<input type="hidden" name="GO_HANDLER" value="<?php echo $_REQUEST['GO_HANDLER']; ?>" />
<input type="hidden" name="clicked_value" />
<input type="hidden" name="clicked_type" />

<?php
if ($show_contacts)
{
	?>
	<table border="0" cellspacing="0" cellpadding="0">
	<tr>
		<td class="ModuleIcons">
		<a class="small" href="javascript:change_mode('search');"><img src="<?php echo $GO_THEME->images['ab_search']; ?>" border="0" height="32" width="32" /><br /><?php echo $contacts_search; ?></a></td>
		</td>
		<td class="ModuleIcons">
		<a class="small" href="javascript:change_mode('contacts');"><img src="<?php echo $GO_THEME->images['ab_browse']; ?>" border="0" height="32" width="32" /><br /><?php echo $ab_browse; ?></a></td>
		</td>
	</tr>
	</table>
	<?php
}

$contacts = isset($_POST['contacts']) ? $_POST['contacts'] : array();
$users = isset($_POST['users']) ? $_POST['users'] : array();
$companies = isset($_POST['companies']) ? $_POST['companies'] : array();

if (isset($_REQUEST['address_string']))
{
	$addresses = cut_address($_REQUEST['address_string'],$charset);
}else
{
	$addresses = isset($_POST['addresses']) ? $_POST['addresses'] : array();;
}

if ($pass_value == 'email')
{
	$addresses = array_merge($addresses, $contacts, $users, $companies);
}

switch($post_action)
{
	case 'search':
	$count = 0;
	?>
	<table border="0" cellpadding="0" cellspacing="3">
	<tr height="30">
		<td nowrap>
		<h2>
		<a href="javascript:letter_click('A')">A</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('B')">B</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('C')">C</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('D')">D</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('Đ')">Đ</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('E')">E</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('F')">F</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('G')">G</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('H')">H</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('I')">I</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('J')">J</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('K')">K</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('L')">L</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('M')">M</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('N')">N</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('O')">O</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('P')">P</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('Q')">Q</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('R')">R</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('S')">S</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('T')">T</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('U')">U</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('V')">V</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('W')">W</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('X')">X</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('Y')">Y</a>&nbsp;&nbsp;
		<a href="javascript:letter_click('Z')">Z</a>&nbsp;&nbsp;
		</h2>
		</td>
	</tr>
	<tr>
		<td>
		<?php
		echo '<table border="0"><tr><td>'.$ab_search_for.':</td><td><table border="0" cellpadding="0" cellspacing="0"><tr><td>';

		$dropbox = new dropbox();
		if ($show_users)
		{
			$dropbox->add_value('users', $contacts_members);
		}
		if ($show_contacts)
		{
			$dropbox->add_value('contacts', $contacts_contacts);
		}
		if ($show_companies)
		{
			$dropbox->add_value('companies', $ab_companies);
		}
		$dropbox->print_dropbox('search_type', $search_type, 'onchange="javascript:change_addressbook()"');
		echo '</td>';

		if ($search_type != 'users')
		{
		  if($ab1->get_subscribed_addressbooks($GO_SECURITY->user_id) > 1)
		  {
			$subscribed_addressbooks = new dropbox();

			$subscribed_addressbooks->add_value('0', $ab_all_your_addressbooks);

			while ($ab1->next_record())
			{
				$subscribed_addressbooks->add_value($ab1->f('id'), $ab1->f('name'));
			}
			echo '<td>'.$ab_search_in.'&nbsp;</td><td>';
			$subscribed_addressbooks->print_dropbox('search_addressbook_id', $ab_settings['search_addressbook_id']);
			echo '</td>';
		  }else
		  {
			  echo '<input type="hidden" name="search_addressbook_id" value="'.$ab_settings['search_addressbook_id'].'" />';
		  }
		}  
		$dropbox = new dropbox();
		$custom_fields_plugin = $GO_MODULES->get_plugin('custom_fields');

		switch ($search_type)
		{
			case 'companies':
				$search_field = $ab_settings['search_companies_field'];
				$dropbox->add_value('name', $strName);
				$dropbox->add_value('email', $strEmail);
				$dropbox->add_value('address',$strAddress);
				$dropbox->add_value('city', $strCity);
				$dropbox->add_value('zip',$strZip);
				$dropbox->add_value('state',$strState);
				$dropbox->add_value('country', $strCountry);
				if ($custom_fields_plugin)
				{
					require_once($custom_fields_plugin['class_path'].'custom_fields.class.inc');
					$cf = new custom_fields('ab_custom_company_fields');

					if($cf->get_fields())
					{
						while($cf->next_record())
						{
							$dropbox->add_value('ab_custom_company_fields.`'.$cf->f('field').'`', $cf->f('field'));
						}
					}
				}

			break;

			case 'contacts':
				$search_field = $ab_settings['search_contacts_field'];
				$dropbox->add_value('ab_contacts.first_name', $strFirstName);
				$dropbox->add_value('ab_contacts.last_name', $strLastName);
				$dropbox->add_value('ab_contacts.email', $strEmail);
				$dropbox->add_value('ab_contacts.department',$strDepartment);
				$dropbox->add_value('ab_contacts.function',$strFunction);
				$dropbox->add_value('ab_contacts.address',$strAddress);
				$dropbox->add_value('ab_contacts.city', $strCity);
				$dropbox->add_value('ab_contacts.zip',$strZip);
				$dropbox->add_value('ab_contacts.state',$strState);
				$dropbox->add_value('ab_contacts.country', $strCountry);
				$dropbox->add_value('ab_contacts.comment', $ab_comment);
				if ($custom_fields_plugin)
				{
					require_once($custom_fields_plugin['class_path'].'custom_fields.class.inc');
					$cf = new custom_fields('ab_custom_contact_fields');

					if($cf->get_fields())
					{
						while($cf->next_record())
						{
							$dropbox->add_value('ab_custom_contact_fields.`'.$cf->f('field').'`', $cf->f('field'));
						}
					}
				}
			break;

			case 'users':
				$_search_field = isset($_POST['search_field']) ? $_POST['search_field'] : 'users.first_name';
				$search_field = isset($ab_settings['search_users_field']) ? $ab_settings['search_users_field'] : $_search_field;
				$dropbox->add_value('users.first_name', $strFirstName);
				$dropbox->add_value('users.last_name', $strLastName);
				$dropbox->add_value('users.email', $strEmail);
				$dropbox->add_value('users.department',$strDepartment);
				$dropbox->add_value('users.function',$strFunction);
				$dropbox->add_value('users.address',$strAddress);
				$dropbox->add_value('users.city', $strCity);
				$dropbox->add_value('users.zip',$strZip);
				$dropbox->add_value('users.state',$strState);
				$dropbox->add_value('users.country', $strCountry);
				$dropbox->add_value('users.comment', $ab_comment);
			break;
		}
		echo '<td>'.$ab_search_on.'&nbsp;</td><td>';
		$dropbox->print_dropbox('search_field', $search_field);
		echo '</td></tr></table></td></tr>';
		?>
		<tr>
			<td><?php echo $ab_search_keyword; ?>:</td>
			<td colspan="3"><input type="text" name="query" size="31" maxlength="255" class="textbox" value="<?php if (isset($_POST['query']) && $task != 'show_letter') echo htmlspecialchars(smartstrip($_POST['query'])); ?>">
			<?php
			$button = new button($cmdSearch, "javascript:search()");
			?>
			</td>
		</tr>
		</table>
		</td>
	</tr>
	</table>
	<br />

	<?php
	if ($task == 'search' || $task == 'show_letter')
	{
		if ($task == 'show_letter')
		{
			$query = smart_addslashes($_POST['query']).'%';
		}else
		{
			$query = '%'.smart_addslashes($_POST['query']).'%';
		}

		if ($search_type == 'contacts' || $search_type == 'users')
		{
			if ($search_type == 'users')
			{
				$click_type = 'user';
				$array_name = 'users[]';
				$ab1 = new GO_USERS();
				$ab1->search($query, $search_field, $GO_SECURITY->user_id);
			}else
			{
				$click_type = 'contact';
				$array_name = 'contacts[]';
				$ab1->search_contacts($GO_SECURITY->user_id, $query, $search_field, $ab_settings['search_addressbook_id']);
			}

			$search_results = '';
			while ($ab1->next_record())
			{
				if ((!$require_email_address || $ab1->f("email") != '') && (($search_type == 'users' && $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab1->f('acl_id'))) || ($search_type != 'users' && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab1->f('acl_read')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab1->f('acl_write'))))))
				{
					$class="Table1";
					$check = "";

					if ($pass_value == 'email')
					{
						if ($ab1->f("email") != "")
						{
							$key = array_search($ab1->f("email"), $addresses);
						}else
						{
							$key = false;
						}
						if (is_int($key))
						{
							unset($addresses[$key]);
							$check = "checked";
							$class = "Table2";
						}
					}elseif($search_type == 'users')
					{
						$key = array_search($ab1->f($pass_value), $users);

						if (is_int($key))
						{
							unset($users[$key]);
							$check = "checked";
							$class = "Table2";
						}
					}elseif($search_type == 'contracts')
					{
						$key = array_search($ab1->f($pass_value), $contacts);

						if (is_int($key))
						{
							unset($contacts[$key]);
							$check = "checked";
							$class = "Table2";
						}
					}

					$search_results .= "<tr id=\"".$ab1->f('id')."\" class=\"".$class."\" height=\"20\">\n";
					if ($multiselect)
					{
						$search_results .= '<td><input onclick="javascript:item_click('.$ab1->f("id").', this);" type="checkbox" name="'.$array_name.'" value="'.$ab1->f($pass_value).'" '.$check.' /></td>';
					}

					if ($search_type != 'users' && $ab1->f('color') != '')
					{
						$style = ' style="color: '.$ab1->f('color').';"';
					}else
					{
						$style = '';
					}
					$middle_name = $ab1->f('middle_name') == '' ? '' : $ab1->f('middle_name').' ';
					$name = $ab1->f('last_name').' '.$middle_name.$ab1->f('first_name');

					$search_results .= '<td><a'.$style.' href="javascript:_click(\''.$ab1->f($pass_value).'\', \''.$click_type.'\');" class="normal">'.$name.'</a>&nbsp;</td>';
					$search_results .= "<td>".mail_to(empty_to_stripe($ab1->f("email")))."&nbsp;</td>\n";
					$search_results .= "</tr>\n";
					$search_results .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
					$count++;
				}
			}
			echo '<tr><td><h2>'.$count.' '.$contacts_results.'</h2>';
			echo '<tr><td>';

			if ($count > 0)
			{
				echo '<table border="0" cellpadding="0" cellspacing="0" width="100%">';
				echo '<tr>';
				if ($multiselect)
				{
					echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
				}
				echo '<td class="TableHead2">'.$strName.'</td>';
				echo '<td class="TableHead2">'.$strEmail.'</td>';
				echo '</tr>';
				echo $search_results;
				echo '</table>';
			}
		}else
		{
			$ab1->search_companies($GO_SECURITY->user_id, $query, $search_field, $ab_settings['search_addressbook_id']);

			$search_results = '';
			while ($ab1->next_record())
			{
				if ((!$require_email_address || $ab1->f("email") != '') && (($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab1->f('acl_read')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab1->f('acl_write')))))
				{
					$count++;

					$class="Table1";
					$check = "";

					if ($pass_value == 'email')
					{
						if ($ab1->f("email") != "")
						{
							$key = array_search($ab1->f("email"), $addresses);
						}else
						{
							$key = false;
						}
						if (is_int($key))
						{
							unset($addresses[$key]);
							$check = "checked";
							$class = "Table2";
						}
					}else
					{
						$key = array_search($ab1->f($pass_value), $companies);
						if (is_int($key))
						{
							unset($companies[$key]);
							$check = "checked";
							$class = "Table2";
						}
					}

					$search_results .= "<tr id=\"".$ab1->f('id')."\" class=\"".$class."\" height=\"20\">\n";
					if ($multiselect)
					{
						$search_results .= '<td><input onclick="javascript:item_click('.$ab1->f("id").', this);" type="checkbox" name="companies[]" value="'.$ab1->f($pass_value).'" '.$check.' /></td>';
					}
					$search_results .= '<td><a href="javascript:_click(\''.$ab1->f($pass_value).'\', \'company\');" class="normal">'.$ab1->f('name').'</a>&nbsp;</td>';
					$search_results .= "<td>".mail_to(empty_to_stripe($ab1->f("email")), empty_to_stripe($ab1->f("email")),'normal',true, $ab1->f("id"))."&nbsp;</td>\n";
					$search_results .= "</tr>\n";
					$search_results .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
				}
			}

			$result_str =  ($count == 1) ? $count.' '.$contacts_result : $count.' '.$contacts_results;
			echo '<br /><h2>'.$result_str.'</h2>';

			if ($count > 0)
			{
				echo '<table border="0" cellpadding="0" cellspacing="0" width="100%">';
				echo '<tr>';
				if ($multiselect)
				{
					echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
				}
				echo '<td class="TableHead2">'.$strName.'</td>';
				echo '<td class="TableHead2">'.$strEmail.'</td>';
				echo '</tr>';

				echo $search_results;

				echo '</table>';
			}
		}
	}
	echo '<script type="text/javascript">document.select.query.focus();</script>';
	break;

	case 'contacts':
		$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : $ab1->get_default_addressbook($GO_SECURITY->user_id);

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
			$addressbook_id = $ab1->add_addressbook($GO_SECURITY->user_id, addslashes($new_ab_name));
		}
		
		if ($task == 'expand')
		{
			if (isset($_POST['expand_id']))
			{
				$key = array_search($_POST['expand_id'], $_SESSION['contacts_expanded']);
				if (!$key)
				{
					$_SESSION['contacts_expanded'][]=$_POST['expand_id'];
				}else
				{
					unset($_SESSION['contacts_expanded'][$key]);
				}
			}
		}

		echo '<input type="hidden" name="new_sort_field" />';
		echo '<input type="hidden" name="expand_id" />';
		if (!isset($_SESSION['contacts_expanded']))
			$_SESSION['contacts_expanded'][]=-1;
		
		if($task == 'sort')
		{
		  $ab1->set_contacts_sorting($GO_SECURITY->user_id, 
								  $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
		  $ab_settings['sort_contacts_order'] = $_REQUEST['new_sort_order'];
		  $ab_settings['sort_contacts_field'] = $_REQUEST['new_sort_field'];  
		}
		if ($ab_settings['sort_contacts_order'] == "DESC")
		{
			$image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
			$new_sort_order = "ASC";
		}else
		{
			$image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
			$new_sort_order = "DESC";
		}
		echo '<input type="hidden" value="'.$new_sort_order.'" name="new_sort_order" />';

		if ($ab1->get_subscribed_addressbooks($GO_SECURITY->user_id) > 1)
		{
			echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td>'.$ab_addressbook.':</td><td>';
			$subscribed_addressbooks = new dropbox();
			while ($ab1->next_record())
			{
				$subscribed_addressbooks->add_value($ab1->f('id'), $ab1->f('name'));
			}
			$subscribed_addressbooks->print_dropbox('addressbook_id', $addressbook_id, 'onchange="javascript:change_addressbook()"');
			echo '</td></tr></table>';
		}else
		{
			echo '<input type="hidden" name="addressbook_id" value="'.$addressbook_id.'" />';
		}

		echo "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">\n";
		echo '<tr><td class="TableHead2">&nbsp;</td>';
		if ($multiselect)
		{
			echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
		}
		echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"javascript:sort('name')\">".$strName;
		if ($ab_settings['sort_contacts_field'] == "name")
		        echo $image_string;
		echo "</a></td>\n";
		echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"javascript:sort('email')\">".$strEmail;
		if ($ab_settings['sort_contacts_field'] == "email")
		        echo $image_string;
		echo "</a></td>\n";
		echo "</tr>\n";

		if($group_count = $ab2->get_groups($addressbook_id))
		{
			while($ab2->next_record())
			{
				if (in_array($ab2->f('id'), $_SESSION['contacts_expanded']))
				{
					echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(".$ab2->f('id').")\"><img src=\"".$GO_THEME->images['min_node']."\" border=\"0\" /></a></td><td><input type=\"checkbox\" name=\"dummy\" value=\"dummy\" onclick=\"javascript:select_group('".$ab2->f('id')."')\" /></td><td colspan=\"4\">".$ab2->f('name')."</td></tr>";
					echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

					if ($ab1->get_contacts_group($addressbook_id, $ab2->f('id'), $ab_settings['sort_contacts_field'], $ab_settings['sort_contacts_order']) > 0)
					{
						echo '<input type="hidden" name="group_start_'.$ab2->f('id').'" />';
						while ($ab1->next_record())
						{
							$check = "";
							$class = 'Table1';

							if ($pass_value == 'email')
							{
								if ($ab1->f("email") != "")
								{
									$key = array_search($ab1->f("email"), $addresses);
								}else
								{
									$key = false;
								}
								if (is_int($key))
								{
									unset($addresses[$key]);
									$check = "checked";
									$class = "Table2";
								}
							}else
							{
								$key = array_search($ab1->f($pass_value), $contacts);
								if (is_int($key))
								{
									unset($contacts[$key]);
									$checked = "checked";
									$class = 'Table2';
								}
							}

							if ($ab1->f('color') != '')
							{
								$style = ' style="color: '.$ab1->f('color').';"';
							}else
							{
								$style = '';
							}

							$middle_name = $ab1->f('middle_name') == '' ? '' : $ab1->f('middle_name').' ';
							$name = $ab1->f('last_name').' '.$middle_name.$ab1->f('first_name');

							echo "<tr id=\"".$ab1->f("id")."\" class=\"".$class."\" height=\"20\"><td></td>\n";
							
							if (!$require_email_address || $ab1->f('email') != '')
							{
								if ($multiselect)
								{
									echo "<td><input id=\"".$name."\" type=\"checkbox\" onclick=\"javacript:item_click(".$ab1->f('id').", this);\" name=\"contacts[]\" value=\"".$ab1->f($pass_value)."\" ".$check." /></td>";
								}
								echo "<td><a".$style." class=\"normal\" href=\"javascript:_click('".$ab1->f($pass_value)."', 'contact');\">".empty_to_stripe($name)."</a>&nbsp;</td>\n";
							}else
							{
								if ($multiselect)
								{
									echo '<td></td>';
								}
								echo '<td><span style="'.$style.'">'.$name.'</span></td>';
							}
							
							echo "<td>".mail_to(empty_to_stripe($ab1->f("email")), empty_to_stripe($ab1->f("email")),'normal',true, $ab1->f("id"))."&nbsp;</td>\n";
							echo "</tr>\n";
							echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
						}
						echo '<input type="hidden" name="group_end_'.$ab2->f('id').'" />';
					}else
					{
						echo "<tr><td colspan=\"99\" height=\"18\">".$contacts_empty_group."</td></tr>";
						echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
					}
				}else
				{
					echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(".$ab2->f('id').")\"><img src=\"".$GO_THEME->images['plus_node']."\" border=\"0\" /></a></td><td>&nbsp;</td><td colspan=\"4\" width=\"100%\">".$ab2->f('name')."</td></tr>";
					echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
				}
			}
		}

		if ($group_count > 0)
		{
			if (in_array(0, $_SESSION['contacts_expanded']))
			{
				echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(0)\"><img src=\"".$GO_THEME->images['min_node']."\" border=\"0\" /></a><td><input type=\"checkbox\" name=\"dummy\" value=\"dummy\" onclick=\"javascript:select_group('0')\" /></td><td colspan=\"4\">".$contacts_other."</td></tr>";
				echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

				$ab1->get_contacts_group($addressbook_id,0, $ab_settings['sort_contacts_field'], $ab_settings['sort_contacts_order']);
				if ($ab1->num_rows() > 0)
				{
					echo '<input type="hidden" name="group_start_0" />';
					while ($ab1->next_record())
					{
						$check = "";
						$class = 'Table1';
						if ($pass_value == 'email')
						{
							if ($ab1->f("email") != "")
							{
								$key = array_search($ab1->f("email"), $addresses);
							}else
							{
								$key = false;
							}
							if (is_int($key))
							{
								unset($addresses[$key]);
								$check = "checked";
								$class = "Table2";
							}
						}else
						{
							$key = array_search($ab1->f($pass_value), $contacts);
							if (is_int($key))
							{
								unset($contacts[$key]);
								$checked = "checked";
								$class = 'Table2';
							}
						}

						if ($ab1->f('color') != '')
						{
							$style = ' style="color: '.$ab1->f('color').';"';
						}else
						{
							$style = '';
						}

						$middle_name = $ab1->f('middle_name') == '' ? '' : $ab1->f('middle_name').' ';
						$name = $ab1->f('last_name').' '.$middle_name.$ab1->f('first_name');
						echo "<tr id=\"".$ab1->f("id")."\" class=\"".$class."\" height=\"20\"><td></td>\n";
						if (!$require_email_address || $ab1->f('email') != '')
						{
							if ($multiselect)
							{
								echo "<td><input id=\"".$name."\" type=\"checkbox\" onclick=\"javacript:item_click(".$ab1->f('id').", this);\" name=\"contacts[]\" value=\"".$ab1->f($pass_value)."\" ".$check." /></td>";
							}
							echo "<td><a".$style." class=\"normal\" href=\"javascript:_click('".$ab1->f($pass_value)."', 'contact');\">".empty_to_stripe($name)."</a>&nbsp;</td>\n";
						}else
						{
							if ($multiselect)
							{
								echo '<td></td>';
							}
							echo '<td><span style="'.$style.'">'.$name.'</span></td>';
						}
						echo "<td>".mail_to(empty_to_stripe($ab1->f("email")), empty_to_stripe($ab1->f("email")),'normal',true, $ab1->f("id"))."&nbsp;</td>\n";
						echo "</tr>\n";
						echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
					}
					echo '<input type="hidden" name="group_end_0" />';
				}else
				{
					if ($group_count > 0)
						$text = $contacts_empty_group;
					else
						$text = $contacts_no_contacts;

					echo "<tr><td colspan=\"99\" height=\"18\" class=\"normal\">".$text."</td></tr>";
					echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
				}
			}else
			{
				echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(0)\"><img src=\"".$GO_THEME->images['plus_node']."\" border=\"0\" /></a><td>&nbsp;</td><td colspan=\"4\" width=\"100%\">".$contacts_other."</td></tr>";
				echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
			}
		}else
		{
			$ab1->get_contacts_group($addressbook_id, 0, $ab_settings['sort_contacts_field'], $ab_settings['sort_contacts_order']);
			if ($ab1->num_rows() > 0)
			{
				echo '<input type="hidden" name="group_start_0" />';
				while ($ab1->next_record())
				{
					$class = 'Table1';
					$check = "";
					if ($pass_value == 'email')
					{
						if ($ab1->f("email") != "")
						{
							$key = array_search($ab1->f("email"), $addresses);
						}else
						{
							$key = false;
						}
						if (is_int($key))
						{
							unset($addresses[$key]);
							$check = "checked";
							$class = "Table2";
						}
					}else
					{
						$key = array_search($ab1->f($pass_value), $contacts);
						if (is_int($key))
						{
							unset($contacts[$key]);
							$checked = "checked";
							$class = 'Table2';
						}
					}
					if ($ab1->f('color') != '')
					{
						$style = ' style="color: '.$ab1->f('color').';"';
					}else
					{
						$style = '';
					}
					$middle_name = $ab1->f('middle_name') == '' ? '' : $ab1->f('middle_name').' ';
					$name = $ab1->f('last_name').' '.$middle_name.$ab1->f('first_name');

					echo "<tr id=\"".$ab1->f("id")."\" class=\"".$class."\" height=\"20\"><td></td>\n";
					if (!$require_email_address || $ab1->f('email') != '')
					{
						if ($multiselect)
						{
							echo "<td><input id=\"".$name."\" type=\"checkbox\" onclick=\"javacript:item_click(".$ab1->f('id').", this);\" name=\"contacts[]\" value=\"".$ab1->f($pass_value)."\" ".$check." /></td>";
						}
						echo "<td><a".$style." class=\"normal\" href=\"javascript:_click('".$ab1->f($pass_value)."', 'contact');\">".empty_to_stripe($name)."</a>&nbsp;</td>\n";
					}else
					{
						if ($multiselect)
						{
							echo '<td></td>';
						}
						echo '<td><span style="'.$style.'">'.$name.'</span></td>';
					}
					echo "<td>".mail_to(empty_to_stripe($ab1->f("email")), empty_to_stripe($ab1->f("email")),'normal',true, $ab1->f("id"))."&nbsp;</td>\n";
					echo "</tr>\n";
					echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
				}
				echo '<input type="hidden" name="group_end_0" />';
			}else
			{
				if ($group_count > 0)
					$text = $contacts_empty_group;
				else
					$text = $contacts_no_contacts;

				echo "<tr><td colspan=\"99\" height=\"18\" class=\"normal\" width=\"100%\">".$text."</td></tr>";
				echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
			}
		}
	break;
}

echo '<table border="0" width="100%"><tr><td align="center"><br />';
if($multiselect && ($post_action != 'search' || $count > 0))
{
	$button = new button($cmdAdd,'javascript:document.forms[0].submit()');
	echo '&nbsp;&nbsp;';
}
$button = new button($cmdCancel,'javascript:window.close();');
echo '</td></tr></table>';


while($address = array_pop($addresses))
{
	echo '<input type="hidden" name="addresses[]" value="'.$address.'" />';
}

echo '</form>';

require($GO_THEME->theme_path."footer.inc");
?>
