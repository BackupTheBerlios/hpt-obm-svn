<input type="hidden" name="task" value="add_folder" />
<input type="hidden" name="folder_id" value="<?php echo $folder_id; ?>" />

<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="400">
<tr>
	<td valign="top">
	<table border="0" cellpadding="1" cellspacing="1" width="100%">
	<tr>
		<td colspan="99" class="TableHead"><?php echo $fbNewFolder; ?></td>
	</tr>
	<tr>
		<td class="TableInside">
		<table border="0" cellpadding="4" cellspacing="0">
		<tr>
			<td colspan="2"><?php if(isset($feedback)) echo $feedback; ?>&nbsp;</td>
		</tr>
		<tr>
			<td>
			<?php echo $strName; ?>:
			</td>
			<td>
			<?php
			$name = isset($_POST['name']) ? smartstrip(htmlspecialchars($_POST['name'])) : '';
			?>
			<input type="text" class="textbox" name="name" value="<?php echo $name; ?>" maxlength="100" size="30" />
			</td>
		</tr>
		<tr>
		<td>
		<?php echo $cms_priority; ?>:
		</td>
		<td>
		<?php
		$priority = isset($_POST['priority']) ? $_POST['priority'] : '0';
		?>
		<input type="text" class="textbox" name="priority" value="<?php echo $priority; ?>" maxlength="3" size="3" />
		</td>
		</tr>
		<tr>
		  <td colspan="2">
		<?php
		$checkbox = new checkbox('disabled', '1', $cms_hide_folder, isset($_POST['disabled']));
		?>
		</td>
		</tr>
		<tr>
			<td colspan="2">
				<br />
				<?php
				$button = new button($cmdOk, "javascript:document.forms[0].submit()");
				echo '&nbsp;&nbsp;';
				$button = new button($cmdCancel, "javascript:document.location='".$_SERVER['PHP_SELF']."?site_id=".$site_id."&folder_id=".$folder_id."';");
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
<script type="text/javascript">
document.forms[0].name.focus();
</script>