<input type="hidden" name="source_id" value="<?php echo $contact['source_id']; ?>" />
<table border="0" cellpadding="0" cellspacing="0">
<tr>
	<td>
		<?php if (isset($feedback)) echo $feedback; ?>
        <table border=0 cellpadding="2" cellspacing="0">
		<tr>
		<td valign="top">
		<table border="0" cellpadding="0" cellspacing="3">
		<tr>
			<td align="right" nowrap>
			<?php echo $strFirstName; ?>*:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="first_name" size="40" value="<?php echo htmlspecialchars($contact['first_name']); ?>" maxlength="50">
			</td>
		</tr>
		<tr heigth="25">
			<td align="right" nowrap><?php echo $strMiddleName; ?>:&nbsp;</td>
			<td width="100%"><input type="text" class="textbox"  name="middle_name" size="40" maxlength="50" value="<?php echo htmlspecialchars($contact["middle_name"]); ?>"></td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strLastName; ?>*:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="last_name" size="40" value="<?php echo htmlspecialchars($contact['last_name']); ?>" maxlength="50">
			</td>
		</tr>
		<tr heigth="25">
			<td align="right" nowrap><?php echo $strTitle; ?>&nbsp;/&nbsp;<?php echo $strInitials; ?>:&nbsp;</td>
			<td width="100%"><input type="text" class="textbox"  name="title" size="11" maxlength="10" value="<?php echo htmlspecialchars($contact['title']); ?>" />&nbsp;/&nbsp;<input type="text" class="textbox"  name="initials" size="11" maxlength="10" value="<?php echo htmlspecialchars($contact['initials']); ?>" /></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strSex; ?>:</td>
			<td>
			<?php
			$radiolist = new radio_list('sex', $contact['sex']);
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
			<input type="text" class="textbox"  name="address" size="40" value="<?php if(isset($contact['address'])) echo htmlspecialchars($contact['address']); ?>" maxlength="100">
			</td>
		</tr>

		<tr>
			<td align="right" nowrap>
			<?php echo $strZip; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="zip" size="40" value="<?php if(isset($contact['zip'])) echo htmlspecialchars($contact['zip']); ?>" maxlength="20">
			</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strCity; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="city" size="40" value="<?php if(isset($contact['city'])) echo htmlspecialchars($contact['city']); ?>" maxlength="50">
			</td>
		</tr>
		<tr>
			<td align="right" nowrap>
			<?php echo $strState; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="state" size="40" value="<?php if(isset($contact['state'])) echo htmlspecialchars($contact['state']); ?>" maxlength="50">
			</td>
		</tr>

		<tr>
			<td align="right" nowrap>
			<?php echo $strCountry; ?>:&nbsp;
			</td>
			<td>
			<input type="text" class="textbox"  name="country" size="40" value="<?php if(isset($contact['country'])) echo htmlspecialchars($contact['country']); ?>" maxlength="50">
			</td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>

		<tr>
			<td align="right"><?php echo $ab_comment; ?>:</td>
			<td><input type="text" class="textbox"  name="comment" size="40" value="<?php if(isset($contact['comment'])) echo htmlspecialchars($contact['comment']); ?>" maxlength="50" /></td>
		</tr>
		</table>
		</td>
		<td valign="top">
			<table border="0" cellpadding="0" cellspacing="3">
			<tr>
				<td align="right" nowrap>
				<?php echo $strPhone; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="home_phone" size="40" value="<?php if(isset($contact['home_phone'])) echo htmlspecialchars($contact['home_phone']); ?>" maxlength="20">
				</td>
			</tr>
			<tr>
				<td align="right" nowrap>
				<?php echo $strFax; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="fax" size="40" value="<?php if(isset($contact['fax'])) echo htmlspecialchars($contact['fax']); ?>" maxlength="20">
				</td>
			</tr>
			<tr>
				<td align="right" nowrap>
				<?php echo $strCellular; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="cellular" size="40" value="<?php if(isset($contact['cellular'])) echo htmlspecialchars($contact['cellular']); ?>" maxlength="20">
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
				<td align="right" nowrap>
				<?php echo $strEmail; ?>:&nbsp;
				</td>
				<td class="small">
				<input type="text" class="textbox"  name="email" size="40" value="<?php if(isset($contact['email'])) echo htmlspecialchars($contact['email']); ?>" maxlength="75">
				</td>
			</tr>
			<tr>
				<td colspan="2">&nbsp;</td>
			</tr>
			<tr>
				<td align="right" nowrap>
				<?php echo $strCompany; ?>:&nbsp;
				</td>
				<td nowrap>

				<?php

				if ($task == 'search_company')
				{
					$contact['company_id'] = "0";
					$company_results = new dropbox();
					//$dropbox->add_value(0, $ab_no_company);

					if($ab->search_company(trim(smart_addslashes($_POST['company_name'])), $contact['addressbook_id']))
					{
						while($ab->next_record())
						{

							if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_write')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_read')))
							{
								if ($contact['company_id'] == 0)
								{
									$contact['company_id'] = $ab->f('id');
								}
								$company_results->add_value($ab->f('id'), $ab->f('name'));
							}
						}
					}

					if ($company_results->count_options())
					{
						$company_results->print_dropbox('company_id', 0);
					}else
					{
						echo '<input type="text" class="textbox"  name="company_name" size="40" value="'.$ab_no_company_found.'" maxlength="50" />';
						echo '<input type="hidden"  name="company_id" value="0" />';
					}
				}else
				{
					$contact['company_name'] = isset($contact['company_name']) ? $contact['company_name'] : '';
					echo '<input type="hidden"  name="company_id" value="'.$contact['company_id'].'" />';
					echo '<input type="text" class="textbox"  name="company_name" size="40" value="'.htmlspecialchars($contact['company_name']).'" maxlength="50"';


					if ($contact['company_id'] > 0)
					{
						echo 'disabled ';
					}
					echo '/>';
					if ($contact['company_id'] > 0)
					{
						echo '<a href="javascript:company_properties()" title="'.$ab_edit_company.'">';
						echo '<img src="'.$GO_THEME->images['edit'].'" border="0" align="absmiddle" style="margin: 2px;" /></a>';
					}
				}

				echo '<a href="javascript:search_company()" title="'.$ab_search_company.'">';
				echo '<img src="'.$GO_THEME->images['magnifier'].'" border="0" align="absmiddle" style="margin: 2px;" /></a>';
				?>
				</td>
			</tr>
			<tr>
				<td align="right" nowrap>
				<?php echo $strDepartment; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="department" size="40" value="<?php if(isset($contact['department'])) echo htmlspecialchars($contact['department']); ?>" maxlength="50">
				</td>
			</tr>

			<tr>
				<td align="right" nowrap>
				<?php echo $strFunction; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="function" size="40" value="<?php if(isset($contact['function'])) echo htmlspecialchars($contact['function']); ?>" maxlength="50">
				</td>
			</tr>
			<tr>
				<td align="right" nowrap>
				<?php echo $strWorkphone; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="work_phone" size="40" value="<?php if(isset($contact['work_phone'])) echo htmlspecialchars($contact['work_phone']); ?>" maxlength="20">
				</td>
			</tr>
			<tr>
				<td align="right" nowrap>
				<?php echo $strWorkFax; ?>:&nbsp;
				</td>
				<td>
				<input type="text" class="textbox"  name="work_fax" size="40" value="<?php if(isset($contact['work_fax'])) echo htmlspecialchars($contact['work_fax']); ?>" maxlength="20">
				</td>
			</tr>
			<tr>
				<td colspan="2">&nbsp;</td>
			</tr>
			<?php
			echo '<tr><td align="right" nowrap>'.$contacts_color.':&nbsp;</td><td>';
			$contact['color'] = isset($contact['color']) ? $contact['color'] : '000000';
			$color_selector = new color_selector();
			$color_selector->add_color('000000');
			$color_selector->add_color('FF0000');
			$color_selector->add_color('089600');
			$color_selector->add_color('3300FF');
			$color_selector->add_color('FF00FF');
			$color_selector->add_color('FF9900');
			$color_selector->add_color('996600');
			$color_selector->print_color_selector('color', $contact['color']);
			echo '</td></tr>';

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
			if (!$addressbook || !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $addressbook['acl_write']))
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
					$addressbook_id = $ab->add_addressbook($GO_SECURITY->user_id, $new_ab_name);
					$subscribed_addressbooks->add_value($addressbook_id, $new_ab_name);
				}else
				{
					$contact['addressbook_id'] = $first_writable_ab;
				}
			}

			//print the writable addressbooks
			echo '<tr><td align="right">'.$ab_addressbook.':</td><td>';
			$subscribed_addressbooks->print_dropbox('addressbook_id', $addressbook_id, 'onchange="javascript:document.forms[0].submit()"');
			echo '</td></tr>';
			if ($ab->get_groups($addressbook_id) > 0)
			{
				echo '<tr><td align="right" nowrap>'.$contacts_group.':&nbsp;</td><td>';
				$dropbox = new dropbox();
				$dropbox->add_value('0',$contacts_other);
				$dropbox->add_sql_data('ab','id','name');
				$dropbox->print_dropbox('group_id',$contact['group_id']);
				echo '</td></tr>';
			}else
			{
				echo '<input type="hidden" name="group_id" value="0" />';
			}
			?>
			</table>
			</td>
		</tr>
		<tr heigth="25">
			<td colspan="2">
			<br />
			<?php
			if (isset($return_to))
			{
				$button = new button($cmdOk, 'javascript:ok_add()');
				echo '&nbsp;&nbsp;';
			}
			$button = new button($cmdApply, 'javascript:apply_add()');
			echo '&nbsp;&nbsp;';
			if ($contact["source_id"] > 0 && $contact_id > 0)
			{
				$button = new button($cmdUpdate, 'javascript:update()');
				echo '&nbsp;&nbsp;';
			}
			if (isset($return_to))
			{
				$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
			}
			?>
			</td>
        </tr>
        </table>
        </td>
</tr>
</table>
<script type="text/javascript" language="javascript">

function company_properties()
{
	document.location='<?php echo $GO_MODULES->url; ?>company.php?addressbook_id=<?php echo $addressbook_id; ?>&company_id='+document.forms[0].company_id.value+'&return_to=<?php echo urlencode($link_back); ?>';
}

function search_company()
{
	<?php
	if ($task != 'search_company')
	{
		?>
		if (document.forms[0].company_name.disabled)
		{
			document.forms[0].company_name.disabled = false;
		}else
		{
	<?php
	}
	?>
	document.forms[0].task.value='<?php if ($task != 'search_company') echo 'search_company'; ?>';
	document.forms[0].submit();
	<?php
	if ($task != 'search_company')
	{
		echo '}';
	}
	?>

}

function ok_add()
{
	document.forms[0].close.value='true';
	document.forms[0].task.value='save';
	document.forms[0].submit();
}

function apply_add()
{
	document.forms[0].task.value='save';
	document.forms[0].submit();
}
function update()
{
	document.add.task.value = "update";
	document.add.submit();
}

document.forms[0].first_name.focus();
</script>
