<?php
/*
Copyright Georg Lorenz
Author: Georg Lorenz <georg@lonux.de>
Version: 1.0 Release date: 29 January 2004

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

require($GO_MODULES->path.'classes/cal_holidays.class.inc');
$holidays = new holidays();

$regions = $GO_LANGUAGE->get_languages();
$holiday_id = isset($_REQUEST['holiday_id']) ? $_REQUEST['holiday_id'] : 0;
$region = isset($_REQUEST['region']) ? $_REQUEST['region'] : '';

$min_year = 1970;
$max_year = 2037;
$date_format = $_SESSION['GO_SESSION']['date_format'];
$current_date = getdate();
$current_year = $current_date["year"];

for($i=-2;$i<3;$i++)
{
	if($i==0)
		$years[] = $year;
	elseif(($year + $i) >= $min_year && ($year + $i) <= $max_year)
		$years[] = $year + $i;
}

if(!in_array($current_year, $years))
	array_unshift($years, $current_year);

$calendar_id = (isset($calendar_id) && $calendar_id > 0) ? $calendar_id : $cal->get_default_calendar($GO_SECURITY->user_id);

switch ($task)
{
	case 'delete_holiday':
		$holidays->delete_holiday($holiday_id);
		$task = "";
	break;


	case 'delete_holidays':
		$holidays->delete_holidays($GO_SECURITY->user_id, $calendar_id, $year, $region);
		$task = "";
	break;

	case 'apply_holidays':
		$holidays->add_holidays($GO_SECURITY->user_id, $calendar_id, $year, $region);
		$task = "";
	break;
}

if(count($regions) == 0)
{
	echo '<tr><td>'.$sc_holidays_no_file_prefix.$GO_LANGUAGE->language.$sc_holidays_no_file_suffix.'</td></tr>';
	echo '<tr><td>';
	$button = new button($cmdBack, "javascript:document.location='".$return_to."'");
}else
{
	switch ($task)
	{
		case 'save_holiday':
			$name = smart_addslashes(trim($_POST['name']));
			if($name != '')
			{
				$date = date_to_unixtime($_POST['date']);
				if($_POST['holiday_id'] > 0)
					$holidays->update_holiday($_POST['holiday_id'] , $date, $name);
				else
					$holidays->add_holiday($GO_SECURITY->user_id, $calendar_id, $_POST['region'], $date, $name);
				$task = "";
			}else
			{
				$feedback = '<p class="Error">'.$error_missing_field.'</p>';
				$task = "edit_holiday";
			}
		break;

		case 'edit_holiday':
			?>
			<input type="hidden" name="holiday_id" value="<?php echo $holiday_id; ?>" />
			<input type="hidden" name="year" value="<?php echo $year; ?>" />
			<input type="hidden" name="region" value="<?php echo $region; ?>" />
			<?php
			if ($holiday_id > 0)
			{
				if($holidays->get_holiday_by_id($holiday_id))
				{
					$date = date($date_format, $holidays->f("date"));
					$name = $holidays->f("name");
				}
			// if $date contains array values obtained by function getdate() -> not compatible with date_picker
			// then redefine the $date variable to be compatible with date_picker control
			}elseif(is_array($date))
			{
				$date = date($date_format);
			}

			if (isset($feedback)) echo $feedback;
			?>
			<table border="0">
			<tr>
				<td>
				<table border="0">
				<tr>
					<?php
					echo '<td>'.$strDate.':</td><td>';
					$datepicker->print_date_picker('date', $date_format, $date);
					echo '</td>';
					?>
				</tr>
				<tr>
					<td><?php echo $strName; ?>:</td>
					<td><input type="text" class="textbox" name="name" value="<?php if (isset($name)) echo htmlspecialchars($name); ?>" style="width: 300px;" /></td>
				</tr>
				<tr>
					<td colspan="2">
					<?php
					$button = new button($cmdOk, 'javascript:save_holiday();');
					echo '&nbsp;&nbsp;';
					$button = new button($cmdCancel, 'javascript:cancel_holidays();');
					?>
					</td>
				</tr>
				</table>
				</td>
			</tr>
			</table>
			<?php
		break;

		case 'generate_holidays':
			$count = $holidays->get_holidays($GO_SECURITY->user_id, $calendar_id, $year);
			$holidays->generate_holidays($region, $year);
			?>
			<input type="hidden" name="year" value="<?php echo $year ?>" />
			<input type="hidden" name="region" value="<?php echo $region ?>" />
			<br /><br /><table border="0">
			<tr>
				<td>
				<?php
				if (isset($feedback)) echo $feedback;
				if ($has_write_permission) {
					echo $sc_holidays_text2;
					echo '<br /><br />';
				}

				if(is_array($holidays->holidays))
				{
					echo '<table border="0"><tr><td><h3>'.$strDate.'</h3></td><td><h3>'.$strName.'</h3></td><td></td><td></td></tr>';
					foreach($holidays->holidays as $index => $value)
					{
						echo '<tr><td width="40%">'.date($date_format, $index).'</td><td>'.$value.'</td></tr>';
					}
					echo '</table><br />';
					echo '&nbsp;&nbsp;';
					if ($has_write_permission) {
						$button = new button($cmdApply, 'javascript:apply_holidays('.$count.');');
						echo '&nbsp;&nbsp;';
					}
					$button = new button($cmdCancel, 'javascript:cancel_holidays();');
				}
				?>
				</td>
			</tr>
			</table>
			<?php
		break;
	}
?>
			<input type="hidden" name="year" />
			<input type="hidden" name="holiday_id" />
			<input type="hidden" name="region" value="<?php echo $region ?>"/>
			<?php if ($has_write_permission) { ?>
			<a class="normal" href="javascript:edit_holiday(0);"><?php echo $cmdAdd; ?></a>
			<br /><br />
			<?php } ?>
			<table border="0" cellpadding="4">
			<tr>
				<td>
				<?php
				$holidays->get_holidays($GO_SECURITY->user_id, $calendar_id, $year);

				if ($holidays->num_rows() > 0)
				{
					$output_lines = '<br /><table border="0"><tr><td><h3>'.$strDate.'</h3></td><td><h3>'.$strName.'</h3></td><td></td><td></td></tr>';
					while($holidays->next_record())
					{
						$output_lines .= '<tr><td width="30%">'.date($date_format, $holidays->f('date')).'</td><td width="60%">'.$holidays->f('name').'</td>';
						$output_lines .= '<td><a href=\'javascript:edit_holiday('.$holidays->f('id').')\' title="'.$strEdit.' \''.htmlspecialchars($holidays->f('name')).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
						$output_lines .= '<td><a href=\'javascript:delete_holiday('.$holidays->f('id').', "'.$holidays->f('name').'")\' title="'.$strDeleteItem.' \''.htmlspecialchars($holidays->f('name')).'\'"><img src="'.$GO_THEME->images['delete'].'" border="0" /></a></td></tr>';
					}
				}else
				{
					$output_lines = '<br /><br /><table border="0"><tr><td>'.$strNoHolidays.'</td></tr></table><br /><table border="0">';
				}

				if (isset($feedback)) echo $feedback;
				echo $sc_holidays_text1;
				echo '<br /><br /><table border="0">';
				echo '<tr><td>'.$strLocalization.':</td><td>';
				$dropbox = new dropbox();
				$languages= $GO_LANGUAGE->get_languages();
				while($language = array_shift($languages))
				{
					if(array_key_exists($language['code'], $holidays->in_holidays["fix"]) || 
							array_key_exists($language['code'], $holidays->in_holidays["var"]) ||
							array_key_exists($language['code'], $holidays->in_holidays["spc"]))
					{
						$dropbox->add_arrays($language['code'], $language['description']);
					}
				}				
			
				$dropbox->print_dropbox('region', $_SESSION['GO_SESSION']['language']['code']);
				echo '</td></tr>';
				echo '<tr><td>'.$strYear.':</td><td>';
				$dropbox = new dropbox();
				$dropbox->add_arrays($years, $years);
				$dropbox->print_dropbox('year',$year,'onchange="javascript:document.forms[0].submit()"');
				echo '</td></tr></table>';
				echo $output_lines;
				echo '</table><br />';

				$button = new button($cmdGenerate, 'javascript:generate_holidays('.$year.');');
				if($holidays->num_rows() > 0)
				{
					echo '&nbsp;&nbsp;';
					$button = new button($cmdDelete, 'javascript:delete_holidays('.$year.');');
				}
				echo '&nbsp;&nbsp;';
				$button = new button($cmdClose,"javascript:document.location='".$return_to."'");
				?>
				</td>
			</tr>
			</table>
<?php
}
?>


<script type="text/javascript">
function delete_holiday(holiday_id, holiday_name)
{
	if (confirm("<?php echo $strDeletePrefix; ?>'"+holiday_name+"'<?php echo $strDeleteSuffix; ?>"))
	{
		document.forms[0].task.value='delete_holiday';
		document.forms[0].holiday_id.value=holiday_id;
		document.forms[0].submit();
	}
}

function delete_holidays(year)
{
	if (confirm("<?php echo $strDeleteHolidaysPrefix; ?>"+year+"<?php echo $strDeleteHolidaysSuffix; ?>"))
	{
		document.forms[0].task.value='delete_holidays';
		document.forms[0].year.value=year;
		document.forms[0].submit();
	}
}

function edit_holiday(id)
{
	document.forms[0].task.value='edit_holiday';
	document.forms[0].holiday_id.value=id;
	document.forms[0].submit();
}

function cancel_holidays()
{
	document.forms[0].task.value='';
	document.forms[0].submit();
}

function save_holiday()
{
	document.forms[0].task.value='save_holiday';
	document.forms[0].submit();
}

function generate_holidays(year)
{
	document.forms[0].task.value='generate_holidays';
	document.forms[0].year.value=year;
	document.forms[0].submit();
}

function apply_holidays(holidays_count)
{
	var apply=true;
	if (holidays_count > 0)
	{
		if (!confirm("<?php echo $strReplaceHolidays; ?>"))
		{
			apply=false;
		}
	}
	if (apply)
	{
		document.forms[0].task.value='apply_holidays';
		document.forms[0].submit();
	}
}
</script>
