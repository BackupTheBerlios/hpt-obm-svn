<input type="hidden" name="task" value="sites" />
<table  border="0" cellspacing="5" cellpadding="0">
<tr>
<td colspan="2">
<?php
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

if(isset($feedback)) echo $feedback;

if($GO_MODULES->write_permissions)
{
	echo '<p><a href="site.php" class="normal">'.$cms_cmd_new_site.'</a></p>';
}

echo '<table border="0">';
$cms->get_authorized_sites($GO_SECURITY->user_id);
$count = 0;
echo '<tr><td width="16"></td>';
echo '<td><h3>'.$strName.'</h3></td>';
echo '<td><h3>'.$cms_domain.'</h3></td>';
echo '<td><h3>'.$strOwner.'</h3></td>';
echo '<td colspan="3"></td></tr>';

$cms2 = new cms();
while($cms->next_record())
{
  $count++;
	//MS: Removed subscribing
  //$check = $cms2->is_subscribed($GO_SECURITY->user_id, $cms->f('id')) ? 'checked' : '';
  //echo '<tr><td width="16"><input type="checkbox" name="subscribed[]" value="'.$cms->f('id').'" '.$check.' /></td>';
  echo '<tr>';
  echo '<td width="16" height="16"><img src="'.$GO_THEME->images['site'].'" border="0" widht="16" height="16" align="absmiddle" /></td>';
  echo '<td><a class="normal" href="browse.php?site_id='.$cms->f('id').'">'.$cms->f('name').'</a></td>';
  echo '<td><a class="normal" href="http://'.$cms->f('domain').'" target="_blank">'.$cms->f('domain').'</a></td>';
  echo '<td>'.show_profile($cms->f('user_id')).'</td>';
  echo '<td><a target="_blank" href="view.php?site_id='.$cms->f('id').'" title="'.$cms_preview.' \''.$cms->f('domain').'\'"><img src="'.$GO_THEME->images['magnifier'].'" border="0" /></a></td>';
  echo '<td><a href="site.php?site_id='.$cms->f('id').'" title="'.$strEdit.' \''.htmlspecialchars($cms->f('domain')).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
  echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?delete_site=".$cms->f('id')."\",\"".rawurlencode($strDeletePrefix."'".addslashes($cms->f('domain'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cms->f('domain'))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";

}
/*
if ($count > 0)
{
  echo '<tr><td colspan="99">';
  $button = new button($cms_subscribe, 'document.forms[0].submit();');
  echo '</td></tr>';
}*/

echo '<tr><td colspan="99"><i>'.$count.' '.$cms_sites.'</i></td></tr>';
echo '</table>';
?>
</td>
</tr>
</table>
