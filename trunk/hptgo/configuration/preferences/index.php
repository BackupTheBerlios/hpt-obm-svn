<?php
/*
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 14 March 2003
 */

require("../../Group-Office.php");
$GO_SECURITY->authenticate();
require($GO_LANGUAGE->get_base_language_file('preferences'));

$return_to = $GO_CONFIG->host.'configuration/';
$save_action = isset($_POST['save_action']) ? $_POST['save_action'] : false;

require($GO_THEME->theme_path."header.inc");

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
  if($save_action)
  {
    $mail_client = isset($_POST['mail_client']) ? $_POST['mail_client'] : "0";

	$DST = isset($_POST['DST']) ? '1' : '0';
    //if date formats are not present in the settings then use this default
    $date_format = (isset($_POST['date_format']) && $_POST['date_format'] != '') ? $_POST['date_format'] : 'd-m-Y H:i';
    $GO_USERS->set_preferences($GO_SECURITY->user_id,
	smart_addslashes($_POST['date_format']),
	smart_addslashes($_POST['time_format']),
	smart_addslashes($_POST['thousands_seperator']),
	smart_addslashes($_POST['decimal_seperator']),
	smart_addslashes($_POST['currency']), $mail_client,
	$_POST['max_rows_list'],
	$_POST['timezone'],
	$DST,
	$_POST['start_module'],
	$_POST['language'],
	$_POST['theme'], $_POST['first_weekday']);



    $save_action = false;

    echo '<script type="text/javascript">';

    if ($_POST['close'] == 'true')
    {
      echo 'parent.location="'.$GO_CONFIG->host.'index.php?return_to='.urlencode($return_to).'";';
    }else
    {
      echo 'parent.location="'.$GO_CONFIG->host.'index.php?return_to='.urlencode($_SERVER['PHP_SELF']).'";';
    }
    echo '</script>';
  }
}

$tabtable = new tabtable('preferences', $menu_preferences, '100%', '400');
?>
<form name="preferences" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="save_action" />
<input type="hidden" name="close" value="false" />

<?php $tabtable->print_head(); ?>
<table border="0" cellpadding="0" cellspacing="3">
<tr>
<td>&nbsp;</td>
</tr>
<tr>
<td ><h2><?php echo $pref_look; ?></h2></td>
</tr>
<tr>
<td>
<table border="0">
<tr>
<td nowrap>
<?php echo $pref_language; ?>:
</td>
<td>
<?php
$dropbox= new dropbox();
$languages = $GO_LANGUAGE->get_languages();
while($language = array_shift($languages))
{
  $dropbox->add_value($language['code'], $language['description']);
}
$dropbox->print_dropbox("language", $_SESSION['GO_SESSION']['language']['code']);
?>
</td>
</tr>

<tr>
<td nowrap>
<?php echo $pref_timezone; ?>:
</td>
<td>
<?php
$tz = isset($_SESSION['GO_SESSION']['timezone']) ?
	$_SESSION['GO_SESSION']['timezone'] : 7;
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
$dropbox->print_dropbox("timezone", "$tz");//$_SESSION['GO_SESSION']['timezone']);

$checkbox = new checkbox('DST', '1', $adjust_to_dst, $_SESSION['GO_SESSION']['DST']);
?>
</td>
</tr>
<?php
if ($GO_CONFIG->allow_themes == true)
{
  ?>
    <tr>
    <td >&nbsp;</td>
    </tr>
    <tr>
    <td nowrap>
    <?php echo $pref_theme; ?>:
    </td>
    <td>
    <?php
    $themes = $GO_THEME->get_themes();
  $dropbox = new dropbox();
  $dropbox->add_arrays($themes, $themes);
  $dropbox->print_dropbox("theme", $_SESSION['GO_SESSION']['theme']);
  ?>
    </td>
    </tr>
    <?php
}
?>
<tr>
<td>
<?php echo $pref_startmodule; ?>:
</td>
<td>
<?php
$dropbox = new dropbox();
$GO_MODULES->get_modules();
while ($GO_MODULES->next_record())
{
  if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $GO_MODULES->f('acl_read')))
  {
    if ($GO_MODULES->f('id') == 'summary')
    {
      $has_summary_module = true;
    }
    $lang_var = isset($lang_modules[$GO_MODULES->f('id')]) ? 
      $lang_modules[$GO_MODULES->f('id')] : $GO_MODULES->f('id');
    $dropbox->add_value($GO_MODULES->f('id'), $lang_var);
  }
}

$start_module = ($_SESSION['GO_SESSION']['start_module'] == '' && 
    isset($has_summary_module)) ? 'summary' : 
$_SESSION['GO_SESSION']['start_module'];

$dropbox->print_dropbox('start_module', $start_module);
?>
</td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<?php
$module = $GO_MODULES->get_module('email');

if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']))
{
  $checked = ($_SESSION['GO_SESSION']['mail_client'] == 1) ? true : false;
  $checkbox = new checkbox('mail_client', '1', $pref_mail_client, $checked);
}
?>
</td>
</tr>
<tr>
<td>
<table border="0">
<tr>
<td><?php echo $pref_max_rows_list; ?>:</td>
<td>
<?php
$dropbox = new dropbox();
$dropbox->add_value('0',$pref_unlimited);
$dropbox->add_value('10','10');
$dropbox->add_value('15','15');
$dropbox->add_value('20','20');
$dropbox->add_value('25','25');
$dropbox->add_value('30','30');
$dropbox->add_value('50','50');
$dropbox->add_value('75','75');
$dropbox->add_value('100','100');
$dropbox->print_dropbox('max_rows_list', $_SESSION['GO_SESSION']['max_rows_list']);
?>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td >&nbsp;</td>
</tr>
<tr>
<td><h2><?php echo $pref_notations; ?></h2></td>
</tr>
<tr>
<td>
<table border="0">
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
</td>
</tr>
<tr>
<td colspan="2" height="40">
<?php
$button = new button($cmdOk, "javascript:save_preferences('true')");
echo '&nbsp;&nbsp;';
$button = new button($cmdApply, "javascript:save_preferences('false')");
if ($_SESSION['GO_SESSION']['start_module'] != '')
{
  echo '&nbsp;&nbsp;';
  $button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
}
?>
</td>
</tr>
</table>
<?php $tabtable->print_foot(); ?>

  <script type="text/javascript">
function save_preferences(close)
{
  document.forms[0].save_action.value=true;
  document.forms[0].close.value=close;
  document.forms[0].submit();
}
</script>
</form>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
