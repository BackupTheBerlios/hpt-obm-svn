<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<table border="0" cellpadding="10">
<tr>
	<td>
	<?php
	$task = isset($task) ? $task : '';
	switch ($task)
	{
		case 'bookmark':
			$bookmark_id = isset($_REQUEST['bookmark_id']) ? $_REQUEST['bookmark_id'] : 0;
			if ($bookmark_id > 0)
			{
				$bookmark = $xpath->getAttributes('/bookmarks/bookmark[@id='.$bookmark_id.']');
				$name=$bookmark['name'];
				$url=$bookmark['url'];
				if($bookmark['new_window'] == 'true')
				{
					$new_window = true;
				}
			}
		
			if (isset($feedback)) echo $feedback;
			?>
			<input type="hidden" value="bookmark" name="task" />
			<input type="hidden" name="bookmark_id" value="<?php echo $bookmark_id; ?>" />
			<table border="0">
			<tr>
				<td>
				<table border="0">
				<tr>
					<td><?php echo $strName; ?>:</td>
					<td><input type="text" class="textbox" name="name" value="<?php if (isset($name)) echo $name; ?>" style="width: 300px;" /></td>
				</tr>
				<tr>
					<td>URL:</td>
					<td><input type="text" class="textbox" name="url" value="<?php if (isset($url)) echo $url; ?>" style="width: 300px;" /></td>
				</tr>
				<tr>
					<td colspan="2"><input type="checkbox" name="new_window" <?php if (isset($new_window)) echo 'checked' ?> />
					<?php echo $admin_bookmark_new_window; ?>
					</td>
				</tr>
				<tr>
					<td colspan="2">
					<?php
					$button = new button($cmdOk, 'javascript:document.forms[0].submit();');
					echo '&nbsp;&nbsp;';
					$button = new button($cmdCancel, 'javascript:cancel_bookmark();');
					?>
					</td>
				</tr>
				</table>
				</td>
			</tr>
			</table>
			<?php
		break;

		default:
			?>
			<input type="hidden" value="delete_bookmark" name="task" />
			<input type="hidden" name="bookmark_id" />
			<a class="normal" href="javascript:bookmark(0);"><?php echo $cmdAdd; ?></a>
			<br /><br />
			<table border="0" cellpadding="4">
			<tr>
				<td>
				<?php
				if (isset($feedback)) echo $feedback;
				echo $admin_bookmarks_text;
	
				$auto_bookmarks = $xpath->match('/bookmarks/bookmark');
				if (count($auto_bookmarks) > 0)
				{
					echo '<br /><br /><table border="0"><tr><td><h3>'.$strName.'</h3></td><td><h3>URL</h3></td><td></td><td></td></tr>';
	
					while($bookmark = array_shift($auto_bookmarks))
					{
						$bookmark = $xpath->getAttributes($bookmark);
	
						echo '<tr><td>'.$bookmark['name'].'</td><td>'.$bookmark['url'].'</td>';
						echo '<td><a href=\'javascript:bookmark('.$bookmark['id'].')\' title="'.$strEdit.' \''.$bookmark['name'].'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
						echo '<td><a href=\'javascript:delete_bookmark('.$bookmark['id'].', "'.$bookmark['name'].'")\' title="'.$strDeleteItem.' \''.$bookmark['name'].'\'"><img src="'.$GO_THEME->images['delete'].'" border="0" /></a></td></tr>';
					}
	
					echo '</table>';
				}
	
				?>
				</td>
			</tr>
			</table>
	
			<?php
		break;
	}
	?>
	</td>
</tr>
</table>

<script type="text/javascript">
function delete_bookmark(bookmark_id, bookmark_name)
{
	if (confirm("<?php echo $strDeletePrefix; ?>'"+bookmark_name+"'<?php echo $strDeleteSuffix; ?>"))
	{
		document.forms[0].bookmark_id.value = bookmark_id;
		document.forms[0].submit();
	}
}

function bookmark(bookmark_id)
{
	document.forms[0].bookmark_id.value = bookmark_id;
	document.forms[0].task.value='bookmark';
	document.forms[0].submit();
}

function cancel_bookmark()
{
	document.forms[0].task.value='bookmarks';
	document.forms[0].submit();

}
</script>
