<?php
/*
   Copyright Intermesh 2004
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 30 March 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require('Group-Office.php');
header('Content-Type: text/html; charset='.$charset);
$GO_SECURITY->authenticate();
?>
<html>
<head>
<script language="javascript" type="text/javascript">
<!-- Centring window Javascipt mods By Muffin Research Labs//-->

function popup(url,w,h,target)
{
  var centered;
  x = (screen.availWidth - w) / 2;
  y = (screen.availHeight - h) / 2;
  centered =',width=' + w + ',height=' + h + ',left=' + x + ',top=' + y + ',scrollbars=no,resizable=no,status=no';
  popup = window.open(url, target, centered);
  if (!popup.opener) popup.opener = self;
  popup.focus();
}
</script>
<title><?php echo $GO_CONFIG->title; ?>
</title>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>">
<?php
echo '<meta http-equiv="refresh" content="'.$GO_CONFIG->refresh_rate.';url='.$_SERVER['PHP_SELF'].'">';

//if user uses the calendar then check for events to remind
$calendar_module = $GO_MODULES->get_module('calendar');
if ($calendar_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_read']) ||
      $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_write'])))
{			
  require_once($calendar_module['class_path'].'calendar.class.inc');
  require_once($calendar_module['class_path'].'todos.class.inc');
  $cal = new calendar();
  $todos = new todos();

  $remind_events = $cal->get_events_to_remind($GO_SECURITY->user_id);
  $remind_todos = $todos->get_todos_to_remind($GO_SECURITY->user_id);
  if ($remind_events || $remind_todos)
  {
    echo '<script language="javascript" type="text/javascript">popup("'.$calendar_module['url'].'reminder.php", "500", "200", "reminder");</script>';
  }
  unset($cal);
}

$_SESSION['notified_new_mail'] = isset($_SESSION['notified_new_mail']) ? $_SESSION['notified_new_mail'] : 0;
$_SESSION['new_mail'] = 0;
//check for email
$email_module = $GO_MODULES->get_module('email');
if ($email_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $email_module['acl_read']) ||
      $GO_SECURITY->has_permission($GO_SECURITY->user_id, $email_module['acl_write'])))
{			
  require_once($email_module['class_path'].'email.class.inc');
  require_once($GO_CONFIG->class_path.'imap.class.inc');
  $imap = new imap();
  $email1 = new email();
  $email2 = new email();

  $email1->get_accounts($GO_SECURITY->user_id);

  while($email1->next_record())
  {
    if ($email1->f('auto_check') == '1')
    {
      $account = $email1->Record;

      if ($imap->open($account['host'], $account['type'],$account['port'],$account['username'],$GO_CRYPTO->decrypt($account['password'])))
      {
	if ($account['type'] == 'imap')
	{
	  $status = $imap->status('INBOX');
	  if ($status->unseen > 0)
	  {
	    $_SESSION['new_mail'] += $status->unseen;
	  }

	  $email2->get_folders($email1->f('id'));
	  while($email2->next_record())
	  {
	    if ($email2->f('name') != 'INBOX')
	    {
	      $status = $imap->status($email2->f('name'));
	      if ($status->unseen > 0)
	      {
		$_SESSION['new_mail'] += $status->unseen;
	      }
	    }
	  }
	}else
	{
	  $status = $imap->status('INBOX');
	  if ($status->unseen > 0)
	  {
	    $_SESSION['new_mail'] += $status->unseen;
	  }
	}
      }else
      {
	$email2->disable_auto_check($account['id']);
	echo '<script language="javascript" type="text/javascript">alert("'.$account['host'].' '.$ml_host_unreachable.'");</script>';
      }
    }
  }

  if ($_SESSION['new_mail'] > 0 && $_SESSION['new_mail'] > $_SESSION['notified_new_mail'])
  {
    echo '<script language="javascript" type="text/javascript">popup("'.$email_module['url'].'notify.php", "400", "120", "email_notify");</script>';
  }
}

$msg_module = $GO_MODULES->get_module('messages');
if ($msg_module) {
  $db = new db();
  $db->query("SELECT messages_messages.* FROM messages_new,messages_messages WHERE messages_messages.user_id='".$GO_SECURITY->user_id."' AND messages_new.id=messages_messages.id ORDER BY messages_messages.ctime DESC");
  if ($db->next_record()) {
    $msg = cut_string($db->f('text'),100);
    $msg = str_replace("\"","\\\"",$msg);
    $msg = str_replace("\n",' ',$msg);
    $msg = str_replace("\r",' ',$msg);
    $id = $db->f('id');
    echo '<script language="javascript">parent.header.SetStatus("Message: '.$msg.'");</script>';
    $db->query("DELETE FROM messages_new WHERE id='$id'");
  }
}

?>
</head>
<body>
</body>
</html>
