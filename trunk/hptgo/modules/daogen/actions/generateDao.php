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
 *@NAME:        generateDao
 *@PURPOSE:     To take an array of input parameters and generate a php Data Access Object
 *@INPUT:       
 *				@param $classname The name of the Dao class
 *				@param $tablename The name of the database table to use with the Dao
 *				@param $pk_num Integer, number of primary keys
 *				@param $col_num Integer, number of columns
 *				@param $colname Array of Database column names 
 *				@param $varname Array of variable names
 *				@param $type_colname Datatype of colname rows
 *				@param $ordering Index of colname that is to be used with sql ORDER BY statments
 *@ACTION:      Generate the code in ASCII text and return it
 *@OUTPUT:      ASCII text of Vo code
 *@EXCEPTIONS:  None
 *@REMARKS:     None
 *
 */

function generateDao($classname,$tablename,$pk_num,$col_num,$colname,$varname,$type_colname,$ordering) {
	//We want to buffer all of this to return it
	ob_start();
?>
/**
  * <?php echo $classname;?> Data Access Object (DAO).
  * This class contains all database handling that is needed to
  * permanently store and retrieve <?php echo $classname;?>  object instances.
  */

/**
 * Replace this with an inclusion of the DAO parent class
 * Replace this with an inclusion of the associated Vo class
 */

class <?php echo $classname;?>Dao extends Dao
{

	/**
	 * populateVo-method.  This will take a database row and populate
	 * a Value Object with it's contents.
	 *
	 * @param row This is a row object from the db_mysql->query call
	 */
	function populateVo(&$row) {
		$vo =& new <?php echo $classname;?>Vo();
<?php
	for($i=0;$i<$col_num;$i++)
		printf("\t\t\$vo->%s = \$row->%s;\n",$varname[$i],$varname[$i]);
    ?>
		return $vo;
	}

	/**
	 * checkPrimaryKeys-method.  Checks to make sure primary keys of
	 * the VO are set and valid.
	 *
	 * @param Object vo Value Object to check
	 * @return Boolean var Clean variable
	 */
	function checkPrimaryKeys(&$vo) {
		$return = TRUE;
<?php
$b= "";
for($i=0;$i<$pk_num;$i++)
{
	echo "\n\t\t{$b}if (\$vo->{$varname[$i]} == \"\")
		{
			//print \"can not check existance without primary-key!\";
			trigger_error('Value Object Error'".'.$vo->toString'.");
			\$return FALSE;
		}";
	$b = "else";
}?>

		return $return;
	}

	/**
	 * selectAll-method. Creates 'SELECT * FROM db_table'
	 */
	function selectAll_SQLHOOK() {
		return "SELECT * FROM <?php echo $tablename;?> ";
	}

	/**
	 * where-method. Creates a SQL 'WHERE' clause for the table.
	 *
	 * @param Object vo  Value Object that contains the primary keys.
	 */
	function where_SQLHOOK(&$vo) 
	{
		$sql = "WHERE 1";
<?php
		for( $i = 0; $i < $pk_num ; $i ++ )
		{
			printf("\t\t\t".'$sql .= " AND %s = ".$this->sanitize($vo->%s);'."\n",$varname[$i],$varname[$i]);
		}
?>

		return $sql;
	}

	/**
	 * insert-method. Creates a SQL 'Insert' statement from the 
	 * supplied Value Object.
	 *
	 * @param Object vo  Value Object that contains the primary keys.
	 */
	function insert_SQLHOOK(&$vo) 
	{
		$sql = "INSERT INTO <?php echo $tablename;?> ( <?php 
$a = "";
for( $i = 0; $i < $col_num ; $i ++ )
{
	$a .= sprintf('%s, ',$varname[$i]);
}
$a = substr_replace ( $a, "", -2 );
echo $a;

?> ) values (";<?php
$a = "";
for( $i = 0; $i < $col_num ; $i ++ )
{
		$a .= sprintf("\n\t\t".'$sql .= $this->sanitize($vo->%s).", ";',$varname[$i]); 
}
$a = substr_replace ( $a, " )\";", -4 );
echo $a;
?>      

		return $sql;
	}

	/**
	 * update-method. This method creates a SQL 'Update' statement from
	 * the supplied Value Object.
	 *
	 * @param Object vo  Value Object that contains the primary keys.
	 */
	function update_SQLHOOK(&$vo) 
	{

		$sql = "UPDATE <?php echo $tablename;?> SET ";<?php
$a = "";
for( $i = 0; $i < $col_num ; $i ++ )
{
		$a .= sprintf("\n\t\t".'$sql .= "%s = ".$this->sanitize($vo->%s).", ";',$varname[$i],$varname[$i]); 
}
$a = substr_replace ( $a, ' ";', -4 );
print $a;
?>

		$sql .= "WHERE 1";<?php
for( $i = 0; $i < $pk_num ; $i ++ )
{
	printf("\n\t\t".'$sql .= " AND %s = ".$this->sanitize($vo->%s);',$varname[$i],$varname[$i]);
}?>		  

		return $sql;
	}


	/**
	 * delete-method. Creates a SQL 'Delete' statement;
	 */
	function delete_SQLHOOK() 
	{
		return "DELETE FROM <?php echo $tablename;?> ";
	}

	/**
	* coutAll-method. Creates a SQL SELECT special case that counts the number of rows.
	*/
	function countAll_SQLHOOK() 
	{
		return "SELECT count(<?php echo $varname[0];?>) AS count FROM <?php echo $tablename;?>";
	}

	/**
	 * searchMatching-Method. Creates a specialized SQL statment that searches
	 * based upon any value in the supplied Value Object.
	 *
	 * @param Object vo  Value Object that contains the primary keys.
	 */
	function searchMatching_SQLHOOK(&$vo) 
	{

		$sql = "SELECT * FROM <?php echo $tablename;?> WHERE 1 ";

<?php
$a = "";
for( $i = 0; $i < $col_num ; $i ++ )
{
	$op = '=';
	$mod = '';

	if( $type_colname[$i] == 'varchar' )
	{
		$op = 'LIKE';
		$mod = '%';
	}

	?>
		if ($vo-><?php echo $varname[$i]; ?> != "") 
		{
			$sql .= " AND <?php echo $varname[$i].' '.$op; ?> ".$this->sanitize('<?php echo $mod;?>'.$vo-><?php echo $varname[$i];?>.'<?php echo $mod;?>');
		}
<?php
}
?>     
		$sql .= " ORDER BY <?php echo $colname[$ordering] ?> ";

		return $sql;
	}
}
<?php

	$contents = ob_get_contents();
	ob_end_clean();
	return $contents;
}
?>
