<?php
	require("../../Group-Office.php");
		
	$GO_SECURITY->authenticate();
	$GO_MODULES->authenticate('products');
	require($GO_LANGUAGE->get_language_file('products'));

	$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
	$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
	$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

	require($GO_THEME->theme_path."header.inc");

	include('product.php');
	
	require($GO_THEME->theme_path."footer.inc"); 
?>
