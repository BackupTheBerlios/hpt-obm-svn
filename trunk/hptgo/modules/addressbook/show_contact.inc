<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<table border="0" cellpadding="0" cellspacing="20">
<tr>
	<td>
	<table border="0" cellpadding="" cellspacing="3" width="100%">
	<tr>
		<td valign="top" width="200">
		<table border="0" class="normal" cellpadding="0" cellspacing="3" width="100%">

		<tr>
			<td align="right" nowrap><?php echo $strFirstName; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["first_name"]); ?></td>
		</tr>
		<tr heigth="25">
			<td align="right" nowrap><?php echo $strMiddleName; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["middle_name"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strLastName; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["last_name"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strInitials; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["initials"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strSex; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($strSexes[$contact['sex']]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strBirthday; ?>:&nbsp;</td>
			<?php
			$birthday = $contact['birthday'] > 0 ? db_date_to_date($contact['birthday']) : '';
			?>
			<td width="100%"><?php echo empty_to_stripe($birthday); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strAddress; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["address"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><?php echo $strZip; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["zip"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><?php echo $strCity; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["city"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><?php echo $strState; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["state"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><?php echo $strCountry; ?>:&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($contact["country"]); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td valign="top"><?php echo $ab_comment; ?>:</td>
			<td><?php echo $contact["comment"]; ?></td>
		</tr>
		</table>
		</td>
		<td valign="top" width="200">
		<table border="0" class="normal" cellpadding="0" cellspacing="3" width="100%">
		<tr>
			<td align="right" nowrap><?php echo $strEmail; ?>:&nbsp;</td>
			<td><?php echo mail_to(empty_to_stripe($contact["email"])); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strPhone; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["home_phone"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strFax; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["fax"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strCellular; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["cellular"]); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strCompany; ?>:&nbsp;</td>
			<td><?php
			if ($contact['company_id'] > 0)
			{
				echo '<a class="normal" href="'.$GO_MODULES->url.'company.php?addressbook_id='.$addressbook_id.'&company_id='.$contact['company_id'].'&return_to='.urlencode($link_back).'">'.$contact['company_name'].'</a>';
			}else
			{
				echo '-';
			}
			?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strDepartment; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["department"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strFunction; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["function"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strWorkphone; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["work_phone"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><?php echo $strWorkFax; ?>:&nbsp;</td>
			<td><?php echo empty_to_stripe($contact["work_fax"]); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<br />
		<?php
		if ($write_perm)
		{
			$button = new button($contacts_edit, "document.forms[0].task.value='edit';document.forms[0].submit();");
			echo '&nbsp;&nbsp;';
		}
		$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
		?>
		</td>
	</tr>
	</table>
	</td>
</tr>
</table>
