<br />
<?php
if (isset($feedback))
{
	echo $feedback;
}
?>
<input type="hidden" name="task" value="configuration" />
<table border="0" cellpadding="3">
<tr>
	<td colspan="2">
	<?php
	$publish_path = $GO_CONFIG->get_setting('cms_publish_path');
	$enable_publish = ($publish_path != '') ? true : false;
	$checkbox = new checkbox('enable_publish', 'true' , $cms_enable_publish, $enable_publish, false, 'onclick="javascript:publishing()"');
	?>
	</td>
</tr>
<tr>
	<td><?php echo $fbPath; ?>:</td>
	<td>
	<?php
	$disabled = $enable_publish ? '' : 'disabled';
	echo '<input class="textbox" type="text" name="publish_path" size="50" value="'.htmlspecialchars($publish_path).'" '.$disabled.' />';
	?>
	</td>
</tr>
<tr>
	<td>URL:</td>
	<td>
	<?php
	$publish_url = $GO_CONFIG->get_setting('cms_publish_url');
	echo '<input class="textbox" type="text" name="publish_url" size="50" value="'.htmlspecialchars($publish_url).'" '.$disabled.' />';
	?>
	</td>
</tr>
<tr>
	<td colspan="2">
	<?php
	$button = new button($cmdSave, 'javascript:document.forms[0].submit()');
	?>
	</td>
</tr>
</table>
<script type="text/javascript">

function publishing()
{
	document.forms[0].publish_path.disabled=!document.forms[0].publish_path.disabled;
	if (!document.forms[0].publish_path.disabled)
	{
		document.forms[0].publish_path.focus();
	}

}
</script>