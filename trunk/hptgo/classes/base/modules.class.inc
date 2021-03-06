<?php
/**
*   Copyright Intermesh 2003
*   @author		Merijn Schering <mschering@intermesh.nl>
*   @version	1.0 Release date: 08 July 2003
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at your
*   option) any later version.
*/

class GO_MODULES extends db
{
	/**
	* @var bool
	*/
  var $read_permissions = false;
	/**
	* @var bool
	*/
  var $write_permissions = false;
	/**
	* @var string
	*/
  var $path;
	/**
	* @var int
	*/
  var $id;
	/**
	* @var string
	*/
  var $url;
	/**
	* @var string
	*/
  var $full_url;
	/**
	* @var string
	*/
  var $class_path;
	/**
	* @var string
	*/
  var $modules;

	/**
	* Constructor
	*/
  function GO_MODULES()
  {
  	global $GO_SECURITY;
  	
    $this->db();
  
		if($GO_SECURITY->logged_in)
		{
			$modules = $this->get_modules_with_locations();
			for($i=0;$i<count($modules);$i++)
			{
				$this->modules[$modules[$i]['id']] = $modules[$i];				
				
				$this->modules[$modules[$i]['id']]['enable'] = $modules[$i]['enable'];
				$this->modules[$modules[$i]['id']]['write_permission'] = 
					$GO_SECURITY->has_permission($_SESSION['GO_SESSION']['user_id'], 
						$modules[$i]['acl_write']);
						
				$this->modules[$modules[$i]['id']]['read_permission'] = 
					$this->modules[$modules[$i]['id']]['write_permission'] ? true : 
						$GO_SECURITY->has_permission($_SESSION['GO_SESSION']['user_id'], 
						$modules[$i]['acl_read']);
			}
		}
  }
  
  /**
   * Get information of module $module_id
   * @param		int		$module_id
   * @return	mixed	An array of module info, otherwise false 
   */
  function get_module_info($module_id)
  {
  	global $GO_CONFIG;
  	
  	$info_file = $GO_CONFIG->root_path.'modules'.$GO_CONFIG->slash.$module_id.$GO_CONFIG->slash.'module.info';
  	
  	if(file_exists($info_file))
  	{
  		require($info_file);
  		if(isset($module[$module_id]))
  		{
  			return $module[$module_id];
  		}
  	}
  	return false;  	
  }
  
  /**
   * Check if the current user has permission to access (read/write) module $module_id
   * redirect to error_docs/403.php when failed
   * @param		int		$module_id
   * @param		bool	$admin
   * @return	bool	true if succesful.
   */
  function authenticate($module_id, $admin=false)
  {
    global $GO_CONFIG, $GO_SECURITY;
    if(isset($this->modules[$module_id]))
    {
      $module = $this->modules[$module_id];
      $_SESSION['GO_SESSION']['active_module'] = $module_id;
      $this->path = $GO_CONFIG->root_path.'modules'.$GO_CONFIG->slash.$module_id.$GO_CONFIG->slash;
      $this->class_path = $this->path.'classes'.$GO_CONFIG->slash;
      $this->read_permissions = $module['read_permission'];
      $this->write_permissions = $module['write_permission'];
      $this->id = $module_id;
      $this->full_url = $GO_CONFIG->full_url.'modules/'.$module_id.'/';
      $this->url = $GO_CONFIG->host.'modules/'.$module_id.'/';

      if ($this->modules[$module_id]['enable'] &&
      ($this->read_permissions || $this->write_permissions))
      {
	if ($admin)
	{
	  if ($this->write_permissions)
	    return true;
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

  /**
   * Check if the current user has permission to read module $module_id
   * @param		int		$user_id
   * @param		int		$module_id
   * @return	bool	True if succesful, false otherwise.
   */
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


  /**
   * Check if the current user has permission to write module $module_id
   * @param		int		$user_id
   * @param		int		$module_id
   * @return	bool	True if succesful, false otherwise.
   */
  function has_write_permission($user_id, $module_id)
  {
    global $GO_SECURITY;
    $module = $this->get_module($module_id);
    return $GO_SECURITY->has_permission($user_id, $module['acl_write']);
  }

  /**
   * Return the record of module $module_id from table modules
   * @param		int		$module_id
   * @return	mixed	Record if exists, false otherwise.
   */
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

  /**
   * Add new module named $module_id with version $version and acl_{read,write} to system.
   * @param		int		$module_id
   * @param		string	$version
   * @param		int		$acl_read
   * @param		int		$acl_write
   * @return	bool	True if successful, false otherwise.
   */
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

  /**
   * Remove a module from named $module_id from system.
   * @param		int		$module_id
   * @return	bool	True if successful, false otherwise.
   */
  function delete_module($module_id)
  {
    global $GO_SECURITY, $GO_CONFIG;
    if ($module = $this->get_module($module_id))
    {
      $GO_SECURITY->delete_acl($module['acl_read']);
      $GO_SECURITY->delete_acl($module['acl_write']);
      $sql = "DELETE FROM modules WHERE id='".addslashes($module_id)."'";
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

  /**
   * Get all modules (accessed through db interface).
   * @return	int		the number of installed modules.
   */
  function get_modules()
  {
    $sql = "SELECT * FROM modules ORDER BY id ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get all modules (accessed through db interface).
   * This function is similiar to get_modules() with additional field (full_url, url, path, class_path)
   * @return	array	The array of modules.
   */
  function get_modules_with_locations()
  {
    global $GO_CONFIG;

    $modules = array();
    $this->get_modules();
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

  /**
   * Get plugin information.
   * Plugins reside in $GO_CONFIG->full_url/modules/module-name/plugin-name
   * @param		int		$plugin_id
   * @param		string	$module_id
   * @return	mixed	plugin (an array of ('id','path','class_path','full_url','url') if successful, false otherwise.
   */
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
