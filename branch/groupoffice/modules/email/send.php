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
require($GO_LANGUAGE->get_language_file('email'));

require($GO_CONFIG->class_path."phpmailer/class.phpmailer.php");
require($GO_CONFIG->class_path."phpmailer/class.smtp.php");
require($GO_CONFIG->class_path."html2text.class.inc");
require($GO_MODULES->class_path."email.class.inc");
$email = new email();
$tp_plugin = $GO_MODULES->get_plugin('templates', 'addressbook');
if ($tp_plugin)
{
  require($tp_plugin['class_path'].'templates.class.inc');
  $tp = new templates();
}

//check for the addressbook module
$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || !($GO_SECURITY->has_permission($GO_SECURITY->user_id, 
																									$ab_module['acl_read']) || 
	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
  $ab_module = false;
}else
{
  require_once($ab_module['class_path'].'addressbook.class.inc');
  $ab = new addressbook();
}

$html_mail_head = '<html><head><meta content="Group-Office '.
									$GO_CONFIG->version.'" name="GENERATOR"></head><body>';
$html_mail_foot = '</body></html>';

$_SESSION['num_attach'] = isset($_SESSION['num_attach']) ? $_SESSION['num_attach'] : 0;

$mail_subject = isset($_REQUEST['mail_subject']) ? smartstrip($_REQUEST['mail_subject']) : '';
$mail_body = isset($_REQUEST['mail_body']) ? smartstrip($_REQUEST['mail_body']) : '';
$mail_from = isset($_REQUEST['mail_from']) ? $_REQUEST['mail_from'] : 0;
$mail_to = isset($_REQUEST['mail_to']) ? $_REQUEST['mail_to'] : '';
$mail_cc = isset($_REQUEST['mail_cc']) ? $_REQUEST['mail_cc'] : '';
$mail_bcc = isset($_REQUEST['mail_bcc']) ? $_REQUEST['mail_bcc'] : '';
$uid = isset($_REQUEST['uid']) ? $_REQUEST['uid'] : 0;

$add_recievers = isset($_POST['add_recievers']) ? $_POST['add_recievers']: '';

$mailing_group_id = isset($_REQUEST['mailing_group_id']) ? $_REQUEST['mailing_group_id'] : 0;

$htmlarea = new htmlarea();
if ($htmlarea->browser_is_supported())
{
  $wysiwyg = true;
  $em_settings = $email->get_settings($GO_SECURITY->user_id);
  $content_type = isset($_POST['content_type']) ? $_POST['content_type'] : $em_settings['send_format'];

  if ($content_type == 'text/PLAIN')
  {
    $htmlarea->force_textmode();
  }
}else
{
  $content_type = 'text/PLAIN';
  $wysiwyg = false;
}

$page_title = $ml_compose;
$sendaction = isset($_REQUEST['sendaction']) ? $_REQUEST['sendaction'] : '';
$attachments_size = 0;

function add_unknown_reciepent($email, $addressbook)
{
  global $GO_SECURITY, $ab;

  $known = false;
  $ab->search_contacts($GO_SECURITY->user_id, $email, 'email');
  while($ab->next_record())
  {
    if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_read')) ||
	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab->f('acl_write')))
    {
      $known = true;
      break;
    }
  }

  if (!$known)
  {
    $acl_read = $GO_SECURITY->get_new_acl('contact read');
    $acl_write = $GO_SECURITY->get_new_acl('contact write');

    if ($acl_read > 0 && $acl_write > 0)
    {
      if (!$ab->add_contact(0, $GO_SECURITY->user_id, $addressbook['id'], '', '', $email, '', '', 'M'
	    , '', $email, "", "", "", "", "", "", "", "", "", 0, "", "",
	    "", "", 0, '', $acl_read, $acl_write))
      {
	$GO_SECURITY->delete_acl($acl_read);
	$GO_SECURITY->delete_acl($acl_write);
      }else
      {
	$GO_SECURITY->copy_acl($addressbook['acl_read'] , $acl_read);
	$GO_SECURITY->copy_acl($addressbook['acl_write'] , $acl_write);
      }
    }
  }
}

switch ($sendaction)
{
  case 'add':
    //Adding the new file to the array
    if (is_uploaded_file($_FILES['mail_att']['tmp_name']))
    {
      // Counting the attachments number in the array
      if (isset($_SESSION['attach_array']))
      {
	for($i=1;$i<=count($_SESSION['attach_array']);$i++)
	{
	  $attachments_size += $_SESSION['attach_array'][$i]->file_size;
	}
      }
      $attachments_size += $_FILES['mail_att']['size'];
      if ($attachments_size < $GO_CONFIG->max_attachment_size)
      {
	$tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));
	copy($_FILES['mail_att']['tmp_name'], $tmp_file);
	$email->register_attachment($tmp_file, basename($_FILES['mail_att']['name']), $_FILES['mail_att']['size'], $_FILES['mail_att']['type']);
      }else
      {
	$feedback = '<script type="text/javascript">alert("'.$ml_file_too_big.format_size($GO_CONFIG->max_attachment_size).' ('.number_format($GO_CONFIG->max_attachment_size, 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']).' bytes)");</script>';
      }
    }else
    {
      $feedback = '<script type="text/javascript">alert("'.$ml_file_too_big.format_size($GO_CONFIG->max_attachment_size).' ('.number_format($GO_CONFIG->max_attachment_size, 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']).' bytes)");</script>';
    }
    break;

  case 'send':
    if (!isset($_POST['mail_from']))
    {
      $profile = $GO_USERS->get_user($GO_SECURITY->user_id);
      $middle_name = $profile['middle_name'] == '' ? '' : $profile['middle_name'].' ';
      $name = $profile['last_name'].' '.$middle_name.$profile['first_name'];
    }else
    {
      $profile = $email->get_account($_POST['mail_from']);
      $name = $profile["name"];
    }

    $mail = new PHPMailer();
    $mail->PluginDir = $GO_CONFIG->class_path.'phpmailer/';
    $mail->SetLanguage($php_mailer_lang, $GO_CONFIG->class_path.'phpmailer/language/');

    switch($GO_CONFIG->mailer)
    {
      case 'smtp':
	$mail->Host = $GO_CONFIG->smtp_server;
	$mail->Port = $GO_CONFIG->smtp_port;
	$mail->IsSMTP();			  
	break;			
      case 'qmail':
	$mail->IsQmail();
	break;			
      case 'sendmail':
	$mail->IsSendmail();
	break;
      case 'mail':
	$mail->IsMail();
	break;
    }
    $mail->Priority = $_POST['priority'];
    $mail->Sender     = $profile["email"];    
    $mail->From     = $profile["email"];
    $mail->FromName = $name;
    $mail->AddReplyTo($profile["email"],$name);
    $mail->WordWrap = 50;
    //$mail->Encoding = "quoted-printable";

    if (isset($_POST['notification']))
    {
      $mail->ConfirmReadingTo = $profile["email"];
    }
    $html_message = $content_type == 'text/HTML' ? true : false;
    $mail->IsHTML($html_message);
    $mail->Subject = smartstrip(trim($mail_subject));

    if (isset($_SESSION['url_replacements']))
    {
      while($url_replacement = array_shift($_SESSION['url_replacements']))
      {
	$mail_body=str_replace($url_replacement['url'], "cid:".$url_replacement['id'], $mail_body);
      }
      unset($_SESSION['url_replacements']);
    }

    // Getting the attachments
    if (isset($_SESSION['attach_array']))
    {
      for ($i=1;$i<=$_SESSION['num_attach'];$i++)
      {
	// If the temporary file exists, attach it
	$tmp_file = stripslashes($_SESSION['attach_array'][$i]->tmp_file);
	if (file_exists($tmp_file))
	{
	  if ($_SESSION['attach_array'][$i]->disposition == 'attachment' || strpos($mail_body, $_SESSION['attach_array'][$i]->content_id))
	  {
	    if ($_SESSION['attach_array'][$i]->disposition == 'attachment')
	    {
	      $mail->AddAttachment($tmp_file, imap_qprint($_SESSION['attach_array'][$i]->file_name), 'base64',  $_SESSION['attach_array'][$i]->file_mime) ;
	    }else
	    {
	      $mail->AddEmbeddedImage($tmp_file, $_SESSION['attach_array'][$i]->content_id, imap_qprint($_SESSION['attach_array'][$i]->file_name), 'base64',  $_SESSION['attach_array'][$i]->file_mime);
	    }
	  }
	}
      }
    }


    if ($mailing_group_id > 0)
    {
      $feedback = '';
      $tp->get_contacts_from_mailing_group($mailing_group_id);
      while($tp->next_record())
      {
	$mail->ClearAllRecipients();
	//add the body
	$content = $tp->replace_data_fields($mail_body, $tp->f('id'));
	if ($html_message)
	{
	  $mail->Body = $html_mail_head.$content.$html_mail_foot;
	  $h2t =& new html2text($content);
	  $mail->AltBody  = $h2t->get_text();
	}else
	{
	  $mail->Body = $content;
	}

	$mail->AddAddress($tp->f('email'));

	if(!$mail->Send())
	{
	  $feedback .= $tp->f('email').': '.$mail->ErrorInfo.'<br />';
	}
      }

      $tp->get_companies_from_mailing_group($mailing_group_id);
      while($tp->next_record())
      {
	$mail->ClearAllRecipients();

	//add the body
	$content = $tp->replace_company_data_fields($mail_body, $tp->f('id'));
	if ($html_message)
	{
	  $mail->Body = $html_mail_head.$content.$html_mail_foot;
	  $h2t =& new html2text($content);
	  $mail->AltBody  = $h2t->get_text();
	}else
	{
	  $mail->Body = $content;
	}

	$mail->AddAddress($tp->f('email'));

	if(!$mail->Send())
	{
	  $feedback .= $tp->f('email').': '.$mail->ErrorInfo.'<br />';
	}
      }

      if ($feedback != '')
      {
	$feedback = '<p class="Error">'.$feedback.'</p>';
      }else
      {
	if (isset($_SESSION['attach_array']))
	{
	  while($attachment = array_shift($_SESSION['attach_array']))
	  {
	    @unlink($attachment->tmp_file);
	  }
	}
	// We need to unregister the attachments array and num_attach
	$_SESSION['num_attach'] = 0;
	$_SESSION['attach_array'] = array();

	echo "<script type=\"text/javascript\">\r\nwindow.close();\r\n</script>\r\n";
	exit();
      }
    }else
    {
      $mail_to_array = cut_address(trim($mail_to), $charset);
      $mail_cc_array = cut_address(trim($mail_cc), $charset);
      $mail_bcc_array = cut_address(trim($mail_bcc), $charset);

      if ($add_recievers > 0)
      {
	$add_reciever_ab = $ab->get_addressbook($add_recievers);
      }else
      {
	$add_reciever_ab = false;
      }

      while ($to_address = array_shift($mail_to_array))
      {
	$mail->AddAddress($to_address);
	if ($add_reciever_ab)
	{
	  add_unknown_reciepent($to_address, $add_reciever_ab);
	}
      }
      while ($cc_address = array_shift($mail_cc_array))
      {
	$mail->AddCC($cc_address);
	if ($add_reciever_ab)
	{
	  add_unknown_reciepent($cc_address, $add_reciever_ab);
	}
      }
      while ($bcc_address = array_shift($mail_bcc_array))
      {
	$mail->AddBCC($bcc_address);
	if ($add_reciever_ab)
	{
	  add_unknown_reciepent($bcc_address, $add_reciever_ab);
	}
      }

      if ($html_message)
      {
	$mail->Body = $html_mail_head.$mail_body.$html_mail_foot;
	$htmlToText = new Html2Text ($mail_body);
	$mail->AltBody = $htmlToText->get_text();
      }else
      {
	$mail->Body = $mail_body;
      }

      if(!$mail->Send())
      {
	$feedback = '<p class="Error">'.$ml_send_error.' '.$mail->ErrorInfo.'</p>';

      }else
      {
	//set Line enidng to \r\n for Cyrus IMAP
	$mail->LE = "\r\n";
	$mime = $mail->GetMime();

	if (isset($_SESSION['attach_array']))
	{
	  while($attachment = array_shift($_SESSION['attach_array']))
	  {
	    @unlink($attachment->tmp_file);
	  }
	}
	// We need to unregister the attachments array and num_attach
	unset($_SESSION['num_attach']);
	unset($_SESSION['attach_array']);

	if ($profile["type"] == "imap")
	{
	  $sent_folder = $profile['sent'];
	  if ($sent_folder != '')
	  {
	    require($GO_CONFIG->class_path."imap.class.inc");
	    $imap_stream = new imap();
	    if ($imap_stream->open($profile["host"], "imap", $profile["port"], $profile["username"], $GO_CRYPTO->decrypt($profile["password"]), $sent_folder))
	    {

	      if ($imap_stream->append_message($sent_folder, $mime,"\\Seen"))
	      {
		if (isset($_REQUEST['action']) && ($_REQUEST['action']== "reply" || $_REQUEST['action'] == "reply_all"))
		{
		  $uid = array($_REQUEST['uid']);
		  $imap_stream->set_message_flag($_POST['mailbox'], $uid, "\\Answered");
		}

		$imap_stream->close();
		require($GO_THEME->theme_path."header.inc");
		echo "<script type=\"text/javascript\">\r\nwindow.close();\r\n</script>\r\n";
		require($GO_THEME->theme_path."footer.inc");
		exit();
	      }
	    }
	    require($GO_THEME->theme_path."header.inc");
	    echo "<script type=\"text/javascript\">\r\nalert('".$ml_sent_items_fail."');\r\nwindow.close();\r\n</script>\r\n";
	    require($GO_THEME->theme_path.'footer.inc');
	    exit();
	  }else
	  {
	    require($GO_THEME->theme_path."header.inc");
	    echo "<script type=\"text/javascript\">\r\nwindow.close();\r\n</script>\r\n";
	    require($GO_THEME->theme_path.'footer.inc');
	    exit();
	  }
	}else
	{
	  require($GO_THEME->theme_path.'header.inc');
	  echo "<script type=\"text/javascript\">\r\nwindow.close();\r\n</script>\r\n";
	  require($GO_THEME->theme_path.'footer.inc');
	  exit();
	}

      }
    }

    break;

  case 'delete':
    // Rebuilding the attachments array with only the files the user wants to keep
    $tmp_array = array();
    for ($i=$j=1;$i<=$_SESSION['num_attach'];$i++)
    {
      $thefile = 'file'.$i;
      if (empty($_POST[$thefile]))
      {
	$tmp_array[$j]->file_name = $_SESSION['attach_array'][$i]->file_name;
	$tmp_array[$j]->tmp_file = $_SESSION['attach_array'][$i]->tmp_file;
	$tmp_array[$j]->file_size = $_SESSION['attach_array'][$i]->file_size;
	$tmp_array[$j]->file_mime = $_SESSION['attach_array'][$i]->file_mime;
	$tmp_array[$j]->content_id = $_SESSION['attach_array'][$i]->content_id;
	$tmp_array[$j]->disposition = $_SESSION['attach_array'][$i]->disposition;
	$j++;
      }else
      {
	@unlink($_SESSION['attach_array'][$i]->tmp_file);
      }
    }

    // Removing the attachments array from the current session
    unset($_SESSION['num_attach']);
    unset($_SESSION['attach_array']);
    $_SESSION['attach_array'] = $tmp_array;
    $_SESSION['num_attach'] = ($j > 1 ? $j - 1 : 0);
    break;

  case 'change_format':
    $email->save_send_format($GO_SECURITY->user_id, $content_type);
    break;
}

//check for the templates plugin
//if a template id is given then process it
$template_id = isset($_REQUEST['template_id']) ? $_REQUEST['template_id'] : 0;
$contact_id = isset($_REQUEST['contact_id']) ? $_REQUEST['contact_id'] : 0;


if($mailing_group_id > 0 && $tp->get_contacts_from_mailing_group($mailing_group_id) == 0 && $tp->get_companies_from_mailing_group($mailing_group_id) == 0)
{
  require($GO_THEME->theme_path."header.inc");
  $tabtable = new tabtable('templates_tab', $ml_attention, '600', '120');
  $tabtable->print_head();
  echo '<p>'.$ml_no_contacts_in_mailing_group.'</p><br />';
  $button = new button($cmdClose, "javascript:window.close();");
  $tabtable->print_foot();
  require($GO_THEME->theme_path."footer.inc");
  exit();
}

if ($tp_plugin)
{
  $template_count = $tp->get_subscribed_templates($GO_SECURITY->user_id, EMAIL_TEMPLATE);
}

if ($_SERVER['REQUEST_METHOD'] != "POST" && $tp_plugin && $template_id == 0 && $template_count > 0)
{
  require($GO_THEME->theme_path."header.inc");
  echo '<form name="sendform" method="post" action="'.$_SERVER['PHP_SELF'].'">';
  if($uid > 0)
  {
    echo '<input type="hidden" name="account_id" value="'.$_REQUEST['account_id'].'" />';
    echo '<input type="hidden" name="uid" value="'.$uid.'" />';
    echo '<input type="hidden" name="mailbox" value="'.$_REQUEST['mailbox'].'" />';
    echo '<input type="hidden" name="action" value="'.$_REQUEST['action'].'" />';
  }
  echo '<input type="hidden" name="mail_subject" value="'.$mail_subject.'" />';
  echo '<input type="hidden" name="mail_body" value="'.smartstrip($mail_body, true).'" />';
  echo '<input type="hidden" name="mail_to" value="'.$mail_to.'" />';
  echo '<input type="hidden" name="mail_cc" value="'.$mail_cc.'" />';
  echo '<input type="hidden" name="mail_bcc" value="'.$mail_bcc.'" />';
  echo '<input type="hidden" name="mail_from" value="'.$mail_from.'" />';
  echo '<input type="hidden" name="contact_id" value="'.$contact_id.'" />';
  echo '<input type="hidden" name="template_id" />';
  echo '<input type="hidden" name="mailing_group_id" value="'.$mailing_group_id.'" />';
  echo '<input type="hidden" name="sendaction" value="load_template" />';

  //get the addressbook language file
  echo '<table border="0" width="100%"><tr><td align="center">';
  require($GO_LANGUAGE->get_language_file('addressbook'));
  $tabtable = new tabtable('templates_tab', $ab_templates, '600', '400');
  $tabtable->print_head();

  echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td>';
  echo $ab_select_template;
  echo '<table border="0" cellpadding="2">';

  echo '<tr><td><a class="normal" href="javascript:document.forms[0].template_id.value=\'0\';document.forms[0].submit();">'.$ab_no_template.'</a></td></tr>';

  while($tp->next_record())
  {
    echo '<tr><td><a class="normal" href="javascript:document.forms[0].template_id.value=\''.$tp->f('id').'\';document.forms[0].submit();">'.$tp->f('name').'</a></td></tr>';
  }

  echo '</table></td></tr></table>';
  echo '<br />';
  $button = new button($cmdClose, "javascript:window.close()");
  $tabtable->print_foot();
  echo '</td></tr></table>';
  echo '</form>';
}else
{
  if ($content_type=='text/HTML')
  {
    if ($fs_module = $GO_MODULES->get_module('filesystem'))
    {
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $fs_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $fs_module['acl_write']))
      {
	$htmlarea->add_button('go_image', '', $GO_CONFIG->control_url.'/htmlarea/images/ed_image.gif', 'false', "function insertGOimage()
	    {
	    popup('select_image.php','600','400');
	    }");

      }
    }
  }
  $GO_HEADER['head'] = $htmlarea->get_header('mail_body', -40, -250, 25, '');
  $GO_HEADER['body_arguments'] = 'onload="initEditor()"';

  require($GO_THEME->theme_path."header.inc");
  require("compose.inc");
}
require($GO_THEME->theme_path."footer.inc");
?>
