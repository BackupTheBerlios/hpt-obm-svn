<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.
	
	session_start();
	include("functions.inc.php");
	require_once("languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php");	
?><html>
	<head>
	<link href="styles/main.css" rel="stylesheet" type="text/css"/>
</head>
<body class="body" >
<input type="button" class="ktml_button" name="upload" value="<?php 
	if ($HTTP_GET_VARS['submode'] == "file") {
		echo (isset($KT_Messages["Upload File"]))? $KT_Messages["Upload File"] : "Upload File"; 
	} else {
		echo (isset($KT_Messages["Upload Image"]))? $KT_Messages["Upload Image"] : "Upload Image"; 
	}
?>" onclick="parent.frames['centru'].cUploadDir();">
<input type="button" class="ktml_button" name="rmDir" value="<?php 
	if ($HTTP_GET_VARS['submode'] == "file") {
		echo (isset($KT_Messages["Delete File"]))? $KT_Messages["Delete File"] : "Delete File"; 
	} else {
		echo (isset($KT_Messages["Delete Image"]))? $KT_Messages["Delete Image"] : "Delete Image"; 
	}
?>" onclick="parent.frames['centru'].cRmFile();"></body>
</html>
