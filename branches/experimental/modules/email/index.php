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

$em_settings = $email->get_settings($GO_SECURITY->user_id);

$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$uid = isset($_REQUEST['uid']) ? $_REQUEST['uid'] : 0;
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first_row = (isset($_REQUEST['first_row'])) ? $_REQUEST['first_row'] : 0;
$table_tabindex = isset($_REQUEST['table_tabindex']) ? $_REQUEST['table_tabindex'] : null;
$mailbox = isset($_REQUEST['mailbox'])?  smartstrip($_REQUEST['mailbox']) : 'INBOX';
$link_back = $GO_MODULES->url.'index.php?account_id='.$account_id.'&mailbox='.$mailbox.'&first_row='.$first_row;

if (!$account = $email->get_account($account_id))
{
  $account = $email->get_account(0);
}

if ($account && $account["user_id"] != $GO_SECURITY->user_id)
{
  header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
  exit();
}
$disable_accounts = ($GO_CONFIG->get_setting('em_disable_accounts') == 'true') ? true : false;

$page_title = $lang_modules['email'];

$GO_HEADER['head'] = 	'<script type="text/javascript" src="'.$GO_MODULES->url.'email.js"></script>';
require($GO_THEME->theme_path."header.inc");
?>
<table border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5"></td>
<td><h1>
  <?php
if ($account)
{
  if ($mailbox == $account['mbroot'] || $mailbox == 'INBOX')
  {
    echo $ml_inbox;
  }elseif ($account['mbroot'] != '')
  {
    echo str_replace($account['mbroot'], '', $mailbox);
  }else
  {
    echo $mailbox;
  }
  echo ' - '.$account['email'];
}else
{
  echo $ml_welcome;
}
?>
</h1>
</td>
</tr>
</table>
<table border="0" cellspacing="0" cellpadding="0">
<tr>
<td class="ModuleIcons">
<a href="javascript:popup('send.php','<?php echo $GO_CONFIG->composer_width; ?>','<?php echo $GO_CONFIG->composer_height; ?>')"><img src="<?php echo $GO_THEME->images['compose']; ?>" border="0" height="32" width="32" /><br /><?php echo $ml_compose; ?></a></td>
  <?php
if (!$disable_accounts)
{
  echo '<td class="ModuleIcons">';
  echo '<a href="accounts.php?return_to='.urlencode($link_back).'"><img src="'.$GO_THEME->images['accounts'].'" border="0" height="32" width="32" /><br />'.$ml_accounts.'</a></td>';
}else
{
  if ($account)
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="account.php?account_id='.$account['id'].'&return_to='.urlencode($link_back).'"><img src="'.$GO_THEME->images['accounts'].'" border="0" height="32" width="32" /><br />'.$ml_edit_account.'</a></td>';
  }
}
if ($account['type'] == "imap" && $account)
{
  echo '<td class="ModuleIcons">';
  echo '<a href="folders.php?account_id='.$account['id'].'&return_to='.urlencode($link_back).'"><img src="'.$GO_THEME->images['folders'].'" border="0" height="32" width="32" /><br />'.$ml_folders.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a href="filters.php?account_id='.$account['id'].'&return_to='.urlencode($link_back).'"><img src="'.$GO_THEME->images['filters'].'" border="0" height="32" width="32" /><br />'.$ml_filters.'</a></td>';
}
if ($account)
{
  echo '<td class="ModuleIcons">';
  echo '<a href="search.php?account_id='.$account['id'].'&mailbox='.$mailbox.'"><img src="'.$GO_THEME->images['ml_search'].'" border="0" height="32" width="32" /><br />'.$ml_search.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a href="'.$_SERVER['PHP_SELF'].'?account_id='.$account['id'].'&mailbox=INBOX"><img src="'.$GO_THEME->images['em_refresh'].'" border="0" height="32" width="32" /><br />'.$ml_refresh.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a href="javascript:confirm_delete()"><img src="'.$GO_THEME->images['delete_big'].'" border="0" height="32" width="32" /><br />'.$ml_delete.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a href="javascript:confirm_empty_mailbox(\''.div_confirm_id($ml_confirm_empty_mailbox).'\')"><img src="'.$GO_THEME->images['empty_folder'].'" border="0" height="32" width="32" /><br />'.$ml_empty_mailbox.'</a></td>';
}
if ($GO_MODULES->write_permissions)
{
  echo '<td class="ModuleIcons">';
  echo '<a href="configuration.php?return_to='.urlencode($link_back).'"><img src="'.$GO_THEME->images['em_settings_admin'].'" border="0" height="32" width="32" /><br />'.$menu_configuration.'</a></td>';
}

?>
</tr>
</table>
<form method="POST" action="<?php echo $_SERVER['PHP_SELF']; ?>" name="email_client">
<input type="hidden" name="empty_mailbox" />
<input type="hidden" name="link_back" value="<?php echo $link_back; ?>" />
<input type="hidden" name="account_id" value="<?php echo $account['id']; ?>" />
<?php

if ($account)
{
  if ($mail->open($account['host'], $account['type'], $account['port'], $account['username'], $GO_CRYPTO->decrypt($account['password']), $mailbox, 0, $account['use_ssl'], $account['novalidate_cert']))
  {
    //block email to spam folder	
    if (isset($_REQUEST['spam_uid']) && $_REQUEST['spam_uid'] >0)
    {
      $spam_folder = $account['spam'];
      if ($_REQUEST['spam_address'] != '' && $spam_folder != '')
      {
	$email->add_filter($account['id'], "sender", smart_addslashes($_REQUEST['spam_address']), addslashes($spam_folder));
	$messages[] = $_REQUEST['spam_uid'];
	$mail->move($spam_folder, $messages);
      }
    }
  }else
  {
    echo '<p class="Error">'.$ml_connect_failed.' \''.$account['host'].'\' '.$ml_at_port.': '.$account['port'].'</p>';
    echo '<p class="Error">'.imap_last_error().'</p>';
    require($GO_THEME->theme_path.'footer.inc');
    exit();
  }
  require("navigation.inc");
}else
{
  echo '<br /><h3>'.$ml_no_accounts.'</h3><p class="normal">'.$ml_text.'</p>';
}
echo '</form>';
$mail->close();
require($GO_THEME->theme_path."footer.inc");
?>
