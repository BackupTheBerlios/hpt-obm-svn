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

if (isset($task) && $task == 'delete')
{
	$cms->delete_template_file($template_file_id);
}
echo '<input type="hidden" name="template_file_id" />';
echo '<input type="hidden" name="task" />';
echo '<br /><a class="normal" href="'.$_SERVER['PHP_SELF'].'?task=upload&template_id='.$template_id.'">'.$fbUpload.'</a>';
echo '<br /><br /><table border="0" cellpadding="3" cellspacing="0"><tr>';
echo '<td width="16">&nbsp;</td>';
echo '<td width="100" nowrap><h3>'.$strName.'</h3></td>';
echo '<td width="100" nowrap><h3>'.$strType.'</h3></td>';
echo '<td width="100" nowrap><h3>'.$strSize.'</h3></td>';
echo '<td width="100" nowrap><h3>'.$strModified.'</h3></td><td></td></tr>';

//list the files
$total_size = 0;
$count_files = $cms->get_template_files($template_id);
while ($cms->next_record())
{
	$total_size += $cms->f('size');
	$short_name = htmlspecialchars(cut_string($cms->f('name'), 30));

	echo '<tr class="Table1">';
	echo '<td><img width="16" height="16" border="0" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$cms->f('extension').'" /></td>';
	echo '<td nowrap>&nbsp;<a href="javascript:popup(\'template_file.php?template_file_id='.$cms->f('id').'\',\'400\',\'300\')" title="'.htmlspecialchars($cms->f('name')).'">'.$short_name.'</a>&nbsp;&nbsp;</td>';
	echo '<td nowrap>'.$cms->f('content_type_friendly').'&nbsp;&nbsp;</td>';
	echo '<td nowrap align="right">'.format_size($cms->f('size')).'&nbsp;&nbsp;</td>';
	echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'], $cms->f('mtime')).'&nbsp;&nbsp;</td>';
	echo "<td><a href='javascript:delete_template_item(".$cms->f('id').",\"".rawurlencode($strDeletePrefix."'".addslashes($cms->f('name'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cms->f('name'))."'\"><img src=\"".$GO_THEME->image_url."buttons/delete.gif\" border=\"0\"></a></td></tr>\n";
}
echo '<tr><td colspan="99" height="18">&nbsp;'.$count_files.' '.$cms_items.'</td></tr>';
echo '<tr><td colspan="99"><br />';
$button = new button($cmdClose, "javascript:document.location='index.php?tabindex=2'");
echo '</td></tr></table>';
?>
<script type="text/javascript">

function delete_template_item(template_file_id, question)
{
	if (confirm(question))
	{
		document.forms[0].task.value='delete';
		document.forms[0].template_file_id.value=template_file_id;
		document.forms[0].submit();
	}
}
</script>