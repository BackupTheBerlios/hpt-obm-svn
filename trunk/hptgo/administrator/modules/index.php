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
$link_back = $_SERVER['PHP_SELF'];

require_once($GO_CONFIG->class_path.'filesystem.class.inc');
$fs = new filesystem();
$fs->root = '';




$page_title = $menu_modules;

$overlib = new overlib();
$GO_HEADER['head'] = $overlib->get_header();
require($GO_THEME->theme_path."header.inc");

switch($task)
{
  case 'process':
    if (isset($_REQUEST['install']) && is_array($_REQUEST['install']))
      do_install($_REQUEST['install']);	
    if (isset($_REQUEST['uninstall']) && is_array($_REQUEST['uninstall']))
      do_uninstall($_REQUEST['uninstall']);	
    if (isset($_REQUEST['enable']) && is_array($_REQUEST['enable']))
      update_enable($_REQUEST['enable']);	
    else
      update_enable(array());	

    break;

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

function do_install($pkgs)
{
  global $GO_MODULES,$GO_SECURITY,$feedback;
  foreach ($pkgs as $module_id) {
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
  }
}

function do_uninstall($pkgs)
{
  global $GO_MODULES;
  foreach ($pkgs as $module_id) {
    if ($module = $GO_MODULES->get_module($module_id))
    {
      $GO_MODULES->delete_module($module_id);			
    }
  }
}

function update_enable($pkgs)
{
	$db = new db();
	$db->query("SELECT id FROM modules");
	$disabled_modules = array();
	while ($db->next_record())
	{
		if (!in_array($db->f('id'),$pkgs))
			$disabled_modules[] = $db->f('id');
	}
	if (!empty($pkgs))
	$db->query("UPDATE modules SET enable=1 WHERE id in ('".implode("','",$pkgs)."')");
	if (!empty($disabled_modules))
	$db->query("UPDATE modules SET enable=0 WHERE id in ('".implode("','",$disabled_modules)."')");

}
?>
