<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require("../../Group-Office.php");
$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('squirrelmail');

require($GO_MODULES->class_path."email.class.inc");
require($GO_LANGUAGE->get_language_file('squirrelmail'));
$email = new email();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$id = isset($_REQUEST['id']) ? $_REQUEST['id'] : 0;

$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
if ($task == 'save_filter')
{
  if ($_POST['keyword'] != "" && $_POST['folder'] != "")
  {
    if ($email->add_filter($id, smart_addslashes($_POST['field']),
	  smart_addslashes($_POST['keyword']),
	  smart_addslashes($_POST['folder'])))
    {
      header('Location: '.$return_to);
      exit();
    }else
    {
      $feedback = '<p class="Error">'.$strSaveError.'</p>';
    }
  }else
  {
    $feedback = '<p class="Error">'.$error_missing_field.'</p>';
  }
}

require($GO_THEME->theme_path."header.inc");

$tabtable = new tabtable('filters_list', $ml_filters, '600', '300', '100', '', true);
$tabtable->print_head();
?>
<form method="POST" action="<?php echo $_SERVER['PHP_SELF']; ?>" name="email_client">
<input type="hidden" name="id" value="<?php echo $id; ?>" />
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>" />
<input type="hidden" name="task" />

<table border="0" cellpadding="4" cellspacing="0">
<tr>
<td colspan="3">
<br />
  <?php
if (isset($feedback))
{
  echo $feedback;
}
?>
</td>
</tr>

<tr>
<td>
<?php
$field = isset($_POST['field']) ? $_POST['field'] : '';
$dropbox=new dropbox();
$dropbox->add_value('sender',$ml_email_is);
$dropbox->add_value('subject',$ml_subject_is);
$dropbox->add_value('to',$ml_to_is);
$dropbox->add_value('cc',$ml_cc_is);
$dropbox->print_dropbox('field',$field);
?>
</td>
<td>
<input type="text" name="keyword" size="30" class="textbox" />
</td>
</tr>
<tr>
<td colspan="2">

<?php
$folder = isset($_POST['folder']) ? $_POST['folder'] : '';
$dropbox=new dropbox();
$dropbox->add_value('',$ml_move_to);
$email->get_all_folders($id, true);
while ($email->next_record())
{
  if (!($email->f('attributes')&LATT_NOSELECT))
  {
    $dropbox->add_value($email->f('name'), str_replace('INBOX'.$email->f('delimiter'), '', $email->f('name')));
  }
}
$dropbox->print_dropbox('folder',$folder);
?>
</td>
</tr>
<tr>
<td colspan="2">
<br />
<?php
$button = new button($cmdOk, 'javascript:save_filter()');
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel,'javascript:document.location=\''.$return_to.'\'');
?>
</td>
</tr>
</table>

  <script type="text/javascript">
function save_filter()
{
  document.forms[0].task.value='save_filter';
  document.forms[0].submit();
}
</script>
</form>
<?php
$tabtable->print_foot();
require($GO_THEME->theme_path.'footer.inc');
?>
