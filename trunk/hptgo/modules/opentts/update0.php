<?php
include("modules/$name/configure.php");

$query[]=" ALTER TABLE `{$prefix}_users` CHANGE `deleted` `active` CHAR( 1 ) DEFAULT '1' NOT NULL ";
$query[]="update `{$prefix}_users` set active='1' where 1";

#$dbi=mysql_connect($dbhostname,$dbuname,$dbpasswd);
if ($dbi){
	#mysql_select_db($dbname);
	foreach($query as $sql_query){
		if (!$res=mysql_query($sql_query,$dbi))
			die("error: $sql_query");
		echo "$sql_query<br>";
	}
}

	
?>
