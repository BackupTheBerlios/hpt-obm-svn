<?php
/*
Copyright Intermesh 2003
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 08 July 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/
require("../../Group-Office.php");

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('addressbook');

require($GO_MODULES->class_path."addressbook.class.inc");
$ab = new addressbook();

if (isset($_REQUEST['company_id']) && $_REQUEST['company_id'] > 0)
{
	if (isset($_REQUEST['clicked_value']) && $_REQUEST['clicked_value'] != '')
	{
		$contacts[] = $_REQUEST['clicked_value'];
	}else
	{
		$contacts = isset($_REQUEST['contacts']) ? $_REQUEST['contacts'] : array();
	}


	if (isset($contacts))
	{
		for($i=0;$i<sizeof($contacts);$i++)
		{
			$contact = $ab->get_contact($contacts[$i]);
			if ($GO_SECURITY->has_permission($GO_SECURITY->user_id,$contact['acl_write']))
				$ab->add_contact_to_company($contacts[$i], $_REQUEST['company_id']);
		}
	}
}
?>
<html>
<body>
<script language="javascript" type="text/javascript">
        opener.document.company_form.submit();
        window.close();
</script>
</body>
</html>
