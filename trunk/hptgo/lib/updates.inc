<?php
// Current version is 1.01.
// The version is defined in Group-Office.php.
// You should not change the version without considering this file.
$updates[101][] = array();

// The first OBM version should be 1.00
// Older installs may use version 2.05 from Group-Office.
// You should modify database to 1.00 before do any update
$updates[100][] = "ALTER TABLE `users` ADD `enable` TINYINT(1) NOT NULL DEFAULT '0' AFTER `version`;";

?>
