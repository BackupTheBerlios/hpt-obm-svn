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
if(isset($_REQUEST['delete_fee']))
{
	$projects->delete_fee($_REQUEST['delete_fee']);
}

$count = $projects->get_fees();
if ($GO_MODULES->write_permissions)
{
	echo '<a href="fee.php" class="normal">'.$pm_new_fee.'</a><br /><br />';
}
echo '<table border="0" cellpadding="4" cellspacing="0">';
echo '<tr><td><h3>'.$strName.'&nbsp;&nbsp;&nbsp;</h3></td>';
echo '<td><h3>'.$pm_value.'</h3></td><td colspan="2">&nbsp;</td></tr>';

while ($projects->next_record())
{
	echo '<tr><td>'.$projects->f('name').'&nbsp;&nbsp;&nbsp;</td><td>'.htmlentities($_SESSION['GO_SESSION']['currency']).'&nbsp;'.number_format($projects->f('value'), 2, $_SESSION['GO_SESSION']['decimal_seperator'],$_SESSION['GO_SESSION']['thousands_seperator']).'&nbsp;/&nbsp;'.$projects->f('time').'&nbsp;'.$pm_mins.'&nbsp;&nbsp;&nbsp;</td>';

	if ($GO_MODULES->write_permissions)
	{
		echo '<td><a href="fee.php?fee_id='.$projects->f('id').'" class="normal" title="'.
					$strEdit.' \''.$projects->f('name').'\'"><img src="'.$GO_THEME->images['edit'].
					'" border="0"></a></td>';

		echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF'].
					"?post_action=fees&delete_fee=".$projects->f("id")."\",\"".
					rawurlencode($strDeletePrefix.htmlspecialchars($projects->f("name")).
					$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".
					htmlspecialchars($projects->f("name")).
					"'\"><img src=\"".$GO_THEME->images['delete'].
					"\" border=\"0\"></a></td></tr>\n";
	}else
	{
		echo '<td colspan="2">&nbsp;</td></tr>';
	}
}
echo '<tr><td colspan="2"><i>'.$count.' '.$pm_fees.'</i></td></tr></table>';
?>