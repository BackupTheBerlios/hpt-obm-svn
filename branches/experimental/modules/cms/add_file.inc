<form name="add_file" action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
<input type="hidden" name="title" value="" />
<input type="hidden" name="description" value="" />
<input type="hidden" name="keywords" value="" />
<input type="hidden" name="task" value="save" />
<input type="hidden" name="content" value="" />
<input type="hidden" name="folder_id" value="<?php echo $folder_id; ?>" />
<input type="hidden" name="site_id" value="<?php echo $site_id; ?>" />

<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="400">
<tr>
	<td valign="top">
	<table border="0" cellpadding="1" cellspacing="1" width="100%">
	<tr>
		<td colspan="99" class="TableHead"><?php echo $cms_new_page; ?></td>
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
				<br />
				<?php
				$button = new button($cmdOk, "javascript:document.forms[0].submit()");
				echo '&nbsp;&nbsp;';
				$button = new button($cmdCancel, "javascript:document.location='browse.php?site_id=".$site_id."&folder_id=".$folder_id."';");
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

</form>
<script type="text/javascript">
document.forms[0].name.focus();
</script>