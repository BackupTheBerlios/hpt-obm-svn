<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

//display number of unread e-mail messages
$email_module = $GO_MODULES->get_module('email');
if ($email_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $email_module['acl_read']) ||
		$GO_SECURITY->has_permission($GO_SECURITY->user_id, $email_module['acl_write'])))
{			
	$_SESSION['notified_new_mail'] = isset($_SESSION['notified_new_mail']) ? $_SESSION['notified_new_mail'] : 0;
	$_SESSION['new_mail']=0;
	require_once($email_module['class_path'].'email.class.inc');
	require_once($GO_CONFIG->class_path.'imap.class.inc');
	$imap = new imap();
	$email1 = new email();
	$email2 = new email();
$checked_accounts = 0;
	if ($email1->get_accounts($GO_SECURITY->user_id))
	{
		while($email1->next_record())
		{
			$account = $email1->Record;
	
			if ($imap->open($account['host'], $account['type'],$account['port'],$account['username'],$GO_CRYPTO->decrypt($account['password']), 'INBOX', 0, $account['use_ssl'], $account['novalidate_cert']))
			{
				if ($account['type'] == 'imap')
				{
					$checked_accounts++;
					$status = $imap->status('INBOX');
					if ($status->unseen > 0)
					{
						$_SESSION['new_mail'] += $status->unseen;
					}
	
					$email2->get_folders($email1->f('id'));
					while($email2->next_record())
					{
						if ($email2->f('name') != 'INBOX')
						{
							$status = $imap->status($email2->f('name'));
							if ($status->unseen > 0)
							{
								$_SESSION['new_mail'] += $status->unseen;
							}
						}
					}
				}
			}else
			{
				$email2->disable_auto_check($account['id']);
				echo '<script language="javascript" type="text/javascript">alert("'.$account['host'].' '.$ml_host_unreachable.'");</script>';
			}
		}
		if ($checked_accounts > 0)
		{
			echo 	'<table border="0" style="margin-top: 10px"><tr><td valign="top">'.
					'<img style="margin-right: 10px;" width="32" height="32" src="'.$GO_THEME->images['ml_inbox'].
					'" border="0" /></td><td>'.
					'<h2><a href="'.$email_module['url'].'">'.
					$lang_modules['email'].'</a></h2></td></tr><tr><td></td><td><a href="'.$email_module['url'].'">';

			switch($_SESSION['new_mail'])
			{
				case 0:
					echo $sum_no_new_mail;
				break;
				case 1:
					echo $sum_you_have.' <b>'.$_SESSION['new_mail'].'</b> '.$sum_new_message;
				break;
				
				default:
					echo $sum_you_have.' <b>'.$_SESSION['new_mail'].'</b> '.$sum_new_messages;
				break;
			}
			$_SESSION['notifie_new_mail'] = $_SESSION['new_mail'];
			echo '</a></td></tr></table>';
		}
	}
}
?>
