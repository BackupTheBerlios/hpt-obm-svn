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
?>
<input type="hidden" name="folder_id" value="<?php echo $folder_id; ?>" />
<input type="hidden" name="task" value="upload" />
<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_file_size; ?>" />

<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="400">
<tr>
	<td valign="top">
	<table border="0" cellpadding="1" cellspacing="1" width="100%">
	<tr>
		<td colspan="99" class="TableHead"><?php echo $fbUpload; ?></td>
	</tr>
	<tr>
		<td class="TableInside">
		<table border="0" cellpadding="4" cellspacing="0">
		<?php if(isset($feedback)) echo '<tr><td>'.$feedback.'</td></tr>';
		?>
		<tr>
			<td><br /><br />
			<?php echo $fbFiles; ?>
			:<br /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
			<input type="file" name="file[]" maxlength="100" class="textbox" size="70" />

			</td>
		</tr>
		<tr>
			<td>
			<?php
			echo $fbMaxUploadSize.": ".format_size($GO_CONFIG->max_file_size)." (".number_format($GO_CONFIG->max_file_size, 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator'])." bytes)";
			?>
			</td>
		</tr>
		<tr>
			<td id="status" class="Success">&nbsp;</td>
		</tr>
		<tr>
			<td>
				<br />
				<?php
				$button = new button($cmdOk, "javascript:upload()");
				echo '&nbsp;&nbsp;';
				$button = new button($cmdCancel, "javascript:document.location='".$_SERVER['PHP_SELF']."?site_id=$site_id&folder_id=$folder_id';");
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
<script type="text/javascript">
<!--
function upload()
{
	var status = null;
	if (status = get_object("status"))
	{
		status.innerHTML = "<?php echo $fbPleaseWait; ?>";
	}
	document.forms[0].submit();
}
-->
</script>
