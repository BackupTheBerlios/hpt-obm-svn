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
 *@NAME:		fieldOptions
 *@PURPOSE:		To show the main page of the module
 *@REMARKS:		This serves as the default action for the entire module
 *
 */

$post = $_POST;

$tabtable = new tabtable('gen_type', 'DAO Generator', '600', '400');
$tabtable->add_tab('new', 'New DAO');
$tabtable->add_tab('sql', 'SQL Based');
$tabtable->add_tab('base', 'Base Classes');
$tabtable->print_head();
?>
<div align="center">
<table border="0" width="80%" cellspacing=0 cellpadding=4>
<form action="generateCode.php" method="POST">

<tr><td colspan="4" align="left"><br><br><font face="Verdana" size="2"><b>
Database Column Names and Types:</b></font></td></tr>

<tr><td colspan="4"><font face="Verdana" size="2"><br>
You must give the names for all columns in your database table. 
Wrong or missing names will make the generated code malfunction. 
You can also give preferred Variable Names for these columns, but 
they are optional. (Column names will be used as variable names, if 
no variable names are provided).</td></tr>

<?php
foreach( $post as $k => $v ) {
	echo '<input type="hidden" name="'.$k.'" value="'.$v.'">';
}

echo <<<HEADERTEXT
<tr><td colspan="4">&nbsp;</td></tr>
<tr><td align="center"><font face="Verdana" size="2">Type:</font></td>
<td align="center"><font face="Verdana" size="2">Column Name:</font></td>
<td align="center"><font face="Verdana" size="2">Variable Name:</font></td>
<td align="center"><font face="Verdana" size="2">Sort:</font></td>
HEADERTEXT;

// Primary Key DB columns
for( $i = 0; $i < $post['pk_num']; $i++ )
{
	printVarForm( "varname", "colname", $i, $i, "background-color:#ff5555", $message = "(Primary Key)",'vo_label' );
}

// DB columns:
for( $i = $i; $i < $post['col_num']; $i++ )
{
	printVarForm( "varname", "colname", $i, $i, "background-color:#f0f0ff",'','vo_label');
}

echo <<<FOOTERTEXT
	<tr>
		<td colspan="4" align="center">
			<br>
			<input type="submit" value="Generate Code">
		</td>
	</tr>

</form>
</table>
</div>
FOOTERTEXT;

function printVarForm( $varname, $colname, $index, $radio_val, $style, $message = "",$labname ) {

$radio_checked = (($radio_val == 0)?"checked":"");

$varname = $varname . '[' . $index . ']';
$colname = $colname . '[' . $index . ']';
echo <<<FORM
	<tr style="$style">
		<td >
			<select name="type_$colname">
			  <option value="varchar" selected>String</option>
			  <option value="Int">Integer</option>
			  <option value="Double">Double</option>
			  <option value="BigDecimal">Decimal</option>
			  <option value="DateTime">DateTime</option>
			  <option value="Date">Date</option>
			  <option value="Time">Time</option>
			  <option value="Timestamp">Timestamp</option>
			</select>
		</td>
		<td>
FORM;
	if( $colname != "" ) {
		echo '<input type="text" name="'.$colname.'" value="" size="30">';
	} 

echo <<<FORM
	<b>$message</b></td>

	<td><input type="text" name="$varname" value="" size="30"></td>
	<td>
FORM;

	if( $radio_val > -1 ) {
		echo '<input type="radio" name="ordering" value="'.$radio_val.'" '.$radio_checked.'>';
	}
	echo '</td></tr>';
}
$tabtable->print_foot();
?>
