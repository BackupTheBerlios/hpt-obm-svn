<?php
include("modules/$name/configure.php");

$query[]=" ALTER TABLE `{$prefix}_users` CHANGE `deleted` `active` CHAR( 1 ) DEFAULT '1' NOT NULL ";
$query[]="update `{$prefix}_users` set active='1' where 1";
$query[]=" ALTER TABLE `{$prefix}{$hlpdsk_prefix}_tickets` CHANGE `t_time` `post_date` VARCHAR( 12 ) NOT NULL ";
$query[]="ALTER TABLE `{$prefix}{$hlpdsk_prefix}_tickets` CHANGE `t_date` `due_date` VARCHAR( 12 ) NOT NULL ";
$query[]=" ALTER TABLE `{$prefix}{$hlpdsk_prefix}_tickets` CHANGE `t_est_time` `change_date` VARCHAR( 12 ) NOT NULL ";

#$dbi=mysql_connect($dbhostname,$dbuname,$dbpasswd);
if ($dbi){
	#mysql_select_db($dbname);
	foreach($query as $sql_query){
		if (!$res=mysql_query($sql_query,$dbi))
			die("error: $sql_query");
		echo "$sql_query<br>";
	}
	$querytickets="select ticket_number,post_date,due_date,change_date from {$prefix}{$hlpdsk_prefix}_tickets";
	echo "$querytickets<br>";
	if (!$res1=mysql_query($querytickets,$dbi))
		die("error: $querytickets");
	else{
		
		while($row=mysql_fetch_object($res1)){
			$query_update="update {$prefix}{$hlpdsk_prefix}_tickets set post_date='".strtotime($row->post_date)."', due_date='".strtotime($row->due_date)."',change_date='".strtotime($row->post_date)."' where ticket_number='".$row->ticket_number."'";
			echo "$query_update<br>";
			if(!$res2=mysql_query($query_update,$dbi))
				die("error: $sql_query");
		}	
	}
	$query_tasks="select task_id,post_date from {$prefix}{$hlpdsk_prefix}_tasks";
	echo "$query_tasks<br>";
	if (!$res3=mysql_query($query_tasks,$dbi))
                die("error: $sql_query");
        else{
                while($row=mysql_fetch_object($res3)){
                        $query_update="update {$prefix}{$hlpdsk_prefix}_tasks set post_date='".strtotime($row->post_date)."' where task_id='{$row->task_id}'";
			echo "$query_update<br>";
		        if(!$res4=mysql_query($query_update,$dbi))
	                        die("error: $sql_query");
                }
        }
	
}


	
?>
