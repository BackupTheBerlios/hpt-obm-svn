<?php
	$preload_files = 'modules/products/classes/products.class.php';
	require("../../Group-Office.php");
	require('../../lib/tkdlib.php');

	$GO_SECURITY->authenticate();
	$GO_MODULES->authenticate('products');
	require($GO_LANGUAGE->get_language_file('products'));

	require($GO_THEME->theme_path."header.inc");
	
	$pro = new products();

	switch ($_REQUEST['task'])
	{
		case 'update':
			if ((int)$pro->update_attach_category($_POST['id'], $_POST['category']) == $pro->err_duplicate) 
				alert(sprintf($sc_Duplicate,$sc_category));
		break;
		case 'add':
			if ((int)$pro->add_attach_category($_POST['category']) == $pro->err_duplicate) 
				alert(sprintf($sc_Duplicate,$sc_category));
		break;
		case 'delete':
			if (!$pro->delete_attach_category($_POST['id'])) alert(sprintf($sc_CannotDelete,$_POST['txt_name']));
		break;
	}
	$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
	$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

	$pro->get_attach_categories(0);
/*	$dropbox = new dropbox();
	$dropbox->add_value(0,$sc_top);
	while($pro->next_record())
		$dropbox->add_value($pro->f('category_id'),$pro->f('category_name'));
*/
	require('templates/edit_attach_category.tmp.php');

	require($GO_THEME->theme_path."footer.inc"); 	
?>
