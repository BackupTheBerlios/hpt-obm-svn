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

require("../../Group-Office.php");
$GO_SECURITY->authenticate(true);
require($GO_LANGUAGE->get_base_language_file('modules'));
$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : 'read';
$module = $GO_MODULES->get_module($_REQUEST['module_id']);

if ($post_action == 'read')
{
  $title = $strReadRights;
  $read_tab = true;
  $acl_id = $module["acl_read"];
}else
{
  $title = $strWriteRights;
  $write_tab = true;
  $acl_id = $module["acl_write"];
}

$page_title=$module['id'];

require($GO_THEME->theme_path."header.inc");
?>
<form method="post" name="permissions" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="close" value="false" />
<input type="hidden" name="module_id" value="<?php echo $_REQUEST['module_id']; ?>" />
<input type="hidden" name="post_action" value="<?php echo $post_action; ?>" />

<table border="0" cellpadding="10" cellspacing="0">
<tr>
<td>
<table border="0" cellpadding="2" cellspacing="0">
<tr>
<td valign="top">
<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="400">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $module['id']; ?></td>
</tr>
<tr>
<td class="<?php if(isset($read_tab)) echo 'ActiveTab'; else echo 'Tab'; ?>" align="center" width="100"><a href="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=read&module_id=<?php echo $_REQUEST['module_id']; ?>" class="<?php if(isset($read_tab)) echo 'ActiveTab'; else echo 'Tab'; ?>"><?php echo $strReadRights; ?></a></td>
<td class="<?php if(isset($write_tab)) echo 'ActiveTab'; else echo 'Tab'; ?>" align="center" width="100"><a href="<?php echo $_SERVER['PHP_SELF']; ?>?post_action=write&module_id=<?php echo $_REQUEST['module_id']; ?>" class="<?php if(isset($write_tab)) echo 'ActiveTab'; else echo 'Tab'; ?>"><?php echo $strWriteRights; ?></a></td>
<td class="Tab" width="200">&nbsp;</td>
</tr>
<tr>
<td class="TableInside" height="300" valign="top" colspan="5">
<table border="0" cellpadding="10">
<tr>
<td>
<?php
print_acl($acl_id, false);
?>
</td>
</tr>
<tr>
<td class="cmd">
<?php
$button = new button($cmdClose, 'javascript:window.close()');
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
</form>

<?php
require($GO_THEME->theme_path."footer.inc");
?>
