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

if (isset($_REQUEST['delete_site']) && $site = $cms->get_site($_REQUEST['delete_site']))
{
  if ($GO_SECURITY->user_id == $site['user_id'] || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $site['acl_write']))
  {
    $cms->delete_site($_REQUEST['delete_site']);
  }else
  {
    $feedback = '<p class="Error">'.$strAccessDenied.'</p>';
  }
}
?>
<input type="hidden" name="task" value="site" />
<input type="hidden" name="site_id" value="0" />
<table border="0" cellspacing="5" cellpadding="0">
<tr>
<td colspan="2">
<a href="site.php" class="normal"><?php echo $cms_cmd_new_site; ?></a><br /><br />
<?php
if(isset($feedback)) echo $feedback.'<br /><br />';

echo '<table border="0">';
$count = $cms->get_subscribed_sites($GO_SECURITY->user_id);

echo '<tr><td></td>';
echo '<td><h3>'.$strName.'</h3></td>';
echo '<td><h3>'.$cms_domain.'</h3></td>';
echo '<td><h3>'.$strOwner.'</h3></td>';
echo '<td colspan="3"></td></tr>';

while($cms->next_record())
{
  echo '<tr><td width="16" height="16"><img src="'.$GO_THEME->images['site'].'" border="0" widht="16" height="16" /></td>';
  echo '<td><a class="normal" href="browse.php?site_id='.$cms->f('id').'">'.$cms->f('name').'</a></td>';
  echo '<td><a class="normal" href="http://'.$cms->f('domain').'" target="_blank">'.$cms->f('domain').'</a></td>';
  echo '<td>'.show_profile($cms->f('user_id')).'</td>';
  echo '<td><a target="_blank" href="view.php?site_id='.$cms->f('id').'" title="'.$cms_preview.' \''.htmlspecialchars($cms->f('domain')).'\'"><img src="'.$GO_THEME->images['magnifier'].'" border="0" /></a></td>';
  echo '<td><a href="site.php?site_id='.$cms->f('id').'" title="'.$strEdit.' \''.$cms->f('domain').'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
  echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?delete_site=".$cms->f('id')."\",\"".rawurlencode($strDeletePrefix."'".addslashes($cms->f('domain'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$cms->f('domain')."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
}

echo '<tr><td colspan="99"><i>'.$count.' '.$cms_sites.'</i></td></tr>';
echo '</table>';
?>
</td>
</tr>
</table>

