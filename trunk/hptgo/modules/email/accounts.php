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

require("../../Group-Office.php");

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('email');

require($GO_CONFIG->class_path."imap.class.inc");
require($GO_MODULES->class_path."email.class.inc");
require($GO_LANGUAGE->get_language_file('email'));
$mail = new imap();
$email = new email();


$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

//delete accounts if requested
if ($task == 'save_account')
{
	$task = 'account';
	$mbroot = isset($_POST['mbroot']) ? $_POST['mbroot'] : '';
	if ($_POST['name'] == "" || $_POST['mail_address'] == "" || $_POST['port'] == "" || $_POST['user'] == "" || $_POST['pass'] == "" || $_POST['host'] == "")
	{
		$feedback = $error_missing_field;
	}else
	{
		$sent = $_POST['type'] == 'pop3' ? '' : $_POST['sent'];
		$draft = $_POST['type'] == 'pop3' ? '' : $_POST['draft'];
		$spam = $_POST['type'] == 'pop3' ? '' : $_POST['spam'];
		$trash = $_POST['type'] == 'pop3' ? '' : $_POST['trash'];

		$auto_check = isset($_POST['auto_check']) ? '1' : '0';
		if (isset($_POST['account_id']))
		{
			if(!$email->update_account($_POST['account_id'], $_POST['type'], $_POST['host'], $_POST['port'], $mbroot, $_POST['user'], $_POST['pass'], $_POST['name'], $_POST['mail_address'], $_POST['signature'], $sent, $spam, $trash, $draft, $auto_check))
			{
				$feedback = '<p class="Error">'.$ml_connect_failed.' \''.$_POST['host'].'\' '.$ml_at_port.': '.$_POST['port'].'</p>';
				$feedback .= '<p class="Error">'.$email->last_error.'</p>';
			}
		}else
		{
			if(!$email_id = $email->add_account($GO_SECURITY->user_id, $_POST['type'], $_POST['host'], $_POST['port'], $mbroot, $_POST['user'], $_POST['pass'], $_POST['name'], $_POST['mail_address'], $_POST['signature'], $sent, $spam, $trash, $draft, $auto_check))
			{
				$feedback = '<p class="Error">'.$ml_connect_failed.' \''.$_POST['host'].'\' '.$ml_at_port.': '.$_POST['port'].'</p>';
				$feedback .= '<p class="Error">'.$email->last_error.'</p>';
			}
		}
	}
}

require($GO_THEME->theme_path."header.inc");

echo '<form method="POST" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
echo '<input type="hidden" name="delete_account_id" />';

$tabtable = new tabtable('accounts_list', $ml_your_accounts, '600', '300', '100', '', true);
$tabtable->print_head();


if (!function_exists('imap_open'))
{
	echo 'Error: the imap extension for PHP is not installed';
}else
{
	if(isset($_REQUEST['delete_account_id']) && $_REQUEST['delete_account_id'] > 0)
	{
		$delete_account_id = smart_addslashes($_REQUEST['delete_account_id']);
		if (!$email->delete_account($GO_SECURITY->user_id, $delete_account_id ))
		{
			echo $strDeleteError;
		}
	}

	if ($_SERVER['REQUEST_METHOD'] == "POST" && isset($_POST['account_id']) && $_REQUEST['delete_account_id'] < 1)
	{
		$email->set_as_default($_POST['account_id'], $GO_SECURITY->user_id);
	}
	?>
	<br />
	<a href="account.php?return_to=<?php echo urlencode($link_back); ?>" class="normal"><?php echo $ml_new_account; ?></a>
	<br />
	<br />
	<table border="0" cellpadding="10" cellspacing="0">
	<tr>
		<td>
		<table border="0" cellpadding="4" cellspacing="0" width="100%">
		<?php
		$count = $email->get_accounts($GO_SECURITY->user_id);

		if ($count > 0)
		{
			echo '<tr><td align="right"><h3>'.$strDefault.'</h3></td>';
			echo '<td><h3>'.$strHost.'</h3></td>';
			echo '<td><h3>'.$strEmail.'</h3></td>';
			echo '<td>&nbsp;</td></tr>';

			while ($email->next_record())
			{
				if ($email->f("standard") == "1")
				{
					$checked = "checked";
				}else
				{
					$checked = "";
				}
				echo '<tr>';
				echo '<td align="right"><input type="radio" onclick="javascript:document.forms[0].submit()" name="account_id" value="'.$email->f("id").'" '.$checked.' /></td>';
				echo '<td>'.$email->f("host").'</a></td>';
				echo '<td>'.$email->f('email').'</td>';
				echo '<td><a href="account.php?account_id='.$email->f('id').'&return_to='.urlencode($link_back).'" title="'.$strEdit.' '.$email->f('host').'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
				echo "<td><a href=\"javascript:delete_account('".$email->f("id")."','".smart_addslashes($strDeletePrefix."'".$email->f("host")."'".$strDeleteSuffix)."')\" title=\"".$strDeleteItem." '".$email->f("host")."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
				echo '</tr>';
			}
		}else
		{
			echo "<tr><td>".$ml_no_accounts."</td></tr>";
		}
		?>
		</table>
		<br />
		<?php
		$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
		?>
		</td>
	</tr>
	</table>

<?php
	$tabtable->print_foot();
	echo '</form>';
}
?>
<script type="text/javascript" language="javascript">
function delete_account(account_id, message)
{
	if (confirm(unescape(message)))
	{
		document.forms[0].delete_account_id.value = account_id;
		document.forms[0].submit();
	}
}
</script>

<?php
require($GO_THEME->theme_path."footer.inc");
?>
