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
		case 'date_format':
			$date_format_id = isset($_REQUEST['date_format_id']) ? $_REQUEST['date_format_id'] : 0;
			if ($date_format_id > 0)
			{
				$date_format = $xpath->getAttributes('/date_formats/date_format[@id='.$date_format_id.']');
				$php_format=$date_format['php_format'];
				$go_format=$date_format['go_format'];
			}
	
			
			if (isset($feedback)) echo $feedback;
			?>
			<input type="hidden" value="date_format" name="task" />
			<input type="hidden" name="date_format_id" value="<?php echo $date_format_id; ?>" />
			<table border="0">
			<tr>
				<td>
				<table border="0">
				<tr>
					<td>PHP:</td>
					<td><input type="text" class="textbox" name="php_format" value="<?php if (isset($php_format)) echo $php_format; ?>" style="width: 300px;" /></td>
				</tr>
				<tr>
					<td><?php echo $strDescription; ?>:</td>
					<td><input type="text" class="textbox" name="go_format" value="<?php if (isset($go_format)) echo $go_format; ?>" style="width: 300px;" /></td>
				</tr>
				<tr>
					<td colspan="2">
					<?php
					$button = new button($cmdOk, 'javascript:document.forms[0].submit();');
					echo '&nbsp;&nbsp;';
					$button = new button($cmdCancel, 'javascript:cancel_date_format();');
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
			<input type="hidden" value="delete_date_format" name="task" />
			<input type="hidden" name="date_format_id" />
			<a class="normal" href="javascript:date_format(0);"><?php echo $cmdAdd; ?></a>
			<br /><br />
			<table border="0" cellpadding="4">
			<tr>
				<td>
				<?php
				if (isset($feedback)) echo $feedback;
				echo $admin_date_formats_text;
	
				$date_formats = $xpath->match('/date_formats/date_format');
				if (count($date_formats) > 0)
				{
					echo '<br /><br /><table border="0"><tr><td><h3>'.$strDescription.'</h3></td><td><h3>PHP</h3></td><td></td><td></td></tr>';
	
					while($date_format = array_shift($date_formats))
					{
						$date_format = $xpath->getAttributes($date_format);
	
						echo '<tr><td>'.$date_format['go_format'].'</td><td>'.$date_format['php_format'].'</td>';
						echo '<td><a href=\'javascript:date_format('.$date_format['id'].')\' title="'.$strEdit.' \''.$date_format['go_format'].'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
						echo '<td><a href=\'javascript:delete_date_format('.$date_format['id'].', "'.$date_format['go_format'].'")\' title="'.$strDeleteItem.' \''.$date_format['go_format'].'\'"><img src="'.$GO_THEME->images['delete'].'" border="0" /></a></td></tr>';
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
function delete_date_format(date_format_id, date_format_name)
{
	if (confirm("<?php echo $strDeletePrefix; ?>'"+date_format_name+"'<?php echo $strDeleteSuffix; ?>"))
	{
		document.forms[0].date_format_id.value = date_format_id;
		document.forms[0].submit();
	}
}

function date_format(date_format_id)
{
	document.forms[0].date_format_id.value = date_format_id;
	document.forms[0].task.value='date_format';
	document.forms[0].submit();
}

function cancel_date_format()
{
	document.forms[0].task.value='date_formats';
	document.forms[0].submit();

}
</script>
