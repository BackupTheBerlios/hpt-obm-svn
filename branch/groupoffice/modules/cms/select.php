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

$site_id = isset($_REQUEST['site_id']) ? $_REQUEST['site_id'] : 0;

$image_filter[] = 'jpg';
$image_filter[] = 'gif';
$image_filter[] = 'png';
$image_filter[] = 'jpeg';
$image_filter[] = 'jpe';

function get_path($folder_id)
{
  global $cms;
  $path = '';

  while($folder = $cms->get_folder($folder_id))
  {
    $path = '/<a href="'.$_SERVER['PHP_SELF'].'?folder_id='.$folder['id'].'">'.$folder['name'].'</a>'.$path;
    $folder_id = $folder['parent_id'];
  }
  return $path;
}

require("../../Group-Office.php");

//authenticate the user
$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('cms');

//get the language file
require($GO_LANGUAGE->get_language_file('cms'));

require($GO_MODULES->class_path.'cms.class.inc');
$cms = new cms();

$cms_settings = $cms->get_settings($GO_SECURITY->user_id);

if(isset($_REQUEST['new_sort_order']) && $_REQUEST['new_sort_order'] != $cms_settings['sort_order'])
{
  $cms->set_sorting($GO_SECURITY->user_id,
      $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
  $cms_settings['sort_order'] = $_REQUEST['new_sort_order'];
  $cms_settings['sort_field'] = $_REQUEST['new_sort_field'];
}

if ($cms_settings['sort_order'] == "DESC")
{
  $image_string = '&nbsp;<img src="'.$GO_THEME->image_url.'buttons/arrow_down.gif" border="0" />';
  $new_sort_order = "ASC";
}else
{
  $image_string = '&nbsp;<img src="'.$GO_THEME->image_url.'buttons/arrow_up.gif" border="0" />';
  $new_sort_order = "DESC";
}

//adjust sorting because folders because they lack some columns
switch ($cms_settings['sort_field'])
{
  case 'cms_files.priority':
    $folders_sort = "priority";
    break;

  case 'cms_files.time':
    $folders_sort = "time";
    break;

  default:
    $folders_sort = "name";
    break;
}



$site = $cms->get_site($site_id);

if (!$GO_SECURITY->has_permission($GO_SECURITY->user_id, $site['acl_write']))
{
  require($GO_THEME->theme_path."header.inc");
  require($GO_CONFIG->root_path.'error_docs/403.inc');
  require($GO_THEME->theme_path."footer.inc");
  exit();

}

//set the folder id we are in
$folder_id = isset($_REQUEST['folder_id']) ? $_REQUEST['folder_id'] : $site['root_folder_id'];

require($GO_THEME->theme_path."header.inc");


$folder = $cms->get_folder($folder_id);

echo '<h2>'.get_path($folder['parent_id']).'/<a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$folder['id'].'">'.$folder['name'].'</a></h2>';
echo '<table border="0">';
if($folder['parent_id']!=0)
{
  echo '<td align="center" width="60" nowrap>';
  echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$folder['parent_id'].'"><img src="'.$GO_THEME->images['uplvl_big'].'" border="0" height="32" width="32" /><br />omhoog</a></td>';
}
echo '<td align="center" width="60" nowrap>';
echo '<a class="small" href="javascript:window.close();"><img src="'.$GO_THEME->images['close'].'" border="0" height="32" width="32" /><br />close</a></td>';

echo '</table>';

echo '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr>';
echo '<td class="TableHead2" width="16"><input type="checkbox" onclick="javascript:invert_selection()" name="dummy" /></td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=cms_files.name&new_sort_order='.$new_sort_order.'">'.$strName;
if ($cms_settings['sort_field'] == "cms_files.name")
echo $image_string;
echo '</a></td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=filetypes.friendly&new_sort_order='.$new_sort_order.'">'.$strType;
if ($cms_settings['sort_field'] == "filetypes.friendly")
echo $image_string;
/*
   echo '</a></td>';

   echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=cms_files.size&new_sort_order='.$new_sort_order.'">'.$strSize;
   if ($cms_settings['sort_field'] == "cms_files.size")
   echo $image_string;
   echo '</a></td>';

   echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=cms_files.mtime&new_sort_order='.$new_sort_order.'">'.$strModified;
   if ($cms_settings['sort_field'] == "cms_files.mtime")
   echo $image_string;
 */
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=cms_files.priority&new_sort_order='.$new_sort_order.'&folder_id='.$folder_id.'">'.$cms_priority;
if ($cms_settings['sort_field'] == "cms_files.priority")
echo $image_string;
echo '</a></td></tr>';

//list the folders first
$total_size = 0;
$count_folders = $cms->get_folders($folder_id, $folders_sort, $cms_settings['sort_order']);
while ($cms->next_record())
{
  $short_name = cut_string($cms->f('name'), 30);
  echo '<tr id="folder_'.$cms->f('id').'" class="Table1">';
  echo '<td><input onclick="javascript:folder_click(this)" type="checkbox" name="folders[]" value="'.$cms->f('id').'" id="'.$cms->f('name').'" /></td>'.
	  '<td nowrap><a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$cms->f('id').'" title="'.$cms->f('name').'">';
	  
  if ($cms->f('disabled') == '1')
  {
    echo '<img width="16" height="16" border="0" src="'.$GO_THEME->images['invisible_folder'].'" align="absmiddle" />';
  }else
  {
    echo '<img width="16" height="16" border="0" src="'.$GO_THEME->images['folder'].'" align="absmiddle" />';
  }

    echo '&nbsp;&nbsp;'.$short_name.'</a>';
  
  echo '<td nowrap>'.$fbFolder.'&nbsp;&nbsp;</td>';
  /*
     echo '<td align="right">-&nbsp;&nbsp;</td>';
     echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'], $cms->f('mtime')).'&nbsp;&nbsp;</td>';
   */
  echo '<td nowrap align="center">'.$cms->f('priority').'</td>';
  echo '</tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

}

//list the files
$count_files = $cms->get_files($folder_id, $cms_settings['sort_field'], $cms_settings['sort_order']);
while ($cms->next_record())
{
     $total_size += $cms->f('size');
    $short_name = strip_extension(cut_string($cms->f('name'), 30));

    echo '<tr id="file_'.$cms->f('id').'" class="Table1">';
    echo '<td><input onclick="javascript:file_click(this)" type="checkbox" name="files[]" value="'.$cms->f('id').'" id="'.$cms->f('name').'" /></td>';
    echo '<td nowrap>';


    if (in_array($cms->f('extension'), $image_filter))
    {
      echo '<a href=\'javascript:_insertImage("'.$GO_MODULES->full_url.'download.php?site_id='.$site_id.'&file_id='.$cms->f('id').'");\' title="'.$cms->f('name').'">';
    }else
    {
      if ($cms->f('title') != '')
      {
	$link_name = $cms->f('title');
      }else
      {
	$link_name = strip_extension($cms->f('name'));
      }
      echo '<a href=\'javascript:_insertHyperlink("'.$GO_MODULES->full_url.'view.php?site_id='.$site_id.'&file_id='.$cms->f('id').'", "'.$link_name.'");\' title="'.$cms->f('name').'">';

    }
	
	echo '<img width="16" height="16" border="0" src="'.$GO_CONFIG->full_url.'controls/icon.php?extension='.$cms->f('extension').'" align="absmiddle" />&nbsp;&nbsp;';

    if (isset($cut_files) && in_array($cms->f('id'), $cut_files))
    {
      echo '<font color="#7d7d7d">'.$short_name.'</font></a>&nbsp;&nbsp;</td>';
    }else
    {
      echo $short_name.'</a>&nbsp;&nbsp;</td>';
    }
    echo '<td nowrap>'.$cms->f('friendly').'&nbsp;&nbsp;</td>';
    /*
       echo '<td nowrap align="right">'.format_size($cms->f('size')).'&nbsp;&nbsp;</td>';
       echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'], $cms->f('mtime')).'&nbsp;&nbsp;</td>';
     */
    echo '<td nowrap align="center">'.$cms->f('priority').'</td>';
    echo '</tr>';
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

}

$count_items = $count_folders+$count_files;
echo '<tr><td colspan="99" class="small" height="18">&nbsp;'.$count_items.' item(s)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$fbFolderSize.': '.format_size($total_size).'</td></tr>';
echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
echo "</table>";
?>
<script type="text/javascript" language="javascript">

function _insertImage(url)
{
  opener.editor_insertHTML('<img src="'+url+'" border="0" align="absmiddle" />');
  //opener.editor_insertImage(url);
  window.close();
}

function _insertHyperlink(url, name)
{
  opener.editor_insertHTML('<a href="'+url+'" target="_self">'+name+'</a>');
  window.close();
}

document.onblur = function() {
  setTimeout('self.focus()',100);
}

</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
