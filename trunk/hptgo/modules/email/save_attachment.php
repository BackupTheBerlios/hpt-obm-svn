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

$GO_MODULES->authenticate('filesystem');
require($GO_LANGUAGE->get_language_file('filesystem'));
$email_module = $GO_MODULES->get_module('email');
if (!$email_module)
{
	die($strDataError);
}
require($GO_CONFIG->class_path."imap.class.inc");
require($email_module['class_path']."email.class.inc");
$mail = new imap();
$email = new email();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$filename= isset($_REQUEST['filename']) ? $_REQUEST['filename'] : '';
if ($filename != '' && $_SERVER['REQUEST_METHOD'] != 'POST')
{
	$filename = stripslashes($filename);
	$_SESSION['email_tmp_file'] = $GO_CONFIG->tmpdir.$filename;

	require($GO_CONFIG->class_path.'filetypes.class.inc');
	$filetypes = new filetypes();
	$extension = get_extension($filename);
	if (!$type = $filetypes->get_type($extension))
	{
		$filetypes->add_type($extesnion, $mime);
	}
}

if ($filename == '')
{
	$filename = basename($_SESSION['email_tmp_file']);
}else
{
	$filename = smartstrip($filename);
}

if (isset($task) && $task == 'GO_HANDLER')
{
	require($GO_CONFIG->class_path.'filesystem.class.inc');
	$fs = new filesystem();

	if (file_exists(smartstrip($_REQUEST['path']).'/'.$filename))
	{
		$feedback = '<p class="Error">'.$fbNameExists.'</p>';

	}elseif(!$fs->has_write_permission($GO_SECURITY->user_id, smartstrip($_REQUEST['path'])))
	{
		$feedback = '<p class="Error">'.$strAccessDenied.': '.smartstrip($_REQUEST['path']).'</p>';
	}else
	{
		$new_path = smartstrip($_REQUEST['path']).'/'.$filename;
		if ($fs->move($_SESSION['email_tmp_file'], $new_path))
		{
			$old_umask = umask(000);
			chmod($new_path, $GO_CONFIG->create_mode);
			umask($old_umask);
			unset($_SESSION['tmp_account_id']);
			unset($_SESSION['email_tmp_file']);

			echo "<script type=\"text/javascript\" language=\"javascript\">\n";
			echo "window.close()\n";
			echo "</script>\n";
		}else
		{
			$feedback = '<p class="Error">'.$strSaveError.'</p>';
		}
	}
}
if (isset($_REQUEST['account_id']))
{
	$_SESSION['tmp_account_id'] = $_REQUEST['account_id'];
}

$account = $email->get_account($_SESSION['tmp_account_id']);

if (!file_exists($_SESSION['email_tmp_file']) && !is_dir($_SESSION['email_tmp_file']))
{
	if ($mail->open($account['host'], $account['type'],$account['port'],$account['username'],$GO_CRYPTO->decrypt($account['password']), $_REQUEST['mailbox']))
	{
		$data = $mail->view_part($_REQUEST['uid'], $_REQUEST['part'], $_REQUEST['transfer'], $_REQUEST['mime']);
		$mail->close();
		$fp = fopen($_SESSION['email_tmp_file'],"w+");
		fputs ($fp, $data, strlen($data));
		fclose($fp);
	}else
	{
		die('Could not connect to mail server!');
	}
}
require_once('../../Group-Office.php');

$module = $GO_MODULES->get_module('email');
$GO_HANDLER = $_SERVER['PHP_SELF'];
$GO_CONFIG->window_mode = 'popup';
$mode = 'save';
$module = $GO_MODULES->get_module('filesystem');
require($module['path'].'index.php');
?>
