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

//reset attachments array in case user aborted a message or changed format
if ($_SERVER['REQUEST_METHOD'] != "POST" || $sendaction=='load_template')
{
  if (!isset($_REQUEST['email_file']))
  {
    unset($attach_array);
    unset($num_attach);
    unset($_SESSION['attach_array']);
    unset($_SESSION['num_attach']);
    unset($_SESSION['url_replacements']);
  }
}

//get users email accounts to determine from addresses
$count = $email->get_accounts($GO_SECURITY->user_id);
while ($email->next_record())
{
  if ($mail_from == 0)
  {
    $mail_from = $email->f('id');
  }
  $addresses[] = $email->f("email");
  $names[] = $email->f("email")." (".$email->f("name").")";
  $ids[] = $email->f("id");
}

if ($ab_module)
{
  $ab->enable_contact_selector();
}

$signature = '';
if ($mail_from > 0)
{
  $account = $email->get_account($mail_from);

  if ($account['signature'] != '')
  {
    if ($content_type == 'text/HTML')
    {
      $signature = '<br />'.text_to_html($account['signature']).'<br /><br />';
    }else
    {
      $signature = "\r\n".$account['signature']."\r\n\r\n";
    }
  }
}


//if a uid is given then the user is replying or forwarding
if ($uid > 0 && ($_SERVER['REQUEST_METHOD']  != 'POST' || $sendaction == 'load_template' || $sendaction == 'change_format'))
{
  //get the original message
  require($GO_CONFIG->class_path."imap.class.inc");
  $mail = new imap();

  $account = $email->get_account($_REQUEST['account_id']);

  if ($account && $mail->open($account['host'], $account['type'],$account['port'],$account['username'],$GO_CRYPTO->decrypt($account['password']), $_REQUEST['mailbox']))
  {
    $preferred_type = ($content_type=='text/HTML') ? 'html' : 'text';
    $content = $mail->get_message($uid, $preferred_type,"");
    $parts = array_reverse($mail->f("parts"));

    //fill in the header fields
    $subject = isset($content['subject']) ? $content['subject'] : $ml_no_subject;
    switch($_REQUEST['action'])
    {
      case "reply":
	$mail_to = $content["reply_to"];
	if (!stristr($subject,"Re: "))
	  $mail_subject = "Re: ".$subject;
	else
	  $mail_subject = $subject;
      break;

      case "reply_all":
	$mail_to = $content["reply_to"];
      $mail_subject = "Re: ".$subject;

      //add all recievers from this email
      if (isset($content["to"]))
      {
	for ($i=0;$i<sizeof($content["to"]);$i++)
	{
	  $lt = strpos($content["to"][$i], "&lt") + 4;
	  $gt = strpos($content["to"][$i], "&gt");

	  $length = $gt-$lt;

	  $address = trim(substr($content["to"][$i],$lt,$length));

	  if ($address != "" && !in_array($address,$addresses))
	  {
	    $mail_to .= ",".$address;
	  }
	}
      }

      if (isset($content["cc"]))
      {
	for ($i=0;$i<sizeof($content["cc"]);$i++)
	{
	  $lt = strpos($content["cc"][$i], "&lt") + 4;
	  $gt = strpos($content["cc"][$i], "&gt");

	  $length = $gt-$lt;

	  $address = trim(substr($content["cc"][$i],$lt,$length));

	  if ($address != "" && !in_array($address,$addresses))
	  {
	    if (!isset($first))
	    {
	      $first = true;
	    }else
	    {
	      $mail_cc .= ',';
	    }
	    $mail_cc .= $address;
	  }
	}
      }

      break;

      case "forward":
	//reattach attachments
	$mail_subject = "Fwd: ".$subject;

      for ($i=0;$i<count($parts);$i++)
      {
	if (eregi("attachment", $parts[$i]["disposition"]) && !eregi("message/RFC822", $parts[$i]["mime"]))
	{
	  $file = $mail->view_part($uid, $parts[$i]["number"], $parts[$i]["transfer"], $parts[$i]["mime"]);

	  $name = $parts[$i]['name'] != '' ? $parts[$i]['name'] : 'attach_'.$i;

	  $tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));
	  $fp = fopen($tmp_file,"wb");
	  fwrite ($fp,$file);
	  fclose($fp);
	  $email->register_attachment($tmp_file, $parts[$i]["name"], $parts[$i]["size"], $parts[$i]["mime"], 'attachment', $parts[$i]["id"]);
	}
      }

      break;
    }

    //reatach inline attachements
    for ($i=0;$i<count($parts);$i++)
    {
      if ($parts[$i]["id"] != '')// && eregi("inline", $parts[$i]["disposition"]))
      {
	$file = $mail->view_part($uid, $parts[$i]["number"], $parts[$i]["transfer"], $parts[$i]["mime"]);

	$tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));

	$fp = fopen($tmp_file,"wb");
	fwrite ($fp,$file);
	fclose($fp);

	if (strpos($parts[$i]["id"],'>'))
	{
	  $parts[$i]["id"] = substr($parts[$i]["id"], 1,strlen($parts[$i]["id"])-2);
	}
	$email->register_attachment($tmp_file, $parts[$i]["name"],
	    $parts[$i]["size"], $parts[$i]["mime"], 'inline', $parts[$i]["id"]);
	//Content-ID's that need to be replaced with urls when message is send

	//replace inline images identified by a content id with the url to display the part by Group-Office
	$url_replacement['id'] = $parts[$i]["id"];
	$url_replacement['url'] = "attachment.php?account_id=".$_REQUEST['account_id']."&mailbox=".$_REQUEST['mailbox']."&uid=".$uid."&part=".$parts[$i]["number"]."&transfer=".$parts[$i]["transfer"]."&mime=".$parts[$i]["mime"]."&filename=".urlencode($parts[$i]["name"]);
	$_SESSION['url_replacements'][] = $url_replacement;
      }
    }

    $html_message_count = 0;
    for ($i=0;$i<count($parts);$i++)
    {
      if($content_type=='text/HTML')
      {
	$mime = strtolower($parts[$i]["mime"]);

	if (!eregi("attachment", $parts[$i]["disposition"]))
	{
	  switch ($mime)
	  {
	    case 'text/plain':
	      $html_part = text_to_html($mail->view_part($uid,
		    $parts[$i]["number"], $parts[$i]["transfer"]));
	      $mail_body .= addslashes($html_part);
	      break;

	    case 'text/html':
	      $html_part = get_html_body($mail->view_part($uid,
		    $parts[$i]["number"], $parts[$i]["transfer"]));
	      $mail_body .= addslashes($html_part);
	      break;

	    case 'text/enriched':
	      $html_part = enriched_to_html($mail->view_part($uid,
		    $parts[$i]["number"], $parts[$i]["transfer"]));
	      $mail_body .= addslashes($html_part);
	      break;
	  }
	}
      }else
      {
	if (strtolower($parts[$i]["mime"]) == "text/plain" &&
	    !eregi("ATTACHMENT", $parts[$i]["disposition"]))
	{
	  $text_part = $mail->view_part($uid, $parts[$i]["number"], $parts[$i]["transfer"]);
	  if ($_REQUEST['action'] == 'forward')
	  {
	    $mail_body .= $text_part;
	  }else
	  {
	    $mail_body .= quote($text_part);
	  }
	}

	//add html messages as an attachment since we don't have an html editor to display it coreect yet
	if (strtolower($parts[$i]["mime"]) == "text/html" &&
	    !eregi("ATTACHMENT", $parts[$i]["disposition"]))
	{
	  if ($parts[$i]["name"] == '' && $parts[$i]["mime"] == "text/HTML")
	  {
	    if ($html_message_count == 0)
	      $parts[$i]["name"] = $content["sender"].".html";
	    else
	      $parts[$i]["name"] = $content["sender"]."(".$html_message_count.").html";

	    $html_message_count++;
	  }

	  $file = $mail->view_part($uid, $parts[$i]["number"],
	      $parts[$i]["transfer"], $parts[$i]["mime"]);

	  $tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));

	  $fp = fopen($tmp_file,"w");
	  fwrite ($fp,$file);
	  fclose($fp);

	  $email->register_attachment($tmp_file, $parts[$i]["name"],
	      $parts[$i]["size"], $parts[$i]["mime"]);
	}
      }
    }

    if ($content_type=='text/HTML')
    {
      if ($mail_body != '')
      {
	//replace inline images with the url to display the part by Group-Office
	if (isset($_SESSION['url_replacements']))
	{
	  for ($i=0;$i<count($_SESSION['url_replacements']);$i++)
	  {
	    $mail_body = str_replace('cid:'.$_SESSION['url_replacements'][$i]['id'], $_SESSION['url_replacements'][$i]['url'], $mail_body);
	  }
	}
      }
      $header_om  = '<font face="verdana" size="2">'.$ml_original_follows."<br />";
      $om_to = '';
      if (isset($content))
      {
	$header_om .= "<b>".$ml_subject.":&nbsp;</b>".addslashes($subject)."<br />";
	$header_om .= "<b>".$ml_from.":&nbsp;</b>".$content["sender"]."<br />";
	if (isset($content['to']))
	{
	  for ($i=0;$i<sizeof($content["to"]);$i++)
	  {
	    $lt = strpos($content["to"][$i], "&lt") + 4;
	    $gt = strpos($content["to"][$i], "&gt");

	    $length = $gt-$lt;

	    $address = trim(substr($content["to"][$i],$lt,$length));
	    if ($i!=0)	$om_to .= ';';
	    $om_to .= $address;

	  }
	}else
	{
	  $om_to=$ml_no_reciepent;
	}
	$header_om .= "<b>".$ml_to.":&nbsp;</b>".addslashes($om_to)."<br />";
	$header_om .= "<b>".$strDate.":&nbsp;</b>".date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'],$content["udate"])."<br />";
      }
      $header_om .= "</font><br /><br />";

      $mail_body = $signature.'<br /><blockquote style="border:0;border-left: 2px solid #22437f; padding:0px; margin:0px; padding-left:5px; margin-left: 5px; ">'.$header_om.$mail_body.'</blockquote>';

    }else
    {
      $header_om  = $ml_original_follows."\r\n";
      if (isset($content))
      {
	$header_om .= $ml_subject.": ".$subject."\r\n";
	$header_om .= $ml_from.": ".$content["sender"]."\r\n";
	$om_to = '';
	for ($i=0;$i<sizeof($content["to"]);$i++)
	{
	  $lt = strpos($content["to"][$i], "&lt") + 4;
	  $gt = strpos($content["to"][$i], "&gt");

	  $length = $gt-$lt;

	  $address = trim(substr($content["to"][$i],$lt,$length));
	  if ($i!=0)	$om_to .= ';';
	  $om_to .= $address;

	}
	$header_om .= $ml_to.": ".$om_to."\r\n";
	$header_om .= $strDate.": ".date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'],$content["udate"])."\r\n\r\n\r\n";
      }

      if ($html_message_count > 0)
      {
	$mail_body = $signature.$ml_html_message_attached."\r\n\r\n".$header_om.$mail_body;
      }else
      {
	$mail_body = $signature.$header_om.$mail_body;
      }
    }
  }
  $mail->close();
}else
{
  if($mail_body == '')
  {
    $mail_body = $signature;
  }
}


//replace signatures when user changed his mail account
$old_signature = isset($_REQUEST['old_signature']) ? $_REQUEST['old_signature'] : '';
if($sendaction == 'change_mail_from' && $old_signature != '')
{
  $mail_body = str_replace($old_signature, $signature, $mail_body);
}

//check for the templates plugin
if ($sendaction == 'load_template')
{
  //if contact_id is not set but email is check if there's contact info available
  if ($mail_to != '' && $contact_id == 0)
  {
    if ($contact = $ab->get_contact_profile_by_email($mail_to, $GO_SECURITY->user_id))
    {
      $contact_id = $contact['id'];
    }
  }

  require($GO_CONFIG->class_path.'mimeDecode.class.inc');

  $template_body = '';

  if ($template_id > 0)
  {
    $template = $tp->get_template($template_id);

    //get the raw mime message and decode it.

    $part_number = 0;
    $url_replacements = array();
    $attachments = array();

    $params['include_bodies'] = true;
    $params['decode_bodies'] = true;
    $params['decode_headers'] = true;
    $params['input'] = $template['content'];

    $structure = Mail_mimeDecode::decode($params);
    $notification_check = isset($structure->headers['disposition-notification-to']) ? true : false;

    $_SESSION['url_replacements'] = array();

    //loop through all parts
    if (isset($structure->parts))
    {
      foreach ($structure->parts as $part) {

	$disposition = isset($part->disposition) ? $part->disposition : '';

	//text part and no attachment so it must be the body
	if ($part->ctype_primary == 'text' && $disposition != 'attachment')
	{
	  //convert text to html
	  if (eregi('plain', $part->ctype_primary))
	  {
	    $text_part = nl2br($part->body);
	  }else
	  {
	    $text_part = $part->body;
	  }
	  $template_body .= $text_part;
	}else
	{
	  //save attachments to a temporarily file
	  $tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));

	  $fp = fopen($tmp_file,"w");
	  fwrite ($fp,$part->body);
	  fclose($fp);

	  $filename = isset($part->d_parameters['filename']) ? $part->d_parameters['filename'] : '';

	  $content_id = isset($part->headers['content-id']) ? trim($part->headers['content-id']) : '';
	  if ($content_id != '')
	  {
	    if (strpos($content_id,'>'))
	    {
	      $content_id = substr($part->headers['content-id'], 1,strlen($part->headers['content-id'])-2);
	    }

	    //replace inline images identified by a content id with the url to display the part by Group-Office
	    $url_replacement['id'] = $content_id;
	    $url_replacement['url'] = $ab_module['url'].'templates/mime_part.php?template_id='.$template_id.'&part_number='.$part_number;
	    $_SESSION['url_replacements'][] = $url_replacement;
	  }
	  $email->register_attachment($tmp_file, $filename, strlen($part->body),
	      $part->ctype_primary.'/'.$part->ctype_secondary,
	      $disposition, $content_id);
	}
	$part_number++;
      }
    }elseif(isset($structure->body))
    {
      //convert text to html
      if (eregi('plain', $structure->ctype_primary))
      {
	$text_part = nl2br($structure->body);
      }else
      {
	$text_part = $structure->body;
      }
      $template_body .= $text_part;

    }
    unset($structure);
    //replace inline images with the url to display the part by Group-Office
    if (isset($_SESSION['url_replacements']))
    {
      for ($i=0;$i<count($_SESSION['url_replacements']);$i++)
      {
	$template_body = str_replace('cid:'.$_SESSION['url_replacements'][$i]['id'], $_SESSION['url_replacements'][$i]['url'], $template_body);
      }
    }

    //get the addressbook language file
    require($GO_LANGUAGE->get_language_file('addressbook'));
    if ($mailing_group_id == 0)
    {
      $template_body = $tp->replace_data_fields($template_body, $contact_id);
    }

    if ($content_type=='text/HTML')
    {
      $mail_body = str_replace('</body>', $mail_body.'</body>', $template_body);
    }else
    {
      $mail_body = $template_body.$mail_body;
    }
  }
}

?>
<form name="sendform" enctype="multipart/form-data" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="sendaction" value="" />
  <?php
if($uid > 0)
{
  echo '<input type="hidden" name="uid" value="'.$uid.'" />';
  echo '<input type="hidden" name="account_id" value="'.$_REQUEST['account_id'].'" />';
  echo '<input type="hidden" name="mailbox" value="'.$_REQUEST['mailbox'].'" />';
  echo '<input type="hidden" name="action" value="'.$_REQUEST['action'].'" />';

}
echo '<input type="hidden" name="old_signature" value="'.htmlspecialchars($signature).'" />';

if (isset($feedback)) echo $feedback;
?>
<input type="hidden" name="num_attach" value="<?php if (isset($_SESSION['num_attach'])) echo $_SESSION['num_attach']; ?>" />
<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_attachment_size; ?>" />

<table  cellspacing="2" cellpadding="1" border="0" width="100%">

  <?php
if ($count > 0)
{
  echo '<tr><td>'.$ml_from.':</td><td colspan="2">';
  $dropbox = new dropbox();
  $dropbox->add_arrays($ids, $names);
  $dropbox->print_dropbox('mail_from',$mail_from, 'onchange="javascript:change_mail_from()"');
  echo '</td></tr>';
}

if ($mailing_group_id > 0 && $mailing_group = $tp->get_mailing_group($mailing_group_id))
{
  echo '<input type="hidden" name="mailing_group_id" value="'.$mailing_group_id.'" />';
  echo '<tr><td>'.$ml_to.': </td><td>'.$mailing_group['name'].'</td></tr>';
}else
{
  ?>
    <tr>
    <td>
    <?php
    if ($ab_module)
    {
      echo 	'<a class="normal" href="'.
	$ab->select_contacts('document.sendform.mail_to',
	    $GO_MODULES->url.'add_contacts.php').'">';

      echo 	'<img src="'. $GO_THEME->images['addressbook_small'].
	'" width="16" height="16" border="0" align="absmiddle" '.
	'style="padding-right: 3px;" />'. $ml_to.'</a> :';
    }else
    {
      echo $ml_to.':';
    }
    ?>
      </td>
      <td colspan="2">
      <input class="textbox" type="text" name="mail_to" size="95" value="<?php echo (isset($mail_to) ? htmlspecialchars($mail_to) : ''); ?>" />
      </td>
      </tr>
      <tr>
      <td>
      <?php
      if ($ab_module)
      {
	echo 	'<a class="normal" href="'.
	  $ab->select_contacts('document.sendform.mail_cc',
	      $GO_MODULES->url.'add_contacts.php').'">';

	echo 	'<img src="'. $GO_THEME->images['addressbook_small'].
	  '" width="16" height="16" border="0" align="absmiddle" '.
	  'style="padding-right: 3px;" />CC</a> :';
      }else
      {
	echo 'CC :';
      }
      ?>
	</td>
	<td colspan="2">
	<input class="textbox" type="text" name="mail_cc" size="95" value="<?php echo (isset($mail_cc) ? htmlspecialchars( $mail_cc) : '') ?>" />
	</td>
	</tr>
	<tr>
	<td>
	<?php
	if ($ab_module)
	{
	  echo 	'<a class="normal" href="'.
	    $ab->select_contacts('document.sendform.mail_bcc',
		$GO_MODULES->url.'add_contacts.php').'">';

	  echo 	'<img src="'. $GO_THEME->images['addressbook_small'].
	    '" width="16" height="16" border="0" align="absmiddle" '.
	    'style="padding-right: 3px;" />BCC</a> :';
	}else
	{
	  echo 'BCC :';
	}
	?>
	  </td>
	  <td colspan="2"><input class="textbox" type="text" name="mail_bcc" size="95" value="<?php echo (isset($mail_bcc) ? htmlspecialchars($mail_bcc) : '') ?>" /></td>
	  </tr>
	  <?php
}//endif mailing_group_id
?>
<tr>
<td ><?php echo $ml_subject ?> : </td>
<td colspan="2"><input class="textbox" type="text" name="mail_subject" size="95" value="<?php echo (isset($mail_subject) ? htmlspecialchars($mail_subject) : '') ?>" /></td>
</tr>
<tr>
<td ><?php echo $ml_attachment ?> : </td>
<td colspan="2" nowrap>
<input class="textbox" type="file" name="mail_att" maxlength="200" size="30" value="" />
<?php
$button = new button($ml_attach, 'javascript:attach()');
echo '&nbsp;&nbsp;';
if ($module = $GO_MODULES->get_module('filesystem'))
{
  if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']))
  {
    $button = new button($ml_online_files, "javascript:popup('select_file.php','600','400')", 120);
  }
}
?>
</td>
</tr>
<tr>
<td valign="top"><?php echo $ml_priority ?> : </td>
<td valign="top">
<?php
$priority = isset($_POST['priority']) ? $_POST['priority'] : '3';

$dropbox = new dropbox();
$dropbox->add_value('1',$ml_high);
$dropbox->add_value('3',$ml_normal);
$dropbox->add_value('5',$ml_low);
$dropbox->print_dropbox('priority',$priority);
$notification_check = isset($_POST['notification']) ? true : false;
?>
&nbsp;&nbsp;
</td>
<?php
echo '<td valign="top" ';
if ($wysiwyg) echo 'rowspan="2"';
echo '>';
$checkbox = new checkbox('notification', 'true', $ml_notification, $notification_check);

if ($ab_module && $mailing_group_id == 0 && $ab->get_subscribed_addressbooks($GO_SECURITY->user_id) > 0)
{
  echo '<br />';
  $subscribed_addressbooks = new dropbox();
  $subscribed_addressbooks->add_value('0', $ml_add_recievers);

  while ($ab->next_record())
  {
    $subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));
  }
  $subscribed_addressbooks->print_dropbox('add_recievers', $add_recievers);
}
?>
</td>
</tr>
  <?php
if ($wysiwyg)
{
  echo '<tr><td>'.$ml_format.':</td><td>';
  $dropbox = new dropbox();
  $dropbox->add_value('text/PLAIN', 'text');
  $dropbox->add_value('text/HTML', 'HTML');
  $dropbox->print_dropbox('content_type',$content_type, 'onchange="javascript:change_format()"');
  echo '</td><td></td></tr>';
}else
{
  echo '<input type="hidden" value="text/PLAIN" name="content_type" />';
}

if (isset($_SESSION['num_attach']) && ($_SESSION['num_attach'] > 0))
{
  echo '<tr><td colspan="3">';
  echo '<table border="0" cellspacing="2"><tr>';
  $totalsize = 0;
  $splitter=0;
  $count = 0;
  for ($i=1;$i<=$_SESSION['num_attach'];$i++)
  {
    if ($_SESSION['attach_array'][$i]->disposition == 'attachment')
    {
      $count++;
      $splitter++;
      $totalsize += $_SESSION['attach_array'][$i]->file_size;
      $att_name = imap_mime_header_decode($_SESSION['attach_array'][$i]->file_name);
      echo '<td><table border="0" cellpadding="0" cellspacing="0"><tr><td><input type="checkbox" name="file'.$i.'" /></td><td><img border="0" width="16" height="16" src="'.$GO_CONFIG->control_url.'icon.php?extension='.get_extension($_SESSION['attach_array'][$i]->file_name).'&mime='.urlencode($_SESSION['attach_array'][$i]->file_mime).'" /></td><td nowrap>&nbsp;'.htmlentities($att_name[0]->text).' ('.format_size($_SESSION['attach_array'][$i]->file_size).'); </td></tr></table></td>';
      if ($splitter=="2")
      {
	echo "</tr><tr>\n";
	$splitter=0;
      }
    }
  }
  if ($count > 0)
  {
    echo '<tr><td>';
    $button = new button($cmdDelete, 'javascript:delete_attach()');
    echo '</td><td>'.$ml_total_size.' : '.format_size($totalsize).'</td></tr></table>';
  }
}
echo '</td></tr>';
?>
<tr>
<td colspan="3" width="100%" height="100%">
<?php
$htmlarea->print_htmlarea(stripslashes(htmlspecialchars($mail_body)));
?>
</td>
</tr>
<tr>
<td align="center" colspan="3">
<?php
$button = new button($ml_send,"javascript:send()");
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel,"javascript:window.close()");
?>
</td>
</tr>
</table>
</form>


<script type="text/javascript">
<!--


function validate()
{
  if (window.RegExp)
  {
    var reg = new RegExp("[0-9A-Za-z]+","g");
    if (!reg.test(document.sendform.mail_to.value) && !reg.test(document.sendform.mail_cc.value) && !reg.test(document.sendform.mail_bcc.value))
    {
      alert("<?php echo $ml_to_empty ?>");
      document.sendform.mail_to.focus();
      return (false);
    }
  }
  if (document.sendform.mail_att.value != "")
  {
    alert("<?php echo $ml_attach_forget ?>")
      return (false);
  }
  return (true);
}

function send()
{
  if (document.sendform.sendaction.value != 'send')
  {
    <?php
      if ($mailing_group_id == 0) echo 'if (validate()){';
	?>

	  document.sendform.sendaction.value = "send";
	<?php
	  if (isset($uid) && $uid > 0)
	  {
	    echo 'document.sendform.uid.value="'.$uid.'";';
	  }
	if ($content_type == 'text/HTML')
	{
	  echo 'document.sendform.onsubmit();';
	}
	?>
	  document.sendform.submit();
	<?php if ($mailing_group_id == 0) echo '}'; ?>
      }
  }

  function attach()
  {
    if (document.sendform.mail_att.value != "")
    {
      <?php
	if ($content_type == 'text/HTML')
	{
	  echo 'document.sendform.onsubmit();';
	}
      ?>
	document.sendform.sendaction.value = "add";
      document.sendform.submit();
    }
  }

  function change_format()
  {
    if (confirm('<?php echo $ml_loose_changes; ?>'))
    {
      <?php
	if (isset($uid) && $uid > 0)
	{
	  echo 'document.sendform.uid.value="'.$uid.'";';
	}
      if ($content_type == 'text/HTML')
      {
	echo 'document.sendform.onsubmit();';
      }
      ?>
	document.sendform.mail_body.value='';
      document.sendform.sendaction.value = 'change_format';
      document.sendform.submit();
    }
  }

  function change_mail_from()
  {
    <?php
      if ($content_type == 'text/HTML')
      {
	echo 'document.sendform.onsubmit();';
      }
    ?>
      document.sendform.sendaction.value = 'change_mail_from';
    document.sendform.submit();
  }

  function delete_attach()
  {
    document.sendform.sendaction.value = "delete";
    <?php
      if ($content_type == 'text/HTML')
      {
	echo 'document.sendform.onsubmit();';
      }
    ?>
      document.sendform.submit();
  }

  //-->
  </script>