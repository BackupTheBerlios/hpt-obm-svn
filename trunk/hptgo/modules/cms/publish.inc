<br />
<?php
echo '<input type="hidden" name="task" value="publish" />';
$radio_list = new radio_list('publish_style', $site['publish_style']);
if (isset($feedback))
{
	echo $feedback;
}
?>
<table border="0" cellpadding="3">
<tr>
	<td colspan="2"><?php $radio_list->add_option('0', $cms_disable_publish, false, ''); ?></td>
</tr>
<tr>
	<td colspan="2"><?php $radio_list->add_option('1', $cms_publish_dynamic, false, ''); ?></td>
</tr>
<tr>
	<td colspan="2"><?php $radio_list->add_option('2', $cms_publish_static, false, ''); ?></td>
</tr>
<tr>
	<td>
	<?php echo $fbPath; ?>:
	</td>
	<td>
	<?php
	echo '<input class="textbox" type="text" name="publish_path" size="50" value="'.$site['publish_path'].'" />';
	?>
	</td>
</tr>
<tr>
	<td colspan="2">
	<?php
	$button = new button($cmdOk, "javascript:save_close_site()");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdApply, "javascript:save_site()");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	?>
	</td>
</tr>
</table>

<script type="text/javascript">

function save_close_site()
{
	document.forms[0].close.value='true';
	document.forms[0].task.value='save_publish';
	document.forms[0].submit();
}

function save_site()
{
	document.forms[0].task.value='save_publish';
	document.forms[0].submit();
}
</script>