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

$time = get_time();
$day = date("j", $time);
$year = date("Y", $time);
$month = date("m", $time);

$date = date($_SESSION['GO_SESSION']['date_format'], $time);

if (isset($_REQUEST['delete_hours']))
{
	$projects->delete_hours($_REQUEST['delete_hours']);
}
?>
<input type="hidden" name="post_action" />
<table border="0" cellpadding="0" cellspacing="0" height="300" width="100%">
<tr>
	<td class="TableInside" valign="top">
	<?php
	if (isset($feedback)) echo $feedback;

	if ($timer = $projects->get_timer($GO_SECURITY->user_id))
	{
		echo 	'<table border="0" class="alert"><tr><td><img src="'.$GO_THEME->images['alert'].'" border="0" /></td>'.
				'<td><a class="normal" href="'.$_SERVER['PHP_SELF'].'?project_id='.$timer['project_id'].'&task=stop_timer">'.$pm_clocked_in.'</a></td></tr></table>';
	}

	?>
	<table border="0" cellpadding="0" cellspacing="5" width="100%">
	<?php
	$pm_user_id = isset($_REQUEST['pm_user_id']) ? $_REQUEST['pm_user_id'] : $GO_SECURITY->user_id;
	if ($GO_MODULES->write_permissions)
	{
		$select = new select('user', 'projects_form', 'pm_user_id', $pm_user_id);
		echo '<tr><td>';
		$select->print_link($pm_employee);
		echo ':</td><td>';
		$select->print_field();
		echo '</td></tr>';
	}else
	{
		echo '<input type="hidden" name="pm_user_id" value="'.$GO_SECURITY->user_id.'" />';
	}
	?>
	<tr>
		<td><?php echo $pm_starttime; ?>:</td>
		<td>
		<?php
		$timer_start_date = isset($timer_start_time) ? date($_SESSION['GO_SESSION']['date_format'], $timer_start_time) : $date;
		$book_start_date = isset($_GET['book_start_date']) ? $_GET['book_start_date'] : $timer_start_date;
		$datepicker->print_date_picker('book_start_date', $_SESSION['GO_SESSION']['date_format'], $book_start_date, '', '', 'onchange="javascript:document.projects_form.book_end_date.value=this.value"');
		echo '&nbsp;';
		$hours = array("00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23", "24");
		//$mins = array("00","15","30","45");
		for ($i=0;$i<=60;$i++)
		{
			$text = strlen($i) < 2 ? '0'.$i : $i;
			$mins[] = $text;
		}
		$hour = isset($timer_start_time) ? date('G', $timer_start_time) : 8;
		$start_hour = isset($_GET['start_hour']) ? $_GET['start_hour'] : $hour;
		$dropbox = new dropbox();
		$dropbox->add_arrays($hours, $hours);
		$dropbox->print_dropbox("start_hour", $start_hour, 'onchange="javascript:update_end_hour(this.value);"');

		echo '&nbsp;:&nbsp;';
		$min = isset($timer_start_time) ? date('i', $timer_start_time) : 0;
		$start_min = isset($_GET['start_min']) ? $_GET['start_min'] : $min;
		$dropbox = new dropbox();
		$dropbox->add_arrays($mins, $mins);
		$dropbox->print_dropbox("start_min", $start_min, 'onchange="javascript:document.projects_form.end_min.value=this.value;"');
		?>
		</td>
		<td colspan="2">
		&nbsp;
		</td>
	</tr>
	<tr><td colspan="3">&nbsp;</td></tr>
	<tr>
		<td colspan="2">
		<?php
		$_COOKIE['registration_method_cookie'] = isset($_COOKIE['registration_method_cookie']) ? $_COOKIE['registration_method_cookie'] : 'endtime';
		$registration_method = isset($_GET['registration_method']) ? $_GET['registration_method'] : $_COOKIE['registration_method_cookie'];

		if ($registration_method == 'units')
		{
			$unit_check = 'checked';
			$endtime_check = '';
		}else
		{
			$unit_check = '';
			$endtime_check = 'checked';
		}
		?>
		<input type="radio" name="registration_method" value="endtime" onclick="javascript:toggle_registration_method(this.value)" <?php echo $endtime_check; ?> /><?php echo $pm_endtime; ?>
		</td>
		<td colspan="2">
		<input type="radio" name="registration_method" value="units" onclick="javascript:toggle_registration_method(this.value)" <?php echo $unit_check; ?> /><?php echo $pm_units; ?>
		</td>
	</tr>

	<tr>
		<td><?php echo $pm_endtime; ?>:</td>
		<td>
		<?php
		$timer_end_date = isset($timer_end_time) ? date($_SESSION['GO_SESSION']['date_format'], $timer_end_time) : $date;
		$book_end_date = isset($_GET['book_end_date']) ? $_GET['book_end_date'] : $timer_end_date;
		$datepicker->print_date_picker('book_end_date', $_SESSION['GO_SESSION']['date_format'], $book_end_date);
		echo '&nbsp;';
		$hour = isset($timer_end_time) ? date('G', $timer_end_time) : 17;
		$end_hour = isset($_GET['end_hour']) ? $_GET['end_hour'] : $hour;
		$dropbox = new dropbox();
		$dropbox->add_arrays($hours, $hours);
		$dropbox->print_dropbox("end_hour", $end_hour);

		echo '&nbsp;:&nbsp;';
		$min = isset($timer_end_time) ? date('i', $timer_end_time) : 0;
		$end_min = isset($_GET['end_min']) ? $_GET['end_min'] : $min;
		$dropbox = new dropbox();
		$dropbox->add_arrays($mins, $mins);
		$dropbox->print_dropbox("end_min", $end_min);
		?>
		</td>
		<td>
		<?php echo $pm_value; ?>:
		</td>
		<td>
		<?php
		$_COOKIE['unit_value_cookie'] = isset($_COOKIE['unit_value_cookie']) ? $_COOKIE['unit_value_cookie'] : '';
		$unit_value = isset($_GET['unit_value']) ? $_GET['unit_value'] : $_COOKIE['unit_value_cookie'];
		$dropbox = new dropbox();
		for ($i=1;$i<=60;$i++)
		{
			$dropbox->add_value($i,$i);
		}
		$dropbox->print_dropbox('unit_value', $unit_value);
		echo '&nbsp;'.$pm_mins;
		?>
		</td>
	</tr>
	<tr>
		<td><?php echo $pm_breaktime; ?>:</td>
		<td>
		<?php
		$break_hours = isset($_GET['break_hours']) ? $_GET['break_hours'] : 0;
		$dropbox = new dropbox();
		$dropbox->add_arrays($hours, $hours);
		$dropbox->print_dropbox("break_hours", $break_hours);

		echo '&nbsp;:&nbsp;';
		$break_mins = isset($_GET['break_mins']) ? $_GET['break_mins'] : 0;
		$dropbox = new dropbox();
		$dropbox->add_arrays($mins, $mins);
		$dropbox->print_dropbox("break_mins", $break_mins);
		?>
		</td>
		<td>
		<?php echo $pm_units; ?>:
		</td>
		<td>
		<?php
		$units = isset($_GET['units']) ? $_GET['units'] : '';
		?>
		<input type="text" name="units" value="<?php echo $units; ?>" class="textbox" size="4" />
		</td>
	</tr>
	<tr><td colspan="3">&nbsp;</td></tr>
	<tr>
		<td valign="top"><?php echo $strComments; ?>:</td>
		<td colspan="3">
		<textarea class="textbox" name="book_comments" cols="40" rows="4"><?php if (isset($_GET['book_comments'])) echo htmlspecialchars($_GET['book_comments']); ?></textarea>
		</td>
	</tr>
	<?php
	echo '<tr><td colspan="4">';
	$button = new button($cmdOk,"javascript:_save('save_hours', 'true')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdApply,"javascript:_save('save_hours', 'false')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	echo '</td></tr>';
	?>
	</table>
	</td>
</tr>
</table>

<script type="text/javascript" language="javascript">
function toggle_registration_method(registration_method)
{
	if (registration_method == "endtime")
	{
		document.projects_form.unit_value.disabled = true;
		document.projects_form.units.disabled = true;
		document.projects_form.end_hour.disabled = false;
		document.projects_form.book_end_date.disabled = false;
		document.projects_form.end_min.disabled = false;
		document.projects_form.break_hours.disabled = false;
		document.projects_form.break_mins.disabled = false;
	}else
	{
		document.projects_form.unit_value.disabled = false;
		document.projects_form.units.disabled = false;
		document.projects_form.book_end_date.disabled = true;
		document.projects_form.end_hour.disabled = true;
		document.projects_form.end_min.disabled = true;
		document.projects_form.break_hours.disabled = true;
		document.projects_form.break_mins.disabled = true;
	}
}
toggle_registration_method("<?php echo $registration_method; ?>");

function update_end_hour(start_hour)
{
	if (start_hour == 24)
	{
		document.projects_form.end_hour.value='01';
	}else
	{
		start_hour = parseInt(start_hour)+1
		if (start_hour < 10)
		{
			start_hour = "0"+start_hour;
		}
		document.projects_form.end_hour.value= start_hour;
	}
}
</script>