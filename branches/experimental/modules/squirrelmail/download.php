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
$mail = new imap();
$email = new email();

require($GO_LANGUAGE->get_language_file('squirrelmail'));

$texts = '';
$images = '';

$mailbox = isset($_REQUEST['mailbox'])?  $_REQUEST['mailbox'] : "INBOX";
$uid = isset($_REQUEST['uid']) ? $_REQUEST['uid'] : 0;
$part = isset($_REQUEST['part']) ? $_REQUEST['part'] : '';
$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;
$ent_id = $_REQUEST['ent_id'];
$uid = $uid = $_REQUEST['uid'];
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

if (isset($aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT']) &&
    is_object($aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT']) ) {
    $message = $aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT'];
} else {
   $message = sqimap_get_message($imapConnection, $uid, $mailbox);
   $aMailbox['MSG_HEADERS'][$uid]['MESSAGE_OBJECT'] = $message;
}

$subject = $message->rfc822_header->subject;
if ($ent_id) {
    $message = &$message->getEntity($ent_id);
    $header = $message->header;

    if ($message->rfc822_header) {
       $subject = $message->rfc822_header->subject;
    } else {
       $header = $message->header;
    }
    $type0 = $header->type0;
    $type1 = $header->type1;
    $encoding = strtolower($header->encoding);
} else {
    /* raw message */
    $type0 = 'message';
    $type1 = 'rfc822';
    $encoding = 'US-ASCII';
    $header = $message->header;
}

/*
 * lets redefine message as this particular entity that we wish to display.
 * it should hold only the header for this entity.  We need to fetch the body
 * yet before we can display anything.
 */

if (isset($override_type0)) {
    $type0 = $override_type0;
}
if (isset($override_type1)) {
    $type1 = $override_type1;
}
$filename = '';
if (is_object($message->header->disposition)) {
    $filename = $header->disposition->getProperty('filename');
    if (!$filename) {
        $filename = $header->disposition->getProperty('name');
    }
    if (!$filename) {
        $filename = $header->getParameter('name');
    }
} else {
    $filename = $header->getParameter('name');
}

$filename = decodeHeader($filename,true,false);
$filename = charset_encode($filename,$default_charset,false);

// If name is not set, use subject of email
if (strlen($filename) < 1) {
    $filename = decodeHeader($subject, true, true);
    $filename = charset_encode($filename,$default_charset,false);
    if ($type1 == 'plain' && $type0 == 'text')
        $suffix = 'txt';
    else if ($type1 == 'richtext' && $type0 == 'text')
        $suffix = 'rtf';
    else if ($type1 == 'postscript' && $type0 == 'application')
        $suffix = 'ps';
    else if ($type1 == 'rfc822' && $type0 == 'message')
        $suffix = 'msg';
    else
        $suffix = $type1;

    if ($filename == '')
        $filename = 'untitled' . strip_tags($ent_id);
    $filename = $filename . '.' . $suffix;
}

/*
 * Note:
 *    The following sections display the attachment in different
 *    ways depending on how they choose.  The first way will download
 *    under any circumstance.  This sets the Content-type to be
 *    applicatin/octet-stream, which should be interpreted by the
 *    browser as "download me".
 *      The second method (view) is used for images or other formats
 *    that should be able to be handled by the browser.  It will
 *    most likely display the attachment inline inside the browser.
 *      And finally, the third one will be used by default.  If it
 *    is displayable (text or html), it will load them up in a text
 *    viewer (built in to squirrelmail).  Otherwise, it sets the
 *    content-type as application/octet-stream
 */
if (isset($absolute_dl) && $absolute_dl) {
    SendDownloadHeaders($type0, $type1, $filename, 1);
} else {
    SendDownloadHeaders($type0, $type1, $filename, 0);
}
/* be aware that any warning caused by download.php will corrupt the
 * attachment in case of ERROR reporting = E_ALL and the output is the screen */
mime_print_body_lines ($imapConnection, $uid, $ent_id, $encoding);
sqimap_logout($imapConnection);
?>
