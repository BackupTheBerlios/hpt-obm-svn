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
require($GO_LANGUAGE->get_language_file('opentts'));
require("includes/visits.php");

//set the page title for the header file
$page_title = "Opentts";

require($GO_THEME->theme_path."header.inc");
$tts= new db();
require_once("classes.php");


require_once("menu.php");
$tabtable = new tabtable('tickets_tabtable', $helpdesk_title_my_tickets, '100%', '400');
$tabtable->print_head();
$admin_tabtable=$tabtable->active_tab;
$submit='';
if (isset($_POST['submit'])){
	$submit=$_POST['submit'];
}elseif (isset($_GET['submit'])){
	$submit=$_GET['submit'];
}

if (isset($_GET['orderby'])){
	$orderby=$_GET['orderby'];
	$_SESSION['orderby']=$orderby;
}elseif (isset($_POST['orderby'])){
        $orderby=$_POST['orderby'];
	$_SESSION['orderby']=$orderby;
}elseif (isset($_SESSION['orderby'])){
        $orderby=$_SESSION['orderby'];
}
/*
*/
if (isset($_POST['hidden_box'])){
	if (isset($_POST['search_hidden'])){
		$_SESSION['search_hidden']=$_POST['search_hidden'];
	}else{
		session_unregister('search_hidden');
	}
}

if ($submit=='filter'){
	$_SESSION['filter_field'][]=$_GET['field'];
	$_SESSION['filter_value'][]=$_GET['strtosearch'];
	$filter=1;
	$field=$_GET['field'];
   	if (isset($_SESSION['count'])) $_SESSION['count']++; else $_SESSION['count']=1;
	$strtosearch=$_GET['strtosearch'];
	$_SESSION['filter']=1;
}
if ($submit=='clear_filters'){
	session_unregister('filter_field');
	session_unregister('filter_value');
	unset($filter_field);
	unset($filter_value);
   	$_SESSION['count'] = 0;
	$filter=0;
	$_SESSION['filter']=0;
}
if (isset($_SESSION['filter']) and $_SESSION['filter']==1){
	$filter_field=$_SESSION['filter_field'];
	$filter_value=$_SESSION['filter_value'];
	$filter=$_SESSION['filter'];
}
echo Search::printdb();

$tabtable->print_foot();

?>
