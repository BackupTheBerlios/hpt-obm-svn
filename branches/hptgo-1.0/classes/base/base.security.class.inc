<?php
/**
* Copyright Intermesh 2003
*  Author: Merijn Schering <mschering@intermesh.nl>
* Version: 1.0 Release date: 08 July 2003
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
*  
* Do not modify this class unless you're bugfixing or inventing some
* truely great new stuff :). Instead extend this class with new
* authentication ans security management stuff.
*/

/**
* This is the base class of the user group management files.
* Don't use this class directly it should be extended by a security manager.
* For example sql.security.class.inc.
*
* @package  base.security.class.inc
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 2.05
*/

class base_security extends db
{
	/**
	* The user_id of the current logged in user
	*
	* @var     int
	* @access  public
	*/
  var $user_id = 0;
	
	/**
	* True if the current user is logged in
	*
	* @var     bool
	* @access  public
	*/ 
  var $logged_in = false;

	/**
	* Constructor. Initialised database connection and user ID
	*
	* @access public
	* @return void
	*/
  function base_security()
  {
    global $GO_CONFIG;
    $this->db();

    if (isset($_SESSION['GO_SESSION']['user_id']) &&
	$_SESSION['GO_SESSION']['user_id'] > 0)
    {
      $this->logged_in=true;
      $this->user_id=$_SESSION['GO_SESSION']['user_id'];
    }
  }

	/**
	* Set's a user as logged in. This does NOT log a user in. $GO_AUTH->login()
	* does that.
	*
	* @param	int	$user_id	The ID of the logged in user.
	* @access public
	* @return void
	*/
  function logged_in( $user_id ) {
    $this->user_id = $user_id;
    $this->logged_in = true;
  }

	/**
	* Log the current user out.
	*
	* @access public
	* @return void
	*/
  function logout()
  {
    session_destroy();
    $this->user_id = 0;
    $this->logged_in = false;
  }
  
  /**
	* Checks if a user is logged in. if not it attempts to log in
  * based on stored cookies. On failure it redirects the user to the login page.
	*
	* @param	bool	$admin	Check for administrator privileges too.
	* @access public
	* @return void
	*/
  function authenticate($admin = false)
  {
    global $GO_CONFIG, $GO_AUTH, $auth_sources, $link_back;

    $GO_AUTH_SOURCE_KEY = isset($_COOKIE['GO_AUTH_SOURCE_KEY']) ? $_COOKIE['GO_AUTH_SOURCE_KEY'] : 0;

    if ($this->logged_in == false)
    {
      if (!isset($_COOKIE['GO_UN']) || !isset($_COOKIE['GO_PW']) || $_COOKIE['GO_UN'] =='' || $_COOKIE['GO_PW'] == '' || !$GO_AUTH->login($_COOKIE['GO_UN'], $_COOKIE['GO_PW'], $auth_sources[$GO_AUTH_SOURCE_KEY]))
      {
	//header('Location: '.$GO_CONFIG->host.'index.php?return_to='.urlencode($_SERVER['REQUEST_URI']));
?>
<html><head></head><body>
<script language="javascript">
window.top.location="<?php echo $GO_CONFIG->host.'index.php'.(isset($link_back) && $link_back == '' ? '' : '?return_to='.rawurlencode(isset($link_back) && $link_back != '' ? $link_back : $_SERVER['REQUEST_URI']));?>";
</script>
</body></html>
<?php
	exit();
      }
    }

    if ($admin && !$this->has_admin_permission($this->user_id))
    {
      header("Location: ".$GO_CONFIG->host."error_docs/403.php");
      exit();
    }
  }

  /**
	* Creates and returns a new Access Control List to secure an object
	*
	* @param	string	$description	Description of the ACL
	* @param	int			$user_id	The owner of the ACL and the one who can modify it
	*									default is the current logged in user.
	* @access public
	* @return int			The ID of the new Access Control List
	*/
  function get_new_acl($description, $user_id=-1)
  {
    if ($user_id == -1)
    {
      $user_id = $this->user_id;
    }
    $id = $this->nextid("acl_items");
    if ($id > 0)
    {
      $this->query("INSERT INTO acl_items (id, description, user_id) ".
										"VALUES ('$id', '$description', '$user_id')");
      return $id;
    }else
    {
      return false;
    }
  }
  
  /**
	* Checks if a user owns the Access Control List
	*
	* @param	int			$user_id	The owner of the ACL and the one who can modify it
	* @param	int			$acl_id	The ID of the Access Control List
	* @access public
	* @return bool
	*/
  function user_owns_acl($user_id, $acl_id)
  {
    $this->query("SELECT user_id FROM acl_items WHERE id='$acl_id'");
    if ($this->next_record())
    {
      if ($user_id == $this->f('user_id'))
      {
	return true;
      }elseif($this->f('user_id') == '0')
      {
	return $this->has_admin_permission($user_id);
      }
    }
    return false;
  }

  /**
	* Deletes an Access Control List
	*
	* @param	int			$acl_id	The ID of the Access Control List
	* @access public
	* @return bool		True on succces
	*/
  function delete_acl($acl_id)
  {
    if($this->query("DELETE FROM acl WHERE acl_id='$acl_id'"))
    {
    	return $this->query("DELETE FROM acl_items WHERE id='$acl_id'");
    }
    return false;
  }

  /**
	* Adds a user to an Access Control List
	*
	* @param	int			$user_id	The user_id to add to the ACL
	* @param	int			$acl_id		The ID of the Access Control List
	* @access public
	* @return bool		True on success
	*/
  function add_user_to_acl($user_id,$acl_id)
  {
    return $this->query("INSERT INTO acl (acl_id,user_id) ".
												"VALUES ('$acl_id','$user_id')");
  }
  
  /**
	* Deletes a user from an Access Control List
	*
	* @param	int			$user_id	The user_id to delete from the ACL
	* @param	int			$acl_id		The ID of the Access Control List
	* @access public
	* @return bool		True on success
	*/
  function delete_user_from_acl($user_id, $acl_id)
  {
    $sql = "DELETE FROM acl WHERE user_id='$user_id' AND acl_id='$acl_id'";
    return $this->query($sql);
  }

  /**
	* Add's a user group to an Access Control List
	*
	* @param	int			$group_id	The group_id to add to the ACL
	* @param	int			$acl_id		The ID of the Access Control List
	* @access public
	* @return bool		True on success
	*/
  function add_group_to_acl($group_id,$acl_id)
  {
    return $this->query("INSERT INTO acl (acl_id,group_id) ".
												"VALUES ('$acl_id','$group_id')");
  }
  
  /**
	* Deletes a user group from an Access Control List
	*
	* @param	int			$group_id	The group_id to add to the ACL
	* @param	int			$acl_id		The ID of the Access Control List
	* @access public
	* @return bool		True on success
	*/
  function delete_group_from_acl($group_id, $acl_id)
  {
    $sql = "DELETE FROM acl WHERE group_id='$group_id' AND acl_id='$acl_id'";
    return $this->query($sql);
  }

	/**
	* Remove all users and user groups from an ACL
	*
	* @param	int			$acl_id		The ID of the Access Control List
	* @access public
	* @return bool		True on success
	*/
  function clear_acl($acl_id)
  {
    return $this->query("DELETE FROM acl WHERE acl_id='$acl_id'");
  }
  
	/**
	* Set's the owner of an access control list
	*
	* @param	int			$acl_id		The ID of the Access Control List
	* @param	int			$user_id	The user ID of the new owner
	* @access public
	* @return bool		True on success
	*/
  function set_acl_owner($acl_id, $user_id)
  {
    return $this->query("UPDATE acl_items SET user_id='$user_id' WHERE id='$acl_id'");
  }

	/**
	* Checks if a user is in the special admins group
	*
	* @param	int			$user_id	The user ID
	* @access public
	* @return bool		True on success
	*/
  function has_admin_permission($user_id)
  {
    global $GO_CONFIG, $GO_GROUPS;
    return $GO_GROUPS->is_in_group($user_id, $GO_CONFIG->group_root);
  }

	/**
	* Get's all groups from an ACL
	*
	* @param	int			$acl_id	The ACL ID
	* @access public
	* @return int			Number of groups in the acl
	*/
  function get_groups_in_acl($acl_id)
  {
    global $GO_CONFIG, $auth_sources;

    $sql = "SELECT groups.* FROM groups INNER JOIN acl ON".
      " acl.group_id=groups.id WHERE acl.acl_id='$acl_id'".
      " ORDER BY groups.name";
    $this->query($sql);
    return $this->num_rows();
  }

	/**
	* Get's all users from an ACL
	*
	* @param	int			$acl_id	The ACL ID
	* @access public
	* @return int			Number of users in the acl
	*/
  function get_users_in_acl($acl_id)
  {
    $sql = "SELECT user_id FROM acl WHERE acl_id='$acl_id'";
    $this->query($sql);
    return $this->num_rows();    
  }

	/**
	* Checks presence of a user in an ACL
	*
	* @param	int			$user_id	The user ID
	* @param	int			$acl_id	The ACL ID
	* @access public
	* @return int			True if the user is in the ACL
	*/
  function user_in_acl($user_id, $acl_id)
  {
    $sql = "SELECT user_id FROM acl WHERE acl_id='$acl_id' AND".
      " user_id='$user_id'";
    $this->query($sql);
    if ($this->num_rows() > 0)
    {
      return true;
    }else
    {
      return false;
    }
  }

	/**
	* Checks presence of a group in an ACL
	*
	* @param	int			$group_id	The group ID
	* @param	int			$acl_id	The ACL ID
	* @access public
	* @return int			True if the group is in the ACL
	*/
  function group_in_acl($group_id, $acl_id)
  {
    $sql = "SELECT group_id FROM acl WHERE acl_id='$acl_id' AND group_id='$group_id'";
    $this->query($sql);
    if ($this->num_rows() > 0)
    {
      return true;
    }else
    {
      return false;
    }
  }

	/**
	* Get's an ACL id based on the desciption. Use carefully.
	*
	* @param	string			$description	The description of an ACL
	* @access public
	* @return int			True if the group is in the ACL
	*/
  function get_acl_id($description)
  {
    $sql = "SELECT id FROM acl_items WHERE description='$description'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->f('id');
    }
    return false;
  }

	/**
	* Copy the user and group permissions of one acl to another
	*
	* @param	int			$sAcl	The source ACL to copy
	* @param	int			$dAcl	The destination ACL to copy to.
	* @access public
	* @return void
	*/
  function copy_acl($sAcl, $dAcl)
  {
    global $GO_CONFIG, $GO_GROUPS;

    $this->clear_acl($dAcl);

    $sql = "SELECT * FROM acl WHERE acl_id='$sAcl'";

    $security = new GO_SECURITY();
    $this->query($sql);
    while($this->next_record())
    {
      $new_security = new GO_SECURITY();
      if ($this->f("group_id") != 0 && $GO_GROUPS->group_is_visible($this->user_id, $this->f("group_id")))
      {
	$new_security->add_group_to_acl($this->f("group_id"), $dAcl);
      }

      if ($this->f("user_id") != 0 && ($security->user_is_visible($this->f("user_id")) || $this->f("user_id") == $this->user_id))
      {
	$new_security->add_user_to_acl($this->f("user_id"), $dAcl);
      }
    }
  }
  
  /**
	* Checks if a user is visible to the current logged in user
	*
	* @param	int			$user_id	The user ID to check
	* @access public
	* @return int			True if the user is visible
	*/
	
  function user_is_visible($user_id)
  {
    if ($this->user_id == $user_id)
      return true;

    $sql = "SELECT acl_id FROM users WHERE id='$user_id'";
    $this->query($sql);
    $this->next_record();
    return $this->has_permission($this->user_id, $this->f("acl_id"));
  }


  /**
	* Called when a user is deleted
	*
	* @param	int			$user_id	The user ID that is about to be deleted
	* @access private
	* @return bool		True on success
	*/
	
  function delete_user($user_id)
  {
    $sql = "DELETE FROM acl WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
	* Called when a group is deleted
	*
	* @param	int			$group_id	The group ID that is about to be deleted
	* @access private
	* @return bool	True on success
	*/
  function delete_group($group_id)
  {
    $sql = "DELETE FROM acl WHERE group_id='$group_id'";
    return $this->query($sql);
  }
}
?>
