<?php
/*
   Copyright HPT Commerce 2004
   Author: Tran Kien Duc <ductk@hptvietnam.com.vn>
   Version: 1.0 Release date: 25 August 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

//	$tabtable = new tabtable("config","Config","100%");
	$tabtable= new tabtable('active_tab', $strConfig, '100%', '400', '120', '', true);
	
	$tabtable->add_tab("edit_config.php",$contacts_contacts);
	$tabtable->add_tab("edit_config.php",$ab_companies);
	$tabtable->add_tab("edit_config.php",$contacts_members);

	if ($GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
		$tabtable->add_tab("categories.php",$strModifyCategories);
	
	$tabtable->set_active_tab( empty($_REQUEST['active_tab'])?0:(isset($_REQUEST['config']) ? $_REQUEST['config'] : null) );

	$tabtable->print_head();
	require($tabtable->get_active_tab_id());
	$tabtable->print_foot();
?>
