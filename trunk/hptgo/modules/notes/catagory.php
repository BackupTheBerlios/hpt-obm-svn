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


$GO_SECURITY->authenticate(true);
$GO_MODULES->authenticate('notes');
require($GO_LANGUAGE->get_language_file('notes'));

$page_title=$lang_modules['notes'];
require($GO_MODULES->class_path."notes.class.inc");
$notes = new notes();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$catagory_id = isset($_REQUEST['catagory_id']) ? $_REQUEST['catagory_id'] : 0;

$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

switch ($task)
{
	case 'save_catagory':
		$name = smart_addslashes(trim($_POST['name']));
		if ($catagory_id > 0)
		{
			if ($name == '')
			{
				$feedback = '<p class="Error">'.$error_missing_field.'</p>';
			}else
			{
				$existing_catagory = $notes->get_catagory_by_name($name);

				if($existing_catagory && $existing_catagory['id'] != $catagory_id)
				{
					$feedback = '<p class="Error">'.$pm_catagory_exists.'</p>';
				}elseif(!$notes->update_catagory($catagory_id, $name))
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
		}else
		{
			if ($name == '')
			{
				$feedback = '<p class="Error">'.$error_missing_field.'</p>';
			}elseif($notes->get_catagory_by_name($name))
			{
				$feedback = '<p class="Error">'.$pm_catagory_exists.'</p>';
			}else
			{
				if (!$catagory_id = $notes->add_catagory($name))
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
	break;
}

if ($catagory_id > 0)
{
	$catagory = $notes->get_catagory($catagory_id);
	$tabtable = new tabtable('catagory_tab', $catagory['name'], '400', '100', '120', '', true);
}else
{
	$tabtable = new tabtable('catagory_tab', $no_new_catagory, '400', '100', '120', '', true);
	$catagory = false;
}

if ($catagory && $task != 'save_catagory')
{
	$name = $catagory['name'];
}else
{
	$name = isset($_REQUEST['name']) ? smartstrip($_REQUEST['name']) : '';
}

$page_title = $lang_modules['notes'];
require($GO_THEME->theme_path."header.inc");

echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'" name="catagories_form">';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="catagory_id" value="'.$catagory_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';

$tabtable->print_head();

if (isset($feedback)) echo $feedback;
?>
<table border="0" cellspacing="0" cellpadding="4">
<tr>
	<td><?php echo $strName; ?>:</td>
	<td>
	<?php
	echo '<input type="text" class="textbox" style="width: 250px;" name="name" value="'.htmlspecialchars($name).'" maxlength="50" />';
	?>
	</td>
<tr>
<tr>
	<td colspan="2">
	<?php
	$button = new button($cmdOk, "javascript:_save('save_catagory', 'true');");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	?>
	</td>
</tr>
</table>
<?php
$tabtable->print_foot();
echo '</form>';
?>
<script type="text/javascript">

function _save(task, close)
{
	document.catagories_form.task.value = task;
	document.catagories_form.close.value = close;
	document.catagories_form.submit();
}
document.catagories_form.name.focus();
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>