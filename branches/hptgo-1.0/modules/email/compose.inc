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

/*
   Modifer: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.1 Release date: 15 October 2004
   Note: Customized version for HPT Open Business Management v1.0
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

$mail_list = '';
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

<table cellspacing="2" cellpadding="1" border="0" width="720">

  <?php
if ($count > 0)
{
  if (isset($mail_cc) && trim($mail_cc) != '') {
    $cc_style = 'visibility: visible;';
    $cc_span_style = 'visibility: hidden; display: none;';
  } else {
    $cc_style = 'visibility: hidden; display: none;';
    $cc_span_style = 'visibility: visible;';
  }
  if (isset($mail_bcc) && trim($mail_bcc) != '') {
    $bcc_style = 'visibility: visible;';
    $bcc_span_style = 'visibility: hidden; display: none;';
  } else {
    $bcc_style = 'visibility: hidden; display: none;';
    $bcc_span_style = 'visibility: visible;';
  }
  $cc_span = '<span id="Add_CC" style="position: relative; '.$cc_span_style.'">'.
             '<a href="javascript:show_field(\'CC\')" class="normal">'.
             '<img src="cc.png" border="0" title="'.$ml_cmd_show.' CC" align="absmiddle" />'.
             '</a>&nbsp;</span>';
  $bcc_span = '<span id="Add_BCC" style="position: relative; '.$bcc_span_style.'">'.
              '<a href="javascript:show_field(\'BCC\')" class="normal">'.
              '<img src="bcc.png" border="0" title="'.$ml_cmd_show.' BCC" align="absmiddle" />'.
              '</a></span>';

  echo '<tr><td>'.$ml_from.':</td><td nowrap>';
  $dropbox = new dropbox();
  $dropbox->add_arrays($ids, $names);
  $dropbox->print_dropbox('mail_from',$mail_from, 'onchange="javascript:change_mail_from()"');
  echo '&nbsp;&nbsp;'.$ml_priority.': ';
  $priority = isset($_POST['priority']) ? $_POST['priority'] : '3';

  $dropbox = new dropbox();
  $dropbox->add_value('1',$ml_high);
  $dropbox->add_value('3',$ml_normal);
  $dropbox->add_value('5',$ml_low);
  $dropbox->print_dropbox('priority',$priority);
  $notification_check = isset($_POST['notification']) ? true : false;

  if ($wysiwyg) {
    echo '&nbsp;&nbsp;'.$ml_format.':&nbsp;';
    $dropbox = new dropbox();
    $dropbox->add_value('text/PLAIN', 'TEXT');
    $dropbox->add_value('text/HTML', 'HTML');
    $dropbox->print_dropbox('content_type',$content_type, 'onchange="javascript:change_format()"');
  } else {
    echo '<input type="hidden" value="text/PLAIN" name="content_type" />';
  }
  echo '</td></tr>';

  $db = new db();
  $db->query('SELECT first_name, middle_name, last_name, email FROM users');
  $emails = array();
  $fnames = array();
  while ($db->next_record()) {
    $m = $db->f('middle_name');
    $m = (isset($m) && trim($m) != '') ? ' '.$m.' ' : '';
    $fnames[] = $db->f('last_name').$m.$db->f('first_name');
    $emails[] = trim(strtolower($db->f('email')));
  }
  $db->query('SELECT first_name, middle_name, last_name, email '.
             'FROM ab_contacts '.
             'WHERE ab_contacts.user_id='.$GO_SECURITY->user_id);
  while ($db->next_record()) {
    $em = strtolower($db->f('email'));
    if ($em != '' && !in_array($em, $emails)) {
      $m = $db->f('middle_name');
      $m = (isset($m) && trim($m) != '') ? ' '.$m.' ' : '';
      $fnames[] = $db->f('last_name').$m.$db->f('first_name');
      $emails[] = $em;
    }
  }
  $mail_list = '<span id="maillist" '.
    'style="visibility: hidden; display: none; position: absolute; z-index: +999;">'.
    '<table cellpadding="1" cellspacing="0" border="0" bgcolor="black">'.
    '<tr><td><span id="maillist_contents" style="cursor: hand;"></span></td></tr>'.
    '</table></span>';
}

if ($mailing_group_id > 0 && $mailing_group = $tp->get_mailing_group($mailing_group_id))
{
  echo '<input type="hidden" name="mailing_group_id" value="'.$mailing_group_id.'" />';
  echo '<tr><td>'.$ml_to.': </td><td>'.$mailing_group['name'].'</td></tr>';
} else {
  ?>
    <tr>
    <td nowrap>
    <?php
    if ($ab_module)
    {
      echo '<a class="normal" href="'.$ab->select_contacts('document.sendform.mail_to',
           $GO_MODULES->url.'add_contacts.php').'">';
      echo '<img src="'.$GO_THEME->images['addressbook_small'].
           '" width="16" height="16" border="0" align="absmiddle" '.
           'style="padding-right: 3px;" />'.$ml_to.'</a>: ';
    } else {
      echo $ml_to.': ';
    }
    ?>
      </td>
      <td>
      <input class="textbox" type="text" name="mail_to" size="98" value="<?php echo (isset($mail_to) ? htmlspecialchars($mail_to) : ''); ?>" onkeydown="return display_list(event);" onblur="cleanup(event);" style="width: 600px;" />
      </td>
      <td width="60" nowrap>
      <?php echo $cc_span.$bcc_span; ?>
      </td>
      </tr>
      <tr id="CC" style="<?php echo $cc_style; ?>">
      <td nowrap>
      <?php
      if ($ab_module)
      {
        echo '<a class="normal" href="'.$ab->select_contacts('document.sendform.mail_cc',
             $GO_MODULES->url.'add_contacts.php').'">';
        echo '<img src="'. $GO_THEME->images['addressbook_small'].
             '" width="16" height="16" border="0" align="absmiddle" '.
             'style="padding-right: 3px;" />CC</a>: ';
      } else {
        echo 'CC: ';
      }
      ?>
	</td>
	<td>
	<input class="textbox" type="text" id="mail_cc" name="mail_cc" size="98" value="<?php echo (isset($mail_cc) ? htmlspecialchars($mail_cc) : '') ?>" onkeydown="return display_list(event);" onblur="cleanup(event);" style="width: 600px;" />
	</td>
  <td>
     <a href="javascript:hide_field('CC')"><img src="close.png" border="0" /></a>
  </td>
	</tr>
	<tr id="BCC" style="<?php echo $bcc_style; ?>">
	<td nowrap>
	<?php
	if ($ab_module)
	{
    echo '<a class="normal" href="'.$ab->select_contacts('document.sendform.mail_bcc',
         $GO_MODULES->url.'add_contacts.php').'">';
    echo '<img src="'. $GO_THEME->images['addressbook_small'].
         '" width="16" height="16" border="0" align="absmiddle" '.
         'style="padding-right: 3px;" />BCC</a>: ';
	} else {
	  echo 'BCC: ';
  }
	?>
    </td>
    <td>
    <input class="textbox" type="text" id="mail_bcc" name="mail_bcc" size="98" value="<?php echo (isset($mail_bcc) ? htmlspecialchars($mail_bcc) : '') ?>" onkeydown="return display_list(event);" onblur="cleanup(event);" style="width: 600px;" />
    </td>
    <td>
        <a href="javascript:hide_field('BCC')"><img src="close.png" border="0" /></a>
    </td>
	  </tr>
	  <?php
}//endif mailing_group_id
?>
<tr>
<td nowrap><?php echo $ml_subject ?>: </td>
<td><input class="textbox" type="text" name="mail_subject" size="98" value="<?php echo (isset($mail_subject) ? htmlspecialchars($mail_subject) : '') ?>" style="width: 600px;" /></td>
</tr>
<!--
<tr>
<td>&nbsp;</td>
<?php
echo '<td>';
if ($ab_module && $mailing_group_id == 0 &&
    $ab->get_subscribed_addressbooks($GO_SECURITY->user_id) > 0)
{
  $subscribed_addressbooks = new dropbox();
  $subscribed_addressbooks->add_value('0', $ml_add_recievers);

  while ($ab->next_record())
    $subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));
  $subscribed_addressbooks->print_dropbox('add_recievers', $add_recievers);
}
?>
</td>
</tr>
-->
<tr>
<td colspan="3">&nbsp;</td>
</tr>
<tr>
<td colspan="3">
<table cellspacing="0" cellpadding="0" border="0">
<tr><td nowrap>
<?php
$button = new button($ml_send,"javascript:send()");
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel,"javascript:window.close()");
?>
</td><td align="right" width="100%">
<?php $checkbox = new checkbox('notification', 'true', $ml_notification, $notification_check); ?>
&nbsp;&nbsp;&nbsp;&nbsp;
<span id="attach_button">
<?php
$button = new button($ml_attachment, 'javascript:show_attach()');
?>
</span>
<span id="attach_dialog" style="visibility: hidden; display: none; position: absolute; z-index: 999">
  <table cellpadding="1" cellspacing="0" border="0" bgcolor="black" width="250">
  <tr><td>
    <table cellspacing="0" cellpadding="0" border="0" bgcolor="#CCCCCC" width="100%">
    <?php
    $count = 0;
    $totalsize = 0;
    $attach_list = new dropbox();
    $att_table = '<table cellspacing="1" cellpadding="3" border="0" width="100%">';
    for ($i = 1; $i <= $_SESSION['num_attach']; $i++)
    {
      if ($_SESSION['attach_array'][$i]->disposition == 'attachment')
      {
        $count++;
        $totalsize += $_SESSION['attach_array'][$i]->file_size;
        $att_name = imap_mime_header_decode($_SESSION['attach_array'][$i]->file_name);
        $att_table .= '<tr bgcolor="white">'.
                      '<td><img border="0" width="16" height="16" src="'.$GO_CONFIG->control_url.'icon.php?extension='.get_extension($_SESSION['attach_array'][$i]->file_name).'&mime='.urlencode($_SESSION['attach_array'][$i]->file_mime).'" /></td>';
        $att_table .= '<td width="100%">'.$att_name[0]->text.'</td>';
        $att_table .= '<td align="right" nowrap>'.
                      format_size($_SESSION['attach_array'][$i]->file_size).'</td>';
        $att_table .= '<td><a href="javascript:delete_attach('.$i.')">'.
                      '<img src="'.$GO_THEME->images['delete'].'" border="0" />'.
                      '</a></td>';
        $att_table .= '</tr>';
      }
    }
    if ($count == 0)
      $att_table .= '<tr bgcolor="white">'.
                    '<td align="center"><i>'.$ml_attachment_empty.'</i></td></tr>';
    $att_table .= '</table>';
    echo '<tr class="TableHead2" height="20">'.
         '<td><img src="attach.png" border="0" /></td>'.
         '<td nowrap>&nbsp;'.$ml_attachment.': '.$count.' file'.($count > 1 ? 's' : '').'</td>'.
         '<td nowrap align="right">&nbsp;&nbsp;&nbsp;&nbsp;'.
         $ml_total_size.': '.format_size($totalsize).'&nbsp;</td>'.
         '</tr>'.
         '<tr><td colspan="3" width="100%">'.$att_table.'</td></tr>';
    ?>
    <tr bgcolor="white">
    <td colspan="3" nowrap>
      <table border="0" cellpadding="3" cellspacing="0" width="100%">
      <tr><td align="center">
      <?php
      $button = new button($ml_attach, 'javascript:attach()');
      echo '&nbsp;&nbsp;';
      if ($module = $GO_MODULES->get_module('filesystem'))
      {
        if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) ||
            $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']))
        {
          $button = new button($ml_online_files,
              "javascript:popup('select_file.php','600','400')", 120);
        }
      }
      ?>
      </td></tr>
      <tr><td align="center">
      <input class="textbox" type="file" name="mail_att" maxlength="200" size="22" value="" />
      </td></tr>
      </table>
    </td>
    </tr>
    </table>
  </td></tr>
  </table>
</span>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td colspan="3" width="100%" height="100%">
<?php
$htmlarea->print_htmlarea(stripslashes(htmlspecialchars($mail_body)));
?>
</td>
</tr>
<tr>
<td colspan="3">
<?php
echo $mail_list;
$button = new button($ml_send,"javascript:send()");
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel,"javascript:window.close()");
?>
<input type="hidden" name="attach_dialog_visible" value="" />
<input type="hidden" name="delete_attached_file" value="" />
<!--
<input class="textbox" type="file" name="mail_att" maxlength="200" size="3" value="" style="visibility: hidden; opacity: 0; filter:alpha(opacity: 0);" />
-->
</td>
</tr>
</table>
</form>

<script type="text/javascript">
<!--
var emails = ['<?php echo implode("','", $emails); ?>'];
var fnames = ['<?php echo implode("','", $fnames); ?>'];
var agent = navigator.userAgent.toLowerCase();
var is_ie = ((agent.indexOf("msie") != -1) && (agent.indexOf("opera") == -1));
var dialog = document.getElementById('attach_dialog');

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
    var fn = ['CC', 'BCC'];
    var ff = ['mail_cc', 'mail_bcc'];

    for (i = 0; i < fn.length; i++) {
      field = document.getElementById(fn[i]);
      if (field.style.visibility == 'hidden')
        document.getElementById(ff[i]).value = '';
    }
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

function show_attach()
{
  if (dialog.style.visibility == 'visible') {
    hide_attach();
    return;
  }
  var button = document.getElementById('attach_button');
  var p = get_position(button);
  dialog.style.top = p.y + button.offsetHeight;
  dialog.style.left = '-1000px';
  dialog.style.display = '';
  dialog.style.visibility = 'visible';
  document.sendform.attach_dialog_visible.value = '1';
  dialog.style.left = (p.x + 100 - dialog.offsetWidth) + "px";
}

function hide_attach()
{
  dialog.style.display = 'none';
  dialog.style.visibility = 'hidden';
  document.sendform.attach_dialog_visible.value = '0';
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

function delete_attach(n)
{
  document.sendform.sendaction.value = "delete";
  <?php
    if ($content_type == 'text/HTML')
    {
      echo 'document.sendform.onsubmit();';
    }
  ?>
  document.sendform.delete_attached_file.value = n;
  document.sendform.submit();
}

function show_field(name)
{
  var field = document.getElementById(name);
  var link = document.getElementById("Add_"+name);
  if (field.style.visibility == 'hidden') {
    field.style.display = '';
    field.style.visibility = 'visible';
    link.style.display = 'none';
    link.style.visibility = 'hidden';
  }
  hide_attach();
}

function hide_field(name)
{
  var field = document.getElementById(name);
  var link = document.getElementById("Add_"+name);
  if (field.style.visibility == 'visible') {
    field.style.display = 'none';
    field.style.visibility = 'hidden';
    link.style.display = '';
    link.style.visibility = 'visible';
  }
  hide_attach();
}

function get_position(element)
{
  var elem = element, tagname = "", x = 0, y = 0;

  while ((typeof(elem) == "object") && (typeof(elem.tagName) != "undefined"))
  {
    y += elem.offsetTop;
    x += elem.offsetLeft;
    tagname = elem.tagName.toUpperCase();

    if (tagname == "BODY") elem = 0;
    if (typeof(elem) == "object")
      if (typeof(elem.offsetParent) == "object")
        elem=elem.offsetParent;
  }

  position = new Object();
  position.x = x;
  position.y = y;
  return position;
}

function get_target(event)
{
  if (event == null)
    event = window.event;

  if (event == null)
    return null;

  if (event.srcElement != null)
    return event.srcElement;

  var r = event.target;
  while (r && r.nodeType != 1)
    r = r.parentNode;

  return r;
}

var sel_top = 0;
var sel_idx = 0;
var sel_row = null;
var sel_emails = null;

function highlight(r)
{
  restore();
  sel_row = document.getElementById('sel_'+r);
  sel_row.style.backgroundColor = '#CCCCCC';
  sel_idx = r;
}

function restore()
{
  if (sel_row) {
    sel_row.style.backgroundColor = 'white';
    sel_row = null;
  }
}

var field = null;
function add_email(r)
{
  var s = '';
  var m = field.value.split(',');
  for (i = 0; i < m.length - 1; i++)
    s += m[i].replace(/^\s+|\s+$/g, '') + ', ';
  s += sel_emails[r] + ', ';
  field.value = s;
  hide_maillist();
}

function cancel_event(e)
{
  e.returnValue = false;
  if (e.preventDefault) e.preventDefault();
}

function show_sel(n)
{
  sel = document.getElementById('sel_'+n);
  sel.style.display = '';
  sel.style.visibility = 'visible';
}

function hide_sel(n)
{
  sel = document.getElementById('sel_'+n);
  sel.style.display = 'none';
  sel.style.visibility = 'hidden';
}

function adjust_sel(n)
{
  restore();
  sel_idx += n;
  if (sel_idx >= sel_emails.length) {
    sel_idx = 0;
    if (sel_top > 0) {
      for (i = 0; i < 4; i++) show_sel(i);
      for (i = 4; i < sel_emails.length; i++) hide_sel(i);
    }
    sel_top = 0;
  }
  else
  if (sel_idx >= sel_top + 4) {
    sel_top = sel_idx - 3;
    for (i = 0; i < sel_top; i++) hide_sel(i);
    for (i = sel_top; i < sel_top + 4; i++) show_sel(i);
    for (i = sel_top + 4; i < sel_emails.length; i++) hide_sel(i);
  }
  else
  if (sel_idx < 0) {
    sel_idx = sel_emails.length - 1;
    sel_top = sel_idx - 3;
    if (sel_top < 0)
      sel_top = 0;
    else {
      for (i = 0; i < sel_top; i++) hide_sel(i);
      for (i = sel_top; i < sel_emails.length; i++) show_sel(i);
    }
  }
  else
  if (sel_idx < sel_top) {
    sel_top = sel_idx;
    for (i = 0; i < sel_top; i++) hide_sel(i);
    for (i = sel_top; i < sel_top + 4; i++) show_sel(i);
    for (i = sel_top + 4; i < sel_emails.length; i++) hide_sel(i);
  }
  highlight(sel_idx);
}

var up_btn = '<a href="javascript:adjust_sel(-1)"><img src="up.png" border="0" /></a>';
var dn_btn = '<a href="javascript:adjust_sel(1)"><img src="down.png" border="0" /></a>';
var ml_visible = false;
function display_list(e)
{
  if (ml_visible)
  switch (e.keyCode) {
    case 9: // tab
    case 13: // enter
    case 39: // right arrow
      add_email(sel_idx);
      cancel_event(e);
      return false;
    case 35: // end
      adjust_sel(sel_emails.length - sel_idx - 1);
      return false;
    case 36: // home
      adjust_sel(-sel_idx);
      return false;
    case 38: // up
      adjust_sel(-1);
      return false;
    case 40: // down
      adjust_sel(1);
      return false;
    case 27: // esc
    case 44: // comma
    case 188:
      hide_maillist();
      return false;
  }

  var f = get_target(e);
  var m = f.value.split(',');
  var n = emails.length;
  var ml = document.getElementById('maillist');

  field = f;
  switch (e.keyCode) {
    case 0: // unknown
    case 9: // tab
    case 16: // shift
    case 17: // ctrl
    case 18: // alt
    case 20: // caps-lock
    case 27: // esc
      return true;
    case 8: // backspace
      m = m[m.length - 1].replace(/.$/g, '');
      break;
    default:
      m = m[m.length - 1] + String.fromCharCode(e.keyCode);
  }
  m = m.replace(/^\s+|\s+$/g, '').toLowerCase();
  if (m == '' || m.length < 1) {
    hide_maillist();
    return true;
  }

  var contents = "";
  sel_emails = new Array();
  for (i = 0; i < n; i++) {
    var name = fnames[i].toLowerCase();
    var match = name.indexOf(m) == 0;
    if (!match) {
      var j = 0;
      name = name.split(' ');
      while (!match && j < name.length)
        match = name[j++].indexOf(m) == 0;
      if (!match) {
        var em = emails[i].toLowerCase();
        match = em.indexOf(m) == 0;
      }
    }

    if (match) {
      var z = sel_emails.length;
      var fn_add_email = 'javascript:add_email('+z+')';
      var mouse_events = 'onmouseover="highlight('+z+')" onmouseout="restore()" '+
                         'onclick="'+fn_add_email+'"';
      var v = z < 4 ? 'style="visibility: visible;"' : 'style="visibility: hidden; display: none;"';
      contents += '<tr id="sel_'+z+'" '+mouse_events+' '+v+'>'+
        '<td nowrap>&nbsp;'+fnames[i]+'&nbsp;</td>'+
        '<td nowrap>&lt;'+emails[i]+'&gt;</td></tr>';
      sel_emails[z] = emails[i];
    }
  }

  if (contents != '') {
    var p = get_position(field);
    var mc = document.getElementById('maillist_contents');
    contents = '<table cellspacing="0" cellpadding="0" border="0" bgcolor="white" width="320">'
               +contents+
               '</table>';
    var updn = sel_emails.length > 4 ? '<td>'+up_btn+'<br/><br/>'+dn_btn+'</td>' : '';
    contents = '<table cellspacing="2" cellpadding="1" border="0" bgcolor="white">'+
               '<tr><td rowspan="4">'+contents+'</td>'+updn+'</tr>'+
               '</table>';
    mc.innerHTML = contents;
    ml.style.top = (p.y + field.offsetHeight) + "px";
    ml.style.left = p.x + "px";
    ml.style.display = '';
    ml.style.visibility = 'visible';
    highlight(sel_idx = 0);
    ml_visible = true;
  }
  else if (ml_visible) {
    hide_maillist();
  }

  return true;
}

function hide_maillist()
{
  ml_visible = false;
  ml = document.getElementById('maillist');
  ml.style.display = 'none';
  ml.style.visibility = 'hidden';
  field.focus();
}

function cleanup(e)
{
  var f = get_target(e);
  var s = f.value.replace(/^\s+|\s+$|,\s+$/g, '');
  if (f.value != s) f.value = s;
}

<?php
if ($_REQUEST['attach_dialog_visible'] == '1')
  echo 'window.setTimeout("show_attach()", 1000);';
?>
//-->
</script>
