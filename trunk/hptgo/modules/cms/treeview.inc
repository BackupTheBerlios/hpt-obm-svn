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

//start of treeview code
$image['mlastnode'] = '<img src="'.$GO_THEME->images['mlastnode'].'" border="0" height="22" width="16" align="absmiddle"/>';
$image['emptylastnode'] = '<img src="'.$GO_THEME->images['emptylastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['plastnode'] = '<img src="'.$GO_THEME->images['plastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['mnode'] = '<img src="'.$GO_THEME->images['mnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['emptynode'] = '<img src="'.$GO_THEME->images['emptynode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['pnode'] = '<img src="'.$GO_THEME->images['pnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['vertline'] = '<img src="'.$GO_THEME->images['vertline'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['blank'] = '<img src="'.$GO_THEME->images['blank'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['group'] = '<img src="'.$GO_THEME->images['workgroup'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['user'] = '<img src="'.$GO_THEME->images['workstation'].'" border="0" height="22" width="20" align="absmiddle" />';

$image['opened_folder'] = '<img src="'.$GO_THEME->images['folderopen'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['closed_folder'] = '<img src="'.$GO_THEME->images['folderclosed'].'" border="0" height="22" width="24" align="absmiddle" />';

/*
   prints the folders in a tree
   $folders is an array of associve arrays containing an 'id' and 'name'
 */
function print_tree($folders, $images='')
{
  global $image, $cms, $folder_id, $site_id, $folders_sort, $cms_settings;

  $count = count($folders);
  for ($i=0;$i<$count;$i++)
  {
    //for each folder check if there are subfolders and
    //add them to an array for the next instance of this recursive function
    $subfolders = array();
    $subfolders_count = $cms->get_folders($folders[$i]['id'], $folders_sort, $cms_settings['sort_order']);
    while($cms->next_record())
    {
      $subfolders[] = $cms->Record;
    }
    //is this folder opened inthe tree?
    $open = in_array($folders[$i]['id'], $_SESSION['expanded']);

    //determine the image and node to display
    if ($subfolders_count > 0)
    {
      if ($i < ($count-1))
      {
	$new_image = $image['vertline'];
	$node = $open ? $image['mnode'] : $image['pnode'];

      }else
      {
	$new_image = $image['blank'];
	$node = $open ? $image['mlastnode'] : $image['plastnode'];
      }
    }else
    {
      if ($i < ($count-1))
      {
	$new_image = $image['vertline'];
	$node = $image['emptynode'];
      }else
      {
	$new_image = $image['blank'];
	$node = $image['emptylastnode'];
      }
    }

    //if the current folder is the current opened folder then show an opened folder
    if ($folders[$i]['id'] == $folder_id)
    {
      $folder_image = $image['opened_folder'];
    }else
    {
      $folder_image = $image['closed_folder'];
    }

    //actually print the current folder
    $short_name = cut_string($folders[$i]['name'], 30);
    echo '<a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&expand_id='.$folders[$i]['id'].'&folder_id='.$folder_id.'">'.$images.$node.'</a>'.
		'<a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$folders[$i]['id'].'" title="'.$folders[$i]['name'].'">'.$folder_image.$short_name.'</a><br />';

    if ($open)
    {
      print_tree($subfolders, $images.$new_image);
    }
  }
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



//fill the first value of the expanded with -1 because of key 0
//results into false at the array_search() function and behaves differently at different PHP versions
if (!isset($_SESSION['expanded']))
{
  $_SESSION['expanded'][]=-1;
}

//expand or collaps the expand id which is passed when a user clicks a node
if (isset($_REQUEST['expand_id']))
{
  $expand_id = smartstrip($_REQUEST['expand_id']);
  $key = array_search($_REQUEST['expand_id'], $_SESSION['expanded']);
  if (!$key)
  {
    $_SESSION['expanded'][] = $expand_id;
  }else
  {
    unset($_SESSION['expanded'][$key]);
  }
}else
{
  //always expand folder clicks
  $key = array_search($folder_id, $_SESSION['expanded']);
  if (!$key)
  {
    $_SESSION['expanded'][] = $folder_id;
  }
}

//end of treeview code

//print the first folder as the site

if($folder = $cms->get_folder($site['root_folder_id']))
{
  $short_name = cut_string($folder['name'], 30);
 echo '<a href="'.$_SERVER['PHP_SELF'].'?expand_id='.$site['root_folder_id'].'"></a>'.
  '<a href="'.$_SERVER['PHP_SELF'].'?site_id='.$site_id.'&folder_id='.$site['root_folder_id'].'" title="'.$folder['name'].'"><img src="'.$GO_THEME->images['site'].'" border="0" widht="16" height="16" align="absmiddle" />&nbsp;&nbsp;'.$short_name.'</a><br />';

  $cms->get_folders($site['root_folder_id'], $folders_sort, $cms_settings['sort_order']);
  $folders = array();
  while($cms->next_record())
  {
    $folders[] = $cms->Record;
  }

  print_tree($folders);
}
?>
