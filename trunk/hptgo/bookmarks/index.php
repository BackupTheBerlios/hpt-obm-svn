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
require($GO_LANGUAGE->get_language_file('bookmarks'));

$page_title=$menu_bookmarks;
require($GO_CONFIG->class_path."bookmarks.class.inc");
$bookmarks = new bookmarks();
if (isset($_REQUEST['delete_bookmark']))
{
	$bookmarks->delete_bookmark($GO_SECURITY->user_id, $_REQUEST['delete_bookmark']);
}
require($GO_THEME->theme_path."header.inc");

?>
<table border="0" cellpadding="10" cellspacing="0">
<tr>
	<td>
	<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="600">
	<tr>
		<td valign="top">
		<table border="0" cellpadding="1" cellspacing="1" width="100%">
		<tr>
			<td colspan="99" class="TableHead"><?php echo $menu_bookmarks; ?></td>
		</tr>
		<tr>
			<td class="TableInside" height="300" valign="top">
			<br />
			<table border="0">
			<?php
			$count = $bookmarks->get_bookmarks($GO_SECURITY->user_id);
			if ($count > 0)
			{
				while($bookmarks->next_record())
				{
					echo "<tr><td><a class=\"normal\" href=\"javascript:popup('".$GO_CONFIG->host."bookmarks/save_bookmark.php?bookmark_id=".$bookmarks->f("id")."&bname=".urlencode($bookmarks->f("name"))."&bURL=".$bookmarks->f("URL")."&new_window=".$bookmarks->f("new_window")."','500','160')\">".$bookmarks->f("name")."</a></td>";
					echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?delete_bookmark=".$bookmarks->f("id")."\",\"".rawurlencode($strDeletePrefix."'".$bookmarks->f("name")."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$bookmarks->f("name")."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>";
				}
			}else
            {
			        echo "<tr><td colspan=\"2\">".$bm_no_bookmarks."</td></tr>";
            }
			?>
			</table>
			</td>
		</tr>
		</table>
	</tr>
	</table>
	</td>
</tr>
</table>

<?php
require($GO_THEME->theme_path."footer.inc");
?>
