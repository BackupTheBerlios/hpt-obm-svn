<?php
$updates[11] = array();

$updates[10][] = "ALTER TABLE `emAccounts` ADD `use_ssl` ENUM( '0', '1' ) NOT NULL AFTER `port`;";
$updates[10][] = "ALTER TABLE `emAccounts` ADD `novalidate_cert` ENUM( '0', '1' ) NOT NULL AFTER `use_ssl`;";
$updates[10][] = "ALTER TABLE `emAccounts` ADD `draft` VARCHAR(100) NOT NULL DEFAULT '' AFTER `novalidate_cert`;";
?>
