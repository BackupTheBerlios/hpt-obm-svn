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

//authenticate the user
$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('cms');

require($GO_MODULES->class_path.'cms.class.inc');
$cms = new cms();

//get the language file
require($GO_LANGUAGE->get_language_file('cms'));

$site_id = isset($_REQUEST['site_id']) ? $_REQUEST['site_id'] : 0;

if(!$site = $cms->get_site($site_id))
{
  header('Location: index.php');
}

if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $site['acl_write']))
{
  require($GO_THEME->theme_path."header.inc");
  require($GO_CONFIG->root_path.'error_docs/403.inc');
  require($GO_THEME->theme_path."footer.inc");
  exit();

}

//set the folder id we are in
$folder_id = isset($_REQUEST['folder_id']) ? $_REQUEST['folder_id'] : $site['root_folder_id'];

//set the folder id we are in
$file_id = isset($_REQUEST['file_id']) ? $_REQUEST['file_id'] : 0;

//what to do before output
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERRER'];

$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

switch ($task)
{
  case 'save_file_properties':
    $task = 'file_properties';
    $name = smart_addslashes(trim($_POST['name']));
    if ($name == '')
    {
      $feedback = '<p class="Error">'.$error_missing_field.'</p>';
    }else
    {
      if ($_POST['extension'] != '')
      {
	$name = $name.'.'.$_POST['extension'];
      }
      $existing_id = $cms->file_exists($folder_id, $name);
      if($existing_id && ($_POST['file_id'] != $existing_id))
      {
	$feedback = '<p class="Error">'.$fbNameExists.'</p>';
      }elseif(!$file=$cms->get_file($_POST['file_id']))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
	if (!$cms->update_file($_POST['file_id'],
	      $name,
	      addslashes($file['content']),
	      smart_addslashes($_POST['title']),
	      smart_addslashes($_POST['description']),
	      smart_addslashes($_POST['keywords']),
	      $_POST['priority']))
	{
	  $feedback = '<p class="Error">'.$strSaveError.'</p>';
	}else
	{
	  if($_POST['close'] == 'true')
	  {
	    header('Location: '.$return_to);
	    exit();
	  }
	}
      }
    }
    break;

  case 'save_folder_properties':
    $task = 'folder_properties';
    $name = smart_addslashes(trim($_POST['name']));
    if ($name == '')
    {
      $feedback = '<p class="Error">'.$error_missing_field.'</p>';
    }elseif(!$folder=$cms->get_folder($folder_id))
    {
      $feedback = '<p class="Error">'.$strSaveError.'</p>';
    }else
    {
      $disabled = isset($_POST['disabled']) ? '1' : '0';
      if (!$cms->update_folder($folder_id, $name, $disabled, $_POST['priority'], $_POST['display_type']))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
	if($_POST['close'] == 'true')
	{
	  header('Location: '.$return_to);
	  exit();
	}
      }
    }
    break;

  case 'save_search_words':
    $cms2 = new cms();

    $selected_search_words =  isset($_POST['selected_search_words']) ? $_POST['selected_search_words'] : array();
    $cms->get_search_words($site_id);

    while ($cms->next_record())
    {
      $attached = $cms2->file_is_attached($_POST['file_id'], $cms->f('id'));
      $selected = in_array($cms->f('id'), $selected_search_words);

      if ($selected && !$attached)
      {
	$cms2->attach_file($_POST['file_id'], $cms->f('id'));
      }

      if ($attached && !$selected)
      {
	$cms2->detach_file($_POST['file_id'],$cms->f('id'));
      }
    }
    $task = 'file_properties';
    if($_POST['close'] == 'true')
    {
      header('Location: '.$return_to);
      exit();
    }

    break;

  case 'search_file':
    $task = 'file_properties';
    $search_file = true;
    break;
}

//set the page title for the header file
$page_title = $lang_modules['cms'];

//require the header file. This will draw the logo's and the menu
require($GO_THEME->theme_path."header.inc");
echo '<form name="cms" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="site_id" value="'.$site_id.'" />';
echo '<input type="hidden" name="folder_id" value="'.$folder_id.'" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
echo '<input type="hidden" name="close" value="false" />';

$tabtable = new tabtable('properties',$fbProperties, '600', '400', '100','', true);
if ($task == 'file_properties')
{
  echo '<input type="hidden" name="task" value="file_properties" />';
  echo '<input type="hidden" name="file_id" value="'.$file_id.'" />';
  $write_perms = true;
  $item = $cms->get_file($file_id);
  $item['parent_id'] = $item['folder_id'];
  $item['size'] = format_size($item['size']);
  $tabtable->add_tab($fbProperties, 'properties');
  if (strtolower($item['content_type']) == 'text/html')
  {
    $tabtable->add_tab('meta', 'meta');
  }
  $tabtable->add_tab('search_words', $cms_search_words);

}else
{
  echo '<input type="hidden" name="task" value="folder_properties" />';
  $item = $cms->get_folder($folder_id);
  $item['content_type'] = 'folder';
  $item['size'] = '-';
  if ($item['parent_id'] == 0)
  {
    $no_hide = true;
  }else
  {
    $no_hide = false;
  }
}

if(isset($_REQUEST['set_active_tab']))
{
  $tabtable->set_active_tab($_REQUEST['set_active_tab']);
}

$tabtable->print_head();
switch($tabtable->get_active_tab_id())
{
  case 'meta':
    echo '<input type="hidden" name="name" value="'.strip_extension($item['name']).'" />';
    echo '<input type="hidden" name="extension" value="'.get_extension($item['name']).'" />';
    echo '<input type="hidden" name="priority" value="'.$item['priority'].'" />';
    ?>
      <table border="0" cellpadding="4" cellspacing="0">
      <?php
      if(isset($feedback)) echo '<tr><td colspan="2">'.$feedback.'&nbsp;</td></tr>';
    ?>
      <tr>
      <td>
      <?php echo $strTitle; ?>:
      </td>
      <td>
      <?php
      $title = ($item['title'] != '') ? $item['title'] : $site['name'];
    ?>
      <input type="text" class="textbox" name="title" value="<?php echo htmlspecialchars($title); ?>" maxlength="100" style="width: 250" />
      </td>
      </tr>
      <tr>
      <td valign="top">
      <?php echo $strDescription; ?>:
      </td>
      <td>
      <?php
      $description = ($item['description'] != '') ? $item['description'] : $site['description'];
    ?>
      <textarea class="textbox" name="description" style="width: 250" rows="5"><?php echo htmlspecialchars($description); ?></textarea>
      </td>
      </tr>
      <tr>
      <td valign="top">
      <?php echo $cms_keywords; ?>:
      </td>
      <td>
      <?php
      $keywords = ($item['keywords'] != '') ? $item['keywords'] : $site['keywords'];
    ?>
      <textarea class="textbox" name="keywords" style="width: 250" rows="5"><?php echo htmlspecialchars($keywords); ?></textarea>
      </td>
      </tr>

      <tr>
      <td colspan="2">
      <br />
      <?php
      $button = new button($cmdOk, "javascript:_save('save_".$task."', 'true');");
    echo '&nbsp;&nbsp;';
    $button = new button($cmdApply, "javascript:_save('save_".$task."', 'false');");
    echo '&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='$return_to';");
    ?>
      </td>
      </tr>

      </table>
      <?php

      break;

  case 'search_words':
    echo '<table border="0" cellspacing="8">';
    echo '<tr><td><a class="normal" href="site.php?task=edit_search_words&site_id='.$site_id.'&return_to='.urlencode($link_back).'">'.$cms_edit_search_words.'</a></td></tr>';

    echo '<tr><td><table border="0">';
    $cms2 = new cms();
    if (isset($search_file))
    {
      $cms->get_search_words($site_id);
    }else
    {
      $cms->get_attached_search_words($file_id);
    }
    while ($cms->next_record())
    {
      if(!isset($search_file) || preg_match("/\b(?<!\/)".$cms->f('search_word')."\b/i", $item['content']) || preg_match("/\b(?<!\/)".htmlentities($cms->f('search_word'))."\b/i", $item['content']))
      {
	echo '<tr><td><input type="checkbox" name="selected_search_words[]" value="'.$cms->f('id').'" checked />';
	echo '<td>'.$cms->f('search_word').'</td></tr>';
      }
    }
    echo '</td></tr></table>';
    echo '<tr><td nowrap>';
    $button = new button($cmdOk, "javascript:_save('save_search_words', 'true');");
    echo '&nbsp;&nbsp;';
    $button = new button($cmdApply, "javascript:_save('save_search_words', 'false');");
    echo '&nbsp;&nbsp;';
    $button = new button($cms_search_files, "javascript:_save('search_file', 'false');");
    echo '&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='$return_to';");
    echo '</td></tr></table>';

    break;

  default:
    if ($task == 'file_properties')
    {
      echo '<input type="hidden" name="title" value="'.$item['title'].'" />';
      echo '<input type="hidden" name="description" value="'.$item['description'].'" />';
      echo '<input type="hidden" name="keywords" value="'.$item['keywords'].'" />';
    }
    ?>
      <table border="0" cellpadding="4" cellspacing="0">
      <tr>
      <td colspan="2"><?php if(isset($feedback)) echo $feedback; ?>&nbsp;</td>
      </tr>
      <tr>
      <td>
      <?php echo $strName; ?>:
      </td>
      <td>
      <?php
      if($task == 'file_properties')
      {
	echo '<input type="text" class="textbox" name="name" value="'.strip_extension(htmlspecialchars($item['name'])).'" maxlength="100" size="30" />';
	echo '<input type="hidden" name="extension" value="'.get_extension($item['name']).'" />';
      }else
      {
	echo '<input type="text" class="textbox" name="name" value="'.htmlspecialchars($item['name']).'" maxlength="100" size="30" />';
      }
      ?>
	</td>
	</tr>
	<tr>
	<td>
	<?php echo $fbLocation; ?>:
	</td>
	<td>
	<?php
	echo $cms->get_path($item['parent_id']);
      ?>
	</td>
	</tr>
	<tr>
	<td valign="top">
	<?php echo $strType; ?>:
	</td>
	<td>
	<table border="0" cellpadding="0" cellspacing="0">
	<tr>
	<td valign="top">
	<?php
	if ($item['content_type'] == 'folder')
	{
	  echo '<img border="0" width="16" height="16" src="'.$GO_THEME->images['folder'].'" />';
	  echo '&nbsp;</td><td valign="top">';
	  echo $fbFolder;
	}else
	{
	  echo '<img border="0" width="16" height="16" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$item['extension'].'" />';
	  echo '&nbsp;</td><td valign="top">';
	  echo $item['content_type_friendly'];
	  if (($item['content_type'] != $item['content_type_friendly']) && $item['content_type'] != '')
	  {
	    echo '<br />('.$item['content_type'].')';
		}
		}
		?>
		</td>
		</tr>
		</table>
		</td>
		</tr>
		<tr>
		<td><?php echo $strSize; ?>:</td>
		<td><?php echo $item['size']; ?></td>
		</tr>
		<tr>
		<td>
		<?php echo $strModified; ?>:
		</td>
		<td>
		<?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $item['mtime']+($_SESSION['GO_SESSION']['timezone']*3600)); ?>
		</td>
		</tr>
		<tr>
	      <td valign="top">
	      <?php echo $cms_priority; ?>:
	      </td>
	      <td>
	      <?php
	      $priority = ($item['priority'] != '') ? $item['priority'] : $site['priority'];
	    ?>
	      <input type="text" class="textbox" name="priority" value="<?php echo $priority; ?>" maxlength="3" size="3" />
	      </td>
	      </tr>
	      <?php
	      if ($item['content_type'] == 'folder')
	      {
		$disabled_check = ($item['disabled'] == '1') ? true : false;
		echo '<tr><td colspan="2">';
		$checkbox = new checkbox('disabled', '1', $cms_hide_folder, $disabled_check, $no_hide);
		echo '</td></tr>';
		
		$display_type = isset($item['display_type']) ? $item['display_type'] : NORMAL_DISPLAY;
		if($site['display_type'] != MULTIPAGE_DISPLAY && $display_type == MULTIPAGE_DISPLAY)
		{
			$display_type = NORMAL_DISPLAY;
		}
		
		echo '<tr><td colspan="2">';
		$radio_list = new radio_list('display_type', $display_type);
		$radio_list->add_option(NORMAL_DISPLAY, $cms_normal_display);
		
		if($site['display_type'] == MULTIPAGE_DISPLAY)
		{
			echo '<br />';
			$radio_list->add_option(MULTIPAGE_DISPLAY, $cms_multipage_display);			
		}
		echo '<br />';
		$radio_list->add_option(HOT_ITEM_DISPLAY, $cms_hot_items);		
		echo '</td></tr>';
	      }
	    ?>
	      <tr>
	      <td colspan="2">
	      <br />
	      <?php
	      $button = new button($cmdOk, "javascript:_save('save_".$task."', 'true');");
	    echo '&nbsp;&nbsp;';
	    $button = new button($cmdApply, "javascript:_save('save_".$task."', 'false');");
	    echo '&nbsp;&nbsp;';
	    $button = new button($cmdClose, "javascript:document.location='$return_to';");
	    ?>
	      </td>
	      </tr>
	      </table>

	      <?php
	      break;
}
$tabtable->print_foot();
?>
  <script type="text/javascript" language="javascript">
function _save(task, close)
{
  document.forms[0].task.value=task;
  document.forms[0].close.value=close;
  document.forms[0].submit();
}
</script>

<?php
require($GO_THEME->theme_path."footer.inc");
?>
