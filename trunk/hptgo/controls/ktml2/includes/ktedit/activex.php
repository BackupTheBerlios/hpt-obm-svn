<?php
// Copyright 2001-2004 Interakt Online. All rights reserved.
	@session_start();
	global $editor_sw;
	global $bd;
	global $ktEdit;
	global $validBrowser;
	$editor_sw = 1;
	$ktEdit = true;

		class KTML_browser {
		var $Name = "unknown";
		var $Version = "unknown";
		var $Platform = "unknown";
		
		function KTML_browser() {
			global $HTTP_SERVER_VARS;
			$HTTP_USER_AGENT = $HTTP_SERVER_VARS['HTTP_USER_AGENT'];
			if (eregi("opera",$HTTP_USER_AGENT)) {
				$this->Name = 'opera';
				preg_match("/opera\s+([0-9]+)/i", $HTTP_USER_AGENT, $tmp);
				$this->Version = $tmp[1];
			} elseif (eregi("msie",$HTTP_USER_AGENT)) {
				$this->Name = 'msie';
				preg_match("/msie\s+([0-9\.]+)/i", $HTTP_USER_AGENT, $tmp);
				$this->Version = $tmp[1];
			} elseif (eregi("safari",$HTTP_USER_AGENT)) {
				$this->Name = 'safari';
			} elseif (eregi("gecko",$HTTP_USER_AGENT)) {
				$this->Name = 'gecko';
				preg_match("/rv:\s*([0-9\.]+)/i", $HTTP_USER_AGENT, $tmp);
				$this->Version = $tmp[1];
			} elseif (eregi("konqueror",$HTTP_USER_AGENT)) {
				$this->Name = 'konqueror';
			}

			if (eregi("windows", $HTTP_USER_AGENT)) {
				$this->Platform = 'windows';
			} elseif (eregi("linux", $HTTP_USER_AGENT)) {
				$this->Platform = 'linux';
			} elseif (eregi("mac", $HTTP_USER_AGENT)) {
				$this->Platform = 'mac';
			} elseif (eregi("unix", $HTTP_USER_AGENT)) {
				$this->Platform = 'unix';
			}
		}
	}
	$bd = new KTML_browser();
	$validBrowser = false;
	if($bd->Name == 'msie' && (float)$bd->Version >= 6.0){
		$validBrowser = true;
	}elseif($bd->Name == 'gecko' && (float)$bd->Version >= 1.4) {
		$validBrowser = true;
	}
  /*
		$name, $width=700, $height=500, $ro, $display="ALL", $dirDepth="", $pathtoStyleFile = "", $pathToImageDir="../ktml2/images/uploads", $pathToFileDir="-1", $include=0, $modulelist=array()
  	showActivex - call this in order to include ktml
		parameters: 
  	$name - string, the name of the control, must be unique
  	$width  - numeric, ktml width
		$height - numeric, ktml height
  	$ro - true/false, readonly
  	$display - string, string with the buttons that are shown
  	$dirDepth - the ktml2 dir depth, RELATIVE TO THE CALLING SCRIPT (index.php);
  	$pathtoStyleFile - the path to the style file, RELATIVE TO THE CALLING SCRIPT (index.php);
  	$pathToImagedir - the path to the image upload directory, RELATIVE TO THE KTML2 INCLUDE DIRECTORY (RELATIV LA UNDE SE AFLA activex.php)
  	$pathToFiledir - the path to the file upload directory, RELATIVE TO THE KTML2 INCLUDE DIRECTORY (RELATIV LA UNDE SE AFLA activex.php)
		$include - generate the js include statements
		$defaultLanguage - default language for spellchecker
		keepAlive - if greater than 0, it expresses the number of seconds after which a new XmlHTTPRequest is sent to the browser, to prevent session expiring
		$ui_language - gui and help language
  */
	
  function showActivex(
		$name, 
		$width=700, 
		$height=500, 
		$ro, 
		$display="ALL", 
		$dirDepth="", 
		$pathtoStyleFile = "", 
		$pathToImageDir="../ktml2/images/uploads", 
		$pathToFileDir="-1", 
		$include=1, 
		$defaultLanguage="English (UK)", 
		$keepAlive = -1, 
		$ui_language="english",
		$autofocus="yes", 
		$xhtml="no"){ //
		
    global 
		$editor_sw, 
		$SCRIPT_URL, 
		$HTTP_SESSION_VARS, 
		$KTML2security, 
		$validBrowser, 
		$bd, 
		$counter, 
		$Ktml_mozilla, 
		$Ktml_ie;

		if (!isset($counter)) {
			$counter = 0;
		} else {
			$counter++;
		}
		//lista simpla
		//TODO de vazut daca se poate folosi ca sa parcurgi cu indecsi numerici array-ul $moduleexists
		$modulelist = array('introspection', 'tableedit', 'spellchecker', 'linkintrospector');
		$moduleexists = array();
		//register browser type in user session
		$Ktml_mozilla = ($bd->Name == 'gecko' || $bd->Name == 'Netscape');
		$Ktml_ie = !$Ktml_mozilla;
		// seteaza care module exista in array-ul moduleexists, verificand existenta directoarelor
		$scripturl = isset($SCRIPT_URL) ? $SCRIPT_URL : '';
		//TODO moduleexists este HASH
		foreach ($modulelist as $v) {
			//str_replace(basename(__FILE__),"", __FILE__)
			if (
				file_exists(
					str_replace(basename(__FILE__),"", __FILE__).
					str_replace("/", DIRECTORY_SEPARATOR, "modules/".$v)
				)
			) {
				$moduleexists[$v] = 1;
			} else {
				$moduleexists[$v] = 0;
			}
		}
		$objectName = $name;
		//creeaza array din string, folosind separatorul
		$display = explode(",", $display);
		if ($pathToFileDir == "-1") {
			$pathToFileDir = $pathToImageDir;
		}
		//verify if the image upload dir exists, if not create it
		if (!file_exists(str_replace(basename(__FILE__),"", __FILE__).str_replace("/", DIRECTORY_SEPARATOR, $pathToImageDir))) {
			mkdir(str_replace(basename(__FILE__),"", __FILE__).str_replace("/", DIRECTORY_SEPARATOR, $pathToImageDir));
		}
		//verify if the file upload dir exists, if not create it
		if (!file_exists(str_replace(basename(__FILE__),"", __FILE__).str_replace("/", DIRECTORY_SEPARATOR, $pathToFileDir))) {
			mkdir(str_replace(basename(__FILE__),"", __FILE__).str_replace("/", DIRECTORY_SEPARATOR, $pathToFileDir));
		}
		if (!preg_match("/^\.\//", $dirDepth)) {
			$dirDepth = "./".$dirDepth;
		}
		/*
		$KTML2security[ ktmlindex ] = array with properties of the ktml object ( path to image dir etc)
		for each ktml added to the page, a new index is added to this
		*/
		if (isset($HTTP_SESSION_VARS)) {
				$KTML2security = (isset($HTTP_SESSION_VARS["KTML2security"])? $HTTP_SESSION_VARS["KTML2security"] : array() );
				$KTML2security[$counter] = array();
				$KTML2security[$counter]['name'] = $objectName;
				if (in_array("Insert Image",$display) || $display[0] =="ALL" ) {
					$KTML2security[$counter]['right_image'] = 1;
					$KTML2security[$counter]['right_file'] = 1;
				} else {
					$KTML2security[$counter]['right_image'] = 0;
					$KTML2security[$counter]['right_file'] = 0;
				}
				$KTML2security[$counter]['right_spellchecker'] = 1;
				$KTML2security[$counter]['path_img'] = $pathToImageDir;
				$KTML2security[$counter]['path_file'] = $pathToFileDir;
				$KTML2security[$counter]['language'] = isset($ui_language)?$ui_language:'english';
				$KTML2security[$counter]['moduleexists'] = $moduleexists;
				/*
				 * if the php version is greater than 4.1.0 use $_SESSION
				 * else , check if register_globals is on
				 * if it is 
				 * if (register_globals) {
				 * 	session_register
				 * } else {
				 * 	$HTTP_SESSION_VARS
				 * }
				 */
				//registers the array in the session
				if (version_compare(phpversion(), "4.1.0", "<")) { //if the version is smaller than php 4.1.0
					if (ini_get('register_globals') == '1') { //if register globals is on
						session_register("KTML2security");
						session_register("Ktml_ie");
						session_register("Ktml_mozilla");						
					} else {//we don't have $_SESSION
						$HTTP_SESSION_VARS["KTML2security"] = $KTML2security;
						$HTTP_SESSION_VARS["Ktml_ie"] = $Ktml_ie;
						$HTTP_SESSION_VARS["Ktml_mozilla"] = $Ktml_mozilla;	
					}
				} else { // 
					$_SESSION['KTML2security'] = $KTML2security;
					$_SESSION["Ktml_ie"] = $Ktml_ie;
					$_SESSION["Ktml_mozilla"] = $Ktml_mozilla;	
				}
		}
		$include = 1; //DEPRECATED
		global $KT_SCRIPTS_INCLUDED;
  	if(isset($include) && $include !=0 && !isset($KT_SCRIPTS_INCLUDED)) {
			$KT_SCRIPTS_INCLUDED = 1;
			//generates the java script include statements and global javascript variables 
			echo "<script language=\"JavaScript\">var Ktml_ext = \"php\"; </script>\n";
			if($HTTP_SESSION_VARS['Ktml_mozilla']) {
				echo "<script language=\"JavaScript\">var Ktml_mozilla = true; </script>\n";
				echo "<script language=\"JavaScript\">var Ktml_ie = false; </script>\n";
			} else {
				echo "<script language=\"JavaScript\">var Ktml_mozilla = false; </script>\n";
				echo "<script language=\"JavaScript\">var Ktml_ie = !Ktml_mozilla; </script>\n";
			}
			
			echo "<script language=\"JavaScript\">var NEXT_ROOT=\"".$dirDepth."\"; </script>\n";
			echo "<script language=\"JavaScript\">var SCRIPT_URL=\"".$scripturl."\"; </script>\n";
			if($HTTP_SESSION_VARS['Ktml_mozilla']) {
				echo "<script language='JavaScript' src=\"".$dirDepth."includes/ktedit/mozilla_ie_compat.js\"></script>\n";
			}
			echo "<script language=\"JavaScript\" src=\"".$dirDepth."includes/ktedit/utils3.js\"></script>\n";
			echo "<script language=\"JavaScript\" src=\"".$dirDepth."includes/ktedit/ktml.js\"></script>\n";
			if (isset($moduleexists['introspection']) && $moduleexists['introspection'] && (in_array("Introspection",$display) || $display[0] =="ALL" )) {
				echo "<script language=\"JavaScript\" src=\"".$dirDepth."includes/ktedit/modules/introspection/scripts.js\"></script>\n";
			}
			if (isset($moduleexists['linkintrospector']) && $moduleexists['linkintrospector']) {
				echo "<script language=\"JavaScript\" src=\"".$dirDepth."includes/ktedit/modules/linkintrospector/scripts.js\"></script>\n";
			}
		}
    if($ro){
			echo "<iframe height=\"$height\" width=\"$width\" id=\"${name}_htmlObject\" name=\"${name}_htmlObject\" src=\"".$dirDepth."includes/ktedit/blank.htm\"></iframe>
			";
			echo "
		    <script>
					if (window.addEventListener)
						window.addEventListener('load', function() { loadIframes(); } , false);
					else if (window.attachEvent)
						window.attachEvent('onload', function() { loadIframes(); });
					else if (document.getElementById)
						window.onload = function() { loadIframes(); };
		    </script> ";
    } else {
		//generates whole ktml code
		echo "
		<link rel=\"stylesheet\" href=\"${dirDepth}includes/ktedit/styles/main.css\" type=\"text/css\"/>
    <script>
		var ktml_".$name." = new Ktml(\"${name}\");
		ktml_".$name.".setPathToStyle(\"${pathtoStyleFile}\");
		ktml_".$name.".setPathToImageDir(\"${pathToImageDir}\");
		ktml_".$name.".setPathToFileDir(\"${pathToFileDir}\");
		ktml_".$name.".setUseTemplates(".(isset($moduleexists['templates'])? (($moduleexists['templates']) ? "1" : "0" ) : "0").");
		ktml_".$name.".setUseIntrospection(".(isset($moduleexists['introspection']) ? (($moduleexists['introspection']) ? (((in_array("Introspection",$display) || $display[0] =="ALL" )) ? "1" : "0") : "0" ) : "0").");
		ktml_".$name.".setUseLinkIntrospection(".(isset($moduleexists['linkintrospector']) ? (($moduleexists['linkintrospector']) ? "1" : "0" ) : "0").");
		ktml_".$name.".setUseSpellcheck(".(isset($moduleexists['spellchecker']) ? (($moduleexists['spellchecker']) ? (((in_array("SpellCheck",$display) || $display[0] =="ALL" )) ? "1" : "0") : "0" ) : "0").");
		ktml_".$name.".setKeepAliveTime(".$keepAlive.");
		ktml_".$name.".setDefaultLanguage('".$defaultLanguage."');
		ktml_".$name.".setUILanguage('".$ui_language."');
		ktml_".$name.".setAutoFocus('".(isset($autofocus) ? $autofocus : "no")."');
		ktml_".$name.".setXHTML('".(isset($xhtml) ? $xhtml : "no")."');
    </script>
	<div class=\"ktml\" id=\"".$name."_div\">
	<table width=\"${width}\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\">
	  <tr>
	    <td width=\"100%\">";
					?>
					<?php include($dirDepth."includes/ktedit/languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$counter]['language']))? $HTTP_SESSION_VARS['KTML2security'][$counter]['language']:"english").".inc.php"); ?>
					<?php
					//check for browser validity and includes the toolbar
					if($validBrowser) {
						if ($counter == 0) {
							echo "<script>
									function pageLoded() {
										return (document.readyState == 'complete');
									}
								</script>";
						}
						//TODO- INCLUDE toolbar
						include($dirDepth."includes/ktedit/toolbar.php");
					} else {
						//browser not valid, alert and stop
						echo "<a href='#' onclick=\"alert('Supported browsers: IE 6.0, Mozilla 1.4, Netscape 7.1 - or greater versions');\"> KTML - safe mode </a>";
					}
		echo "
			</td>
  	</tr>
  	<tr>
  		<td>";
		//if browser valid, dispolay KTML iframe and hidden texdtarea for source editing
		if($validBrowser) {
				echo "<iframe 
						id=\"${name}_htmlObject\" 
						height=\"${height}\"
						width=\"${width}\"
						style=\"width: ${width}; height: ${height};\"
						src=\"".$dirDepth."includes/ktedit/blank.htm\"
					></iframe>";
				echo "<textarea id=\"${name}_textObject\" name=\"${name}_textObject\" style=\"width:${width}px;height:${height}px;display:none\">
							</textarea>";
		} else {
			//browser not valid, display only textarea 
			echo "<textarea 
		    	id=\"${name}_htmlObject\" 
		    	height=\"${height}\"
		    	width=\"${width}\"
					style=\"width: ${width}px; height: ${height}px;\"
					src=\"about:blank\"
					onblur=\"document.getElementById('${name}').value = document.getElementById('${name}_htmlObject').value; \"
		    ></textarea>";
		}
		if($validBrowser) {
			echo "<script>
					if (window.addEventListener) {
						window.addEventListener('load', function() { document.readyState = 'complete'; ktml_".$name.".initializeUI(); } , false);
						window.addEventListener('load', function() { ktml_".$name.".focus(); } , false);
					} else if (window.attachEvent) {
						window.attachEvent('onload', function() { ktml_".$name.".focus(); });
						window.attachEvent('onload', function() { ktml_".$name.".initializeUI(); });
					} else if (document.getElementById) {
						window.onload = ktmlAddEvent(window.onload, ktml_".$name.".initializeUI());
						window.onload = ktmlAddEvent(window.onload, ktml_".$name.".focus());
					}
		    </script>";
		} else {
			echo "<script>
					if (window.addEventListener)
						window.addEventListener('load', function() { document.getElementById('${name}_htmlObject').value = document.getElementById('${name}').value; } , false);
					else if (window.attachEvent)
						window.attachEvent('onload', function() { document.getElementById('${name}_htmlObject').value = document.getElementById('${name}').value; });
					else if (document.getElementById)
						window.onload = function() { document.getElementById('${name}_htmlObject').value = document.getElementById('${name}').value; };
		    </script>";
		}
			if ($validBrowser) {
					echo "</td>
					</tr>
					<tr class='ktml_bg'>
						<td><table cellpadding=\"0\" cellspacing=\"0\" border=\"0\">
								<tr>
									<td valign=\"bottom\"><div name=\"DOMProperty_${objectName}\" id=\"DOMProperty_${objectName}\" class=\"taginspector\" style=\"display: block;height: 16px; \"></div></td>
			";
			echo "</tr>
							</table></td>
					</tr><tr class=\"ktml_bg\"><td id=\"".$name."_props\" name=\"".$name."_props\">";
					//TODO : include statement
					if (isset($moduleexists['introspection']) && $moduleexists['introspection']) {
//							include($dirDepth."includes/ktedit/modules/introspection/ui.php");
					}
					if (isset($moduleexists['linkintrospector']) && $moduleexists['linkintrospector']) {
							include($dirDepth."includes/ktedit/modules/linkintrospector/ui.php");
					}
			}
			echo '</td></tr></table></div>';
			echo '<script>
				function '.$name.'_transferContent() {
					var text = window.frames["'.$name.'_buffer"].document.body.innerHTML;
					var div = document.getElementById("'.$name.'_props");
					div.innerHTML += text;
					ktml_'.$name.'.properties.isLoaded = true;
					ktml_'.$name.'.properties.update();
					ktml_'.$name.'.propertieslink.update()
				}
				</script>';
			echo '<iframe id="'.$name.'_buffer" name="'.$name.'_buffer" style="display: none"></iframe>';
		}
  }
?>
