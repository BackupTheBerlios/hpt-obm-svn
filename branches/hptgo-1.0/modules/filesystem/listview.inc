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

$is_project_folder = $GO_CONFIG->window_mode == 'projects';

if(isset($_REQUEST['new_sort_order']) && $_REQUEST['new_sort_order'] != $fs_settings['sort_order'])
{
  $fs->set_sorting($GO_SECURITY->user_id, $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
  $fs_settings['sort_order'] = $_REQUEST['new_sort_order'];
  $fs_settings['sort_field'] = $_REQUEST['new_sort_field'];
}

if ($fs_settings['sort_order'] == "DESC")
{
  $sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $new_sort_order = "ASC";
}else
{
  $sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $new_sort_order = "DESC";
}

if ($fs->is_common_folder($path))
  $folders = $fs->get_visible_folders_sorted($GO_SECURITY->user_id,$path, $fs_settings['sort_field'], $fs_settings['sort_order']);
else
  $folders = $fs->get_folders_sorted($path, $fs_settings['sort_field'], $fs_settings['sort_order']);
$files = $fs->get_files_sorted($path, $fs_settings['sort_field'], $fs_settings['sort_order']);
$item_count = count($files) + count($folders);
if (!$is_project_folder) {
  echo '<h1>'.$fbPath.': '.$page_title.'</h1>';
}
if (isset($feedback)) echo $feedback;
echo '<input type="hidden" name="task" />';

echo '<table border="0" cellpadding="0" cellspacing="0"><tr>';

if (!$is_project_folder) {
  if (isset($_REQUEST['parent_path']))
    $parent_path = $_REQUEST['parent_path'];
  else
    $parent_path = dirname($path);
  if ($parent_path)
  {
    if ($fs->has_read_permission2($GO_SECURITY->user_id, $parent_path))
    {
      echo '<td class="ModuleIcons">';
      echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($parent_path).'"><img src="'.$GO_THEME->images['uplvl_big'].'" border="0" height="32" width="32" /><br />'.$fbUpLevel.'</a></td>';
    }
  }
}
echo '<td class="ModuleIcons">';
echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?path='.$urlencoded_path.'"><img src="'.$GO_THEME->images['fs_refresh'].'" border="0" height="32" width="32" /><br />'.$fbRefresh.'</a></td>';
if (!$is_project_folder) {
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href=\'javascript:properties("'.rawurlencode($fs_no_multi_select).'")\'><img src="'.$GO_THEME->images['properties'].'" border="0" height="32" width="32" /><br />'.$fbProperties.'</a></td>';
}

if ($write_permission)
{
  if ($GO_CONFIG->window_mode != 'projects') {
    echo '<td class="ModuleIcons">';
    echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?task=new_folder&path='.$urlencoded_path.'"><img src="'.$GO_THEME->images['new_folder'].'" border="0" height="32" width="32" /><br />'.$fbNewFolder.'</a></td>';
  }
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?task=upload&path='.$urlencoded_path.'"><img src="'.$GO_THEME->images['upload'].'" border="0" height="32" width="32" /><br />'.$fbUpload.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="javascript:delete_items()"><img src="'.$GO_THEME->images['delete_big'].'" border="0" height="32" width="32" /><br />'.$fbDelete.'</a></td>';
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href=\'javascript:cut_items("'.rawurlencode($fbNoSelect).'")\'><img src="'.$GO_THEME->images['cut'].'" border="0" height="32" width="32" /><br />'.$fbCut.'</a></td>';
}
echo '<td class="ModuleIcons">';
echo '<a class="small" href=\'javascript:copy_items("'.rawurlencode($fbNoSelect).'")\'><img src="'.$GO_THEME->images['copy'].'" border="0" height="32" width="32" /><br />'.$fbCopy.'</a></td>';

if((count($_SESSION['cut_files']) + count($_SESSION['cut_folders']) + count($_SESSION['copy_files']) + count($_SESSION['copy_folders'])) > 0)
{
  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="javascript:paste_items()"><img src="'.$GO_THEME->images['paste'].'" border="0" height="32" width="32" /><br />'.$fbPaste.'</a></td>';
}

if($GO_CONFIG->window_mode == 'normal')
{
  if ($email_module)
  {
    echo '<td class="ModuleIcons">';
    echo '<a class="small" href=\'javascript:mail_files("'.rawurlencode($fbNoSelect).'")\'><img src="'.$GO_THEME->images['fs_email'].'" border="0" height="32" width="32" /><br />'.$lang_modules['email'].'</a></td>';
  }

  if (!$is_project_folder) {
    echo '<td class="ModuleIcons">';
    echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?task=shares&path='.$urlencoded_path.'"><img src="'.$GO_THEME->images['sharing'].'" border="0" height="32" width="32" /><br />'.$fbSharing.'</a></td>';
  }

  echo '<td class="ModuleIcons">';
  echo '<a class="small" href="'.$_SERVER['PHP_SELF'].'?task=search&path='.$urlencoded_path.'"><img src="'.$GO_THEME->images['fs_search'].'" border="0" height="32" width="32" /><br />'.$fs_search.'</a></td>';

  if ($write_permission)
  {
    echo '<td class="ModuleIcons">';
    echo '<a class="small" href="javascript:create_archive()"><img src="'.$GO_THEME->images['compress'].'" border="0" height="32" width="32" /><br />'.$fb_compress.'</a></td>';

    echo '<td class="ModuleIcons">';
    echo '<a class="small" href="javascript:extract()"><img src="'.$GO_THEME->images['decompress'].'" border="0" height="32" width="32" /><br />'.$fb_decompress.'</a></td>';
  }}
echo '</tr></table>';


if ($GO_CONFIG->window_mode == 'popup')
{
  echo '<table border="0" width="100%">';
  if ($mode == 'save')
  {
    echo '<tr><td><h3>'.$fbFilename.':</h3></td><td><input type="text" class="textbox" name="filename" value="'.$filename.'" size="40" /></td></tr>';
  }
  echo '<tr><td><h3>'.$strLookIn.':</h3></td><td>';

  $dropbox = new dropbox();
  $dropbox->add_value($home_path, 'home');

  $fs2 = new filesystem();
  $fs2->get_authorized_sharers($GO_SECURITY->user_id);

  while ($fs2->next_record())
  {
    $shares = array();
    $share_count = $fs->get_shares($fs2->f('user_id'));
    while ($fs->next_record())
    {
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $fs->f('acl_read')) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $fs->f('acl_write')))
      {
	if (strpos($path, $fs->f('path')) !== false)
	{
	  $location = $fs->f('path');
	}
	$shares[] = $fs->f('path');
      }
    }
    $share_count = count($shares) ;
    if ($share_count > 0)
    {
      if ($user = $GO_USERS->get_user($fs2->f('user_id')))
      {
	$dropbox->add_optgroup($user['username']);
	for ($i=0;$i<$share_count;$i++)
	{
	  $dropbox->add_value($shares[$i], basename($shares[$i]));
	}
      }
    }
  }
  $location = isset($_POST['location']) ? $_POST['location'] : $home_path;
  $dropbox->print_dropbox('location',$location,'onchange="javascript:change_location(this)"');
  echo '</td></tr></table>';
}


echo '<table border="0" width="100%">';
echo '<tr>';

if ($GO_CONFIG->window_mode == 'normal')
{
  global $is_project_folder;

  if (!$is_project_folder) {
    echo '<td valign="top" nowrap style="padding-right: 25px;">';
    require('treeview.inc');
    echo '</td>';
  }
}

echo '<td valign="top" width="100%">';
echo '<table border="0" cellspacing="0" cellpadding="0" width="100%"><tr>';
echo '<td class="TableHead2" width="16"><input type="checkbox" onclick="javascript:invert_selection()" name="dummy" /></td>';
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?path='.$urlencoded_path.'&new_sort_field=basename&new_sort_order='.$new_sort_order.'">'.$strName.'</a>';
if ($fs_settings['sort_field'] == 'basename')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?path='.$urlencoded_path.'&new_sort_field=filemimefriendly&new_sort_order='.$new_sort_order.'">'.$strType.'</a>';
if ($fs_settings['sort_field'] == 'filemimefriendly')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" nowrap align="right"><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?path='.$urlencoded_path.'&new_sort_field=filesize&new_sort_order='.$new_sort_order.'">'.$strSize.'</a>';
if ($fs_settings['sort_field'] == 'filesize')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?path='.$urlencoded_path.'&new_sort_field=filemtime&new_sort_order='.$new_sort_order.'">'.$strModified.'</a>';
if ($fs_settings['sort_field'] == 'filemtime')
{
  echo $sort_arrow;
}
echo '</td>';
echo '</tr>';
while ($folder = array_shift($folders))
{
  $short_name = cut_string($folder['name'], 30);
  echo '<tr id="'.$folder['path'].'" class="Table1">';
  echo '<td><input onclick="javascript:item_click(this)" type="checkbox" name="folders[]" value="'.$folder['path'].'" /></td>';
  if ($fs->is_common_folder($folder['path']))
    $is_system_folder = $fs->is_system_folder($folder['path']);
  if (($fs->is_common_folder($folder['path']) && $is_system_folder))
    $folder_image = $GO_THEME->images['system_folder'];
  else if (!$fs->is_common_folder($folder['path']) && $fs->get_share($folder['path']))
  {
    $folder_image = $GO_THEME->images['shared_folder'];
  }else
  {
    $folder_image = $GO_THEME->images['folder'];
  }


  echo '<td nowrap>';
  if (isset($_SESSION['cut_folders']) && in_array(addslashes($folder['path']), $_SESSION['cut_folders']))
  {
    echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($folder['path']).'"><img width="16" height="16" border="0" src="'.$folder_image.'" align="absmiddle" /><font color="#7d7d7d">'.$folder['name'].'</font></a></td>';
  }else
  {
    echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($folder['path']).'" title="'.htmlspecialchars($folder['name']).'"><img width="16" height="16" border="0" src="'.$folder_image.'" align="absmiddle" />&nbsp;'.htmlspecialchars($short_name).'</a></td>';
  }
  echo '<td nowrap>'.$fbFolder.'&nbsp;&nbsp;</td>';
  echo '<td align="right">-&nbsp;&nbsp;</td>';
  echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time($folder['mtime'])).'&nbsp;&nbsp;</td>';
  echo '</tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}

$total_size = 0;

if ($mode != 'save')
{
  while ($file = array_shift($files))
  {
    $extension = get_extension($file['name']);
    if (!isset($GO_FILTER_TYPES) || in_array($extension, $GO_FILTER_TYPES))
    {
      $total_size += $file['size'];
      $short_name = cut_string(strip_extension($file['name']), 30);

      echo '<tr id="'.$file['path'].'" class="Table1">';
      echo '<td><input onclick="javascript:item_click(this)" type="checkbox" name="files[]" value="'.$file['path'].'" /></td>';
      echo '<td nowrap>';
      if (isset($_SESSION['cut_files']) && in_array(addslashes($file['path']), $_SESSION['cut_files']))
      {
	echo '<a target="'.$target_frame.'" href="'.$GO_HANDLER.'?path='.urlencode($file['path']).'" title="'.htmlspecialchars($file['name']).'"><img width="16" height="16" border="0" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$extension.'" align="absmiddle" /><font color="#7d7d7d">'.$short_name.'</font></a></td>';
      }else
      {
	echo '<a target="'.$target_frame.'" href="'.$GO_HANDLER.'?path='.urlencode($file['path']).'" title="'.htmlspecialchars($file['name']).'"><img width="16" height="16" border="0" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$extension.'" align="absmiddle" />&nbsp;'.htmlspecialchars($short_name).'</a></td>';
      }
      echo '<td nowrap>'.$file['type'].'&nbsp;&nbsp;</td>';
      echo '<td nowrap align="right">'.format_size($file['size']).'&nbsp;&nbsp;</td>';
      echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time($file['mtime'])).'&nbsp;&nbsp;</td>';
      echo '</tr>';
      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
    }
  }
}

echo '<tr><td colspan="99" class="small" height="18">&nbsp;'.$item_count.' '.$fbItems.'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$fbFolderSize.': '.format_size($total_size).'</td></tr>';
echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
echo '</table>';

echo '</td></tr>';

if ($GO_CONFIG->window_mode=='popup')
{
  echo '<tr><td colspan="2" align="center"><br />';
  $button = new button($cmdOk,'javascript:submit_to_handler()');
  echo '&nbsp;&nbsp;';
  $button = new button($cmdCancel,'javascript:window.close()');
  echo '</td></tr>';
}
echo '</table>';
?>
<script type="text/javascript" language="javascript" src="<?php echo $GO_MODULES->url.'filesystem.js'; ?>"></script>
<script type="text/javascript" language="javascript">

function submit_to_handler()
{
  <?php
    if ($GO_MULTI_SELECT == false)
    {
      ?>
	var count = 0;
      var path = new String;

      for (var i=0;i<document.forms[0].elements.length;i++)
      {
	if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
	{
	  if (document.forms[0].elements[i].checked == true)
	  {
	    document.forms[0].path.value = document.forms[0].elements[i].value;
	    count++;
	  }
	}
      }
      if (count > 1)
      {
	alert("<?php echo $fs_no_multi_select; ?>");
	return false;
      }
      <?php
    }
  ?>
    document.forms[0].task.value = 'GO_HANDLER';
  document.forms[0].action = "<?php echo $GO_HANDLER; ?>";
  document.forms[0].submit();
}

function delete_items()
{
  var count = 0;
  var filename = new String;
  var path = new String;

  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      if (document.forms[0].elements[i].checked == true)
      {
	count++;
	path = document.forms[0].elements[i].value;
	filename = path.substring(path.lastIndexOf('/')+1, path.length);
      }
    }
  }
  switch (count)
  {
    case 0:
      alert("<?php echo $fbNoSelect; ?>");
      break;

    case 1:
      if (confirm("<?php echo $strDeletePrefix; ?>'"+filename+"'<?php echo $strDeleteSuffix; ?>"))
      {
	document.forms[0].task.value = 'delete';
	document.forms[0].submit();
      }
      break;

    default:
      if (confirm("<?php echo $strDeletePrefix.$strThis; ?> "+count+" <?php echo $strItems.$strDeleteSuffix; ?>"))
      {
	document.forms[0].task.value = 'delete';
	document.forms[0].submit();
      }
      break;
  }
}

function create_archive()
{
  var count = 0;

  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      if (document.forms[0].elements[i].checked == true)
      {
	count++;
      }
    }
  }
  if (count == 0)
  {
    alert("<?php echo $fbNoSelect; ?>");
  }else
  {
    document.forms[0].task.value = 'create_archive';
    document.forms[0].submit();
  }
}

function extract()
{
  var count = 0;

  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      if (document.forms[0].elements[i].checked == true)
      {
	count++;
      }
    }
  }
  if (count == 0)
  {
    alert("<?php echo $fbNoSelect; ?>");
  }else
  {
    document.forms[0].task.value = 'extract';
    document.forms[0].submit();
  }
}
</script>
