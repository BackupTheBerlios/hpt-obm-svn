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
$GO_MODULES->authenticate('calendar');

require($GO_LANGUAGE->get_language_file('calendar'));

require($GO_MODULES->path.'classes/calendar.class.inc');
$cal = new calendar();

$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : $cal->get_default_calendar($GO_SECURITY->user_id);
$event_id = isset($_REQUEST['event_id']) ? $_REQUEST['event_id'] : 0;
$calendar = $cal->get_calendar($calendar_id);
$event = $cal->get_event($_REQUEST['event_id']);
if ($calendar && $event)
{
	$calendar['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_write']);
	$event['write_permission'] = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $event['acl_write']);
}else
{
	exit($strDataError);
}

switch($task)
{
	case 'delete':
	if ($event['write_permission'])
	{
		$cal->delete_event($event_id);
	}
	/*$return_to_url = $GO_MODULES->url;
	if ($return_to_url = parse_url($return_to))
	{
		parse_str($return_to_url['query'], $query_str);
		if (isset($query_str['return_to']))
		{
			$return_to = $query_str['return_to'];
		}
	}*/

	header('Location: '.$return_to);
	exit();
	break;

	case 'unsubscribe':
		if ($calendar['write_permission'])
		{
			if ($cal->get_event_subscribtions($event_id) < 2)
			{
				if($cal->delete_event($event_id))
				{
					$GO_SECURITY->delete_acl($event['acl_read']);
					$GO_SECURITY->delete_acl($event['acl_write']);
				}

			}else
			{
				$cal->unsubscribe_event($event_id, $calendar_id);
			}
		}
		/*$return_to_url = $GO_MODULES->url;
		if ($return_to_url = parse_url($return_to))
		{
			if (isset($return_to_url['query']))
			{
				parse_str($return_to_url['query'], $query_str);
				if (isset($query_str['return_to']))
				{
					$return_to = $query_str['return_to'];
				}
			}
		}*/

		header('Location: '.$return_to);
		exit();
	break;

}

require($GO_THEME->theme_path.'header.inc');
?>
<form method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="event_id" value="<?php echo $event_id; ?>" />
<input type="hidden" name="calendar_id" value="<?php echo $calendar_id; ?>" />
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>" />
<input type="hidden" name="task" value="<?php echo $task; ?>" />

<table border="0" cellpadding="2">
<tr>
	<td>
	<table border="0" cellpadding="4">
	<tr>
		<td><img src="<?php echo $GO_THEME->images['questionmark']; ?>" border="0" /></td><td align="center"><h2><?php echo $sc_delete_event; ?></h2></td>
	</tr>
	</table>
	</td>
</tr>
<tr>
	<td><?php echo $sc_delete_pre." '".$event['name']."' ".$sc_delete_suf; ?></td>
</tr>
<tr>
	<td>
	<br />
	<?php
	if ($calendar['write_permission'])
	{
		$button = new button($sc_from_calendar,"javascript:document.forms[0].task.value='unsubscribe';document.forms[0].submit();");
		echo '&nbsp;&nbsp;';
	}
	if ($event['write_permission'])
	{
		$button = new button($sc_enitrely,"javascript:document.forms[0].task.value='delete';document.forms[0].submit();");

	}
	echo '&nbsp;&nbsp;';
	$button = new button($cmdCancel,"javascript:document.location='".$return_to."';");
	?>
	</td>
</tr>
</table>
</form>
<?php

require($GO_THEME->theme_path.'footer.inc');
?>