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

$page_title=$menu_projects;
require($GO_MODULES->class_path."projects.class.inc");
$projects = new projects();

$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$time = get_time();
$day = date("j", $time);
$year = date("Y", $time);
$month = date("m", $time);

$date = date($_SESSION['GO_SESSION']['date_format'], $time);



require($GO_THEME->theme_path."header.inc");

?>

<table border="0" cellspacing="0" cellpadding="0">
<tr>

<?php
     
  echo '<td class="ModuleIcons">';
  echo '<a href="'.$projects_module['url'].'index.php?page=projects&return_to='.rawurlencode($link_back).'"><img src="'.$GO_THEME->images['project'].'" border="0" height="32" width="32" /><br />'.$pm_project.'</a></td>';
  
  echo '<td class="ModuleIcons">';
  echo '<a href="'.$projects_module['url'].'project.php?contact_id='.$contact_id.'&return_to='.rawurlencode($link_back).'"><img src="' .$GO_THEME->images['pr_new_project'].'" border="0" height="32" width="32" /><br />' .$pm_new_project.'</a></td>';

  echo '<!-- ';    
  
  
  echo '<td class="ModuleIcons">';
  echo '<img src="'.$GO_THEME->images['pr_load'].'" border="0" height="32" width="32" /><br />'.$pm_load.'</td>';
    
  echo '<td class="ModuleIcons">';
  echo '<img src="'.$GO_THEME->images['pr_fees'].'" border="0" height="32" width="32" /><br />'.$pm_total_fee.'</td>';
 
  echo '-->';
  
  if ($GO_MODULES->write_permissions) {
  echo '<td class="ModuleIcons">';
  echo '<a href="'.$projects_module['url'].'index.php?page=config&return_to='.rawurlencode($link_back).'"><img src="'.$GO_THEME->images['pr_fees'].'" border="0" height="32" width="32" /><br />'.$pm_config.'</a></td>';
}    
 
?>
</tr>
</table>

<?php
echo '<form name="projects_form" method="get" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="task" />';

echo '<br />';

if (isset($feedback)) echo $feedback;
switch ($_REQUEST['page'])
{
	case 'config':
		if ($GO_MODULES->write_permissions)
		{
		$tabtable = new tabtable('projects_tab', $pm_config, '100%', '400','120','&page=config');
			$tabtable->add_tab('template.inc', $pm_process_template);
			$tabtable->add_tab('category.inc', $pm_category);
		$tabtable->print_head();
		require($tabtable->get_active_tab_id());
		$tabtable->print_foot();
		}
	break;

	case 'projects':
		require('projects.inc');
	break;
	  
	default:
		require('projects.inc');
	break;
}

echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
