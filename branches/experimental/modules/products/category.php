<?php
	$preload_files = 'modules/products/classes/products.class.php';
	require("../../Group-Office.php");
	require('../../lib/tkdlib.php');

	$GO_SECURITY->authenticate();
	$GO_MODULES->authenticate('products');
	require($GO_LANGUAGE->get_language_file('products'));

	require($GO_THEME->theme_path."header.inc");
	
	$pro = new products();
	$choice = new choice_list();
	
	switch ($_REQUEST['task'])
	{
		case 'update':
			$i = (int)$pro->update_category($_POST['id'], $_POST['category'], $_POST['parent_id'], $_POST['template_id']); 
			if ($i == $pro->err_duplicate) alert(sprintf($sc_Duplicate,$sc_category));
			if ($i == $pro->err_child_of_itsself) alert($sc_ChildOfItsself);
		break;
		case 'add':
			if ((int)$pro->add_category($_POST['category'],$_POST['parent_id'], $_POST['template_id']) == $pro->err_duplicate)
				alert(sprintf($sc_Duplicate,$sc_category));
		break;
		case 'delete':
			if (!$pro->delete_category($_POST['id'])) alert(sprintf($sc_CannotDelete,$_POST['txt_name']));
		break;
		case 'edit':
			$edit_id = $_POST['id'];
			$disabled = ($_POST['close_win']>0?'disabled':'');
			$pro->get_categories($edit_id,false);
			if ($pro->next_record())
			{
				$edit_name = $pro->f('category_name');
				$edit_parent = $pro->f('parent_id');
			}

			$pro->get_attach_categories($edit_id, true);
			while ($pro->next_record())
				$att_arr[] = $pro->f('id');
		break;
	}
	$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
	$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

	$pro->get_attach_categories();
	while ($pro->next_record())
	{
		$att_arr[] = -5;
		if (in_array($pro->f('id'),$att_arr))
			$choice->add_option($pro->f('name'),$pro->f('id'),true);
		else
			$choice->add_option($pro->f('name'),$pro->f('id'));
	}
	
	$pro->get_categories(0);
	$dropbox = new dropbox();
	$dropbox->add_value(0,$sc_top);
	while($pro->next_record())
		$dropbox->add_value($pro->f('category_id'),$pro->f('category_name'));

	require('templates/edit_category.tmp.php');

	require($GO_THEME->theme_path."footer.inc"); 		
?>
