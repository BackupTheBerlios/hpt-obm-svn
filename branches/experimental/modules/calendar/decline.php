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

require($GO_MODULES->path."classes/calendar.class.inc");
$cal = new calendar();
require($GO_LANGUAGE->get_language_file('calendar'));
require($GO_THEME->theme_path.'header.inc');

echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td><h1>'.$sc_decline_title.'</h1>';

if ($_REQUEST['task']=='submit')
{
if ($_REQUEST['event_id'] > 0 && $_REQUEST['email'] != '')
{
	if ($cal->set_event_status($_REQUEST['event_id'],'2', $_REQUEST['email']))
	{
		$db = new db();
		
		$email = $_REQUEST['email'];
		$db->query("SELECT first_name, middle_name, last_name FROM users WHERE email='".$email."'");
		if ($db->next_record())
			$name = $db->f('last_name') .' '. $db->f('middle_name') .' '. $db->f('first_name');
		
//		$db->query("SELECT u.email, c.name FROM cal_events c INNER JOIN users u ON c.user_id = u.id WHERE c.id = '".$_REQUEST['event_id']."'");
		$db->query("SELECT u.email, u.date_format, u.time_format, u.DST, c.* FROM cal_events c INNER JOIN users u ON c.user_id = u.id WHERE c.id = '".$_REQUEST['event_id']."'");
		
		if ($db->next_record())
		{
			$mail_to = $db->f('email');
			$title = $db->f('name');
			
			require('decline_mail.php');
			
			require_once($GO_CONFIG->root_path.'lib/tkdlib.php');
			
			echo send_mail($mail_to,$mail_body,$sc_declined_mail_title,$name,$email);
			
			echo $sc_decline_confirm;
		}
	}
}else
{
	echo $sc_bad_event;
}
}
else
{
echo '</td></tr>';
echo '<tr>';
echo '<form name="frmdecline" method="post">';
echo '<td>'.$sc_reason.' : <input type="textbox" name="reason">';
echo '<input type="hidden" name="task" value="submit"><input type="hidden" name="event_id" value="'.$_REQUEST['event_id'].'"><input type="hidden" name="email" value="'.$_REQUEST['email'].'"></td>';
echo '<td align="right">';
$button = new button($cmdOk,'javascript:document.forms[0].submit();');
echo '</td>';
}
echo '</td></tr></table>';

require($GO_THEME->theme_path.'footer.inc');
?>
