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
require($GO_LANGUAGE->get_language_file('email'));
$mail = new imap();
$email = new email();

$em_settings = $email->get_settings($GO_SECURITY->user_id);

$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$uid = isset($_REQUEST['uid']) ? $_REQUEST['uid'] : 0;
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first_row = isset($_REQUEST['first_row']) ? $_REQUEST['first_row'] : 0;
$table_tabindex = isset($_REQUEST['table_tabindex']) ? $_REQUEST['table_tabindex'] : null;
$mailbox = isset($_REQUEST['mailbox'])?  smartstrip($_REQUEST['mailbox']) : 'INBOX';
$link_back = $GO_MODULES->url.'index.php?account_id='.$account_id.'&mailbox='.$mailbox.'&first_row='.$first_row;
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$query = isset($_REQUEST['query']) ? base64_decode($_REQUEST['query']) : '';
$link_back = 'search.php';

if (!$account = $email->get_account($account_id))
{
  $account = $email->get_account(0);
}

if ($account && $account["user_id"] != $GO_SECURITY->user_id)
{
  header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
  exit();
}

if (!$mail->open($account['host'], $account['type'],
      $account['port'], $account['username'], 
      $GO_CRYPTO->decrypt($account['password']), $mailbox))
{
  echo '<p class="Error">'.$ml_connect_failed.' \''.$account['host'].'\' '.$ml_at_port.': '.$account['port'].'</p>';
  echo '<p class="Error">'.imap_last_error().'</p>';
  require($GO_THEME->theme_path.'footer.inc');
  exit();
}

$date_sorting = $mail->is_imap() ? SORTARRIVAL : SORTDATE;
                                                                                                                                                             
if (isset($_REQUEST['new_sort_field']) && $_REQUEST['new_sort_order'] != $em_settings['sort_order'])
{
  $email->set_sorting($GO_SECURITY->user_id, $_REQUEST['new_sort_field'], $_REQUEST['new_sort_field']);
  $em_settings['sort_field'] = $_REQUEST['new_sort_field'];
  $em_settings['sort_order'] = $_REQUEST['new_sort_order'];
}
if ($em_settings['sort_field'] == SORTDATE && $mail->is_imap()) $em_settings['sort_field'] = SORTARRIVAL;
if ($em_settings['sort_field'] == SORTARRIVAL && !$mail->is_imap()) $em_settings['sort_field'] = SORTDATE;

if ($em_settings['sort_order'] == 1)
{
  $image_string = '<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $new_sort_order=0;
}else
{
  $image_string = '<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $new_sort_order=1;
}

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

$GO_HEADER['head'] = '<script type="text/javascript" src="'.$GO_MODULES->url.'email.js"></script>';
$date_picker = new date_picker();
$GO_HEADER['head'] .= $date_picker->get_header();

require($GO_THEME->theme_path."header.inc");

echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
echo '<input type="hidden" name="empty_mailbox" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="account_id" value="'.$account['id'].'" />';
echo '<input type="hidden" name="task" value="search" />';
echo '<input type="hidden" name="first_row" value="'.$first_row.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="form_action" value="" />';
echo '<input type="hidden" name="new_sort_field" value="'.$em_settings['sort_field'].'" />';
echo '<input type="hidden" name="new_sort_order" value="'.$em_settings['sort_order'].'" />';

echo '<table border="0"><tr>';

echo '<td class="ModuleIcons">';
echo '<a href="javascript:confirm_delete()"><img src="'.$GO_THEME->images['delete_big'].'" border="0" height="32" width="32" /><br />'.$ml_delete.'</a></td>';

echo '<td class="ModuleIcons">';
echo '<a href="index.php?account_id='.$account_id.'&mailbox='.$mailbox.'"><img src="'.$GO_THEME->images['close'].'" border="0" height="32" width="32" /><br />'.$cmdClose.'</a></td>';

echo '</tr></table>';		
$tabtable = new tabtable('search_tab', $ml_search.' - '.$account['email'], '100%', '');
$tabtable->print_head();

$subject = isset($_POST['subject']) ?smartstrip(trim($_POST['subject'])) : '';
$from = isset($_POST['from']) ? smartstrip(trim($_POST['from'])) : '';
$to = isset($_POST['to']) ? smartstrip(trim($_POST['to'])) : '';
$cc = isset($_POST['cc']) ?  smartstrip(trim($_POST['cc'])) : '';
$body = isset($_POST['body']) ? smartstrip(trim($_POST['body'])) : '';
$before = isset($_POST['before']) ? smartstrip(trim($_POST['before'])) : '';
$since = isset($_POST['since']) ? smartstrip(trim($_POST['since'])) : '';
$before = isset($_POST['before']) ? $_POST['before'] : '';	
$since = isset($_POST['since']) ? $_POST['since'] : '';		
$flagged = isset($_POST['flagged']) ? $_POST['flagged'] : '';	
$answered = isset($_POST['answered']) ? $_POST['answered'] : '';	

if($_SERVER['REQUEST_METHOD'] == 'POST')
{
  //build query	
  if ($subject != '')
  {
    $query = 'SUBJECT "'.$subject.'" ';
  }
  if ($from != '')
  {
    $query .= 'FROM "'.$from.'" ';
  }
  if ($to != '')
  {
    $query .= 'TO "'.$to.'" ';
  }
  if ($cc != '')
  {
    $query .= 'CC "'.$cc.'" ';
  }
  if ($body != '')
  {
    $query .= 'BODY "'.$body.'" ';
  }

  if($before != '')
  {
    $unix_before = date_to_unixtime($before);
    $query .= 'BEFORE "'.date('d-M-Y', $unix_before).'" ';
  }

  if($since != '')
  {
    $unix_since = date_to_unixtime($since);
    $query .= 'SINCE "'.date('d-M-Y', $unix_since).'" ';
  }

  if ($_POST['flagged'] != '')
  {
    $query .= $_POST['flagged'].' ';
  }

  if ($_POST['answered'] != '')
  {
    $query .= $_POST['answered'].' ';
  }

  if ($query == '')
  {
    echo '<p class="Error">'.$ml_select_one_criteria.'</p>';
  }
}	

echo '<table border="0"><tr><td valign="top">';
echo '<table border="0">';
if ($account['type'] == "imap")
{
  if ($email->get_all_folders($account['id'],true) > 0)
  {
    $dropbox = new dropbox();
    $dropbox->add_value('INBOX',$ml_inbox);
    while ($email->next_record())
    {
      if (!($email->f('attributes')&LATT_NOSELECT))
      {
	$dropbox->add_value($email->f('name'), str_replace('INBOX'.$email->f('delimiter'), '', $email->f('name')));
      }
    }
    echo '<tr><td>'.$ml_folder.':</td><td>';
    $dropbox->print_dropbox('mailbox', $mailbox);
    echo '</td></tr>';
  }
}

echo 	'<tr><td>'.$ml_subject.':</td><td>'.		
'<input type="text" name="subject" size="40" class="textbox" value="'.htmlspecialchars($subject).'" />'.
'</td></tr>';

echo 	'<tr><td>'.$ml_from.':</td><td>'.		
'<input type="text" name="from" size="40" class="textbox" value="'.htmlspecialchars($from).'" />'.
'</td></tr>';

echo 	'<tr><td>'.$ml_to.':</td><td>'.		
'<input type="text" name="to" size="40" class="textbox" value="'.htmlspecialchars($to).'" />'.
'</td></tr>';

echo 	'<tr><td>CC:</td><td>'.		
'<input type="text" name="cc" size="40" class="textbox" value="'.htmlspecialchars($cc).'" />'.
'</td></tr>';

echo 	'<tr><td>'.$ml_body.':</td><td>'.		
'<input type="text" name="body" size="40" class="textbox" value="'.htmlspecialchars($body).'" />'.
'</td></tr>';		

echo '</table></td><td valign="top"><table border="0">';

echo 	'<tr><td>'.$ml_before.':</td><td>';			
$date_picker->print_date_picker('before', $_SESSION['GO_SESSION']['date_format'], $before);
echo '</td></tr>';

echo 	'<tr><td>'.$ml_since.':</td><td>';			
$date_picker->print_date_picker('since', $_SESSION['GO_SESSION']['date_format'], $since);
echo '</td></tr>';

echo '<tr><td>'.$ml_flag.':</td><td>';
$radio_list = new radio_list('flagged', $flagged);
$radio_list->add_option('', $ml_doesnt_matter);
$radio_list->add_option('FLAGGED', $cmdYes);
$radio_list->add_option('UNFLAGGED', $cmdNo);
echo '</td></tr>';

echo '<tr><td>'.$ml_answered.':</td><td>';
$radio_list = new radio_list('answered', $answered);
$radio_list->add_option('', $ml_doesnt_matter);
$radio_list->add_option('ANSWERED', $cmdYes);
$radio_list->add_option('UNANSWERED', $cmdNo);
echo '</td></tr>';
echo '</table>';

echo '</td></tr>';

echo '<tr><td colspan="2">';
$button = new button($cmdSearch, 'javascript:document.forms[0].submit();');
echo '</td></tr>';
echo '</table>';

$tabtable->print_foot();

if($query != '')
{
  $messages_display = '';

  $msg_count = $mail->search($em_settings['sort_field'], $em_settings['sort_order'], $query);
  $mail->get_messages($first_row, $max_rows);

  $filters = array();
  //if there are new messages get the filters
  $email->get_filters($account['id']);
  while ($email->next_record())
  {
    $filter["field"] = $email->f("field");
    $filter["folder"] = $email->f("folder");
    $filter["keyword"] = $email->f("keyword");
    $filters[] = $filter;
  }


  while($mail->next_message(true))
  {
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
	      if (strpos($field, $filters[$i]["keyword"]) !== false)
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
		if (eregi($filters[$i]["keyword"], $field[$x]))
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

    $subject = $mail->f('subject') ? $mail->f('subject') : $ml_no_subject;
    $short_subject = cut_string($subject, 50);

    $short_from = cut_string($mail->f('from'), 40);

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
    $link = "message.php?account_id=".$account['id']."&uid=".$mail->f('uid')."&mailbox=".$mailbox."&first_row=".$first_row."&return_to=".urlencode($link_back)."&query=".base64_encode($query);
    $messages_display .= '<td nowrap width="20">'.$image.'</td>';
    $messages_display .= "<td nowrap><a href=\"".$link."\" title=\"".$mail->f('from')."&nbsp;&lt;".$mail->f("sender")."&gt;\">".$short_from."&nbsp;</a></td>";
    $messages_display .= "<td nowrap><a href=\"".$link."\" title=\"".$to."\">".$short_to."&nbsp;</a></td>";
    $messages_display .= "<td nowrap><a href=\"".$link."\" title=\"".$mail->f('subject')."\">".$short_subject."&nbsp;</a></td>";
    $messages_display .= '<td nowrap>'.format_size($mail->f('size')).'&nbsp;</td>';
    $messages_display .= '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time($mail->f('udate'))).'</td>';
    $messages_display .= "</tr>\n";
    $messages_display .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td>';
    $messages_display .= "</tr>\n";

  }
  $messages_display .= '<tr><td colspan="99" class="small" height="18">&nbsp;'.$msg_count.' '.$ml_messages.'&nbsp;&nbsp;&nbsp;';
  $messages_display .= '</td></tr>';
  $messages_display .= '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td>';
  $messages_display .= "</tr>\n";


  ?>
    <table border="0" cellpadding="5" cellspacing="0" width="100%">
    <tr>
    <td valign="top">
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
	    $dropbox->add_value($email->f('name'), $email->f('name'));
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
    ?>
      </td>
      </tr>

      <tr>
      <td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>
      <td class="TableHead2" width="8">&nbsp;</td>
      <td class="TableHead2" width="16">&nbsp;</td>
      <?php
      echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.SORTFROM.','.$new_sort_order.');">'.$ml_from;
    if ($em_settings['sort_field'] == SORTFROM)
    {
      echo '&nbsp;'.$image_string;
    }
    echo '</a></td>';

    echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:sort('.SORTTO.','.$new_sort_order.');">'.$ml_to;
    if ($em_settings['sort_field'] == SORTTO)
    {
      echo '&nbsp;'.$image_string;
    }
    echo '</a></td>';

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
	  $links .= '<a href="javascript:change_page(0, '.$max_rows.');">&lt&lt</a>&nbsp;';
	  $links .= '<a href="javascript:change_page('.$previous_start.', '.$max_rows.');">'.$cmdPrevious.'</a>&nbsp;';
	}else
	{
	  $links .= '<font color="#cccccc">&lt&lt '.$cmdPrevious.'</font>&nbsp;';
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
	  $links .= '<a href="javascript:change_page('.$last_page.', '.$max_rows.');">&gt&gt</a>';
	}else
	{
	  $links .= '<font color="#cccccc">'.$cmdNext.' &gt&gt</font>';
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
      <?php
}
echo '</form>';
?>
<script type="text/javascript">
document.forms[0].subject.focus();

var nav4 = window.Event ? true : false;
function processkeypress(e)
{
  if(nav4)
  {      
  	var whichCode = e.which;
  }else         
  {
    var whichCode = event.keyCode;         
  }

  if (whichCode == 13)
  {
    document.forms[0].submit();        
    return true;
  }
}
if (window.Event) //if Navigator 4.X
{
  document.captureEvents(Event.KEYPRESS)
}
document.onkeypress = processkeypress;

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
	    document.forms[0].form_action.value="delete";
	    document.forms[0].submit();
	  }
	  break;

	default:
	  if (confirm("'.$strDeletePrefix.$strThis.' "+count+" '.$ml_messages2.$strDeleteSuffix.'"))
	  {
	    document.forms[0].form_action.value="delete";
	    document.forms[0].submit();
	  }
	  break;
	  ';
      }else
      {
	echo '
	default:
	  document.forms[0].form_action.value="delete";
	  document.forms[0].submit();
	  break;
	  ';
      }
      ?>
  }
}
</script>
<?php
$mail->close();
require($GO_THEME->theme_path."footer.inc");
?>
