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
$GO_MODULES->authenticate('squirrelmail');
require($GO_LANGUAGE->get_language_file('squirrelmail'));
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
	$profile = $GO_USERS->get_user($GO_SECURITY->user_id);

  $reader = $profile['last_name'].
    ($profile['middle_name'] == '' ? ' ' : ' '.$profile['middle_name'].' ').
    $profile['first_name'];
	$body = $ml_displayed."\r\n".$reader." <".$profile["email"].">\r\n\r\n";
	$body .= $ml_subject.": ".$_POST['subject']."\r\n".$strDate.": ".$_POST['date'];

	if (!sendmail($_POST['email'], $profile["email"], $reader, $ml_notify, $body))
	{
		$feedback = '<p class="Error">'.$ml_send_error.'</p>';
	}else
	{
		echo "<script type=\"text/javascript\">\nwindow.close();\n</script>";
		exit;
	}
}else
{
	$pattern[0]="/(.*)</";
	$pattern[1]="/>/";
	$replace="";
	$email = preg_replace($pattern, $replace, $_REQUEST['notification']);
}

$page_title=$ml_notify;
require($GO_THEME->theme_path."header.inc");
?>
<form method="post" name="notify" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="email" value="<?php echo $email; ?>" />
<input type="hidden" name="date" value="<?php echo $_REQUEST['date']; ?>" />
<input type="hidden" name="subject" value="<?php echo $_REQUEST['subject']; ?>" />

<table border="0" cellspacing="0" cellpadding="5" align="center">
<tr>
	<td><img src="<?php echo $GO_THEME->images['questionmark']; ?>" border="0" /></td><td><h2><?php echo $ml_notify; ?></h2></td>
</tr>
</table>
<?php
if (isset($feedback))
{
	echo "<tr><td colspan=\"2\">".$feedback."</td></tr>\n";
}
?>
<table border="0" cellspacing="0" cellpadding="5" align="center">
<tr>
	<td colspan="2">
	<?php echo $ml_ask_notify; ?>
	</td>
</tr>
<tr>
	<td colspan="2" align="center">
	<?php
	$button = new button($cmdOk, "javascript:document.forms[0].submit()");
    echo '&nbsp;&nbsp;';
    $button = new button($cmdCancel, "javascript:window.close()");
	?>
	</td>
</tr>
</table>
</form>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
