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

if (isset($_REQUEST['unseen_state_changed']))
{
  $unseen_state_changed=true;
}

if (isset($_REQUEST['empty_mailbox']) && $_REQUEST['empty_mailbox']=='true')
{
  $mail->sort(SORTDATE,0,true);
  if ($mailbox == $account['trash'] || $account['type'] == 'pop3' || $account['trash'] == '')
  {
    $mail->delete($mail->sort);
    //set this var so we can check if unseen messages were deleted
    //if so, the auto mail checker must now too.
    $unseen_state_changed = true;
  }else
  {
    $mail->move($account['trash'], $mail->sort);
  }
}

if (isset($_REQUEST['delete_message_uid']) && $_REQUEST['delete_message_uid'] > 0)
{
  $messages[] = $_REQUEST['delete_message_uid'];
  if ($mailbox == $account['trash'] || $account['type'] == 'pop3' || $account['trash'] == '')
  {
    $mail->delete($messages);
    //set this var so we can check if unseen messages were deleted
    //if so, the auto mail checker must now too.
    $unseen_state_changed = true;
  }else
  {
    $mail->move($account['trash'], $messages);
  }
}

$messages_display = '';

//when this is post request delete selected messages
if ($_SERVER['REQUEST_METHOD'] == "POST" && isset($_POST['messages']))
{
  switch ($_POST['form_action'])
  {
    case 'delete':
      if ($mailbox == $account['trash'] || $account['type'] == 'pop3' || $account['trash'] == '')
      {
	$mail->delete($_POST['messages']);
	//set this var so we can check if unseen messages were deleted
	//if so, the auto mail checker must now too.
	$unseen_state_changed = true;
      }else
      {
	$mail->move($account['trash'], $_POST['messages']);
      }
      break;

    case 'move':
      $mail->move(smartstrip($_POST['folder']), $_POST['messages']);
      break;

    case 'set_flag':
      switch($_POST['flag'])
      {
	case 'read':
	  //set this var so we can check if unseen messages were deleted
	  //if so, the auto mail checker must now too.
	  $unseen_state_changed = true;
	  $mail->set_message_flag($mailbox, $_POST['messages'], "\\Seen");
	  break;

	case 'unread':
	  //set this var so we can check if unseen messages were deleted
	  //if so, the auto mail checker must now too.
	  $unseen_state_changed = true;
	  $mail->set_message_flag($mailbox, $_POST['messages'], "\\Seen", "reset");
	  break;

	case 'flag':
	  $mail->set_message_flag($mailbox, $_POST['messages'], "\\Flagged");
	  break;

	case 'clear_flag':
	  $mail->set_message_flag($mailbox, $_POST['messages'], "\\Flagged", "reset");
	  break;
      }
      break;
  }
}
$date_sorting = $mail->is_imap() ? SORTARRIVAL : SORTDATE;

if (isset($_REQUEST['new_sort_order']) && $_REQUEST['new_sort_order'] != $em_settings['sort_order'])
{
  $email->set_sorting($GO_SECURITY->user_id, $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
  $em_settings['sort_field'] = $_REQUEST['new_sort_field'];
  $em_settings['sort_order'] = $_REQUEST['new_sort_order'];
}
if ($em_settings['sort_field'] == SORTDATE && $mail->is_imap()) $em_settings['sort_field'] = SORTARRIVAL;
if ($em_settings['sort_field'] == SORTARRIVAL && !$mail->is_imap()) $em_settings['sort_field'] = SORTDATE;

$mail->sort($em_settings['sort_field'], $em_settings['sort_order']);

if ($em_settings['sort_order'] == 1)
{
  $image_string = '<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $new_sort_order=0;
}else
{
  $image_string = '<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $new_sort_order=1;
}

//count total and new messages
$mail->count();
$msg_count = $mail->count;
$unseen = $mail->unseen;


/*
   show from address in normal mail folders and show to in sent items folders
 */
$show = "from";
$get_to_addresses = true;
if ($mail->is_imap() && $account['sent'] != '')
{
  if (strpos($mailbox, $account['sent']) === 0)
  {
    $show = "to";
    $get_to_addresses = true;
  }
}

$_SESSION['unseen_in_mailbox'] = isset($_SESSION['unseen_in_mailbox']) ? $_SESSION['unseen_in_mailbox'] : $unseen;
if (isset($unseen_state_changed) && $account['auto_check'] == '1')
{
  if ($unseen != $_SESSION['unseen_in_mailbox'])
  {
    $diff = $_SESSION['unseen_in_mailbox'] - $unseen;
    $_SESSION['notified_new_mail'] -= $diff;
  }
}
$_SESSION['unseen_in_mailbox'] = $unseen;

//loop through messages and display them

if ($msg_count > 0)
{
  $mail->get_messages($first_row, $max_rows);

  $email_filter = array();
  $subject_filter = array();
  $filters = array();
  //if there are new messages get the filters
  if ($mail->unseen > 0)
  {
    $email->get_filters($account['id']);
    while ($email->next_record())
    {
      $filter["field"] = $email->f("field");
      $filter["folder"] = $email->f("folder");
      $filter["keyword"] = $email->f("keyword");
      $filters[] = $filter;
    }
  }
  $mail->next_message_id = $first_row;
  $row_count = 0;
  while($mail->next_message($get_to_addresses))// && ($row_count < $max_rows || $max_rows == 0))
  {
    $row_count++;
    $continue = false;
    //check if message is new and apply users filters to new messages only in the inbox folder.
    if ($mail->f('new') == 1)
    {
      if (strtolower($mailbox) == "inbox")
      {
	for ($i=0;$i<sizeof($filters);$i++)
	{
	  if ($filters[$i]["folder"])
	  {
	    $field = $mail->f($filters[$i]["field"]);
	    if (!is_array($field))
	    {
	     if (stristr($field, $filters[$i]["keyword"]) !== false)
	      {
		$messages[] = $mail->f("uid");
		if ($mail->move($filters[$i]["folder"], $messages))
		{
		  $msg_count--;
		  $unseen--;
		  $continue = true;
		  break;
		}
	      }
	    }else
	    {
	      for ($x=0;$x<sizeof($field);$x++)
	      {
		if (stristr($field[$x], $filters[$i]["keyword"]))
		{
		  $messages[] = $mail->f("uid");
		  if ($mail->move($filters[$i]["folder"], $messages))
		  {
		    $msg_count--;
		    $unseen--;
		    $continue = true;
		    break;
		  }
		}
	      }
	    }
	  }
	}
      }

      if ($continue)
      {
	continue;
      }

      $class = ' class="Table4"';
      $image = '<img src="'.$GO_THEME->images['newmail'].'" border="0" width="16" height="16" />';
    }else
    {
      if ($mail->f('answered'))
      {
	$image = '<img src="'.$GO_THEME->images['mail_repl'].'" border="0" width="16" height="16" />';
      }else
      {
	$image = '<img src="'.$GO_THEME->images['mail'].'" border="0" width="16" height="16" />';
      }
      if($mail->f('uid')==$uid)
      {
	$class = ' class="Table5"';
      }else
      {
	$class = ' class="Table1"';
      }
    }

    /*
       Disabled because of huge performance hit on large mailboxes

       went from 3.5 sec to 0.8 sec to query a mailbox with 300 messages!

       check if message has attachments
       if ($mail->f("attachment"))
       {
       $attachment = '<img src="'.$GO_THEME->images['attach'].'" border="0" />';
       }else
       {
       $attachment = '&nbsp;';
       }

       switch ($mail->f("priority"))
       {
       case "4":
       $priority = '<img src="'.$GO_THEME->images['low_priority'].'" width="8" height="16" />';
       break;

       case "2":
       $priority = '<img src="'.$GO_THEME->images['high_priority'].'" width="8" height="16" />';
       break;

       default:
       $priority = '';
       break;
       }*/

    //display message
    $subject = $mail->f('subject') ? $mail->f('subject') : $ml_no_subject;
    $short_subject = cut_string($subject, 50);
    if ($show == "from")
    {
      $short_from = cut_string($mail->f('from'), 40);
    }else
    {
      $to = '';
      $to_array = $mail->f("to");
      for ($i=0;$i<sizeof($to_array);$i++)
      {
	if ($i != 0)
	{
	  $to .= ", ";
	}
	$to .= $to_array[$i];
      }
      if ($to == "")
      {
	$to = $ml_no_reciepent;
      }
      $short_to = cut_string($to, 50);
    }
    if ($mail->f('flagged') == '1')
    {
      $flag = '<img src="'.$GO_THEME->images['flag'].'" border="0" width="16" height="16" />';
    }else
    {
      $flag = '&nbsp;';
    }

    $messages_display .= '<tr'.$class.' id="'.$mail->f('uid').'">';
    $messages_display .= '<td nowrap><input type="checkbox" name="messages[]" value="'.$mail->f('uid').'" onclick="javascript:item_click(this);" /></td>';
    //$messages_display .= '<td nowrap>'.$priority.'&nbsp;</td>';
    $messages_display .= '<td nowrap>'.$flag.'</td>';
    //$messages_display .= '<td nowrap>'.$attachment.'&nbsp;</td>';
    $messages_display .= '<td nowrap width="20">'.$image.'</td>';
    $link = "message.php?account_id=".$account['id']."&uid=".$mail->f('uid')."&mailbox=".$mailbox."&first_row=".$first_row."&return_to=".urlencode($link_back);
    if ($show=="from")
    {
      $messages_display .= "<td nowrap><a href=\"".$link."\" title=\"".$mail->f('from')."&nbsp;&lt;".$mail->f("sender")."&gt;\">".$short_from."&nbsp;</a></td>";
    }else
    {
      $messages_display .= "<td nowrap><a href=\"".$link."\" title=\"".$to."\">".$short_to."&nbsp;</a></td>";
    }

    $messages_display .= "<td nowrap><a href=\"".$link."\" title=\"".$mail->f('subject')."\">".$short_subject."&nbsp;</a></td>";
    $messages_display .= '<td nowrap>'.format_size($mail->f('size')).'&nbsp;</td>';
    $messages_display .= '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time($mail->f('udate'))).'</td>';
    $messages_display .= "</tr>\n";
    $messages_display .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td>';
    $messages_display .= "</tr>\n";

  }
  $messages_display .= '<tr><td colspan="99" class="small" height="18">&nbsp;'.$msg_count.' '.$ml_messages.'&nbsp;&nbsp;&nbsp;';
  if ($mail->is_imap())
  {
    $messages_display .= '('.$unseen.' '.$ml_new.')';
  }

  $messages_display .= '</td></tr>';
  $messages_display .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td>';
  $messages_display .= "</tr>\n";

}
if ($msg_count == 0)
{
  $email_folder = $email->get_folder($account['id'], addslashes($mailbox));
  if ($email_folder['attributes']&LATT_NOSELECT)
  {
    $messages_display = '<tr><td colspan="99" height="18">'.$ml_no_mailbox.'</td></tr>';
  }else
  {
    $messages_display = '<tr><td colspan="99" height="18">'.$ml_no_messages.'</td></tr>';
  }
  $messages_display .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}

?>
<table border="0" cellpadding="5" cellspacing="0" width="100%">
<tr>
<td valign="top" nowrap><?php
require("treeview.inc"); ?>
</td>
<td valign="top">
<input type="hidden" name="max_rows" value="<?php echo $max_rows; ?>" />
<input type="hidden" name="first_row" value="<?php echo $first_row; ?>" />
<input type="hidden" name="mailbox" value="<?php echo $mailbox; ?>" />
<input type="hidden" name="new_sort_field" value="<?php echo $em_settings['sort_field']; ?>" />
<input type="hidden" name="new_sort_order" value="<?php echo $em_settings['sort_order']; ?>" />
<input type="hidden" name="form_action" />
<table border="0" cellspacing="0" cellpadding="0" class="Table1" width="100%">
<tr>
<td colspan="6" nowrap>

<?php
if ($account['type'] == "imap")
{
  if ($email->get_all_folders($account['id'],true) > 0)
  {
    $dropbox = new dropbox();
    $dropbox->add_value('', $ml_move_mail);
    if (strtolower($mailbox) != 'inbox')
    {
      $dropbox->add_value('INBOX',$ml_inbox);
    }
    while ($email->next_record())
    {
      if (!($email->f('attributes')&LATT_NOSELECT) && $email->f('name') != $mailbox)
      {
	//$dropbox->add_value($email->f('name'), $email->f('name'));
	$dropbox->add_value($email->f('name'), str_replace('INBOX'.$email->f('delimiter'), '', $email->f('name')));
      }
    }
    $dropbox->print_dropbox('folder','','onchange="javascript:move_mail()"');
  }

  $dropbox = new dropbox();
  $dropbox->add_value('', $ml_mark);
  $dropbox->add_value('read', $ml_markread);
  $dropbox->add_value('unread', $ml_markunread);
  $dropbox->add_value('flag', $ml_flag);
  $dropbox->add_value('clear_flag', $ml_clearflag);
  $dropbox->print_dropbox('flag', '', 'onchange="javascript:set_flag()"');
}else
{
  echo '&nbsp;';
}

?>

</td>
<td colspan="3" align="right" class="small" nowrap>
<?php
echo $msg_count." ".$ml_messages;
if ($mail->is_imap())
{
  echo "&nbsp;&nbsp;&nbsp;(".$unseen." ".$ml_new.")";
}
?>
</td>
</tr>

<tr>
<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>
<td class="TableHead2" width="8">&nbsp;</td>
<td class="TableHead2" width="16">&nbsp;</td>
<?php
if ($show == "from")
{
  echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.SORTFROM.','.$new_sort_order.');">'.$ml_from;
  if ($em_settings['sort_field'] == SORTFROM)
  {
    echo '&nbsp;'.$image_string;
  }
  echo '</a></td>';
}else
{
  echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.SORTTO.','.$new_sort_order.');">'.$ml_to;
  if ($em_settings['sort_field'] == SORTTO)
  {
    echo '&nbsp;'.$image_string;
  }
  echo '</a></td>';
}
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.SORTSUBJECT.','.$new_sort_order.');">'.$ml_subject;
if ($em_settings['sort_field'] == SORTSUBJECT)
{
  echo '&nbsp;'.$image_string;
}
echo '</a></td>';

echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.SORTSIZE.','.$new_sort_order.');">'.$ml_size;
if ($em_settings['sort_field'] == SORTSIZE)
{
  echo '&nbsp;'.$image_string;
}
echo '</a></td>';

echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.$date_sorting.','.$new_sort_order.');">'.$strDate;
if ($em_settings['sort_field'] == $date_sorting)
{
  echo '&nbsp;'.$image_string;
}
echo '</a></td></tr>';

echo $messages_display;

$links = '';
$max_links=10;
if ($max_rows != 0)
{
  if ($msg_count > $max_rows)
  {
    $links = '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td>';
    $next_start = $first_row+$max_rows;
    $previous_start = $first_row-$max_rows;
    if ($first_row != 0)
    {
      $links .= '<a href="javascript:change_page(0, '.$max_rows.');">&lt;&lt;</a>&nbsp;';
      $links .= '<a href="javascript:change_page('.$previous_start.', '.$max_rows.');">'.$cmdPrevious.'</a>&nbsp;';
    }else
    {
      $links .= '<font color="#cccccc">&lt;&lt; '.$cmdPrevious.'</font>&nbsp;';
    }

    $start = ($first_row-(($max_links/2)*$max_rows));

    $end = ($first_row+(($max_links/2)*$max_rows));

    if ($start < 0)
    {
      $end = $end - $start;
      $start=0;
    }
    if ($end > $msg_count)
    {
      $end = $msg_count;
    }

    if ($start > 0)
    {
      $links .= '...&nbsp;';
    }

    for ($i=$start;$i<$end;$i+=$max_rows)
    {
      $page = ($i/$max_rows)+1;
      if ($i==$first_row)
      {
	$links .= '<b><i>'.$page.'</i></b>&nbsp;';
      }else
      {
	$links .= '<a href="javascript:change_page('.$i.','.$max_rows.');">'.$page.'</a>&nbsp;';
      }
    }

    if ($end < $msg_count)
    {
      $links .= '...&nbsp;';
    }

    $last_page = floor($msg_count/$max_rows)*$max_rows;

    if ($msg_count > $next_start)
    {
      $links .= '<a href="javascript:change_page('.$next_start.', '.$max_rows.');">'.$cmdNext.'</a>&nbsp;';
      $links .= '<a href="javascript:change_page('.$last_page.', '.$max_rows.');">&gt;&gt;</a>';
    }else
    {
      $links .= '<font color="#cccccc">'.$cmdNext.' &gt;&gt;</font>';
    }
    $links .= '</td><td align="right"><a class="normal" href="javascript:change_page(0,0);">'.$cmdShowAll.'</a></td></tr></table>';
    echo '<tr><td colspan="99" height="20">'.$links.'</td></tr>';
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
  }
}
echo '</table>';
?>
</td>
</tr>
</table>
  <script type="text/javascript">
function confirm_delete()
{
  var count = 0;

  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      if (document.forms[0].elements[i].checked == true)
      {
	count++;

      }
    }
  }
  switch (count)
  {
    case 0:
      alert("<?php echo $fbNoSelect; ?>");
      break;
      <?php
	$trash_folder = $account['trash'];
      if ($trash_folder == '' || $trash_folder == $mailbox)
      {
	echo '
	case 1:
	  if (confirm("'.$strDeletePrefix.$ml_message.$strDeleteSuffix.'"))
	  {
	    document.forms[0].form_action.value=\'delete\';
	    document.forms[0].submit();
	  }
	  break;

	default:
	  if (confirm("'.$strDeletePrefix.$strThis.' "+count+" '.$ml_messages2.$strDeleteSuffix.'"))
	  {
	    document.forms[0].form_action.value=\'delete\';
	    document.forms[0].submit();
	  }
	  break;
	  ';
      }else
      {
	echo '
	default:
	  document.forms[0].form_action.value=\'delete\';
	  document.forms[0].submit();
	  break;
	  ';
      }
      ?>
  }
}
</script>
