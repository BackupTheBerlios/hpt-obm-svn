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
$GO_SECURITY->authenticate(true);

require($GO_THEME->theme_path."header.inc");
?>
<table border="0" cellpadding="10">
<tr>
	<td>
	<a href="<?php echo $GO_CONFIG->administrator_url; ?>users/">
	<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['users']; ?>" style="margin: 2px;" />
	<?php echo $menu_users; ?></a>
	</td>
</tr>
<tr>
	<td>
	<a href="<?php echo $GO_CONFIG->administrator_url; ?>groups/">
	<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['groups']; ?>" style="margin: 2px;" />
	<?php echo $menu_groups; ?></a>
	</td>
</tr>
<tr>
	<td>
	<a href="<?php echo $GO_CONFIG->administrator_url; ?>modules/">
	<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['modules']; ?>" style="margin: 2px;" />
	<?php echo $menu_modules; ?></a>
	</td>
</tr>
<tr>
	<td>
	<a href="<?php echo $GO_CONFIG->administrator_url; ?>phpsysinfo/">
	<img align="absmiddle" border="0" width="32" heigh="32" src="<?php echo $GO_THEME->images['sysinfo']; ?>" style="margin: 2px;" />
	<?php echo $menu_sysinfo; ?></a>
	</td>
</tr>
</table>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
