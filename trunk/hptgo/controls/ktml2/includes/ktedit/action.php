<?php
// Copyright 2001-2004 Interakt Online. All rights reserved.
	
	session_start();
	include("functions.inc.php");
	require_once('security.php');

	if (!$secTest || !$sessionTest) {
		die();
	}
	if (!isset($HTTP_GET_VARS['currentPath'])) {
		die();
	}

	function deldir($location) { 
		if (substr($location,-1) <> "/") 
		$location = $location."/"; 
		$all=opendir($location); 
		while ($file=readdir($all)) { 
			if (is_dir($location.$file) && $file <> ".." && $file <> ".") { 
				deldir($location.$file); 
				rmdir($location.$file); 
				unset($file); 
			} elseif (!is_dir($location.$file)) {
					unlink($location.$file); 
					unset($file); 
			} 
		} 
		closedir($all);
		rmdir($location); 
	}
	$currentPath = urldecode($HTTP_GET_VARS['currentPath']);
	$dirnamed=  urldecode(@$HTTP_GET_VARS['dirname']);
	$filenamed = urldecode(@$HTTP_GET_VARS['filename']);
	switch ($HTTP_GET_VARS['param1']) {
		case 1:
				if (!preg_match("/^\w[0-9A-Za-z\s_\.]*$/", $dirnamed)) {
					die();
				}
				if ($currentPath!="") {
					mkdir(urldecode($KT_PATH_VAR)."/".$currentPath."/".$dirnamed, 0777);
				} else {
					mkdir(urldecode($KT_PATH_VAR)."/".$dirnamed, 0777);
				}
		break;
		case 2: 
			deldir(urldecode($KT_PATH_VAR)."/".$currentPath);
		break;
		case 10: 
			unlink(urldecode($KT_PATH_VAR)."/".$currentPath);
		break;
		case 11: 
			if (!file_exists(urldecode($KT_PATH_VAR)."/".$currentPath."/".$filenamed)) {
				touch(urldecode($KT_PATH_VAR)."/".$currentPath."/".$filenamed, 0777); // Create blank file 
			}
		break;
		default:break;
	}
?>
<script>
<?php if ($HTTP_GET_VARS['param1']!= 10 && $HTTP_GET_VARS['param1']!= 2) { ?>	
	<?php if ($HTTP_GET_VARS['param1'] == 1) { ?>
	parent.frames['centru'].location = parent.frames['centru'].location  + '&rand='+Math.random();
	<?php } ?>
	parent.frames['menu'].location = parent.frames['menu'].location  + '&rand='+Math.random();
<?php } ?>
<?php if ($HTTP_GET_VARS['param1'] == 10) { ?>
	//parent.frames['centru'].location.reload(true);
	parent.frames['centru'].location = parent.frames['centru'].location + ((parent.frames['centru'].location.toString().indexOf("?") >= 0)? "&" :"?") + 'rand='+Math.random();
	parent.frames['props'].location = "rdframes.php?mode=<?php echo $HTTP_GET_VARS['mode'] ?>&submode=<?php echo $HTTP_GET_VARS['submode']; ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>";
<?php } ?>
<?php if ($HTTP_GET_VARS['param1'] == 2) { ?>	
	var deletedPath = '<?php echo $currentPath; ?>';
	var menuOldLoc_ = new String();
	var centruOldLoc_ = new String();
	var tmparr = new Array();
	var menuOldLoc = parent.frames['menu'].location.toString();
	tmparr = menuOldLoc.match(/currentPath=([^&]*)&/);
	if (tmparr && tmparr.length > 1) {
		 menuOldLoc_ = tmparr[1];
	}
	var centruOldLoc = parent.frames['centru'].location.toString();
	tmparr = centruOldLoc.match(/currentPath=([^&]*)&/);
	if (tmparr && tmparr.length > 1) {
		centruOldLoc_ = tmparr[1];
	}
	if (menuOldLoc_ == deletedPath) {
		if (menuOldLoc_.indexOf("/") != -1) {
			var newPath = menuOldLoc_.replace(/\/[^\/]*$/, "");
		} else {
			var newPath = "";
		}
		parent.frames['menu'].location = menuOldLoc.replace(/currentPath=([^&]*)&/, "currentPath="+newPath+"&");
	} else {
		//parent.frames['menu'].location.reload(true);
		parent.frames['menu'].location = parent.frames['menu'].location + '&rand='+Math.random();
	}
	if (centruOldLoc_ == deletedPath) {
		if (menuOldLoc_.indexOf("/") != -1) {
			var newPath = menuOldLoc_.replace(/\/[^\/]*$/, "");
		} else {
			var newPath = "";
		}
		parent.frames['centru'].location = centruOldLoc.replace(/currentPath=([^&]*)&/, "currentPath="+newPath+"&");
	} else {
		//parent.frames['centru'].location.reload(true);
		parent.frames['centru'].location = parent.frames['centru'].location + ((parent.frames['centru'].location.toString().indexOf("?") >= 0)? "&" :"?") + 'rand='+Math.random();
		//+ ((parent.frames['centru'].location.toString().indexOf("?") >= 0)? "&" :"?") + 'rand='+Math.random()
	}
	parent.frames['props'].location = "rdframes.php?mode=<?php echo $HTTP_GET_VARS['mode'] ?>&submode=<?php echo $HTTP_GET_VARS['submode']; ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>";
<?php } ?>
</script>
