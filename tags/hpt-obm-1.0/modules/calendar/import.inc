<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<script type="text/javascript" langugae="javascript">
<!--
function upload()
{
	document.forms[0].task.value="import";
	var status = null;
	if (status = get_object("status"))
	{
		status.innerHTML = "<?php echo $cal_please_wait; ?>";
	}
	document.forms[0].submit();
}


-->
</script>

<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_file_size; ?>" />

<table border="0" cellpadding="3">
<tr>
	<td colspan="2">
	<?php
	if (isset($feedback)) echo $feedback;
	echo $cal_import_text;
	?>
	<br /><br />
	</td>
</tr>
<tr>
	<td><?php echo $cal_filename; ?>:</td>
	<td><input type="file" name="ical_file" maxlength="100" size="60" class="textbox" /></td>
</tr>
<tr>
	<td colspan="2">
	<br />
	<?php
	$button = new button($cmdOk, 'javascript:upload();');
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
	?>
	</td>
</tr>
<tr>
	<td id="status" class="Success" colspan="2">&nbsp;</td>
</tr>
</table>

