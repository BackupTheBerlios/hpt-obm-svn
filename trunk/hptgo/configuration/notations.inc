<input type="hidden" name="query_string" value="<?php echo $_SERVER['QUERY_STRING']; ?>" />

<table border="0" cellpadding="0" cellspacing="3">
<tr>
<td nowrap>
<?php echo $pref_timezone; ?>:
</td>
<td>
<?php
if(date('I') > 0)
{
	$timezone = $_SESSION['GO_SESSION']['timezone'] - $_SESSION['GO_SESSION']['DST'];
}else
{
	$timezone = $_SESSION['GO_SESSION']['timezone'];
}

$dropbox = new dropbox();
$dropbox->add_value('12','+12 GMT');
$dropbox->add_value('11','+11 GMT');
$dropbox->add_value('10','+10 GMT');
$dropbox->add_value('9','+1 GMT');
$dropbox->add_value('8','+8 GMT');
$dropbox->add_value('7','+7 GMT');
$dropbox->add_value('6','+6 GMT');
$dropbox->add_value('5','+5 GMT');
$dropbox->add_value('4','+4 GMT');
$dropbox->add_value('3','+3 GMT');
$dropbox->add_value('2','+2 GMT');
$dropbox->add_value('1','+1 GMT');
$dropbox->add_value('0','GMT');
$dropbox->add_value('-1','-1 GMT');
$dropbox->add_value('-2','-2 GMT');
$dropbox->add_value('-3','-3 GMT');
$dropbox->add_value('-4','-4 GMT');
$dropbox->add_value('-5','-5 GMT');
$dropbox->add_value('-6','-6 GMT');
$dropbox->add_value('-7','-7 GMT');
$dropbox->add_value('-8','-8 GMT');
$dropbox->add_value('-9','-9 GMT');
$dropbox->add_value('-10','-10 GMT');
$dropbox->add_value('-11','-11 GMT');
$dropbox->add_value('-12','-12 GMT');
$dropbox->print_dropbox("timezone", $timezone);

$checkbox = new checkbox('DST', '1', $adjust_to_dst, $_SESSION['GO_SESSION']['DST']);
?>
</td>
</tr>
<tr>
<td nowrap>
<?php echo $pref_date_format; ?>:
</td>
<td>
<?php
//create an xml object because the date formats are stored as a setting in xml format
//add them to a dropbox
$dropbox = new dropbox();
for ($i=0;$i<count($GO_CONFIG->date_formats);$i++)
{
  $dropbox->add_value($GO_CONFIG->date_formats[$i], date($GO_CONFIG->date_formats[$i], get_time()));
}
$dropbox->print_dropbox("date_format", $_SESSION['GO_SESSION']['date_format']);
?>
</td>
</tr>
<tr>
<td nowrap>
<?php echo $pref_time_format; ?>:
</td>
<td>
<?php
//create an xml object because the date formats are stored as a setting in xml format
//add them to a dropbox
$dropbox = new dropbox();
for($i=0;$i<count($GO_CONFIG->time_formats);$i++)
{
  $dropbox->add_value($GO_CONFIG->time_formats[$i], date($GO_CONFIG->time_formats[$i], get_time()));
}
$dropbox->print_dropbox("time_format", $_SESSION['GO_SESSION']['time_format']);
?>
</td>
</tr>
<tr>
<td nowrap>
<?php echo $pref_first_weekday; ?>:
</td>
<td>
<?php
//create an xml object because the date formats are stored as a setting in xml format
//add them to a dropbox
$dropbox = new dropbox();
$dropbox->add_value('0', $full_days[0]);
$dropbox->add_value('1', $full_days[1]);
$dropbox->print_dropbox("first_weekday", $_SESSION['GO_SESSION']['first_weekday']);
?>
</td>
</tr>
<tr>
<td >&nbsp;</td>
</tr>
<tr>
<td>
<?php echo $pref_thousands_seperator; ?>:
</td>
<td>
<input type="text" class="textbox" size="3" name="thousands_seperator" value="<?php echo $_SESSION['GO_SESSION']['thousands_seperator']; ?>" maxlength="1" />
</td>
</tr>
<tr>
<td>
<?php echo $pref_decimal_seperator; ?>:
</td>
<td>
<input type="text" class="textbox" size="3" name="decimal_seperator" value="<?php echo $_SESSION['GO_SESSION']['decimal_seperator']; ?>" maxlength="1" />
</td>
</tr>
<tr>
<td>
<?php echo $pref_currency; ?>:
</td>
<td>
<input type="text" class="textbox" size="3" max_length="3" name="currency" value="<?php echo $_SESSION['GO_SESSION']['currency']; ?>" />
</td>
</tr>
</table>
<?php
/*$button = new button($cmdOk, "javascript:save_preferences('true')");
echo '&nbsp;&nbsp;';
$button = new button($cmdApply, "javascript:save_preferences('false')");account/
if ($_SESSION['GO_SESSION']['start_module'] != '')
{
  echo '&nbsp;&nbsp;';
  $button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
}*/
$button = new button($cmdSave, "javascript:save_preferences('false')");
?>


<script type="text/javascript">
function save_preferences(close)
{
  document.forms[0].task.value='save_notations';
  document.forms[0].close.value=close;
  document.forms[0].submit();
}
</script>

