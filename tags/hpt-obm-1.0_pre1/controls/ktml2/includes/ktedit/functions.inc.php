<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.
	
	//configuration options

	//file configuration
	$KT_flinkmode = "simple"; // "simple", "complex"
	$KT_flinktoreplace = "";
	$KT_flinkprefix = ""; //a file which takes a parameter - urlencoded path of the serving script

	//https
	if (isset($HTTP_SERVER_VARS['HTTPS']) && $HTTP_SERVER_VARS['HTTPS']=="on")
		$KT_https = true;
	else
		$KT_https = false;

	//takes as argument an path string FROM THE WEB SERVER and returns an real path
	function __realpath($__path, $to_escape) {
		$tmp1 = realpath($__path);
		if ($to_escape) {
			$tmp1 = str_replace("\\", "\\\\", $tmp1);
		}
		return $tmp1;
	}
	//must return the path of the ROOT FOLDER OF THE WEB SERVER, OR THE ROOT FOLDER OF THE SOLUTION
	function documentroot() {
		global $HTTP_SERVER_VARS;
		
		if (isset($HTTP_SERVER_VARS['PATH_TRANSLATED'])) {
			$var_script_filename = "PATH_TRANSLATED";
		} else {
			$var_script_filename = "SCRIPT_FILENAME";
		}
		if (get_magic_quotes_gpc()) {
			$SCRIPT_FILENAME = stripslashes($HTTP_SERVER_VARS[$var_script_filename]);
		} else {
			$SCRIPT_FILENAME = $HTTP_SERVER_VARS[$var_script_filename];
		}
		$v2 = str_replace(DIRECTORY_SEPARATOR, "/", $SCRIPT_FILENAME);
		if (strstr($v2, "webapps") && strstr($v2, "admin/ktml2")) {
			$tmp = preg_replace("#/admin/ktml2/includes/ktedit.*$#", "", $v2);
		} else {
			$tmp = preg_replace("#/ktml2/includes/ktedit.*$#", "", $v2);
			//patch for PHP Installed as CGI on Linux
			if ($tmp == "") {
				 $tmp = "/../../..";
			}
			
		}
		return $tmp;
	}
	function __webtofs() {
	}
	function __fstoweb() {
	}
	
?>
