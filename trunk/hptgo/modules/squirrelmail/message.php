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

$sort_field = $_REQUEST['sort_field'];
$sort_order = $_REQUEST['sort_order'];
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

$aMailbox['SORT'] = $sort_order ? $sort_field + 1 : $sort_field;
fetchMessageHeaders($imapConnection,$aMailbox);

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


require($GO_THEME->theme_path."header.inc");

echo '<form method="get" action="'.$_SERVER['PHP_SELF'].'" name="email_form">';


echo '<table border="0" width="100%" height="100%"><tr><td>';


if (!$print)
{
  echo '<table border="0" cellspacing="0" cellpadding="0"><tr><td class="ModuleIcons">';
  if (($account['sent'] != '' && strpos($mailbox, $account['sent']) === 0) ||
      ($account['draft'] != '' && strpos($mailbox, $account['draft']) === 0)) {
    echo '<td class="ModuleIcons">';
    echo "<a href=\"javascript:popup('send.php?account_id=".$account_id."&uid=".$uid."&mailbox=".urlencode($mailbox)."&action=edit','".$GO_CONFIG->composer_width."','".$GO_CONFIG->composer_height."')\"><img src=\"".$GO_THEME->images['compose']."\" border=\"0\" height=\"32\" width=\"32\" /><br />".$ml_compose."</a></td>\n";
  }
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
  if ($account['spam'] != '')
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="javascript:spam();"><img src="'.$GO_THEME->images['block'].'" border="0" height="32" width="32" /><br />'.$ml_block.'</a></td>';
  }

  $previous_id = findPreviousMessage($aMailbox['UIDSET'][0],$uid);
  if ($previous_id != -1)
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="javascript:get_message('.$previous_id.');"><img src="'.$GO_THEME->images['previous'].'" border="0" height="32" width="32" /><br />'.$cmdPrevious.'</a></td>';
  }

  $next_id = findNextMessage($aMailbox['UIDSET'][0],$uid);
  if ($next_id != -1)
  {
    echo '<td class="ModuleIcons">';
    echo '<a href="javascript:get_message('.$next_id.');"><img src="'.$GO_THEME->images['next'].'" border="0" height="32" width="32" /><br />'.$cmdNext.'</a></td>';
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
<input type="hidden" name="sort_field" value="<?php echo $sort_field; ?>" />
<input type="hidden" name="sort_order" value="<?php echo $sort_order; ?>" />

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
  document.email_form.spam_address.value = "<?php echo $sender; ?>";
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

function toggle_div(div_name,div_icon)
{
  div_header = document.getElementById(div_name);
  if (div_header.style.display == '') {
    div_header.style.visibility = 'hidden';
    div_header.style.display = 'none';
    document.getElementById(div_icon).src = "<?php echo $GO_THEME->images['plus_node'];?>";
  } else {
    div_header.style.visibility = 'visible';
    div_header.style.display = '';
    document.getElementById(div_icon).src = "<?php echo $GO_THEME->images['min_node'];?>";
  }
}

//-->
</script>





<table border="0" cellpadding="1" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td>
<table border="0" cellpadding="1" cellspacing="0" class="TableInside" width="100%">
<tr>
<td>
<table border="0" cellpadding="1" cellspacing="0"><!-- BEGIN --><tr><td width="100%"><div id="div_header">
<?php
obm_formatEnvheader($aMailbox,$uid,false,$message,0,true);
?>
</div></td><td valign="top"><a href="javascript:toggle_div('div_header','div_header_icon')"><img id="div_header_icon" border="0" src="<?php echo $GO_THEME->images['min_node'];?>"/></a></td></tr></table><!-- END -->
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
$parts = array();//array_reverse($mail->f("parts"));

$browser = detect_browser();
$target = $browser['name'] == 'MSIE' ? '_blank' : '_self';
$attachments = "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\"><tr>";
$attachments .= '<td width="100%" ><table width="100%" id="div_attachments">'.obm_formatAttachments($message,$ent_ar,$mailbox, $uid).'</table></td><td valign="top"><a href="javascript:toggle_div(\'div_attachments\',\'div_attachments_icon\')"><img id="div_attachments_icon" border="0" src="'.$GO_THEME->images['min_node'].'"/></a></td></tr><tr>';


$attachments .= "</tr></table>";

if (true /*$count>0*/)
{
  echo '<br /><table border="0" cellpadding="1" cellspacing="0" class="TableBorder" width="100%"><tr><td><table border="0" cellpadding="1" cellspacing="0" class="TableInside" width="100%"><tr><td valign="top">';
  echo '<table width="100%" border="0" cellpadding="0" cellspacing="0"><tr><td nowrap valign="top"><b>'.$ml_attachments.':</b>&nbsp;&nbsp;</td><td width="100%">'.$attachments.'</td></tr></table>';
  echo '</td></tr></table></td></tr></table>';
}
?>
</td>
</tr>
<tr>
	<td height="100%">
	<iframe frameborder="no" src="message_body.php?account_id=<?php echo $account_id; ?>&uid=<?php echo $uid; ?>&part=<?php echo $part; ?>&mailbox=<?php echo urlencode($mailbox); ?>" height="100%" width="100%"></iframe>
	</td>
</tr>
</table>
<?php
if ($content["notification"] != '' && $content["new"] == 1)
{
  echo "<script type=\"text/javascript\">\npopup('"."notification.php?notification=".urlencode($content["notification"])."&date=".urlencode(date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'],$content['udate']))."&subject=".urlencode($subject)."&to=".urlencode($to)."','500','150');\n</script>\n";
}

sqimap_logout($imapConnection);

echo '</form>';

if ($print)
{
  echo "\n<script type=\"text/javascript\">\nwindow.print();\n</script>\n";
}
require($GO_THEME->theme_path."footer.inc");

function formatRecipientString($recipients, $item ) {
    global $show_more_cc, $show_more, $show_more_bcc,
           $PHP_SELF;

    $string = '';
    if ((is_array($recipients)) && (isset($recipients[0]))) {
        $show = false;

        if ($item == 'to') {
            if ($show_more) {
                $show = true;
                $url = set_url_var($PHP_SELF, 'show_more',0);
            } else {
                $url = set_url_var($PHP_SELF, 'show_more',1);
            }
        } else if ($item == 'cc') {
            if ($show_more_cc) {
                $show = true;
                $url = set_url_var($PHP_SELF, 'show_more_cc',0);
            } else {
                $url = set_url_var($PHP_SELF, 'show_more_cc',1);
            }
        } else if ($item == 'bcc') {
            if ($show_more_bcc) {
                $show = true;
                $url = set_url_var($PHP_SELF, 'show_more_bcc',0);
            } else {
                $url = set_url_var($PHP_SELF, 'show_more_bcc',1);
            }
        }

        $cnt = count($recipients);
        foreach($recipients as $r) {
            $add = decodeHeader($r->getAddress(true),true.false);
            if ($string) {
                $string .= '<BR>' . $add;
            } else {
                $string = $add;
                if ($cnt > 1) {
                    $string .= '&nbsp;(<A HREF="'.$url;
                    if ($show) {
                       $string .= '">'._("less").'</A>)';
                    } else {
                       $string .= '">'._("more").'</A>)';
                       break;
                    }
                }
            }
        }
    }
    return $string;
}



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

function obm_formatEnvheader($aMailbox, $passed_id, $passed_ent_id, $message,
                         $color, $FirstTimeSee) {
    global $msn_user_support, $default_use_mdn, $default_use_priority,
           $show_xmailer_default, $mdn_user_support, $PHP_SELF, $javascript_on,
           $squirrelmail_language;

    $mailbox = $aMailbox['NAME'];

    $header = $message->rfc822_header;
    $env = array();
    $env[_("Subject")] = str_replace("&nbsp;"," ",decodeHeader($header->subject));

    $from_name = $header->getAddr_s('from');
    if (!$from_name)
        $from_name = $header->getAddr_s('sender');
    if (!$from_name)
        $env[_("From")] = _("Unknown sender");
    else
        $env[_("From")] = decodeHeader($from_name);
    $env[_("Date")] = getLongDateString($header->date);
    $env[_("To")] = formatRecipientString($header->to, "to");
    $env[_("Cc")] = formatRecipientString($header->cc, "cc");
    $env[_("Bcc")] = formatRecipientString($header->bcc, "bcc");
    if ($default_use_priority) {
        $env[_("Priority")] = htmlspecialchars(getPriorityStr($header->priority));
    }
    if ($show_xmailer_default) {
        $env[_("Mailer")] = decodeHeader($header->xmailer);
    }

    $s  = '<table width="100%" cellpadding="0" cellspacing="2" border="0"';
    $s .= ' align="center" bgcolor="'.$color[0].'">';
    foreach ($env as $key => $val) {
        if ($val) {
            $s .= '<tr>';
            $s .= html_tag('td', '<b>' . $key . ':&nbsp;&nbsp;</b>', 'right', '', 'valign="top" width="0%"') . "\n";
            $s .= html_tag('td', $val, 'left', '', 'valign="top" width="100%"') . "\n";
            $s .= '</tr>';
        }
    }
    echo '<table bgcolor="'.$color[9].'" width="100%" cellpadding="1"'.
         ' cellspacing="0" border="0" align="center">'."\n";
    echo '<tr><td height="5" colspan="2" bgcolor="'.
          $color[4].'"></td></tr><tr><td align="center">'."\n";
    echo $s;
    //do_hook('read_body_header');
    //formatToolbar($mailbox, $passed_id, $passed_ent_id, $message, $color);
    echo '</table>';
    echo '</td></tr><tr><td height="5" colspan="2" bgcolor="'.$color[4].'"></td></tr>'."\n";
    echo '</table>';
}

function findNextMessage($uidset,$passed_id='backwards') {
    if (!is_array($uidset)) {
        return -1;
    }
    if ($passed_id=='backwards' || !is_array($uidset)) { // check for backwards compattibilty gpg plugin
        $passed_id = $uidset;
    }
    $result = -1;
    $count = count($uidset) - 1;
    foreach($uidset as $key=>$value) {
        if ($passed_id == $value) {
            if ($key == $count) {
                break;
            }
            $result = $uidset[$key + 1];
            break;
        }
    }
    return $result;
}

/**
 * Given an IMAP message id number, this will look it up in the cached
 * and sorted msgs array and return the index of the previous message
 *
 * @param int $passed_id The current message UID
 * @return the index of the next valid message from the array
 */

function findPreviousMessage($uidset, $passed_id) {
    if (!is_array($uidset)) {
        return -1;
    }
    $result = -1;
    foreach($uidset as $key=>$value) {
        if ($passed_id == $value) {
            if ($key != 0) {
                $result = $uidset[$key - 1];
            }
            break;
        }
    }

    return $result;
}

function obm_formatAttachments($message, $exclude_id, $mailbox, $id) {
    global $where, $what, $startMessage, $color,$account_id, $GO_CONFIG,$GO_MODULES;
    static $ShownHTML = 0;

    $att_ar = $message->getAttachments($exclude_id);

    if (!count($att_ar)) return '';

    $attachments = '';

    $urlMailbox = urlencode($mailbox);

    foreach ($att_ar as $att) {
        $ent = $att->entity_id;
        $header = $att->header;
        $type0 = strtolower($header->type0);
        $type1 = strtolower($header->type1);
        $name = '';
        $links['download link']['text'] = _("download");
        $links['download link']['href'] = $GO_MODULES->url.
                "download.php?absolute_dl=true&amp;uid=$id&amp;mailbox=$urlMailbox&amp;ent_id=$ent";
        $ImageURL = '';
        if ($type0 =='message' && $type1 == 'rfc822') {
            $default_page = $GO_MODULES->url. 'read_body.php';
            $rfc822_header = $att->rfc822_header;
            $filename = $rfc822_header->subject;
            if (trim( $filename ) == '') {
                $filename = 'untitled-[' . $ent . ']' ;
            }
            $from_o = $rfc822_header->from;
            if (is_object($from_o)) {
                $from_name = $from_o->getAddress(false);
            } else {
                $from_name = _("Unknown sender");
            }
            $from_name = decodeHeader(($from_name));
            $description = $from_name;
        } else {
            $default_page = $GO_MODULES->url. 'download.php';
            if (is_object($header->disposition)) {
                $filename = $header->disposition->getProperty('filename');
                if (trim($filename) == '') {
                    $name = decodeHeader($header->disposition->getProperty('name'));
                    if (trim($name) == '') {
                        $name = $header->getParameter('name');
                        if(trim($name) == '') {
                            if (trim( $header->id ) == '') {
                                $filename = 'untitled-[' . $ent . ']' ;
                            } else {
                                $filename = 'cid: ' . $header->id;
                            }
                        } else {
                            $filename = $name;
                        }
                    } else {
                        $filename = $name;
                    }
                }
            } else {
                $filename = $header->getParameter('name');
                if (!trim($filename)) {
                    if (trim( $header->id ) == '') {
                        $filename = 'untitled-[' . $ent . ']' ;
                    } else {
                        $filename = 'cid: ' . $header->id;
                    }
                }
            }
            if ($header->description) {
                $description = decodeHeader($header->description);
            } else {
                $description = '';
            }
        }

        $display_filename = $filename;
        if (isset($passed_ent_id)) {
            $passed_ent_id_link = '&amp;passed_ent_id='.$passed_ent_id;
        } else {
            $passed_ent_id_link = '';
        }
        $defaultlink = $default_page . "?startMessage=$startMessage"
                     . "&amp;uid=$id&amp;mailbox=$urlMailbox"
                     . '&amp;ent_id='.$ent.$passed_ent_id_link;
        if ($where && $what) {
           $defaultlink .= '&amp;where='. urlencode($where).'&amp;what='.urlencode($what);
        }
        /* This executes the attachment hook with a specific MIME-type.
         * If that doesn't have results, it tries if there's a rule
         * for a more generic type.
         */
        $hookresults = do_hook("attachment $type0/$type1", $links,
                               $startMessage, $id, $urlMailbox, $ent, $defaultlink,
                               $display_filename, $where, $what);
        if(count($hookresults[1]) <= 1) {
            $hookresults = do_hook("attachment $type0/*", $links,
                                   $startMessage, $id, $urlMailbox, $ent, $defaultlink,
                                   $display_filename, $where, $what);
        }

        $links = $hookresults[1];
        $defaultlink = $hookresults[6];

        $attachments .= '<TR><TD>' .
                        '<A HREF="'.$defaultlink.'">'.decodeHeader($display_filename).'</A>&nbsp;</TD>' .
                        '<TD><SMALL><b>' . show_readable_size($header->size) .
                        '</b>&nbsp;&nbsp;</small></TD>' .
                        '<TD><SMALL>[ '. 
                         htmlspecialchars($type0).'/'.htmlspecialchars($type1).
                        ' ]&nbsp;</SMALL></TD>'.
                        '<TD><SMALL>';
        $attachments .= '<b>' . $description . '</b>';
        $attachments .= '</SMALL></TD><TD><SMALL>&nbsp;';

        $skipspaces = 1;
        foreach ($links as $val) {
            if ($skipspaces) {
                $skipspaces = 0;
            } else {
                $attachments .= '&nbsp;&nbsp;|&nbsp;&nbsp;';
            }
            $attachments .= '<a href="' . $val['href'] . '">' .  $val['text'] . '</a>';
        }
        unset($links);
        $attachments .= "</TD></TR>\n";
    }
    return $attachments;
}

?>
