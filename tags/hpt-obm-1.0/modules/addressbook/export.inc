<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<table border="0" cellpadding="5" cellspacing="0">
<tr>
<td><?php echo $contacts_export_text; ?></td>
</tr>
<tr>
<td>
<table border="0" cellpadding="5" cellspacing="0">
<tr>
<td valign="top"><?php echo $ab_export_type; ?>:</td>
<td>
<?php
$export_type = isset($_REQUEST['export_type']) ? $_REQUEST['export_type'] : 'contacts';
$radio_list = new radio_list('export_type', $export_type);
$radio_list->add_option('contacts', $contacts_contacts);
echo '<br />';
$radio_list->add_option('companies', $ab_companies);
?>
</td>
</tr>
<tr>
<td valign="top"><?php echo "$ab_encoding:"; ?></td>
<td>
<?php
require_once($GO_CONFIG->class_path."/phpvnconv/LIST.inc");
$encoding = isset($_POST['encoding']) ? $_POST['encoding'] : 'none';
$droplist = new dropbox('encoding');
$droplist->add_value('utf16','Unicode');
$droplist->add_value('none','None');
foreach ($vnconv_charset_list as $cs => $text) {
  $droplist->add_value($cs,$text);
}
$droplist->print_dropbox('encoding',$encoding);
?>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<?php
$seperator = isset($_POST['seperator']) ? $_POST['seperator'] : 'comma';
$quote = isset($_POST['quote']) ? $_POST['quote'] : '&quot;';
$crlf = isset($_POST['crlf']) ? $_POST['crlf'] : '\r\n';
?>
<table border="0"  cellpadding="0" cellspacing="3">
<tr>
<td><?php echo $contacts_seperator_text; ?>:</td>
<td>
<?php
$droplist = new dropbox('encoding');
$droplist->add_value('comma',$str_sep['comma']);
$droplist->add_value('semicolon',$str_sep['semicolon']);
$droplist->add_value('colon',$str_sep['colon']);
$droplist->add_value('tab',$str_sep['tab']);
$droplist->print_dropbox('seperator',$seperator);
?>
<!-- input class="textbox" type="text" size="3" name="seperator" value="<?php echo $seperator; ?>" / -->
</td>
</tr>
<tr>
<td><?php echo $contacts_quote; ?>:</td>
<td><input class="textbox" type="text" size="3" name="quote" value="<?php echo $quote; ?>" /></td>
</tr>
<tr>
<td><?php echo $contacts_clrf; ?>:</td>
<td><input class="textbox" type="text" size="3" name="crlf" value="<?php echo $crlf; ?>" /></td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<br />
<?php
$button = new button($cmdOk, "javascript:document.forms[0].task.value='export';document.forms[0].submit();document.forms[0].task.value='';");
echo '&nbsp;&nbsp;';
$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
?>
</td>
</tr>
</table>
