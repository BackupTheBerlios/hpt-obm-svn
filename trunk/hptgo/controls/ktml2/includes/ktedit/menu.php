<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.
	
	session_start();
	include("functions.inc.php");
	include_once("languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php");
	require_once('security.php');
?>
<html>
<head>
<link href="styles/main.css" rel="stylesheet" type="text/css">
<?php
if ($secTest && $sessionTest) {
?>
<?php 
	$path_var = urldecode($KT_PATH_VAR);
	$current_path = (isset($HTTP_GET_VARS["currentPath"]) ?urldecode($HTTP_GET_VARS["currentPath"]) : "");
	$dir = $path_var."/".$current_path;
	$directory=opendir(__realpath($dir, 1));
	$arra = explode("/", $current_path);
	$sz = sizeof($arra);
?>
	<script>
	var activObject = null;
	var currentPath = '<?php echo $current_path; ?>';
	function divMouseDownDir(o, par){
		if (par == 0) {
			if (activObject) {
				activObject.className = "directory";
			}
			activObject = o;
			activObject.className = "directory_selected";
			parent.updR(activObject.getAttribute('path'));
		} else if (par == 1) {
			parent.updL(o.getAttribute('path'));
		}
		return true;
	}
	
	function getpath(o){
		return o.getAttribute('path');
	}

	function getpathFile(o){
		return o.getAttribute('path');
	}

	function cMkDir() {
		if (activObject) {
			p = activObject.getAttribute('path');
		} else {
			p = currentPath;
		}
		a = prompt("<?php echo (isset($KT_Messages["Enter folder name"]))? $KT_Messages["Enter folder name"] : "Please enter a folder name:"; ?>", 
					"<?php echo (isset($KT_Messages["New Folder"]))? $KT_Messages["New Folder"] : "New Folder"; ?>");
		if (a!="" && a!="undefined" && a!=null) {
			if (a.match(/^\w[0-9A-Za-z\s_\.]*$/)) {
				parent.frames['action'].location = 'action.php?param1=1&currentPath='+p+'&dirname='+escape(a)+'&mode=<?php echo $HTTP_GET_VARS['mode'] ?>&submode=<?php echo $HTTP_GET_VARS['submode'] ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>'+ '&rand='+Math.random();
			} else {
				alert("<?php echo (isset($KT_Messages["Folder name invalid"]))? $KT_Messages["Folder name invalid"] : "Folder name is not valid!"; ?>");
			}
		}
	}
	
	function cRmDir() {
		if (!activObject && (currentPath == "")) {
			alert("<?php echo (isset($KT_Messages["Cannot delete main"]))? $KT_Messages["Cannot delete main"] : "Cannot delete main directory !"; ?>");
		} else {
			if (activObject) {
				p = activObject.getAttribute('path');
			} else {
				p = currentPath;
			}
			if (confirm("<?php echo (isset($KT_Messages["Are you sure dir"]))? $KT_Messages["Are you sure dir"] : "Are you sure that you want to delete directory "; ?>\""+unescape(p)+"\" ?")) {
				parent.frames['action'].location = 'action.php?param1=2&currentPath='+p+'&mode=<?php echo $HTTP_GET_VARS['mode'] ?>&submode=<?php echo $HTTP_GET_VARS['submode'] ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>'+ '&rand='+Math.random();
				parent.frames['centru'].location = 'popups/blank.html';
				parent.updLR('');
			}
		}
	}
	function unselectDir() {
		if (activObject && window.event && event.srcElement.tagName!="SPAN" && event.srcElement.tagName!="INPUT" ) {
			activObject.className= "directory";
			activObject = null;
		}
		parent.updR(currentPath);
	}
	</script>
<?php
}
?>
</head>
<body class="body" >
<?php
if ($secTest && $sessionTest) {
?>
<table width=100% border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td class="directory" onClick="unselectDir();"><img border="0" src="../../images/editor_images/folder.gif" alt="."/><b>.</b></td>
	</tr>
<?php
if (isset($HTTP_GET_VARS['currentPath']) && $HTTP_GET_VARS['currentPath']!="") {
?>
	<tr>
		<?php
			//TODO: 
			//display "POINT" button: return to base dir to display in right frame
			// extract the parent directory from the $current_path variable: 
			$tmparr = explode("/", $current_path);
			$strToReturn = "";
			for($i=0;$i<count($tmparr)-1;$i++) {
				$strToReturn .=$tmparr[$i]."/";
			}
		?>
		<td class="directory" onClick="parent.updLR('<?php echo urlencode(substr($strToReturn, 0, -1)); ?>');" title="" valign="middle"><img border="0" src="../../images/editor_images/folder.gif" alt=".."/><b>..</b></td>
	</tr>
<?php
}
//generate the directories under the $current_path
$arr = array();
if($directory) {
while (false !== ($readFile = readdir($directory))) { 
	$l = array('.', '..');
	if (!in_array( $readFile, $l) ){ 
		if (is_dir($dir."/".$readFile)){
				if($current_path!=""){
					$arr[$readFile] = $current_path."/".$readFile;
				} else {
					$arr[$readFile] = $readFile;
				}
		} 
  } 
}
}
//die(var_dump($arr));
//$arr: array with directory names under the current dir

asort($arr);
foreach($arr as $k=>$v) { ?>
	<tr>
		<td path="<?php echo $v; ?>" class="directory" onDblClick="divMouseDownDir(this,1);" onClick="divMouseDownDir(this,0);"><img border="0" src="../../images/editor_images/folder.gif" alt="<?php echo $v; ?>"/><b><?php echo $k; ?></b></td>
	</tr>
<?php
} ?>
</table>
<br>
	<input type="button" class="ktml_button" name="mkDir" value="<?php echo (isset($KT_Messages["Make Directory"]))? $KT_Messages["Make Directory"] : "Make Directory"; ?>" onclick="cMkDir();">
	<input type="button" class="ktml_button" name="rmDir" value="<?php echo (isset($KT_Messages["Delete Directory"]))? $KT_Messages["Delete Directory"] : "Delete Directory"; ?>" onclick="cRmDir();">
<?php
} else { ?>
<?php
}
?>
</body>
</html>
