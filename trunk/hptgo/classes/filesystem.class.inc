<?php
/**
*   Copyright Intermesh 2003
*   Author: Merijn Schering <mschering@intermesh.nl>
*   Version: 1.0 Release date: 08 July 2003
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at your
*   option) any later version.
*/

/**
 * Class filesystem is used by module filesystem and other modules to provide
 * a filesystem interface.
 */
class filesystem extends db
{
/**
* @var	array	$search_result
*/
  var $search_results = array();
/**
* @var	bool	$disable_go_permissions
*/
  var $disable_go_permissions = false;
/**
* @var	bool	$action_result
*/
  var $action_result = false;

/**
* @var	string	the root dir
*/
  var $root = '';

  /**
   * Get filesystem settings from fs_settings.
   *
   * @param 	integer $user_id
   * @return	array	the setting record
   */
  function get_settings($user_id)
  {
    $this->query("SELECT * FROM fs_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO fs_settings (user_id, sort_field, sort_order) 
	VALUES ('$user_id', 'basename', 'ASC')");
      return $this->get_settings($user_id);
    }		
  }

  /**
   * Set filesystem sort setting.
   *
   * @param integer $user_id
   * @param string $sort_field sort field
   * @param string $sort_order sort order
   * @return bool
   */
  function set_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE fs_settings SET sort_field='$sort_field' , 
      sort_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
   * Check if $sub_path is a subdirectory of $parent_path.
   *
   * @param		string	$sub_path
   * @param		string	$parent_path 
   * @return	bool	true if it is, false otherwise.
   */
  function is_sub_dir($sub_path, $parent_path)
  {
    if (strpos($sub_path, $parent_path) === false)
    {
      return false;
    }else
    {
      if (substr($sub_path, strlen($parent_path), 1) != '/')
      {
	return false;
      }
    }
    return true;
  }

  /**
   * Filesystem class constructor.
   *
   * @param bool $disable_go_permissions ignore checking go permissions
   */
  function filesystem($disable_go_permissions=false)
  {
    global $GO_CONFIG;
    $this->db();
    $this->disable_go_permissions = $disable_go_permissions;
    $this->root = $GO_CONFIG->file_storage_path;
    while ($this->root[strlen($this->root)-1] == '/')
      $this->root = substr($this->root,0,strlen($this->root)-1);
    
    // WORKAROUND: used when file_storage_path has a trailing slash
    $GO_CONFIG->file_storage_path = $this->root;
  }

  /**
   * Check if $path is the home directory of user $user_id.
   *
   * @param integer $user_id
   * @param string $path
   * @return bool
   */
  function is_home_path($user_id, $path)
  {
    global $GO_CONFIG, $GO_USERS;

    if ($this->root != $GO_CONFIG->file_storage_path)
      return false;

    if ($user = $GO_USERS->get_user($user_id))
    {
      $home_path = '/users/'.$user['username'];

      if (dirname($path).basename($path) == dirname($home_path).basename($home_path))
      {
	return true;
      }
    }
    return false;
  }

  /**
   * Check if $path is owned by user $user_id.
   *
   * @param integer $user_id
   * @param string $path
   * @return bool
   */
  function is_owner($user_id, $path)
  {
    global $GO_CONFIG, $GO_USERS;

    if ($this->root != $GO_CONFIG->file_storage_path)
      return false;

    if ($user = $GO_USERS->get_user($user_id))
    {
      $home_path = '/users/'.$user['username'];
      if (strpos($path, $home_path) === 0)
      {
	return true;
      }
    }

    $home_path = '/projects/';
    global $GO_MODULES,$GO_SECURITY;
    $project = $GO_MODULES->get_module('projects');
    if ($project && ereg('^'.$home_path.'/*([0-9]*)/([0-9]*)/*',$path,$matches)) {
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $project['acl_read']))
        return true;
      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $project['acl_write']))
      {
        require_once($project['class_path'].'projects.class.inc');
        require_once($project['class_path'].'task.class.inc');
        $prjs = new projects();
        $prj = $prjs->get_project($matches[1]);
        $db = new db();
        $db->query("SELECT task_person_id FROM task WHERE task_project_id=".(int)$matches[1]." AND task_id=".(int)$matches[2]);
        if ($GO_SECURITY->user_id == $prj['user_id'] ||
            ($db->next_record() && $db->f('task_person_id') == $GO_SECURITY->user_id))
           return true;
      }
    }


    if ($this->is_common_folder($path) &&
	$GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
      return true;

    return false;
  }

  /**
   * Check if $folder is a common folder.
   *
   * @param string $folder
   * @return bool
   */
  function is_common_folder($folder)
  {
    global $GO_CONFIG,$GO_SECURITY;

    if ($this->root != $GO_CONFIG->file_storage_path)
      return false;

    $home_path = '/common';
    $ret = substr($folder,0,strlen($home_path)) == $home_path;
    /*
    if ($ret &&
	$GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
    {
      $fs2 = new filesystem();
      if (!$fs2->get_share($folder))
	$fs2->add_share($GO_SECURITY->user_id,$folder);
    }
    */
    //echo "Common $folder is $ret<br/>\n";
    return $ret;
  }

  /**
   * Check if $folder is a system folder.
   *
   * @param string $folder
   * @return bool
   */
  function is_system_folder($folder)
  {
    global $GO_CONFIG;

    if ($this->root != $GO_CONFIG->file_storage_path)
      return false;

    if ('/common' == $folder)
      return true;
    $db = new db();
    return $db->query("SELECT * FROM fsSystemFolders WHERE path='".$folder."'") && $db->num_rows();
  }

  /**
   * Rename/Move a system folder from $old_folder to $new_path.
   *
   * @param string $old_folder
   * @param string $new_folder
   * @return bool
   */
  function update_system_folder($old_folder,$new_folder)
  {
    $db = new db();
    if ($this->is_common_folder($new_folder))
      return $db->query("UPDATE fsSystemFolders SET path='$new_folder' WHERE path='".$old_folder."'");
    else
      return $db->query("DELETE FROM fsSystemFolders WHERE path='".$old_folder."'");
  }

  /**
   * Get all shares of user $user_id.
   * Return the number of shares. 
   * Share information can be retrieved through db interface (table fsShares).
   *
   * @param integer $user_id
   * @return int number of shares
   */
  function get_shares($user_id)
  {
    //ORDER BY PATH important so higher order shares come first
    $sql = "SELECT * FROM fsShares WHERE user_id='$user_id' ORDER BY path ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get all users who share files to me.
   *
   * @return array an array of user_id. 
   */
  function get_my_shares()
  {
    global $GO_SECURITY;
/*    $sql = "SELECT DISTINCT fsShares.user_id FROM fsShares, acl, users_groups WHERE (".
      "fsShares.acl_read = acl.acl_id OR fsShares.acl_write = acl.acl_id AND fsShares.user_id != ".$GO_SECURITY->user_id.
      ") AND ( ( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$GO_SECURITY->user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$GO_SECURITY->user_id." ) )";*/
    $sql = "SELECT DISTINCT fsShares.user_id, fsShares.acl_read, fsShares.acl_write FROM ".
	   "fsShares, acl WHERE ( fsShares.acl_read = acl.acl_id OR fsShares.acl_write = acl.acl_id )";//AND fsShares.user_id != ".$GO_SECURITY->user_id." )";
    $this->query($sql);
    $list = array();
    while ( $this->next_record() ) {
      if ( $GO_SECURITY->has_permission( $GO_SECURITY->user_id, $this->f( 'acl_read' ) ) ||
	  $GO_SECURITY->has_permission( $GO_SECURITY->user_id, $this->f( 'acl_write' ) ) ) {
	if ( !in_array( $this->f('user_id'), $list ) ) {
	  $list[] = $this->f('user_id');
	}
      }
    }
    return $list;
  }

  /**
   * Get all users who share files to user $user_id.
   * Return the number of users. 
   * User info can be retrieved through db interface. 
   *
   * @param int $user_id
   * @return int number of shares
   */
  function get_authorized_sharers($user_id)
  {
    global $GO_SECURITY;
    $sql = "SELECT DISTINCT fsShares.user_id FROM fsShares, acl, users_groups WHERE (".
      "fsShares.acl_read = acl.acl_id OR fsShares.acl_write = acl.acl_id AND fsShares.user_id != ".$user_id.
      ") AND ( ( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id." ) )";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Make directory $path sharable by user $user_id.
   *
   * @param		int		$user_id
   * @param		string	$path
   * @return 	bool	true if successful, false otherwise. 
   */
  function add_share($user_id, $path)
  {
    $path = addslashes($path);

    global $GO_SECURITY;
    $acl_read = $GO_SECURITY->get_new_acl('read: '.$path);
    $acl_write = $GO_SECURITY->get_new_acl('write: '.$path);
    if($acl_read && $acl_write)
    {
      $sql = "INSERT INTO fsShares (user_id, path, acl_read, acl_write) 
	VALUES ('$user_id', '$path', '$acl_read', '$acl_write')";
	
      global $GO_CONFIG;
      if($GO_CONFIG->dav_switch) {
	global $GO_DAV;
	$GO_DAV->add_share($user_id, $path);
      }
      
      if ($this->is_common_folder($path))
      {
	$parent = dirname($path);
	$parentfs = new filesystem();
	$share = $parentfs->get_share($parent);
	if ($share)
	{
	  $GO_SECURITY->copy_acl($share['acl_read'],$acl_read);
	  $GO_SECURITY->copy_acl($share['acl_write'],$acl_write);
	}
      }
      

      return $this->query($sql);
    }else
    {
      $GO_SECURITY->delete_acl($acl_read);
      $GO_SECURITY->delete_acl($acl_write);
    }

    return false;
  }

  /**
   * Make directory $path unsharable.
   * @param		string	$path	
   * @return	bool	true if successful, false otherwise. 
   */
  function delete_share($path)
  {
    if ($share = $this->get_share($path))
    {
      $path = addslashes($path);
      global $GO_SECURITY;
      $GO_SECURITY->delete_acl($share['acl_read']);
      $GO_SECURITY->delete_acl($share['acl_write']);

      $sql = "DELETE FROM fsShares WHERE path='$path'";

      global $GO_CONFIG;
      if($GO_CONFIG->dav_switch) {
	global $GO_DAV;
	$GO_DAV->delete_share($path);
      }

      return $this->query($sql);
    }

    return false;
  }

  /**
   * Rename/Move a shared directory from $old_path to $new_path.
   * @param		string	$old_path
   * @param		string	$new_path
   * @return 	bool	true if successful, false otherwise. 
   */
  function update_share($old_path, $new_path)
  {
    $new_path = addslashes($new_path);
    $old_path = addslashes($old_path);

    $sql = "UPDATE fsShares SET path='$new_path' WHERE path='$old_path'";

    global $GO_CONFIG;
    if($GO_CONFIG->dav_switch) {
      global $GO_DAV;
      $GO_DAV->update_share($old_path, $new_path);
    }

    return $this->query($sql);
  }

  /**
   * Check if folder $path is sharable or not.
   * A folder is sharable if it's in fsShares table. For checking if the parent directory is sharable, use find_share() instead.
   * @param		string	$path
   * @return	mixed	the fsShares record sharable, false otherwise. 
   */
  function get_share($path)
  {
    $path = addslashes($path);
    $sql = "SELECT * FROM fsShares WHERE path='$path'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Check if folder $path or its parent is sharable.
   * @param		string	$path
   * @return	mixed	the shared folder (which is in fsShares), false otherwise. 
   */
  function find_share($path)
  {
    if ($share = $this->get_share($path))
    {
      return $share;
    }else
    {
      global $GO_CONFIG;
      $parent = dirname($path);
      if ($parent == '/' || $parent == '' || $parent == '.' || $parent == $GO_CONFIG->slash)
      {
	return false;
      }else
      {
	// prevent loops on Windows when $parent reachs C:\
	if (stristr(php_uname(), 'windows') && ereg('^[A-Za-z]:[\\/]?$', $parent))
    return false;
	return $this->find_share($parent);
      }
    }
  }

  /**
   * Check if user $user_id has read/write permission on shared folder $path.
   * @param		int		$user_id
   * @param		string	$path
   * @return	bool	true if successful, false otherwise. 
   */
  function has_read_permission2($user_id, $path)
  {
    return $this->has_read_permission($user_id,$path) || $this->has_write_permission($user_id,$path);
  }

  /**
   * Check if user $user_id has read permission on shared folder $path.
   * A folder is readable to an user if the user owns the folder, or the folder is shared to the user by another.
   * if $disable_go_permissions is true, then owner checking is ignored.
   * @param		int		$user_id
   * @param		string	$path
   * @return true if successful, false otherwise. 
   */
  function has_read_permission($user_id, $path)
  {
    if ($this->disable_go_permissions || $this->is_owner($user_id, $path))
    {
      return $this->chroot_is_readable($path);
    }else
    {
      if ($share = $this->find_share($path))
      {
	global $GO_SECURITY;
	if($GO_SECURITY->has_permission($user_id, $share['acl_read']))
	{
	  return $this->chroot_is_readable($path);
	}
      }
      global $GO_CONFIG;
      if (strpos($path, $GO_CONFIG->tmpdir) === 0)
      {
	return $this->chroot_is_readable($path);
      }
    }
    return false;
  }

  /**
   * Check if user $user_id has write permission on shared folder $path.
   * A folder is readable to an user if the user owns the folder, or the folder is shared to the user by another.
   * if $disable_go_permissions is true, then owner checking is ignored.
   * @param		int		$user_id
   * @param		string	$path
   * @return	bool	true if successful, false otherwise. 
   */
  function has_write_permission($user_id, $path)
  {
    if ($this->disable_go_permissions || $this->is_owner($user_id, $path))
    {
      return $this->chroot_is_writable($path);
    }else
    {
      global $GO_SECURITY;
      if ($share = $this->find_share($path))
      {
	if($GO_SECURITY->has_permission($user_id, $share['acl_write']))
	{
	  return $this->chroot_is_writable($path);
	}
      }
      global $GO_CONFIG;
      if (strpos($path, $GO_CONFIG->tmpdir) === 0)
      {
	return $this->chroot_is_writable($path);
      }
    }
    return false;
  }

  /**
   * Get the size of folder/file $path. If it's a folder, count the size of its files/subfolders.
   * @param		string	$path
   * @return	int		the size of $path. 
   */
  function size($path)
  {
    if ($this->chroot_is_dir($path))
    {
      $size = 0;
      $children = $this->get_folders($path);
      while ($child = array_shift($children))
      {
	$size += $this->size($child['path']);
      }

      $files = $this->get_files($path);
      while ($file = array_shift($files))
      {
	$size += $file['size'];
      }
      return $size;
    }else
    {
      return $this->chroot_filesize($path);
    }
  }

  /**
   * Move file/folder $source_path to $destination_path.
   * @param		string	$source_path
   * @param		string	$destination_path
   * @return	bool	true if successful, false otherwise. 
   */
  function move($source_path, $destination_path)
  {
    global $GO_CONFIG;
    //do not move into own path
    $source_dir_count = count(explode('/',$source_path));
    $destination_dir_count = count(explode('/',$destination_path));

    if ((strpos($destination_path, $source_path) === 0) && ($destination_dir_count > $source_dir_count))
    {
      return false;
    }elseif($source_path == $destination_path)
    {
      return true;
    }else
    {
      if ($this->chroot_is_dir($source_path))
      {
	if (!$this->chroot_file_exists($destination_path))
	{
	  if (!$this->chroot_mkdir($destination_path, $GO_CONFIG->create_mode))
	  {
	    return false;
	  }else
	  {
	    if ($this->get_share($source_path))
	    {
	      $this->update_share($source_path, $destination_path);
	    }
	    if ($this->is_system_folder($source_path))
	    {
	      $this->update_system_folder($source_path,$destination_path);
	    }
	  }
	}
	$files = $this->get_files($source_path, true);
	while ($file = array_shift($files))
	{
	  if(!$this->move($file['path'], $destination_path.'/'.$file['name']))
	  {
	    return false;
	  }
	}
	$children = $this->get_folders($source_path);
	while ($child = array_shift($children))
	{

	  if (!$this->move($child['path'], $destination_path.'/'.$child['name']))
	  {
	    return false;
	  }else
	  {
	    if ($this->get_share($child['path']))
	    {
	      $this->update_share($child['path'], $destination_path.'/'.$child['name']);
	    }
	    if ($this->is_system_folder($source_path))
	    {
	      $this->update_system_folder($source_path,$destination_path);
	    }
	  }
	}
	//Here we have a little problem with WebDAV... Have to DELETE ALL files
	//and there are also hidden files in the folders...
	if($GO_CONFIG->dav_switch) {
	  $cmd = "rm -rf $source_path";
	  `$cmd`;
	  return true;
	} else {
	  return $this->chroot_rmdir($source_path);
	}
      }else
      {
	//rename fails when moving accross partitions
	//return rename($source_path, $destination_path);
	if ($this->chroot_copy($source_path, $destination_path))
	{

	  return $this->chroot_unlink($source_path);
	}
      }
    }
  }

  /**
   * Copy file/folder $source_path to $destination_path.
   * @param		string	$source_path
   * @param		string	$destination_path
   * @return 	bool	true if successful, false otherwise. 
   */
  function copy($source_path, $destination_path)
  {
    global $GO_CONFIG;
    //do not $this->chroot_copy into own path
    if (strpos($destination_path, $source_path) === 0)
    {
      // TODO add translation
      $this->action_result = "source and destination path are the same";
      return false;
    }else
    {
      if ($this->chroot_is_dir($source_path))
      {
	if (!$this->chroot_file_exists($destination_path))
	{
	  if (!$this->chroot_mkdir($destination_path, $GO_CONFIG->create_mode))
	  {
	    // TODO add translation
	    $this->action_result = "unable to create destination path";
	    return false;
	  }
	}
	$files = $this->get_files($source_path);
	while ($file = array_shift($files))
	{
	  if(!$this->chroot_copy($file['path'], $destination_path.'/'.$file['name']))
	  {
	    // this->action_result is set by the call itself.
	    return false;
	  }
	}
	$children = $this->get_folders($source_path);
	while ($child = array_shift($children))
	{
	  if (!$this->chroot_copy($child['path'], $destination_path.'/'.$child['name']))
	  {
	    // this->action_result is set by the call itself.
	    return false;
	  }
	}
	$this->action_result = true;
	return true;
      }else
      {
	// No directory. Check free space in destination_path.
	if ( $GO_CONFIG->user_quota != 0 ) {
	  // get file size of $source_path
	  $this->chroot_filesize( $source_path );
	  // extract user-home-path from destination_path
	  if ( strstr( $destination_path, $GO_CONFIG->file_storage_path ) ) {
	    $dest = substr( $destination_path, strlen( $GO_CONFIG->file_storage_path ) );
	    $dest = substr( $dest, 0, strpos( $dest, "/" ) );
	    exec( "du ".$GO_CONFIG->file_storage_path.$dest." -s", $retval );
	    list($usedspace) = sscanf( $retval[0], "%d");
	    if ( ( $usedspace*1024 + $this->chroot_filesize( $source_path ) ) >= $GO_CONFIG->user_quota*1024 ) {
	      // TODO add translation
	      $this->action_result = "not enough space (over quota)";
	      return false;
	    }
	  }
	}
	return $this->chroot_copy($source_path, $destination_path);
      }
    }
  }

  /**
   * Delete file/folder $source_path to $destination_path.
   * If it's a folder, delete all its contents.
   * Write permission is needed/checked to delete.
   * @param		string	$path
   * @return	bool	true if successful, false otherwise. 
   */
  function delete($path)
  {
    global $GO_SECURITY;
    if ($this->chroot_is_dir($path))
    {
      if ($this->is_system_folder($path) &&
	  !$GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
	return false;

      $children = $this->get_folders($path);
      while ($child = array_shift($children))
      {
	if (!$this->delete($child['path']))
	{
	  return false;
	}
      }

      $files = $this->get_files($path);
      while ($file = array_shift($files))
      {
	if (!$this->delete($file['path']))
	{
	  return false;
	}
      }
      if ($this->disable_go_permissions || 
      		$this->has_write_permission($GO_SECURITY->user_id, $path))
      {
	if (!$this->disable_go_permissions &&  $this->get_share($path))
	{
	  $this->delete_share($path);
	}
	if ($this->is_system_folder($source_path))
	{
	  $db = new db();
	  $db->query("DELETE FROM fsSystemFolders WHERE path='$source_path'");
	  unset($db);
	}

	global $GO_CONFIG;
	if($GO_CONFIG->dav_switch) {
	  global $GO_DAV;
	  if($GO_DAV->is_share($path))
	    $GO_DAV->delete_share($path);
	  $cmd = "rm -rf $path/.*";
	  `$cmd`;
	}

	return @$this->chroot_rmdir($path);
      }else
      {
	return false;
      }
    }else
    {
      if ($this->disable_go_permissions || 
     	 $this->has_write_permission($GO_SECURITY->user_id, $path))
      {
	return @$this->chroot_unlink($path);
      }else
      {
	return false;
      }
    }
  }

  /**
   * Get parent folder of folder $path.
   * @param		string	$path
   * @return	bool	false if unsuccessful.
   */
  function get_parent_path($path)
  {
    $last_folder_pos = strrpos($path, '/');
    if (is_integer($last_folder_pos))
    {
      if ($last_folder_pos === 0)
      {
	return '/';
      }else
      {
	return substr($path, 0, $last_folder_pos);
      }
    }else
    {
      return false;
    }
  }

  /**
   * Get subfolders of folder $path (faster then get_folders_sorted)
   * @param		string	$path
   * @return	mixed	an folder array.
   * Each element is an array ('path','name','mtime','size','type')
   * FIXME: more information about the array
   */
  function get_folders($path)
  {
    global $GO_CONFIG;
    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;

    $folders = array();
    if($dir = $this->chroot_opendir($path))
    {
      while($item=readdir($dir))
      {
	$folder_path = $path.$item;
	if ($this->chroot_is_dir($folder_path) && $item != "." && $item != ".." &&
	  $item != $GO_CONFIG->name_of_sharedir && !(strpos($item,".") === 0) )
	{
	  $folder['path'] = $folder_path;
	  $folder['name'] = basename($folder_path);
	  $folder['mtime'] = $this->chroot_filemtime($folder_path);
	  $folder['size'] = $this->chroot_filesize($folder_path);
	  $folder['type'] = $this->chroot_filetype($folder_path);
	  $folders[] = $folder;
	}
      }
      closedir($dir);
    }
    return $folders;
  }

  /**
   * Get common subfolders of folder $path, which is visible to user $user_id.
   * Retrieved folders may be direct children of $path or not.
   * @param		int		$user_id
   * @param		string	$path
   * @return	array	an folder array.
   * Each element is an array ('path','name','mtime','size','type')
   * FIXME: more information about the array
   */
  function get_visible_folders($user_id,$path)
  {
    global $GO_CONFIG;
    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;

    $folders = array();
    if($dir = $this->chroot_opendir($path))
    {
      while($item=readdir($dir))
      {
	$folder_path = $path.$item;
	if ($this->chroot_is_dir($folder_path) && $item != "." && $item != ".." &&
	  $item != $GO_CONFIG->name_of_sharedir && !(strpos($item,".") === 0) )
	{
	  if ($this->has_read_permission2($user_id,$folder_path))
	  {
	    //echo "Got $folder_path<br/>\n";
	    $folder['path'] = $folder_path;
	    $folder['name'] = basename($folder_path);
	    $folder['mtime'] = $this->chroot_filemtime($folder_path);
	    $folder['size'] = $this->chroot_filesize($folder_path);
	    $folder['type'] = $this->chroot_filetype($folder_path);
	    $folders[] = $folder;
	  }
	  else
	  {
	    //echo "Dive into $folder_path<br/>\n";
	    $subfolders = $this->get_visible_folders($user_id,$folder_path);
	    if (is_array($subfolders))
	      foreach ($subfolders as $folder)
		$folders[]  = $folder;
	  }
	}
      }
      closedir($dir);
    }
    return $folders;
  }

 /**
 * Get visible folders array
 *
 * @param	int		$user_id
 * @param	string	$path
 * @param	string	$sort_field
 * @return	array	folder
 */
  function get_visible_folders_array($user_id,$path,$sort_field)
  {
    global $GO_CONFIG;
    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;

    $folders = array();
    if($dir = $this->chroot_opendir($path))
    {
      while($item=readdir($dir))
      {
	$folder_path = $path.$item;
	if ($this->chroot_is_dir($folder_path) && $item != "." && $item != ".." &&
	  $item != $GO_CONFIG->name_of_sharedir && !(strpos($item,".") === 0) )
	{
	  if ($this->has_read_permission2($user_id,$folder_path))
	  {
	    $key_id = 0;
	    $first_key = strtolower($sort_field($folder_path));
	    $key = $first_key;
	    while (array_key_exists($key, $folders))
	    {
	      $key = $first_key.'_'.$key_id;
	      $key_id++;
	    }
	    $folders[$key] = $folder_path;
	  }
	  else
	  {
	    //echo "Dive into $folder_path<br/>\n";
	    $subfolders = $this->get_visible_folders($user_id,$folder_path);
	    if (is_array($subfolders))
	      foreach ($subfolders as $folder_path) {
	    	$key_id = 0;
	    	$first_key = strtolower($sort_field($folder_path));
	    	$key = $first_key;
	    	while (array_key_exists($key, $folders))
	    	{
	      	  $key = $first_key.'_'.$key_id;
	      	  $key_id++;
	    	}
	    	$folders[$key] = $folder_path;
	      }
	  }
	}
      }
      closedir($dir);
    }
    return $folders;
  }

  /**
   * Get common subfolders of folder $path, which is visible to user $user_id,
   * sorted by $sort_field with $sort_direction
   * Retrieved folders may be direct children of $path or not.
   * @param		int		$user_id
   * @param		string	$path
   * @param		string	$sort_field
   * @param		string	$sort_direction
   * @return 	array	an folder array.
   * Each element is an array ('path','name','mtime','size','type')
   * FIXME: more information about the array
   */
  function get_visible_folders_sorted($user_id,$path,$sort_field='basename',$sort_direction='ASC')
  {
    global $GO_CONFIG;
    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;
    if(strstr($_SERVER['HTTP_HOST'], $path))
    {
      $url = str_replace($_SERVER['DOCUMENT_ROOT'], 'http://'.$_SERVER['HTTP_HOST'], $path);
      if ($slash == '\\')
      {
	$url = str_replace('\\','/',$url);
      }
    }
    $sort_field = function_exists($sort_field) ? $sort_field : 'basename';
    $sorted_list = $this->get_visible_folders_array($user_id,$path,$sort_field);

    if ($sort_direction == 'ASC')
      ksort($sorted_list);
    else
      krsort($sorted_list);
    $folders = array();
    while ($item=array_shift($sorted_list))
    {
      $folder = array();
      $folder['path'] = $item;
      $folder['name'] = basename($item);
      $folder['mtime'] = $this->chroot_filemtime($item);
      $folder['size'] = $this->chroot_filesize($item);
      $folder['type'] = $this->chroot_filetype($item);
      if(isset($url))
	$folder['url'] = $url.$folder['name'];
      $folders[] = $folder;
    }
    return $folders;
  }
  
  /**
   * Get subfolders of folder $path sorted by $sort_field with $sort_direction
   * returns all subfolders of a folder sorted based on the result of a function
   * passed that is performed on the pathname. (For example $this->chroot_filesize();)
   * @param		string	$path
   * @param		string	$sort_field
   * @param		string	$sort_direction
   * @return	array	an folder array.
   * Each element is an array ('path','name','mtime','size','type')
   * FIXME: more information about the array
   */
  function get_folders_sorted($path,$sort_field='basename',$sort_direction='ASC')
  {
    global $GO_CONFIG;
    $folders = array();
    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;
    if(strstr($_SERVER['HTTP_HOST'], $path))
    {
      $url = str_replace($_SERVER['DOCUMENT_ROOT'], 'http://'.$_SERVER['HTTP_HOST'], $path);
      if ($slash == '\\')
      {
	$url = str_replace('\\','/',$url);
      }
    }
    $sort_field = function_exists($sort_field) ? $sort_field : 'basename';
    if ($this->chroot_is_dir($path))
    {
      $sorted_list = array();

      if(@$dir = $this->chroot_opendir($path))
      {
	while($item=readdir($dir))
	{
	  $folder_path = $path.$item;

	  if ($this->chroot_is_dir($folder_path) && $item != "." && $item != ".." &&
	    $item != $GO_CONFIG->name_of_sharedir && !(strpos($item,".")===0) )
	  {
	    $key_id = 0;
	    $first_key = strtolower($sort_field($folder_path));
	    $key = $first_key;
	    while (array_key_exists($key, $sorted_list))
	    {
	      $key = $first_key.'_'.$key_id;
	      $key_id++;
	    }
	    $sorted_list[$key] = $folder_path;
	  }
	}
	closedir($dir);

	if ($sort_direction == 'ASC')
	{
	  ksort($sorted_list);
	}else
	{
	  krsort($sorted_list);
	}

	while ($item=array_shift($sorted_list))
	{
	  $folder = array();
	  $folder['path'] = $item;
	  $folder['name'] = basename($item);
	  $folder['mtime'] = $this->chroot_filemtime($item);
	  $folder['size'] = $this->chroot_filesize($item);
	  $folder['type'] = $this->chroot_filetype($item);
	  if(isset($url))
	  {
	    $folder['url'] = $url.$folder['name'];
	  }
	  $folders[] = $folder;
	}
      }
    }
    return $folders;
  }

  /**
   * Get files/subfolders of folder $path (faster then get_files_sorted)
   * @param		string	$path
   * @param		bool	$move
   * @return 	array	an file array.
   * Each element is an array ('path','name','mtime','size','type')
   * FIXME: more information about the array
   */
  function get_files($path, $move=false)
  {
    global $GO_CONFIG;
    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;
    $files = array();
    if($dir = @$this->chroot_opendir($path))
    {
      while($item=readdir($dir))
      {
	$file_path = $path.$item;
	if (!$this->chroot_is_dir($file_path) && $move) {
	  $file['path'] = $file_path;
	  $file['name'] = basename($file_path);
	  $file['size'] = $this->chroot_filesize($file_path);
	  $file['mtime'] = $this->chroot_filemtime($file_path);
	  $file['type'] = $this->chroot_get_mime_type($file_path);

	  $files[] = $file;
	}
	if (!$this->chroot_is_dir($file_path) && !$move &&
	  $item != $GO_CONFIG->name_of_sharedir && !(strpos($item,".") === 0) )
	{
	  $file['path'] = $file_path;
	  $file['name'] = basename($file_path);
	  $file['size'] = $this->chroot_filesize($file_path);
	  $file['mtime'] = $this->chroot_filemtime($file_path);
	  $file['type'] = $this->chroot_get_mime_type($file_path);

	  $files[] = $file;
	}
      }
      closedir($dir);
    }
    return $files;
  }

  /**
   * Get files/subfolders of folder $path sorted by $sort_field with sort order $sort_direction
   * returns all subfolders of a folder sorted based on the result of a function
   * passed that is performed on the pathname. (For example $this->chroot_filesize();)
   * @param		string	$path	
   * @param		string	$sort_field
   * @param		string	$sort_direction
   * @return 	array	an file array.
   * Each element is an array ('path','name','mtime','size','type')
   * FIXME: more information about the array
   */
  function get_files_sorted($path,$sort_field='basename',$sort_direction='ASC')
  {
    global $GO_CONFIG;
    $files = array();

    $slash = stristr(PHP_OS, 'Windows') ? '\\' : '/';
    if (substr($path, -1) != $slash) $path .= $slash;

    if(strstr($_SERVER['HTTP_HOST'], $path))
    {
      $url = str_replace($_SERVER['DOCUMENT_ROOT'], 'http://'.$_SERVER['HTTP_HOST'], $path);
      if ($slash == '\\')
      {
	$url = str_replace('\\','/',$url);
      }
    }
    $sort_field = function_exists($sort_field) ? $sort_field : 'basename';
    if ($this->chroot_is_dir($path))
    {
      $sorted_list = array();

      if($dir = @$this->chroot_opendir($path))
      {
	while($item=readdir($dir))
	{
	  $file = $path.$item;
	  if (!$this->chroot_is_dir($file) &&
	    $item != $GO_CONFIG->name_of_sharedir && !(strpos($item,".") === 0))
	  {
	    $key_id = 0;
	    $first_key = strtolower($sort_field($file));
	    $key = $first_key;
	    while (array_key_exists($key, $sorted_list))
	    {
	      $key = $first_key.'_'.$key_id;
	      $key_id++;
	    }
	    $sorted_list[$key] = $file;
	  }
	}
	closedir($dir);

	if ($sort_direction == 'ASC')
	{
	  ksort($sorted_list);
	}else
	{
	  krsort($sorted_list);
	}

	while ($item=array_shift($sorted_list))
	{
	  $file = array();
	  $file['path'] = $item;
	  $file['name'] = basename($item);
	  $file['mtime'] = $this->chroot_filemtime($item);
	  $file['size'] = $this->chroot_filesize($item);
	  $file['type'] = $this->chroot_get_mime_type($item);

	  if(isset($url))
	  {
	    $file['url'] = $url.$file['name'];
	  }
	  $files[] = $file;
	}
      }

    }
    return $files;
  }

  /**
   * FIXME
   */
  function search($path, $keyword, $modified_later_then=0, $modified_earlier_then=0)
  {
    global $GO_SECURITY;

    if ($modified_earlier_then == 0)
    {
      $modified_earlier_then = time();
    }

    if($this->has_read_permission2($GO_SECURITY->user_id, $path))
    {
      $folders = $this->get_folders($path);
      while ($folder = array_shift($folders))
      {
	$this->search($folder['path'], $keyword, $modified_later_then, $modified_earlier_then);
      }
    }
    $folder['path'] = $path;
    $folder['name'] = basename($path);
    $folder['mtime'] = $this->chroot_filemtime($path);
    $folder['size'] = $this->chroot_filesize($path);
    $folder['type'] = $this->chroot_filetype($path);

    if (stristr(basename($path), $keyword) && $modified_later_then < $folder['mtime'] && $modified_earlier_then > $folder['mtime'])
    {
      $this->search_results[] = $folder;
    }

    $files = $this->get_files($path);
    while ($file = array_shift($files))
    {
      if (stristr($file['name'], $keyword) && $modified_later_then < $file['mtime'] && $modified_earlier_then > $file['mtime'])
      {
	$this->search_results[] = $file;
      }
    }

    return $this->search_results;
  }

  /**
   * Delete home folder of user $user_id and his/her shares
   * @param	int	$user_id
   */
  function delete_user($user_id)
  {
    $fs = new filesystem();
    $this->get_shares($user_id);
    while($this->next_record())
    {
      $fs->delete_share($this->f('path'));
    }
  }

  /**
   * Make $path system folder.
   * @param		string		$path		path of file   
   * @param		string		$feedback	is the error message when things go wrong.
   * @return	bool		true if successful, false otherwise.
   */
  function add_system_folder($path,&$feedback)
  {
    global $GO_CONFIG;
    $parent = dirname($path);
    if ($path != '/common' &&
	!$this->is_system_folder($parent))
    {
      global $strParentMustBeSystemFolder;
      $feedback = $strParentMustBeSystemFolder;
      return false;
    }

    $db = new db();
    @$db->query("REPLACE INTO fsSystemFolders SET path='$path'");
    return true;
  }

  /**
   * Removed system folder attribute from folder $path.
   * @param		string	$path	path of file
   * @param		string	$msg	is the error message when things go wrong.
   * @return	bool	true if successful, false otherwise.
   */
  function delete_system_folder($path,&$msg)
  {
    $children = $this->get_folders($path);
    foreach ($children as $child)
    {
      if ($this->is_system_folder($child['path']))
      {
	global $strChildMustNotBeSystemFolder;
	$msg = $strChildMustNotBeSystemFolder;
	return false;
      }
    }
    $db = new db();
    @$db->query("DELETE FROM fsSystemFolders WHERE path='$path'");
    return true;
  }


  function chroot_file_exists($path)
  {
    return file_exists($this->root.$path);
  }

  function chroot_is_writable($dir)
  {
    return is_writable($this->root.$dir);
  }

  function chroot_is_readable($dir)
  {
    return is_readable($this->root.$dir);
  }

  function chroot_is_dir($dir)
  {
    return is_dir($this->root.$dir);
  }

  function chroot_mkdir($dir,$mode = -1)
  {
    return $mode != -1 ? mkdir($this->root.$dir,$mode) : mkdir($this->root.$dir);
  }

  function chroot_rmdir($dir)
  {
    return rmdir($this->root.$dir);
  }

  function chroot_filesize($path)
  {
    return filesize($this->root.$path);
  }

  function chroot_copy_r2($p1,$p2)
  {
    return copy($this->root.$p1,$p2);
  }

  function chroot_copy_r1($p1,$p2)
  {
    return copy($p1,$this->root.$p2);
  }

  function chroot_copy($p1,$p2)
  {
    return copy($this->root.$p1,$this->root.$p2);
  }

  function chroot_unlink($path)
  {
    return unlink($this->root.$path);
  }

  function chroot_fileatime($p)
  {
    return fileatime($this->root.$p);
  }

  function chroot_filectime($p)
  {
    return filectime($this->root.$p);
  }

  function chroot_filemtime($p)
  {
    return filemtime($this->root.$p);
  }

  function chroot_filetype($p)
  {
    return filetype($this->root.$p);
  }

  function chroot_opendir($d)
  {
    return opendir($this->root.$d);
  }

  function chroot_get_mime_type($path)
  {
    return get_mime_type($this->root.$path);
  }

  function chroot_fopen($path,$mode)
  {
    return fopen($this->root.$path,$mode);
  }
}
