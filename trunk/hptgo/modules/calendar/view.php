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

require($GO_MODULES->class_path.'calendar.class.inc');
$cal = new calendar();

$date = isset($_REQUEST['date']) ? $_REQUEST['date'] : getdate();
$year = isset($_POST['year']) ? $_POST['year'] : $date["year"];
$month = isset($_POST['month']) ? $_POST['month'] : $date["mon"];
$day = isset($_POST['day']) ? $_POST['day'] : $date["mday"];

$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

$view_id = isset($_REQUEST['view_id']) ? $_REQUEST['view_id'] : 0;

$hours = array("00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23");

if ($task == 'save')
{
	$name = smart_addslashes(trim($_POST['name']));
	if ($name != "")
	{
		if ($view_id > 0)
		{
			$existing_view = $cal->get_view_by_name($GO_SECURITY->user_id, $name);

			if ($existing_view && $existing_view['id'] != $view_id)
			{
				$feedback = "<p class=\"Error\">".$sc_view_exists."</p>";

			}elseif(!$cal->update_view($view_id, $name, $_POST['view_start_hour'], $_POST['view_end_hour'], $_POST['type']))
			{
				$feedback = "<p class=\"Error\">".$strSaveError."</p>";
			}
		}else
		{
			if ($cal->get_view_by_name($GO_SECURITY->user_id, $name))
			{
				$feedback = "<p class=\"Error\">".$sc_view_exists."</p>";
			}else
			{
				if (!$view_id = $cal->add_view($GO_SECURITY->user_id, $name, $_POST['view_start_hour'], $_POST['view_end_hour'], $_POST['type']))
				{
					$feedback = "<p class=\"Error\">".$strSaveError."</p>";
				}
				else
				{
					$db = new db();
					$db->query('INSERT INTO cal_view_subscriptions VALUES ("'.$GO_SECURITY->user_id.'","'.$view_id.'")');
					if ($_POST['close'] == 'true')
					{
						header('Location: '.$return_to);
						exit();
					}
				}
			}
		}
	}else
	{
		$feedback = "<p class=\"Error\">".$error_missing_field."</p>";
	}

	if (!isset($feedback))
	{
		$calendars = isset($_POST['calendars']) ? $_POST['calendars'] : array();
		$cal->get_authorised_calendars($GO_SECURITY->user_id);
		$cal2 = new calendar();
		while ($cal->next_record())
		{
			if (in_array($cal->f('id'), $calendars))
			{
				if (!$cal2->calendar_is_in_view($cal->f('id'), $view_id))
				{
					$cal2->add_calendar_to_view($cal->f('id'), $view_id);
				}
			}else
			{
				if ($cal2->calendar_is_in_view($cal->f('id'), $view_id))
				{
					$cal2->remove_calendar_from_view($cal->f('id'), $view_id);
				}
			}
		}
		if ($_POST['close'] == 'true')
		{
			header('Location: '.$return_to);
			exit();
		}
	}
}

if ($view_id > 0)
{
	$view = $cal->get_view($view_id);
	$title = $view['name'];
}else
{
	$view['start_hour'] = isset($_POST['view_start_hour']) ? $_POST['view_start_hour'] : '07';
	$view['end_hour'] = isset($_POST['view_end_hour']) ? $_POST['view_end_hour'] : '20';
	$view['name'] = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
	$title = $cal_new_view;
}

$tabtable = new tabtable('view', $title, '100%', '400', '120', '', true);

if ($view_id > 0) {
  $tabtable->add_tab('view', $strProperties);
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
echo '<input type="hidden" name="view_id" value="'.$view_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="type" value="merged" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';

$tabtable->print_head();
switch ($tabtable->get_active_tab_id()) {
 case 'read_permissions':
   $read_only = ($view['user_id'] == $GO_SECURITY->user_id) ? false : true;
   print_acl($view['acl_read'], $read_only);
   echo '<br /><br />';
   echo '&nbsp;&nbsp;&nbsp;&nbsp;';
   $button = new button($cmdClose,"javascript:document.location='".$return_to."'");
   break;

 case 'write_permissions':
   $read_only = ($view['user_id'] == $GO_SECURITY->user_id) ? false : true;
   print_acl($view['acl_write'], $read_only);
   echo '<br /><br />';
   echo '&nbsp;&nbsp;&nbsp;&nbsp;';
   $button = new button($cmdClose,"javascript:document.location='".$return_to."'");
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
	<input type="text" class="textbox" name="name" maxlength="100" size="50" value="<?php echo htmlspecialchars($view['name']); ?>" />
	</td>
</tr>
<tr>
	<td>
	<?php echo $sc_show_hours; ?>:
	</td>
	<td>
	<?php
	$dropbox = new dropbox();
	$dropbox->add_arrays($hours, $hours);
	$dropbox->print_dropbox('view_start_hour', $view['start_hour']);
	?>
	&nbsp;<?php echo $sc_to; ?>&nbsp;
	<?php
	$dropbox = new dropbox();
	$dropbox->add_arrays($hours, $hours);
	$dropbox->print_dropbox('view_end_hour', $view['end_hour']);
	?>
	</td>
</tr>
<tr>
	<td valign><?php echo $sc_calendars; ?>:</td>
	<td>
	<table border="0">
	<tr>
		<td><h3><?php echo $strName; ?></td>
		<td><h3><?php echo $strOwner; ?></td>
	</tr>
	<?php
	
	$cal->get_authorised_calendars($GO_SECURITY->user_id);
	$cal2 = new calendar();
	while($cal->next_record())
	{
		if ($view_id > 0 && $task != 'save')
		{
			$check = $cal2->calendar_is_in_view($cal->f('id'), $view_id);
		}else
		{
			$check = isset($_POST['calendars']) ? in_array($cal->f('id'), $_POST['calendars']) : false;
		}
		echo '<tr><td>';
		$checkbox = new checkbox('calendars[]', $cal->f('id'), $cal->f('name'), $check);
		echo '</td><td>'.show_profile($cal->f('user_id')).'</td></tr>';
	}
	?>
	</table>
	</td>
</tr>
<tr>
	<td colspan="2">
		<?php
		$button = new button($cmdOk,"javascript:document.forms[0].close.value='true';document.forms[0].task.value='save';document.forms[0].submit()");
		echo '&nbsp;&nbsp;';
		$button = new button($cmdApply,"javascript:document.forms[0].task.value='save';document.forms[0].submit()");
		if ($view_id > 0)
		{
			echo '&nbsp;&nbsp;';
			$button = new button($cal_export, "document.location='export.php?view_id=$view_id';");
		}
		echo '&nbsp;&nbsp;';
		$button = new button($cmdClose,"javascript:document.location='$return_to'");
		?>
	</td>
</tr>
</table>

<?php
}
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path.'footer.inc');
?>