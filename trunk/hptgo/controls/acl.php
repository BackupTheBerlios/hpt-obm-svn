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

require("../Group-Office.php");
$GO_SECURITY->authenticate();

if (isset($_REQUEST['search_field']))
{
	SetCookie("user_search_field",$_REQUEST['search_field'],time()+3600*24*365,"/","",0);
	$_COOKIE['user_search_field'] = $_REQUEST['search_field'];
}
$GO_HEADER['body_arguments'] = 'class="TableInside"';
require($GO_THEME->theme_path."header.inc");
echo '<table border="0" cellpadding="0" cellspacing="0" width="100%" height="100%"><tr><td class="TableInside" valign="top">';

if (isset($_REQUEST['project_acl']))
  require($GO_LANGUAGE->get_language_file('projects'));

$acl = new acl($_REQUEST['acl_id'],isset($_REQUEST['acl_table']) ? $_REQUEST['acl_table'] : 'acl');
echo '</td></tr></table>';
require($GO_THEME->theme_path."footer.inc");
?>
