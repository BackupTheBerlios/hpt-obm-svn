<?php
// Copyright 2001-2004 Interakt Online. All rights reserved.
	
	session_start();
	//die(var_dump($_SESSION));
?><?php
	include("functions.inc.php");
	include_once("languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php");
	$special_files = array(".htaccess");
	
?>
<html>
<head>
<?php require_once('security.php'); ?>
<link href="styles/main.css" rel="stylesheet" type="text/css">
<?php
if ($secTest && $sessionTest) {
?>	
<?php 
	$path_var = urldecode($KT_PATH_VAR);
	$current_path = (isset($HTTP_GET_VARS["currentPath"]) ?urldecode($HTTP_GET_VARS["currentPath"]) : "");
	$current_file = (isset($HTTP_GET_VARS["currentFile"]) ?urldecode($HTTP_GET_VARS["currentFile"]) : "");
	$dir = $path_var."/".$current_path;
	if (is_dir($dir)) {
		$directory = opendir(__realpath($dir, 1));
	} else {
		$directory = false;
	}
	$arra = explode("/", $current_path);
	$sz = sizeof($arra);
	$TDS = 5;
?>
	<script>
	var activFileObject = null;
	var activObject = null;
	var currentFilePath = '<?php echo $current_path; ?>';
	function divDoubleClickFile(o) {
		divMouseDownFile(o);
<?php if ($HTTP_GET_VARS['submode'] == "file") { ?>		
		parent.submitFileLink();
<?php } else { ?>
		parent.submitImage();
<?php } ?>		
	}

	function divMouseDownDir(o, par){
		if (par == 0) {
			if (activObject) {
				activObject.className = "directory";
			}
			activObject = o;
			activObject.className = "directory_selected";
			parent.updLR(activObject.getAttribute('path'));
		} else if (par == 1) {
			parent.updLR(o.getAttribute('path'));
		}
		return true;
	}
	
	var oldColor = 'buttonface';
	/**
		Search for a specific div and change it's color
		
		@params
			- parentNode - parent node to searxh for the div
			- colorDivId - id of the div to be searched
			- newcolor - new color of the div
		@return
			- boolean = true if succeded false otherwise
	*/
	function changeColorDiv(parentNode, colorDivId, newcolor) {
		if (!newcolor) {
			newcolor = oldColor;
		}
		var tmDivs = parentNode.getElementsByTagName('DIV');
		var divIdx;
		for (divIdx = 0;divIdx < tmDivs.length;divIdx++) {
			if (tmDivs[divIdx].getAttribute('id') == colorDivId) {
				oldColor = tmDivs[divIdx].style.borderColor;
				tmDivs[divIdx].style.borderColor = newcolor;
				return true;
			}
		}
		return false;
	}
	
	function divMouseDownFile(o) {
		var widt, heig;
		if (activFileObject) {
			changeColorDiv(activFileObject, 'colorDiv', 'buttonface');
		}
		activFileObject = o;
		changeColorDiv(activFileObject, 'colorDiv', '#000000');
		changeColorDiv(activFileObject, 'colorDiv', '#000000');
		var arrg = document.getElementsByTagName("DIV");	
		for ( var i=0;i< arrg.length;i++) {
			if (arrg[i].getAttribute('path')!=o.getAttribute('path')) {
				//arrg[i].className = "invisible";
			} else {
				//arrg[i].className = "visible";
				widt = arrg[i].getAttribute('wdt');
				heig = arrg[i].getAttribute('hgt');
			}
		}
		if (activFileObject.getAttribute('path').match(/^\//)) {
			pth = activFileObject.getAttribute('path');
		} else {
			pth = '/'+activFileObject.getAttribute('path');
		}
		parent.updateImgPropFrame(pth, widt, heig);
	}
//file



function util_openwnd(u,n,w,h) {
	var top, left, top, features;
	left = (screen.width - w) / 2;
	top = (screen.height - h) / 2;
	features = ";left="+left+",top="+top;
  winargs="width="+w+",height="+h+",resizable=no,scrollbars=no,status=1" + features;
	dialogargs = "center=yes;dialogHeight="+h+"px;dialogWidth="+w+"px;help=no;status=yes,resizable=no";
	if (window.showModalDialog) {
		n +="&rand="+Math.random();
		remote=window.showModelessDialog(n, window, dialogargs);
		if (n.match(/dirbrowser/)) {
			window._dlg_ = remote;
		}
	} else {
		remote=window.open(n, u, winargs);
		remote.focus();
	}
  
  if (remote != null) {
    if (remote.opener == null)
      remote.opener = self;
  }
  return remote;
}


	function cUploadDir() {
<?php
	if ($HTTP_GET_VARS['submode'] == "file") {
		echo "h=100;";
	} else {
		echo "h=250;";
	}
?>
		var w, h, top, left, top, features;
		w = 360;
		left = (screen.width - w) / 2;
		top = (screen.height - h) / 2;
		features = "height="+h+",width="+w+",resizable=no,scrollbars=no,left="+left+",top="+top;
		p = currentFilePath;
		if (typeof dialogArguments == 'undefined') {
			wnd = 				 window.open('popups/<?php if ($HTTP_GET_VARS['submode']=='file') {echo 'file';} else {echo 'new';} ?>upload.php?'+'currentPath='+p+'&counter=<?php echo $HTTP_GET_VARS['counter']; ?>&submode=<?php echo $HTTP_GET_VARS['submode']; ?>', 'UPLOAD', features);
		} else {
			wnd = dialogArguments.open('popups/<?php if ($HTTP_GET_VARS['submode']=='file') {echo 'file';} else {echo 'new';} ?>upload.php?'+'currentPath='+p+'&counter=<?php echo $HTTP_GET_VARS['counter']; ?>&submode=<?php echo $HTTP_GET_VARS['submode']; ?>', 'UPLOAD', features);
		}
		
	}
	
	function cRmFile() {
		if (activFileObject) {
			if (confirm("<?php echo (isset($KT_Messages["Are you sure del"]))? $KT_Messages["Are you sure del"] : "Are you sure you want to delete "; ?>\"" +decodeURIComponent(activFileObject.getAttribute("title")) +"\" ?")) {
				p = activFileObject.getAttribute('path');	
				parent.frames['action'].location = 'action.php?param1=10&currentPath='+p+'&mode=<?php echo $HTTP_GET_VARS['mode'] ?>&submode=<?php echo $HTTP_GET_VARS['submode'] ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>';
			}
		} else {
			alert("<?php echo (isset($KT_Messages["Select a file to delete"]))? $KT_Messages["Select a file to delete"] : "Please select a file to delete."; ?>");
		}
	}
	
	function lasyAssign(text) {
		if (parent.frames['showpath'].document.body) {
			try {
				var el = parent.frames['showpath'].document.getElementById('ktml_path');
				el.innerHTML = text;
			} catch (e) {
				return;
			}
		} else {
			setTimeout('lasyAssign(' + text + ')',50);
		}
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
<?php
$tmpcnt = 0; ?>
<table border="0" cellpadding="5" cellspacing="0" width="100%">
	<tr>
<?php
$arrdirs = array(); $arrfiles = array();

// this is the correct way to loop over a directory
if($directory) {
while (false !== ($readfile = readdir($directory))) {
	$l = array('.', '..');
	if (!in_array( $readfile, $l) ){ 
		if (is_dir($dir."/".$readfile)){
			if ($current_path!="") {
				$arrdirs[$readfile] = $current_path."/".$readfile;
			} else {
				$arrdirs[$readfile] = $readfile;
			}
		} elseif (!in_array($readfile, $special_files)) { //the readFile is a file
				if ($current_path!="") {
					$arrfiles[$readfile] = $current_path."/".$readfile;
				} else {
					$arrfiles[$readfile] = $readfile;
				}
		}
	}
}
}
asort($arrdirs); asort($arrfiles);
?>
<?php

foreach ($arrfiles as $k=>$v) { ?>
	<td width="20%" valign="center" align="center" nowrap>
<?php
		if($k == $current_file) {
			echo "<script>
			function b(oldHandler,newHandler) {
				var a = function () {
					newHandler();
					if (oldHandler) {
						oldHandler();
					}
				}
				return a;
			}
			
			//window.onload = b(window.onload, divMouseDownFile(document.getElementById(\"".$k."\")));
			window.onload = function () {divMouseDownFile(document.getElementById(\"".$k."\")); };
			</script>";
		}		

			if ((stristr($k,".jpg") || stristr($k,".gif") || stristr($k,".png") || stristr($k,".bmp") || stristr($k,".jpe")) && $HTTP_GET_VARS['submode'] == 'img') {
				$im = getImageSize(str_replace("//", "/", $KT_PATH_VAR."/".$v));
		    $srcWidth = $im[0];
		    $srcHeight = $im[1];
		    if ($srcWidth<=100 && $srcHeight<=100) {
		    	$destWidth = $srcWidth;
		    	$destHeight = $srcHeight;
			  } else {
			    $ratioWidth = $srcWidth/100;
			    $ratioHeight = $srcHeight/100;
			    if( $ratioWidth < $ratioHeight ){ 
			      $destWidth = 100 * $srcWidth/$srcHeight; 
			      $destHeight = 100; 
			    } else { 
			      $destWidth = 100; 
			      $destHeight = $srcHeight/$ratioWidth; 
			    } 				  	
			  }
					  
?>
	<a title="<?php echo $k ?>" id="<?php echo $k ?>" class="file" onmouseout="changeColorDiv(this,'colorDiv');" onmouseover="changeColorDiv(this,'colorDiv','#aaaaaa');" onmousedown="divMouseDownFile(this)" ondblclick="divDoubleClickFile(this)" path="<?php echo urlencode($v)?>" detail="0">
		<div id="colorDiv" style="border: solid 2px buttonface">
		<div style="width:105px;height:100px;border: solid 1px #000000" path="<?php echo urlencode($v)?>" wdt="<?php echo $srcWidth ?>" hgt="<?php echo $srcHeight ?>">
			<img style="margin-top:<?php echo floor((100-$destHeight)/2); ?>px;margin-left:0px;" id="PREVIEWPIC" name="PREVIEWPIC" height="<?php echo floor($destHeight); ?>" width="<?php echo floor($destWidth); ?>" src="<?php echo $KT_PATH_VAR."/".$v ?>" alt="<?php echo $k ?>">
		</div>
		</div>
	</a>
<?php
			} else { //not an image 
?>
	<a title="<?php echo $k ?>" class="file" onmouseout="changeColorDiv(this,'colorDiv');" onmouseover="changeColorDiv(this,'colorDiv','#aaaaaa');"  onmousedown="divMouseDownFile(this)" ondblclick="divDoubleClickFile(this)" path="<?php echo urlencode($v)?>" detail="0">
		<div id="colorDiv" style="border: solid 2px buttonface">
			<div style="width:105px;height:100px;border: solid 1px #000000" path="<?php echo urlencode($v) ?>">
				<div style="margin-top:40px" >
						<?php echo (isset($KT_Messages["Download"]))? $KT_Messages["Download"] : "Download"; ?>
				</div>
			</div>
		</div>
	</a>
<?php			} ?>
		<a title="<?php echo $k ?>"class="file" path="<?php echo urlencode($v)?>" detail="0"><?php echo substr($k, 0, 15)?></a>
			</td>
<?php
			if ($tmpcnt>2) { ?>
		</tr>
		<tr>
<?php
				$tmpcnt = 0;
			}	else {
				$tmpcnt ++;
			}
?>
<?php
} 
while ($tmpcnt<=4) { ?>
		<td width="20%">&nbsp;</td>
<?php
		$tmpcnt++;
} ?>
		</tr>
	</table>
<?php
} else {
?>
You have supplied an invalid path!!!
<?php
}
?>
</body>
</html>
