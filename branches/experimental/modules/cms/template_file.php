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

  //header('Content-Length: '.$file['size']);
  header('Expires: '.gmdate('D, d M Y H:i:s') . ' GMT');
  if ($browser['name'] == 'MSIE')
  {
  header('Content-Type: '.$file['content_type']);
    header('Content-Disposition: inline; filename="'.$file['name'].'"');
    header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
    header('Pragma: public');
  }else
  {
    header('Content-Type: '.$file['content_type']);
    header('Pragma: no-cache');
    header('Content-Disposition: inline; filename="'.$file['name'].'"');
  }
  header('Content-Transfer-Encoding: binary');
  echo $file['content'];
  exit();
}
?>
