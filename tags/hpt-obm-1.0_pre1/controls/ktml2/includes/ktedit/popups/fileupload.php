<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.
session_start();
	include("../functions.inc.php");
	//language resources
	include_once("../languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php"); 
	$ALT_PATH = "../";
	require_once("../security.php");
?>
<html>
	<head>
<title><?php echo (isset($KT_Messages["Upload File"])) ? $KT_Messages["Upload File"] : "Upload File"; ?></title>
<link href="../styles/main.css" rel="stylesheet" type="text/css">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<?php 
if ($secTest && $sessionTest) {
	if ($HTTP_GET_VARS['currentPath'] != "") {
		$the_path = __realpath($KT_PATH_VAR."/".urldecode($HTTP_GET_VARS['currentPath']), 1);
	} else {
		$the_path = __realpath($KT_PATH_VAR, 1);
	}
	// array containing permitted mime types
	$allowed_types = array(
		"application/x-shockwave-flash", 
		"application/octet-stream",
		"application/vnd.ms-powerpoint", 
		"application/vnd.ms-excel",
		"application/vnd.sun.xml.calc",
		"application/vnd.sun.xml.writer",
		"application/vnd.sun.xml.impress",
		"application/vnd.sun.xml.draw",
		"application/exe",
		"application/cab",
		"application/x-exe",
		"application/dos-exe",
		"application/pdf", 
		"application/postscript",
		"application/x-pdf", 
		"application/msword", 
		"application/mspowerpoint",
		"application/msexcell",
		"application/rtf",
		"application/z",
		"application/x-z",
		"application/arj",
		"application/x-arj", 
		"application/zip",
		"application/lzh",
		"application/lha",
		"application/x-lzh",
		"application/x-lha",
		"application/x-compress",
		"application/x-compressed",
		"application/x-lzh-archive",
		"application/x-zip-compressed",
		"application/x-gzip-compressed",
		"application/x-gzip",
		"application/x-lha",
		"application/x-rar",
		"application/x-tar", 
		"application/stuffit",
		"application/x-stuffit",
		"application/x-sit",
		"application/tif",
		"application/x-tif",
		"application/tiff",
		"application/x-tiff",
		"application/tga",
		"application/x-tga",
		"application/x-targa", 
		"application/wpg",
		"application/x-wpg",
		"application/photoshop",
		"application/psd",
		"application/ico",
		"application/x-ico", 
		"application/x-msmetafile",
		"application/wmf",
		"application/x-wmf",
		"application/asx", 
		"application/x-mplayer2",
		"zz-application/zz-winassoc-jif",
		"text/plain",
		"image/svg",
		"image/svg-xml",
		"image/svg+xml", 
		"image/wmf",
		"image/x-wmf",
		"image/x-win-metafile", 
		"image/ico",
		"image/x-icon", 
		"image/photoshop",
		"image/x-photoshop",
		"image/psd",
		"image/wpg",
		"image/x-wpg",
		"image/x-wordperfect-graphics",	
		"image/tga",
		"image/x-tga",
		"image/targa",
		"image/x-targa",
		"image/tif",
		"image/x-tif",
		"image/tiff",
		"image/x-tiff", 
		"image/x-png",
		"image/png",
		"image/gif",
		"image/x-xbitmap",
		"image/gi_",
		"image/bmp",
		"image/pjpeg", 
		"image/jpeg", 
		"video/quicktime", 
		"audio/basic",
		"audio/x-basic",
		"audio/asf", 
		"audio/au",
		"audio/x-au",
		"audio/x-pn-au", 
		"audio/aiff",
		"audio/x-aiff",
		"sound/aiff",
		"audio/rmf",
		"audio/x-rmf",
		"audio/x-pn-aiff",
		"audio/x-gsm",
		"audio/mid",
		"audio/m",
		"audio/midi",
		"audio/x-midi",
		"audio/vnd.qcelp",
		"audio/x-pn-realaudio", 
		"audio/x-aifc", 
		"audio/mpeg", 
		"audio/mp3", 
		"audio/x-wav",
		"audio/x-ms-wma",
		"audio/x-mpegurl",
		"audio/mpeg-url",
		"audio/vnd.rn-realaudio",
		"audio/x-pn-realaudio",
		"audio/x-realaudio",
		"audio/x-pm-realaudio-plugin",
		"video/quicktime",
		"video/x-quicktime", 
		"video/x-ms-wmv",
		"video/x-ms-asf",
		"video/mpeg", 
		"video/x-msvideo", 
		"video/x-ms-wmv", 
		"video/msvideo",
		"video/avi", 
		"video/x-ms-asf-plugin",
		"video/x-ms-asf" 
		);
	//allowed file extensions to be uploaded
	$allowed_ext = array(
			"bmp", "gif", "jpeg", "jpg", "jpe", "png", "tif", "jif", "pcx", "fpx", "pcd", "pct", "tga", "wpg", "dib", "eps", "cut", "msp", "psd", "ico", "wmf", "svg", 
			"pdf", "doc", "ppt", "pps", "xls", "rtf", "txt", "asc",
			"sxc", "sxw", "sxd", "sxi", 
			"mp2", "mp3", "wav", "aiff", "aifc", "aif", "au", "mpa", "m1v", "wma", "wmv", "mid", "rmi", "au", "snd", 
			"mpg", "mpeg", "avi", "asf", "m3u", "mp2v", "mpe", "mov", "ra", 
			"mov", "qt", "rm", "swf", 
			"tar", "gz", "zip", "rar", "arj", "arc", "lzh", "lha", "tgz", "z", "bz2",  
			"msi", "sit", "cab", "exe", "dxf"
		);
	function validate_upload($the_file, $allowed_types, $allowed_ext) {
		global $HTTP_GET_VARS, $allowed_types, $allowed_ext;
		$the_file_ext = explode(".", $the_file['name']);
		$the_file_ext = $the_file_ext[count($the_file_ext)-1];
		$iserror = "";
		if (isset($the_file['error'])) {
			if ($the_file['error'] == 0) {
				if (!in_array($the_file['type'], $allowed_types) || !in_array(strtolower($the_file_ext), $allowed_ext)) {
					$iserror = "You cannot upload this file type:<br>&lt;".$the_file_ext.': '.$the_file['type']."&gt;<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
				}
			} else {
				switch ($the_file['error']) {
					case 1:
						$iserror = "File exceeds global maximum file size !!!<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
					break;
					case 2:
						$iserror = "File exceeds local maximum file size !!!<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
					break;
					case 3:
						$iserror = "File only partial uploaded !!!<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
					break;
					case 4:
						$iserror = "No file !!!<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
					break;
				}
			}
		} else {
			if ($the_file == "none") {
				$iserror = "No file !!!<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
			} else {
				if (!in_array($the_file['type'], $allowed_types) || !in_array(strtolower($the_file_ext), $allowed_ext)) {
					$iserror = "You cannot upload this file type! (".$the_file['type'].")<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
				}
			}
		}
		return $iserror;
	}

	function upload($the_file) {
		global $the_path, $PHP_SELF, $HTTP_POST_VARS, $HTTP_GET_VARS;
		$copy_error = "";
		if(isset($HTTP_POST_VARS['newname']) && $HTTP_POST_VARS['newname'] != '') {
			$uploadnm = $HTTP_POST_VARS['newname'];
		} else {
			//verifies if the file name must be escaped
			if ($the_file['name'] != urldecode($the_file['name'])) {
				$uploadnm = stripslashes(urldecode($the_file['name']));
			} else {
				$uploadnm = stripslashes($the_file['name']);
			}
		}
		//TODO: aicie se face practic mutarea fisierului din temporar in loc care vrea userul
		if (!move_uploaded_file($the_file['tmp_name'], $the_path.DIRECTORY_SEPARATOR.$uploadnm)) {
			$copy_error = "Check the path to and the permissions for the upload directory.<br><a href=\"fileupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] ."&submode=".$HTTP_GET_VARS['submode']."\">Press here</a> to go back";
		}
		return $copy_error;
	}

	if (isset($HTTP_POST_VARS['action'])) {
		$validate_error = validate_upload($HTTP_POST_FILES['the_file'], $allowed_types, $allowed_ext);
		if ($validate_error == "") {
			$upload_error = upload($HTTP_POST_FILES['the_file']);
			if ($upload_error == "") {
				echo "<script>\n";
				echo "if (window.opener._dlg_) { \n";
				//echo "	window.opener._dlg_.frames['centru'].location.reload(true);\n";
				echo "	window.opener._dlg_.frames['centru'].location = window.opener._dlg_.frames['centru'].location + '&rand=' + Math.random();\n";
				echo "} else { \n";
				echo "	window.opener.parent.frames['centru'].location.reload(true);\n";
				echo "}\n";
				echo "window.close();</script>";
			} else {
				die($upload_error);
			}
			exit();
		} else {
			die($validate_error);
		}
	}
}
?>
</head>
<?php
if ($secTest && $sessionTest) {
?>
<script>
	function openHelp(helpStr) {
		if (typeof dialogArguments != "undefined") {
			dialogArguments.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr);
		} else {
			if (window.opener.ktmls) {
				window.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr);
			} else {
				window.opener.parent.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr);
			}
		}
	}
<?php 
	$handle = dir($the_path);
	$tmpstr1 = "arra = Array(";
	while ($file = $handle->read()) {
		if (($file != ".") && ($file != "..")) {
			$tmpstr1.= "'".$file."',";
	   }
	}
	if ($tmpstr1 != "arra = Array(") {
		$tmpstr1 = substr($tmpstr1, 0, -1);
	}
	$tmpstr1.=");\r\n";
	echo $tmpstr1;
?>
	function checkFile() {
		var nnel = document.forms["form2"].newname;
		var tmpcond = false;
		var filename = '';
		if( nnel.value == '' ) {
			var arrf = document.forms["form2"].the_file.value.split("\\");
			filename = arrf[arrf.length-1];
		} else {
			filename = nnel.value;
		}
		// chech file name compliace
		if(filename.match(/^[a-z0-9\-\s\._]*$/i)) {
			if(nnel.value != '') {
				// check file name extensions
				var arrf = document.forms["form2"].the_file.value.split("\\");
				var ufile = arrf[arrf.length-1];
				var u_the_file_ext = ufile.split(".");
				u_the_file_ext = u_the_file_ext[u_the_file_ext.length-1];
				var nfile = nnel.value;
				var n_the_file_ext = nfile.split(".");
				n_the_file_ext = n_the_file_ext[n_the_file_ext.length-1];
				if(n_the_file_ext != u_the_file_ext) {
					nnel.value = nfile+'.'+u_the_file_ext;
					filename = nfile+'.'+u_the_file_ext;
				}
			}

			for ( var i=0;i<arra.length;i++) {
				if (arra[i] == filename) { //file exists
					tmpcond = true;
				}
			}
		} else {
			var nn = prompt(KT_js_messages["Bad name"]?KT_js_messages["Bad name"]:"The file name is not compliant!\r\nPlease enter a new name for the file");
			if(!nn) {
				return false;
			}
			nnel.value = nn;
			return checkFile();
		}

		if (tmpcond) {
			var nn = prompt(KT_js_messages["File exists"]?KT_js_messages["File exists"]:"The file already exists!!!\r\nPlease enter a new name for the file:");
			if(!nn) {
				return false;
			}
			nnel.value = nn;
			return checkFile();
		} else {
			return true;
		}
	}

	function checkImage(o) {
		var tmpstr = o.value;
		var tmparr = tmpstr.split("\\"); //the ktml only works on windows anyway :)
		var str = tmparr[tmparr.length-1];
	}
</script>
<?php
}
?>
<body class="body" >
<?php
if ($secTest && $sessionTest) {
?>
	<form name="form2" enctype="multipart/form-data" method="post" action="fileupload.php?currentPath=<?php echo urlencode($HTTP_GET_VARS['currentPath']) ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>&submode=<?php echo $HTTP_GET_VARS['submode']; ?>" onsubmit="return checkFile()">
<table>
	<tr>
		<td>
		<fieldset  class="ktml_fieldset"><legend class="ktml_legend"><?php echo (isset($KT_Messages["Upload File"])) ? $KT_Messages["Upload File"] : "Upload File"; ?></legend>
		<table class="ktml_table">
			<tr>
				<td>
					<label class="ktml_label"><?php echo (isset($KT_Messages["Select a file to upload"]))? $KT_Messages["Select a file to upload"] : "Select a file to upload"; ?>:</label>
		  	</td>
		  </tr>
			<tr>
				<td>
		  		<input type="file" name="the_file" onchange="checkImage(this);" class="ktml_input">
				<input type="hidden" name="task" value="upload">
				<input type="hidden" name="newname" value=""/>
		  	</td>		  	
		  </tr>
		</table>
		</td>
		</td>
		<td valign="top">
			<input type="submit" class="ktml_button" name="action" value="<?php echo (isset($KT_Messages["Upload"]))? $KT_Messages["Upload"] : "Upload"; ?>">
			<input type="button" class="ktml_button" name="close" value="<?php echo (isset($KT_Messages["Cancel"]))? $KT_Messages["Cancel"] : "Cancel"; ?>" onclick="window.close()">
			<input type="button" onclick="openHelp('fileupload');" class="ktml_button" style="margin-top:15px;" value="<?php echo (isset($KT_Messages["Help"])) ? $KT_Messages["Help"] : "Help"; ?>"/>
		</td>
	</tr>
</table>
	</form>
<?php
} ?>
</body>
</html>
