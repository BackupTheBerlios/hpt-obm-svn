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

$ab_module = $GO_MODULES->get_module('addressbook');

//TODO: Check if user has access to addressbook.

require($ab_module['class_path']."addressbook.class.inc");
$ab = new addressbook();

if(isset($_REQUEST['clicked_contact']))
{
	$contact = $ab->get_contact($_REQUEST['clicked_contact']);
}
unset($_SESSION['GO_HANDLER']);
?>
<html>
<body>
<script language="javascript" type="text/javascript">
	//opener.document.forms[0].contact_id.value = '<?php echo $contact['id']; ?>';
	//opener.document.forms[0].contact_name.value = "<?php echo $contact['name']; ?>";
	opener.document.forms[0].t_from.value = "<?php echo $contact['name']; ?>";
	//opener.document.forms[0].contact_name_text.value = "<?php echo $contact['name']; ?>";
	window.close();
</script>
</body>
</html>
