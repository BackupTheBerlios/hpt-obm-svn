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
require($GO_CONFIG->class_path."filetypes.class.inc");

$filetypes = new filetypes();
$mail = new imap();
$email = new email();

require($GO_LANGUAGE->get_language_file('email'));

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

if ($account && $mail->open($account['host'], $account['type'], $account['port'],$account['username'], $GO_CRYPTO->decrypt($account['password']),$mailbox))
{
  if ($task == 'move_mail')
  {
    $messages = array($uid);
    $move_to_mailbox = smartstrip($_REQUEST['move_to_mailbox']);
    if($mail->move($move_to_mailbox, $messages) && $mail->reopen($move_to_mailbox))
    {
      header('Location: '.$GO_MODULES->url.'index.php?account_id='.$account_id.'&mailbox='.$mailbox);
      exit();
    }
  }
  //sort messages for determination of previous and next message
  if ($query != '')
  {	
    $mail->search($em_settings['sort_field'], $em_settings['sort_order'], base64_decode($query));
  }else
  {
    $mail->sort($em_settings['sort_field'], $em_settings['sort_order']);
  }

  $content = $mail->get_message($uid, 'html', $part);
  $subject = isset($content["subject"]) ? $content["subject"] : $ml_no_subject;

}else
{
  require($GO_THEME->theme_path.'header.inc');
  echo '<table border="0" cellpadding="10" width="100%"><tr><td>';
  echo '<p class="Error">'.$ml_connect_failed.' \''.$account['host'].'\' '.$ml_at_port.': '.$account['port'].'</p>';
  echo '<p class="Error">'.imap_last_error().'</p>';
  require($GO_THEME->theme_path.'footer.inc');
  exit();
}

//update notified mail state
if ($content["new"] == '1')
{
  $_SESSION['notified_new_mail'] -= 1;
  $_SESSION['unseen_in_mailbox'] -= 1;
}

require($GO_THEME->theme_path."header.inc");

echo '<form method="get" action="'.$_SERVER['PHP_SELF'].'" name="email_form">';
if (!$print)
{
  echo '<table border="0" cellspacing="0" cellpadding="0"><tr><td class="ModuleIcons">';
  echo '<td class="ModuleIcons">';
  echo "<a href=\"javascript:popup('send.php?account_id=".$account_id."&uid=".$uid."&mailbox=".urlencode($mailbox)."&action=reply','".$GO_CONFIG->composer_width."','".$GO_CONFIG->composer_height."')\"><img src=\"".$GO_THEME->images['reply']."\" border=\"0\" height=\"32\" width=\"32\" /><br />".$ml_reply."</a></td>\n";
  echo '<td class="ModuleIcons">';
  echo "<a href=\"javascript:popup('send.php?account_id=".$account_id."&uid=".$uid."&mailbox=".urlencode($mailbox)."&action=reply_all','".$GO_CONFIG->composer_width."','".$GO_CONFIG->composer_height."')\"><img src=\"".$GO_THEME->images['reply_all']."\" border=\"0\" height=\"32\" width=\"32\" /><br />".$ml_reply_all."</a></td>\n";
  echo '<td class="ModuleIcons">';
  echo "<a href=\"javascript:popup('send.php?account_id=".$account_id."&uid=".$uid."&mailbox=".urlencode($mailbox)."&action=forward','".$GO_CONFIG->composer_width."','".$GO_CONFIG->composer_height."')\"><img src=\"".$GO_THEME->images['forward']."\" border=\"0\" height=\"32\" width=\"32\" /><br />".$ml_forward."</a></td>\n";
  echo '<td class="ModuleIcons">';
  echo "<a href=\"javascript:popup('properties.php?account_id=".$account_id."&uid=".$uid."&mailbox=".urlencode($mailbox)."','450','500')\"><img src=\"".$GO_THEME->images['properties']."\" border=\"0\" height=\"32\" width=\"32\" /><br />".$fbProperties."</a></td>\n";

  echo '<td class="ModuleIcons">';
  echo '<a href="javascript:confirm_delete()"><img src="'.$GO_THEME->images['delete_big'].'" border="0" height="32" width="32" /><br />'.$ml_delete.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a href="javascript:popup(\'message.php?account_id='.$account_id.'&uid='.$uid.'&mailbox='.urlencode($mailbox).'&print=true\',\'\',\'\')"><img src="'.$GO_THEME->images['print'].'" border="0" height="32" width="32" /><br />'.$ml_print.'</a></td>';
  if ($mail->is_imap() && $account['spam'] != '')
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="javascript:spam();"><img src="'.$GO_THEME->images['block'].'" border="0" height="32" width="32" /><br />'.$ml_block.'</a></td>';
  }

  if ($content["previous"] != 0)
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="javascript:get_message('.$content["previous"].');"><img src="'.$GO_THEME->images['previous'].'" border="0" height="32" width="32" /><br />'.$cmdPrevious.'</a></td>';
  }

  if ($content["next"] != 0)
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="javascript:get_message('.$content["next"].');"><img src="'.$GO_THEME->images['next'].'" border="0" height="32" width="32" /><br />'.$cmdNext.'</a></td>';
  }
  echo '<td class="ModuleIcons">';
  echo '<a href="javascript:_close();"><img src="'.$GO_THEME->images['close'].'" border="0" height="32" width="32" /><br />'.$cmdClose.'</a></td>';
  echo '</tr></table>';
}

?>

<input type="hidden" name="empty_mailbox" />
<input type="hidden" name="link_back" value="<?php echo $link_back; ?>" />
<input type="hidden" name="mailbox" value="<?php echo $mailbox; ?>" />
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>" />
<input type="hidden" name="account_id" value="<?php echo $account_id; ?>" />
<input type="hidden" name="uid" value="<?php echo $uid; ?>" />
<input type="hidden" name="first_row" value="<?php echo $first_row; ?>" />
<input type="hidden" name="delete_message_uid" />
<input type="hidden" name="spam_uid" />
<input type="hidden" name="spam_address" />
<input type="hidden" name="task" />
<input type="hidden" name="query" value="<?php echo $query; ?>"/>

<script type="text/javascript">
  <!--
function _close()
{
  document.email_form.link_back.value = '';
  document.email_form.action='<?php echo $return_to; ?>';
  document.email_form.return_to.value = '';
  document.email_form.submit();
}
function confirm_delete()
{
  <?php
    $trash_folder = $account['trash'];
  if ($trash_folder == '' || $trash_folder == $mailbox)
  {
    echo '
      if (confirm("'.$ml_delete_message.'"))
      {
	document.email_form.link_back.value = "";
	document.email_form.delete_message_uid.value = '.$uid.';
	document.email_form.action = "index.php";
	document.email_form.submit();
      }
    ';
  }else
  {
    echo '
      document.email_form.link_back.value = "";
    document.email_form.delete_message_uid.value = '.$uid.';
    document.email_form.action= "index.php";
    document.email_form.submit();
    ';
  }
    ?>
}

function spam()
{
  document.email_form.link_back.value = '';
  document.email_form.spam_address.value = "<?php echo $content["sender"]; ?>";
  document.email_form.spam_uid.value = '<?php echo $uid; ?>';
  document.email_form.action = "index.php";
  document.email_form.submit();
}

function move_mail()
{
  document.email_form.task.value='move_mail';
  document.email_form.submit();
}

function get_message(uid)
{
  document.email_form.uid.value=uid;
  document.email_form.submit();
}

//-->
</script>
<table border="0" width="100%">
<tr>
<td>
<table border="0" cellpadding="1" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td>
<table border="0" cellpadding="1" cellspacing="0" class="TableInside" width="100%">
<?php
switch ($content["priority"])
{
  case "4":
    echo '<tr><td class="Table2"><table border="0" cellpadding="1" cellspacing="1"><tr><td><img src="'.$GO_THEME->images['info'].'" border="0" width="16" height="16" /></td><td class="Success">'.$ml_low_priority.'</td></tr></table></td></tr>';
  break;

  case "2":
    echo '<tr><td class="Table2"><table border="0" cellpadding="1" cellspacing="1"><tr><td><img src="'.$GO_THEME->images['info'].'" border="0" width="16" height="16" /></td><td class="Error">'.$ml_high_priority.'</td></tr></table></td></tr>';
  break;
}
?>
<tr>
<td>
<table border="0" cellpadding="1" cellspacing="0">
<tr>
<td nowrap><b><?php echo $ml_subject; ?>:&nbsp;</b></td>
<td><?php echo htmlspecialchars($subject); ?></td>
</tr>

<tr>
<td nowrap><b><?php echo $ml_from; ?>:&nbsp;</b></td>
<td>
<?php
echo show_profile_by_email($content['sender'], $content['from']).'&nbsp;&lt;'.$content['sender'].'&gt;';
?>
</td>
</tr>
<tr>
<td valign="top" nowrap><b><?php echo $ml_to; ?>:&nbsp;</b></td>
<td>
<?php
$to == "";
if (isset($content["to"]))
{
  for ($i=0;$i<sizeof($content["to"]);$i++)
  {
    if ($i != 0)
    {
      $to .=", ";
    }
    $to .= $content["to"][$i];
  }
}
if ($to == "")
{
  $to = $ml_no_reciepent;
}
echo htmlspecialchars($to);
?>
</td>
</tr>
<?php
if (isset($content["cc"]))
{
  $cc = '';
  for ($i=0;$i<sizeof($content["cc"]);$i++)
  {
    if ($i != 0)
    {
      $cc .=", ";
    }
    $cc .= $content["cc"][$i];
  }
  if ($cc != '')
  {
    echo '<tr><td valign="top"><b>Cc:</b>&nbsp;</td><td>';
    echo htmlspecialchars($cc);
    echo '</td></tr>';
  }
}
if (isset($content["bcc"]))
{
  $bcc = '';
  for ($i=0;$i<sizeof($content["bcc"]);$i++)
  {
    if ($i != 0)
    {
      $bcc .=", ";
    }
    $bcc .= $content["bcc"][$i];
  }
  if ($bcc != '')
  {
    echo '<tr><td valign="top"><b>Bcc:</b>&nbsp;</td><td>';
    echo htmlspecialchars($bcc);
    echo '</td></tr>';
  }
}
?>
<tr>
<td><b><?php echo $strDate; ?>:&nbsp;</b></td>
<td><?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time($content['udate'])); ?></td>
</tr>
<?php
if ($account['type'] == "imap" && !$print)
{
  echo '<tr><td nowrap><b>'.$ml_folder.':&nbsp;</b></td><td>';

  if ($email->get_all_folders($account['id'],true) > 0)
  {
    $dropbox = new dropbox();

    $dropbox->add_value('INBOX',$ml_inbox);

    while ($email->next_record())
    {
      if (!($email->f('attributes')&LATT_NOSELECT))
      {
	$dropbox->add_value($email->f('name'), str_replace('INBOX'.$email->f('delimiter'), '', $email->f('name')));
	//$dropbox->add_value($email->f('name'), $email->f('name'));
      }
    }
    $dropbox->print_dropbox('move_to_mailbox', $mailbox,'onchange="javascript:move_mail()"');
  }
}
echo '</td></tr>';
?>
</table>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td>
<?php
$count = 0;
$splitter = 0;
$parts = array_reverse($mail->f("parts"));

$browser = detect_browser();
$target = $browser['name'] == 'MSIE' ? '_blank' : '_self';
$attachments = "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\"><tr>";

for ($i=0;$i<count($parts);$i++)
{

  if ((eregi("ATTACHMENT", $parts[$i]["disposition"]) && $parts[$i]["name"] != '') || (eregi("INLINE", $parts[$i]["disposition"]) && $parts[$i]["name"] != '') || eregi("message/rfc822", $parts[$i]["mime"]))
  {

    if ($parts[$i]["name"] == "")
    {
      $parts[$i]["name"] = $parts[$i]["mime"];
      $pos = strrpos($parts[$i]["name"] ,'/');
      if ($pos)
      {
	$parts[$i]["name"] = substr($parts[$i]["name"],$pos+1,strlen($parts[$i]["name"]));
      }

      if ($extension = $filetypes->get_mime_extension($parts[$i]["mime"]))
      {
	$parts[$i]["name"] .='.'.$extension;
      }
    }

    $link = "attachment.php?account_id=".$account['id']."&mailbox=".urlencode($mailbox)."&uid=".$uid."&part=".$parts[$i]["number"]."&transfer=".$parts[$i]["transfer"]."&mime=".$parts[$i]["mime"]."&filename=".urlencode($parts[$i]["name"]);

    $splitter++;
    $count++;

    $attachments .= '<td><img border="0" width="16" height="16" src="'.$GO_CONFIG->control_url.'icon.php?extension='.get_extension($parts[$i]["name"]).'&mime='.urlencode($parts[$i]["mime"]).'" /></td>';
    $attachment_name = '';
    $tmp = imap_mime_header_decode($parts[$i]["name"]);
    foreach ($tmp as $t)
      if (isset($t->text))
        $attachment_name .= $t->text;
    $attachments .= '<td valign="center" nowrap>&nbsp;<a href="'.$link.'" target="'.$target.'" title="'.$parts[$i]["name"].'">'.cut_string($attachment_name,50).'</a> ('.format_size($parts[$i]["size"]).')</td>';
    $filesystem_module = $GO_MODULES->get_module('filesystem');
    if ($filesystem_module && $GO_SECURITY->has_permission($GO_SECURITY->user_id, $filesystem_module['acl_read']))
    {
      $attachments .= "<td>&nbsp;<a title=\"".$ml_save_attachment."\" href=\"javascript:popup('save_attachment.php?account_id=".$account['id']."&mailbox=".urlencode($mailbox)."&uid=".$uid."&part=".$parts[$i]["number"]."&transfer=".$parts[$i]["transfer"]."&mime=".$parts[$i]["mime"]."&filename=".urlencode(addslashes($parts[$i]["name"]))."','600','400')\"><img src=\"".$GO_THEME->images['save']."\" border=\"0\" /></a>;&nbsp;</td>\n";
    }else
    {
      $attachments .='<td>;</td>';
    }
    if ($splitter == 3)
    {
      $splitter = 0;
      $attachments .= "</tr><tr>";
    }
  }
}

$attachments .= "</tr></table>";

if ($count>0)
{
  echo '<br /><table border="0" cellpadding="1" cellspacing="0" class="TableBorder" width="100%"><tr><td><table border="0" cellpadding="1" cellspacing="0" class="TableInside" width="100%"><tr><td valign="top">';
  echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td valign="top"><b>'.$ml_attachments.':</b>&nbsp;&nbsp;</td><td>'.$attachments.'</td></tr></table>';
  echo '</td></tr></table></td></tr></table>';
}
?>
</td>
</tr>
<tr>
<td>
<br />
<?php
require_once("smileys.php");
//get all text and html content
for ($i=0;$i<sizeof($parts);$i++)
{
  $mime = strtolower($parts[$i]["mime"]);

  if (($mime == "text/html") || ($mime == "text/plain") || ($mime == "text/enriched"))
  {
    $part = $mail->view_part($uid, $parts[$i]["number"], $parts[$i]["transfer"]);

    switch($mime)
    {
      case 'text/plain':
	$part = text_to_html($part);
	break;

      case 'text/html':
	$part = convert_html($part);
	break;

      case 'text/enriched':
	$part = enriched_to_html($part);
	break;
    }

//    $part = preg_replace_callback ($smiley_patterns, "smiley_symbols_to_images", $part);

    if ($parts[$i]["name"] != '')
    {
      $texts .= "<p class=\"normal\" align=\"center\">--- ".$parts[$i]["name"]." ---</p>";
    }else
    {
      $texts .= '<br /><br /><br />';
    }

    $texts .= $part;
  }
}

//Content-ID's that need to be replaced with urls when message needs to be reproduced
$replace_url = array();
$replace_id = array();
//preview all images

for ($i=0;$i<sizeof($parts);$i++)
{
  if (eregi("image",$parts[$i]["mime"]))
  {
    //when an image has an id it belongs somewhere in the text we gathered above so replace the
    //source id with the correct link to display the image.
    if ($parts[$i]["id"] != '')
    {
      $tmp_id = $parts[$i]["id"];
      if (strpos($tmp_id,'>'))
      {
	$tmp_id = substr($parts[$i]["id"], 1,strlen($parts[$i]["id"])-2);
      }
      $id = "cid:".$tmp_id;
      $url = "attachment.php?account_id=".$account['id']."&mailbox=".$mailbox."&amp;uid=".$uid."&amp;part=".$parts[$i]["number"]."&amp;transfer=".$parts[$i]["transfer"]."&amp;mime=".$parts[$i]["mime"]."&amp;filename=".urlencode($parts[$i]["name"]);
      $texts = str_replace($id, $url, $texts);
    }else
    {
      $images .= "<br /><p class=\"normal\" align=\"center\">--- ".$parts[$i]["name"]." ---</p><div align=\"center\"><img src=\"attachment.php?account_id=".$account['id']."&mailbox=".$mailbox."&uid=".$uid."&part=".$parts[$i]["number"]."&transfer=".$parts[$i]["transfer"]."&mime=".$parts[$i]["mime"]."&filename=".urlencode($parts[$i]["name"])."\" border=\"0\" /></div>";
    }
  }
}
echo $texts.$images;
?>
</td>
</tr>
</table>

<?php
if ($content["notification"] != '' && $content["new"] == 1)
{
  echo "<script type=\"text/javascript\">\npopup('"."notification.php?notification=".urlencode($content["notification"])."&date=".urlencode(date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'],$content['udate']))."&subject=".urlencode($subject)."&to=".urlencode($to)."','500','150');\n</script>\n";
}

$mail->close();

echo '</form>';

if ($print)
{
  echo "\n<script type=\"text/javascript\">\nwindow.print();\n</script>\n";
}
require($GO_THEME->theme_path."footer.inc");
?>
