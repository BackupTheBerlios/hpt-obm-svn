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

require("../Group-Office.php");

$GO_SECURITY->authenticate();
require($GO_LANGUAGE->get_language_file('addressbook'));
$profile = $GO_USERS->get_user($_REQUEST['id']);

if ($GO_SECURITY->user_id != $_REQUEST['id'])
{
	if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id,$profile["acl_id"]))
	{
		Header("Location: ".$GO_CONFIG->host."error_docs/403.php");
		exit();
	}
}

require($GO_THEME->theme_path."header.inc");
if (!$profile)
{
    echo "<p class=\"Error\">".$strDataError."</p>";
	exit();
}

$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];;
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];;
?>
<table border="0" cellpadding="10" cellspacing="0" width="100%">
<tr>
	<td>
	<table border="0" cellpadding="" cellspacing="3" width="100%">
	<tr>
		<td height="40" colspan="2"><h1><?php echo $user_profile; ?></h1></td>
	</tr>
	<tr>
		<td valign="top" width="50%">
		<table border="0" class="normal" cellpadding="0" cellspacing="3" width="100%">

		<tr>
			<td align="right" nowrap><i><?php echo $strLastName; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["last_name"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strFirstName; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["first_name"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strInitials; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["initials"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strSex; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($strSexes[$profile["sex"]]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strBirthday; ?></i>:</td>
			<td>
			<?php
				$birthday = $profile['birthday'] > 0 ? db_date_to_date($profile['birthday']) : '';			
				echo empty_to_stripe($birthday);
			?>
			</td>
		</tr>
		<tr><td colspan="2">&nbsp;</td></tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strAddress; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["address"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strZip; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["zip"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strCity; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["city"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strState; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["state"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strCountry; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["country"]); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strEmail; ?>:</i>&nbsp;</td>
			<td><?php echo mail_to(empty_to_stripe($profile["email"])); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strPhone; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["home_phone"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strFax; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["fax"]); ?></td>
		</tr>


		<tr>
			<td align="right" nowrap><i><?php echo $strCellular; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["cellular"]); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<?php
		if (isset($contact_id))
		{
			echo '<tr><td valign="top"><i>'.$strComments.':</i></td><td>'.text_to_html($profile["comments"]).'</td></tr>';
		}
		?>
		</table>
		</td>
		<td valign="top" width="50%">
		<table border="0" class="normal" cellpadding="0" cellspacing="3" width="100%">
		<tr>
			<td align="right" nowrap><i><?php echo $strCompany; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["company"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strDepartment; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["department"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strFunction; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["function"]); ?></td>
		</tr>
		<tr><td colspan="2">&nbsp;</td></tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strAddress; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["work_address"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strZip; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["work_zip"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strCity; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["work_city"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strState; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["work_state"]); ?></td>
		</tr>

		<tr>
			<td align="right" nowrap><i><?php echo $strCountry; ?>:</i>&nbsp;</td>
			<td width="100%"><?php echo empty_to_stripe($profile["work_country"]); ?></td>
		</tr>
		<tr>
			<td colspan="2">&nbsp;</td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strPhone; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["work_phone"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strFax; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe($profile["work_fax"]); ?></td>
		</tr>
		<tr>
			<td align="right" nowrap><i><?php echo $strHomepage; ?>:</i>&nbsp;</td>
			<td><?php echo empty_to_stripe(text_to_html($profile["homepage"])); ?></td>
		</tr>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<br />
		<?php
		$ab_module = $GO_MODULES->get_module('addressbook');

	
		if ($ab_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
		{
			require_once($ab_module['path'].'classes/addressbook.class.inc');
			$ab = new addressbook();
			if (!$ab->user_is_contact($GO_SECURITY->user_id, $_REQUEST['id']))
			{
				$button = new button($cmdAdd, "document.location='".$ab_module['url']."contact.php?user_id=".$_REQUEST['id']."&return_to=".urlencode($link_back)."';");
				echo '&nbsp;&nbsp;';
			}
		}
		$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
		?>
		</td>
	</tr>
	</table>
	</td>
</tr>
</table>
<?php
require($GO_THEME->theme_path."footer.inc");
?>

