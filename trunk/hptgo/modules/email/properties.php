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
$GO_MODULES->authenticate('email');
require($GO_LANGUAGE->get_language_file('email'));
require($GO_CONFIG->class_path."imap.class.inc");
require($GO_MODULES->class_path."email.class.inc");
$mail = new imap();
$email = new email();

$account = $email->get_account($_REQUEST['account_id']);

if ($mail->open($account['host'], $account['type'], $account['port'], $account['username'], $GO_CRYPTO->decrypt($account['password']), $_REQUEST['mailbox']))
{
	$content = $mail->get_message($_REQUEST['uid']);
}
$page_title=$fbProperties;
require($GO_THEME->theme_path."header.inc");
?>
<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
	<td height="50">
	<h1><?php echo $fbProperties; ?></h1>
	</td>
</tr>
<tr>
	<td>
	<?php
		echo text_to_html($content["header"]);
	?>
	</td>

</table>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
