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

//authenticate the user
$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('cms');

require($GO_MODULES->class_path.'cms.class.inc');
require_once($GO_CONFIG->class_path.'filetypes.class.inc');
$filetypes = new filetypes();
$cms = new cms();

//get the language file
require($GO_LANGUAGE->get_language_file('cms'));

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$file_id = isset($_REQUEST['file_id']) ? $_REQUEST['file_id'] : 0;
$folder_id = isset($_REQUEST['folder_id']) ? $_REQUEST['folder_id'] : 0;
$site_id = isset($_REQUEST['site_id']) ? $_REQUEST['site_id'] : 0;

if ($folder_id == 0 || $site_id == 0)
{
  //no folder or site given so back off cowardly
  header('Location: index.php');
  exit();
}

if($task=='save')
{
  if ($file_id > 0)
  {
    //fix for inserted iframes
    $content = preg_replace("'<iframe([^>]*)/>'si", "<iframe$1></iframe>", $_POST['content']);
    $cms->update_file($file_id,
	smart_addslashes($_POST['name']),
	smart_addslashes($content),
	smart_addslashes($_POST['title']),
	smart_addslashes($_POST['description']),
	smart_addslashes($_POST['keywords']),
	$_POST['priority']);
  }else
  {
    $name = smart_addslashes(trim($_POST['name']));
    if ($name == '')
    {
      $feedback = '<p class="Error">'.$error_missing_field.'</p>';
    }else
    {
      $filename = $name.'.html';

      if($cms->file_exists($folder_id, $filename))
      {
	$feedback = '<p class="Error">'.$fbNameExists.'</p>';
      }elseif(!$file_id = $cms->add_file($folder_id,
	    $filename,
	    smart_addslashes($_POST['content']),
	    '', '', '', $_POST['priority']))
      {
	$feedback = '<p class="Error">'.$strSaveError.'</p>';
      }
    }
  }
}

if ($file_id > 0)
{
  $file = $cms->get_file($file_id);
  $content = $file['content'];
  $name= $file['name'];
  $title = $file['title'];
  $description = $file['description'];
  $keywords = $file['keywords'];
  $priority = $file['priority'];
}else
{
  require($GO_THEME->theme_path."header.inc");
  require("add_file.inc");
  require($GO_THEME->theme_path."footer.inc");
  exit();
}

$link_back = 'edit.php?site_id='.$site_id.'&file_id='.$file_id.'&folder_id='.$folder_id;

if ($task == 'clean_formatting')
{
  $content = $cms->clean_up_html($content);
}

//set the page title for the header file
$page_title = $lang_modules['cms'];

//get the site template
if($site = $cms->get_site($site_id))
{
  $template = $cms->get_template($site['template_id']);
}

$pagestyle = (isset($template)) ? $template['style'] : '';
$pagestyle = str_replace("\r", '', $pagestyle);
$pagestyle = str_replace("\n", '', $pagestyle);
$pagestyle = str_replace("\t", '', $pagestyle);
$pagestyle = str_replace("'", '"', $pagestyle);
$pagestyle = str_replace(' ', '', $pagestyle);

//create htmlarea
$htmlarea = new htmlarea();
$htmlarea->add_button('insert', $cms_insert_file, $GO_CONFIG->control_url.'htmlarea/images/go_image.gif', 'false', "function insertObject()
    {
    popup('select.php?site_id=".$site_id."', '600', '400');
    }");
$qn_plugin = $GO_MODULES->get_plugin('questionnaires');
if($qn_plugin)
{
	$htmlarea->add_button('insert_qn', 'Vragenlijst invoegen', $GO_CONFIG->control_url.'htmlarea/images/questionnaire.gif', 'false', "function insertQN()
    {
    popup('questionnaires/select.php?site_id=".$site_id."', '300', '400');
    }");
	
}
$com_plugin = $GO_MODULES->get_plugin('components');
if($com_plugin)
{
	$htmlarea->add_button('insert_reg', 'Insert component', $GO_CONFIG->control_url.'htmlarea/images/component.gif', 'false', "function insert_component()
    {
    popup('".$com_plugin['url']."select.php?site_id=".$site_id."', '300', '400');
    }");
	
}

$GO_HEADER['head'] = $htmlarea->get_header('content', -70, -125, 25, $pagestyle, "config.baseURL='';", true);
$GO_HEADER['body_arguments'] = 'onload="initEditor()"';

//require the header file. This will draw the logo's and the menu
require($GO_THEME->theme_path."header.inc");

//echo '<table border="0" cellpadding="10" cellspacing="0"><tr><td>';
if (isset($feedback)) echo $feedback;
echo '<form method="post" name="editor" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="site_id" value="'.$site_id.'" />';
echo '<input type="hidden" name="file_id" value="'.$file_id.'" />';
echo '<input type="hidden" name="folder_id" value="'.$folder_id.'" />';
echo '<input type="hidden" name="old_name" value="'.htmlspecialchars($name).'" />';
echo '<input type="hidden" name="title" value="'.htmlspecialchars($title).'" />';
echo '<input type="hidden" name="description" value="'.htmlspecialchars($description).'" />';
echo '<input type="hidden" name="keywords" value="'.htmlspecialchars($keywords).'" />';
echo '<input type="hidden" name="priority" value="'.$priority.'" />';
echo '<input type="hidden" name="unedited" value = "" />';
echo '<input type="hidden" name="task" value="save" />';
echo '<input type="hidden" name="name" value="'.htmlspecialchars($name).'" />';

echo '<table border="0">';
echo '<td class="ModuleIcons" nowrap>';
echo '<a href="javascript:document.editor.onsubmit();document.editor.submit();"><img src="'.$GO_THEME->images['save_big'].'" border="0" height="32" width="32" /><br />'.$cmdSave.'</a></td>';
if ($file_id >0)
{
  echo '<td class="ModuleIcons"  nowrap>';
  echo '<a class="small" href="properties.php?site_id='.$site_id.'&task=file_properties&file_id='.$file_id.'&return_to='.urlencode($link_back).'&folder_id='.$folder_id.'"><img src="'.$GO_THEME->images['properties'].'" border="0" height="32" width="32" /><br />'.$fbProperties.'</a></td>';

  echo '<td class="ModuleIcons" nowrap>';
  echo '<a class="small" target="_blank" href="view.php?site_id='.$site_id.'&folder_id='.$folder_id.'&file_id='.$file_id.'" title="'.$cms_preview.' \''.$name.'\'"><img src="'.$GO_THEME->images['magnifier_big'].'" border="0" /><br />'.$cms_preview.'</a></td>';
}
echo '<td class="ModuleIcons"  nowrap>';
echo '<a class="small" href="javascript:confirm_close(\''.$GO_MODULES->url.'browse.php?site_id='.$site_id.'&folder_id='.$folder_id.'\')"><img src="'.$GO_THEME->images['close'].'" border="0" height="32" width="32" /><br />'.$cmdClose.'</a></td>';


/*echo '<td class="ModuleIcons" nowrap>';
echo '<a class="small" href=\'javascript:clean_formatting()\'><img src="'.$GO_THEME->images['filters'].'" border="0" height="32" width="32" /><br />Cleanup</a></td>';*/
echo '</table>';

$tabtable = new tabtable('cms_edit', htmlspecialchars($name));
$tabtable->print_head();
$htmlarea->print_htmlarea(htmlspecialchars($content));
$tabtable->print_foot();

echo '</form>';
?>
  <script type="text/javascript">
function clean_formatting()
{
  document.forms[0].task.value='clean_formatting';
  document.forms[0].onsubmit();
  document.forms[0].submit();
}

function confirm_close(URL)
{
  //TODO: detect if content has been changed
  //if (confirm('<?php echo $cms_confirm_close; ?>'))
  //{
  document.location=URL;
  //}
}

</script>

<?php
require($GO_THEME->theme_path."footer.inc");
?>


