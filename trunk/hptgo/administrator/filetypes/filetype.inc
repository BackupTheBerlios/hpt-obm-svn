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

if (isset($filetype_reload))
{
	session_unregister("filetype_reload");
	echo "<script type=\"text/javascript\">\ndocument.location.reload();\n</script>";
	exit();
}
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
	$filetypes->update_filetype($_POST['extension'], smart_addslashes($_POST['friendly']));
}
$filetype = $filetypes->get_type($_REQUEST['extension']);

if ($_SERVER['REQUEST_METHOD'] == "POST")
{

 	if (isset($_FILES['uploaded_file']) && is_uploaded_file($_FILES['uploaded_file']['tmp_name']))
	{
		if (eregi("gif", $_FILES['uploaded_file']['type']) || eregi("png", $_FILES['uploaded_file']['type']))
		{
			$filetype = $filetypes->get_type($_POST['extension']);
			$filetypes->update_filetype($_POST['extension'], smart_addslashes($filetype['friendly']), $_FILES['uploaded_file']['tmp_name']);
			$_SESSION['filetype_reload'] = true;
			echo "<script type=\"text/javascript\">\ndocument.location='".$_SERVER['PHP_SELF']."?task=filetype&extension=".$_POST['extension']."&first=".$_POST['first']."';\n</script>";
			exit();
		}
	}
}
?>
<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>" enctype="multipart/form-data">
<input type="hidden" name="task" value="filetype" />
<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_file_size; ?>" />
<input type="hidden" name="extension" value="<?php echo $_REQUEST['extension']; ?>" />
<input type="hidden" name="first" value="<?php echo $_REQUEST['first']; ?>" />

<table border="0" cellpadding="10" cellspacing="0">
<tr>
	<td>
	<table border="0" cellpadding="2" cellspacing="0">
	<tr>
		<td valign="top">
		<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="600">
		<tr>
			<td valign="top">
			<table border="0" cellpadding="1" cellspacing="1" width="100%">
			<tr>
				<td colspan="99" class="TableHead"><?php echo $ft_properties; ?></td>
			</tr>
			<tr>
				<td class="TableInside" height="300" valign="top">
				<table border="0" cellpadding="10">
				<tr>
					<td>
					<table  border="0" cellspacing="5" cellpadding="0">
					<tr>
						<td>
						<?php if (isset($feedback)) echo $feedback; ?>
						<table border="0" cellpadding="4" cellspacing="0">
						<tr>
							<td><?php echo $ft_extension; ?>:&nbsp;&nbsp;</td>
							<td><?php echo $_REQUEST['extension']; ?></td>

						</tr>
						<tr>
							<td><?php echo $ft_content; ?>:&nbsp;&nbsp;</td>
							<td><?php echo $filetype['mime']; ?></td>
						</tr>
						<tr>
							<td><?php echo $ft_friendly; ?>:&nbsp;&nbsp;</td>
							<td><input size="40" type="text" class="textbox" name="friendly" value="<?php echo htmlspecialchars($filetype['friendly']); ?>" /></td>
						</tr>
						<tr>
							<td><?php echo $ft_icon; ?>:&nbsp;&nbsp;</td>
							<td>
								<table border="0" cellpadding="0" cellspacing="0">
								<tr>
									<td id="status" colspan="2" class="Success">
									<img src="<?php echo $GO_CONFIG->control_url; ?>icon.php?extension=<?php echo $_REQUEST['extension']; ?>" width="16" height="16" border="0"/>&nbsp;&nbsp;
									</td>
									<td><input class="textbox" type="file" name="uploaded_file" maxlength="100" /></td>
								</tr>
								</table>
							</td>
						</tr>
						</table>
						</td>
					</tr>

					<tr>
							<td colspan="2">
							<br />
							<?php
							$button = new button($cmdSave, 'javascript:upload();');
							echo '&nbsp;&nbsp;';
							$button = new button($cmdBack, 'javascript:document.location=\'index.php?&first='.$_REQUEST['first'].'\';');
							?>
							</td>
					</tr>
					</table>
					</td>
				</tr>
				</table>
				</td>
			</tr>
			</table>
			</td>
		</tr>
		</table>
		</td>
	</tr>
	</table>
	</td>
</tr>
</table>
</form>
<script type="text/javascript">
<!--
	function upload()
	{
		if (document.forms[0].uploaded_file.value != '')
		{
			var status = null;
			if (status = get_object("status"))
			{
				status.innerHTML = "<?php echo $ft_please_wait; ?>";
			}
		}
		document.forms[0].submit();
	}
-->
</script>