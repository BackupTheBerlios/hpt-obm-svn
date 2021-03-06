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

define('SHOW_ALL', 0);
define('SHOW_OWN', 1);
define('SHOW_SUB', 2);

require("../../Group-Office.php");

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('calendar');
require($GO_LANGUAGE->get_language_file('calendar'));

require($GO_MODULES->path.'classes/calendar.class.inc');
$cal = new calendar();

$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$view_type = isset($_REQUEST['view_type']) ? $_REQUEST['view_type'] : 0;

$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : 0;
$db = new db();

switch ($task)
{
  case 'delete_calendar':
    $calendar = $cal->get_calendar($_POST['delete_calendar_id']);

    if($GO_SECURITY->user_id ==  $calendar['user_id'])
    {
      if ($cal->delete_calendar($_POST['delete_calendar_id']))
      {
	$holidays->delete_holidays($GO_SECURITY->user_id, $_POST['delete_calendar_id']);
	$GO_SECURITY->delete_acl($calendar['acl_write']);
	$GO_SECURITY->delete_acl($calendar['acl_read']);
      }
    }
    $db->query('SELECT calendar_id FROM cal_config WHERE user_id="'.$GO_SECURITY->user_id.'"');		
    while ($db->next_record())
      $subscribed[] = $db->f('calendar_id');
    break;

  case 'save_calendar':
    $cal->set_default_calendar($GO_SECURITY->user_id, $_POST['default_calendar_id']);
    if ($_POST['close_action'] == 'true')
    {
      header('Location: '.$return_to);
      exit();
    }
    break;

	case 'subscribe':
		$db->query('DELETE FROM cal_config WHERE user_id="'.$GO_SECURITY->user_id.'"');
		
		$subscribed = $_REQUEST['subscribed'];
		for ($i=0; $i<sizeof($subscribed); $i++)
			$db->query('INSERT INTO cal_config VALUES ("'.$GO_SECURITY->user_id.'","'.$subscribed[$i].'")');
    	if ($_POST['close_action'] == 'true')
    	{
      		header('Location: '.$return_to);
      		exit();
    	}
	break;
	
	default:
		$db->query('SELECT calendar_id FROM cal_config WHERE user_id="'.$GO_SECURITY->user_id.'"');		
		while ($db->next_record())
			$subscribed[] = $db->f('calendar_id');
}

$tabtable = new tabtable('calendar', $sc_calendars, '100%', '400', '120', '', true);
require($GO_THEME->theme_path.'header.inc');

echo '<form name="events" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="calendar_id" value="'.$calendar_id.'" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="delete_calendar_id" value="" />';
echo '<input type="hidden" name="task" value="'.$task.'" />';
echo '<input type="hidden" name="close_action" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
//echo '<input type="hidden" name="view_type" value="'.$view_type.'" />';

$dropbox = new dropbox();
$dropbox->add_value(0, $cmdShowAll);
$dropbox->add_value(1, $cmdShowOwned);
$dropbox->add_value(2, $cmdShowSubscribed);
$tabtable->print_head();
?>
<table border="0" cellpadding="10">
<tr>
<td>
<table border="0" cellpadding="3" cellspacing="0">
<tr><td colspan="1" height="25">
<a href="calendar.php?return_to=<?php echo rawurlencode($link_back); ?>" class="normal"><?php echo $cmdAdd; ?></a></td>
<td colspan="4" align="right"><?php echo "$sc_view: "; $dropbox->print_dropbox('view_type', $view_type, 'onchange="frm.submit()"'); ?>
</td></tr>
  <?php
if (isset($feedback))
{
  echo '<tr><td colspan="5" height="25">'.$feedback.'</td></tr>';
}

echo '<tr><td><h3>'.$strName.'</h3></td><td><h3>'.$sc_owner.'</h3></td><td><h3>'.$sc_subscribed.'</h3></td><td>&nbsp;</td><td>&nbsp;</td></tr>';

$settings = $cal->get_settings($GO_SECURITY->user_id);

$calendar_count = $cal->get_authorised_calendars($GO_SECURITY->user_id);
if ($calendar_count > 0)
{
  $subscr_count=0;		
  while ($cal->next_record())
  {
  	$checked = '';
	$uid = $cal->f('user_id');
	if (!isset($subscribed)) {
		if ($uid == $GO_SECURITY->user_id)
			$checked = 'checked';
		elseif ($view_type != SHOW_ALL) continue;
	}
	else {
		$go_next = true;
		if (in_array($cal->f('id'),$subscribed))
			$checked = 'checked';
		switch ($view_type) {
			case SHOW_OWN:
				$go_next = $uid == $GO_SECURITY->user_id;
				break;
			case SHOW_SUB:
				$go_next = $checked == 'checked';
				break;
		}
		if (!$go_next) continue;
	}
	
	$subscr_count++;		
	
    echo '<tr>';
//    echo '<td align="center"><input type="radio" name="default_calendar_id" value="'.$cal->f('id').'"></td>';
	echo '<td nowrap><a href="index.php?calendar_id='.$cal->f("id").'" class="normal">'.htmlspecialchars($cal->f("name")).'</a>&nbsp;</td>';
    echo '<td nowrap>'.show_profile($cal->f("user_id")).'&nbsp;</td>';
    echo '<td align="center"><input type="checkbox" id="subscribed" name="subscribed[]" value="'.$cal->f('id').'" '.$checked.'></td>';	
    echo '<td><a href="calendar.php?calendar_id='.$cal->f("id").'&return_to='.rawurlencode($link_back).'" title="'.$strEdit.' \''.htmlspecialchars(addslashes($cal->f("name"))).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
	if ($cal->f('user_id') != $GO_SECURITY->user_id)
		echo "<td>&nbsp;</td></tr>\n";
	else
    	echo "<td><a href='javascript:delete_calendar(\"".$cal->f("id")."\",\"".div_confirm_id($strDeletePrefix."'".addslashes($cal->f("name"))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cal->f("name"))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
  }
}
echo '</table>';
echo '<br /><br />';
$button = new button($cmdOk, "javascript:_save('subscribe', 'true', ".$subscr_count.", '".$sc_choice_calendar_msg."')");
echo '&nbsp;&nbsp;';
$button = new button($cmdApply, "javascript:_save('subscribe', 'false', ".$subscr_count.", '".$sc_choice_calendar_msg."')");
echo '&nbsp;&nbsp;';
$button = new button($cmdClose,"javascript:document.location='".$return_to."'");
?>
</td>
</tr>
</table>

<script type="text/javascript">
var frm = document.forms[0];

function _set_view_type()
{
	frm.submit();
}

function delete_calendar(calendar_id, message_id)
{
	if (div_confirm(message_id))
	{
		frm.delete_calendar_id.value = calendar_id;
		frm.task.value = 'delete_calendar';
		frm.submit();
	}
}

function _save(task, close, count, msg)
{
	have_check = false;
	if (count == 1)
	{
		if (frm.subscribed.checked == true)
			have_check = true;
	}
	else
	{
		for (i = 0; i < count; i++)
			if (frm.subscribed[i].checked == true)
				have_check = true;
	}
	
	if (!have_check) 
	{
		alert(msg);
		return;
	}
	
	frm.task.value = task;
	frm.close_action.value = close;
	frm.submit();
}

function save_calendar(close_me)
{
  frm.close_action.value = close_me;
  frm.task.value = 'save_calendar';
  frm.submit();
}
</script>
<?php
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path.'footer.inc');
?>
