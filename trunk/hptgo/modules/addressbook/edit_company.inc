<?php if (isset($feedback)) echo $feedback; ?>
<table border=0 cellpadding="2" cellspacing="0">
<tr>
	<td valign="top">
	<table border="0" cellpadding="0" cellspacing="3">
	<tr>
		<td align="right" nowrap>
		<?php echo $strName; ?>*:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="name" size="40" value="<?php echo htmlspecialchars($company['name']); ?>" maxlength="50">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strAddress; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="address" size="40" value="<?php echo htmlspecialchars($company['address']); ?>" maxlength="100">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strZip; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="zip" size="40" value="<?php echo htmlspecialchars($company['zip']); ?>" maxlength="10">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strCity; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="city" size="40" value="<?php echo htmlspecialchars($company['city']); ?>" maxlength="50">
		</td>
	</tr>


	<tr>
		<td align="right" nowrap>
		<?php echo $strState; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="state" size="40" value="<?php echo htmlspecialchars($company['state']); ?>" maxlength="50">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strCountry; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="country" size="40" value="<?php echo htmlspecialchars($company['country']); ?>" maxlength="50">
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
		<input type="text" class="textbox"  name="phone" size="40" value="<?php echo htmlspecialchars($company['phone']); ?>" maxlength="20">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strFax; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="fax" size="40" value="<?php echo htmlspecialchars($company['fax']); ?>" maxlength="20">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strEmail; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="email" size="40" value="<?php echo htmlspecialchars($company['email']); ?>" maxlength="75">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strHomepage; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="homepage" size="40" value="<?php echo htmlspecialchars($company['homepage']); ?>" maxlength="100">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $ab_bank_no; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="bank_no" size="40" value="<?php echo htmlspecialchars($company['bank_no']); ?>" maxlength="20">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $ab_vat_no; ?>:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="vat_no" size="40" value="<?php echo htmlspecialchars($company['vat_no']); ?>" maxlength="30">
		</td>
	</tr>
	<tr>
		<td colspan="2">&nbsp;</td>
	</tr>
	<?php
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
			$addressbook_id = $ab->add_addressbook($GO_SECURITY->user_id, $new_ab_name);
			$subscribed_addressbooks->add_value($addressbook_id, $new_ab_name);
		}else
		{
			$addressbook_id = $first_writable_ab;
		}
	}

	//print the writable addressbooks
	echo '<tr><td align="right">'.$ab_addressbook.':</td><td>';
	$subscribed_addressbooks->print_dropbox('addressbook_id', $addressbook_id);
	echo '</td></tr>';
	?>
	</table>
	</td>
</tr>
<tr heigth="25">
	<td colspan="2">
	<br />
	<?php
	$button = new button($cmdOk, "javascript:_save('save_company','true')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdApply, "javascript:_save('save_company','false')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
	?>
	</td>
</tr>
</table>
<script type="text/javascript" language="javascript">

document.forms[0].name.focus();

function _save(task, close)
{
	document.forms[0].task.value = task;
	document.forms[0].close.value = close;
	document.forms[0].submit();
}
</script>
