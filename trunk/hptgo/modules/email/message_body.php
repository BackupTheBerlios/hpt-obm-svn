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

$texts = '';
$images = '';

$mailbox = isset($_REQUEST['mailbox'])?  $_REQUEST['mailbox'] : "INBOX";
$uid = isset($_REQUEST['uid']) ? $_REQUEST['uid'] : 0;
$part = isset($_REQUEST['part']) ? $_REQUEST['part'] : '';
$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;
$account = $email->get_account($account_id);

if ($account && $mail->open($account['host'], $account['type'], $account['port'],$account['username'], $GO_CRYPTO->decrypt($account['password']),$mailbox, 0, $account['use_ssl'], $account['novalidate_cert']))
{
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


$count = 0;
$splitter = 0;
$parts = array_reverse($mail->f("parts"));

require_once("smileys.php");
//get all text and html content
for ($i=0;$i<sizeof($parts);$i++)
{
  $mime = strtolower($parts[$i]["mime"]);

  if (($mime == "text/html") || ($mime == "text/plain") || ($mime == "text/enriched"))
  {
    $mail_charset = $parts[$i]['charset'];
 
    $part = $mail->view_part($uid, $parts[$i]["number"], $parts[$i]["transfer"], $parts[$i]["charset"]);

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
    
    $part = preg_replace_callback ($smiley_patterns, "smiley_symbols_to_images", $part);

    if ($parts[$i]["name"] != '')
    {
      $texts .= "<p class=\"normal\" align=\"center\">--- ".$parts[$i]["name"]." ---</p>";
    }elseif($texts != '')
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
}header('Content-Type: text/html; charset='.$mail_charset);
echo $texts.$images;
?>
