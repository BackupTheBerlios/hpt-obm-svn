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

require_once($GO_MODULES->path.'classes/calendar.class.inc');
require_once($GO_MODULES->path.'classes/todos.class.inc');
$cal = new calendar();
$todos = new todos();

$page_title=$sc_reminder;
require($GO_THEME->theme_path."header.inc");
echo '<embed src="'.$GO_THEME->sounds['reminder'].'" hidden="true" autostart="true"><noembed><bgsound src="'.$GO_THEME->sounds['reminder'].'"></noembed>';


if($event_count = $cal->get_events_to_remind($GO_SECURITY->user_id))
{
	echo '<h2>'.$sc_events.'</h2>';

	echo 	'<table border="0" cellspacing="0" cellpadding="0" width="450">'.
				'<tr height="20"><td class="TableHead2">'.$strName.'</td>'.
				'<td class="TableHead2">'.$strDate.'</td></tr>';
	$cal2 = new calendar();
	while($cal->next_record())
	{
		$event = $cal2->get_event($cal->f('event_id'));
		$next_recurrence_time = $cal2->get_next_recurrence_time(0,0, $event);

		echo '<tr><td><a class="normal" href="javascript:goto_event(\''.$cal->f('event_id').'\')">'.$event['name'].'</a></td>';
		if($event['all_day_event'])
		{
			$date_format = $_SESSION['GO_SESSION']['date_format'];
			$timezone_offset = 0;
		}else
		{
			$date_format = $_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'];
			$timezone_offset = $_SESSION['GO_SESSION']['timezone']*3600;
		}

		echo '<td>'.date($date_format, $next_recurrence_time+$timezone_offset).'</td></tr>';

		$update_reminder = $cal2->get_next_recurrence_time(0, $next_recurrence_time, $event);

		if ($update_reminder > $next_recurrence_time)
		{
			$cal2->update_reminder($GO_SECURITY->user_id, $event['id'], $update_reminder);
		}else
		{
			$cal2->delete_reminder($GO_SECURITY->user_id, $event['id']);
		}
	}
}

if ($todo_count = $todos->get_todos_to_remind($GO_SECURITY->user_id))
{
	$todos2=new todos();
	echo '<h2>'.$cal_todos.'</h2>';

	echo 	'<table border="0" cellspacing="0" cellpadding="0" width="450">'.
				'<tr height="20"><td class="TableHead2">'.$strName.'</td>'.
				'<td class="TableHead2">'.$sc_start_at.'</td>'.
				'<td class="TableHead2">'.$cal_due_at.'</td></tr>';
	while($todos->next_record())
	{
		$todos2->set_reminded($todos->f('id'));

		echo 	'<tr><td><a class="normal" href="javascript:goto_todo(\''.
					$todos->f('id').'\')">'.$todos->f('name').'</a></td>'.
					'<td>'.date($_SESSION['GO_SESSION']['date_format'].
					' '.$_SESSION['GO_SESSION']['time_format'], $todos->f('start_time')).
					'</td>'.
					'<td>'.date($_SESSION['GO_SESSION']['date_format'].
					' '.$_SESSION['GO_SESSION']['time_format'], $todos->f('due_time')).
					'</td></tr>';
	}
}
echo '</table><br />';


$button = new button($cmdClose, "javascript:window.close()");

?>
<script type="text/javascript">
function goto_event(event_id)
{
	opener.parent.main.document.location=
		'<?php echo $GO_MODULES->url; ?>event.php?event_id='+event_id+'&return_to='+escape(opener.parent.main.location);
	opener.parent.main.focus();
}

function goto_todo(todo_id)
{
	opener.parent.main.document.location=
		'<?php echo $GO_MODULES->url; ?>todo.php?todo_id='+todo_id+'&return_to='+escape(opener.parent.main.location);
	opener.parent.main.focus();
}
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
