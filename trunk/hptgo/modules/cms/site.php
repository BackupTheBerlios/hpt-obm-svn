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

$site_id = isset($_REQUEST['site_id']) ? $_REQUEST['site_id'] : 0;
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';

$search_word_id = isset($_REQUEST['search_word_id']) ? $_REQUEST['search_word_id'] : 0;

$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : 'index.php';

$root_publish_path = $GO_CONFIG->get_setting('cms_publish_path');

if ($_SERVER['REQUEST_METHOD'] =='POST' && $task == 'save_site')
{
  $domain = $cms->prepare_domain(smart_addslashes(trim($_POST['domain'])));
  $name = smart_addslashes(trim($_POST['name']));  
  if ($domain == '' || $name == '')
  {
    $feedback= '<p class="Error">'.$error_missing_field.'</p>';
  }else
  {
    if ($_POST['site_id'] > 0)
    {
      if(!$site = $cms->get_site($_POST['site_id']))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
	$existing_site = $cms->get_site_by_domain($domain);
	if ($existing_site && $existing_site['id'] != $_POST['site_id'])
	{
	  $feedback = '<p class="Error">'.$cms_site_exists.'</p>';
	}else
	{
	  if (isset($_POST['secure']))
	  {
	    if ($site['acl_read'] == 0)
	    {
	      if (!$acl_read = $GO_SECURITY->get_new_acl('cms read: '.$domain))
	      {
		die($strAclError);
	      }
	    }else
	    {
	      $acl_read = $site['acl_read'];
	    }
	  }else
	  {
	    $acl_read = 0;
	    if($site['acl_read'] > 0)
	    {
	      $GO_SECURITY->delete_acl($site['acl_read']);
	    }
	  }

	  if (!$cms->update_site($site_id,
		$name,
		$domain,
		smart_addslashes($_POST['description']),
		smart_addslashes($_POST['keywords']),
		$_POST['template_id'], 
		$_POST['display_type'],
		$acl_read))
	  {
	    $feedback = '<p class="Error">'.$strSaveError.'</p>';
	  }else
		{
		    if($_POST['close'] == 'true')
    {
      header('Location: '.$return_to);
      exit();
    }
	}
		
	}
      }
    }else
    {
      if (!$cms->get_site_by_domain($domain))
      {
	if (isset($_POST['secure']))
	{
	  if (!$acl_read = $GO_SECURITY->get_new_acl('cms read: '.$domain))
	  {
	    die($strAclError);
	  }
	}else
	{
	  $acl_read = 0;
	}

	if (!$acl_write = $GO_SECURITY->get_new_acl('cms write: '.$domain))
	{
	  $GO_SECURITY->delete_acl($acl_read);
	  die($strAclError);
	}

	if (!$GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write))
	{
	  $GO_SECURITY->delete_acl($acl_read);
	  $GO_SECURITY->delete_acl($acl_write);
	  die($strAclError);
	}

	if($site_id = $cms->add_site($GO_SECURITY->user_id,
		  $name,
	      $domain,
	      $acl_read,
	      $acl_write,
	      smart_addslashes($_POST['description']),
	      smart_addslashes($_POST['keywords']),
	      $_POST['template_id'],
				$_POST['display_type']))
	{
	  $cms->subscribe_site($GO_SECURITY->user_id, $site_id);
	if($_POST['close'] == 'true')
    {
      header('Location: '.$return_to);
      exit();
    }

	}else
	{
	  $GO_SECURITY->delete_acl($acl_read);
	  $GO_SECURITY->delete_acl($acl_write);
	  $feedback = '<p class="Error">'.$strSaveError.'</p>';

	}
      }else
      {
	$feedback = '<p class="Error">'.$cms_site_exists.'</p>';
      }
    }

  }
}elseif($_SERVER['REQUEST_METHOD'] == 'POST' && $task == 'save_publish')
{
  $publish_path = smartstrip(trim($_POST['publish_path']));
  if ($publish_path == '')
  {
    $feedback = '<p class="Error">'.$error_missing_field.'</p>';
  }else
  {
    if (substr($publish_path,0,1) == $GO_CONFIG->slash) $publish_path = substr($publish_path,1);
    if (substr($publish_path, -1) != $GO_CONFIG->slash) $publish_path = $publish_path.$GO_CONFIG->slash;

    $existing_site = $cms->get_site_by_publish_path(addslashes($publish_path));

    if ($existing_site && $existing_site['id'] != $site_id)
    {
      $feedback = '<p class="Error">'.$cms_path_already_used.'</p>';
    }else
    {
      $full_publish_path = '';
      $dirs = explode($GO_CONFIG->slash, $publish_path);
      while($dir = array_shift($dirs))
      {
	$full_publish_path = $full_publish_path.$dir.$GO_CONFIG->slash;
	$existing_site = $cms->get_site_by_publish_path(addslashes($full_publish_path));
	if ($existing_site && $existing_site['id'] != $site_id)
	{
	  $path_used = true;
	  break;
	}
      }

      if (isset($path_used))
      {
	$feedback = '<p class="Error">'.$cms_path_already_used.'</p>';
      }else
      {
	require_once($GO_CONFIG->class_path.'filesystem.class.inc');
	$fs = new filesystem(true);

	$site = $cms->get_site($site_id);

	if ($site['publish_path'] != '' && file_exists($root_publish_path.$site['publish_path']) && $site['publish_path']  != $publish_path)
	{
	  $publish_dir = $root_publish_path;
	  $dirs = explode($GO_CONFIG->slash, $site['publish_path']);
	  while($dir = array_shift($dirs))
	  {
	    $publish_dir = $publish_dir.$dir.$GO_CONFIG->slash;
	    $fs->delete($publish_dir);
	  }
	}

	$full_publish_path = $root_publish_path;

	$dirs = explode($GO_CONFIG->slash, $publish_path);
	while($dir = array_shift($dirs))
	{
	  $full_publish_path = $full_publish_path.$dir.$GO_CONFIG->slash;
	  if (!file_exists($full_publish_path))
	  {
	    @mkdir($full_publish_path);
	  }
	  if (!is_writable($full_publish_path))
	  {
	    $feedback = '<p class="Error">'.$cms_path_not_writable.': \''.$full_publish_path.'\'</p>';
	    break;
	  }
	}
	if ($full_publish_path == $root_publish_path.$publish_path)
	{
	  $cms->set_publishing($site_id, $publish_style, addslashes($publish_path));
	  require($GO_CONFIG->class_path.'cms_site.class.inc');
	  $cms_site = new cms_site($site_id);
	  $cms_site->publish();

	  if ($close == 'true')
	  {
	    header('Location: '.$return_to);
	    exit();
	  }
	}
      }
    }
  }
}

if ($site_id > 0)
{
  //create a tab window
  $site = $cms->get_site($site_id);

  if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $site['acl_write']))
  {
    header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
    exit();
  }

  $tabtable = new tabtable('sites', htmlspecialchars($site['name'].' ('.$site['domain'].')'), '600', '400','100','',true);
  $tabtable->add_tab('properties', $strProperties);
  $tabtable->add_tab('search_words', $cms_search_words);

  if ($root_publish_path != '')
  {
    $tabtable->add_tab('publish', $cms_publish);
  }

  $tabtable->add_tab('write_permissions', $strWriteRights);

  if ($site['acl_read'] > 0)
  {
    $tabtable->add_tab('read_permissions', $strReadRights);
  }
  if ($task == 'edit_search_words')
  {
    $tabtable->set_active_tab(1);
  }
}else
{
  $tabtable = new tabtable('properties', $cms_new_site, '600', '400');
}

switch($task)
{
  case 'save_search_word':
  	if($_POST['close'] == 'false')
  	{
  		$task = 'add_search_word';
  	}
    $search_word_name = smart_addslashes(trim($_POST['search_word_name']));
    if ($search_word_name == '')
    {
      $feedback= '<p class="Error">'.$error_missing_field.'</p>';
      $task = 'add_search_word';
    }else
    {
      $search_word_id = isset($_POST['search_word_id']) ? $_POST['search_word_id'] : 0;
      if($search_word_id > 0)
      {
	$search_word = $cms->get_search_word_by_name($site_id, $search_word_name);
	if ($search_word && $search_word['id'] != $search_word_id)
	{
	  $feedback= '<p class="Error">'.$fbNameExists.'</p>';
	  $task = 'add_search_word';
	}else
	{
	  $cms->update_search_word($search_word_id, $search_word_name);
	}
      }else
      {
	if ($cms->get_search_word_by_name($site_id, $search_word_name))
	{
	  $feedback= '<p class="Error">'.$fbNameExists.'</p>';
	  $task = 'add_search_word';
	}else
	{
	  if (!$search_word_id = $cms->add_search_word($site_id, $search_word_name))
	  {
	    $feedback = '<p class="Error">'.$strSaveError.'</p>';
	  }else
	  {
	    $cms->search_files($site['root_folder_id'], $search_word_id);
	    $feedback = "<p>".$cms_search_files_prefix." '".$search_word_name."' ".$cms_search_files_suffix."</p>";
	  }
	}
      }

      if ($search_word_id > 0 && isset($_POST['files']))
      {
	$selected_files = isset($_POST['selected_files']) ? $_POST['selected_files'] : array();
	while ($file_id = array_shift($_POST['files']))
	{
	  $selected = in_array($file_id, $selected_files);
	  $attached = $cms->file_is_attached($file_id, $search_word_id);
	  if ($selected && !$attached)
	  {
	    $cms->attach_file($file_id, $search_word_id);
	  }

	  if ($attached && !$selected)
	  {
	    $cms->detach_file($file_id, $search_word_id);
	  }
	}
      }
    }
    break;

  case 'delete_search_word':
    $cms->delete_search_word($_REQUEST['search_word_id']);
    break;

  case 'search_files':
    $site = $cms->get_site($site_id);
    $task = 'add_search_word';
    $cms->search_files($site['root_folder_id'], $_POST['search_word_id']);
    $feedback = "<p>".$cms_search_files_prefix." '".$_POST['search_word_name']."' ".$cms_search_files_suffix."</p>";
    break;
}

//set the page title for the header file
$page_title = $lang_modules['cms'];

//require the header file. This will draw the logo's and the menu
require($GO_THEME->theme_path."header.inc");
echo '<form name="cms" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="site_id" value="'.$site_id.'" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';

$tabtable->print_head();

switch($tabtable->get_active_tab_id())
{
  case 'search_words':
    require('site_search_words.inc');
    break;

  case 'publish':
    require('publish.inc');
    break;

  case 'write_permissions':
    echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td>';
    $read_only = ($site['user_id'] == $GO_SECURITY->user_id) ? false : true;
    print_acl($site["acl_write"], $read_only);
    echo '</td></tr></table><br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;

  case 'read_permissions':
    echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td>';
    $read_only = ($site['user_id'] == $GO_SECURITY->user_id) ? false : true;
    print_acl($site["acl_read"], $read_only);
    echo '</td></tr></table><br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;

  default:
    if($site_id > 0)
    {
	  	$name = $site['name'];
      $domain = $site['domain'];
      $description = $site['description'];
      $keywords = $site['keywords'];
      $template_id = $site['template_id'];
      $secure_check = ($site['acl_read'] > 0) ? true : false;
      $display_type = $site['display_type'];
    }else
    {
	 		$name = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
      $domain = isset($_POST['domain']) ? smartstrip($_POST['domain']) : '';
      $description = isset($_POST['description']) ? smartstrip($_POST['description']) : '';
      $keywords = isset($_POST['keywords']) ? smartstrip($_POST['keywords']) : '';
      $secure_check = isset($_POST['secure']) ? true : false;
      $template_id = isset($_POST['template_id']) ? $_POST['template_id'] : '';
      $display_type = isset($_POST['display_type']) ? $_POST['display_type'] : NORMAL_DISPLAY;
    }

    if ($cms->get_templates() == 0)
    {
      echo '<br />';
      echo $cms_no_themes;
      echo '<br /><br />';
      $button = new button($cmdOk, "javascript:document.location='".$GO_MODULES->url."index.php?tabindex=2';");
    }else
    {
      ?>
	<input type="hidden" name="task" />
	<br />
	<table border="0" cellpadding="4" cellspacing="0">
	<?php
	if(isset($feedback)) echo '<tr><td colspan="2">'.$feedback.'&nbsp;</td></tr>';

      /*if ($site_id > 0)
      {
	echo '<tr><td>'.$cms_id.':</td><td>'.$site_id.'</td></tr>';
      }*/
      ?>

	<tr>
	<td>
	<?php echo $cms_domain; ?>:
	</td>
	<td>
	<input type="text" class="textbox" name="domain" value="<?php echo htmlspecialchars($domain); ?>" maxlength="100" style="width: 250" />
	</td>
	</tr>
	<tr>
	<td>
	<?php echo $strName; ?>:
	</td>
	<td>
	<input type="text" class="textbox" name="name" value="<?php echo htmlspecialchars($name); ?>" maxlength="100" style="width: 250" />
	</td>
	</tr>
	<tr>
	<td valign="top">
	<?php echo $strDescription; ?>:
	</td>
	<td>
	<textarea class="textbox" name="description" style="width: 250" rows="5"><?php echo htmlspecialchars($description); ?></textarea>
	</td>
	</tr>
	<tr>
	<td valign="top">
	<?php echo $cms_keywords; ?>:
	</td>
	<td>
	<textarea class="textbox" name="keywords" style="width: 250" rows="5"><?php echo htmlspecialchars($keywords); ?></textarea>
	</td>
	</tr>
	<tr>
	<td>
	<?php echo $cms_theme; ?>:
	</td>
	<td>
	<?php

	$dropbox=new dropbox();

      while ($cms->next_record())
      {
	if ((isset($site) && $cms->f('id') == $site['template_id']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $cms->f('acl_read')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $cms->f('acl_write')))
	{
	  $dropbox->add_value($cms->f('id'), $cms->f('name'));
	}
      }
      $dropbox->print_dropbox('template_id', $template_id);
      ?>
	</td>
	</tr>
	<tr>
	<td colspan="2">
	<?php
	$checkbox = new checkbox('secure', 'true', $cms_use_go_auth, $secure_check);
  ?>
	</td>
	</tr>
	
	<tr>
	<td colspan="2">
	<?php
	echo '<br />'.$cms_display_type.'<br />';
	$radio_list = new radio_list('display_type', $display_type);
	$radio_list->add_option(NORMAL_DISPLAY, $cms_normal_display);
	echo '<br />';
	$radio_list->add_option(MULTIPAGE_DISPLAY, $cms_multipage_display);	
      ?>
	</td>
	</tr>
	<tr>
	<td colspan="2">
	<br />
	<?php
	$button = new button($cmdOk, "javascript:save_close_site()");
      echo '&nbsp;&nbsp;';
      $button = new button($cmdApply, "javascript:save_site()");
      echo '&nbsp;&nbsp;';
      $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
      ?>
	</td>
	</tr>
	</table>
	<script type="text/javascript">
	function save_close_site()
	{
	  document.forms[0].close.value='true';
	  document.forms[0].task.value='save_site';
	  document.forms[0].submit();
	}

      function save_site()
      {
	document.forms[0].task.value='save_site';
	document.forms[0].submit();
      }
      document.forms[0].domain.focus();
      </script>
	<?php

    }
    break;
}

$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
