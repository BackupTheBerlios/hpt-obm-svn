<table border=0 cellpadding="2" cellspacing="0">
<tr>
	<td valign="top">
	<table border="0" cellpadding="0" cellspacing="3">
	<tr>
		<td align="right" nowrap><?php echo $strParentCompany;?>:</td>
		<td><?php echo empty_to_stripe($ab->parent_companies_urls($company_id)); ?>&nbsp;</td>
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
		<?php echo $strShortCompanyName; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['shortname']); ?>
		</td>
	</tr>

	<tr>
		<td align="right" nowrap>
		<?php echo $strEnglishCompanyName; ?>:&nbsp;
		</td>
		<td>
		<?php echo empty_to_stripe($company['engname']); ?>
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
	
	<td width="10%"></td>
	<td valign="top">
		<table width="100%"  border="0" cellspacing="3" cellpadding="0">
			<tr nowrap>
				<td align="right" width="1%" nowrap><?php echo $strRelationDate; ?>&nbsp;:&nbsp;</td>
				<td nowrap>&nbsp;
<?php
	$company['relation_date'] = db_date_to_date($company['relation_date']);
		
	if ($company['relation_date']=="00-00-0000")
		$company['relation_date'] = "";
			
	echo empty_to_stripe($company['relation_date']);
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
	$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
	?>
	</td>
</tr>
</table>
