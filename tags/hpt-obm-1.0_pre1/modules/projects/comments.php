<?php
/*
   Copyright HPT Corporation 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 30 June 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require("../../Group-Office.php");

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('projects');
require($GO_LANGUAGE->get_language_file('projects'));

require($GO_THEME->theme_path."header.inc");
$comments = $_SESSION['GO_SESSION']['task_comments'];
$data = strtok($comments, "\xFF");

echo '<table border="0" width="100%" cellspacing="0" cellpadding="0" bgcolor="#BBBBBB">';
echo '<tr><td width="100%">';
echo '<table border="0" width="100%" cellspacing="1" cellpadding="0">';
while ($data) {
  if ($data != '') {
    list($time, $who, $text) = explode("|", $data);
    echo '<tr class="ProjectComment2"><td height="20"><i>('.$time.')</i>&nbsp;&nbsp;-&nbsp;&nbsp;'.
         '<b>'.$who.'</b></td></tr>';
    $text = str_replace("\n", '<br>', $text);
    echo '<tr class="ProjectComment1" height="20"><td>'.$text.'</td></tr>';
  }
  $data = strtok("\xFF");
}
echo '</table></td></tr>';
echo '</table>';

unset($_SESSION['GO_SESSION']['task_comments']);
require($GO_THEME->theme_path."footer.inc");
?>
