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
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ?
							$_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ?
							$_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;

$account = $email->get_account($account_id);
if ($account && $mail->open($account['host'], $account['type'],
			$account['port'],$account['username'],
			$GO_CRYPTO->decrypt($account['password'])))
{
	if ($task == 'create_folder')
	{
		$name = smartstrip(trim($_POST['name']));

		if ($name == '')
		{
			$feedback = '<p class="Error">'.$error_missing_field.'</p>';
		}else
		{
			$parent_folder_name = isset($_POST['parent_folder_name']) ?
					smartstrip($_POST['parent_folder_name']) : '';

			if ($parent_folder_name != '' &&
					substr($parent_folder_name, -1) != $_POST['delimiter'])
			{
				$parent_folder_name .= $_POST['delimiter'];
			}
			$mail->create_folder($parent_folder_name.$name);
		}
	}

	if (isset($_REQUEST['delete_folder']))
	{
		$delete_folder = smartstrip($_REQUEST['delete_folder']);
		if ($mail->delete_folder($delete_folder, $account['mbroot']))
		{
			/*
				(cyrus imap) if folder still exists then don't delete it from the
				database,
				because it contains at least one child mailbox
			*/
			if (!is_array($mail->get_mailboxes($delete_folder)))
			{
				$email->delete_folder($account['id'], addslashes($delete_folder));
			}
		}
	}

	$edit_name = isset($_REQUEST['edit_name']) ?
									smart_addslashes($_REQUEST['edit_name']) : '';

	if ($task =='save')
	{
		$subscribed = $mail->get_subscribed();
		$subscribed_names = array();
		if (isset($_POST['use']))
		{
			while($mailbox = array_shift($subscribed))
			{
				$subscribed_names[] = $mailbox['name'];

				$search_name = get_magic_quotes_gpc() ? addslashes($mailbox['name']) : $mailbox['name'];
				if (!in_array($search_name, $_POST['use']) &&
						$mailbox['name'] != 'INBOX')
				{
					if($mail->unsubscribe($mailbox['name']))
					{
						$email->unsubscribe($account['id'], addslashes($mailbox['name']));
					}
				}
			}

			for ($i=0;$i<count($_POST['use']);$i++)
			{
				$must_be_subscribed = smartstrip($_POST['use'][$i]);

				if (!in_array($must_be_subscribed, $subscribed_names)
							&& $must_be_subscribed != "INBOX")
				{
					if($mail->subscribe($must_be_subscribed))
					{
						$email->subscribe($account['id'], addslashes($must_be_subscribed));
					}
				}
			}
		}else
		{
			while($mailbox = array_shift($subscribed))
			{
				if($mail->unsubscribe($mailbox['name']))
				{
					$email->unsubscribe($account['id'], addslashes($mailbox['name']));
				}
			}
		}

		$sent = isset($_POST['sent']) ? smart_addslashes($_POST['sent']) : '';
		$spam = isset($_POST['spam']) ? smart_addslashes($_POST['spam']) : '';
		$trash = isset($_POST['trash']) ? smart_addslashes($_POST['trash']) : '';

		$email->update_folders($account['id'], $sent, $spam, $trash);

		if (isset($_POST['new_name']))
		{
			$new_name = smart_addslashes(trim($_POST['new_name']));
			$old_name = smart_addslashes(trim($_POST['old_name']));
			$location = smart_addslashes(trim($_POST['location']));
			if ($new_name == '')
			{
				$feedback = '<p class="Error">'.$error_missing_field.'</p>';
			}else
			{
				if ($mail->rename_folder($old_name, $location.$new_name))
				{
					$email->rename_folder($account_id, $old_name, $location.$new_name);
				}
			}
		}
	}

	if (isset($_POST['close']) && $_POST['close'] == 'true')
	{
		header('Location: '.$return_to);
		exit();
	}

	require($GO_THEME->theme_path."header.inc");

	echo '<form method="POST" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
	echo '<input type="hidden" name="task" value="" />';
	echo '<input type="hidden" name="close" value="false" />';
	echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
	echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
	echo '<input type="hidden" name="account_id" value="'.$account_id.'" />';

	$tabtable = new tabtable('folders_list', $ml_folders, '100%', '300', '100',
													'', true);
	$tabtable->print_head();


	//synchronise Group-Office with the IMAP server
	$subscribed = $mail->get_subscribed($account['mbroot']);
	$mailboxes = $mail->get_mailboxes($account['mbroot']);
	$email->synchronise($account['id'], $mailboxes, $subscribed);

	//get all the folders and the subscribed folders as an array
	//and add all missing subscribed folders to Group-Office
	$mailboxes = array();
	$mailboxes = $mail->get_mailboxes($account['mbroot']);

	//get all the Group-Office folders as an array
	$account = $email->get_account($account['id']);

	$email->get_all_folders($account['id']);
	$go_mailboxes = array();

	while ($email->next_record())
	{
		$go_mailboxes[] = $email->Record;
		$delimiter = $email->f('delimiter');
	}
	$mcount = count($go_mailboxes);

	?>
	<table border="0" cellpadding="0" cellspacing="8" class="normal">
	<tr>
		<td>
		<?php if (isset($feedback)) echo $feedback; ?>
		<table border="0" cellpadding="2" cellspacing="0">
		<tr>
			<td colspan="2">
			<table border="0">
			<tr>
				<td nowrap><?php echo $ml_sent_items; ?>:</td>
				<td>
				<?php
				$dropbox=new dropbox();
				$dropbox->add_value('', $ml_disable);
				for ($i=0;$i<$mcount;$i++)
				{
					if ($go_mailboxes[$i]['attributes'] != LATT_NOSELECT)
					{
						$dropbox->add_value($go_mailboxes[$i]['name'], 
								str_replace('INBOX'.$go_mailboxes[$i]['delimiter'], '', $go_mailboxes[$i]['name']));
					}
				}
				$dropbox->print_dropbox('sent', $account['sent']);
				?>
				</td>
			</tr>
			<tr>
				<td nowrap><?php echo $ml_spam; ?>:</td>
				<td>
				<?php
				$dropbox->print_dropbox('spam', $account['spam']);
				?>
				</td>
			</tr>
			<tr>
				<td nowrap><?php echo $ml_trash; ?>:</td>
				<td>
				<?php
				$dropbox->print_dropbox('trash', $account['trash']);
				?>
				</td>
			</tr>
			</table>
			</td>
		</tr>
		<tr>
			<td>&nbsp;</td>
		</tr>

		<tr>
			<td>
			<?php echo $em_new_folder; ?>:
			</td>
			<td>
			<?php $name = isset($_POST['name']) ? htmlspecialchars(smartstrip($_POST['name'])) : ''; ?>
			<input type="text" class="textbox" name="name" value="<?php echo $name; ?>" maxlength="100" size="30" />
			</td>
			<?php
			$delimiter = isset($delimiter) ? $delimiter : '/';
			echo '<input type="hidden" name="delimiter" value="'.$delimiter.'" />';
			echo '<td>'.$ml_inside.'</td>';
			echo '<td>';
			$parent_folder_name = isset($parent_folder_name) ? $parent_folder_name : '';
			$dropbox=new dropbox();
			$dropbox->add_value($account['mbroot'],$ml_root_mailbox);
			for ($i=0;$i<$mcount;$i++)
			{
				if ($go_mailboxes[$i]['attributes'] != LATT_NOINFERIORS)
				{
					$dropbox->add_value($go_mailboxes[$i]['name'], 
							str_replace('INBOX'.$go_mailboxes[$i]['delimiter'], '', $go_mailboxes[$i]['name']));
				}
			}		
			$dropbox->print_dropbox('parent_folder_name',$parent_folder_name);
			echo '</td>';
			?>
			<td>
			<?php
			$button = new button($cmdOk, "javascript:_save('create_folder', 'false')");
			?>
			</td>
		</tr>
		</table>
		<br />
		<table border="0" cellpadding="0" cellspacing="0">
		<tr>
			<td><h3><?php echo $strName; ?>&nbsp;&nbsp;</h3></td>
			<td><h3><?php echo $ml_use; ?>&nbsp;&nbsp;</h3></td>
			<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td><img border="0" src="<?php echo $GO_THEME->images['newmail']; ?>" /></td>
			<td colspan="5">&nbsp;</td>
		</tr>
		<?php

		$fm_image['folder'] = '<img src="'.$GO_THEME->images['folderclosed'].'" border="0" height="22" width="24" />';
		$fm_image['emptynode'] = '<img src="'.$GO_THEME->images['emptynode'].'" border="0" height="22" width="16" />';
		$fm_image['emptylastnode'] = '<img src="'.$GO_THEME->images['emptylastnode'].'" border="0" height="22" width="16" />';
		$fm_image['blank'] = '<img src="'.$GO_THEME->images['blank'].'" border="0" height="22" width="16" />';
		$fm_image['vertline'] = '<img src="'.$GO_THEME->images['vertline'].'" border="0" height="22" width="16" />';


		$sublevel = 0; // number of levels deep
		$lastfolder = '';
		$prefix = array();
		for ($a=0;$a<$mcount;$a++)
		{
			$sublevel = substr_count($go_mailboxes[$a]['name'], $go_mailboxes[$a]['delimiter']) + 1;

			if ($sublevel > 1)
			{
				$folder_name = substr($go_mailboxes[$a]['name'],strrpos($go_mailboxes[$a]['name'], $go_mailboxes[$a]['delimiter'])+1);
			}else
			{
				$folder_name = $go_mailboxes[$a]['name'];
			}

			//hide dot files
			if (substr($folder_name,0,1) != '.')
			{
				//check if this is the last node on this level
				$node = $fm_image['emptylastnode'];
				$prefix[$sublevel] = $fm_image['blank'];
				if (isset($go_mailboxes[$a+1]['name']) && strtolower($go_mailboxes[$a+1]['name']) != 'inbox')
				{
					for ($i=$a+1;$i<$mcount;$i++)
					{
						$next_sublevel = substr_count($go_mailboxes[$i]['name'], $go_mailboxes[$a]['delimiter']) + 1;

						if ($next_sublevel < $sublevel)
						{
							$prefix[$sublevel] = $fm_image['blank'];
							$node = $fm_image['emptylastnode'];
							break;
						}

						if ($next_sublevel == $sublevel)
						{
							$prefix[$sublevel] = $fm_image['vertline'];
							$node = $fm_image['emptynode'];
							break;
						}

					}
				}

				$image = $GO_THEME->images['folder'];

				echo '<tr><td><table border="0" cellpadding="0" cellspacing="0">';
				echo '<tr><td>';
				for ($i=1;$i<$sublevel;$i++)
				{
					if(array_key_exists($i, $prefix)) echo $prefix[$i];
				}

				echo $node;
				echo '</td><td><img src="'.$image.'" border="0" /></td>';
				if ($edit_name == $go_mailboxes[$a]['name'])
				{
					if ($pos = strrpos($go_mailboxes[$a]['name'], $go_mailboxes[$a]['delimiter']))
					{
						$location = substr($go_mailboxes[$a]['name'],0,$pos+1);
					}else
					{
						$location = '';
					}
					echo '<input type="hidden" name="location" value="'.$location.'" />';
					echo '<td>&nbsp;<input class="textbox" type="text" name="new_name" value="'.$folder_name.'" /><input type="hidden" name="old_name" value="'.$go_mailboxes[$a]['name'].'" /></td></tr>';
				}else
				{
					echo "<td>&nbsp;".$folder_name."</td></tr>";
				}

				if ($go_mailboxes[$a]['attributes']&LATT_NOSELECT)
					$disabled = ' disabled';
				else
					$disabled = '';

				echo '</table></td>';
				if ($go_mailboxes[$a]['subscribed'] == 1)
					$checked = 'checked';
				else
					$checked = '';

				echo '<td align="center"><input type="checkbox" name="use[]" value="'.$go_mailboxes[$a]['name'].'" '.$checked.' /></td>';
				echo '<td><a href="'.$_SERVER['PHP_SELF'].'?account_id='.$account_id.'&return_to='.urlencode($return_to).'&link_back='.urlencode($link_back).'&edit_name='.urlencode($go_mailboxes[$a]['name']).'" title="'.$strEdit.' '.$go_mailboxes[$a]['name'].'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
				echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?account_id=".$account_id."&return_to=".urlencode($return_to)."&delete_folder=".urlencode($go_mailboxes[$a]['name'])."\",\"".rawurlencode($strDeletePrefix."'".$folder_name."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$go_mailboxes[$a]['name']."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>";
				echo '</tr>';
				$lastfolder = $go_mailboxes[$a]['name'];
			}
		}


		echo '<tr><td colspan="6"><br />';
		$button = new button($cmdOk, "javascript:_save('save', 'true');");
		echo '&nbsp;&nbsp;';
		$button = new button($cmdApply, "javascript:_save('save', 'false');");
		echo '&nbsp;&nbsp;';
		if ($edit_name != '')
		{
			$button = new button($cmdCancel,'javascript:document.location=\''.$link_back.'\'');
		}else
		{
			$button = new button($cmdClose,'javascript:document.location=\''.$return_to.'\'');
		}
		echo '</td></tr>';
		echo "</td></tr></table>";
		echo "</td></tr></table>";
		$mail->close();
}else
{
	require($GO_THEME->theme_path."header.inc");

	echo '<form method="POST" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
	echo '<input type="hidden" name="task" value="" />';
	echo '<input type="hidden" name="close" value="false" />';
	echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
	echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
	echo '<input type="hidden" name="account_id" value="'.$account_id.'" />';

	$tabtable = new tabtable('folders_list', $ml_folders, '600', '300', '100', '', true);
	$tabtable->print_head();

	echo '<br /><p class="Error">'.$ml_connect_failed.' \''.$account['host'].'\' '.$ml_at_port.': '.$account['port'].'</p>';
	echo '<p class="Error">'.imap_last_error().'</p><br />'.$ml_solve_error;
	unset($_SESSION['email_id']);
}
?>
<script type="text/javascript" language="javascript">
function _save(task, close)
{
	document.forms[0].task.value = task;
	document.forms[0].close.value = close;
	document.forms[0].submit();
}
</script>

<?php
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
