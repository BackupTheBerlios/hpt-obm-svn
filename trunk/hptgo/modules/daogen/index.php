<?php
/*
Copyright T & M Web Enterprises 2003
Author: Mike Hostetler <mike@tm-web.com>
Version: 1.0 Release date: 01 November 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

/**
 *
 *@NAME:        index
 *@PURPOSE:		To show the index page
 *@REMARKS:     This serves as the default action for the entire module
 *
 */

echo '<div style="padding: 10px">';

$tabtable = new tabtable('gen_type', 'DAO Generator', '600', '400');

$tabtable->add_tab('new', 'New DAO');
$tabtable->add_tab('sql', 'SQL Based');
$tabtable->add_tab('base', 'Base Classes');
//$tabtable->add_tab('xul', 'XUL Interface (experimental)');

$tabtable->print_head();
switch($tabtable->get_active_tab_id())
{
        case 'new':
                newDaoForm();
        break;
        case 'sql':
                sqlDaoForm();
        break;
        case 'base':
                include('base.inc');
        break;
		/*
        case 'xul':
                showXulLauncher();
        break;
		*/
}
$tabtable->print_foot();
echo '</div>';

function showXulLauncher() {
	echo "XUL is an XML based language that is used by Mozilla to render it's user interface.  If you have a Mozilla browser, or a derivative of a mozilla browser, you will be able to view this experimental code";
}
function newDaoForm() {
?>

<div align="center">
<br>
<br>

<h1>Data Access Object Generator</h1>

<form action="fieldOptions.php" method="post">
  <table>
	<tr>
	  <td colspan="2" align="left">
		<br>
		<br>
		<font face="Verdana" size="2"><b>1. Select Basic Generator Options</b></font>
		<hr>
	  </td>
	</tr>

	<tr>
	  <td><b>Database Table Name:</b></td>

	  <td><input type="text" name="tablename" value="" size="20"> <em>(no spaces)</em></td>
	</tr>

	<tr>
	  <td>Class Name (optional):</td>

	  <td><input type="text" name="classname" value="" size="20"> <em>(no spaces)</em></td>
	</tr>

	<tr>
	  <td><b>Total number of Columns</b>:</td>

	  <td><input type="text" name="col_num" value="2" size="4"></td>
	</tr>

	<tr>
	  <td><b>Primarykey Columns</b> :</td>

	  <td><input type="text" name="pk_num" value="1" size="4"></td>
	</tr>

	<tr>
	  <td colspan="2" align="center">
		<hr>
		<input type="submit" name="continue" value="Continue">
	  </td>
	</tr>
  </table>
</form>
</div><?php
}

function sqlDaoForm() {
	$db = new db();
	$tables = $db->table_names();
	//debug($tables);
	$s = '<select name="tablename">';
	foreach($tables as $v) {
		$s .= '<option name="'.$v['table_name'].'">'.$v['table_name'].'</option>';
	}
	$s .= '</select>';
	?>
<div align="center">
<br>
<br>

<h1>Data Access Object Generator</h1>

<form action="sqlGenerateCode.php" method="post">
  <table>
	<tr>
	  <td><b>Database Table:</b></td>

	  <td><?php echo $s;?></td>
	</tr>

	<tr>
	  <td>Class Name (optional):</td>

	  <td><input type="text" name="classname" value="" size="20"> <em>(no spaces)</em></td>
	</tr>

	<tr>
	  <td colspan="2" align="center">
		<hr>
		<input type="submit" name="continue" value="Continue">
	  </td>
	</tr>
  </table>
</form>
</div>
	<?php
}
?>
