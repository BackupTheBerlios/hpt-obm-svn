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
$GO_MODULES->authenticate('notes');
require($GO_LANGUAGE->get_language_file('notes'));

$page_title=$lang_modules['notes'];
require($GO_MODULES->class_path."notes.class.inc");
$notes = new notes();

//check for the addressbook module
$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
  $ab_module = false;
}else
{
  require_once($ab_module['class_path'].'addressbook.class.inc');
  $ab = new addressbook();
}

$projects_module = $GO_MODULES->get_module('projects');
if($projects_module)
{
	require($projects_module['class_path'].'projects.class.inc');
}

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$note_id = isset($_REQUEST['note_id']) ? $_REQUEST['note_id'] : 0;

$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

switch ($task)
{
  case 'save_note':
    $due_date = date_to_unixtime($_POST['due_date']);
    $name = smart_addslashes(trim($_POST['name']));
    if ($note_id > 0)
    {
      if ($name == '')
      {
	$feedback = '<p class="Error">'.$error_missing_field.'</p>';
      }else
      {
	$existing_note = $notes->get_note_by_name($name);
	if($existing_note && $existing_note['id'] != $note_id)
	{
	  $feedback = '<p class="Error">'.$pm_note_exists.'</p>';
	}elseif(!$notes->update_note($_POST['note_id'],
	      $name,
	      $_POST['catagory_id'],
	      $_POST['responsible_user_id'],
	      $due_date,
	      smart_addslashes($_POST['content'])))
	{
	  $feedback = '<p class="Error">'.$strSaveError.'</p>';
	}else
	{
	  if($note = $notes->get_note($_POST['note_id']))
	  {
	    if ($_POST['responsible_user_id'] > 0 && (!$GO_SECURITY->user_in_acl($_POST['responsible_user_id'], $note['acl_read']) && !$GO_SECURITY->user_in_acl($_POST['responsible_user_id'], $note['acl_write'])))
	    {
	      $GO_SECURITY->add_user_to_acl($_POST['responsible_user_id'], $note['acl_read']);
	    }
	  }

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
      }elseif($notes->get_note_by_name($name))
      {
	$feedback = '<p class="Error">'.$pm_note_exists.'</p>';
      }else
      {
	$acl_read = $GO_SECURITY->get_new_acl('note read');
	$acl_write = $GO_SECURITY->get_new_acl('note write');

	if ($acl_read > 0 && $acl_write > 0)
	{
	  if (!$note_id = $notes->add_note($_POST['user_id'],
		$_POST['contact_id'],
		$_POST['project_id'],
		addslashes($_POST['file_path']),
		$_POST['catagory_id'],
		$_POST['responsible_user_id'],
		$due_date,
		$name,
		smart_addslashes($_POST['content']),
		$acl_read, $acl_write))
	  {
	    $GO_SECURITY->delete_acl($acl_read);
	    $GO_SECURITY->delete_acl($acl_write);

	    $feedback = '<p class="Error">'.$strSaveError.'</p>';
	  }else
	  {
	    if ($_POST['contact_id'] > 0)
	    {
	      $addressbook = $ab->get_contact($_POST['contact_id']);

	      $GO_SECURITY->copy_acl($addressbook['acl_read'], $acl_read);
	      $GO_SECURITY->copy_acl($addressbook['acl_write'], $acl_write);

	    }elseif($_POST['project_id'] > 0)
	    {
	      
	      $projects = new projects();
	      $project = $projects->get_project($_POST['project_id']);

	      $GO_SECURITY->copy_acl($project['acl_read'], $acl_read);
	      $GO_SECURITY->copy_acl($project['acl_write'], $acl_write);
	    }elseif($_POST['file_path'] != '')
	    {
	      require_once($GO_CONFIG->class_path.'filesystem.class.inc');
	      $fs = new filesystem();

	      if ($share = $fs->find_share($_POST['file_path']))
	      {
		$GO_SECURITY->copy_acl($share['acl_read'], $acl_read);
		$GO_SECURITY->copy_acl($share['acl_write'], $acl_write);
	      }

	      $GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write);

	    }else
	    {
	      $GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write);
	    }

	    if ($_POST['responsible_user_id'] > 0 && (!$GO_SECURITY->user_in_acl($_POST['responsible_user_id'], $acl_read) && !$GO_SECURITY->user_in_acl($_POST['responsible_user_id'], $acl_write)))
	    {
	      $GO_SECURITY->add_user_to_acl($_POST['responsible_user_id'], $acl_write);
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

if ($note_id > 0)
{
  $note = $notes->get_note($note_id);
  $tabtable = new tabtable('note_tab', $note['name'], '100%', '400', '120', '', true);
  $tabtable->add_tab('properties', $strProperties);
  $tabtable->add_tab('read_permissions', $strReadRights);
  $tabtable->add_tab('write_permissions', $strWriteRights);
}else
{
  $tabtable = new tabtable('note_tab', $no_new_note, '', '400', '120', '', true);
  $note = false;
}


if ($note && $task != 'save_note')
{
  $name = $note['name'];
  $contact_id = $note['contact_id'];
  $project_id = $note['project_id'];
  $user_id = $note['user_id'];
  $file_path = $note['file_path'];
  $content = $note['content'];
  $catagory_id = $note['catagory_id'];
  $due_date = $note['due_date'] > 0 ? date($_SESSION['GO_SESSION']['date_format'], $note['due_date']) : '';
  $responsible_user_id = $note['res_user_id'];
  $ctime = date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $note['ctime']+($_SESSION['GO_SESSION']['timezone']*3600));
  $mtime = date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $note['mtime']+($_SESSION['GO_SESSION']['timezone']*3600));

}else
{
  $name = isset($_REQUEST['name']) ? smartstrip($_REQUEST['name']) : '';
  $catagory_id = isset($_REQUEST['catagory_id']) ? $_REQUEST['catagory_id'] : '0';
  $contact_id = isset($_REQUEST['contact_id']) ? $_REQUEST['contact_id'] : '0';
  $project_id = isset($_REQUEST['project_id']) ? $_REQUEST['project_id'] : '0';
  $file_path = isset($_REQUEST['file_path']) ? smartstrip($_REQUEST['file_path']) : '';
  $content = isset($_REQUEST['content']) ? smartstrip($_REQUEST['content']) : '';
  $user_id = isset($_REQUEST['user_id']) ? $_REQUEST['user_id'] : $GO_SECURITY->user_id;
  $responsible_user_id = isset($_REQUEST['responsible_user_id']) ? $_REQUEST['responsible_user_id'] : $GO_SECURITY->user_id;
  $due_date = isset($_REQUEST['due_date']) ? $_REQUEST['due_date'] : '';
  $ctime = date($_SESSION['GO_SESSION']['date_format'], get_time());
  $mtime = date($_SESSION['GO_SESSION']['date_format'], get_time());
}

if ($note)
{
  $write_permissions = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $note['acl_write']);
  $read_permissions = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $note['acl_read']);
}else
{
  $write_permissions = true;
  $read_permissions = true;
}

if (!$write_permissions && !$read_permissions)
{
  header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
  exit();
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

$page_title = $lang_modules['notes'];
require($GO_THEME->theme_path."header.inc");
echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'" name="notes_form">';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="note_id" value="'.$note_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
echo '<input type="hidden" name="contact_id" value="'.$contact_id.'" />';
echo '<input type="hidden" name="user_id" value="'.$user_id.'" />';
echo '<input type="hidden" name="project_id" value="'.$project_id.'" />';
echo '<input type="hidden" name="file_path" value="'.stripslashes($file_path).'" />';

$tabtable->print_head();

switch ($tabtable->get_active_tab_id())
{
  case 'read_permissions':
    print_acl($note['acl_read']);
    echo '<br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;

  case 'write_permissions':
    print_acl($note['acl_write']);
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
      <td><?php echo $strName; ?>:</td>
      <td>
      <?php
      if ($write_permissions)
      {
	echo '<input type="text" class="textbox" style="width: 250px;" name="name" value="'.htmlspecialchars($name).'" maxlength="50" />';
      }else
      {
	echo htmlspecialchars($note['name']);
      }
      ?>
	</td>
	<tr>
	<?php
	if ($notes->get_catagories() > 0)
	{
	  echo '<tr><td>'.$no_catagory.':</td><td>';
	  $dropbox = new dropbox();
	  $dropbox->add_value('', $no_none);
	  while($notes->next_record())
	  {
	    $dropbox->add_value($notes->f('id'), $notes->f('name'));
	  }

	  $dropbox->print_dropbox('catagory_id', $catagory_id);
	  echo '</td><tr>';

	}else
	{
	  echo '<input type="hidden" name="catagory_id" value="0" />';
	}
	echo '<tr><td>'.$no_due_date.':</td><td>';
	$datepicker->print_date_picker('due_date', $_SESSION['GO_SESSION']['date_format'], $due_date);
	echo '</td></tr>';

	$select = new select('user', 'notes_form', 'responsible_user_id', $responsible_user_id);
	echo '<tr><td>';
	$select->print_link($no_responsible);
	echo ':</td><td>';
	$select->print_field();
	echo '</td></tr>';
	?>
	  </table>
	  </td>
	  <td valign="top">
	  <table border="0" cellspacing="0" cellpadding="4">
	  <?php
	  echo '<tr><td>'.$strOwner.':</td><td>'.show_profile($user_id, '', 'normal', $link_back).'</td></tr>';
	echo '<tr><td>'.$strCreatedAt.':</td><td>'.$ctime.'</td><tr>';
	echo '<tr><td>'.$strModifiedAt.':</td><td>'.$mtime.'</td><tr>';
	if ($project_id > 0)
	{
	  $projects = new projects();

	  if($project = $projects->get_project($project_id))
	  {
	    $project_name = $project['description'] == '' ? $project['name'] : $project['name'].' ('.$project['description'].')';
		

		if ($projects_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $projects_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $projects_module['acl_write'])))
		{
		echo '<tr><td>'.$no_project.':</td><td><a href="'.$projects_module['url'].'project.php?project_id='.$project_id.'&return_to='.urlencode($_SERVER['REQUEST_URI']).'" class="normal">'.htmlspecialchars($project_name).'</a></td><tr>';
		}else
		{
		echo '<tr><td>'.$no_project.':</td><td>'.htmlspecialchars($project_name).'</td><tr>';
		}
		}
		}elseif($contact_id > 0)
		{
		echo '<tr><td>'.$no_contact.':</td><td>'.show_contact($contact_id, '', $link_back).'</td><tr>';
		}elseif($file_path != '' && $fs_module = $GO_MODULES->get_module('filesystem'))
		{
		if (file_exists($file_path))
		{
		echo 	'<tr><td>'.$no_file.':</td><td><a class="normal" href="'.
		$fs_module['url'].'index.php?path='.urlencode($file_path).
		'">'.stripslashes($file_path).'</a></td><tr>';
		}
		}
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
		  if ($write_permissions)
		  {
		    $button = new button($cmdOk, "javascript:_save('save_note', 'true');");
		    echo '&nbsp;&nbsp;';
		    $button = new button($cmdApply, "javascript:_save('save_note', 'false')");
		    echo '&nbsp;&nbsp;';
		  }
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
  document.notes_form.task.value = task;
  document.notes_form.close.value = close;
  <?php
    if (isset($htmlarea) && $htmlarea->browser_is_supported())
    {
      echo 'document.notes_form.onsubmit();';
    }
  ?>
    document.notes_form.submit();
}

function remove_user()
{
  document.notes_form.responsible_user_id.value = 0;
  document.notes_form.user_name.value = '';
  document.notes_form.user_name_text.value = '';
}

</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
