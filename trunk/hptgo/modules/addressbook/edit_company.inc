<?php if (isset($feedback)) echo $feedback; ?>
<table border=0 cellpadding="2" cellspacing="0">
<tr>
	<td valign="top">
	<table border="0" cellpadding="0" cellspacing="3">
<?php //if ($company_id > 0) { /* When adding new company, addressbook is not specified yet, so disable this thing.*/?>
		<?php //} ?>

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
		<?php echo $strShortCompanyName; ?>*:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="shortname" size="40" value="<?php echo htmlspecialchars($company['shortname']); ?>" maxlength="50">
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strEnglishCompanyName; ?>*:&nbsp;
		</td>
		<td>
		<input type="text" class="textbox"  name="engname" size="40" value="<?php echo htmlspecialchars($company['engname']); ?>" maxlength="50">
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strParentCompany; ?>:&nbsp;
		</td>
		<td><?php $parent_dropbox->print_dropbox('parent_id',$company['parent_id'],'',false,10,230);?></td>
	</tr>
	<tr><td>&nbsp;</td></tr>
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
	//print the writable addressbooks
	echo '<tr><td align="right">'.$ab_addressbook.':</td><td>';
	$subscribed_addressbooks->print_dropbox('addressbook_id', $subscribed_addressbook_id,' onchange=document.forms[0].submit()');
	echo '</td></tr>';
	?>
	</table>
	</td>
	<td width="10%"></td>
	<td valign="top">
		<table width="100%"  border="0" cellspacing="3" cellpadding="0">
			<tr nowrap>
				<td align="right" width="1%" nowrap><?php echo $strRelationDate; ?>&nbsp;:&nbsp;</td>
				<td nowrap>&nbsp;
<?php
	if ( empty($task) && $company['relation_date'] ) 
		$company['relation_date'] = db_date_to_date($company['relation_date']);
		
	if ($company['relation_date']==0)
		$company['relation_date'] = "";
			
	$datepicker->print_date_picker('relation_date', $_SESSION['GO_SESSION']['date_format'], $company['relation_date']);
?>
				</td>
			</tr>
			<?php account_manager($company['acl_write']);?>
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
