<?php
if ($template_id > 0 && $task != 'replace_template_files')
{
	$check = (isset($template) && $template['restrict_editor'] == '1') ? true : false;
	$name = $template['name'];
	$style = $template['style'];
	$additional_style= $template['additional_style'];
}else
{
	$check = isset($_POST['restrict_editor']) ? true : false;
	$name = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
	$additional_style = isset($_POST['additional_style']) ? smartstrip($_POST['additional_style']) : '';
	$style = isset($_POST['style']) ? smartstrip($_POST['style']) : "
body{
	margin: 0px;
}


hr{

}

a:link, a:visited, a:active, a:hover  {
	color:  #000000;
}

.search{

}

.folder {
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	font-size: 12px;
}

a.folder:link, a.folder:visited, a.folder:active {
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	font-size: 12px;
    text-decoration:    none;
}

a.folder:hover{
	text-decoration: underline;
}

.file {
	font-family: Arial, Helvetica, sans-serif;
	font-weight: normal;
	font-size: 12px;
}

a.file:link, a.file:visited, a.file:active {
	font-family: Arial, Helvetica, sans-serif;
	font-weight: normal;
	font-size: 12px;
    text-decoration:    none;
}

a.file:hover{
	text-decoration: underline;
}


.path {
	background: #000000;
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	font-size: 14px;
	color: #f1f1f1;
}

a.path:link, a.path:visited, a.path:active {
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	font-size: 14px;
	color: #f1f1f1;
	text-decoration:    none;
}

a.path:hover{
	text-decoration: underline;
}
.home {
	background: #666666;
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	font-size: 14px;
	color: #f1f1f1;
}


a.home:link, a.home:visited, a.home:active {
	font-family: Arial, Helvetica, sans-serif;
	font-weight: bold;
	font-size: 14px;
	text-decoration:    none;
	color: #f1f1f1;
}

a.home:hover{
	text-decoration: underline;
}";
}

if($task == 'replace_template_files')
{
  $style = $cms->replace_template_files($template_id, $style);
  $additional_style = $cms->replace_template_files($template_id, $additional_style);  
}

?>
<input type="hidden" name="task" />
<input type="hidden" name="template_id" value="<?php echo $template_id; ?>" />
<input type="hidden" name="close" value="false" />
<table border="0" cellpadding="10" cellspacing="0">
<tr>
	<td>
	<?php
	if(isset($feedback)) echo $feedback;
	?>
	<table border="0">
	<tr>
		<td><?php echo $strName; ?>:</td>
		<td><input type="text" value="<?php echo $name; ?>" name="name" class="textbox" size="30" /></d>
	</tr>
	<tr>
		<td colspan="2">
		<?php $checkbox = new checkbox('restrict_editor', 'true', $cms_restrict_editor, $check); ?></td>
	</tr>
	</table>
	<table border="0">
	<tr>
		<td colspan="2">
		<table border="0">
		<tr><td><?php echo $cms_style; ?>:</td></tr>
		<tr><td><a href="javascript:popup('template_file_selector.php?template_id=<?php echo $template_id; ?>&SET_FIELD=style','250','300')" class="normal"><?php echo $cms_insert_file_url; ?></a>
		&nbsp;&nbsp;|&nbsp;&nbsp;<a href="javascript:replace_template_files()" class="normal"><?php echo $cms_replace_filenames; ?></a><br />
		<input type="file" class="textbox" name="style_file" /></td></tr>
		<tr><td>
		<textarea class="textbox" name="style" style="width: 300px;height:300px;"><?php echo htmlspecialchars($style); ?></textarea>
		</td></tr></table>
		</td>
		<td>
		<table border="0">
		<tr><td><?php echo $cms_additional_style; ?>:</p></td></tr>
		<tr><td><a href="javascript:popup('template_file_selector.php?template_id=<?php echo $template_id; ?>&SET_FIELD=additional_style','250','300')" class="normal"><?php echo $cms_insert_file_url; ?></a></td></tr>
		<tr><td><input type="file" class="textbox" name="additional_style_file" /></td></tr>		
		<tr><td><textarea class="textbox" name="additional_style" style="width: 300px;height:300px;"><?php echo htmlspecialchars($additional_style); ?></textarea></td></tr>
		</table>
		</td>
	</tr>
	</table>
	<br />
	<?php
	$button = new button($cmdOk, "javascript:save_close_template_item()");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdSave, "javascript:save_template_item()");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='index.php?tabindex=2'");
	?>
	</td>
</tr>
</table>
	<script type="text/javascript">
	function save_template_item()
	{
		document.forms[0].task.value='save_template';
		document.forms[0].submit();
	}
	function save_close_template_item()
	{
		document.forms[0].close.value='true';
		document.forms[0].task.value='save_template';
		document.forms[0].submit();
	}
	function replace_template_files()
	{
		document.forms[0].task.value = 'replace_template_files';
		document.forms[0].submit();
	}
	</script>
