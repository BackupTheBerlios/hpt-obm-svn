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

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$template_id = isset($_REQUEST['template_id']) ? $_REQUEST['template_id'] : 0;
if ($template_id > 0)
{
  $template = $cms->get_template($template_id);

  if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $template['acl_write']))
  {
    header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
    exit();
  }
}
//create a tab window
$tpl_table = new tabtable('template_tab',$cms_theme,'100%','100%', '100','', true);

switch($task)
{
  case 'save_template':
    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $name = trim($_POST['name']);
      $restrict_editor = isset($_POST['restrict_editor']) ? $_POST['restrict_editor'] : '';
      if ($name == '')
      {
	$feedback= '<p class="Error">'.$error_missing_field.'</p>';
      }else
      {
	if (isset($_FILES['additional_style_file']) && is_uploaded_file($_FILES['additional_style_file']['tmp_name']))
	{
	  $fp = fopen($_FILES['additional_style_file']['tmp_name'], 'r');
	  $additional_style = addslashes(fread($fp, $_FILES['additional_style_file']['size']));
	  fclose($fp);
	  unlink($_FILES['additional_style_file']['tmp_name']);
	}else
	{
	  $additional_style = smart_addslashes($_POST['additional_style']);
	}

	if (isset($_FILES['style_file']) && is_uploaded_file($_FILES['style_file']['tmp_name']))
	{
	  $fp = fopen($_FILES['style_file']['tmp_name'], 'r');
	  $style = addslashes(fread($fp, $_FILES['style_file']['size']));
	  fclose($fp);
	  unlink($_FILES['style_file']['tmp_name']);
	}else
	{
	  $style = smart_addslashes($_POST['style']);
	}

	if ($template_id > 0)
	{
	  $template = $cms->get_template_by_name($GO_SECURITY->user_id, $name);
	  if ($template && $template['id'] != $template_id)
	  {
	    $feedback = '<p class="Error">'.$fbNameExists.'</p>';
	  }else
	  {
	    if (!$cms->update_template($template_id, $name, $style, $additional_style, $restrict_editor))
	    {
	      $feedback = '<p class="Error">'.$strSaveError.'</p>';
	    }else
	    {
	      $template = $cms->get_template($template_id);
	    }
	  }
	}else
	{
	  if ($cms->get_template_by_name($GO_SECURITY->user_id, $name))
	  {
	    $feedback = '<p class="Error">'.$fbNameExists.'</p>';
	  }else
	  {
	    if (!$acl_read = $GO_SECURITY->get_new_acl('cms template read: '.$name))
	    {
	      die($strAclError);
	    }

	    if (!$acl_write = $GO_SECURITY->get_new_acl('cms template write: '.$name))
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

	    if(!$template_id = $cms->add_template($GO_SECURITY->user_id, $name, $style, $additional_style, $restrict_editor, $acl_read, $acl_write))
	    {
	      $GO_SECURITY->delete_acl($acl_read);
	      $GO_SECURITY->delete_acl($acl_write);
	      $feedback = '<p class="Error">'.$strSaveError.'</p>';
	    }else
	    {
	      $template = $cms->get_template($template_id);
	    }
	  }
	}
	if ($_POST['close'] == 'true')
	{
	  header('Location: index.php?tabindex=2');
	  exit();
	}

      }
    }
    break;

  case 'save_template_item':
    $task='template_item';
    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      $name = trim($_POST['name']);
      if ($name == '')
      {
	$feedback= '<p class="Error">'.$error_missing_field.'</p>';
      }else
      {
	if (isset($_FILES['content_file']) && is_uploaded_file($_FILES['content_file']['tmp_name']))
	{
	  $fp = fopen($_FILES['content_file']['tmp_name'], 'r');
	  $_POST['content'] = addslashes(fread($fp, $_FILES['content_file']['size']));
	  fclose($fp);
	  unlink($_FILES['content_file']['tmp_name']);
	}else
	{
	  $_POST['content'] = smart_addslashes($_POST['content']);
	}
	
	$content = get_html_body($_POST['content']);

	$template_item_id = isset($_POST['template_item_id']) ? $_POST['template_item_id'] : 0;

	if ($template_item_id > 0)
	{
	  if ($template_item = $cms->get_template_by_name($template_id, $name) && $template_item['id'] != $template_item_id)
	  {
	    $feedback = '<p class="Error">'.$fbNameExists.'</p>';
	  }else
	  {
	    if (!$cms->update_template_item($template_item_id, $name, $_POST['content']))
	    {
	      $feedback = '<p class="Error">'.$strSaveError.'</p>';
	    }
	  }
	}else
	{
	  if ($cms->get_template_item_by_name($template_id, $name))
	  {
	    $feedback = '<p class="Error">'.$fbNameExists.'</p>';
	  }else
	  {
	    if(!$template_item_id = $cms->add_template_item($template_id, $name, $_POST['content']))
	    {
	      $feedback = '<p class="Error">'.$strSaveError.'</p>';
	    }
	  }
	}
	if ($cms->get_template_items($template_id) == 1)
	{
	  $cms->set_main_template_item($template_id, $template_item_id);
	}
	if ($_POST['close'] == 'true')
	{
	  $task = '';
	}
      }
    }
    break;

  case 'upload':

    if ($_SERVER['REQUEST_METHOD'] == 'POST')
    {
	  $active_tab = 2;
      $task = 'list';
      if (isset($_FILES['file']))
      {

	for ($i=0;$i<count($_FILES['file']['tmp_name']);$i++)
	{
	  if (is_uploaded_file($_FILES['file']['tmp_name'][$i]))
	  {
	    $name = $_FILES['file']['name'][$i];
	    while ($cms->template_file_exists($template_id, $name))
	    {
	      $x++;
	      $name = strip_extension($_FILES['file']['name'][$i]).' ('.$x.').'.get_extension($_FILES['file']['name'][$i]);
	    }

	    $fp = fopen($_FILES['file']['tmp_name'][$i], 'r');
	    $content = addslashes(fread($fp, $_FILES['file']['size'][$i]));
	    fclose($fp);
	    unlink($_FILES['file']['tmp_name'][$i]);
	    $file_id = $cms->add_template_file($template_id, $name, $content);
	  }
	}
      }
    }
    break;

    case "files":
      $active_tab =2;
    break;
  case 'save_main_template_item':
    $cms->set_main_template_item($template_id, $_POST['main_template_item_id']);
    if ($_POST['close'] == 'true')
    {
      header('Location: index.php?tabindex=2');
      exit();
    }
    break;
}

//set the page title for the content file
$page_title = $lang_modules['cms'];

//require the content file. This will draw the logo's and the menu
require($GO_THEME->theme_path."header.inc");

?>
<form name="cms" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>" enctype="multipart/form-data">
<input type="hidden" name="template_id" value="<?php echo $template_id; ?>" />

  <?php
switch ($task)
{
  case 'upload':
    require('upload_template_file.inc');
    break;

  default:
    if ($template_id > 0)
    {
      $tpl_table->add_tab('template.inc', $strProperties);
      $tpl_table->add_tab('template_items.inc', $cms_templates);
      $tpl_table->add_tab('template_files.inc', $cms_files);
      $tpl_table->add_tab('template_read_permissions.inc', $strReadRights);
      $tpl_table->add_tab('template_write_permissions.inc', $strWriteRights);
	  if(isset($active_tab))
	  {
		$tpl_table->set_active_tab($active_tab);
	  }
      $tpl_table->print_head();
      require($tpl_table->get_active_tab_id());
    }else
    {
      $tpl_table->print_head();
      require('template.inc');
    }
    $tpl_table->print_foot();
    break;
}
?>

</form>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
