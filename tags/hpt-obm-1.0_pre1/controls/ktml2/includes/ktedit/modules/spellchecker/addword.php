<?php
	@session_start();
	require_once('../../security.php');
	
// Copyright 2001-2004 Interakt Online. All rights reserved.
	
	$word = $HTTP_GET_VARS['word'];
	$f = fopen('addedWords.txt', 'a+');
	//flock($f);
	fwrite($f, "\n$word");
	//funlock($f);
	fclose($f);
?>