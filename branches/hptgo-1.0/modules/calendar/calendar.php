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

$date = isset($_REQUEST['date']) ? $_REQUEST['date'] : getdate();
$year = isset($_POST['year']) ? $_POST['year'] : $date["year"];
$month = isset($_POST['month']) ? $_POST['month'] : $date["mon"];
$day = isset($_POST['day']) ? $_POST['day'] : $date["mday"];

$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : 0;

$hours = array("00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23");

switch($task)
{
	case 'import':
		if (!isset($_FILES['ical_file']['tmp_name']))
		{
			$feedback = 'No file';
		}else
		{
			if($cal->import_ical_file($GO_SECURITY->user_id, $_FILES['ical_file']['tmp_name'], $calendar_id))
			{
				$feedback = '<p class="Success">'.$cal_import_success.'</p>';
			}else
			{
				$feedback = '<p class="Error">'.$strDataError.'</p>';
			}
			unlink($_FILES['ical_file']['tmp_name']);
		}
	break;

	case 'save':

		$name = smart_addslashes(trim($_POST['name']));
		if ($name != "")
		{
			if ($calendar_id > 0)
			{
				$existing_calendar = $cal->get_calendar_by_name($name);

				if ($existing_calendar && $existing_calendar['id'] != $calendar_id)
				{
					$feedback = "<p class=\"Error\">".$sc_calendar_exists."</p>";

				}else
				{
					$cal->update_calendar($calendar_id, $name, $_POST['calendar_start_hour'], $_POST['calendar_end_hour']);
					if ($_POST['close'] == 'true')
					{
						header('Location: '.$return_to);
						exit();
					}
				}
			}else
			{
				if ($cal->get_calendar_by_name($name))
				{
					$feedback = "<p class=\"Error\">".$sc_calendar_exists."</p>";
				}else
				{
					if ($calendar_id = $cal->add_calendar($GO_SECURITY->user_id, $name, $_POST['calendar_start_hour'], $_POST['calendar_end_hour']))
					{
						$db = new db();
						$db->query('INSERT INTO cal_config VALUES ("'.$GO_SECURITY->user_id.'","'.$calendar_id.'")');
						if ($_POST['close'] == 'true')
						{
							header('Location: '.$return_to);
							exit();
						}
					}else
					{
						$feedback = "<p class=\"Error\">".$strSaveError."</p>";
					}
				}
			}
		}else
		{
			$feedback = "<p class=\"Error\">".$error_missing_field."</p>";
		}
	break;
}

if ($calendar_id > 0)
{
	$calendar = $cal->get_calendar($calendar_id);
	$title = $calendar['name'];
	$has_write_permission = $GO_SECURITY->has_permission($GO_SECURITY->user_id,$calendar['acl_write']);
}else
{
	$calendar['start_hour'] = isset($_POST['calendar_start_hour']) ? $_POST['calendar_start_hour'] : '07';
	$calendar['end_hour'] = isset($_POST['calendar_end_hour']) ? $_POST['calendar_end_hour'] : '20';
	$calendar['name'] = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
	$title = $sc_new_calendar;
}

$tabtable = new tabtable('calendar', $title, '100%', '400', '120', '', true);
if ($calendar_id > 0)
{
	$tabtable->add_tab('calendar', $strProperties);
	$tabtable->add_tab('holidays', $sc_holidays);
	if ($has_write_permission)
		$tabtable->add_tab('import', $cal_import);
	$tabtable->add_tab('read_permissions', $strReadRights);
	$tabtable->add_tab('write_permissions', $strWriteRights);
}

if ($tabtable->get_active_tab_id() == 'holidays')
{
	$datepicker = new date_picker();
	$GO_HEADER['head'] = $datepicker->get_header();
}
require($GO_THEME->theme_path.'header.inc');

echo '<form name="event" method="post" action="'.$_SERVER['PHP_SELF'].'" enctype="multipart/form-data">';
echo '<input type="hidden" name="calendar_id" value="'.$calendar_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';

$tabtable->print_head();
switch($tabtable->get_active_tab_id())
{
	case 'read_permissions':
		$read_only = ($calendar['user_id'] == $GO_SECURITY->user_id) ? false : true;
		print_acl($calendar['acl_read'], $read_only);
		echo '<br /><br />';
    		echo '&nbsp;&nbsp;&nbsp;&nbsp;';
		$button = new button($cmdClose,"javascript:document.location='".$return_to."'");
	break;
	
	case 'write_permissions':
		$read_only = ($calendar['user_id'] == $GO_SECURITY->user_id) ? false : true;
		print_acl($calendar['acl_write'], $read_only);
		echo '<br /><br />';
    		echo '&nbsp;&nbsp;&nbsp;&nbsp;';
		$button = new button($cmdClose,"javascript:document.location='".$return_to."'");
	break;

	case 'holidays':
		require('holidays.inc');
	break;

	case 'import':
		require('import.inc');
	break;

	default:
	?>
	<table border="0" cellpadding="5" cellspacing="0">
	<?php
	if (isset($feedback))
	{
		echo '<tr><td colspan="2">'.$feedback.'</td></tr>';
	}
	?>
	<tr>
		<td>
		<?php echo $strName; ?>:
		</td>
		<td>
		<?php if ($has_write_permission) { ?>	
		<input type="text" class="textbox" name="name" maxlength="100" size="50" value="<?php echo htmlspecialchars($calendar['name']); ?>" />
		<?php } else { 
			echo htmlspecialchars($calendar['name']);
		}
		?>
		</td>
	</tr>
	<tr>
		<td>
		<?php echo $sc_show_hours; ?>:
		</td>
		<td>
		<?php
		if ($has_write_permission) {
			$dropbox = new dropbox();
			$dropbox->add_arrays($hours, $hours);
			$dropbox->print_dropbox('calendar_start_hour', $calendar['start_hour']);
		} else {
			echo $calendar['start_hour'];
		}
		?>
		&nbsp;<?php echo $sc_to; ?>&nbsp;
		<?php
		if ($has_write_permission) {
			$dropbox = new dropbox();
			$dropbox->add_arrays($hours, $hours);
			$dropbox->print_dropbox('calendar_end_hour', $calendar['end_hour']);
		} else {
			echo $calendar['end_hour'];
		}
		?>
		</td>
	</tr>
	<tr>
		<td colspan="2">
			<?php
			if ($has_write_permission) {
				$button = new button($cmdOk,"javascript:document.forms[0].close.value='true';document.forms[0].task.value='save';document.forms[0].submit()");
				echo '&nbsp;&nbsp;';
				$button = new button($cmdApply,"javascript:document.forms[0].task.value='save';document.forms[0].submit()");
				echo '&nbsp;&nbsp;';
			}
			if ($calendar_id > 0)
			{
				$button = new button($cal_export, "document.location='export.php?calendar_id=$calendar_id';");
				echo '&nbsp;&nbsp;';
			}
			$button = new button($cmdClose,"javascript:document.location='".$return_to."'");
			?>
		</td>
	</tr>
	</table>

	<?php
	break;
}
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path.'footer.inc');
?>
