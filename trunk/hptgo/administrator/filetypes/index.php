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
$GO_SECURITY->authenticate(true);
require($GO_LANGUAGE->get_base_language_file('filetypes'));
require($GO_CONFIG->class_path."filetypes.class.inc");
$filetypes = new filetypes;

$return_to = $GO_CONFIG->host.'configuration/';

$page_title= $ft_title;
require($GO_THEME->theme_path."header.inc");

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
switch ($task)
{
  case 'filetype':
    require("filetype.inc");
    break;

  default:
    require("filetypes.inc");
    break;
}
require($GO_THEME->theme_path."footer.inc");
?>

