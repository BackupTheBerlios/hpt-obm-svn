<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.

	
	$aspellpaths = array('', 'c:\\progra~1\\aspell\\bin\\');
	$aspellpath = "";
	$aspellpath = "";
	foreach ($aspellpaths as $path) {
		$str = my_exec($path."aspell");
		if ($str != "") {
			$aspellpath = $path;
		}
	}
	session_start();
$languagelist = array(
	array("en", "british","English (UK)"),
	array("us", "american","English (USA)"),
	array("ge", "german", "German"),
	array("sp", "spanish", "Spanish"),
	array("fr", "french", "French"), 
	array("nl", "dutch", "Dutch")
	);
?>
<?php
	include("../../functions.inc.php");
	include_once("./languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php"); 
?>
<?php 
	$sessionTest = isset($HTTP_SESSION_VARS["KTML2security"]) && isset($HTTP_GET_VARS['counter']) && isset($HTTP_SESSION_VARS["KTML2security"][$HTTP_GET_VARS['counter']]); 
	if (!$sessionTest) {
		die((isset($KT_Messages["No credentials"]))? $KT_Messages["No credentials"] :
		'You don\'t have credentials to access this part of the editor. Please click <a href=# onclick="window.close()">here</a> to close this window');
	}
	
	$addedWords = array();
	if (file_exists('addedWords.txt')) {
		$tmpAddedWords = file('addedWords.txt');
		foreach($tmpAddedWords as $key => $value) {
			$addedWords[trim($value)] = 1;
		}
	}
	
	function addedWord($word) {
		global $addedWords;
		if (isset($addedWords[$word])) {
			return true;
		} else {
			return false;
		}
	}

	function testAspell($dictname) {
		global $aspellpath;
		if(strpos(my_exec($aspellpath."aspell"), 'Aspell') !== false 
				&& !strstr(my_exec($aspellpath."aspell"), ".33.7.1 alpha") 
				&& my_exec($aspellpath."aspell -d $dictname -a ") != '') {
			return 1;
		} else {
			return 0;
		}
	}


	function testPspell($isocode) {
		if (function_exists('pspell_new') && @pspell_new($isocode)) {
			return 1;
		} else {
			return 0;
		}
	}

	function my_exec($command) {
		$retArray = Array();
		exec($command,$retArray);
		return implode('',$retArray);
	}

?>
<html>
<?php
$errorstring = "";
function parseStringA($text, $dialects = array()) {
	global $errorstring, $parameters, $aspellpath;
	$dialect = "english";
	if (isset($dialects[1]) && $dialects[1] != "") {
		$dialect = $dialects[1];
	}
	$text = str_replace(array("\r","\n"), array(" ", " "), $text);
	$temptext= tempnam(".", "spelltext");
	$tempouttext = $temptext.".out.spell";
	// do not check alt attributes
	if ($fd=fopen($temptext,"w")) {
		$textarray= explode("\n",$text);
		fwrite($fd,"!\n");
		foreach($textarray as $key=>$value) {
			fwrite($fd,"^$value\n"); //prohibit aspell from believing that lines are aspell commands
		}
		fclose($fd);
		// next run aspell
		$return = my_exec($aspellpath."aspell -a -d ".$dialect." -H --rem-sgml-check=alt < ".$temptext." > ".$tempouttext);

		//RST: works on PHP4 < 4.3.0
		if (!function_exists('file_get_contents')) {
		   $file = @fopen($tempouttext, "rb", 0);
		   if ($file) {
		     while (!feof($file)) $return .= fread($file, 1024);
		     fclose($file);
		   } else {
		     /* There was a problem opening the file. */
		     return FALSE;
		   }
		   return $return;
		} else {
			$return = file_get_contents($tempouttext);
		} 		

		// unlink tempfile
		$ureturn= unlink($temptext);
		$ureturn= unlink($tempouttext);
		// next parse $return and $text line by line, eh
		$returnarray= explode("\n",$return);
		$returnlines= count($returnarray);
		$textlines= count($textarray);
		$lineindex= -1;
		$poscorrect= 0;
		$jstext = ""; $jsarrindex = 0;
		
		foreach($returnarray as $key=>$value) {
			// if there is a correction here, processes it, else move the $textarray pointer to the next line
			if (substr($value,0,1)=="&") {
				$correction= explode(" ",$value);
				$word = $correction[1];
				if (addedWord($word)) {
					continue;
				}
				$absposition= substr($correction[3],0,-1)-1;
				$position= $absposition+$poscorrect;
				$niceposition= $lineindex.",".$absposition;
				$suggstart= strpos($value,":")+2;
				$suggestions= substr($value,$suggstart);
				$suggestionarray= explode(", ",$suggestions);
				//$opener= '<span onclick="fillOptions(this)" onmouseover="captureTooltip(event);" options="'.implode("|", $suggestionarray).'" class="nosuggestion">';
				$opener= '<span badword=1>';
				//$closer= "</span>";
				$closer = "</span>";
				// highlight in text
				$beforeword= substr($textarray[$lineindex],0,$position);
				$afterword= substr($textarray[$lineindex],$position+strlen($word));
				$textarray[$lineindex]= $beforeword."$opener$word$closer".$afterword;

				$jstext .= "\nwords[".$jsarrindex."] = new Object();";
				$jstext .= "\nwords[".$jsarrindex."].status = 0;";
				$jstext .= "\nwords[".$jsarrindex."].word = \"".$word."\";";
				$jstext .= "\nwords[".$jsarrindex."].suggestions = new Array();";
				$jsarrindex2 = 0;
				foreach ($suggestionarray as $suggestion) {
					$jstext .= "\nwords[".$jsarrindex."].suggestions[".$jsarrindex2."] = \"".str_replace(array("\r", "\n"), array("", ""),$suggestion)."\";";
					$jsarrindex2++;
				}
				$jsarrindex++;

				// dirty hack for recalculating next index position
				$poscorrect= $poscorrect+strlen("$opener$closer");
			} elseif (substr($value,0,1)=="#") {
				$correction= explode(" ",$value);
				$word= $correction[1];
				$absposition= $correction[2] - 1;
				$position= $absposition+$poscorrect;
				$niceposition= $lineindex.",".$absposition;
				$suggestions = "";
				$suggestionarray= explode(", ",$suggestions);
				
				//$opener= '<span onclick="fillOptions(this)" onmouseover="captureTooltip(event);" options="'.implode("|", $suggestionarray).'" class="nosuggestion">';
				$opener= '<span badword=1>';
				//$closer= "</span>";
				$closer = "</span>";
			
				$jstext .= "\nwords[".$jsarrindex."] = new Object();";
				$jstext .= "\nwords[".$jsarrindex."].status = 0;";
				$jstext .= "\nwords[".$jsarrindex."].word = \"".$word."\";";
				$jstext .= "\nwords[".$jsarrindex."].suggestions = new Array();";
				$jsarrindex2 = 0;
				foreach ($suggestionarray as $suggestion) {
					$jstext .= "\nwords[".$jsarrindex."].suggestions[".$jsarrindex2."] = \"".str_replace(array("\r", "\n"), array("", ""),$suggestion)."\";";
					$jsarrindex2++;
				}
				$jsarrindex++;

				// highlight in text
				$beforeword= substr($textarray[$lineindex],0,$position);
				$afterword= substr($textarray[$lineindex],$position+strlen($word));
				$textarray[$lineindex]= $beforeword."$opener$word$closer".$afterword;
				
				// dirty hack for recalculating next index position
				$poscorrect= $poscorrect+strlen("$opener$closer");
			} else {
				$poscorrect=0;
				$lineindex= $lineindex+1;
			}
		}
	}
	$toret = implode("\n", $textarray);
	//$toret = str_replace("\"", "&quot;", $toret);
	return array($toret, $jstext);
}

/*
 *	parses a string using the pspell php functions - multiple test cases for verification of correct text splitting
 */

function parseStringP($txt, $dialects) {
	global $errorstring, $parameters;
	$dialect = "british";
	if (isset($dialects[1]) && $dialects[1] != "") {
		$dialect = $dialects[1];
		$lang = $dialects[0];
	}
	//pspell parse initialisation
	//$psp_conf = pspell_config_create ("en", $dialect);
	$psp_conf = pspell_config_create ($lang);
	pspell_config_runtogether($psp_conf, false);
	pspell_config_mode($psp_conf, PSPELL_NORMAL);
	$psp = pspell_new_config($psp_conf);
	//join the string so that it is easier to parse - does not matter when formatting because it is html
	$txt = str_replace("\r\n", " ", $txt);
	$txt = str_replace("\r", " ", $txt);
	$txt = str_replace("\n", " ", $txt);
	$jstext = ""; $jsarrindex = 0;
	//split string by html tags
	$regexp = "#(<[^>]*>)#";
	$arr = preg_split($regexp, $txt, -1, PREG_SPLIT_OFFSET_CAPTURE|PREG_SPLIT_DELIM_CAPTURE);
	foreach ($arr as $comp) {
		$arr2[] = $comp[0];
	}
	//we now have the $arr2 - containing html tags and plain text portions
	
	for($i = 0; $i < count($arr2); $i++) {
		$portion = $arr2[$i]; //the part
		if (strstr($portion, "<a")) {//we have a link
			//$arr2[$i] = str_replace('href', 'href1', $arr2[$i]); //replace so when clicking on the content we do not redirect
		}
		if (!strstr($portion, "<") && $portion != "") { //we do not work on html portions and empty text - which should be resolved in the preg_split function
			$portion = str_replace("&nbsp;", " ", $portion); //replace for parsing
			//get distinct words from string portion
			$words = explode(" ", $portion);
			$arr2[$i] = $words;
			for ($j = 0; $j< count($arr2[$i]); $j++) {
				$word = $arr2[$i][$j];
				//we check if the "word" is really a word			
				$cond = true;
				//is it a link address ?
				if (strstr($word, "http://")) {
					$cond = false;
				}
				/*
				//is it a number?
				if (preg_match("/^[0-9\.%]*$/", $word)) {
					$cond = false;
				}
				//does it begin with a capital letter ?
				if (ucwords($word) == $word) {
					//$cond = false;
				}
				*/				
				if ($cond) {
					$regexp3 = "/([^a-zA-Z]*)([a-zA-Z']*)([^a-zA-Z]*)/";//get the actual words - we can have strings like ",something.". We separate in 3 portions
					preg_match($regexp3, $word, $mt);
					$beforeword = $mt[1];
					$inword = $mt[2];
					if (addedWord($inword)) {
						continue;
					}
					$afterword = $mt[3];
					if(!pspell_check($psp, $inword)) {
						$sugs = pspell_suggest($psp, $inword);
						if (count($sugs) > 0) {
							//build the option string
							//$arr2[$i][$j] = $beforeword.'<span onclick="fillOptions(this)" options="'.implode('|', $sugs).'" class="havesuggestion">'.$inword.'</span>'.$afterword;
							$arr2[$i][$j] = $beforeword.'<span badword=1>'.$inword.'</span>'.$afterword;

							$jstext .= "\nwords[".$jsarrindex."] = new Object();";
							$jstext .= "\nwords[".$jsarrindex."].status = 0;";
							$jstext .= "\nwords[".$jsarrindex."].word = \"".$word."\";";
							$jstext .= "\nwords[".$jsarrindex."].suggestions = new Array();";
							$jsarrindex2 = 0;
							foreach ($sugs as $suggestion) {
								$jstext .= "\nwords[".$jsarrindex."].suggestions[".$jsarrindex2."] = \"".str_replace(array("\r", "\n"), array("", ""),$suggestion)."\";";
								$jsarrindex2++;
							}
							$jsarrindex++;

						} else {
							//build the option string with red so that we know there is no replacement
							//$arr2[$i][$j] = $beforeword.'<span onclick="fillOptions(this)" options="'.implode('|', $sugs).'" class="nosuggestion">'.$inword.'</span>'.$afterword;
							$arr2[$i][$j] = $beforeword.'<span badword=1>'.$inword.'</span>'.$afterword;

							$jstext .= "\nwords[".$jsarrindex."] = new Object();";
							$jstext .= "\nwords[".$jsarrindex."].status = 0;";
							$jstext .= "\nwords[".$jsarrindex."].word = \"".$word."\";";
							$jstext .= "\nwords[".$jsarrindex."].suggestions = new Array();";
							$jsarrindex2 = 0;
							foreach ($sugs as $suggestion) {
								$jstext .= "\nwords[".$jsarrindex."].suggestions[".$jsarrindex2."] = \"".str_replace(array("\r", "\n"), array("", ""),$suggestion)."\";";
								$jsarrindex2++;
							}
							$jsarrindex++;

						}
					}
				}
			}
		}
	}
	//implosion of the parts to reconstitute the string
	$toret = "";
	foreach ($arr2 as $portion) {
		if (is_array($portion)) {
			$toret .= implode(" ", $portion);
		} else {
			$toret .= $portion;
		}
	}
	//$toret = str_replace("\"", "&quot;", $toret);
	return array($toret, $jstext);
}

$tmparr = array();

if (@$_POST['submitted'] != '') {
	//get dialects 
	//var_dump($HTTP_GET_VARS['dialect']);
	$tmparr = explode("|", $HTTP_GET_VARS['dialect']);
	$aspellhasdict = (int)$tmparr[2];
	$pspellhasdict = (int)$tmparr[3];
	if ($pspellhasdict) {
		$txt_corrected = parseStringP(stripslashes($_POST['htmltext']), $tmparr);	
		$havespell = true;		
	} else if ($aspellhasdict) {
		$txt_corrected = parseStringA(stripslashes($_POST['htmltext']), $tmparr);
		$havespell = true;
	} else {
		$havespell = false;	
		$txt_corrected[0] = $_POST['htmltext'];
		$txt_corrected[1] = "";
	}
}
?>
	<head>	
		<title>KTML <?php echo (isset($KT_Messages["Spellcheck"])) ? $KT_Messages["Spellcheck"] : "Spellcheck"; ?><?php if (@$tmparr[1]!= "") { ?> : <?php echo ucfirst($tmparr[1]) ?><?php } ?></title>
		<link href="../../styles/main.css" rel="stylesheet" type="text/css">
			<script>
			
			function openHelp(helpStr) {
				if (typeof dialogArguments != "undefined") {
					dialogArguments.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr, "../../../../../", "modules/spellchecker");
				} else {
					window.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr, "../../../../../", "modules/spellchecker");
				}
			}

			function loadContent(id) {
				if (window.opener) {
					var kt = window.opener.ktmls[id];
				} else {
					var kt = dialogArguments.ktmls[id];
				}
				if (!kt || !kt.edit || !kt.edit.body) {
					setTimeout("loadContent();", 300);
				}
				document.getElementById("htmltext").value = kt.edit.body.innerHTML;
			
				document.forms["form1"].submit();
			}
			
			var words = new Array();	
			<?php echo @$txt_corrected[1]; ?>
			
			function removeOptions() {
				var el = document.getElementById("sugg")
				for (i = 0; i<= el.options.length; i++) {
					el.options[i] = null;
				}
				el.options.length = 0;
			}
			
			function addOptions (arr) {
				var el = document.getElementById("sugg");
				for (i = 0; i< arr.length; i++) {
					if (arr[i] != "") {
						opt = new Option(arr[i], arr[i]);
						el.options[el.options.length] = opt;
					}
				}

				el.options.selectedIndex = 0;
	
				return true;
			}
	
			var innerStr = "";
			var wordArr = new Array();
			var kt = null;
			var currec = 0;
			var numCorrected = 0;

			function initPage() {
				if (window.opener) {
					kt = window.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>];
				} else {
					kt = dialogArguments.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>];
				}
				kt.edit.body.innerHTML = document.getElementById("innerStr").value;
				wordArrTmp = kt.edit.getElementsByTagName("SPAN");
				while (wordArr.length) {
					wordArr.pop();
				}
				for (var i = 0; i< wordArrTmp.length; i++) {
					if (wordArrTmp[i].getAttribute('badword') != null && wordArrTmp[i].getAttribute('badword') != "") {
						wordArr.push(wordArrTmp[i]);
					}
				}
				goToWord(0);
			}

			function fixWord(wordIndex, newVal){
				wordArr[wordIndex].innerHTML = newVal;
				var arr = new Array();
				arrtmp = kt.edit.getElementsByTagName("SPAN");
				for (var i = 0; i< arrtmp.length; i++) {
					if (wordArrTmp[i].getAttribute('badword') != null && arrtmp[i].getAttribute('badword') != "") {
						arr.push(arrtmp[i]);
					}
				}
				wordArr[wordIndex] = arr[wordIndex];
				numCorrected++;
			}
			
			function goToWord(idx){
				var oOption;
				if (idx >= words.length || currec >= words.length || words.length == 0 || numCorrected == words.length) {
					alert("<?php echo (isset($KT_Messages["Spellchecker complete"])) ? $KT_Messages["Spellchecker complete"] : "Speller Complete!"; ?>");
					if (window.opener) {
						kt.edit.body.innerHTML = window.opener.hndlr_load(kt.edit.body.innerHTML);
					} else {
						kt.edit.body.innerHTML = dialogArguments.hndlr_load(kt.edit.body.innerHTML);
					}
					//window.close();
				} else {
					if(words[idx].status != 0){
						goToWord(++currec);
						return;
					}
					if (window.addedWords && window.addedWords[words[idx].word]) {
						goToWord(++currec);
						return;
					}
					if (window.opener) {
						var _test = window.opener.Ktml_mozilla;
					} else {
						var _test = dialogArguments.Ktml_mozilla;
					}
					if (_test) {
						// this part select the current word in text
						var rng = kt.edit.createRange();
						var sel = kt.cw.getSelection();
						sel.collapseToStart();
						rng.selectNodeContents(wordArr[idx]);
						sel.removeAllRanges;
						sel.addRange(rng);
					} else {
						var rng = kt.edit.body.createTextRange();
						try {
							rng.moveToElementText(wordArr[idx]);
						} catch(e) {
							setTimeout("goToWord();", 100);
						}
						rng.select();
					}

					document.getElementById("notWord").value = words[idx].word;

					if(words[idx].suggestions.length > 0) {
						removeOptions();
						addOptions(words[idx].suggestions);
						if (words[idx].suggestions[0]  != "") { 
							document.getElementById("repWord").value = words[idx].suggestions[0];
							document.getElementById("sugg").disabled = false;
						} else {
							document.getElementById("repWord").value = words[idx].word;
							document.getElementById("repWord").select();
							document.getElementById("repWord").focus();
							document.getElementById("sugg").disabled = true;
						}
					}
				}
			}

			function selectSuggestion(obj){
				try {
				document.getElementById("repWord").value = obj.options[obj.options.selectedIndex].value;
				} catch(e) {
				}
			}

			function ignore(){
				//if there are no words (the text is corect)
				if (words.length ==0) {
					alert("<?php echo (isset($KT_Messages["No Words"])) ? $KT_Messages["No Words"] : "There are no words to add in the dictionary!"; ?>");
					return;
				}
				//if we have corected the test and we want to add the last word in the dictionary
				if (currec >= words.length) {
					alert("<?php echo (isset($KT_Messages["End of string"])) ? $KT_Messages["End of string"] : "You are allready at the end of the text!"; ?>");
					return;
				}
				words[currec].status=1;
				goToWord(++currec);
			}
			
			function ignoreAll(){
				//if there are no words (the text is corect)
				if (words.length ==0) {
					alert("<?php echo (isset($KT_Messages["No Words"])) ? $KT_Messages["No Words"] : "There are no words to add in the dictionary!"; ?>");
					return;
				}
				//if we have corected the test and we want to add the last word in the dictionary
				if (currec >= words.length) {
					alert("<?php echo (isset($KT_Messages["End of string"])) ? $KT_Messages["End of string"] : "You are allready at the end of the text!"; ?>");
					return;
				}
				var ic = currec;
				words[currec].status=1;
				for(i=currec;i<wordArr.length;i++){
					if(words[ic].word == words[i].word) words[i].status=1;
				}
				goToWord(++currec);
			}
			
			function addToDict() {
				//if there are no words (the text is corect)
				if (words.length ==0) {
					alert("<?php echo (isset($KT_Messages["No Words"])) ? $KT_Messages["No Words"] : "There are no words to add in the dictionary!"; ?>");
					return;
				}
				//if we have corected the test and we want to add the last word in the dictionary
				if (currec >= words.length) {
					alert("<?php echo (isset($KT_Messages["End of string"])) ? $KT_Messages["End of string"] : "You are allready at the end of the text!"; ?>");
					return;
				}
				
				if (confirm("<?php echo (isset($KT_Messages["Are you sure"])) ? $KT_Messages["Are you sure"] : "Are you sure you want to add the word to dictionary?"; ?>")) {
					var iframe = document.createElement('iframe');
					iframe.src = "addword.php?word=" + escape(words[currec].word);
					iframe.style.display = "none";
					document.body.appendChild(iframe);
					if (!window.addedWords) {
						window.addedWords = new Array();
					}
					window.addedWords[words[currec].word] = 1;
					goToWord(++currec);
				}
			}

			function isvalid(wrd){
				for(i=0;i<document.getElementById("sugg").options.length;i++) 
					if(document.getElementById("sugg").options[i].value==wrd) 
						return true;
				return false;
			}

			function change(){
				//if there are no words (the text is corect)
				if (words.length ==0) {
					alert("<?php echo $KT_Messages["No Words"]; ?>");
					return;
				}
				//if we have corected the test and we want to add the last word in the dictionary
				if (currec >= words.length) {
					alert("<?php echo $KT_Messages["End of string"]; ?>");
					return;
				}
				
				var newword = document.getElementById("repWord").value;
				words[currec].status=2;
				fixWord(currec, newword);
				goToWord(++currec);
			}

			function changeAll(){
				//if there are no words (the text is corect)
				if (words.length ==0) {
					alert("<?php echo $KT_Messages["No Words"]; ?>");
					return;
				}
				//if we have corected the test and we want to add the last word in the dictionary
				if (currec >= words.length) {
					alert("<?php echo $KT_Messages["End of string"]; ?>");
					return;
				}
				
				var ic = currec;
				var newword = document.getElementById("repWord").value;
				words[currec].status=2;
				for(i=ic;i<wordArr.length;i++){
					if(words[ic].word == words[i].word){
						words[i].status=2;
						fixWord(i, newword);
					}
				}
				goToWord(++currec);
			}
                        
            function unload() {
				var theKtml;
                <?php if (@$_POST['submitted'] != '') { ?>
                    if (window.opener) {
						theKtml = window.opener.ktmls[<?php echo $_GET['counter'] ?>];  
                        theKtml.spellcheck.wnd = null; 
                        if (!window.nospell) {
                            theKtml.edit.body.innerHTML = window.opener.hndlr_load(theKtml.edit.body.innerHTML); 
                        } 
						if(theKtml.undo) {
							theKtml.undo.addEdit();
						}
                    } else {
						theKtml = dialogArguments.ktmls[<?php echo $_GET['counter'] ?>];
                        theKtml.spellcheck.wnd = null;
                        theKtml.edit.body.innerHTML = dialogArguments.hndlr_load(theKtml.edit.body.innerHTML); 
						if(theKtml.undo) {
							theKtml.undo.addEdit();
						}
                    }
                <?php } ?>
            }
		</script>
	</head>
	<body onload="<?php if (@$_POST['submitted'] == '') { ?>loadContent(<?php echo $_GET['counter'] ?>);<?php } ?><?php if (@$_POST['submitted'] != '') { ?><?php if (!$havespell) { ?>//if (confirm('You do not have any speller interface installed!\nPress OK to open a technical note window with possible explanations to your problem. Press Cancel to close this window')) { var wnd = window.open('http://www.interakt.ro/products/technical_17.html', '', '');window.nospell = true; window.close() ; } else { window.nospell = true;window.close(); };<?php } else { ?>initPage();<?php } } ?>" 
	onunload="unload()" 
	class="body" 
	>

	<?php if (@$_POST['submitted'] == '') { ?>
		<?php 
			//die(var_dump($languagelist));
			$val = "";
			foreach($languagelist as $lang) {
				$isocode = $lang[0];
				$dictname = $lang[1];
				if ($lang[2] == $HTTP_GET_VARS['dialect']) {
					$testdic_aspell = testAspell($dictname)."";
					$testdic_pspell = testPspell($isocode)."";
					if ($testdic_pspell || $testdic_aspell) {
						$val = $lang[0]."|".$lang[1]."|".$testdic_aspell."|".$testdic_pspell;
					} else {
						$isocode = $languagelist[0][0];
						$dictname = $languagelist[0][1];
						$testdic_aspell = testAspell($dictname);
						$testdic_pspell = testPspell($isocode);
						$val = $languagelist[0][0]."|".$languagelist[0][1]."|".$testdic_aspell."|".$testdic_pspell;
					}
				}
			}
			?>	
		<form method="post" action="spellcheck.php?counter=<?php echo $_GET['counter'] ?>&dialect=<?php echo $val;?>" name="form1" id="form1" style="display:none">
			<input type="hidden" name="htmltext" id="htmltext" value="">
			<input type="hidden" name="submitted" value="submitted">
			<input type="hidden" name="pathToStyle" id="pathToStyle" value="">
			<input type="submit" name="action" value="">
		</form>
		<?php } else { 
					$dict = 0;
					foreach ($languagelist as $lang) {
						$isocode = $lang[0];
						$dictname = $lang[1];
						$testdic_aspell = testAspell($dictname);
						$testdic_pspell = testPspell($isocode);
						if ($testdic_pspell || $testdic_aspell) {
							$dict = 1;
						}
					}
			if($dict) {
			?>
			<table width="340" height="98%" border="0" cellpadding="0" cellspacing="0">
			<tr>
			  <td valign="top">
			  	<fieldset  class="ktml_fieldset">
					<legend class="ktml_legend"> <?php echo (isset($KT_Messages["Spellcheck"])) ? $KT_Messages["Spellcheck"] : "Spellcheck"; ?>
					<?php if ($tmparr[1]!= "") { ?> <?php echo (isset($KT_Messages["for"])) ? $KT_Messages["for"] : "for"; ?> <?php echo ucfirst($tmparr[1]) ?><?php } ?></legend>	
					<div>
					<label for="notWord" class="ktml_label"><?php echo (isset($KT_Messages["Spellchecker notinddict"])) ? $KT_Messages["Spellchecker notinddict"] : "Not in Dictionary"; ?>:</label>
					</div>
					<div><input type="text" id="notWord" style="width:200px;" disabled="" class="edit_txt"></div>
					<div style="margin-top:7px;">
					<label for="repWord" class="ktml_label"><?php echo (isset($KT_Messages["Spellchecker replace"])) ? $KT_Messages["Spellchecker replace"] : "Replace With"; ?>:</label>
					</div>
					<div><input type="text" id="repWord" style="width:200px;" class="ktml_input"></div>
					<div style="margin-top:7px;">
					<label for="sugg" class="ktml_label"><?php echo (isset($KT_Messages["Spellchecker suggestions"])) ? $KT_Messages["Spellchecker suggestions"] : "Suggestions"; ?>:</label>
					</div>
					<select id="sugg" size="5" style="width:200px;" onclick="selectSuggestion(this);" ondblclick="change()" class="edit_slc"></select>				  <div style="margin-top:7px;">
					<label for="spellcheck_dialect" class="ktml_label"><?php echo (isset($KT_Messages["Language:"])) ? $KT_Messages["Language:"] : "Language:"; ?> </label>
					<!-- 307 -->
					<select name="select" class="ktml_select" id="spellcheck_dialect"
						onchange="window.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']?>].spellcheck.setLanguage(this.options[this.selectedIndex].text,window, <?php echo $HTTP_GET_VARS['counter']?>);">
					<?php 
					foreach ($languagelist as $lang) {
						$isocode = $lang[0];
						$dictname = $lang[1];
						$testdic_aspell = testAspell($dictname);
						$testdic_pspell = testPspell($isocode);
						if ($testdic_pspell || $testdic_aspell) {
								$tmp = explode("|", $HTTP_GET_VARS['dialect']);
								$selected = "";
								if ($tmp[0] == $lang[0] && $tmp[1] == $lang[1])  {
									$selected = "selected";
								}
					?>
                    		<option isoname="<?php echo $lang[0]; ?>" value="<?php echo $isocode; ?>|<?php echo $dictname ?>|<?php echo $testdic_aspell; ?>|<?php echo $testdic_pspell; ?>" <?php echo $selected; ?>>
								<?php echo $lang[2]; ?>
							</option>
					<?php 	
						}
					} 
					?>
				</select>
                    </div>
				</fieldset>
			  </td>
				<td width="100" align="right" valign="top">
					<input type="button" onclick="ignore();" class="ktml_button" value="<?php echo (isset($KT_Messages["Spellchecker ignore"])) ? $KT_Messages["Spellchecker ignore"] : "Ignore"; ?>"/><br/>
					<input type="button" onclick="ignoreAll();" class="ktml_button" value="<?php echo (isset($KT_Messages["Spellchecker ignoreall"])) ? $KT_Messages["Spellchecker ignoreall"] : "Ignore All"; ?>"/><br>
					<input name="button" type="button" class="ktml_button" onclick="addToDict();" value="<?php echo (isset($KT_Messages["Spellchecker adddict"])) ? $KT_Messages["Spellchecker adddict"] : "Add to Dict."; ?>"/><br/>
					<input type="button" onclick="change();" class="ktml_button" style="margin-top:15px;" value="<?php echo (isset($KT_Messages["Spellchecker change"])) ? $KT_Messages["Spellchecker change"] : "Change"; ?>"/><br/>
					<input type="button" onclick="changeAll();" class="ktml_button" value="<?php echo (isset($KT_Messages["Spellchecker changeall"])) ? $KT_Messages["Spellchecker changeall"] : "Change All"; ?>"><br/>
					<input type="button" onclick="window.close();" class="ktml_button" style="margin-top:15px;" value="<?php echo (isset($KT_Messages["Spellchecker cancel"])) ? $KT_Messages["Spellchecker cancel"] : "Close"; ?>"/>
					<input type="button" onclick="openHelp('speller');" class="ktml_button" style="margin-top:15px;" value="<?php echo (isset($KT_Messages["Help"])) ? $KT_Messages["Help"] : "Help"; ?>"/>
			  </td>
			</tr>
	</table>
	<form>
		<input type="hidden" name="innerStr" id="innerStr" value="<?php echo htmlentities(@$txt_corrected[0]);?>">
	</form>
		<?php
			} else { // no dictionary
				echo '<table width="100%" height="98%"><tr><td><b>';
				echo (isset($KT_Messages["No Dictionary"])) ? $KT_Messages["No Dictionary"] : "There are no dictionaries found!<br/>Maybe there is no spell checking tool installed on the server.";
				echo '</b></td></tr><tr><td valign="bottom" align="center">';
				echo '<input type="button" onclick="window.close();" class="ktml_button" style="margin-top:15px;" value="'.((isset($KT_Messages["Spellchecker cancel"])) ? $KT_Messages["Spellchecker cancel"] : "Close").'"/></td></tr></table>';
			}
		?>

<?php
	} 
?>
</body>
</html>
