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

define('REMIND_BEFORE_STARTTIME', '0');
define('REMIND_BEFORE_DUETIME', '1');

define('SHOW_ALL', '0');
define('SHOW_OWN', '1');
define('SHOW_RESPONSIBLE', '2');

class todos extends db
{
	/**
	* Constructor
	*/
	function todos()
	{
		$this->db();
	}

	/**
	* Add a new todo
	* @param	int		$user_id
	* @param	int		$contact_id
	* @param	int		$res_user_id
	* @param	string	$start_time
	* @param	string	$due_time
	* @param	int		$status
	* @param	int		$priority
	* @param	string	$completion_time
	* @param	string	$remind_time
	* @param	bool	$remind_style
	* @param	string	$name
	* @param	string	$description
	* @param	string	$location
	* @param	string	$background
	* @return	bool
	*/
	function add_todo($user_id, $contact_id, $res_user_id, $start_time,
										$due_time, $status, $priority, $completion_time,
										$remind_time, $remind_style, $name, $description,
										$location, $background)
	{
		$todo_id = $this->nextid('cal_todos');
		if ($todo_id > 0)
		{
			$sql = 	"INSERT INTO cal_todos (id, user_id, contact_id, res_user_id, ".
							"start_time, due_time, status, priority, completion_time, ".
							"remind_time, remind_style, name, description, location, ".
							"background, ctime, mtime) VALUES ('$todo_id', ".
							"'$user_id', '$contact_id', '$res_user_id', '$start_time',".
							"'$due_time', '$status', '$priority', '$completion_time', ".
							"'$remind_time', '$remind_style', '$name', '$description', ".
							"'$location', '$background','".get_gmt_time()."', '".
							get_gmt_time()."')";

			return $this->query($sql);
		}
		return false;
	}

	/**
	* Update a todo
	* @param	int		$todo_id
	* @param	int		$contact_id
	* @param	int		$res_user_id
	* @param	string	$start_time
	* @param	string	$due_time
	* @param	int		$status
	* @param	int		$priority
	* @param	string	$completion_time
	* @param	string	$remind_time
	* @param	bool	$remind_style
	* @param	string	$name
	* @param	string	$description
	* @param	string	$location
	* @param	string	$background
	* @return	bool
	*/
	function update_todo($todo_id, $contact_id, $res_user_id, $start_time,
											$due_time, $status, $priority, $completion_time,
											$remind_time, $remind_style, $name, $description,
											$location, $background)
	{
		$sql = 	"UPDATE cal_todos SET res_user_id='$res_user_id', ".
						"contact_id='$contact_id', start_time='$start_time', ".
						"due_time='$due_time', status='$status', priority='$priority',".
						"completion_time='$completion_time', name='$name', ".
						"remind_time='$remind_time', remind_style='$remind_style', ".
						"description='$description', location='$location',  ".
						"background='$background',mtime='".get_gmt_time()."' ".
						"WHERE id='$todo_id'";

		return $this->query($sql);
	}

	/**
	* Set completed
	* @param	int		$todo_id
	* @return	bool
	*/
	function set_completed($todo_id)
	{
		$sql = 	"UPDATE cal_todos SET completion_time='".get_gmt_time()."' ".
						"WHERE id='$todo_id'";
		return $this->query($sql);
	}

	/**
	* Set uncomplete
	* @param	int		$todo_id
	* @return	bool
	*/
	function set_uncompleted($todo_id)
	{
		$sql = 	"UPDATE cal_todos SET completion_time='0' ".
						"WHERE id='$todo_id'";
		return $this->query($sql);
	}

	/**
	* Delete a todo
	* @param	int		$todo_id
	* @return	bool
	*/
	function delete_todo($todo_id)
	{
		$sql = "DELETE FROM cal_todos WHERE id='$todo_id'";
		return $this->query($sql);
	}

	/**
	* Get a todo
	* @param	int		$todo_id
	* @return	mixed	array if success, false otherwise
	*/
	function get_todo($todo_id)
	{
		$sql = "SELECT * FROM cal_todos WHERE id='$todo_id'";
		$this->query($sql);
		if ($this->next_record())
		{
			return $this->Record;
		}
		return false;
	}

	/**
	* Get todos
	* @param	bool	$complete_only
	* @param	int		$user_id
	* @param	int		$res_user_id
	* @param	int		$contact_id
	* @param	int		$start
	* @param	int		$offset
	* @return	int		Number of records
	*/
	function get_todos($completed_only=true, $user_id=0, $res_user_id=0,
											$contact_id=0, $start=0, $offset=0)
	{
		$sql = "SELECT * FROM cal_todos";

		if ($user_id > 0)
		{
			$where = true;
			$sql .= " WHERE user_id='$user_id'";
		}

		if ($res_user_id > 0)
		{
			if (!isset($where))
			{
				$sql .= " WHERE ";
				$where = true;
			}else
			{
				$sql .= " AND ";
			}
			$sql .= "res_user_id='$res_user_id'";
		}

		if ($contact_id > 0)
		{
			if (!isset($where))
			{
				$sql .= " WHERE ";
				$where = true;
			}else
			{
				$sql .= " AND ";
			}
			$sql .= "contact_id='$contact_id'";
		}

		if ($completed_only)
		{
			if (!isset($where))
			{
				$sql .= " WHERE ";
				$where = true;
			}else
			{
				$sql .= " AND ";
			}
			$sql .= "completion_time='0'";
		}

		$sql .= " ORDER BY due_time DESC";

		if ($offset > 0)
		{
			$this->query($sql);
			$count = $this->num_rows();

			if ($count < $offset)
			{
				return $count;
			}else
			{
				$sql2 = $sql." LIMIT $start, $offset";
				$this->query($sql2);
				return $count;
			}
		}else
		{
			$this->query($sql);
			return $this->num_rows();
		}
	}
	
	/**
	* Get todos for today
	* @return	int		Number of records
	*/
	function get_todos_for_today()
	{
		$local_time = get_time();
		$year = date("Y", $local_time);
		$month = date("m", $local_time);
		$day = date("j", $local_time);
		
		$day_start = mktime(0,0,0,$month, $day, $year)-($_SESSION['GO_SESSION']['timezone']*3600);
		$day_end = mktime(0,0,0,$month, $day+1, $year)-($_SESSION['GO_SESSION']['timezone']*3600);
		
		$sql = 	"SELECT * FROM cal_todos WHERE res_user_id='".$_SESSION['GO_SESSION']['user_id']."' ".
						"AND completion_time=0 AND start_time<=$day_end";
		$this->query($sql);
		return $this->num_rows();		
	}

	/**
	* Get todos to remind
	* @param	int		$user_id
	* @return	int		Number of records
	*/
	function get_todos_to_remind($user_id)
	{
		$sql = 	"SELECT * FROM cal_todos WHERE ".
						"res_user_id='".$user_id."' AND ".
						"remind_time<".get_gmt_time()." AND ".
						"remind_time>0";
		$this->query($sql);
		return $this->num_rows();
	}

	/**
	* Set reminder
	* @param	int		$todo_id
	* @return	bool
	*/
	function set_reminded($todo_id)
	{
		$sql = "UPDATE cal_todos SET remind_time=0 WHERE id='$todo_id'";
		return $this->query($sql);
	}
}
?>