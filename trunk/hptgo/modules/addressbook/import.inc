
<script type="text/javascript" langugae="javascript">
<!--
function upload()
{
	document.forms[0].task.value="upload";
	var status = null;
	if (status = get_object("status"))
	{
		status.innerHTML = "<?php echo $fbPleaseWait; ?>";
	}
	document.forms[0].action='import.php';
	document.forms[0].submit();
}


-->
</script>

<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_file_size; ?>" />

<table border="0" cellpadding="3">
<tr>
	<td colspan="2">
	<?php
	if (isset($feedback)) echo $feedback;
	echo $contacts_import_text;
	?>
	<br /><br />
	</td>
</tr>
<tr>
	<td valign="top"><?php echo $ab_file_type; ?>:</td>
	<td>
	<?php
	$file_type = isset($_REQUEST['file_type']) ? $_REQUEST['file_type'] : 'csv';
	$dropbox = new dropbox();
	$dropbox->add_value('csv', "CSV (Comma Separated Values)");
	$dropbox->add_value('vcf', "VCF (vCard)");
	$dropbox->print_dropbox('file_type', $file_type, 'onchange="javascript:document.forms[0].submit()"');
	?>
	</td>
</tr>
<tr>
	<td><?php echo $fbFilename; ?>:</td>
	<td><input type="file" name="import_file" maxlength="100" size="60" class="textbox" /></td>
</tr>
<tr>
	<td valign="top"><?php echo $ab_import_type; ?>:</td>
	<td>
	<?php
	$import_type = isset($_REQUEST['import_type']) ? $_REQUEST['import_type'] : 'contacts';
	$attribute = ($file_type == 'csv') ? '' : 'disabled';
	$radio_list = new radio_list('import_type', $import_type);
	$radio_list->add_option('contacts', $contacts_contacts, $attribute);
	echo '<br />';
	$radio_list->add_option('companies', $ab_companies, $attribute);
	?>
	</td>
</tr>
<tr>
<td valign="top"><?php echo "$ab_encoding:"; ?></td>
<td>
<?php
require_once($GO_CONFIG->class_path."/phpvnconv/LIST.inc");
$droplist = new dropbox('encoding');
$droplist->add_value('None','none');
foreach ($vnconv_charset_list as $cs => $text) {
  $droplist->add_value($cs,$text);
}
$droplist->print_dropbox('encoding','none');
?>
</td>
</tr>
<tr>
	<td colspan="2">
	<br />
	<?php
	$button = new button($cmdOk, 'javascript:upload();');
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
	?>
	</td>
</tr>
<tr>
	<td id="status" class="Success" colspan="2">&nbsp;</td>
</tr>
</table>

