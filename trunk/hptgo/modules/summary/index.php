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
$GO_MODULES->authenticate('summary');
require($GO_LANGUAGE->get_language_file('summary'));
require($GO_MODULES->class_path."announcements.class.inc");
$announcements = new announcements();

$link_back = $GO_MODULES->url;

//get the local times
$local_time = get_time();
$year = date("Y", $local_time);
$month = date("m", $local_time);
$day = date("j", $local_time);
$hour = date("H", $local_time);
$min = date("i", $local_time);

$GO_HEADER['auto_refresh']['interval'] = '60';
$GO_HEADER['auto_refresh']['url'] = $GO_MODULES->full_url;
$overlib = new overlib();
$GO_HEADER['head'] = $overlib->get_header();

require($GO_THEME->theme_path."header.inc");

echo 	'<form method="post" name="summary_form" action="'.$_SERVER['PHP_SELF'].'">'.
'<table border="0" width="100%" cellspacing="0" cellpadding="10"><tr><td width="50%">'.
'<h3>'.$sum_welcome_to.' '.$GO_CONFIG->title.'</h3></td><td width="50%" align="right"><h3>';
$date = date($_SESSION['GO_SESSION']['date_format']);
if($_SESSION['GO_SESSION']['date_format'] == 'd-m-Y')
{
  echo $full_days[date('w', $local_time)].' '.$day.' '.$months[$month-1];
}else
{
  echo $full_days[date('w', $local_time)].' '.$months[$month-1].' '.$day;
}
echo 	'</h3></td></tr>'.
'<tr><td valign="top" style="border: 1px solid black;border-left:0;">';
require('sum_email.inc');
require('sum_calendar.inc');
require('sum_notes.inc');

echo 	'&nbsp;</td><td valign="top" style="border: 1px solid black; border-left:0;border-right:0;">';
if ($GO_MODULES->write_permissions)
{
  echo '<div align="right"><a class="normal" href="announcements.php">'.$sum_manage_announcements.'</a></div>';
}

$announcements->get_announcements();

while($announcements->next_record())
{
  echo 	'<h1>'.htmlspecialchars($announcements->f('title')).'</h1>'.
    '<p>'.$announcements->f('content').'</p>';	
}

echo 	'&nbsp;</td></tr></table>'.
'</form>';

require($GO_THEME->theme_path."footer.inc");
?>
