<?php

session_start();
if (isset($filter_field)){
	foreach ($filter_field as $key=>$value){
		echo "filter $key ={$value}:{$filter_value[$key]}<br>";
	}
}
// Use $HTTP_SESSION_VARS with PHP 4.0.6 or less
$_SESSION['filter_field'][]="issuer_id";
$_SESSION['filter_value'][]="2";

session_unregister('filter_field');
session_unregister('filter_value');
?>
