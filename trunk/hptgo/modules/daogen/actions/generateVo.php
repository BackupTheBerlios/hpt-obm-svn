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
 *@NAME:        generateVo
 *@PURPOSE:     To take an array of input parameters and generate a php Value Object
 *@INPUT:       
 *				@param $classname The name of the Vo class
 *				@param $varname An indexed array of the variable names for the Value Object
 *@ACTION:      Generate the code in ASCII text and return it
 *@OUTPUT:      ASCII text of Vo code
 *@EXCEPTIONS:  None
 *@REMARKS:     None
 *
 */

function generateVo($classname,$varname) {
	//We want to buffer all of this to return it
	ob_start();
?>
/**
* <?php echo $classname;?> Value Object.
* This class is a value object representing the database table. 
* It is intented to be used together with associated Dao object. 
*/

/**
 * Replace this with an inclusion of the parent valueObject class
 */

class <?php echo $classname;?>Vo extends valueObject {

	/** 
	 * Persistent Instance variables. This data is directly 
	 * mapped to the columns of database table.
	 */
<?php
	foreach( $varname as $v )
	{
		printf("\tvar $%s = '';\n",$v);
	}
	?>

	/** 
	 * Constructors. DaoGen generates two constructors by default.
	 * The first one takes no arguments and provides the most simple
	 * way to create object instance. The another one takes one
	 * argument, which is the primary key of the corresponding table.
	 */

	function  <?php echo $classname;?>Vo(<?php
	$a = "";
	foreach( $varname as $v )
	{
		$a .= sprintf("$%s = '', ",$v);
	}
	$a = substr_replace ( $a, "", -2 );
	echo $a;
	?>) 
	{
<?php
	foreach( $varname as $v )
	{
		printf("\t\t\$this->%s = $%s;\n",$v,$v);
	}
    ?>
	}


	/** 
	 * equals-method will compaire two vo instances
	 * and return true if they contain same values in all
	 * instance variables. If two objects are equal, it does
	 * not mean that they are the same instance. However it
	 * does mean that in that moment, they contain exactly 
	 * same data. (ie. they are mapped to same row in database.)
	 */
	function equals(&$vo) 
	{
<?php
	$a = "";
	$b = "";
	foreach( $varname as $v )
	{
		printf("\t\t%sif(\$this->%s != \$vo->%s)\n\t\t{\n\t\t\treturn FALSE;\n\t\t}\n",
				$b,$v,$v);
		$b = "else";
	}
	echo $a;
	?>
		return TRUE;
	}
}
<?php
	$contents = ob_get_contents();
	ob_end_clean();
	return $contents;
}
?>
