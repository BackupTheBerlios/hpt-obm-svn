<?php
/*
Copyright T & M Web Enterprises 2003
Author: Mike Hostetler <mike@tm-web.com>
Version: 1.0 Release date: 01 November 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

/**
 **MODULE CONFIG VARIABLES
 **/

//This should correspond with the name of 
//the directory the module currently resides in
global $moduleName;
$moduleName = 'daogen';
//set the page title for the header file
$page_title = "DAO Generator";
error_reporting(E_ALL);
/*
 *	THIS FILE IS NOT MEANT TO BE MODIFIED BELOW THIS POINT!
 *	
 *	This file serves as a frontController for all module file.  It sets
 *  up global module variables and includes global module files.
 *
 *	DO NOT MODIFY BELOW THIS POINT IF YOU DON'T KNOW WHAT YOU'RE DOING!
 */

/**
 **START GROUP OFFICE STUFF
 **/

//Pull in the global configuration file
require("../../Group-Office.php");

$GO_SECURITY->authenticate();

$GO_MODULES->authenticate($moduleName);

// security check, can be improved
if (preg_match('/[^\/,-_;a-z]/i', $_REQUEST['rw_url'])) die('NO MESSING AROUND!');
															
// check for dynamic content and bring the string together
$page = implode('/',array_filter(explode('/', $_REQUEST['rw_url']), create_function('$v','return "$v" !== "";')));
																			
//Rewrite this so it doesn't point to 'rewrite.php'
$PHP_SELF = $page;

//Force the extension to php
$page = preg_replace('/\..*/','.php',$page);

//Test out the XUL extension
if(isset($_REQUEST['xul']) && $_REQUEST['xul'] == 1) {
	@require($GO_THEME->theme_path."header.inc");
	$page = 'xul/'.$page;
	//Force the extension to xul
	$page = preg_replace('/\..*/','.xul',$page);

	$page = 'xul/Login.xul';
	//Check if the action exists
	if(file_exists($page)) {
		echo '<iframe src="'.$page.'" width="100%" height="100%">';
	}
	else {
		//Error, include the error page
		if(!file_exists('errorPage.php'))
			echo "ERROR";
		else
			include_once('errorPage.php');
	}
	@require($GO_THEME->theme_path."footer.inc");
}
else {
	//require the header file. This will draw the logo's and the menu
	// there is also the simple_header.inc file without logo's and menu
	@require($GO_THEME->theme_path."header.inc");

	//Check if the action exists
	if(file_exists($page)) {
		//We have a winner, include the file

		@include_once($page);
	}
	else {
		//Error, include the error page
		if(!file_exists('errorPage.php'))
			echo "ERROR";
		else
			@include_once('errorPage.php');
	}

	@require($GO_THEME->theme_path."footer.inc");
}

//********************
//SPECIAL FUNCTIONS
//********************
//Mimiced Behavior: eval('$classname::$methodname($param1,$param2)');
//using call_user_func is safer and faster, that's why it's wrapped
//To use do call_static_class_method('class::method',$arg1,$arg2,$arg3);
function call_static_class_method() {
	$args = array_slice(func_get_args(),1);		//Take of the first element of the array
	$arg = func_get_arg(0);
	list($class,$method) = explode('::',$arg);	//Snag out the class and method name
	return call_user_func_array(array($class,$method),$args);	//Call the function and return it's value
}

//A very useful debug function
function debug( $v )
{
	print "<pre>";
	if( is_array( $v ) || is_object( $v )) 
		print_r( $v );
	else 
		print $v;
	print "</pre>";
}

function import( $type,$file ) {
	global $GO_CONFIG,$moduleName;
	$module_path = $GO_CONFIG->root_path."modules/".$moduleName."/";
	//Import a file and optionally return the path to the common directory
	switch($type) {
		case 'vo':
			$path = $module_path."vo/";
			break;
		case 'dao':
			$path = $module_path."dao/";
			break;
		case 'class':
			$path = $module_path."classes/";
			break;
		case 'action':
			$path = $module_path."actions/";
			break;
		default:
			//A special feature of this function is that if a type is specified
			//that is not recognized, it will use the type as part of the path
			$path = $module_path.$type;
			break;
	}

	//if($this->vars['debug'])
		//echo "Type: $type File: $file Path: $path Exists: $path$file.php<br>\n";

	//Make sure to strip off the extension just in case someone included it
	$file = str_replace('.php','',$file);

	if(file_exists($path.$file.'.php')) {
		require_once( $path . $file . '.php' );
		return $path;
	}
	else {
		return FALSE;
	}
}

?>
