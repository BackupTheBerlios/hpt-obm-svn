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
<title><?php echo (isset($KT_Messages["Upload Image"])) ? $KT_Messages["Upload Image"] : "Upload Image"; ?></title>
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
	"image/bmp", 
	"image/gif", 
	"image/pjpeg", 
	"image/jpeg", 
	"image/png", 
	"image/x-png", 
	"audio/x-pn-realaudio", 
	"audio/mpeg", 
	"audio/mp3", 
	"audio/x-wav", 
	"video/mpeg", 
	"video/quicktime", 
	"video/x-msvideo", 
	"video/x-ms-wmv", 
	"video/avi", 
	"application/x-shockwave-flash"
	);
$allowed_ext = array("bmp", "gif", "jpeg", "jpg", "jpe", "png", "swf");
// The Laplacian operator radius for image sharpening.
$NWS_Sharpen_radius = 1;
// The standard deviation for image sharpening.
$NWS_Sharpen_sigma = 1;

function validate_upload($the_file, $allowed_types, $allowed_ext) {
	global $HTTP_GET_VARS;
  	$the_file_ext = explode(".", $the_file['name']);
  	$the_file_ext = $the_file_ext[count($the_file_ext)-1];
	$iserror = "";
	if (isset($the_file['error'])) {
		if ($the_file['error'] == 0) {
			if (!in_array($the_file['type'], $allowed_types) || !in_array(strtolower($the_file_ext), $allowed_ext)) {
				$iserror = "You cannot upload this file type!!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter'] . "&submode=" . $HTTP_GET_VARS['submode'] . "\">Press here</a> to go back";
			}
		} else {
			switch ($the_file['error']) {
				case 1:
					$iserror = "File exceeds global maximum file size !!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."&submode=" . $HTTP_GET_VARS['submode'] ."\">Press here</a> to go back";
				break;
				case 2:
					$iserror = "File exceeds local maximum file size !!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."&submode=" . $HTTP_GET_VARS['submode'] ."\">Press here</a> to go back";
				break;
				case 3:
					$iserror = "File only partial uploaded !!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."&submode=" . $HTTP_GET_VARS['submode'] ."\">Press here</a> to go back";
				break;
				case 4:
					$iserror = "No file !!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."&submode=" . $HTTP_GET_VARS['submode'] ."\">Press here</a> to go back";
				break;
			}
		}
	} else {
		if ($the_file == "none") {
			$iserror = "No file !!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."&submode=" . $HTTP_GET_VARS['submode'] ."\">Press here</a> to go back";
		} else {
			if (!in_array($the_file['type'], $allowed_types) || !in_array(strtolower($the_file_ext), $allowed_ext)) {
				$iserror = "You cannot upload this file type!!!<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."&submode=" . $HTTP_GET_VARS['submode'] ."\">Press here</a> to go back";
			}
		}
	}
	return $iserror;
}

	function validate_gd() {
		$strret = "";
		if (function_exists('ImageTypes')) {
			if (@ImageTypes() & IMG_GIF) {
				$strret .= " *.gif";
			}
			if (@ImageTypes() & IMG_PNG) {
				$strret .= " *.png";
			}
			if (@ImageTypes() & IMG_JPG) {
				$strret .= " *.jpg";
			}
		}
	}
	
	function my_exec($command) {
		$retArray = Array();
		exec($command,$retArray);
		return implode('',$retArray);
	}

	function validate_imagick () {
		$pos = strpos(my_exec('convert'), 'ImageMagick');
		if ($pos === false) {
			return 0;
		} else {
			return 1;
		}		
	}

	function ResizeImage ($image, $pathToSave, $newWidth, $newHeight, $imagetype, $sharpen, $keep) {
		global $HTTP_GET_VARS;
		if (file_exists($pathToSave)) {
			$a=unlink($pathToSave);
		}
		if (!preg_match("#^image/#", $imagetype)) {
			return "<br>Image type not recognized : " . $imagetype;
		}
		$resize = ($newWidth != '' || $newHeight != '');
		if($sharpen == '' && !$resize){
			if (!move_uploaded_file($image, $pathToSave)) {
				return "Could not copy file. Check the path to and the permissions for the upload directory.<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."\">Press here</a> to go back";
			}
			return "";
		}
		$gd = false;
		if (function_exists("ImageTypes")) {
			if ($imagetype == 'image/gif' && @ImageTypes() & IMG_GIF) {
				$gd = true;
			} elseif (($imagetype == 'image/x-png' || $imagetype == 'image/png') && @ImageTypes() & IMG_PNG) {
				$gd = true;
			} elseif (($imagetype == 'image/pjpeg' || $imagetype == 'image/jpeg' || $imagetype == 'image/jpg') && @ImageTypes() & IMG_JPG) {
				$gd = true;
			}
		}
		if ($sharpen == 0 && $gd) {
			switch($imagetype) {
				case "image/gif":
					$srcImage=@imagecreatefromgif($image['tmp_name']);
					break;
				case "image/pjpeg":
				case "image/jpeg":
				case "image/jpg":
					$srcImage=@imagecreatefromjpeg($image['tmp_name']);
					break;
				case "image/x-png":
				case "image/png":
					$srcImage=@imagecreatefrompng($image['tmp_name']);
					break;
				default:
					$srcImage=@imagecreatefromjpeg($image['tmp_name']);
					break;
			}
			if ($srcImage) {
				$srcWidth = ImageSX( $srcImage ); 
				$srcHeight = ImageSY( $srcImage ); 
				if ($keep == '1') {
					if ($newWidth != '' && $newHeight != '') {
						$ratioWidth = $srcWidth/$newWidth; 
						$ratioHeight = $srcHeight/$newHeight; 
						if( $ratioWidth < $ratioHeight ){ 
							$destWidth = $newWidth * $srcWidth/$srcHeight; 
							$destHeight = $newHeight; 
						} else { 
							$destWidth = $newWidth; 
							$destHeight = $srcHeight/$ratioWidth; 
						}
					} else {
						if ($newWidth != '') {
							$ratioWidth = $srcWidth/$newWidth; 
							$destWidth = $newWidth; 
							$destHeight = $srcHeight/$ratioWidth; 
						} else {
							$ratioHeight = $srcHeight/$newHeight; 
							$destHeight = $newHeight; 
							$destWidth = $srcWidth/$ratioHeight;
						}
					}
				} else {
					$destWidth = $newWidth; 
					$destHeight = $newHeight; 
				}
				ob_start();
				phpinfo(8);
				$phpinfo=ob_get_contents();
				ob_end_clean();
				$phpinfo=strip_tags($phpinfo);
				$phpinfo=stristr($phpinfo,"gd version");
				$phpinfo=stristr($phpinfo,"version");
				$end=strpos($phpinfo,".");
				$phpinfo=substr($phpinfo,0,$end);
				$length = strlen($phpinfo)-1;
				$phpinfo=substr($phpinfo,$length);

				if (function_exists('imagecreatetruecolor') && $phpinfo>=2) {
					$destImage = @imagecreatetruecolor ($destWidth, $destHeight); 
				} else {
					$destImage = @imagecreate($destWidth, $destHeight); 
				}
				@ImageCopyResized ($destImage, $srcImage, 0, 0, 0, 0, $destWidth, $destHeight, $srcWidth, $srcHeight);
				@ImageJPEG ($destImage, $pathToSave, 90);
				@ImageDestroy ($srcImage); 
				@ImageDestroy ($destImage);
				if (file_exists ($pathToSave)) {
					return "";
				} else {
					return "<br>Error writing file to " . $pathToSave . ".<br>Please check folder permissions.<br/><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."\">Press here</a> to go back";
				}
			} else {
				return "<br>Unidentified GD Error<br/><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."\">Press here</a> to go back";
			}
		} else {
			$cmd = "convert";
			if ($resize) {
				$cmd .= " -sample ${newWidth}x${newHeight}";
				if (!$keep) {
					$cmd .= "!";
				}
				if ($imagetype == "jpg" or $imagetype == "png") {
					$cmd .= " -quality 90";
				}
			}
			if ($sharpen) {
				$cmd .= " -sharpen 1x1";
			}
			$cmd .=" ".$image['tmp_name']." ".escapeshellarg($pathToSave);
			if (@exec("$cmd 2>&1") != ''){
				return "<br>Error writing file to " . $pathToSave . "<br> <b>Please check folder permissions and ImageMagik installation.</b> <br/><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."\">Press here</a> to go back";
			}
		}
	}
} else {
	die((isset($KT_Messages["No credentials"]))? $KT_Messages["No credentials"] :
	'You don\'t have credentials to access this part of the editor. Please click <a href=# onclick="window.close()">here</a> to close this window');
}
function upload($the_file) {
  global $HTTP_GET_VARS, $HTTP_POST_VARS,$the_path;
	$copy_error = "";
	if(isset($HTTP_POST_VARS['newname']) && $HTTP_POST_VARS['newname'] != '') {
		$uploadnm = $HTTP_POST_VARS['newname'];
	} else {
		if ($the_file['name'] != urldecode($the_file['name'])) {
			$uploadnm = stripslashes(urldecode($the_file['name']));
		} else {
			$uploadnm = stripslashes($the_file['name']);
		}
	}
	if (!move_uploaded_file($the_file['tmp_name'], $the_path.DIRECTORY_SEPARATOR.$uploadnm)) { //not first dir
		$copy_error = "Could not copy file. Check the path to and the permissions for the upload directory.<br><a href=\"newupload.php?currentPath=".urlencode($HTTP_GET_VARS['currentPath'])."&counter=" . $HTTP_GET_VARS['counter']."\">Press here</a> to go back";
	}
	return $copy_error;
}

if (isset($HTTP_POST_VARS['action'])) {
	$validate_error = validate_upload($HTTP_POST_FILES['the_file'], $allowed_types, $allowed_ext);
	if ($validate_error == "") {
		$fl1 = ($HTTP_POST_VARS['newX'] == "" && $HTTP_POST_VARS['newY'] == "") ? "0":"1";
		$fl2 = ($HTTP_POST_VARS['newX'] != "" || $HTTP_POST_VARS['newY'] != "") ? "0":"1";
		$flg = $fl1.$fl2;
		$flg = ( ($HTTP_POST_VARS['newX'] == "" && $HTTP_POST_VARS['newY'] == "") ? "0":"1" ).( (isset($HTTP_POST_VARS['Sharpen'])) ? "1":"0" );
		$upload_error = "";
		switch ($flg) {
			case "00":
				$upload_error = upload($HTTP_POST_FILES['the_file']);
			break;
			case "01": 
			case "10":
			case "11":
				if(isset($HTTP_POST_VARS['newname']) && $HTTP_POST_VARS['newname'] != '') {
					$uploadnm = $HTTP_POST_VARS['newname'];
				} else {
					if ($HTTP_POST_FILES['the_file']['name'] != urldecode($HTTP_POST_FILES['the_file']['name'])) {
						$uploadnm = urldecode($HTTP_POST_FILES['the_file']['name']);
					} else {
						$uploadnm = $HTTP_POST_FILES['the_file']['name'];
					}
				}
				$size = array($HTTP_POST_VARS['newX'], $HTTP_POST_VARS['newY']);
				$kp = "";
				//if ($HTTP_POST_VARS['newX'] == "" || $HTTP_POST_VARS['newY'] == "") {
					$kp = "1";
				//}
				$upload_error = ResizeImage($HTTP_POST_FILES['the_file'], $the_path.DIRECTORY_SEPARATOR.$uploadnm, $size[0], $size[1], $HTTP_POST_FILES['the_file']['type'], (int)(isset($HTTP_POST_VARS['Sharpen'])), $kp);
			break;
		}
		if ($upload_error != "") {
			echo $upload_error;
		}	else {
	    echo "<script>\n";
			echo "if (window.opener._dlg_) { \n";
			//echo "	window.opener._dlg_.frames['centru'].location.reload(true);\n";
			echo "	window.opener._dlg_.frames['centru'].location = window.opener._dlg_.frames['centru'].location + '&rand=' + Math.random();\n";
			echo "} else { \n";
			echo "	window.opener.parent.frames['centru'].location.reload(true);\n";
			echo "}\n";
			echo "window.close();</script>";
		}
		exit();
	} else {
		die($validate_error);
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
		var o = document.getElementById('trtohide');		
		if (str.match(/\.(?:gif|jpg|jpeg|png)/gi)) {
			o.style.display="block";
		} else {
			//document.forms[0].resize.selectedIndex = 4;
			//document.forms[0].Sharpen.checked = false;
			o.style.display="none";
		}
	}

	function updateTexts(o) {
		var xul = document.getElementById("newX");
		var yul = document.getElementById("newY");
		var str = o.options[o.selectedIndex].value;
		if(o.options[o.selectedIndex].value == 'c') {
			xul.readOnly = false;
			yul.readOnly = false;
			xul.removeAttribute('readonly');
			yul.removeAttribute('readonly');
			xul.value = '';
			yul.value = '';
		} else {
			arr = str.split("x");
			xul.value = arr[0];
			yul.value = arr[1];
			xul.readOnly = true;
			yul.readOnly = true;
			xul.setAttribute('readonly', 'true');
			yul.setAttribute('readonly', 'true');;
		}
	}

	function checkFld(el, max) {
		var cond = true;
		//
		//if (!el.value.match(/^#[0-9a-fA-F]{6}*$/) && (el.value.match(/^[0-9]*%$/))) {
		if (!el.value.match(/^[0-9]*%?$/)) {
			cond = false;
			text = (KT_js_messages["Invalid value"]?KT_js_messages["Invalid value"]:"Invalid value");
		}
		if ( parseInt(el.value) > max) {
			cond = false;
			text = KT_sprintf((KT_js_messages["Value to big"]?KT_js_messages["Value to big"]:"Value too big! Must be less then %s"), max);
		}
		if (el.value == "") {
		 cond = true;
		}
		if (cond) {
			return true;
		} else {
			return false;
		}
	}
</script>
<?php
} ?>
<body class="body">
	<form name="form2" enctype="multipart/form-data" method="post" action="newupload.php?currentPath=<?php echo urlencode($HTTP_GET_VARS['currentPath']) ?>&counter=<?php echo $HTTP_GET_VARS['counter']; ?>&submode=<?php echo $HTTP_GET_VARS['submode']; ?>" onsubmit="return checkFile()">
<table border="0">
	<tr>
		<td>
		<fieldset  class="ktml_fieldset"><legend class="ktml_legend"><?php echo (isset($KT_Messages["Upload Image"])) ? $KT_Messages["Upload Image"] : "Upload Image"; ?></legend>
		<table border="0" class="ktml_table" cellspacing="2">
			<tr>
				<td>
					<label class="ktml_label"><?php echo (isset($KT_Messages["Select a file to upload"]))? $KT_Messages["Select a file to upload"] : "Select a file to upload"; ?>:</label>
		  	</td>
		  </tr>
			<tr>
				<td>
		  		<input type="file" name="the_file" onchange="checkImage(this);" class="ktml_input">
				  <input type="hidden" name="task" value="upload">
		  	</td>		  	
		  </tr>
		  <tr name="trtohide" id="trtohide">
			  <td nowrap align="left"> 
					<label class="ktml_label"><?php echo (isset($KT_Messages["resize to this"]))? $KT_Messages["resize to this"] : "Resize To"; ?>:</label>
					<br/>
			  	<select name="resize" onchange="updateTexts(this);" class="ktml_select" style="width:100px">
						<option value="60x60">60x60</option>
						<option value="100x100">100x100</option>
						<option value="150x150">150x150</option>
						<option value="250x250">250x250</option>
						<option value="c"><?php echo (isset($KT_Messages["Custom"]))? $KT_Messages["Custom"] : "Custom"; ?></option>
						<option value="x" selected><?php echo (isset($KT_Messages["Leave Image"]))? $KT_Messages["Leave Image"] : "Leave Image"; ?></option>
					</select>
					<input type="text" onChange="if (!checkFld(this, 3000)) {this.value=''; return false;}" readonly="true" name="newX" id="newX" size="5" value="" class="ktml_input"><label class="ktml_label"> x </label><input readonly="true" type="text" name="newY" id="newY" size="5" value="" class="ktml_input" onChange="if (!checkFld(this, 3000)) {this.value=''; return false;}">
					<input type="hidden" name="resize" value="1x1"/>
					<input type="hidden" name="newname" value=""/>
					<br/>
			  </td>
				
			</tr>
			<tr>
<?php
	if (validate_imagick())  { ?>			  
			  <td nowrap align=left>
			  	<label class="ktml_label"><?php echo (isset($KT_Messages["Sharpen"]))? $KT_Messages["Sharpen"] : "Sharpen"; ?>:</label>
				<input type=checkbox name="Sharpen">
			  </td>
<?php
	} ?>			  
		  </tr>
		  <tr>
		  	<td>
			  	<br>
					<?php
						$strhidden = "";
						$strshow = "";
						if (function_exists('ImageTypes')) {
							if (@ImageTypes() & IMG_GIF) {
								$strshow .= " *.gif";
							}
							if (@ImageTypes() & IMG_PNG) {
								$strshow .= " *.png";
							}
							if (@ImageTypes() & IMG_JPG) {
								$strshow .= " *.jpg";
							}
							if (@ImageTypes() & IMG_WBMP) {
								$strshow .= " *.bmp";
							}
						}?>
				</td>
			</tr>
		</table>
		</fieldset>
		</td>
		<td valign="top" align="right">
			<input type="submit" class="ktml_button" name="action" value="<?php echo (isset($KT_Messages["Upload"]))? $KT_Messages["Upload"] : "Upload"; ?>"><br/>
			<input type="button" class="ktml_button" name="close" value="<?php echo (isset($KT_Messages["Cancel"]))? $KT_Messages["Cancel"] : "Cancel"; ?>" onclick="window.close()">	
			<input type="button" onclick="openHelp('imageupload');" class="ktml_button" style="margin-top:15px;" value="<?php echo (isset($KT_Messages["Help"])) ? $KT_Messages["Help"] : "Help"; ?>"/>
		</td>
	</tr>
</table>
</form>
<?php
if ($secTest && $sessionTest) { ?>
	</form>
<?php echo (isset($KT_Messages["Accepted Image Types"]))? $KT_Messages["Accepted Image Types"] : "Accepted Image Types"; ?>:
<?php
	echo $strshow;
}
?> ;<br>
<?php echo (isset($KT_Messages["Other Media Files"]))? $KT_Messages["Other Media Files"] : "Other Media Files"; ?>: *.swf 
</body>
</html>
