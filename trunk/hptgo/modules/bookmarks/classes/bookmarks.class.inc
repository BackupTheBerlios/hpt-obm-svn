<?php
/*
Copyright Intermesh 2004
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 08 July 2003
Version: 2.0 Release date: 12 March 2004

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

class bookmarks extends db
{
	/**
	* Constructor
	*/
	function bookmarks()
	{
		$this->db();
	}

	/**
	* Add a new bookmark
	* @param	int		$user_id
	* @param	int		$category_id
	* @param	string	$URL
	* @param	string	$name
	* @param	bool	$new_window
	* @param	int		$acl_read
	* @param	int		$acl_write
	* @return	mixed	Bookmark ID if success, false otherwise
	*/
	function add_bookmark($user_id, $catagory_id, $URL, $name, $new_window, $acl_read, $acl_write)
	{
		if ($new_window == true)
		{
			$new_window = 1;
		}else
		{
			$new_window = 0;
		}

		$bookmark_id = $this->nextid("bm_bookmarks");
		if ($bookmark_id > 0)
		{
			$sql = "INSERT INTO bm_bookmarks (id, user_id, catagory_id, URL, name, ".
							"new_window, acl_read, acl_write) VALUES ('$bookmark_id', '$user_id', ".
							"'$catagory_id', '$URL', ".
							"'$name', '$new_window', '$acl_read', '$acl_write')";
			if ($this->query($sql))
			{
				return $bookmark_id;
			}

		}
		return false;
	}

	/**
	* Update bookmark
	* @param	int		$bookmark_id
	* @param	int		$category_id
	* @param	string	$URL
	* @param	string	$name
	* @param	bool	$new_window
	* @return	bool	true id success, false otherwise
	*/
	function update_bookmark($bookmark_id, $catagory_id, $URL, $name, $new_window)
	{
		if ($new_window == true)
		{
			$new_window = 1;
		}else
		{
			$new_window = 0;
		}

		$sql = "UPDATE bm_bookmarks SET catagory_id='$catagory_id', ".
						"URL='$URL', ".
						"name='$name', ".
						"new_window='$new_window' WHERE id='$bookmark_id'";

		return ($this->query($sql));

	}

	/**
	* Move a bookmark
	* @param	int		$bookmark_id
	* @param	int		$category_id
	* @return	bool	true id success, false otherwise
	*/	
	function move_bookmark($bookmark_id, $catagory_id)
	{
		$sql = "UPDATE bm_bookmarks SET catagory_id='$catagory_id' ".
								"WHERE id='$bookmark_id'";
		return ($this->query($sql));
	}

	/**
	* Delete a bookmark
	* @param	int		$bookmark_id
	* @return	bool	true id success, false otherwise
	*/
	function delete_bookmark($bookmark_id)
	{
		$sql = "DELETE FROM bm_bookmarks WHERE id='$bookmark_id'";
		return $this->query($sql);
	}

	/**
	* Get bookmarks
	* @param	int		$user_id
	* @param	int		$category_id
	* @return	int		Number of rows
	*/
	function get_bookmarks($user_id, $catagory_id)
	{
		$sql = "SELECT DISTINCT bm_bookmarks.* FROM bm_bookmarks ".
						"INNER JOIN acl ON (bm_bookmarks.acl_read = acl.acl_id OR ".
						"bm_bookmarks.acl_write = acl.acl_id) ".
						"LEFT JOIN users_groups ON (acl.group_id=users_groups.group_id) ".
						"WHERE ((users_groups.user_id='".$user_id."' AND acl.user_id='0') ".
						"OR (acl.group_id = '0' AND acl.user_id = '".$user_id."')) ".
						"AND catagory_id='$catagory_id' ORDER BY name ASC";

		$this->query($sql);
		return $this->num_rows();
	}

	/**
	* Get user bookmark
	* @param	int		$user_id
	* @param	int		$category_id
	* @return	array	
	*/
	function get_user_bookmarks($user_id, $catagory_id=-1)
	{
		$sql = "SELECT * FROM bm_bookmarks WHERE user_id='$user_id'";

		if ($catagory_id > -1)
		{
			$sql .= " AND catagory_id='$catagory_id'";
		}
		$this->query($sql);
		return $this->next_record();
	}

	/**
	* Get a bookmark by bookmark_id
	* @param	int		$bookmark_id
	* @return	mixed	array if success, false otherwise
	*/
	function get_bookmark($bookmark_id)
	{
		$sql = "SELECT * FROM bm_bookmarks WHERE id='$bookmark_id'";
		$this->query($sql);
		if($this->next_record())
		{
			return $this->Record;
		}
		return false;
	}

	/**
	* Add a new catagory
	* @param	int		$user_id
	* @param	string	$name
	* @param	int		$acl_read
	* @param	int		$acl_write
	* @return	mixed	catagory ID if success, false otherwise
	*/
	function add_catagory($user_id, $name, $acl_read, $acl_write)
	{
		$catagory_id = $this->nextid("bm_catagories");
		if ($catagory_id > 0)
		{
			if($this->query("INSERT INTO bm_catagories ".
							"(id, user_id, name, acl_read, acl_write) VALUES ".
							"('$catagory_id', '$user_id', '$name', '$acl_read', ".
							"'$acl_write')"))
			{
				return $catagory_id;
			}
		}
		return false;
	}

	/**
	* Update a catagory
	* @param	int		$catagory_id
	* @param	string	$name
	* @return	bool	
	*/
	function update_catagory($catagory_id, $name)
	{
		return $this->query("UPDATE bm_catagories SET ".
						"name='$name' WHERE ".
						"id='$catagory_id'");
	}

	/**
	* Delete a catagory
	* @param	int		catagory_id
	* @return	bool	
	*/
	function delete_catagory($catagory_id)
	{
		global $GO_SECURITY;

		$this->query("SELECT * FROM bm_bookmarks WHERE catagory_id='$catagory_id'");

		$bookmarks = new bookmarks();

		while($this->next_record())
		{
			if ($bookmarks->delete_bookmark($this->f('id')))
			{
				$GO_SECURITY->delete_acl($this->f('acl_read'));
				$GO_SECURITY->delete_acl($this->f('acl_write'));
			}
		}

		return $this->query("DELETE FROM bm_catagories WHERE ".
												"id='$catagory_id'");
	}

	/**
	* Get catagories
	* @param	int		$user_id
	* @param	bool	$only_writable
	* @return	int		Number of rows
	*/
	function get_catagories($user_id, $only_writable=false)
	{
		$sql = "SELECT DISTINCT bm_catagories.* FROM bm_catagories ".
						"INNER JOIN acl ON (bm_catagories.acl_write=acl.acl_id ";
		if (!$only_writable)
		{
			$sql .= "OR bm_catagories.acl_read=acl.acl_id";
		}

		$sql .= ") LEFT JOIN users_groups ON (acl.group_id=users_groups.group_id) ".
						"WHERE ((users_groups.user_id='".$user_id."' AND acl.user_id='0') ".
						"OR (acl.group_id = '0' AND acl.user_id = '".$user_id."')) ".
						"ORDER BY name ASC";
		$this->query($sql);
		return $this->num_rows();
	}

	/**
	* Get a catagory
	* @param	int		$catagory_id
	* @return	mixed	array if success, false otherwise
	*/
	function get_catagory($catagory_id)
	{
		$sql = "SELECT * FROM bm_catagories WHERE id='$catagory_id'";
		$this->query($sql);
		if($this->next_record())
		{
			return $this->Record;
		}
		return false;
	}

	/**
	* Get a catagory by name
	* @param	string	$name
	* @return	mixed	array if success, false otherwise
	*/
	function get_catagory_by_name($name)
	{
		$sql = "SELECT * FROM bm_catagories WHERE name='$name'";
		$this->query($sql);
		if($this->next_record())
		{
			return $this->Record;
		}
		return false;
	}

	/**
	* Delete a user
	* @param	int		$user_id
	*/
	function delete_user($user_id)
	{
		$this->get_user_bookmarks($user_id);
		$del = new bookmarks();
		while ($this->next_record())
		{
			$del->delete_bookmark($user_id);
		}
	}
}
