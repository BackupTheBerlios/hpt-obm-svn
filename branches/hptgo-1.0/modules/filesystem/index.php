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

$popup_feedback = '';
$mode = isset($mode) ? $mode : 'normal';

function access_denied_box($file)
{
  global $strAccessDenied;
  $string = "<script type=\"text/javascript\" language=\"javascript\">\n";
  $string .= "alert('".$strAccessDenied.": ".basename($file)."');\n";
  $string .=  "</script>\n";
  return $string;
}

function feedback($text)
{
  $string = "<script type=\"text/javascript\" language=\"javascript\">\n";
  $string .= 'alert("'.$text.'");';
  $string .= "</script>\n";
  return $string;
}
//set umask to 000 and remember the old umaks to reset it below
//umask must be 000 to create 777 files and folders
$old_umask = umask(000);

//basic group-office authentication
if (!defined('GO_LOADED'))
{
  require_once("../../Group-Office.php");
}
$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('filesystem');
require($GO_LANGUAGE->get_language_file('filesystem'));

$email_module = $GO_MODULES->get_module('email');
if($email_module && 
    (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $email_module['acl_read']) ||
     !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $email_module['acl_write'])))
{
  $email_module = false;
}

$GO_HANDLER = isset($GO_HANDLER) ? $GO_HANDLER : 'download.php';
$GO_MULTI_SELECT = isset($GO_MULTI_SELECT) ? $GO_MULTI_SELECT : true;

$target_frame = isset($target_frame) ? $target_frame : '_self';

//set path to browse
$home_path = $GO_CONFIG->file_storage_path.'users/'.$_SESSION['GO_SESSION']['username'];
if (!isset($_SESSION['GO_FILESYSTEM_PATH']))
{
  if (file_exists($home_path) || 
    ((file_exists($GO_CONFIG->file_storage_path.'users/') || mkdir($GO_CONFIG->file_storage_path.'users/')) &&
     mkdir($home_path)))
  {
    $_SESSION['GO_FILESYSTEM_PATH'] = $home_path;
  }else
  {
    die('Failed creating home directory. Check server configuration. See if "'.
	$GO_CONFIG->file_storage_path.'" exists and is writable for the webserver.'); 
  }
}
$path = isset($_REQUEST['path']) ? smartstrip($_REQUEST['path']) : $_SESSION['GO_FILESYSTEM_PATH'];
$urlencoded_path = urlencode($path);
$return_to_path = isset($_REQUEST['return_to_path']) ? smartstrip($_REQUEST['return_to_path']) : $path;

//create filesystem and filetypes object
require_once($GO_CONFIG->class_path.'filesystem.class.inc');
require_once($GO_CONFIG->class_path.'filetypes.class.inc');
$fs = new filesystem();
$filetypes = new filetypes();

$fs_settings = $fs->get_settings($GO_SECURITY->user_id);

//define task to peform
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$_SESSION['cut_files'] = isset($_SESSION['cut_files']) ? $_SESSION['cut_files'] : array();
$_SESSION['cut_folders'] = isset($_SESSION['cut_folders']) ? $_SESSION['cut_folders'] : array();
$_SESSION['copy_folders'] = isset($_SESSION['copy_folders']) ? $_SESSION['copy_folders'] : array();
$_SESSION['copy_files'] = isset($_SESSION['copy_files']) ? $_SESSION['copy_files'] : array();

//vars used to remember files that are to be overwritten or not
$overwrite_destination_path = isset($_POST['overwrite_destination_path']) ? smartstrip($_POST['overwrite_destination_path']) : '';
$overwrite_source_path = isset($_POST['overwrite_source_path']) ? smartstrip($_POST['overwrite_source_path']) : '';
$overwrite_all = (isset($_POST['overwrite_all']) && $_POST['overwrite_all'] == 'true') ? 'true': 'false';
$overwrite = isset($_POST['overwrite']) ? $_POST['overwrite'] : $overwrite_all;

require_once('group_folders.inc');
$group_folders = get_group_folders($GO_SECURITY->user_id, 0);

$read_permission = $write_permission = true;
if (!is_group_folder($group_folders, $path))
{
  //check read permissions and remember last browsed path
  $read_permission = $fs->has_read_permission($GO_SECURITY->user_id, $path);
  $write_permission = $fs->has_write_permission($GO_SECURITY->user_id, $path);
}

if (!$read_permission && !$write_permission)
{
  $_SESSION['GO_FILESYSTEM_PATH'] = $home_path;
  $task = 'access_denied';
}else
{
  if ($GO_CONFIG->window_mode != 'projects')
  $_SESSION['GO_FILESYSTEM_PATH'] = $path;
}

//cut paste or copy before output has started
switch ($task)
{
  case 'upload':
    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $task = 'list';
      if (isset($_FILES['file']))
      {
	$_SESSION['cut_files'] = array();
	$_SESSION['cut_folders'] = array();
	$_SESSION['copy_folders'] = array();
	$_SESSION['copy_files'] = array();
	for ($i=0;$i<count($_FILES['file']['tmp_name']);$i++)
	{
	  if (is_uploaded_file($_FILES['file']['tmp_name'][$i]))
	  {
	    $extension = get_extension($_FILES['file']['name'][$i]);
	    if (!$filetypes->get_type($extension))
	    {
	      $filetypes->add_type($extension, $_FILES['file']['type'][$i]);
	    }

	    if($fs->copy($_FILES['file']['tmp_name'][$i], $GO_CONFIG->tmpdir.'/'.$_FILES['file']['name'][$i]))
	    {
	      $_SESSION['copy_files'][] = $GO_CONFIG->tmpdir.'/'.$_FILES['file']['name'][$i];
	    }
	  }
	}

	while ($file = smartstrip(array_shift($_SESSION['copy_files'])))
	{
	  $new_path = $path.'/'.basename($file);
	  if (!$write_permission)
	  {
	    $popup_feedback .= access_denied_box($path);
	    break;
	  }elseif(file_exists($new_path))
	  {
	    if ($overwrite_destination_path == $new_path && $overwrite_all != 'true')
	    {
	      if ($overwrite == "true")
	      {
		$fs->copy($file, $new_path);
	      }
	    }else
	    {
	      array_unshift($_SESSION['copy_files'], $file);
	      $overwrite_source_path = $file;
	      $overwrite_destination_path = $new_path;
	      $task = 'overwrite';
	      break;
	    }
	  }else
	  {
	    $fs->copy($file, $path.'/'.basename($file));
	  }
	}
      }else
      {
	$task = 'upload';
	$feedback = '<p class="Error">'.$fbNoFile.' '.format_size($GO_CONFIG->max_file_size).'</p>';
      }
    }
    break;

  case  'cut':
    $_SESSION['cut_files'] = isset($_POST['files']) ? $_POST['files'] : array();
    $_SESSION['cut_folders'] = isset($_POST['folders']) ? $_POST['folders'] : array();
    $_SESSION['copy_folders'] = array();
    $_SESSION['copy_files'] = array();
    break;

  case 'copy':
    $_SESSION['copy_files'] = isset($_POST['files']) ? $_POST['files'] : array();
    $_SESSION['copy_folders'] = isset($_POST['folders']) ? $_POST['folders'] : array();
    $_SESSION['cut_folders'] = array();
    $_SESSION['cut_files'] = array();
    break;

  case 'paste':
    while ($file = smartstrip(array_shift($_SESSION['cut_files'])))
    {
      if ($file != $path.'/'.basename($file))
      {
	if (!$fs->has_write_permission($GO_SECURITY->user_id, $file))
	{
	  $popup_feedback .= access_denied_box($file);
	  break;
	}elseif(!$fs->has_write_permission($GO_SECURITY->user_id, $path))
	{
	  $popup_feedback .= access_denied_box($path);
	  break;
	}elseif(file_exists($path.'/'.basename($file)))
	{
	  if ($overwrite_destination_path == $path.'/'.basename($file) || $overwrite_all == 'true')
	  {
	    if ($overwrite == "true")
	    {
	      $fs->move($file, $path.'/'.basename($file));
	    }
	  }else
	  {
	    array_unshift($_SESSION['cut_files'], $file);
	    $overwrite_source_path = $file;
	    $overwrite_destination_path = $path.'/'.basename($file);
	    $task = 'overwrite';
	    break;
	  }
	}else
	{
	  $fs->move($file, $path.'/'.basename($file));
	}
      }
    }
    while ($file = smartstrip(array_shift($_SESSION['copy_files'])))
    {
      if ($file != $path.'/'.basename($file))
      {
	if (!$fs->has_read_permission($GO_SECURITY->user_id, $file))
	{
	  $popup_feedback .= access_denied_box($file);
	  break;
	}elseif(!$fs->has_write_permission($GO_SECURITY->user_id, $path))
	{
	  $popup_feedback .= access_denied_box($path);
	  break;
	}elseif(file_exists($path.'/'.basename($file)))
	{
	  if ($overwrite_destination_path == $path.'/'.basename($file) || $overwrite_all == 'true')
	  {
	    if ($overwrite == "true")
	    {
	      $fs->copy($file, $path.'/'.basename($file));
	    }
	  }else
	  {
	    array_unshift($_SESSION['copy_files'], $file);
	    $overwrite_source_path = $file;
	    $overwrite_destination_path = $path.'/'.basename($file);
	    $task = 'overwrite';
	    break;
	  }
	}else
	{
	  $fs->copy($file, $path.'/'.basename($file));
	}
      }
    }
    while ($folder = smartstrip(array_shift($_SESSION['cut_folders'])))
    {
      if ($folder != $path.'/'.basename($folder))
      {
	if (!$fs->has_write_permission($GO_SECURITY->user_id, $folder))
	{
	  $popup_feedback .= access_denied_box($folder);
	  break;
	}elseif(!$fs->has_write_permission($GO_SECURITY->user_id, $path))
	{
	  $popup_feedback .= access_denied_box($path);
	  break;
	}elseif(file_exists($path.'/'.basename($folder)))
	{
	  if ($overwrite_destination_path == $path.'/'.basename($folder) || $overwrite_all == 'true')
	  {
	    if ($overwrite == "true")
	    {
	      $fs->move($folder, $path.'/'.basename($folder));
	    }
	  }else
	  {
	    array_unshift($_SESSION['cut_folders'], $folder);
	    $overwrite_source_path = $folder;
	    $overwrite_destination_path = $path.'/'.basename($folder);
	    $task = 'overwrite';
	    break;
	  }
	}else
	{
	  $fs->move($folder, $path.'/'.basename($folder));
	}
      }
    }
    while ($folder = smartstrip(array_shift($_SESSION['copy_folders'])))
    {
      if ($folder != $path.'/'.basename($folder))
      {
	if (!$fs->has_read_permission($GO_SECURITY->user_id, $folder))
	{
	  $popup_feedback .= access_denied_box($folder);
	  break;
	}elseif(!$fs->has_write_permission($GO_SECURITY->user_id, $path))
	{
	  $popup_feedback .= access_denied_box($folder);
	  break;
	}elseif(file_exists($path.'/'.basename($folder)))
	{
	  if ($overwrite_destination_path == $path.'/'.basename($folder) || $overwrite_all == 'true')
	  {
	    if ($overwrite == "true")
	    {
	      $fs->copy($folder, $path.'/'.basename($folder));
	    }
	  }else
	  {
	    array_unshift($_SESSION['copy_folders'], $folder);
	    $overwrite_source_path = $folder;
	    $overwrite_destination_path = $path.'/'.basename($folder);
	    $task = 'overwrite';
	    break;
	  }
	}else
	{
	  $fs->copy($folder, $path.'/'.basename($folder));
	}
      }
    }
    break;

  case 'properties':
    if (isset($_POST['name']))
    {
      $name = trim($_POST['name']);
      if(validate_input($name))
      {
	if (isset($_POST['share_folder']) && !$fs->get_share($path))
	{
	  $fs->add_share($GO_SECURITY->user_id, $path);
	}else
	{
	  if (!isset($_POST['share_folder']))
	  {
	    $fs->delete_share($path);
	  }
	}

	if ($GO_SECURITY->has_admin_permission($GO_SECURITY->user_id) && $fs->is_common_folder($path))
	{
	  if (isset($_POST['system_folder']) && !$fs->is_system_folder($path))
	  {
	    if (!$fs->add_system_folder($path,$msg))
	      $feedback = '<p class="Error">'.$msg.'</p>';
	  }
	  else
	  {
	    if (!isset($_POST['system_folder']) && $fs->is_system_folder($path))
	    {
	      if (!$fs->delete_system_folder($path,$msg))
		$feedback = '<p class="Error">'.$msg.'</p>';
	    }
	  }
	}

	if (!$fs->has_write_permission($GO_SECURITY->user_id, $path))
	{
	  $feedback = '<p class="Error">'.$strAccessDenied.'</p>';
	}elseif ($name == '')
	{
	  $feedback = '<p class="Error">'.$error_missing_field.'</p>';
	}else
	{
	  if ($_POST['extension'] != '')
	  {
	    $_POST['extension'] = '.'.$_POST['extension'];
	  }
	  $location = dirname($path);
	  $name = smartstrip($name);
	  $new_path = $location.'/'.$name.$_POST['extension'];
	  if($name.$_POST['extension'] != basename($path))
	  {
	    if (file_exists($new_path))
	    {
	      $feedback = '<p class="Error">'.$fbNameExists.'</p>';
	    }else
	    {
	      if ($fs->move($path, $new_path))
	      {
		if ($return_to_path == $path)
		{
		  $return_to_path = $new_path;
		}
		$path = $new_path;
		$urlencoded_path = urlencode($path);
	      }
	    }
	  }
	}
      }else
      {
	$feedback = '<p class="Error">'.$invalid_chars .': " & ? / \</p>';
      }
      if ($_POST['close']=='true' && !isset($feedback))
      {
	$path = $return_to_path;
	$urlencoded_path = urlencode($path);
	$task = '';
      }

    }
    break;

  case 'save_archive':
    if (isset($_POST['archive_files']))
    {
      $name = trim($_POST['name']);
      if ($name == '')
      {
	$feedback = '<p class="Error">'.$error_missing_field.'</p>';
	$task = 'create_archive';
      }else
      {

	switch ($_POST['compression_type'])
	{
	  case 'zip':
	    if (get_extension($name) != $_POST['compression_type'])
	    {
	      $name .= '.'.$_POST['compression_type'];
	    }
	    require($GO_CONFIG->class_path.'pclzip.class.inc');
	    $zip = new PclZip($path.$GO_CONFIG->slash.$name);
	    $zip->create($_POST['archive_files'], PCLZIP_OPT_REMOVE_PATH, $path);
	    break;

	  default:
	    if (get_extension($name) != $_POST['compression_type'])
	    {
	      $name .= '.tar.'.$_POST['compression_type'];
	    }
	    require($GO_CONFIG->class_path.'pearTar.class.inc');
	    $tar = new Archive_Tar($path.$GO_CONFIG->slash.$name, $_POST['compression_type']);

	    if (!$tar->createModify($_POST['archive_files'], '', $path.$GO_CONFIG->slash))
	    {
	      $feedback = '<p class="Error">'.$fb_failed_to_create.'</p>';
	      $task = 'create_archive';
	    }
	    break;
	}
      }
    }
    break;

  case 'extract':
    if (isset($_POST['files']))
    {
      require($GO_CONFIG->class_path.'pearTar.class.inc');
      require($GO_CONFIG->class_path.'pclzip.class.inc');
      while ($file = array_shift($_POST['files']))
      {
	if (strtolower(get_extension($file)) == 'zip')
	{
	  $zip = new PclZip($file);
	  if (!$zip->extract(PCLZIP_OPT_PATH, $path, PCLZIP_OPT_SET_CHMOD, $GO_CONFIG->create_mode))
	  {
	    $popup_feedback .= feedback($zip->errorInfo(true));
	  }
	}else
	{
	  $tar = new Archive_Tar($file);
	  if(!$tar->extract($path))
	  {
	    $popup_feedback .= feedback($fb_failed_to_create.": '$file'");
	  }
	}
      }
    }
    break;
}

$page_title = htmlspecialchars(str_replace($GO_CONFIG->file_storage_path,$GO_CONFIG->slash,$path));

require($GO_THEME->theme_path.'header.inc');

echo $popup_feedback;

echo '<form name="filesystem" method="post" action="'.$_SERVER['PHP_SELF'].'" enctype="multipart/form-data">';
echo '<input type="hidden" name="path" value="'.$path.'" />';
echo '<input type="hidden" name="return_to_path" value="'.$return_to_path.'" />';
echo '<input type="hidden" name="share_path" />';

switch ($task)
{
  case 'mail_files':

    $_SESSION['attach_array'] = array();
    $_SESSION['num_attach']=0;
    require($email_module['class_path']."email.class.inc");
    $email = new email();
    if (isset($_POST['files']))
    {
      while ($file = smartstrip(array_shift($_POST['files'])))
      {
	if ($fs->has_read_permission2($GO_SECURITY->user_id, $file))
	{
		$tmp_file = $GO_CONFIG->tmpdir.md5(uniqid(time()));
		if (copy($file, $tmp_file))
		{
		  $filename = basename($file);
		  $extension = get_extension($filename);
		  if (!$type = $filetypes->get_type($extension))
		  {
		    $type = $filetypes->add_type($extension);
		  }
	
		  $email->register_attachment($tmp_file, $filename, filesize($file), $type['mime']);
		}
	}else
	{
	  $popup_feedback .= access_denied_box(basename($file));
	}
      }
      echo '<script type="text/javascript" language="javascript">';
      echo 'popup("'.$email_module['url'].'send.php?email_file=true","'.$GO_CONFIG->composer_width.'","'.$GO_CONFIG->composer_height.'");';
      echo '</script>';
    }
    require('listview.inc');
    break;

  case 'delete':
    if (isset($_POST['files']))
    {
      for ($i=0;$i<count($_POST['files']);$i++)
      {
	$file = smartstrip($_POST['files'][$i]);
	if(!$fs->delete($file))
	{
	  $popup_feedback .= access_denied_box(basename($file));
	}
      }
    }

    if (isset($_POST['folders']))
    {
      for ($i=0;$i<count($_POST['folders']);$i++)
      {
	$folder = smartstrip($_POST['folders'][$i]);
	if(!$fs->delete($folder))
	{
	  $popup_feedback .= access_denied_box(basename($folder));
	}
      }
    }
    require('listview.inc');
    break;

  case 'access_denied':
    require($GO_CONFIG->root_path.'error_docs/403.inc');
    break;

  case 'new_folder':
    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $name = smartstrip($_POST['name']);
      if ($name =='')
      {
	$feedback = '<p class="Error">'.$error_missing_field.'</p>';
	require('new_folder.inc');
      }elseif(!validate_input($name))
      {
	$feedback = '<p class="Error">'.$invalid_chars .': " & ? / \</p>';
	require('new_folder.inc');
      }elseif(file_exists($path.'/'.$name))
      {
	$feedback = '<p class="Error">'.$fbFolderExists.'</p>';
	require('new_folder.inc');
      }elseif(!@mkdir($path.'/'.$name, $GO_CONFIG->create_mode))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
	require('new_folder.inc');
      }else
      {
	require('listview.inc');
      }
    }else
    {
      if ($write_permission)
      {
	require('new_folder.inc');
      }else
      {
	require($GO_CONFIG->root_path.'error_docs/401.inc');
      }
    }
    break;

  case 'upload':
    if ($write_permission)
    {
      require('upload.inc');
    }else
    {
      require($GO_CONFIG->root_path.'error_docs/401.inc');
    }
    break;

  case 'overwrite':
    require('overwrite.inc');
    break;

  case 'properties':
    require('properties.inc');
    break;

  case 'read_permissions':
    require('read_permissions.inc');
    break;

  case 'write_permissions':
    require('write_permissions.inc');
    break;

  case 'shares':
    require('shares.inc');
    break;

  case 'search':
    require('search.inc');
    break;

  case 'create_archive':
    require('compress.inc');
    break;

  default:
    require($GO_MODULES->path.'listview.inc');
    break;

}

echo '</form>';

umask($old_umask);
require($GO_THEME->theme_path.'footer.inc');
?>
