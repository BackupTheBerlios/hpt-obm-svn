<?php
/*
Copyright Intermesh 2003
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 08 July 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

require("../Group-Office.php");

//load filetypes management class
require($GO_CONFIG->class_path."filetypes.class.inc");
$filetypes = new filetypes();
$mime = isset($_REQUEST['mime']) ? $_REQUEST['mime'] : '';
if(!$filetype = $filetypes->get_type($_REQUEST['extension'], true))
{
	$filetype = $filetypes->add_type($_REQUEST['extension'], $mime,'','',true);
}

header("Cache-Control: max-age=2592000\n");
header("Content-type: image/gif\n");
header("Content-Disposition: filename=".$filetype['extension'].".gif\n");
header("Content-Transfer-Encoding: binary\n");
echo $filetype['image'];
?>
