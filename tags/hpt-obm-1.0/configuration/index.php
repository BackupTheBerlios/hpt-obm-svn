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

require("../Group-Office.php");
$GO_SECURITY->authenticate();
$page_title = $menu_about;
require($GO_THEME->theme_path."header.inc");
?>
<h1><?php echo $menu_configuration; ?></h1>
<table border="0" cellpadding="10">
<tr>
	<td>
	<a href="<?php echo $GO_CONFIG->host; ?>configuration/account/">
	<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['account']; ?>" style="margin: 2px;" />
	<?php echo $menu_accounts; ?>
	</a>
	</td>
</tr>
<tr>
	<td>
	<a href="<?php echo $GO_CONFIG->host; ?>configuration/preferences/">
	<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['preferences']; ?>" style="margin: 2px;" />
	<?php echo $menu_preferences; ?>
	</a>
	</td>
</tr>
<?php
if ($GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
{
	echo '<tr><td><a href="'.$GO_CONFIG->host.'configuration/groups/">';
	echo '<img align="absmiddle" border="0" width="32" heigh="32" src="'.$GO_THEME->images['groups'].'" style="margin: 2px;" />'.$menu_groups.'</a>';
	echo '</td></tr>';
}
?>
</table>
<br />
<?php
if ($GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
{
	?>
	<h1><?php echo $menu_admin; ?></h1>
	<table border="0" cellpadding="10">
	<tr>
		<td>
		<a href="<?php echo $GO_CONFIG->administrator_url; ?>settings/">
		<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['modules']; ?>" style="margin: 2px;" />
		<?php echo $menu_modules; ?></a>
		</td>
	</tr>
	<tr>
		<td>
		<a href="<?php echo $GO_CONFIG->administrator_url; ?>users/">
		<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['users']; ?>" style="margin: 2px;" />
		<?php echo $menu_users; ?></a>
		</td>
	</tr>
	<tr>
		<td>
		<a href="<?php echo $GO_CONFIG->administrator_url; ?>system/">
		<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['sysinfo']; ?>" style="margin: 2px;" />
		<?php echo $menu_sysinfo; ?></a>
		</td>
	</tr>
	<tr>
		<td>
		<a href="<?php echo $GO_CONFIG->administrator_url; ?>filetypes/">
		<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['filetypes']; ?>" style="margin: 2px;" />
		<?php echo $menu_filetypes; ?></a>
		</td>
	</tr>
	</table>
<?php
}

require($GO_THEME->theme_path."footer.inc");
?>
