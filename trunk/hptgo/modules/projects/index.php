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

$tabtable = new tabtable('projects_tab', $lang_modules['projects'], '100%', '400');
$tabtable->add_tab('projects.inc', $lang_modules['projects']);
$tabtable->add_tab('template.inc', $pm_process_template);
$tabtable->add_tab('category.inc', $pm_category);
/*
$tabtable->add_tab('load.inc', $pm_load);
$tabtable->add_tab('fees.inc', $pm_fees);
*/

switch($post_action)
{
	case 'projects':
		$tabtable->set_active_tab(0);
	break;

	case 'template':
		$tabtable->set_active_tab(1);
	break;

	case 'category':
		$tabtable->set_active_tab(2);
	break;

	case 'fees':
		$tabtable->set_active_tab(2);
	break;
}
if ($tabtable->get_active_tab_id() == 'load.inc')
{
	$datepicker = new date_picker();
	$GO_HEADER['head'] = $datepicker->get_header();
}

require($GO_THEME->theme_path."header.inc");

echo '<form name="projects_form" method="get" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="task" />';

$tabtable->print_head();
echo '<br />';

if (isset($feedback)) echo $feedback;
require($tabtable->get_active_tab_id());
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
