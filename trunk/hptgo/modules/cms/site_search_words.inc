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

echo '<input type="hidden" name="task" />';

echo '<input type="hidden" name="search_word_id" value="'.$search_word_id.'" />';

if($task == 'add_search_word')
{
	if ($search_word_id > 0)
	{
		$search_word = $cms->get_search_word($search_word_id);
		$search_word_name = $search_word['search_word'];
	}else
	{
		$search_word_name = isset($_POST['search_word_name']) ? smartstrip($_POST['search_word_name']) : '';
	}
	if (isset($feedback)) echo '<br />'.$feedback;
	?>
	<table border="0" cellpadding="4">
	<tr>
		<td><?php echo $strName; ?></td>
		<td><input type="text" class="textbox" size="30" name="search_word_name" value="<?php echo htmlspecialchars($search_word_name); ?>" />
	</tr>
	<tr>
		<td colspan="2">
		<table border="0">
		<?php
		function print_files($folder_id)
		{
			global $cms, $GO_CONFIG, $search_word_id, $searched_files;
			$cms2 = new cms();

			$cms->get_files($folder_id);
			while($cms->next_record())
			{
				if(!isset($searched_files))
				{
					$check = $cms2->file_is_attached($cms->f('id'), $search_word_id) ? 'checked' : '';
				}else
				{
					$check = in_array($cms->f('id'), $searched_files) ? 'checked' : '';
				}
				echo '<tr><td><input type="hidden" name="files[]" value="'.$cms->f('id').'" /><input type="checkbox" name="selected_files[]" value="'.$cms->f('id').'" '.$check.' /></td>';
				echo '<td><img align="absmiddle" border="0" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$cms->f('extension').'" /></td>';
				echo '<td>'.$cms2->get_path($folder_id).'/'.$cms->f('name');

			}

			$cms2->get_folders($folder_id);
			while ($cms2->next_record())
			{
				print_files($cms2->f('id'));
			}
		}

		$site = $cms->get_site($site_id);
		//print_files($site['root_folder_id']);
		if ($search_word_id > 0)
		{
			$cms2 = new cms();
			$cms->get_attached_files($search_word_id);

			while($cms->next_record())
			{
				echo '<tr><td><input type="hidden" name="files[]" value="'.$cms->f('id').'" /><input type="checkbox" name="selected_files[]" value="'.$cms->f('id').'" checked /></td>';
				echo '<td><img align="absmiddle" border="0" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$cms->f('extension').'" /></td>';
				echo '<td><a href="browse.php?site_id='.$site_id.'&folder_id='.$cms->f('folder_id').'&file_id='.$cms->f('id').'">'.$cms2->get_path($cms->f('folder_id')).'/'.htmlspecialchars($cms->f('name')).'</a></td></tr>';
			}
		}
		?>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<?php
		$button = new button($cmdOk, "javascript:save_search_word('true')");
		echo '&nbsp;&nbsp;';
		$button = new button($cmdApply, "javascript:save_search_word('false')");
		echo '&nbsp;&nbsp;';

		if($search_word_id > 0)
		{
		$button = new button($cms_search_files, "javascript:search_files()");
		echo '&nbsp;&nbsp;';
		}
		$button = new button($cmdClose, "javascript:document.forms[0].submit()");
		?>
		</td>
	</tr>
	</table>
	<script type="text/javascript">

	function save_search_word(close)
	{
		document.forms[0].close.value=close;
		document.forms[0].task.value='save_search_word';
		document.forms[0].submit();
	}

	function search_files()
	{
		document.forms[0].task.value='search_files';
		document.forms[0].submit();
	}
	document.forms[0].search_word_name.focus();
	</script>
	<?php
}else
{
	?>
	<br /><a class="normal" href="javascript:edit_search_word(0)"><?php echo $cmdAdd; ?></a><br /><br />
	<?php
	echo '<table border="0" cellspacing="8"><tr>';
	$columns=3;
	$column_count = 0;
	$last_char = '';
	$cms->get_search_words($site_id);
	while ($cms->next_record())
	{
		$name = $cms->f('search_word');
		$char = strtoupper($name[0]);
		if ($char != $last_char)
		{
			if ($last_char != '')
			{
				echo '</td>';
				$column_count++;
			}

			if ($column_count == $columns)
			{
				echo '</tr><tr>';
				$column_count=0;
			}

			echo '<td valign="top" width="150"><h2>'.$char.'</h2>';
		}

		echo '<table border="0" cellpadding="0" cellspacing="2"><tr><td nowrap>'.htmlspecialchars($name).'</td>';
		echo '<td><a align="absmiddle" href="javascript:edit_search_word('.$cms->f('id').')" title="'.$strEdit.' \''.htmlspecialchars($cms->f('search_word')).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
		echo "<td><a align=\"absmiddle\" href='javascript:confirm_action(\"javascript:delete_search_word(".$cms->f('id').")\",\"".rawurlencode($strDeletePrefix."'".addslashes($cms->f('search_word'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cms->f('search_word'))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>";
		echo '</tr></table>';
		$last_char = $char;
	}

	for ($i=$column_count+1;$i<$columns;$i++)
	{
		echo '</td><td width="150">&nbsp;';
	}

	echo '</td></tr></table><br />';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	?>

	<script type="text/javascript">

	function edit_search_word(search_word_id)
	{
		document.forms[0].search_word_id.value = search_word_id;
		document.forms[0].task.value='add_search_word';
		document.forms[0].submit();
	}

	function delete_search_word(search_word_id)
	{
		document.forms[0].search_word_id.value = search_word_id;
		document.forms[0].task.value='delete_search_word';
		document.forms[0].submit();
	}
	</script>
	<?php
}
?>
