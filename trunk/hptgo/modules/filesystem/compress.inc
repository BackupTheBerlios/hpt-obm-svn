<?php
echo '<input type="hidden" name="task" />';

if (isset($_POST['files']))
{
	while($file = array_shift($_POST['files']))
	{
		echo '<input type="hidden" name="archive_files[]" value="'.$file.'" />';
	}
}
if (isset($_POST['folders']))
{
	while($folder = array_shift($_POST['folders']))
	{
		echo '<input type="hidden" name="archive_files[]" value="'.$folder.'" />';
	}	
}
$tabtable = new tabtable('go_archiver', 'Comprimeren', '400', '100', '100', '', true);
$tabtable->print_head();
if (isset($feedback))
{
	echo '<p class="Error">'.$feedback.'</p>';
}
$compression_type = isset($_POST['compression_type']) ? $_POST['compression_type'] : 'gz';

$name = isset($_POST['name']) ? $_POST['name'] : '';


echo '<table border="0"><tr><td>'.$strName.':</td><td>';
echo '<input type="text" class="textbox" name="name" value="'.$name.'" size="30" />';
echo '</td></tr><tr><td valign="top">'.$fb_compression_type.':</td><td>';

$radio_list = new radio_list('compression_type', $compression_type);
$radio_list->add_option('gz', 'gzip');
echo '<br />';
$radio_list->add_option('bz2','bzip');
echo '<br />';
$radio_list->add_option('zip','zip');
echo '</td></tr><tr><td colspan="2">';
$button = new button($cmdOk, "javascript:document.forms[0].task.value='save_archive';document.forms[0].submit();");
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel, "javascript:document.forms[0].task.value='';document.forms[0].submit();");
echo '</td></tr></table>';

$tabtable->print_foot();
?>
<script type="text/javascript">
document.forms[0].name.focus();
</script>