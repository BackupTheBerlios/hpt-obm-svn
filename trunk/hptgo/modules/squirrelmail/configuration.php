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

require($GO_CONFIG->class_path."imap.class.inc");
require($GO_MODULES->class_path."email.class.inc");
require($GO_LANGUAGE->get_language_file('squirrelmail'));
$mail = new imap();
$email = new email();


$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

if ($task == 'save')
{
	$disable_accounts = isset($_POST['disable_accounts']) ? $_POST['disable_accounts'] : 'false';
	$GO_CONFIG->save_setting('em_disable_accounts', $disable_accounts);
	if ($_POST['close'] == 'true')
	{
		header('Location: '.$return_to);
		exit();
	}
}


require($GO_THEME->theme_path."header.inc");

echo '<form method="POST" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

$cfg_tab = new tabtable('configuration', $menu_configuration, '600','300');

$cfg_tab->print_head();
$disable_accounts_check = ($GO_CONFIG->get_setting('em_disable_accounts') == 'true') ? true : false;
echo '<br />';
$checkbox = new checkbox('disable_accounts', 'true', $ml_disable_accounts, $disable_accounts_check);
echo '<br /><br />';

$button = new button($cmdOk, "javascript:_save('true')");
echo '&nbsp;&nbsp;';
$button = new button($cmdApply, "javascript:_save('false')");
echo '&nbsp;&nbsp;';
$button = new button($cmdCancel, "javascript:document.location='".$return_to."';");
$cfg_tab->print_foot();
?>
<script type="text/javascript">
function _save(close)
{
	document.forms[0].close.value=close;
	document.forms[0].task.value='save';
	javascript:document.forms[0].submit();
}
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
