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
require($GO_LANGUAGE->get_language_file('squirrelmail'));

$email_module = $GO_MODULES->get_module('email');
if (!$email_module)
{
	die($strDataError);
}

//load file management class
require($GO_CONFIG->class_path."filesystem.class.inc");
require($email_module['class_path'].'email.class.inc');
require($GO_CONFIG->class_path.'filetypes.class.inc');
$email = new email();
$fs = new filesystem();
$filetypes = new filetypes();

$path = stripslashes($_REQUEST['path']);
$task = '';

if (!$fs->has_read_permission($GO_SECURITY->user_id, $path))
{
	header('Location: '.$GO_CONFIG->host.'error_docs/401.php');
	exit();
}

$attachments_size = 0;

if (isset($_SESSION['attach_array']))
{
	for($i=1;$i<=sizeof($_SESSION['attach_array']);$i++)
	{
		$attachments_size += $_SESSION['attach_array'][$i]->file_size;
	}
}

if (isset($_REQUEST['files']))
{
	for ($i=0;$i<count($_REQUEST['files']); $i++)
	{
		$attachments_size += filesize(smartstrip($_REQUEST['files'][$i]));
	}
	if ($attachments_size < $GO_CONFIG->max_attachment_size)
	{
		while ($file = smartstrip(array_shift($_REQUEST['files'])))
		{
			$tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));
			if (copy($file, $tmp_file))
			{
				$filename = basename($file);
				$extension = get_extension($filename);
				if (!$type = $filetypes->get_type($extension))
				{
					$type = $filetypes->add_type($extension);
				}
				$email->register_attachment($tmp_file, $filename, filesize($file), $type['mime'], 'attachment');
			}
		}
	}else
	{
		$task = 'too_big';
	}
}else
{
	if (isset($path) && !is_dir($path))
	{	$filesize = filesize($path);
		$attachments_size += $filesize;
		if ($attachments_size < $GO_CONFIG->max_attachment_size)
		{
			$tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));
			if (copy($path, $tmp_file))
			{
				$filename = basename($path);
				$email->register_attachment($tmp_file, $filename, $filesize);
				$task = 'attached';
			}else
			{
				die($strDataError);
			}
		}else
		{
			$task = 'too_big';
		}
	}
}

if ($task == 'too_big')
{
?>
	<html>
	<body>
	<script type="text/javascript">
			alert("<?php echo $ml_file_too_big.format_size($GO_CONFIG->max_attachment_size)." (".number_format($GO_CONFIG->max_attachment_size, 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator'])." bytes)."; ?>");
			window.close();
	</script>
	</body>
	</html>
<?php
}else
{
	?>

	<html>
	<body>
	<script type="text/javascript">
			opener.document.sendform.sendaction.value = 'attach_online';
			if(opener.document.sendform.content_type.value == 'text/HTML')
			{
				opener.document.sendform.onsubmit();
			}
			opener.document.sendform.submit();
			window.close();
	</script>
	</body>
	</html>
<?php

}
?>
