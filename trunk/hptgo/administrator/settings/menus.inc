<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<br />
<?php echo $admin_menus_text ?>
<br /><br />
<table border="0">
<tr>
	<?php
	$checked = ($GO_CONFIG->get_setting('applications') != 'false') ? '' : 'checked';
	?>
	<td><input type="hidden" name="menus[]" value="applications" /><input type="checkbox" name="enabled_menus[]" value="applications" <?php echo $checked; ?> /></td>
	<td><?php echo $menu_applications; ?></td>
</tr>
<tr>
	<?php
	$checked = ($GO_CONFIG->get_setting('configuration') != 'false') ? '' : 'checked';
	?>
	<td><input type="hidden" name="menus[]" value="configuration" /><input type="checkbox" name="enabled_menus[]" value="configuration" <?php echo $checked; ?> /></td>
	<td><?php echo $menu_configuration; ?></td>
</tr>
<tr>
	<?php
	$checked = ($GO_CONFIG->get_setting('bookmarks') != 'false') ? '' : 'checked';
	?>
	<td><input type="hidden" name="menus[]" value="bookmarks" /><input type="checkbox" name="enabled_menus[]" value="bookmarks" <?php echo $checked; ?> /></td>
	<td><?php echo $menu_bookmarks; ?></td>
</tr>
<tr>
	<?php
	$checked = ($GO_CONFIG->get_setting('help') != 'false') ? '' : 'checked';
	?>
	<td><input type="hidden" name="menus[]" value="help" /><input type="checkbox" name="enabled_menus[]" value="help" <?php echo $checked; ?> /></td>
	<td><?php echo $menu_help; ?></td>
</tr>
</table>
<?php
$button = new button($cmdSave, "javascript:document.forms[0].submit();");
?>
