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
//load file management class
$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('filesystem');

require($GO_CONFIG->class_path.'filetypes.class.inc');
require_once($GO_CONFIG->class_path.'filesystem.class.inc');
require_once('group_folders.inc');
$fs = new filesystem();
$filetypes = new filetypes();
$path = smartstrip($_REQUEST['path']);
$group_folders = get_group_folders($GO_SECURITY->user_id, 0);
if (is_group_folder($group_folders, $path) || $fs->has_read_permission($GO_SECURITY->user_id, $path) || $fs->has_write_permission($GO_SECURITY->user_id, $path))
{
	$filename = basename($path);
	$extension = get_extension($filename);

	$type = $filetypes->get_type($extension);

	$browser = detect_browser();

	header('Content-Type: '.$type['mime']);
	header('Content-Length: '.filesize($path));
	header('Expires: '.gmdate('D, d M Y H:i:s') . ' GMT');
	if ($browser['name'] == 'MSIE')
	{
		header('Content-Type: application/octet-stream');
		header('Content-Disposition: attachment; filename="'.$filename.'"');
		header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
		header('Pragma: public');
	}else
	{
		header('Content-Type: '.$type['mime']);
		header('Pragma: no-cache');
		header('Content-Disposition: attachment; filename="'.$filename.'"');
	}
	header('Content-Transfer-Encoding: binary');
	$fd = fopen($path,'rb');
	while (!feof($fd)) {
		print fread($fd, 32768);
	}
	fclose($fd);
}else
{
	header('Location: '.$GO_CONFIG->host.'error_docs/401.php');
	exit();
}
?>
