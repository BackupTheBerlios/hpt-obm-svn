<?php
/************************************************************************/
/* TTS: Ticket tracking system                                          */
/* ============================================                         */
/*                                                                      */
/* Copyright (c) 2002 by Meir Michanie                                  */
/* http://www.riunx.com                                                 */
/*                                                                      */
/* This program is free software. You can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License.       */
/************************************************************************/
require("../../Group-Office.php");

//authenticate the user
//if $GO_SECURITY->authenticate(true); is used the user needs admin permissons

$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('opentts');

//set the page title for the header file
$page_title = "Opentts";
require($GO_CONFIG->class_path."opentts.class.inc");
require($GO_THEME->theme_path."header.inc");
$tts= new go_opentts();
require_once("classes.php");

require_once("language/lang-$language.php");

$start_time=time();
require("includes/visits.php");

Opentts::menu("Show_Tickets");
OpenTable();
if (isset($_POST['submit'])){
$submit=$_POST['submit'];
if ($submit=='filter'){
	$_SESSION['filter_field'][]="$field";
	$_SESSION['filter_value'][]="$strtosearch";
	$filter=1;
	$_SESSION['filter']=1;
}
if ($submit=='clear_filters'){
	session_unregister('filter_field');
	session_unregister('filter_value');
	unset($filter_field);
	unset($filter_value);
	$filter=0;
	$_SESSION['filter']=0;
}
}
echo Search::printdb();

CloseTable();
$stop_time=Time();
$stop_time=$stop_time - $start_time;
echo "This page was loaded in " . $stop_time ." miliseconds\n<br>";

?>
