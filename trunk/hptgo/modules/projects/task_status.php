<?php
/*
   Copyright HPT Corporation 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 30 June 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require("../../Group-Office.php");


$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('projects');
require($GO_LANGUAGE->get_language_file('projects'));

$page_title=$lang_modules['projects'];
require($GO_MODULES->class_path."projects.class.inc");
$projects = new projects();

$project_id = isset($_GET['project_id']) ? $_GET['project_id'] : '0';
$project_task_id = isset($_GET['project_task_id']) ? $_GET['project_task_id'] : '0';

$projects->query("SELECT * FROM task WHERE task_project_id='$project_id' AND task_id='$project_task_id'");
if ($projects->next_record()) {
	$tstat = $projects->f("task_status");
	$tcomm = $projects->f("task_comment");
}
require($GO_THEME->theme_path."header.inc");
echo '<form method="get" action="'.$_SERVER['PHP_SELF'].'" name="projects_form">';
$dropbox = new dropbox();
$dropbox->add_value(0, $pm_task_status_values[0]);
$dropbox->add_value(1, $pm_task_status_values[1]);
$dropbox->add_value(2, $pm_task_status_values[2]);
$dropbox->print_dropbox("task_status", $tstat);
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="project_id" value="'.$project_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo "p = $project_id, t = $project_task_id";

require($GO_THEME->theme_path."footer.inc");
?>
