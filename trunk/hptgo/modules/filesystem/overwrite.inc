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

if ($task == 'overwrite')
{
	$task = 'paste';
}
echo '<input type="hidden" value="'.$task.'" name="task" />';
echo '<input type="hidden" name="overwrite_destination_path" value="'.$overwrite_destination_path.'" />';
echo '<input type="hidden" name="overwrite_source_path" value="'.$overwrite_source_path.'" />';
echo '<input type="hidden" name="overwrite" value="false" />';
echo '<input type="hidden" name="overwrite_all" value="false" />';
?>
<table border="0" cellspacing="0" cellpadding="4">
<tr>
	<td><img src="<?php echo $GO_THEME->images['questionmark']; ?>" border="0" /></td><td><h2><?php echo $fbConfirmOverwrite; ?></h2></td>
</tr>
<tr>
	<td colspan="2">
	<?php echo $strOverwritePrefix."'".basename($overwrite_destination_path)."'".$strOverwriteSuffix; ?>
	</td>
</tr>
<tr>
	<td colspan="2" class="cmd">
	<br />
	<?php
	$button = new button($cmdOk,'javascript:overwrite_file(true);');
	echo '&nbsp;&nbsp;';
	$button = new button($cmdCancel,'javascript:overwrite_file(false);');
	echo '&nbsp;&nbsp;';
	$button = new button($cmdYesToAll,'javascript:overwrite_all_files();');
	?>
	</td>
</tr>
</table>

<script type="text/javascript" language="javascript">
function overwrite_file(overwrite)
{
	if (overwrite)
	{
		document.forms[0].overwrite.value = "true";
	}
	document.forms[0].submit();
}

function overwrite_all_files()
{
	document.forms[0].overwrite_all.value = "true";
	document.forms[0].overwrite.value = "true";
	document.forms[0].submit();
}
</script>