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

//load Group-Office
require("../../Group-Office.php");

//authenticate the user
$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('cms');

//load the CMS module class library
require($GO_MODULES->class_path.'cms.class.inc');
$cms = new cms();


//get the language file
require($GO_LANGUAGE->get_language_file('cms'));

//create a tab window
$tabtable = new tabtable('sites', $lang_modules['cms'], '100%', '400');
$tabtable->add_tab('subscribed.inc', $cms_your_sites);
$tabtable->add_tab('sites.inc', $cms_all_sites);

if ($GO_MODULES->write_permissions)
{
  $tabtable->add_tab('templates.inc', $cms_themes_menu);
  $tabtable->add_tab('configuration.inc', $menu_configuration);
}

//perform tasks before output to client
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

if(isset($_REQUEST['tabindex']))
{
  $tabtable->set_active_tab($_REQUEST['tabindex']);
}

switch($task)
{
  case 'sites':
    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $subscribed = isset($_POST['subscribed']) ? $_POST['subscribed'] : array();

      $cms->get_authorized_sites($GO_SECURITY->user_id);
      $cms2 = new cms();
      while ($cms->next_record())
      {
	$is_subscribed = $cms2->is_subscribed($GO_SECURITY->user_id, $cms->f('id'));
	$in_array = in_array($cms->f('id'), $subscribed);

	if ($is_subscribed && !$in_array)
	{
	  $cms2->unsubscribe_site($GO_SECURITY->user_id, $cms->f('id'));
	}

	if (!$is_subscribed && $in_array)
	{
	  $cms2->subscribe_site($GO_SECURITY->user_id, $cms->f('id'));
	}
      }
    }
    break;

  case 'configuration':
    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      if (isset($_POST['publish_path']))
      {
	$publish_path = smart_addslashes(trim($_POST['publish_path']));
	if (!is_writable($publish_path))
	{
	  $feedback ='<p class="Error">'.$cms_path_not_writable.'</p>';
	}else
	{
	  $publish_url = smart_addslashes(trim($_POST['publish_url']));
	  if (substr($publish_path, -1) != $GO_CONFIG->slash) $publish_path = $publish_path.$GO_CONFIG->slash;
	  if (!eregi('(^http[s]*:[/]+)(.*)', $publish_url))
	  {
	    $publish_url= "http://".$publish_url;
	  }
	  if (substr($publish_url, -1) != '/') $publish_url = $publish_url.'/';
	  $GO_CONFIG->save_setting('cms_publish_path', $publish_path);
	  $GO_CONFIG->save_setting('cms_publish_url', $publish_url);
	}
      }else
      {
	$GO_CONFIG->delete_setting('cms_publish_path');
	$GO_CONFIG->delete_setting('cms_publish_url');
      }
    }
    break;
}

//set the page title for the header file
$page_title = $lang_modules['cms'];

//require the header file. This will draw the logo's and the menu
require($GO_THEME->theme_path."header.inc");
?>
<form name="cms" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">

<?php
$tabtable->print_head();
require($tabtable->get_active_tab_id());
$tabtable->print_foot();
?>

</form>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
