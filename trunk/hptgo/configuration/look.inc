<input type="hidden" name="query_string" value="<?php echo $_SERVER['QUERY_STRING']; ?>" />

<table border="0" cellpadding="2" cellspacing="3">
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
<?php
if ($GO_CONFIG->allow_themes == true)
{
  ?>
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
<!--
<tr>
<td colspan="2">
<?php
$module = $GO_MODULES->get_module('email');

if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']))
{
  $checked = ($_SESSION['GO_SESSION']['mail_client'] == 1) ? true : false;
  $checkbox = new checkbox('mail_client', '1', $pref_mail_client, $checked);
}
?>
</td>
</tr>-->
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
  document.forms[0].task.value='save_look';
  document.forms[0].close.value=close;
  document.forms[0].submit();
}
</script>

