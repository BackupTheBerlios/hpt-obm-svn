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
<input type="hidden" name="template_id" value="<?php echo $template_id; ?>" />

<table border="0" cellpadding="10" cellspacing="0">
<tr>
	<td>
	<?php
	print_acl($template["acl_write"]);
	?>
	</td>
</tr>
</table>
<br />
<?php
$button = new button($cmdClose, "javascript:document.location='index.php?tabindex=2'");
?>