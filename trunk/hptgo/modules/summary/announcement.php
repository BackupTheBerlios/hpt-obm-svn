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
$GO_MODULES->authenticate('summary', true);
require($GO_LANGUAGE->get_language_file('summary'));

require($GO_MODULES->class_path."announcements.class.inc");
$announcements = new announcements();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$announcement_id = isset($_REQUEST['announcement_id']) ? $_REQUEST['announcement_id'] : 0;

$return_to = 'announcements.php';
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

switch ($task)
{
	case 'save_announcement':
		$due_time = date_to_unixtime($_POST['due_time']);
		$title = smart_addslashes(trim($_POST['title']));
		if ($title == '')
		{
			$feedback = '<p class="Error">'.$error_missing_field.'</p>';
		}else
		{
			if ($announcement_id > 0)
			{
				if(!$announcements->update_announcement($_POST['announcement_id'],
																			$title,																			
																			smart_addslashes($_POST['content']),
																			$due_time))
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
				$acl_id = $GO_SECURITY->get_new_acl('announcement');
				if ($acl_id > 0)
				{
					if (!$announcement_id = $announcements->add_announcement($_POST['user_id'],
																					$due_time,
																					$title,
																					smart_addslashes($_POST['content']),
																					$acl_id))
					{
						$GO_SECURITY->delete_acl($acl_id);

						$feedback = '<p class="Error">'.$strSaveError.'</p>';
					}else
					{						
						if (!isset($_POST['private']))
						{
							$GO_SECURITY->add_group_to_acl($GO_CONFIG->group_everyone, $acl_id);
						}
											
						if ($_POST['close'] == 'true')
						{
							header('Location: '.$return_to);
							exit();
						}
					}
				}else
				{
					$feedback = '<p class="Error">'.$strSaveError.'</p>';
				}
			}
		}
	break;
}

if ($announcement_id > 0)
{
	$announcement = $announcements->get_announcement($announcement_id);
	$tabtable = new tabtable('announcement_tab', $announcement['title'], '100%', '400', '120', '', true);
	$tabtable->add_tab('properties', $strProperties);
	$tabtable->add_tab('read_permissions', $strReadRights);
}else
{
	$tabtable = new tabtable('announcement_tab', $sum_new_announcement, '', '400', '120', '', true);
	$announcement = false;
}


if ($announcement && $task != 'save_announcement')
{
	$title = $announcement['title'];
	$user_id = $announcement['user_id'];
	$content = $announcement['content'];
	$due_time = $announcement['due_time'] > 0 ? date($_SESSION['GO_SESSION']['date_format'], $announcement['due_time']) : '';
	$ctime = date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $announcement['ctime']+($_SESSION['GO_SESSION']['timezone']*3600));
	$mtime = date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $announcement['mtime']+($_SESSION['GO_SESSION']['timezone']*3600));

}else
{
	$title = isset($_REQUEST['title']) ? smartstrip($_REQUEST['title']) : '';
	$content = isset($_REQUEST['content']) ? smartstrip($_REQUEST['content']) : '';
	$user_id = isset($_REQUEST['user_id']) ? $_REQUEST['user_id'] : $GO_SECURITY->user_id;
	$due_time = isset($_REQUEST['due_time']) ? $_REQUEST['due_time'] : '';
	$ctime = date($_SESSION['GO_SESSION']['date_format'], get_time());
	$mtime = date($_SESSION['GO_SESSION']['date_format'], get_time());
}

//create htmlarea


if ($tabtable->get_active_tab_id() != 'read_permissions' && $tabtable->get_active_tab_id() != 'write_permissions')
{
	$htmlarea = new htmlarea();
	$GO_HEADER['head'] = $htmlarea->get_header('content', -70, -240, 25);
	$datepicker = new date_picker();
	$GO_HEADER['head'] .= $datepicker->get_header();
	$GO_HEADER['body_arguments'] = 'onload="initEditor()"';
}

require($GO_THEME->theme_path."header.inc");
echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'" name="announcements_form">';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="announcement_id" value="'.$announcement_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="user_id" value="'.$user_id.'" />';

$tabtable->print_head();

switch ($tabtable->get_active_tab_id())
{
	case 'read_permissions':
		print_acl($announcement['acl_id']);
		echo '<br />';
		$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
	break;

	default:
		if (isset($feedback)) echo $feedback;
		?>
		<table border="0" cellspacing="0" cellpadding="4">
		<tr>
			<td valign="top">
			<table border="0" cellspacing="0" cellpadding="4">
			<tr>
				<td><?php echo $strTitle; ?>:</td>
				<td>
				<?php
				echo '<input type="text" class="textbox" style="width: 250px;" name="title" value="'.htmlspecialchars($title).'" maxlength="50" />';
				?>
				</td>
			<tr>
			<?php

			echo '<tr><td>'.$sum_due_time.':</td><td>';
			$datepicker->print_date_picker('due_time', $_SESSION['GO_SESSION']['date_format'], $due_time);
			echo '</td></tr>';
			if ($announcement_id == 0)
			{
				echo '<tr><td colspan="2">';
				$checkbox = new checkbox('private', 'true', $sum_private, isset($_POST['private']));
				echo '</td></tr>';
			}
			?>
			</table>
			</td>
			<td valign="top">
			<table border="0" cellspacing="0" cellpadding="4">
			<?php
			echo '<tr><td>'.$strOwner.':</td><td>'.show_profile($user_id, '', 'normal', $link_back).'</td></tr>';
			echo '<tr><td>'.$strCreatedAt.':</td><td>'.$ctime.'</td><tr>';
			echo '<tr><td>'.$strModifiedAt.':</td><td>'.$mtime.'</td><tr>';
			?>
			</table>
			</td>
		</tr>
		<tr>
			<td colspan="2">
			<?php
			$htmlarea->print_htmlarea(htmlspecialchars($content));
			?>
			</td>
		</tr>
		<tr>
			<td colspan="2">
			<?php
			$button = new button($cmdOk, "javascript:_save('save_announcement', 'true');");
			echo '&nbsp;&nbsp;';
			$button = new button($cmdApply, "javascript:_save('save_announcement', 'false')");
			echo '&nbsp;&nbsp;';
			$button = new button($cmdClose, "javascript:document.location='".$return_to."';");
			?>
			</td>
		</tr>
		</table>

		<?php
	break;
}

$tabtable->print_foot();
echo '</form>';
?>
<script type="text/javascript">

function _save(task, close)
{
	document.announcements_form.task.value = task;
	document.announcements_form.close.value = close;
	<?php
	if (isset($htmlarea) && $htmlarea->browser_is_supported())
	{
		echo 'document.announcements_form.onsubmit();';
	}
	?>
	document.announcements_form.submit();
}

function remove_user()
{
	document.announcements_form.responsible_user_id.value = 0;
	document.announcements_form.user_title.value = '';
	document.announcements_form.user_title_text.value = '';
}

</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
