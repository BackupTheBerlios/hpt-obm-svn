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

//Classname and tablename should come through $_POST
$db = new db();
$result = $db->metadata($post['tablename'],true);
//debug($result);
//exit;
for($i=0;$i<$result['num_fields'];$i++) {
	$post['colname'][$i] = $result[$i]['name'];
	$post['varname'][$i] = $result[$i]['name'];
	$post['type_colname'][$i] = $result[$i]['type'];
}
$post['col_num'] = $result['num_fields'];
$db->query("SHOW KEYS FROM ".$post['tablename']);
$post['pk_num'] = $db->nf();
$post['ordering'] = 0;

import('action','generateVo');
import('action','generateDao');

//Remove empty column names
foreach($post['colname'] as $k => $v) {
	if($v == "") {
		//We have an empty entry, remove it
		unset($post['colname'][$k]);
		unset($post['type_db_colname'][$k]);
		unset($post['varname'][$k]);
		$post['col_num']--;
	}
}
foreach( $post['varname'] as $k => $v )
{
	if( $v == "" )
	{
		$post['varname'][$k] = $post['colname'][$k];
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
	<td><h1>Value Object Code</h1>
	<br>Copy this code into a file named the same as the classname
	<br>(Data Access object is below)
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
<tr>
	<td><h1>Data Access Object Code</h1>
	<br>Copy this code into a file named the same as the classname
	</td>
</tr>
<tr>
	<td>
<?php
print "<pre>";
print "<textarea name=\"valueObj\" cols=\"80\" rows=\"15\">";
print htmlspecialchars("<?php\n");
$dao = generateDao($post['classname'],$post['tablename'],$post['pk_num'],$post['col_num'],$post['colname'],$post['varname'],$post['type_colname'],$post['ordering']);
print ($dao);
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
<br>
