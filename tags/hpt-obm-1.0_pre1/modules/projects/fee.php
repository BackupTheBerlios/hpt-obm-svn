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
$GO_MODULES->authenticate('projects');
require($GO_LANGUAGE->get_language_file('projects'));

require($GO_MODULES->class_path."projects.class.inc");
$projects = new projects();

$fee_id = isset($_REQUEST['fee_id']) ? $_REQUEST['fee_id'] : 0;

if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
	if($GO_MODULES->write_permissions)
	{
		$name = smart_addslashes(trim($_POST['name']));
		$value = trim(str_replace(',','.',smart_addslashes($_POST['value'])));

		if ($name == '' || $value == '')
		{
			$feedback = '<p class="Error">'.$error_missing_field.'</p>';
		}else
		{
			if (isset($_POST['fee_id']))
			{
				if ($projects->update_fee($_POST['fee_id'], $name, $value, smart_addslashes($_POST['time'])))
				{
					header('Location: '.$GO_MODULES->url.'index.php?post_action=fees');
					exit();
				}else
				{
					$feedback = '<p class="Error">'.$strSaveError.'</p>';
				}
			}else
			{
				if($projects->add_fee($name, $value, $_POST['time']))
				{
					header('Location: '.$GO_MODULES->url.'index.php?post_action=fees');
					exit();
				}else
				{
					$feedback = '<p class="Error">'.$strSaveError.'</p>';
				}
			}
		}
	}else
	{
		$title = $strAccessDenied;
		$require = $GO_CONFIG->root_path.'error_docs/403.inc';
	}
}
$page_title = $lang_modules['projects'];
require($GO_THEME->theme_path."header.inc");

$tabtable = new tabtable('fee_tab', $pm_fees, '600', '400', '100', '', true);
$tabtable->print_head();
?>
<form method="post" name="add_fee" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="post_action" value="<?php echo $task; ?>" />
<?php
if ($fee_id > 0)
{
	echo '<input type="hidden" name="fee_id" value="'.$fee_id.'" />';
	$fee = $projects->get_fee($fee_id);
	$name = $fee["name"];
	$value = $fee["value"];
	$time = $fee["time"];
}else
{
	$name = isset($_POST['name']) ? $_POST['name'] : '';
	$time = isset($_POST['time']) ? $_POST['time'] : '60';
	$value = isset($_POST['value']) ? $_POST['value'] : '';
}

if (isset($feedback)) echo $feedback;
?>
<table border="0" cellpadding="0" cellspacing="3">

<tr>
	<td><?php echo $strName; ?>:</td>
	<td><input type="text" name="name" value="<?php echo htmlspecialchars($name); ?>" maxlength="50" size="40" class="textbox" /></td>
</tr>
<tr>
	<td valign="top"><?php echo $pm_value; ?>:</td>
	<td><?php echo htmlspecialchars($_SESSION['GO_SESSION']['currency']); ?>&nbsp;<input type="text" name="value" value="<?php echo htmlspecialchars($value); ?>" maxlength="10" size="6" class="textbox" />
	&nbsp;/&nbsp;
	<?php
	$dropbox = new dropbox();
	for ($i=1;$i<=60;$i++)
	{
		$dropbox->add_value($i,$i);
	}
	$dropbox->print_dropbox('time', $time);
	echo '&nbsp;'.$pm_mins;
	?>
	</td>
</tr>
<tr>
	<td colspan="2"><br />
	<?php
	$button = new button($cmdOk, "javascript:document.forms[0].submit()");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdCancel, "javascript:document.location='index.php?post_action=fees';");
	?>
	</td>
</tr>

</table>
</form>
<?php
$tabtable->print_foot();
require($GO_THEME->theme_path."footer.inc");
?>