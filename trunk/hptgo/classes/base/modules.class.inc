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

class GO_MODULES extends db
{
  var $read_permissions = false;
  var $write_permissions = false;
  var $path;
  var $id;
  var $url;
  var $full_url;
  var $class_path;

  function GO_MODULES()
  {
    $this->db();
  }
  function authenticate($module_id, $admin=false)
  {
    global $GO_CONFIG, $GO_SECURITY;
    if($module = $this->get_module($module_id))
    {
      $_SESSION['GO_SESSION']['active_module'] = $module_id;
      $this->path = $GO_CONFIG->root_path.'modules'.$GO_CONFIG->slash.$module_id.$GO_CONFIG->slash;
      $this->class_path = $this->path.'classes'.$GO_CONFIG->slash;
      $this->read_permissions = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']);
      $this->write_permissions = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']);
      $this->id = $module_id;
      $this->full_url = $GO_CONFIG->full_url.'modules/'.$module_id.'/';
      $this->url = $GO_CONFIG->host.'modules/'.$module_id.'/';

      if ($this->read_permissions || $this->write_permissions)
      {
	if ($admin)
	{
	  return $this->write_permissions;
	}else
	{
	  return true;
	}
      }else
      {
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
      }
    }else
    {
      exit('Illegal module name specified');
    }
  }

  function has_read_permission($user_id, $module_id)
  {
    global $GO_SECURITY;
    $module = $this->get_module($module_id);
    if($GO_SECURITY->has_permission($user_id, $module['acl_read']) ||
	$GO_SECURITY->has_permission($user_id, $module['acl_write']))
    {
      return true;
    }else
    {
      return false;
    }
  }


  function has_write_permission($user_id, $module_id)
  {
    global $GO_SECURITY;
    $module = $this->get_module($module_id);
    return $GO_SECURITY->has_permission($user_id, $module['acl_write']);
  }

  function get_module($module_id)
  {
    global $GO_CONFIG;

    $sql = "SELECT * FROM modules WHERE id='".addslashes($module_id)."'";
    $this->query($sql);
    if ($this->next_record())
    {			
      $this->Record['full_url'] = $GO_CONFIG->full_url.'modules/'.$module_id.'/';
      $this->Record['url'] = $GO_CONFIG->host.'modules/'.$module_id.'/';
      $this->Record['path'] = $GO_CONFIG->root_path.'modules'.$GO_CONFIG->slash.$module_id.$GO_CONFIG->slash;
      $this->Record['class_path'] = $this->Record['path'].'classes'.$GO_CONFIG->slash;
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function add_module($module_id, $version, $acl_read, $acl_write)
  {
    global $GO_CONFIG; 

    $sql = 	"INSERT INTO modules (id, version, acl_read, acl_write) VALUES ".
      "('".addslashes($module_id)."', '$version',".
      "'$acl_read', '$acl_write')";

    if($this->query($sql))
    {
      $install_sql_file = $GO_CONFIG->root_path.'modules'.
	$GO_CONFIG->slash.$module_id.$GO_CONFIG->slash.'sql'.
	$GO_CONFIG->slash.$module_id.'.install.sql';

      if (file_exists($install_sql_file) && $queries = get_sql_queries($install_sql_file))
      {
	while($query = array_shift($queries))
	{
	  $this->query($query);
	}
      }
      return true;
    }
    return false;
  }

  function delete_module($module_id)
  {
    global $GO_SECURITY, $GO_CONFIG;
    if ($module = $this->get_module($module_id))
    {
      $GO_SECURITY->delete_acl($module['acl_read']);
      $GO_SECURITY->delete_acl($module['acl_write']);
      $sql = "DELETE FROM modules WHERE id='".$module_id."'";
      if($this->query($sql))
      {
	$uninstall_sql_file = $GO_CONFIG->root_path.'modules'.
	  $GO_CONFIG->slash.$module_id.$GO_CONFIG->slash.'sql'.
	  $GO_CONFIG->slash.$module_id.'.uninstall.sql';

	if (file_exists($uninstall_sql_file) && $queries = get_sql_queries($uninstall_sql_file))
	{
	  while($query = array_shift($queries))
	  {
	    $this->query($query);
	  }					
	}
      }
      return true;			
    }
    return false;
  }

  function get_modules()
  {
    $sql = "SELECT * FROM modules ORDER BY id ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_modules_with_locations()
  {
    global $GO_CONFIG;

    $modules = array();
    $sql = "SELECT * FROM modules ORDER BY id ASC";
    $this->query($sql);
    while($this->next_record())
    {
      $this->Record['full_url'] = $GO_CONFIG->full_url.'modules/'.$this->f('id').'/';
      $this->Record['url'] = $GO_CONFIG->host.'modules/'.$this->f('id').'/';
      $this->Record['path'] = $GO_CONFIG->root_path.'modules'.$GO_CONFIG->slash.$this->f('id').$GO_CONFIG->slash;
      $this->Record['class_path'] = $this->Record['path'].'classes'.$GO_CONFIG->slash;
      $modules[] = $this->Record;
    }
    return $modules;
  }

  function get_plugin($plugin_id, $module_id='')
  {
    global $GO_CONFIG;

    if ($module_id == '')
    {
      $module_id = $this->id;
      $module_path = $this->path;
    }else
    {
      if(!$module = $this->get_module($module_id))
      {
	return false;
      }else
      {
	$module_path = $module['path'];
      }
    }

    $plugin['id'] = $plugin_id;
    $plugin['path'] = $module_path.$plugin_id.$GO_CONFIG->slash;
    $plugin['class_path'] = $plugin['path'].'classes'.$GO_CONFIG->slash;

    if(file_exists($plugin['path']))
    {
      $plugin['full_url'] = $GO_CONFIG->full_url.'modules/'.$module_id.'/'.$plugin_id.'/';
      $plugin['url'] = $GO_CONFIG->host.'modules/'.$module_id.'/'.$plugin_id.'/';
      return $plugin;
    }else
    {
      return false;
    }
  }
}
?>
