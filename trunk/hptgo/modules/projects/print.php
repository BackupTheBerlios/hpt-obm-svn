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

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

require($GO_THEME->theme_path."header.inc");
require("load.inc");
echo "\n<script type=\"text/javascript\">\nwindow.print();\n</script>\n";
require($GO_THEME->theme_path."footer.inc");
?>