<?php
/*
*/

require("../../Group-Office.php");
require($GO_CONFIG->class_path.'email.class.inc');
$email = new email();
$email2 = new email();

$email->query("SELECT password, id FROM emAccounts");

while($email->next_record())
{
	$email2->query("UPDATE emAccounts SET password='".$GO_CRYPTO->encrypt($email->f('password'))."' WHERE id='".$email->f('id')."'");
}
?>