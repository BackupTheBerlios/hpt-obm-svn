<?php

function get_group_folders($uid, $level)
{
	global $GO_GROUPS, $GO_CONFIG;

  if ($level >= 2)
		return array();

	$group_folders = array();
	if ($GO_GROUPS->get_user_groups($uid))
	{
		$groups = array();

		while ($GO_GROUPS->next_record()) {
			$gid = $GO_GROUPS->f('group_id');
			if ($gid != $GO_CONFIG->group_everyone )
				$groups[$gid] = $GO_GROUPS->f('name');
		}

		$staffs = array();
		foreach( $groups as $gid => $gname )
			if ($GO_GROUPS->is_group_leader($gid, $uid)) {
				$GO_GROUPS->query(
					"SELECT users.id, users.username, users.first_name, users.middle_name , users.last_name ".
					"FROM users, users_groups ".
					"WHERE users.id=users_groups.user_id AND users.id <> $uid ".
					"AND users_groups.group_id=$gid");
				while ($GO_GROUPS->next_record())
				{
					$staff_middle_name = $GO_GROUPS->f('middle_name');
					if ($staff_middle_name)
						$staff_middle_name = ' '.$staff_middle_name.' ';
					else
						$staff_middle_name = ' ';
					$staffs[] = $GO_GROUPS->f('id');
					$folder['path'] = $GO_CONFIG->file_storage_path.$GO_GROUPS->f('username');
					$folder['name'] = $GO_GROUPS->f('last_name').$staff_middle_name.$GO_GROUPS->f('first_name');
					$group_folders[] = $folder;
				}
			}

		foreach ($staffs as $staff_id)
		{
			$gfolders = get_group_folders($staff_id, $level + 1);
			$group_folders = array_merge($group_folders, $gfolders);
		}
	}

	return $group_folders;
}

function is_group_folder($group_folders, $path)
{
	foreach ($group_folders as $gf)
	{
		$p = strpos($path, $gf['path']);
		if ($p === false) continue;
		if (is_integer($p) && $p == 0) return true;
	}
	return false;
}

?>
