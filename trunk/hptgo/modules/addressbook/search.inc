<input type="hidden" name="post_action" value="search" />
<input type="hidden" name="task" />
<input type="hidden" name="_delete" value="false" />

<br />
<table border="0" cellpadding="0" cellspacing="3">
<tr height="30">
	<td nowrap>
	<h2>
	<a href="javascript:letter_click('A')">A</a>&nbsp;&nbsp;
	<a href="javascript:letter_click('B')">B</a>&nbsp;&nbsp;
	<a href="javascript:letter_click('C')">C</a>&nbsp;&nbsp;
	<a href="javascript:letter_click('D')">D</a>&nbsp;&nbsp;
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
	if(isset($_REQUEST['search_type']))
	{
	  $ab_settings['search_type'] = $_REQUEST['search_type'];
	  switch($_REQUEST['search_type'])
	  {
		case 'contacts':
		  $ab_settings['search_contacts_field'] = $_REQUEST['search_field'];
		  $ab_settings['search_addressbook_id'] = $_REQUEST['search_addressbook_id'];
		break;
		
		case 'companies':
		  $ab_settings['search_addressbook_id'] = $_REQUEST['search_addressbook_id'];
		  $ab_settings['search_companies_field'] = $_REQUEST['search_field'];
		  $ab_settings['search_category'] = $_REQUEST['search_category'];
		break;
		
		case 'users':
		  $ab_settings['search_users_field'] = $_REQUEST['search_field'];
		break;
	  }
	  
	  $ab->set_search($GO_SECURITY->user_id, 
					  $ab_settings['search_type'], 
					  $ab_settings['search_contacts_field'], 
					  $ab_settings['search_companies_field'], 
					  $ab_settings['search_users_field'],
					  $ab_settings['search_addressbook_id']);

	}
	
	echo '<table border="0"><tr><td>'.$ab_search_for.':</td><td><table border="0" cellpadding="0" cellspacing="0"><tr><td>';
	$dropbox = new dropbox();
	$dropbox->add_value('contacts', $contacts_contacts);
	$dropbox->add_value('companies', $ab_companies);
	$dropbox->add_value('users', $contacts_members);
	$dropbox->print_dropbox('search_type', $ab_settings['search_type'], 'onchange="javascript:document.forms[0].submit()"');
	echo '</td>';
	
	if ($ab_settings['search_type'] != 'users' && $ab->get_subscribed_addressbooks($GO_SECURITY->user_id) > 1)
	{
		$subscribed_addressbooks = new dropbox();
		$subscribed_addressbooks->add_value('0', $ab_all_your_addressbooks);

		while ($ab->next_record())
		{
			$subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));
		}
		echo '<td>'.$ab_search_in.'&nbsp;</td><td>';
		$subscribed_addressbooks->print_dropbox('search_addressbook_id', $ab_settings['search_addressbook_id']);
		echo '</td>';
	}else
	{
		echo '<input type="hidden" name="search_addressbook_id" value="'.$ab_settings['search_addressbook_id'].'" />';
	}
	
	$dropbox = new dropbox();

	switch ($ab_settings['search_type'])
	{
		case 'companies':
			$search_field = $ab_settings['search_companies_field'];
			$dropbox->add_value('ab_companies.name', $strName);
			$dropbox->add_value('ab_companies.shortname', $strShortCompanyName);
			$dropbox->add_value('ab_companies.engname', $strEnglishCompanyName);
			$dropbox->add_value('ab_companies.email', $strEmail);
			$dropbox->add_value('ab_companies.address',$strAddress);
			$dropbox->add_value('ab_companies.city', $strCity);
			$dropbox->add_value('ab_companies.zip',$strZip);
			$dropbox->add_value('ab_companies.state',$strState);
			$dropbox->add_value('ab_companies.country', $strCountry);

			if ($custom_fields_plugin)
			{
				require_once($custom_fields_plugin['path'].'classes/custom_fields.class.inc');
				$cf = new custom_fields('ab_custom_company_fields');

				if($cf->get_fields())
				{
					while($cf->next_record())
					{
						$dropbox->add_value('ab_custom_company_fields.`'.$cf->f('field').'`', $cf->f('field'));
					}
				}
			}
			$db = new db();
			$db->query("SELECT * FROM ab_categories WHERE parent_id<>0");
			
			$catedrop = new dropbox();

			$catedrop->add_value('-1',$ab_search_all);
			while ($db->next_record())
				$catedrop->add_value($db->f('category_id'),$db->f('category'));

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
				require_once($custom_fields_plugin['path'].'classes/custom_fields.class.inc');
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
			$search_field = $ab_settings['search_users_field'];
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
	echo '</td>';
if ($ab_settings['search_type'] == 'companies')
{	
	echo '<td>'.$ab_search_category.'&nbsp;</td><td>';
	$catedrop->print_dropbox('search_category', $ab_settings['search_category']);
	echo '</td>';
}
	echo '</tr></table></tr></td>';
	?>
	<tr>
		<td><?php echo $ab_search_keyword; ?>:</td>
		<td colspan="3"><input type="text" name="query" size="31" maxlength="255" class="textbox" value="<?php if (isset($_POST['query']) && $task != 'show_letter') echo smartstrip($_POST['query']); ?>">
		<?php echo $ab_wildcard; ?>
		</td>
	</tr>
	</table>
	<br />
	<?php
	$button = new button($cmdSearch, "javascript:_search();");
	?>
	</td>
</tr>
</table>

<?php
if (isset($_POST['_delete']) && $_POST['_delete'] == 'true')
{
	if (isset($_POST['contact']))
	{
		for ($i=0;$i<sizeof($_POST['contact']);$i++)
		{
			$contact = $ab->get_contact($_POST['contact'][$i]);

			if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $contact['acl_write']))
			{
				$ab->delete_contact($_POST['contact'][$i]);
				$GO_SECURITY->delete_acl($contact['acl_write']);
				$GO_SECURITY->delete_acl($contact['acl_read']);
			}
		}
	}

	if (isset($_POST['companies']))
	{
		for ($i=0;$i<sizeof($_POST['companies']);$i++)
		{
			$company = $ab->get_company($_POST['companies'][$i]);

			if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $company['acl_write']))
			{
				$ab->delete_company($_POST['companies'][$i]);
				$GO_SECURITY->delete_acl($company['acl_write']);
				$GO_SECURITY->delete_acl($company['acl_read']);
			}
		}
	}
}

if ($task=='search' || $task == 'show_letter')
{
	if ($_POST['task'] != 'show_letter')
	{
		$query = '%'.smart_addslashes($_POST['query']).'%';
	}else
	{
		$query = smart_addslashes($_POST['query']).'%';
	}

	$count = 0;
	$results = '';
	switch($ab_settings['search_type'])
	{
		case 'contacts':
			
			
			$ab->search_contacts($GO_SECURITY->user_id, $query, $search_field, $ab_settings['search_addressbook_id']);

			while ($ab->next_record())
			{
				if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_write')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_read')))
				{
					$count++;
					if ($ab->f('color') != '')
					{
						$style = ' style="color: '.$ab->f('color').';"';
					}else
					{
						$style = '';
					}

					$middle_name = $ab->f('middle_name') == '' ? '' : $ab->f('middle_name').' ';
					$name = $ab->f('last_name').' '.$middle_name.$ab->f('first_name');

					$results .= '<tr height="20" id="'.$ab->f('id').'">';
					$results .= '<td><input id="'.addslashes($name).'" type="checkbox" onclick="javacript:item_click(this)" name="contact[]" value="'.$ab->f("id").'" /></td>';
					$results .= "<td nowrap><a".$style." class=\"normal\" href=\"contact.php?contact_id=".$ab->f("id")."&return_to=".$GO_MODULES->url."index.php\" title=\"".$strShowProfile."\">".htmlspecialchars($name)."</a>&nbsp;&nbsp;</td>\n";
					$results .= "<td>".mail_to(empty_to_stripe($ab->f("email")), empty_to_stripe($ab->f("email")),'normal',true, $ab->f("id"))."&nbsp;&nbsp;</td>\n";
					$results .= '<td>'.empty_to_stripe($ab->f('home_phone')).'&nbsp;&nbsp;</td>';
					$results .= '<td>'.empty_to_stripe($ab->f('work_phone')).'&nbsp;&nbsp;</td>';
					$results .= "</tr>\n";
					$results .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
				}
			}

			$result_str =  ($count == 1) ? $count.' '.$contacts_result : $count.' '.$contacts_results;
			echo '<br /><h2>'.$result_str.'</h2>';

			if ($count > 0)
			{
				echo '<table border="0" cellpadding="0" cellspacing="0" width="100%">';
				echo '<tr>';
				echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
				echo '<td class="TableHead2">'.$strName.'</td>';
				echo '<td class="TableHead2">'.$strEmail.'</td>';
				echo '<td class="TableHead2">'.$strPhone.'</td>';
				echo '<td class="TableHead2">'.$strWorkphone.'</td>';
				echo '</tr>';

				echo $results;

				echo '</table>';
			}
		break;

		case 'companies':

			$parents = $ab->get_companies_parents($ab_settings['search_addressbook_id'],$GO_SECURITY->user_id);
			$ab->search_companies($GO_SECURITY->user_id, $query, $search_field, $ab_settings['search_addressbook_id'], $ab_settings['search_category']);

			while ($ab->next_record())
			{
				if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_write')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_read')))
				{
					$count++;

					$results .= '<tr height="20" id="'.$ab->f('id').'">';
					$results .= '<td><input id="'.htmlspecialchars($ab->f('name')).'" type="checkbox" onclick="javacript:item_click(this)" name="companies[]" value="'.$ab->f("id").'" /></td>';
					$results .= '<td nowrap><a class="normal" href="'.$GO_MODULES->url.'company.php?company_id='.$ab->f('id').'&return_to='.rawurlencode($link_back).'" title="'.$strShowProfile.'">'.htmlspecialchars($ab->company_name($parents)).'</a>&nbsp;&nbsp;</td>';
					$results .= '<td>'.empty_to_stripe($ab->f('city')).'&nbsp;&nbsp;</td>';
					$results .= "<td>".mail_to(empty_to_stripe($ab->f("email")), empty_to_stripe($ab->f("email")),'normal',true, $ab->f("id"))."&nbsp;&nbsp;</td>\n";
					$results .= '<td>'.empty_to_stripe($ab->f('phone')).'&nbsp;&nbsp;</td>';
					$results .=  '<td><a class="normal" target="_blank" href="'.$ab->f('homepage').'">'.$ab->f('homepage').'</a>&nbsp;&nbsp;</td>';
					$results .= "</tr>\n";
					$results .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
				}
			}

			$result_str =  ($count == 1) ? $count.' '.$contacts_result : $count.' '.$contacts_results;
			echo '<br /><h2>'.$result_str.'</h2>';

			if ($count > 0)
			{
				echo '<table border="0" cellpadding="0" cellspacing="0" width="100%">';
				echo '<tr>';
				echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
				echo '<td class="TableHead2">'.$strName.'</td>';
				echo '<td class="TableHead2">'.$strCity.'</td>';
				echo '<td class="TableHead2">'.$strEmail.'</td>';
				echo '<td class="TableHead2">'.$strPhone.'</td>';
				echo '<td class="TableHead2">'.$strHomepage.'</td>';
				echo '</tr>';

				echo $results;

				echo '</table>';
			}
		break;

		case 'users':
			$GO_USERS->search($query, $search_field, $GO_SECURITY->user_id);

			while ($GO_USERS->next_record())
			{
				if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $GO_USERS->f('acl_id')))
				{
					$middle_name = $GO_USERS->f('middle_name') == '' ? '' : $GO_USERS->f('middle_name').' ';
					$name = $GO_USERS->f('last_name').' '.$middle_name.$GO_USERS->f('first_name');

					$results .= '<tr height="20"><td><a href="" class="normal">'.show_profile($GO_USERS->f('id'), $name).'</a>&nbsp;</td>';
					$results .= "<td>".mail_to(empty_to_stripe($GO_USERS->f("email")))."&nbsp;</td>\n";
					$results .= "</tr>\n";
					$results .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
					$count++;
				}
			}

			$result_str =  ($count == 1) ? $count.' '.$contacts_result : $count.' '.$contacts_results;
			echo '<br /><h2>'.$result_str.'</h2>';

			if ($count > 0)
			{
				echo '<table border="0" cellpadding="0" cellspacing="0" width="100%">';
				echo '<tr>';
				echo '<td class="TableHead2">'.$strName.'</td>';
				echo '<td class="TableHead2">'.$strEmail.'</td>';
				echo '<td class="TableHead2">'.$strPhone.'</td>';
				echo '<td class="TableHead2">'.$strWorkphone.'</td>';
				echo '</tr>';

				echo $results;

				echo '</table>';
			}

		break;
	}
}
?>


<script type="text/javascript">
document.forms[0].query.focus();

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
		_search();
	return true;
  }
}
if (window.Event) //if Navigator 4.X
{
	document.captureEvents(Event.KEYPRESS)
}
document.onkeypress = processkeypress;


function letter_click(letter)
{
	document.forms[0].task.value='show_letter';
	document.forms[0].query.value=letter;
	document.forms[0].submit();
}

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

function _search()
{
	document.forms[0].task.value='search';
	document.forms[0].submit()
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

function confirm_delete()
{
	var count = 0;
	var name = new String;

	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
		{
			if (document.forms[0].elements[i].checked == true)
			{
				count++;
				name = document.forms[0].elements[i].id;
			}
		}
	}
	switch (count)
	{
		case 0:
			alert("<?php echo $contacts_no_select; ?>");
		break;

		case 1:
			if (confirm("<?php echo $strDeletePrefix; ?> '"+name+"' <?php echo $strDeleteSuffix; ?>"))
			{
				document.forms[0].task.value='<?php echo $task; ?>';
				document.forms[0]._delete.value='true';
				document.forms[0].submit();
			}
		break;

		default:
			if (confirm("<?php echo $strDeletePrefix.$strThis; ?> "+count+" <?php echo $contacts_contacts.$strDeleteSuffix; ?>"))
			{
				document.forms[0].task.value='<?php echo $task; ?>';
				document.forms[0]._delete.value='true';
				document.forms[0].submit();
			}
		break;
	}
}
</script>
