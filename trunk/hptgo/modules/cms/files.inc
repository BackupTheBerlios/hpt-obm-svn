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

//add blank task var
echo '<input type="hidden" name="task" />';
echo '<input type="hidden" name="return" value="'.$folder_id.'" />';
echo '<input type="hidden" name="folder_id" value="'.$folder_id.'" />';
echo '<input type="hidden" name="file_id" />';
//builds the full path to this directory with links
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

//get current folder information and print path and buttons
$folder = $cms->get_folder($folder_id);
echo '<h2>'.get_path($folder['parent_id']).'/<a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$folder['id'].'">'.$folder['name'].'</a></h2>';
echo '<table border="0" cellpadding="0" cellspacing="0">';
if($folder['parent_id']!=0)
{
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$folder['parent_id'].'"><img src="'.$GO_THEME->images['uplvl_big'].'" border="0" height="32" width="32" /><br />'.$fbUpLevel .'</a></td>';
}
echo '<td class="ModuleIcons">';
echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&task=add_folder&folder_id='.$folder_id.'"><img src="'.$GO_THEME->images['new_folder'].'" border="0" height="32" width="32" /><br />'.$fbNewFolder.'</a></td>';
echo '<td class="ModuleIcons">';
echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&task=upload&folder_id='.$folder_id.'"><img src="'.$GO_THEME->images['upload'].'" border="0" height="32" width="32" /><br />'.$fbUpload.'</a></td>';
echo '<td class="ModuleIcons">';
echo '<a class="small" href="javascript:delete_items()"><img src="'.$GO_THEME->images['delete_big'].'" border="0" height="32" width="32" /><br />'.$fbDelete.'</a></td>';
echo '<td class="ModuleIcons">';
echo '<a class="small" href="edit.php?site_id='.$site_id.'&folder_id='.$folder_id.'"><img src="'.$GO_THEME->images['new_doc'].'" border="0" height="32" width="32" /><br />'.$cms_new_page.'</a></td>';
echo '<td class="ModuleIcons">';
echo '<a class="small" href="javascript:properties()"><img src="'.$GO_THEME->images['properties'].'" border="0" height="32" width="32" /><br />'.$fbProperties.'</a></td>';
echo '<td class="ModuleIcons">';
echo '<a class="small" href="javascript:cut_items()"><img src="'.$GO_THEME->images['cut'].'" border="0" height="32" width="32" /><br />'.$fbCut.'</a></td>';
echo '<td class="ModuleIcons">';
echo '<a class="small" href="javascript:copy_items()"><img src="'.$GO_THEME->images['copy'].'" border="0" height="32" width="32" /><br />'.$fbCopy.'</a></td>';

$_SESSION['cut_files'] = isset($_SESSION['cut_files']) ? $_SESSION['cut_files'] : array();
$_SESSION['cut_folders'] = isset($_SESSION['cut_folders']) ? $_SESSION['cut_folders'] : array();
$_SESSION['copy_folders'] = isset($_SESSION['copy_folders']) ? $_SESSION['copy_folders'] : array();
$_SESSION['copy_files'] = isset($_SESSION['copy_files']) ? $_SESSION['copy_files'] : array();

if((count($_SESSION['cut_files'] ) + count($_SESSION['cut_folders']) + count($_SESSION['copy_folders']) + count($_SESSION['copy_files'] )) > 0)
{
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="javascript:paste_items()"><img src="'.$GO_THEME->images['paste'].'" border="0" height="32" width="32" /><br />'.$fbPaste.'</a></td>';
}
echo '<td class="ModuleIcons">';
echo '<a class="small" target="_blank" href="view.php?site_id='.$site['id'].'&folder_id='.$folder_id.'" title="'.$cms_preview.' \''.$site['name'].'\'"><img src="'.$GO_THEME->images['magnifier_big'].'" border="0" /><br />'.$cms_preview.'</a></td>';

echo '<td class="ModuleIcons">';
echo '<a class="small" href="site.php?site_id='.$site_id.'&return_to='.rawurlencode($GO_MODULES->url.'browse.php?site_id='.$site_id.'&folder_id='.$folder_id).'"><img src="'.$GO_THEME->images['cms'].'" border="0" height="32" width="32" /><br />'.$cms_settings.'</a></td>';

if ($qn_plugin = $GO_MODULES->get_plugin('questionnaires'))
{
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="'.$qn_plugin['url'].'?site_id='.$site_id.'"><img src="'.$GO_THEME->images['questionmark'].'" border="0" height="32" width="32" /><br />Vragenlijsten</a></td>';
}

echo '<td class="ModuleIcons">';
echo '<a class="small" href="index.php"><img src="'.$GO_THEME->images['exit'].'" border="0" height="32" width="32" /><br />'.$cmdClose.'</a></td>';

echo '</table>';
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

?>

<table border="0" cellpadding="10" width="100%">
<tr>
<td valign="top" nowrap>
<?php
require('treeview.inc');
?>
</td>
<td valign="top" nowrap>
<?php
require('listview.inc');
?>
</td>
</tr>
</table>
