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

require($GO_CONFIG->class_path."imap.class.inc");
require($GO_MODULES->class_path."email.class.inc");
require($GO_LANGUAGE->get_language_file('squirrelmail'));
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

if ($account)
{
  if (!$mail->open($account['host'], $account['type'], $account['port'], $account['username'], $GO_CRYPTO->decrypt($account['password']), $mailbox, 0, $account['use_ssl'], $account['novalidate_cert']))
  {
    echo '<p class="Error">'.$ml_connect_failed.' \''.$account['host'].'\' '.$ml_at_port.': '.$account['port'].'</p>';
    echo '<p class="Error">'.imap_last_error().'</p>';
    require($GO_THEME->theme_path.'footer.inc');
    exit();
  }
  $imapConnection = $mail->conn;
  $mailbox = $_REQUEST['mailbox'];
  $passed_id = $_REQUEST['uid'];
  require_once($GO_CONFIG->class_path.'squirrelmail/src/read_body.php');
}else
{
  echo '<br /><h3>'.$ml_no_accounts.'</h3><p class="normal">'.$ml_text.'</p>';
}
$mail->close();
require($GO_THEME->theme_path."footer.inc");
?>

