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

if($calendar_module = $GO_MODULES->get_module('calendar'))
{
	require($calendar_module['path']."classes/calendar.class.inc");
}
$cal = new calendar();
require($GO_LANGUAGE->get_language_file('calendar'));

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
	if (isset($_POST['calendars']))
	{
		$cal->set_event_status($_REQUEST['event_id'], '1', $_POST['email']);
		while($calendar_id = array_shift($_POST['calendars']))
		{
			if (!$cal->event_is_subscribed($_POST['event_id'], $calendar_id))
			{
				if ($cal->subscribe_event($_POST['event_id'], $calendar_id))
				{
					if (!$cal->set_event_status($_POST['event_id'], '1', $_POST['email']))
					{
						$error = true;
					}
				}else
				{
					$error = true;
				}
			}
		}
		require($GO_THEME->theme_path.'header.inc');
		echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td><h1>'.$sc_accept_title.'</h1>';
		echo $sc_accept_confirm;

		if (isset($error))
			echo '<p class="Error">'.$strSaveError.'</p>';

		echo '</td></tr></table>';
		require($GO_THEME->theme_path.'footer.inc');
		exit();
	}else
	{
		$feedback = $sc_select_calendar_please;
	}
}


$event = $cal->get_event($_REQUEST['event_id']);
if ($event && $_REQUEST['email'] != '')
{
	if ($_REQUEST['member'] == "true")
	{
		$GO_SECURITY->authenticate(false);
		
		if ($calendar_module && $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_write']))
		{
			require($GO_THEME->theme_path.'header.inc');

			echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td><h1>'.$sc_accept_title.'</h1></td></tr><tr><td>';

			if (isset($feedback))
			{
				echo '<p class="Error">'.$feedback.'</p>';
			}
			if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $event["acl_read"]) && !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $event["acl_write"]))
			{
				$GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $event["acl_read"]);
			}

			$calendar_count = $cal->get_user_calendars($GO_SECURITY->user_id);
			$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : 0;

			if($calendar_count > 1)
			{
				echo $sc_select_calendar.': ';
				echo '<form name="accept" method="post" action="'.$_SERVER['PHP_SELF'].'">';
				echo '<input type="hidden" name="email" value="'.$_REQUEST['email'].'" />';
				echo '<input type="hidden" name="event_id" value="'.$_REQUEST['event_id'].'" />';
				echo '<input type="hidden" name="member" value="'.$_REQUEST['member'].'" />';
				echo '<table border="0">';

				while ($cal->next_record())
				{
					if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $cal->f('acl_write')))
					{
						$calendars_check = (isset($_POST['calendars']) && in_array($cal->f('id'), $_POST['calendars'])) ? 'checked' : '';
						echo '<tr><td><input type="checkbox" name="calendars[]" value="'.$cal->f('id').'" '.$calendars_check.'  /></td><td>'.$cal->f('name').'</td></tr>';
					}
				}
				echo '</table>';
				$button = new button($cmdOk, "javascript:document.forms[0].submit();");
				echo '</form>';
			}else
			{
				if ($calendar_count == 1)
				{
					$cal->next_record();
					$calendar_id = $cal->f('id');
					if ($calendar_id > 0)
					{
						if (!$cal->event_is_subscribed($_REQUEST['event_id'], $calendar_id))
						{
							if ($cal->subscribe_event($_REQUEST['event_id'], $calendar_id))
							{
								if ($cal->set_event_status($_REQUEST['event_id'], '1', $_REQUEST['email']))
								{
									echo $sc_accept_confirm;
								}else
								{
									echo $strSaveError;
								}
							}else
							{
								echo $strSaveError;
							}
						}else
						{
							if ($cal->set_event_status($_REQUEST['event_id'], '1', $_REQUEST['email']))
							{
								echo $sc_accept_confirm;
							}else
							{
								echo $strSaveError;
							}
						}
					}
				}else
				{
					echo $sc_no_calendars;
				}
			}
		}else
		{
			require($GO_THEME->theme_path.'header.inc');
			echo '<table border="0"  cellspacing="0"><tr><td><h1>'.$sc_accept_title.'</h1>';
			if ($cal->set_event_status($event_id,'1', $email))
			{
				echo $sc_accept_confirm;
			}
		}
	}else
	{
		require($GO_THEME->theme_path.'header.inc');
		echo '<table border="0"  cellspacing="0"><tr><td><h1>'.$sc_accept_title.'</h1>';
		if ($cal->set_event_status($_REQUEST['event_id'],'1', $_REQUEST['email']))
		{
			echo $sc_accept_confirm;
		}
	}
}else
{
	require($GO_THEME->theme_path.'header.inc');

	echo '<table border="0" cellspacing="0"><tr><td class="Error"><h1>'.$sc_accept_title.'</h1>';

	echo $sc_bad_event;
}

echo '</td></tr></table>';
require($GO_THEME->theme_path.'footer.inc');
?>
