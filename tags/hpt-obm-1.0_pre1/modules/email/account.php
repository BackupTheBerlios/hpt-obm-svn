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
$GO_MODULES->authenticate('email');

require($GO_CONFIG->class_path."imap.class.inc");
require($GO_MODULES->class_path."email.class.inc");
require($GO_LANGUAGE->get_language_file('email'));
$mail = new imap();
$email = new email();


$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ?
$_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ?
$_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$disable_accounts = ($GO_CONFIG->get_setting('em_disable_accounts') == 'true') ?
true : false;


//delete accounts if requested
if ($task == 'save_account')
{
  $task = 'account';
  $mbroot = isset($_POST['mbroot']) ? smart_addslashes($_POST['mbroot']) : '';
  if ($_POST['name'] == "" ||
      $_POST['mail_address'] == "" ||
      $_POST['port'] == "" ||
      $_POST['user'] == "" ||
      $_POST['pass'] == "" ||
      $_POST['host'] == "")
  {
    $feedback = $error_missing_field;
  }else
  {
    $sent = $_POST['type'] == 'pop3' ? '' : smart_addslashes($_POST['sent']);
    $spam = $_POST['type'] == 'pop3' ? '' : smart_addslashes($_POST['spam']);
    $trash = $_POST['type'] == 'pop3' ? '' : smart_addslashes($_POST['trash']);

    $auto_check = isset($_POST['auto_check']) ? '1' : '0';
    if (isset($_POST['account_id']))
    {
      if(!$email->update_account($_POST['account_id'], $_POST['type'],
	    smart_addslashes($_POST['host']),
	    $_POST['port'], $mbroot,
	    smart_addslashes($_POST['user']),
	    $_POST['pass'], smart_addslashes($_POST['name']),
	    smart_addslashes($_POST['mail_address']),
	    smart_addslashes($_POST['signature']),
	    $sent, $spam, $trash, $auto_check))
      {
	$feedback = '<p class="Error">'.$ml_connect_failed.' \''.$_POST['host'].
	  '\' '.$ml_at_port.': '.$_POST['port'].'</p>';
	$feedback .= '<p class="Error">'.$email->last_error.'</p>';
      }else
      {
	header('Location: '.$return_to);
	exit();
      }
    }else
    {
      if(!$email_id = $email->add_account($GO_SECURITY->user_id, $_POST['type'],
	    smart_addslashes($_POST['host']),
	    $_POST['port'], $mbroot,
	    smart_addslashes($_POST['user']),
	    $_POST['pass'],
	    smart_addslashes($_POST['name']),
	    smart_addslashes($_POST['mail_address']),
	    smart_addslashes($_POST['signature']),
	    $sent, $spam, $trash, $auto_check))
      {
	$feedback = '<p class="Error">'.$ml_connect_failed.' \''.
	  $_POST['host'].'\' '.$ml_at_port.': '.$_POST['port'].'</p>'.
	  '<p class="Error">'.$email->last_error.'</p>';
      }else
      {
	header('Location: '.$return_to);
	exit();
      }
    }
  }
}

if (isset($_REQUEST['account_id']) && $_SERVER['REQUEST_METHOD'] != "POST")
{
  $account = $email->get_account($_REQUEST['account_id']);
  if ($account['user_id'] != $GO_SECURITY->user_id)
  {
    require($GO_CONFIG->root_path."error_docs/403.inc");
    require($GO_THEME->theme_path."footer.inc");
    exit();
  }

  $page_title=$ml_edit_account;
  $name = $account["name"];
  $mail_address = $account["email"];
  $host = $account["host"];
  $type = $account["type"];
  $port = $account["port"];
  $user = $account["username"];
  $pass = $GO_CRYPTO->decrypt($account['password']);
  $signature = $account["signature"];
  $mbroot = $account["mbroot"];
  $spam = $account["spam"];
  $trash = $account["trash"];
  $sent = $account["sent"];
  $auto_check = $account['auto_check'] == '1' ? true : false;
}else
{
  $page_title=$ml_new_account;
  $name = isset($_REQUEST['name']) ? smartstrip($_REQUEST['name']) : $_SESSION['GO_SESSION']['name'];
  $mail_address = isset($_REQUEST['mail_address']) ? smartstrip($_REQUEST['mail_address']) : $_SESSION['GO_SESSION']['email'];
  $host = isset($_REQUEST['host']) ? smartstrip($_REQUEST['host']) : '';
  $type = isset($_REQUEST['type']) ? smartstrip($_REQUEST['type']) : 'pop3';
  $port = isset($_REQUEST['port']) ? smartstrip($_REQUEST['port']) : '110';
  $user = isset($_REQUEST['user']) ? smartstrip($_REQUEST['user']) : '';
  $pass = isset($_REQUEST['pass']) ? smartstrip($_REQUEST['pass']) : '';
  $signature = isset($_REQUEST['signature']) ? smartstrip($_REQUEST['signature']) : '';
  $mbroot = isset($_REQUEST['mbroot']) ? smartstrip($_REQUEST['mbroot']) : '';
  $spam = $mbroot."Spam";
  $trash = $mbroot."Trash";
  $sent = $mbroot."Sent items";
  $auto_check = isset($_REQUEST['auto_check']) ? true : false;
}


require($GO_THEME->theme_path."header.inc");

echo '<form method="POST" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

if (isset($_REQUEST['account_id']))
{
  $title = $ml_edit_account;
}else
{
  $title = $ml_new_account;
}
$tabtable = new tabtable('account_tab', $title, '600', '300', '100', '', true);
$tabtable->print_head();

if(isset($_REQUEST['account_id']))
{
  echo '<input type="hidden" value="'.$_REQUEST['account_id'].'" name="account_id" />';
}
if ($type  == 'pop3')
{
  $disabled = 'disabled';
}else
{

  $disabled = '';
}

echo '<input type="hidden" name="spam" value="'.$spam.'" />';
echo '<input type="hidden" name="sent" value="'.$sent.'" />';
echo '<input type="hidden" name="trash" value="'.$trash.'" />';
?>

<script type="text/javascript">

function change_port()
{
  if (document.forms[0].type.value == "imap")
  {
    document.forms[0].port.value = "143";
    document.forms[0].mbroot.disabled = false;
  }else
  {
    document.forms[0].port.value = "110";
    document.forms[0].mbroot.disabled = true;
  }
}

function save_account()
{
  document.forms[0].task.value='save_account';
  document.forms[0].submit();
}

</script>

<table border="0" cellpadding="10" cellspacing="0">
<tr>
<td>
<table border="0" cellpadding="0" cellspacing="5">
<?php if (isset($feedback)) echo '<tr><td class="Error" colspan="2">'.$feedback.'</td></tr>'; ?>
<tr>
<td><?php echo $strName; ?>:</td>
<td><input name="name" type="text" class="textbox" maxlength="100" size="40" value="<?php echo htmlspecialchars($name); ?>" /></td>
</tr>
<tr>
<td><?php echo $strEmail; ?>:</td>
<td><input name="mail_address" type="text" class="textbox" maxlength="100" size="40" value="<?php echo htmlspecialchars($mail_address); ?>" /></td>
</tr>
  <?php
if ($disable_accounts)
{
  echo '<input type="hidden" name="host" value="'.$host.'" />';
  echo '<input type="hidden" name="mbroot" value="'.$mbroot.'" />';
  echo '<input type="hidden" name="type" value="'.$type.'" />';
  echo '<input type="hidden" name="port" value="'.$port.'" />';
  echo '<input type="hidden" name="user" value="'.$user.'" />';
  echo '<input type="hidden" name="pass" value="'.$pass.'" />';
}else
{
  ?>

    <tr><td colspan="2">&nbsp;</td></tr>
    <tr>
    <td><?php echo $ml_type; ?>/<?php echo $ml_port; ?>:</td>
    <td>
    <?php
    $dropbox = new dropbox();
  $dropbox->add_value('pop3','POP3');
  $dropbox->add_value('imap','IMAP');
  $dropbox->print_dropbox('type',$type,'onchange="javascript:change_port()"');
  ?>
    &nbsp;/&nbsp;<input name="port" type="text" class="textbox" maxlength="6" size="4" value="<?php echo htmlspecialchars($port); ?>" />
    </td>
    </tr>

    <tr>
    <td><?php echo $ml_host; ?>:</td>
    <td><input name="host" type="text" class="textbox" maxlength="100" size="40" value="<?php echo htmlspecialchars($host); ?>" /></td>
    </tr>
    <tr>
    <?php
    echo '<td>'.$ml_root.':</td>';
  echo '<td><input type="text" class="textbox" size="40" name="mbroot" value="'.$mbroot.'" '.$disabled.' /></td>';
  ?>
    </tr>
    <tr><td colspan="2">&nbsp;</td></tr>
    <tr>
    <td><?php echo $strUsername; ?>:</td>
    <td><input name="user" type="text" class="textbox" maxlength="100" size="40" value="<?php echo htmlspecialchars($user); ?>" /></td>
    </tr>
    <tr>
    <td><?php echo $strPassword; ?>:</td>
    <td><input name="pass" type="password" class="textbox" maxlength="100" size="40" value="<?php echo htmlspecialchars($pass); ?>" /></td>
    </tr>
    <tr><td colspan="2">&nbsp;</td></tr>
    <?php
}
?>
<tr>
<td valign="top"><?php echo $ml_signature; ?>:</td>
<td><textarea name="signature" class="textbox" cols="37" rows="4"><?php echo htmlspecialchars($signature); ?></textarea></td>
</tr>
<tr>
<td colspan="2">
<?php
$checkbox = new checkbox('auto_check', '1', $ml_auto_check, $auto_check);
?>
</td>
</tr>
</table>
<br />
<?php
$button = new button($cmdOk, "javascript:_save('save_account', 'true');");
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel,'javascript:document.location=\''.$return_to.'\'');
?>
</td>
</tr>
</table>
<script type="text/javascript" language="javascript">

function _save(task, close)
{
  document.forms[0].task.value = task;
  document.forms[0].close.value = close;
  document.forms[0].submit();
}
</script>
<?php
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
