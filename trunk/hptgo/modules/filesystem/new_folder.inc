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
<input type="hidden" name="task" value="new_folder" />
<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="400">
<tr>
	<td valign="top">
	<table border="0" cellpadding="1" cellspacing="1" width="100%">
	<tr>
		<td colspan="99" class="TableHead"><?php echo $fbNewFolder; ?></td>
	</tr>
	<tr>
		<td class="TableInside">
		<table border="0" cellpadding="4" cellspacing="0">
		<tr>
			<td colspan="2"><?php if(isset($feedback)) echo $feedback; ?>&nbsp;</td>
		</tr>
		<tr>
			<td>
			<?php echo $strName; ?>:
			</td>
			<td>
			<input type="text" class="textbox" name="name" value="<?php if (isset($_POST['name'])) echo $_POST['name']; ?>" maxlength="100" size="30" />
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<br />
				<?php
				$button = new button($cmdOk, "javascript:document.forms[0].submit()");
				echo '&nbsp;&nbsp;';
				$button = new button($cmdCancel, "javascript:document.location='".$_SERVER['PHP_SELF']."?path=".$urlencoded_path."';");
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
document.forms[0].name.focus();
</script>