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
//get the site properties and remember the site id
if(isset($_REQUEST['site_id']))
{
  $_SESSION['preview_site_id'] = $_REQUEST['site_id'];
  $site_id = $_REQUEST['site_id'];
}else
{
  if (isset($_SESSION['preview_site_id']))
  {
    $site_id = $_SESSION['preview_site_id'];
  }else
  {
    die('No site requested');
  }
}
if ($cms_module = $GO_MODULES->get_module('cms'))
{
  require('view.inc');
}else
{
  die('Failed to get Content Management Module');
}
?>
