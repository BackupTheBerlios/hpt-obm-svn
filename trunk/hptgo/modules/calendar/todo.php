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

//get the local times
$local_time = get_time();
$year = isset($_REQUEST['year']) ? $_REQUEST['year'] : gmdate("Y", $local_time);
$month = isset($_REQUEST['month']) ? $_REQUEST['month'] : gmdate("m", $local_time);
$day = isset($_REQUEST['day']) ? $_REQUEST['day'] : gmdate("j", $local_time);
$hour = isset($_REQUEST['hour']) ? $_REQUEST['hour'] : date("H", $local_time);
$min = isset($_REQUEST['min']) ? $_REQUEST['min'] : gmdate("i", $local_time);

$hours = array("00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23");
for ($i=0;$i<=60;$i++)
{
	$text = strlen($i) < 2 ? '0'.$i : $i;
	$mins[] = $text;
}

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('calendar');
require($GO_LANGUAGE->get_language_file('calendar'));

require_once($GO_MODULES->path.'classes/todos.class.inc');
$todos = new todos();

$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

$todo_id = isset($_REQUEST['todo_id']) ? $_REQUEST['todo_id'] : 0;

if ($task == 'save_todo')
{
	$name = smart_addslashes(trim($_POST['name']));
	if ($name == '')
	{
		$feedback = '<p class="Error">'.$error_missing_field.'</p>';
	}else
	{
		$start_time = date_to_unixtime($_POST['start_date'].' '.$_POST['start_hour'].':'.$_POST['start_min'])-
																	($_SESSION['GO_SESSION']['timezone']*3600);

		$due_time = date_to_unixtime($_POST['due_date'].' '.$_POST['due_hour'].':'.$_POST['due_min'])-
																($_SESSION['GO_SESSION']['timezone']*3600);


		if (isset($_POST['completed']))
		{
			$status = '100';
			$completion_time = date_to_unixtime($_POST['completion_date'].' '.$_POST['completion_hour'].':'.$_POST['completion_min'])-
																					($_SESSION['GO_SESSION']['timezone']*3600);
		}else
		{
			$status = $_POST['status'];
			$completion_time = 0;
		}

		//substract timezone offset

		if (isset($_POST['reminder']))
		{
			$remind_style = $_POST['remind_style'];
			if ($remind_style == REMIND_BEFORE_STARTTIME)
			{
				$remind_time = $start_time - $_POST['remind_interval'];
			}else
			{
				$remind_time = $due_time - $_POST['remind_interval'];
			}
		}else
		{
			$remind_style = '0';
			$remind_time = '0';
		}

		if ($todo_id > 0)
		{
			if (!$todos->update_todo($todo_id,
															$_POST['contact_id'],
															$_POST['res_user_id'],
															$start_time,
															$due_time,
															$status,
															$_POST['priority'],
															$completion_time,
															$remind_time,
															$remind_style,
															$name,
															smart_addslashes($_POST['description']),
															smart_addslashes($_POST['location']),
															$_POST['background']))
			{
				$feedback = '<p class="Error">'.$strSaveError.'</p>';
			}else
			{
				if ($_POST['close'] == 'true')
				{
					header('Location: '.$return_to);
					exit();
				}
			}

		}else
		{
			if (!$todo_id = $todos->add_todo($GO_SECURITY->user_id,
																				$_POST['contact_id'],
																				$_POST['res_user_id'],
																				$start_time,
																				$due_time,
																				$status,
																				$_POST['priority'],
																				$completion_time,
																				$remind_time,
																				$remind_style,
																				$name,
																				smart_addslashes($_POST['description']),
																				smart_addslashes($_POST['location']),
																				$_POST['background']))
			{
				$feedback = '<p class="Error">'.$strSaveError.'</p>';
			}else
			{
				if ($_POST['close'] == 'true')
				{
					header('Location: '.$return_to);
					exit();
				}
			}
		}
	}
}

if ($todo_id > 0 && $task != 'save_todo')
{
	//get the todo
	$todo = $todos->get_todo($todo_id);

	$todo['start_time'] += ($_SESSION['GO_SESSION']['timezone']*3600);
	$todo['start_hour'] = date('G', $todo['start_time']);
	$todo['start_min'] = date('i', $todo['start_time']);

	$todo['due_time'] += ($_SESSION['GO_SESSION']['timezone']*3600);
	$todo['due_hour'] = date('G', $todo['due_time']);
	$todo['due_min'] = date('i', $todo['due_time']);

	$todo['start_date'] = date($_SESSION['GO_SESSION']['date_format'], $todo['start_time']);
	$todo['due_date'] = date($_SESSION['GO_SESSION']['date_format'], $todo['due_time']);

	if ($todo['completion_time'] > 0)
	{
		$todo['completion_time'] += ($_SESSION['GO_SESSION']['timezone']*3600);
		$todo['completed'] = true;
		$todo['completion_date'] = date($_SESSION['GO_SESSION']['date_format'], $todo['completion_time']);
		$todo['completion_hour'] = date('G', $todo['completion_time']);
		$todo['completion_min'] = date('i', $todo['completion_time']);

	}else
	{
		$todo['completed'] = false;
		$todo['completion_date'] = date($_SESSION['GO_SESSION']['date_format'], $local_time);
		$todo['completion_hour'] = date('G', $local_time);
		$todo['completion_min'] = date('i', $local_time);
	}

	if ($todo['remind_style'] == REMIND_BEFORE_STARTTIME)
	{
		$todo['remind_interval'] = $todo['start_time'] - $todo['remind_time'];
	}else
	{
		$todo['remind_interval'] = $todo['due_time'] - $todo['remind_time'];
	}

	$todo['reminder'] = ($todo['remind_time'] > 0) ? true : false;
	$title = $todo['name'];
}else
{
	$title = $sc_new_app;

	$date = date($_SESSION['GO_SESSION']['date_format'], $local_time);

	//new todo declare all vars
	$todo['description'] = isset($_POST['description']) ? smartstrip($_POST['description']) : '';
	$todo['name'] = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
	$todo['contact_id'] = isset($_REQUEST['contact_id']) ? $_REQUEST['contact_id'] : '';

	$todo['start_date'] = isset($_POST['start_date']) ? smartstrip($_POST['start_date']) : $date;
	$tmp = (strlen($hour) == 1) ? '0'.$hour : $hour;
	$todo['start_hour'] = isset($_POST['start_hour']) ? $_POST['start_hour'] : $tmp;
	$todo['start_min'] = isset($_POST['start_min']) ? $_POST['start_min'] : '00';

	$todo['due_date'] = isset($_POST['due_date']) ? $_POST['due_date'] : $date;
	$tmp = (strlen($hour+1) == 1) ? '0'.$hour+1 : $hour+1;
	$todo['due_hour'] = isset($_POST['due_hour']) ? $_POST['due_hour'] : $tmp;
	$todo['due_min'] = isset($_POST['due_min']) ? $_POST['due_min'] : '00';

	$todo['completed'] = isset($_POST['completed']) ? true : false;
	$todo['completion_date'] = isset($_POST['completion_date']) ? $_POST['completion_date'] : $date;

	$todo['completion_hour'] = date('G', $local_time);
	$todo['completion_min'] = date('i', $local_time);

	$todo['reminder'] = isset($_POST['reminder']) ? true :false;
	$todo['remind_interval'] = isset($_POST['remind_interval']) ? $_POST['remind_interval'] :'300';
	$todo['remind_style'] = isset($_POST['remind_style']) ? $_POST['remind_style'] : REMIND_BEFORE_STARTTIME;
	$todo['background'] = isset($_POST['background']) ? $_POST['background'] :'FFFFCC';
	$todo['location'] = isset($_POST['location']) ? smartstrip($_POST['location']) :'';
	$todo['priority'] = isset($_POST['priority']) ? smartstrip($_POST['priority']) : '1';

	$todo['status'] = isset($_POST['status']) ? smartstrip($_POST['status']) :'1';
	$todo['res_user_id'] = isset($_POST['res_user_id']) ? smartstrip($_POST['res_user_id']) :$GO_SECURITY->user_id;
}

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();
require($GO_THEME->theme_path.'header.inc');

echo '<form name="todo_form" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="todo_id" value="'.$todo_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';

//address_string used by the addressbok selector
echo '<input type="hidden" name="address_string" value="" />';

$tabtable = new tabtable('todo_table', $title, '600', '400', '120', '', true);
$tabtable->print_head();
echo '<br />';

echo '<table border="0" cellpadding="2" cellspacing="0">';
if (isset($feedback))
{
	echo '<tr><td colspan="2" class="Error">'.$feedback.'</td></tr>';
}
echo 	'<tr><td>'.$strName.':&nbsp;</td>'.
			'<td><input type="text" class="textbox" maxlength="50" name="name" style="width: 300px;" value="'.htmlspecialchars($todo['name']).'" /></td></tr>'.
			'<tr><td>';

$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
	$ab_module = false;
}else
{
	require_once ($ab_module['path'].'classes/addressbook.class.inc');
}

if ($todo['res_user_id'] > 0 && $user = $GO_USERS->get_user($todo['res_user_id']))
{
	$middle_name = $user['middle_name'] == '' ? '' : $user['middle_name'].' ';
	$user_name = htmlspecialchars($user['last_name'].' '.$middle_name.$user['first_name']);
}else
{
	$user_name = isset($_REQUEST['user_name']) ? htmlspecialchars(smartstrip($_REQUEST['user_name'])) : '';
}

$select = new select('user', 'todo_form', 'res_user_id', $todo['res_user_id']);
echo '<tr><td>';
$select->print_link($cal_responsible);
echo ':</td><td>';
$select->print_field();
echo '</td></tr>';

if($ab_module)
{
	$select = new select('contact', 'todo_form', 'contact_id', $todo['contact_id']);
	echo '<tr><td>';
	$select->print_link($sc_client);
	echo ':</td><td>';
	$select->print_field();
	echo '</td></tr>';
}else
{
	echo '<input type="hidden" value="0" name="contact_id" />';
}
echo '<tr><td>'.$sc_location.':</td>'.
			'<td><input type="text" class="textbox" style="width: 300px;"'.
			'name="location" value="'.htmlspecialchars($todo['location']).
			'" /></td></tr>'.
			'<tr><td valign="top">'.$sc_description.':&nbsp;</td><td>'.
			'<textarea class="textbox" name="description" cols="60" rows="4">'.
			htmlspecialchars($todo['description']).'</textarea></td></tr>'.
			'<tr><td colspan="2">&nbsp;</td></tr>';

echo '<tr><td>'.$cal_priority.':</td><td>';
$dropbox = new dropbox();
$dropbox->add_value('0', $cal_priority_values[0]);
$dropbox->add_value('1', $cal_priority_values[1]);
$dropbox->add_value('2', $cal_priority_values[2]);
$dropbox->print_dropbox('priority', $todo['priority']);
echo '</td></tr>';


echo '<tr><td>'.$sc_start_at.':&nbsp;</td><td>';
echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td>';

$datepicker->print_date_picker('start_date', $_SESSION['GO_SESSION']['date_format'], $todo['start_date'], '', '', 'onchange="javascript:document.forms[0].due_date.value=this.value;"');

echo '</td><td>&nbsp;&nbsp;';

$dropbox = new dropbox();
$dropbox->add_arrays($hours, $hours);
$dropbox->print_dropbox("start_hour", $todo['start_hour'], 'onchange="javascript:update_due_hour(this.value);"');
echo '&nbsp;:&nbsp;';
$dropbox = new dropbox();
$dropbox->add_arrays($mins, $mins);
$dropbox->print_dropbox("start_min", $todo['start_min'], 'onchange="javascript:document.forms[0].due_min.value=this.value;"');

echo '</td></tr></table>';
echo '</td></tr>';

echo '<tr><td>'.$cal_due_at.':&nbsp;</td><td>';

echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td>';
$datepicker->print_date_picker('due_date', $_SESSION['GO_SESSION']['date_format'], $todo['due_date']);
echo '</td><td>&nbsp;&nbsp;';
$dropbox = new dropbox();
$dropbox->add_arrays($hours, $hours);
$dropbox->print_dropbox("due_hour", $todo['due_hour']);
echo '&nbsp;:&nbsp;';
$dropbox = new dropbox();
$dropbox->add_arrays($mins, $mins);
$dropbox->print_dropbox("due_min", $todo['due_min']);
echo '</td></tr></table>';
echo '</td></tr>';

echo '<tr><td colspan="2">&nbsp;</td></tr>';
echo '<tr><td>';
$checkbox = new checkbox('reminder', 'true', $sc_reminder.':', $todo['reminder'], false, 'onclick="javascript:disable_reminder(!this.checked);"');
echo'</td><td>';

$dropbox=new dropbox();
$dropbox->add_value('300', '5 '.$sc_mins);
$dropbox->add_value('900', '15 '.$sc_mins);
$dropbox->add_value('1800', '30 '.$sc_mins);
$dropbox->add_value('2700', '45 '.$sc_mins);
$dropbox->add_value('3600', '1 '.$sc_hour);
$dropbox->add_value('7200', '2 '.$sc_hours);
$dropbox->add_value('10800', '3 '.$sc_hours);
$dropbox->add_value('14400', '4 '.$sc_hours);
$dropbox->add_value('18000', '5 '.$sc_hours);
$dropbox->add_value('21600', '6 '.$sc_hours);
$dropbox->add_value('25200', '7 '.$sc_hours);
$dropbox->add_value('28800', '8 '.$sc_hours);
$dropbox->add_value('32400', '9 '.$sc_hours);
$dropbox->add_value('36000', '10 '.$sc_hours);
$dropbox->add_value('39600', '11 '.$sc_hours);
$dropbox->add_value('43200', '12 '.$sc_hours);
$dropbox->add_value('86400', '1 '.$sc_day);
$dropbox->add_value('172800', '2 '.$sc_days);
$dropbox->add_value('259200', '3 '.$sc_days);
$dropbox->add_value('345600', '4 '.$sc_days);
$dropbox->add_value('432000', '5 '.$sc_days);
$dropbox->add_value('518400', '6 '.$sc_days);
$dropbox->add_value('604800', '1 '.$sc_week);
$dropbox->add_value('1209600', '2 '.$sc_weeks);
$dropbox->add_value('1814400', '3 '.$sc_weeks);

$disabled = $todo['reminder']  ? '' : 'disabled';

$dropbox->print_dropbox('remind_interval', $todo['remind_interval'], $disabled);

$dropbox = new dropbox();
$dropbox->add_value(REMIND_BEFORE_STARTTIME, $cal_before_task_start);
$dropbox->add_value(REMIND_BEFORE_DUETIME, $cal_before_task_due);
$dropbox->print_dropbox('remind_style', $todo['remind_style'], $disabled);
echo '</td></tr>';

echo '<tr><td colspan="2">&nbsp;</td></tr>';

echo '<tr><td>';
$checkbox = new checkbox('completed', 'true', $cal_completed.':', $todo['completed'], false, 'onclick="javascript:disable_completion_time(!this.checked);"');
echo '</td><td>';
echo '<table border="0" cellpadding="0" cellspacing="0"><tr><td>';

$disabled = $todo['completed']  ? '' : 'disabled';
$status_disabled = $todo['completed']  ? 'disabled' : '';

$datepicker->print_date_picker('completion_date', $_SESSION['GO_SESSION']['date_format'], $todo['completion_date'], '', '', '', !$todo['completed']);

echo '</td><td>&nbsp;&nbsp;';

$dropbox = new dropbox();
$dropbox->add_arrays($hours, $hours);
$dropbox->print_dropbox("completion_hour", $todo['completion_hour'], $disabled);
echo '&nbsp;:&nbsp;';
$dropbox = new dropbox();
$dropbox->add_arrays($mins, $mins);
$dropbox->print_dropbox("completion_min", $todo['completion_min'], $disabled);

echo '&nbsp;&nbsp;&nbsp;&nbsp;';

$dropbox = new dropbox();

for ($i=0;$i<101;$i=$i+10)
{
	$dropbox->add_value($i, "$i");
}
$dropbox->print_dropbox('status', $todo['status'], $status_disabled);
echo '&nbsp;&nbsp;';
echo $cal_percent_completed;
echo '</td></tr></table>';

echo '<tr><td colspan="2">&nbsp;</td></tr>';
echo '<tr><td nowrap>'.$sc_background.':&nbsp;</td><td>';
$color_selector = new color_selector();
$color_selector->add_color('FFFFCC');
$color_selector->add_color('FF6666');
$color_selector->add_color('CCFFCC');
$color_selector->add_color('99CCFF');
$color_selector->add_color('FF99FF');
$color_selector->add_color('FFCC66');
$color_selector->add_color('CCCC66');
$color_selector->add_color('F1F1F1');
$color_selector->add_color('FFCCFF');
$color_selector->print_color_selector('background', $todo['background']);
echo '</td></tr>';

echo '<tr><td colspan="2">';
$button = new button($cmdOk, "javascript:save_todo('true');");
echo '&nbsp;&nbsp;';
$button = new button($cmdApply, "javascript:save_todo('false');");
echo '&nbsp;&nbsp;';
if ($todo_id > 0)
{
	$button = new button($cal_export, "document.location='export.php?todo_id=$todo_id';");
	echo '&nbsp;&nbsp;';
}
$button = new button($cmdCancel, "javascript:document.location='$return_to'");
echo '</td></tr>';
echo '</table>';
?>
<script type="text/javascript" language="javascript">
<?php
if ($ab_module)
{

?>
	function open_addressbook(field, address_string)
	{
		var popup = window.open('about:blank', 'ab_select', 'width=550,height=400,scrollbars=yes,resizable=yes,status=yes');
		if (!popup.opener) popup.opener = self;
		popup.focus();

		document.todo_form.address_string.value = address_string;
		document.todo_form.action = '<?php echo $ab_module['url']."select.php?show_contacts=true&show_users=true&show_companies=true&multiselect=true&require_email_address=true&GO_HANDLER=".$GO_MODULES->url."add_contacts.php&GO_FIELD="; ?>'+field;
		document.todo_form.target = 'ab_select';
		document.todo_form.submit();
		document.todo_form.target = '_self';
		document.todo_form.action = '<?php echo $_SERVER['PHP_SELF']; ?>';
	}
<?php
}
?>

function disable_completion_time(value)
{
	document.todo_form.completion_date.disabled=value;
	document.todo_form.completion_date_button.disabled=value;
	document.todo_form.completion_hour.disabled=value;
	document.todo_form.completion_min.disabled=value;
	document.todo_form.status.disabled=!value;
}

function update_due_hour(start_hour)
{
	if (start_hour == 24)
	{
		document.forms[0].due_hour.value='01';
	}else
	{
		start_hour = parseInt(start_hour)+1
		if (start_hour < 10)
		{
			start_hour = "0"+start_hour;
		}
		document.forms[0].due_hour.value= start_hour;
	}
}

function get_date(dateString)
{
	<?php
	if ($_SESSION['GO_SESSION']['date_format'] == "d-m-Y")
	{
		echo "
			var date = new Date(dateString.substring(6,10),
						dateString.substring(3,5)-1,
						dateString.substring(0,2),
						dateString.substring(11,13),
						dateString.substring(14,16)
						);";
	}else
	{
		echo "
			var date = new Date(dateString.substring(6,10),
						dateString.substring(0,2),
						dateString.substring(3,5)-1,
						dateString.substring(11,13),
						dateString.substring(14,16)
						);";
	}
	?>

	return date;
}

function save_todo(close)
{
	start_date = get_date(document.todo_form.start_date.value.replace(/-/g,'/')+' '+document.todo_form.start_hour.value+':'+document.todo_form.start_min.value+':00');
	due_date = get_date(document.todo_form.due_date.value.replace(/-/g,'/')+' '+document.todo_form.due_hour.value+':'+document.todo_form.due_min.value+':00');

	if (start_date > due_date)
	{
		alert("<?php echo $sc_start_later; ?>");
		return;
	}

	document.todo_form.task.value = 'save_todo';
	document.todo_form.close.value = close;
	document.todo_form.submit();

}

function disable_reminder(value)
{
	document.todo_form.remind_interval.disabled=value;
	document.todo_form.remind_style.disabled=value;
}

function remove_client()
{
	document.todo_form.contact_id.value = 0;
	document.todo_form.contact_name.value = '';
	document.todo_form.contact_name_text.value = '';
}
document.todo_form.name.focus();
</script>
<?php
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path.'footer.inc');
?>
