<?php
	if(!isset($HTTP_SESSION_VARS["KTML2security"])) {
		die('You don\'t have credentials to access this part of the editor. Please click <a href=# onclick="window.close()">here</a> to close this window');
	}
	$KT_PATH_VAR = @$HTTP_SESSION_VARS["KTML2security"][$HTTP_GET_VARS['counter']]['path_'.@$HTTP_GET_VARS['submode']];
	if (isset($ALT_PATH)) {
		$KT_PATH_VAR = $ALT_PATH . $KT_PATH_VAR;
	}
	//the checks must be made with realpaths
	$minPath = realpath($KT_PATH_VAR);
	$realPath = realpath($KT_PATH_VAR . "/" . @$HTTP_GET_VARS['currentPath']);
	if ($HTTP_SESSION_VARS["KTML2security"][$HTTP_GET_VARS['counter']]['right_image'] == 1) {
		$secTest = strstr($realPath, $minPath);
	} else {
		$secTest = false;
	}
	$sessionTest = isset($HTTP_SESSION_VARS["KTML2security"]) && isset($HTTP_GET_VARS['counter']) && isset($HTTP_SESSION_VARS["KTML2security"][$HTTP_GET_VARS['counter']]);

	if (!$secTest) {
	  //echo "KT_PATH_VAR: $KT_PATH_VAR<br>";
	  //echo "minPath: $minPath<br>";
	  //echo "realPath: $realPath<br>";
	}
?>
