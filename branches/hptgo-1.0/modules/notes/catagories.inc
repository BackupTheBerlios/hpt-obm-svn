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
if ($GO_MODULES->write_permissions)
{
	if (isset($_REQUEST['delete_catagory_id']) && $_REQUEST['delete_catagory_id'] > 0)
	{
		$notes->delete_catagory($_REQUEST['delete_catagory_id']);
	}

	echo '<a href="catagory.php" class="normal">'.$cmdAdd.'</a><br /><br />';

	$count = $notes->get_catagories();
	echo '<input type="hidden" name="delete_catagory_id" />';

	if ($count > 0)
	{
		echo '<table border="0" cellspacing="0" cellpadding="1">';

		$str_count = $count == 1 ? $no_catagories_count_single : $no_catagories_count;
		echo '<tr><td colspan="99" class="small" align="right">'.$count.' '.$str_count.'</td></tr>';
		echo '<tr height="20"><td><h3>'.$strName.'</h3><td>&nbsp;</td></tr>';

		$module_url = isset($module_url) ? $module_url : $GO_MODULES->url;
		while($notes->next_record())
		{
			echo '<tr><td><a class="normal" href="'.$module_url.'catagory.php?catagory_id='.$notes->f('id').'&return_to='.rawurlencode($link_back).'">'.htmlspecialchars($notes->f('name')).'</a></td>';
			echo "<td><a href='javascript:confirm_action(\"javascript:delete_catagory(".$notes->f('id').")\",\"".rawurlencode($strDeletePrefix."'".htmlspecialchars($notes->f('name'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$notes->f('name')."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
			//echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
		}
		echo '</table>';
	}else
	{
		echo $no_no_catagories;
	}
}else
{
	require($GO_CONFIG->root_path.'error_docs/403.inc');
}
?>
<script type="text/javascript">

function delete_catagory(id)
{
	document.forms[0].delete_catagory_id.value=id;
	document.forms[0].submit();
}

</script>
