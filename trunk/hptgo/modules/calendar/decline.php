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
if ($_REQUEST['event_id'] > 0 && $_REQUEST['email'] != '')
{
	if ($cal->set_event_status($_REQUEST['event_id'],'2', $_REQUEST['email']))
	{
		echo $sc_decline_confirm;
	}
}else
{
	echo $sc_bad_event;
}

echo '</td></tr></table>';
require($GO_THEME->theme_path.'footer.inc');
?>
