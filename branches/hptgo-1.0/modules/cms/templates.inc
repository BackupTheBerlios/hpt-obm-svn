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
$template_id = isset($_REQUEST['template_id']) ? $_REQUEST['template_id'] : 0;

if ($task == 'delete')
{
	$template = $cms->get_template($template_id);
	if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $template['acl_write']))
	{
		if($cms->delete_template($template_id))
		{
			$GO_SECURITY->delete_acl($template['acl_write']);
			$GO_SECURITY->delete_acl($template['acl_read']);
		}
	}else
	{
		$feedback = '<p class="Error">'.$strAccessDenied.'</p>';
	}
}
?>
<input type="hidden" name="task" value="template" />
<input type="hidden" name="template_id" value="0" />
<table  border="0" cellspacing="5" cellpadding="0">
<tr>
	<td>
	<a href="template.php" class="normal"><?php echo $cms_new_theme; ?></a><br /><br />
	<?php
	if(isset($feedback)) echo $feedback.'<br /><br />';

	echo '<table border="0">';
	echo '<tr><td></td><td><h3>'.$strName.'</h3></td>';
	echo '<td><h3>'.$strOwner.'</h3></td>';
	echo '<td colspan="2">&nbsp;</td></tr>';

	$count = $cms->get_authorized_templates($GO_SECURITY->user_id);
	while($cms->next_record())
	{
		echo '<tr><td width="16" height="16"><img src="'.$GO_THEME->images['site'].'" border="0" widht="16" height="16" /></td>';
		echo '<td><a class="normal" href="template.php?template_id='.$cms->f('id').'" title="'.$strEdit.' \''.$cms->f('name').'\'">'.$cms->f('name').'</a></td>';
		echo '<td>'.show_profile($cms->f('user_id')).'</td>';
		echo "<td><a href='javascript:delete_template(".$cms->f('id').",\"".rawurlencode($strDeletePrefix."'".$cms->f('name')."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$cms->f('name')."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
	}
	echo '<tr><td colspan="2"><i>'.$count.'&nbsp;'.$cms_themes.'</i></td></tr>';
	echo '</table>';
	?>
	</td>
</tr>
</table>
<script type="text/javascript">

function delete_template(template_id, question)
{
	if (confirm(question))
	{
		document.forms[0].task.value='delete';
		document.forms[0].template_id.value=template_id;
		document.forms[0].submit();
	}
}
</script>

