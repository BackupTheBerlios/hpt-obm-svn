<?php
//	$tabtable = new tabtable("config","Config","100%");
	$tabtable= new tabtable('active_tab', $strConfig, '100%', '400', '120', '', true);
	
	$tabtable->add_tab("edit_config.php",$contacts_contacts);
	$tabtable->add_tab("edit_config.php",$ab_companies);
	$tabtable->add_tab("edit_config.php",$contacts_members);

	if ($GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
		$tabtable->add_tab("categories.php",$strModifyCategories);
	
	$tabtable->set_active_tab( empty($_REQUEST['active_tab'])?0:$_REQUEST['config'] );

	$tabtable->print_head();
	require($tabtable->get_active_tab_id());
	$tabtable->print_foot();
?>