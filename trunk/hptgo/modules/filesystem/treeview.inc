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

$image['mlastnode'] = '<img src="'.$GO_THEME->images['mlastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
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
$image['opened_folder_shared'] = '<img src="'.$GO_THEME->images['shared_folder_open'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['closed_folder_shared'] = '<img src="'.$GO_THEME->images['shared_folder_closed'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['opened_system_folder'] = '<img src="'.$GO_THEME->images['system_folder_open'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['closed_system_folder'] = '<img src="'.$GO_THEME->images['system_folder_closed'].'" border="0" height="22" width="24" align="absmiddle" />';
function print_filesystem($folders, $images='', $arguments='')
{
	global $image, $path, $GO_SECURITY, $group_folders;

	$fs = new filesystem();

	$count = count($folders);
	for ($i=0;$i<$count;$i++)
	{
    if (is_group_folder($group_folders, $folders[$i]['path']) ||
		  $fs->has_read_permission2($GO_SECURITY->user_id, $folders[$i]['path']))
//		if ($fs->has_read_permission($GO_SECURITY->user_id, $folders[$i]['path']))
		{
			$subfolders = $fs->get_folders($folders[$i]['path']);
			$subfolders_count = count($subfolders);
			$open = in_array($folders[$i]['path'], $_SESSION['expanded']);
		}else
		{
			$subfolders = array();
			$subfolders_count = 0;
			$open = false;
		}

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

		if ($fs->is_common_folder($folders[$i]['path']))
			$is_system_folder = $fs->is_system_folder($folders[$i]['path']);

		if ($folders[$i]['path'] == $path)
		{
			if (!$fs->is_common_folder($folders[$i]['path']) && $fs->get_share($folders[$i]['path']))
			{
				$folder_image = $image['opened_folder_shared'];
			}else if ($fs->is_common_folder($folders[$i]['path']) && $is_system_folder)
				$folder_image = $image['opened_system_folder'];
			else
				$folder_image = $image['opened_folder'];
		}else
		{
			if (!$fs->is_common_folder($folders[$i]['path']) && $fs->get_share($folders[$i]['path']))
			{
				$folder_image = $image['closed_folder_shared'];
			}else if ($fs->is_common_folder($folders[$i]['path']) && $is_system_folder)
				$folder_image = $image['closed_system_folder'];
			else
				$folder_image = $image['closed_folder'];

		}

		$short_name = cut_string($folders[$i]['name'], 30);

		echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($path).'&expand_path='.urlencode($folders[$i]['path']).$arguments.'">'.$images.$node.'</a>';
		echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($folders[$i]['path']).$arguments.'" title="'.htmlspecialchars($folders[$i]['name']).'">'.$folder_image.htmlspecialchars($short_name).'</a><br />';

		if ($open)
		{
			print_filesystem($subfolders, $images.$new_image, $arguments);
		}
	}
	unset($fs);
}

function print_common_filesystem($folders, $images='', $arguments='')
{
	global $image, $path, $GO_SECURITY;

	$fs = new filesystem();

	$count = count($folders);
	for ($i=0;$i<$count;$i++)
	{
		if ($fs->has_read_permission2($GO_SECURITY->user_id, $folders[$i]['path']))
		{
			$subfolders = $fs->get_visible_folders($GO_SECURITY->user_id,$folders[$i]['path']);
			$subfolders_count = count($subfolders);
			for ($ii = 0;$ii < $subfolders_count; $ii ++)
				$subfolders[$ii]['parent'] = $folders[$i]['path'];
			$open = in_array($folders[$i]['path'], $_SESSION['expanded']);
		}
		else
		{
			$subfolders = array();
			$subfolders_count = 0;
			$open = false;
		}

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

		if ($fs->is_common_folder($folders[$i]['path']))
			$is_system_folder = $fs->is_system_folder($folders[$i]['path']);

		if ($folders[$i]['path'] == $path)
		{
			if ((!$fs->is_common_folder($folders[$i]['path']) && $fs->get_share($folders[$i]['path'])))
			{
				$folder_image = $image['opened_folder_shared'];
			}else if ($fs->is_common_folder($folders[$i]['path']) && $is_system_folder)
				$folder_image = $image['opened_system_folder'];
			else {
				$folder_image = $image['opened_folder'];
			}
		}else
		{
			if ((!$fs->is_common_folder($folders[$i]['path']) && $fs->get_share($folders[$i]['path'])))
			{
				$folder_image = $image['closed_folder_shared'];
			}else if ($fs->is_common_folder($folders[$i]['path']) && $is_system_folder)
				$folder_image = $image['closed_system_folder'];
			else
			{
				$folder_image = $image['closed_folder'];
			}

		}

		$short_name = cut_string($folders[$i]['name'], 30);

		echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($path).'&expand_path='.urlencode($folders[$i]['path']).$arguments.'">'.$images.$node.'</a>';
		echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($folders[$i]['path']).$arguments.'&parent_path='.urlencode($folders[$i]['parent']).'" title="'.htmlspecialchars($folders[$i]['name']).'">'.$folder_image.htmlspecialchars($short_name).'</a><br />';

		if ($open)
		{
			print_common_filesystem($subfolders, $images.$new_image, $arguments);
		}
	}
	unset($fs);
}


if (!isset($_SESSION['expanded']))
{
	$_SESSION['expanded'][]=-1;
}

//expand or collaps the expand id which is passed when a user clicks a node
if (isset($_REQUEST['expand_path']))
{
	$expand_path = smartstrip($_REQUEST['expand_path']);
	$key = array_search($expand_path, $_SESSION['expanded']);
	if (!$key)
	{
		$_SESSION['expanded'][] = $expand_path;
	}else
	{
		unset($_SESSION['expanded'][$key]);
	}
}else
{
	//always expand folder clicks
	if (isset($path))
	{
		$key = array_search($path, $_SESSION['expanded']);
		if (!$key)
		{
			$_SESSION['expanded'][] = $path;
		}

	}
}

//stay in search mode if search is clicked
if ($task == 'search')
{
	$arguments = '&task=search';
}else
{
	$arguments = '';
}


echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($home_path).$arguments.'">'.$image['user'].'home</a><br />';

$folder = array();
$folder['path'] = $home_path;
$folder['name'] = basename($folder['path']);
print_filesystem(array($folder),'',$arguments);


	echo '<table border="0" cellpadding="0" cellspacing="0">';
	echo '<tr><td>'.$image['group'].'</td>';
	echo '<td>'.$fbWorkspace.'</td></tr></table>';
//echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($home_path).$arguments.'">'.$image['user'].'home</a><br />';

$folder = array();
$folder['path'] = $GO_CONFIG->file_storage_path.'common';
$folder['name'] = basename($folder['path']);
$folder['parent'] = '';

// We MUST share the root folder.
if (!$fs->get_share($folder['path']) &&
    $GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
	$fs->add_share($GO_SECURITY->user_id,$folder['path']);

if ($fs->has_read_permission2($GO_SECURITY->user_id,$folder['path']))
	print_common_filesystem(array($folder),'',$arguments);
else
{
	$my_folders = $fs->get_visible_folders($GO_SECURITY->user_id,$folder['path']);
	for ($i = 0;$i < count($my_folders);$i ++)
		$my_folders[$i]['parent'] = '';
	print_common_filesystem($my_folders,'',$arguments);
}

$users_array = array();

foreach ( $fs->get_my_shares() as $sharer )
{
	$GO_USERS->get_user( $sharer );

	if ($GO_USERS->f('id') != $GO_SECURITY->user_id)
	{

		$user['id'] = $GO_USERS->f('id');
		$user['username'] = $GO_USERS->f('username');
		$middle_name = $GO_USERS->f('middle_name') == '' ? '' : $GO_USERS->f('middle_name').' ';
		$user['name'] = $GO_USERS->f('last_name').' '.$middle_name.$GO_USERS->f('first_name');

	//	$shared_folders = array();

//		if($open = in_array($sharer, $_SESSION['expanded']))
//		{
			$share_count = $fs->get_shares($GO_USERS->f('id'));

			$shared_folders = array();
			$had_permission = false;
			while ($fs->next_record())
			{
				$is_sub_dir = false;
				if ($fs->is_common_folder($fs->f('path')))
				{
					$share_count --;
					continue;
				}

				if (file_exists($fs->f('path')))
				{
					if (is_dir($fs->f('path')))
					{
						$is_sub_dir = isset($last_folder) ? $fs->is_sub_dir($fs->f('path'), $last_folder) : false;

						if (!$had_permission || !$is_sub_dir)
						{
							if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $fs->f('acl_read')) || 
									$GO_SECURITY->has_permission($GO_SECURITY->user_id, $fs->f('acl_write')))
							{
								$folder['path'] = $fs->f('path');
								$folder['name'] = basename($folder['path']);
								$folder['share_path'] = $folder['path'];
								$shared_folders[] = $folder;
								$had_permission = true;
							}else
							{
								$had_permission = false;
								$share_count--;
							}
						}else
						{
							$share_count--;
						}
					}else
					{
						$share_count--;
					}
				}else
				{
					$fs2 = new filesystem();
					$fs2->delete_share($fs->f('path'));
					$share_count--;
				}
				if (!$is_sub_dir)
				{
					$last_folder = $fs->f('path');
				}
			}
//		}

//		if (count($shared_folders) > 0) {
		if ($share_count > 0) {
			$user['shared_folders'] = $shared_folders;
			$users_array[] = $user;
		}
	}
}

$counter = $count = count($users_array);

if ($count > 0)
{
	echo '<table border="0" cellpadding="0" cellspacing="0">';
	echo '<tr><td>'.$image['group'].'</td>';
	echo '<td>'.$fbShares.'</td></tr></table>';

	while($user = array_shift($users_array))
	{
		$open = in_array($user['id'], $_SESSION['expanded']);
		$sign = $open ? 'm' : 'p';

		$share_count = count($user['shared_folders']);

		if ($counter > 1)
		{	$leftside = $image['vertline'];
			$node = $image[$sign.'node'];
		}else
		{
			$leftside = $image['blank'];
			$node = $image[$sign.'lastnode'];
		}
		$short_name = cut_string($user['name'], 30);
		echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($path).'&expand_path='.urlencode($user['id']).$arguments.'" title="'.$user['name'].'">'.$node.$image['user'].'</a>';
		echo '<a href="'.$_SERVER['PHP_SELF'].'?path='.urlencode($path).'&expand_path='.urlencode($user['id']).$arguments.'" title="'.$user['name'].'">'.$short_name.'</a><br />';


		if($open)
		{
			print_filesystem($user['shared_folders'], $leftside,$arguments);
		}

		$counter--;
	}
}

if (count($group_folders) > 0) {
  echo '<table border="0" cellpadding="0" cellspacing="0">';
  echo '<tr><td>'.$image['group'].'</td>';
  echo '<td>Group folders</td></tr></table>';
  print_filesystem($group_folders, '', '');
}

?>
