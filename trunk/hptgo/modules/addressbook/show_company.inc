<table border=0 cellpadding="2" cellspacing="0">
<tr>
	<td valign="top">
	<table border="0" cellpadding="0" cellspacing="3">
	<tr>
		<td align="right" nowrap><?php echo $strParentCompany;?>:</td>
		<td><?php echo $ab->parent_companies_urls($company_id); ?>&nbsp;</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strName; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['name']); ?>
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strAddress; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['address']); ?>
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strZip; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['zip']); ?>
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strCity; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['city']); ?>
		</td>
	</tr>


	<tr>
		<td align="right" nowrap>
		<?php echo $strState; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['state']); ?>
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strCountry; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['country']); ?>
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
		<?php echo empty_to_stripe($company['phone']); ?>
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strFax; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['fax']); ?>
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strEmail; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['email']); ?>
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $strHomepage; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['homepage']); ?>
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $ab_bank_no; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['bank_no']); ?>
		</td>
	</tr>
	<tr>
		<td align="right" nowrap>
		<?php echo $ab_vat_no; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['vat_no']); ?>
		</td>
	</tr>
	</table>
	</td>
</tr>
<tr heigth="25">
	<td colspan="2">
	<br />
	<?php
	$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
	?>
	</td>
</tr>
</table>
