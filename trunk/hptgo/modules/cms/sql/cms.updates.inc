<?php
$updates[12] = array();

$updates[11][] = "ALTER TABLE `cms_files` ADD INDEX ( `folder_id` );";

$updates[10][] = "ALTER TABLE `cms_sites` ADD `domain` varchar(100) NOT NULL default '';";
$updates[10][] = "ALTER TABLE `cms_sites` ADD `display_type` ENUM( '0', '1');";
$updates[10][] = "ALTER TABLE `cms_files` ADD `hot_item` ENUM( '0', '1');";
$updates[10][] = "ALTER TABLE `cms_folders` ADD `disable_multipage` ENUM( '0', '1');";
?>
