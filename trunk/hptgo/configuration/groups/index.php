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
$GO_SECURITY->authenticate();
require($GO_LANGUAGE->get_base_language_file('groups'));

if (!$GO_SECURITY->has_admin_permission($GO_SECURITY->user_id)) {
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}

//perform on delete request
if (isset($_REQUEST['delete_group']))
{
  //only owners can delete groups
  if ($GO_GROUPS->user_owns_group($GO_SECURITY->user_id, 
	$_REQUEST['delete_group']) 
      && $_REQUEST['delete_group'] != $GO_CONFIG->group_everyone 
      && $_REQUEST['delete_group'] != $GO_CONFIG->group_root)
  {
    $GO_GROUPS->delete_group($_REQUEST['delete_group']);
  }else
  {
    $feedback = "<P class=\"Error\">".$strAccessDenied."</p>";
  }
}

$page_title = $groups_title;
require($GO_THEME->theme_path."header.inc");

$tabtable = new tabtable('groups', $groups_title, '600', '300');

$tabtable->print_head();
echo '<table cellpadding="4" cellspacing="0" border="0">';
if (isset($feedback))
{
  echo $feedback;
}
echo '<tr height="30"><td colspan="3"><a href="'.$GO_CONFIG->host.'configuration/groups/group.php" class="normal">'.$cmdAdd.'</a></td></tr>';
echo '<tr><td><h3>'.$strName.'</h3></td>';
echo '<td><h3>'.$strOwner.'</h3></td><td>&nbsp;</td></tr>';

//show the groups the user is in and owns.
$GO_GROUPS->get_authorised_groups($GO_SECURITY->user_id);
while ($GO_GROUPS->next_record())
{
  echo '<tr>';
  echo "<td><a class=\"normal\" href=\"group.php?group_id=".$GO_GROUPS->f("id")."&group_name=".$GO_GROUPS->f("name")."\">".$GO_GROUPS->f("name")."</a></td>\n";
  echo "<td>".show_profile($GO_GROUPS->f("user_id"))."</td>\n";
  echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?delete_group=".$GO_GROUPS->f("id")."\",\"".rawurlencode($strDeletePrefix."'".$GO_GROUPS->f("name")."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$GO_GROUPS->f("name")."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
  echo "</tr>";
  if ($GO_GROUPS->f("id") == 2)
     echo '<tr><td colspan=3><hr></td></tr>';
}
echo '</table>';
echo '<br />';
$button = new button($cmdClose, "javascript:document.location='".$GO_CONFIG->host."configuration/'");
$tabtable->print_foot();

require($GO_THEME->theme_path."footer.inc");
?>
