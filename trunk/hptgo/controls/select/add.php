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

if ($_REQUEST['type'] == 'user')
{
	if(isset($_REQUEST['clicked_value']))
	{
		$user = $GO_USERS->get_user($_REQUEST['clicked_value']);
	}

	$middle_name = $user['middle_name'] == '' ? '' : $user['middle_name'].' ';
	$name_field_value = $user['last_name'].' '.$middle_name.$user['first_name'];
}else
{
	$GO_MODULES->authenticate('addressbook');

	require($GO_MODULES->class_path."addressbook.class.inc");

	$ab = new addressbook();

	if(isset($_REQUEST['clicked_value']))
	{
		$contact = $ab->get_contact($_REQUEST['clicked_value']);
	}

	$middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
	if (!empty($contact['company_name']))
		$name_field_value = $contact['last_name'].' '.$middle_name.$contact['first_name'].' / '.$contact['company_name'];
	else
		$name_field_value = $contact['last_name'].' '.$middle_name.$contact['first_name'];
}
unset($_SESSION['GO_HANDLER']);
?>

<html>
<body>
<script language="javascript" type="text/javascript">
	opener.<?php echo $_REQUEST['id_field']; ?>.value = '<?php echo $_REQUEST['clicked_value']; ?>';
	opener.<?php echo $_REQUEST['name_field']; ?>.value = "<?php echo $name_field_value; ?>";
	window.close();
</script>
</body>
</html>
