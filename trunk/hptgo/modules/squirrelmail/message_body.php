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
require($GO_CONFIG->class_path."filetypes.class.inc");

$filetypes = new filetypes();
//$mail = new imap();
$email = new email();

require($GO_LANGUAGE->get_language_file('squirrelmail'));

$em_settings = $email->get_settings($GO_SECURITY->user_id);

$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$to = '';
$texts = '';
$images = '';

$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$mailbox = isset($_REQUEST['mailbox'])?  $_REQUEST['mailbox'] : "INBOX";
$uid = isset($_REQUEST['uid']) ? $_REQUEST['uid'] : 0;
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first_row = (isset($_REQUEST['first_row'])) ? $_REQUEST['first_row'] : 0;
$table_tabindex = isset($_REQUEST['table_tabindex']) ? $_REQUEST['table_tabindex'] : null;
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : null;
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$task = (isset($_REQUEST['task']) && $_REQUEST['task'] != '') ? $_REQUEST['task'] : '';
$print = isset($_REQUEST['print']) ? true : false;
$part = isset($_REQUEST['part']) ? $_REQUEST['part'] : '';
$query = isset($_REQUEST['query']) ? $_REQUEST['query'] : '';
$account = $email->get_account($account_id);

if (!$account)
  email_error();
     // && $mail->open($account['host'], $account['type'], $account['port'],$account['username'], $GO_CRYPTO->decrypt($account['password']),$mailbox, 0, $account['use_ssl'], $account['novalidate_cert']))
$imapConnection = sqimap_login($account['username'], $GO_CRYPTO->decrypt($account['password']),$account['host'],$account['port'],true);
if (!$imapConnection)
  email_error();

$aMailboxPref = array(
		      MBX_PREF_SORT         => 0,
		      MBX_PREF_LIMIT        => (int)  $show_num,
		      MBX_PREF_AUTO_EXPUNGE => (bool) $auto_expunge,
		      MBX_PREF_INTERNALDATE => (bool) getPref($data_dir, $username, 'internal_date_sort')
		      // MBX_PREF_FUTURE       => (var)  $future
		      );
$aConfig = array(
		 'allow_thread_sort' => $allow_thread_sort,
		 'allow_server_sort' => $allow_server_sort,
		 'user'              => $username,
		 //'showall' => true,
		 // incoming vars
		 'offset' => $startMessage
		 );
$aMailbox = sqm_api_mailbox_select($imapConnection,$mailbox,$aConfig,$aMailboxPref);
if (!$aMailbox)
  email_error();

if ($task == 'move_mail') {
  $messages = array($uid);
  $move_to_mailbox = smartstrip($_REQUEST['move_to_mailbox']);
  if(sqimap_msgs_list_move($imapConnection,$messages,$move_to_mailbox)) {
    header('Location: '.$GO_MODULES->url.'index.php?account_id='.$account_id.'&mailbox='.$mailbox);
    sqimap_logout($imapConnection);
    exit();
  }
}

if (isset($aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT'])) {
  $message = $aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT'];
  $FirstTimeSee = !$message->is_seen;
} else {
  $message = sqimap_get_message($imapConnection, $uid, $mailbox);
  $FirstTimeSee = !$message->is_seen;
  $message->is_seen = true;
  $aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT'] = $message;
}

$header = $message->header;

$ent_ar = $message->findDisplayEntity(array());

$cnt = count($ent_ar);
for ($i = 0; $i < $cnt; $i++) {
   $messagebody .= formatBody($imapConnection, $message, $color, $wrap_at, $ent_ar[$i], $uid, $mailbox);
   if ($i != $cnt-1) {
       $messagebody .= '<hr noshade size="1" />';
   }
}

require($GO_THEME->theme_path."header.inc");

echo $messagebody;

require($GO_THEME->theme_path."footer.inc");
sqimap_logout($imapConnection);


function email_error()
{
  global $imapConnection;
  require($GO_THEME->theme_path.'header.inc');
  echo '<table border="0" cellpadding="10" width="100%"><tr><td>';
  echo '<p class="Error">'.$ml_connect_failed.' \''.$account['host'].'\' '.$ml_at_port.': '.$account['port'].'</p>';
  echo '<p class="Error">'.imap_last_error().'</p>';
  require($GO_THEME->theme_path.'footer.inc');
  sqimap_logout($imapConnection);
  exit();
}

?>
