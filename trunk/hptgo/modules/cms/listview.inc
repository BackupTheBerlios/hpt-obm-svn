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
echo '<table border="0" cellpadding="0" cellspacing="0" width="400"><tr>';
echo '<td class="TableHead2" width="16"><input type="checkbox" onclick="javascript:invert_selection()" name="dummy" /></td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=cms_files.name&new_sort_order='.$new_sort_order.'&folder_id='.$folder_id.'">'.$strName;
if ($cms_settings['sort_field'] == "cms_files.name")
echo $image_string;
echo '</a></td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&new_sort_field=cms_files.extension&new_sort_order='.$new_sort_order.'&folder_id='.$folder_id.'">'.$strType;
if ($cms_settings['sort_field'] == "cms_files.extension")
echo $image_string;
echo '</a></td>';
/*
   echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?new_sort_field=cms_files.size&new_sort_order='.$new_sort_order.'&folder_id='.$folder_id.'">'.$strSize;
   if ($cms_sort == "cms_files.size")
   echo $image_string;
   echo '</a></td>';

   echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="'.$_SERVER['PHP_SELF'].'?new_sort_field=cms_files.mtime&new_sort_order='.$new_sort_order.'&folder_id='.$folder_id.'">'.$strModified;
   if ($cms_sort == "cms_files.mtime")
   echo $image_string;
   echo '</a></td>';
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
  echo '<td><input onclick="javascript:folder_click(this)" type="checkbox" name="folders[]" value="'.$cms->f('id').'" id="'.htmlspecialchars($cms->f('name')).'" /></td>'.
	  '<td nowrap>&nbsp;<a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$cms->f('id').'" title="'.htmlspecialchars($cms->f('name')).'">';
  if ($cms->f('disabled') == '1')
  {
    echo '<img width="16" height="16" border="0" src="'.$GO_THEME->images['invisible_folder'].'" align="absmiddle" />&nbsp;&nbsp;';
  }else
  {
    echo '<img width="16" height="16" border="0" src="'.$GO_THEME->images['folder'].'" align="absmiddle" />&nbsp;&nbsp;';
  }
  if (isset($_SESSION['cut_folders']) && in_array($cms->f('id'), $_SESSION['cut_folders']))
  {
    echo '<font color="#7d7d7d">'.htmlspecialchars($cms->f('name')).'</font>';
  }else
  { 
	echo htmlspecialchars($short_name);
  }
  echo '</a>&nbsp;&nbsp;</td><td nowrap>'.$fbFolder.'&nbsp;&nbsp;</td>';
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
  $short_name = cut_string(strip_extension($cms->f('name')), 30);

  echo '<tr id="file_'.$cms->f('id').'" class="Table1">';
  echo '<td><input onclick="javascript:file_click(this)" type="checkbox" name="files[]" value="'.$cms->f('id').'" id="'.htmlspecialchars($cms->f('name')).'" /></td><td nowrap>';

  if(eregi('htm', $cms->f('extension')))
  {
    echo '<a href="edit.php?site_id='.$site_id.'&file_id='.$cms->f('id').'&folder_id='.$folder_id.'" title="'.htmlspecialchars($cms->f('name')).'">';
  }else
  {
    echo '<a target="_self" href="download.php?site_id='.$site_id.'&file_id='.$cms->f('id').'" title="'.htmlspecialchars($cms->f('name')).'">';
  }
  
  echo '<img width="16" height="16" border="0" src="'.get_filetype_image($cms->f('extension')).'" align="absmiddle" />&nbsp;&nbsp;';

  if (isset($_SESSION['cut_files']) && in_array($cms->f('id'), $_SESSION['cut_files']))
  {
    echo '<font color="#7d7d7d">'.$short_name.'</font>';
  }else
  {
    echo $short_name;
  }
  echo '</a>&nbsp;&nbsp;</td><td nowrap>'.get_filetype_description($cms->f('extension')).'&nbsp;&nbsp;</td>';
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

function file_click(check_box)
{
  var item = get_object('file_'+check_box.value);
  if (check_box.checked)
  {
    item.className = 'Table2';
  }else
  {
    item.className = 'Table1';
  }
}

function folder_click(check_box)
{
  var item = get_object('folder_'+check_box.value);
  if (check_box.checked)
  {
    item.className = 'Table2';
  }else
  {
    item.className = 'Table1';
  }
}


function invert_selection()
{
  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
      if (document.forms[0].elements[i].name == 'files[]')
      {
	file_click(document.forms[0].elements[i]);
      }else
      {
	folder_click(document.forms[0].elements[i]);
      }
    }
  }
}

function cut_items()
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

  if (count > 0)
  {
    document.forms[0].task.value = 'cut';
    document.forms[0].submit();
  }else
  {
    alert("<?php echo $fbNoSelect; ?>");
  }
}

function copy_items()
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

  if (count > 0)
  {
    document.forms[0].task.value = 'copy';
    document.forms[0].submit();
  }else
  {
    alert("<?php echo $fbNoSelect; ?>");
  }
}

function paste_items()
{
  document.forms[0].task.value = 'paste';
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
	filename = document.forms[0].elements[i].id;
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



function properties()
{
  var count = 0;
  var id = new String;
  var type = new String;

  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      if (document.forms[0].elements[i].checked == true)
      {
	count++;
	id = document.forms[0].elements[i].value;
	if(document.forms[0].elements[i].name == 'files[]')
	{
	  type = 'file';
	}else
	{
	  type = 'folder';
	}
      }
    }
  }
  switch (count)
  {
    case 0:
      document.location='properties.php?site_id=<?php echo $site_id; ?>&folder_id=<?php echo $folder_id; ?>&return_to=<?php echo urlencode($link_back); ?>&task=folder_properties';
      break;

    case 1:
      if (type == 'folder')
      {
	document.location='properties.php?site_id=<?php echo $site_id; ?>&folder_id='+id+'&return_to=<?php echo urlencode($link_back); ?>&task=folder_properties';
      }else
      {
	document.location='properties.php?site_id=<?php echo $site_id; ?>&folder_id=<?php echo $folder_id; ?>&file_id='+id+'&return_to=<?php echo urlencode($link_back); ?>&task=file_properties';
      }
      break;

    default:
      alert("<?php echo $fs_no_multi_select; ?>");
      break;
  }
}
</script>
