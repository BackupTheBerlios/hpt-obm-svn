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
require("../../Group-Office.php");

$cms_module = $GO_MODULES->get_module('cms');

require($GO_CONFIG->class_path.'filetypes.class.inc');
require($cms_module['class_path'].'cms.class.inc');
$cms = new cms();
$filetypes = new filetypes();

if ($file = $cms->get_template_file($_REQUEST['template_file_id']))
{
  $browser = detect_browser();

  header("Cache-Control: max-age=2592000\n");
  header("Content-Disposition: filename=".$file['name']."\n");
  header('Content-Type: '.$file['content_type']);
  header('Content-Transfer-Encoding: binary');
  echo $file['content'];
  exit();
}
?>
