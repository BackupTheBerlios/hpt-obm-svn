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
 *@NAME:		generateCode
 *@PURPOSE:		To call each of the actions and display the output
 *@REMARKS:		This serves as the default action for the entire module
 *
 */
$post = $_POST;

import('action','generateVo');
import('action','generateDao');

//Remove empty column names
foreach($post['db_colname'] as $k => $v) {
	if($v == "") {
		//We have an empty entry, remove it
		unset($post['db_colname'][$k]);
		unset($post['vartype_db_varname'][$k]);
		unset($post['db_varname'][$k]);
		unset($post['vo_label'][$k]);
		$post['db_varcount']--;
	}
}
foreach( $post['db_varname'] as $k => $v )
{
	if( $v == "" )
	{
		$post['db_varname'][$k] = $post['db_colname'][$k];
	}

}
echo "<br>";
$tabtable = new tabtable('gen_type', 'DAO Generator', '600', '400');
$tabtable->add_tab('new', 'New DAO');
$tabtable->add_tab('sql', 'SQL Based');
$tabtable->add_tab('base', 'Base Classes');
$tabtable->print_head();
?>
<div align="center"><a href="index.php">[Start Over]</a></div>
<table border="0" cellpadding="5" cellspacing="0">
<tr>
	<td><h1>Data Access Object Code</h1>
	<br>Copy this code into a file named the same as the classname
	<br>(Value object is below)
	</td>
</tr>
<tr>
	<td>
	<?php
print "<pre>";
echo '<textarea name="dao" cols=80 rows=15>';
//print_r($post);
print htmlspecialchars("<?php\n");
$dao = generateDao($post['classname'],$post['tablename'],$post['pk_num'],$post['col_num'],$post['colname'],$post['varname'],$post['type_colname'],$post['ordering']);
print ($dao);
print htmlspecialchars("\n?>");
echo '</textarea>';
print "</pre>";
?>
	</td>
</tr>
<tr>
	<td><h1>Value Object Code</h1>
	<br>Copy this code into a file named the same as the classname
	</td>
</tr>
<tr>
	<td>
	<?php
print "<pre>";
print "<textarea name=\"valueObj\" cols=\"80\" rows=\"15\">";
print htmlspecialchars("<?php\n");
$vo = generateVo($post['classname'],$post['varname']);
print ($vo);
print htmlspecialchars("\n?>");
echo '</textarea>';
print "</pre>";
?>
	</td>
</tr>
</table>
<?php
$tabtable->print_foot();
?>
