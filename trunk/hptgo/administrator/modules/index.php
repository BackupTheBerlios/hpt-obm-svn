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
require($GO_LANGUAGE->get_base_language_file('modules'));

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = $GO_CONFIG->host.'administrator/';

require_once($GO_CONFIG->class_path.'filesystem.class.inc');
$fs = new filesystem();




$page_title = $menu_modules;

$overlib = new overlib();
$GO_HEADER['head'] = $overlib->get_header();
require($GO_THEME->theme_path."header.inc");

switch($task)
{
  case 'install':
    $module_id = $_POST['module_id'];

    $acl_read = $GO_SECURITY->get_new_acl('Module read: '.$module_id, 0);
    $acl_write = $GO_SECURITY->get_new_acl('Module write: '.$module_id, 0);

    if ($acl_read > 0 && $acl_write > 0)
    {
      if ($GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write) && 
	  $GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id,$acl_read))
      {
	if(!$GO_MODULES->add_module($module_id, $_REQUEST['version'], $acl_read, $acl_write))
	{
	  $feedback = '<p class="Error">'.$strSaveError.'</p>';
	}
      }else
      {			
	$GO_SECURITY->delete_acl($acl_read);
	$GO_SECURITY->delete_acl($acl_write);				
	$feedback = '<p class="Error">'.$strAclError.'</p>';
      }
    }else
    {
      $GO_SECURITY->delete_acl($acl_read);
      $GO_SECURITY->delete_acl($acl_write);
      $feedback = '<p class="Error">'.$strAclError.'</p>';
    }
    break;

  case 'uninstall':
    $module_id = $_POST['module_id'];

    if ($module = $GO_MODULES->get_module($_POST['module_id']))
    {
      $GO_MODULES->delete_module($module_id);			
    }
    break;
    
}

if ($task == 'install' || $task == 'uninstall')
{
  echo '<script type="text/javascript">';
  echo 'parent.location="'.$GO_CONFIG->host.'index.php?return_to='.urlencode($_SERVER['PHP_SELF']).'";';
  echo '</script>';
}

echo '<form method="post" name="modules" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="task" />';
echo '<input type="hidden" name="version" />';
echo '<input type="hidden" name="module_id" />';
echo '<input type="hidden" name="close" value="false" />';
require('modules.inc');
echo '</form>';

require($GO_THEME->theme_path."footer.inc");
?>
