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

$link_back = $GO_MODULES->url.'browse.php?site_id='.$site_id.'&folder_id='.$folder_id;

//what to do before output
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
switch ($task)
{
  case 'upload':
    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $task = 'list';
      if (isset($_FILES['file']))
      {
	require_once($GO_CONFIG->class_path.'filetypes.class.inc');
	$filetypes = new filetypes();
	for ($i=0;$i<count($_FILES['file']['tmp_name']);$i++)
	{
	  if (is_uploaded_file($_FILES['file']['tmp_name'][$i]))
	  {
	    $extension = get_extension($_FILES['file']['name'][$i]);
	    if (!$filetypes->get_type($extension))
	    {
	      $filetypes->add_type($extension, $_FILES['file']['type'][$i]);
	    }

	    $name = $_FILES['file']['name'][$i];
	    $x=0;
	    while ($cms->file_exists($folder_id, $name))
	    {
	      $x++;
	      $name = strip_extension($_FILES['file']['name'][$i]).' ('.$x.').'.get_extension($_FILES['file']['name'][$i]);
	    }

	    $fp = fopen($_FILES['file']['tmp_name'][$i], 'r');
	    $content = addslashes(fread($fp, $_FILES['file']['size'][$i]));
	    fclose($fp);
	    if (eregi('htm', get_extension($name)))
	    {
	      $content = $cms->get_body($content);
	    }
	    $file_id = $cms->add_file($folder_id, $name, $content);
	    unlink($_FILES['file']['tmp_name'][$i]);
	  }
	}
      }
    }
    break;

  case 'add_folder':
    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $name = smart_addslashes(trim($_POST['name']));
      if ($name == '')
      {
	$feedback = '<p class="Error">'.$error_missing_field;
      }elseif($cms->folder_exists($folder_id, $name))
      {
	$feedback = '<p class="Error">Mapnaam bestaat al</p>';
      }elseif(!$cms->add_folder($folder_id, $name, $_POST['priority'],
					isset($_POST['disabled'])))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
	$task = '';
      }
    }
    break;

  case 'delete':
    if (isset($_POST['files']))
    {
      for ($i=0;$i<count($_POST['files']);$i++)
      {
	$cms->delete_file($_POST['files'][$i]);
      }
    }

    if (isset($_POST['folders']))
    {
      for ($i=0;$i<count($_POST['folders']);$i++)
      {
	$cms->delete_folder($_POST['folders'][$i]);
      }
    }
    break;

  case  'cut':
    $_SESSION['cut_files'] = isset($_POST['files']) ? $_POST['files'] : array();
    $_SESSION['cut_folders'] = isset($_POST['folders']) ? $_POST['folders'] : array();
    $_SESSION['copy_folders'] = array();
    $_SESSION['copy_files'] = array();
    $task = '';
    break;

  case 'copy':
    $_SESSION['copy_files'] = isset($_POST['files']) ? $_POST['files'] : array();
    $_SESSION['copy_folders'] = isset($_POST['folders']) ? $_POST['folders'] : array();
    $_SESSION['cut_folders'] = array();
    $_SESSION['cut_files'] = array();
    $task = '';
    break;

  case 'paste':
    while ($file = smartstrip(array_shift($_SESSION['cut_files'] )))
    {
      $cms->move_file($file, $folder_id);
    }
    while ($file = smartstrip(array_shift($_SESSION['copy_files'])))
    {
      $cms->copy_file($file, $folder_id);
    }
    while ($folder = smartstrip(array_shift($_SESSION['cut_folders'])))
    {
      $cms->move_folder($folder, $folder_id);
    }
    while ($folder = smartstrip(array_shift($_SESSION['copy_folders'])))
    {
      $cms->copy_folder($folder, $folder_id);
    }
    break;
}

//set the page title for the header file
$page_title = $lang_modules['cms'];

//require the header file. This will draw the logo's and the menu
require($GO_THEME->theme_path."header.inc");
echo '<form name="cms" method="post" action="'.$_SERVER['PHP_SELF'].'" enctype="multipart/form-data">';
echo '<input type="hidden" name="site_id" value="'.$site_id.'" />';
switch ($task)
{
  case 'upload':
    require('upload.inc');
    break;

  case 'add_folder':
    require('add_folder.inc');
    break;

  default:
    require('files.inc');
    break;
}
echo '</form>';

require($GO_THEME->theme_path."footer.inc");
?>
