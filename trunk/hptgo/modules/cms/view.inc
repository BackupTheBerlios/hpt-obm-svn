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

$cms_site_module = $GO_MODULES->get_module('cms');

require($cms_site_module['class_path'].'cms.class.inc');
require($cms_site_module['class_path'].'cms_site.class.inc');

require_once($GO_LANGUAGE->get_language_file('cms'));

if(!isset($site_id))
{
  //site_id will be determnined in cms_site.class.inc by the domain.
  $site_id = 0;
}

/*
Get the domain name without this page eg.
http://www.test.com/test/index.php?file_id=0&folder_id=9 becomes:
www.test.com/test
 */
$domain = $_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
if (strpos($domain, basename($_SERVER['PHP_SELF'])) !== false)
{
  //chop off filename
  $pos = strrpos($domain,'/');
  if ($pos !== false)
  {
    $domain = substr($domain, 0, $pos);
  }
}

if(!$cms_site = new cms_site($site_id, $domain))
{
  die('No site or invalid site requested');
}

if ($cms_site->site['acl_read'] > 0)
{
  //authenticate the user
  $GO_SECURITY->authenticate();

  if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $cms_site->site['acl_read']) && 
      !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $cms_site->site['acl_write']))
  {
    require($GO_THEME->theme_path."header.inc");
    require($GO_CONFIG->root_path.'error_docs/403.inc');
    require($GO_THEME->theme_path."footer.inc");
    exit();
  }
}

$publish_path = $GO_CONFIG->get_setting('cms_publish_path');
if (isset($published) && $publish_path != '' && $cms_site->site['publish_style'] == '0')
{
  die($cms_site_offline);
}

//set the folder id we are in
$folder_id = isset($_REQUEST['folder_id']) ? $_REQUEST['folder_id'] : $cms_site->site['root_folder_id'];
$file_id =  isset($_REQUEST['file_id']) ? $_REQUEST['file_id'] : 0;


$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$search_word_id = isset($_REQUEST['search_word_id']) ? $_REQUEST['search_word_id'] : '';
echo $cms_site->generate_page($file_id, $task, $search_word_id);
?>
