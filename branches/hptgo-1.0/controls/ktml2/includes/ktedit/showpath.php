<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.
	
	session_start();
?><?php
	include("functions.inc.php");
	include_once("languages/".((isset($HTTP_SESSION_VARS['KTML2security']) && isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php"); 
	
?>
<html>
<head>
<link href="styles/main.css" rel="stylesheet" type="text/css">
<?php
	$current_path = (isset($HTTP_GET_VARS["currentPath"]) ? urldecode($HTTP_GET_VARS["currentPath"]) : "");
	$dir = "/".$current_path;
?>
</head>
<body class="body">
<span class="directory" id="ktml_path"><?php echo (isset($KT_Messages["You are here:"])) ? $KT_Messages["You are here:"] : "You are here:"; ?><?php echo '  '.$dir;?></span>
</body>
