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

$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

//load contact management class
require($GO_MODULES->class_path."notes.class.inc");
$notes = new notes();

$page_title = $lang_modules['notes'];
require($GO_THEME->theme_path."header.inc");

echo '<form name="template" method="post" action="'.$_SERVER['PHP_SELF'].'" />';
echo '<input type="hidden" name="template_id" value="" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';

//create a tab window
$tabtable = new tabtable('notes_tab', $lang_modules['notes'], '100%', '400');

if ($GO_MODULES->write_permissions)
{
  $tabtable->add_tab('notes', $lang_modules['notes']);
  $tabtable->add_tab('catagories', $no_catagories);
}

$tabtable->print_head();

//set the user_id so it will only show notes from this user
$user_id = $GO_SECURITY->user_id;

if ($tabtable->get_active_tab_id() == 'catagories')
{
  require('catagories.inc');
}else
{
  require('notes.inc');
}

$tabtable->print_foot();

echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
