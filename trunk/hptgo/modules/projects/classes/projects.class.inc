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
define('STATUS_RUNNING', -2);
define('STATUS_DROP', -1);
define('STATUS_DONE', 0);
define('STATUS_BEGIN', 1);
define('STATUS_OFFER', 1);
define('STATUS_SIGNED', 2);
define('STATUS_IMPLEMENT', 3);

/**
* Class for Projects module
* 
*  @author: Merijn Schering <mschering@intermesh.nl>
*  @version: 1.0 Release date: 08 July 2003
*/
class projects extends db
{

/**
* Constructor
*/
  function projects()
  {
    $this->db();
  }

/**
* Get setting for user
*
* Get setting from database, if not, insert a default setting then get again.
* @param	int		$user_id	User ID
* @return	Record	a record in pm_settings table
*/
  function get_settings($user_id)
  {
    $this->query("SELECT * FROM pm_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO pm_settings ".
          "(user_id, sort_field, sort_order, show_projects, show_catalog) VALUES ".
          "('$user_id', 'name', 'ASC', '1', '0')");
      return $this->get_settings($user_id);
    }                
  }

/**
* Set order and field sorting
*
* @param	int		$user_id	User ID
* @param	string	$sort_field	Field name to sort
* @param	string	$sort_order	Order string ("ASC","DESC")
* @return	int 	= 0 if fail
*/
  function set_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE pm_settings SET sort_field='$sort_field' , ".
      "sort_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

/**
* Save show projects for current user
*
* @param	int		$user_id		User ID
* @param	int		$show_projects	Show projects value
* @return	int 	= 0 if fail
*/
  function set_show_projects($user_id, $show_projects)
  {
    $sql = "UPDATE pm_settings SET show_projects='$show_projects'  WHERE user_id='$user_id'";
    return $this->query($sql);
  }

/**
* Save show catalog of current user
*
* @param	int		$user_id		User ID
* @param	string	$sort_catalog	Show catalog value
* @return	int 	= 0 if fail
*/
  function set_show_catalog($user_id, $show_catalog)
  {
    $sql = "UPDATE pm_settings SET show_catalog='$show_catalog'  WHERE user_id='$user_id'";
    return $this->query($sql);
  }

/**
* Set start timer (don't use now)
*
* @param	int		$user_id	User ID
* @param	string	$project_id	Project ID
* @return	int 	= 0 if fail
*/
  function start_timer($user_id, $project_id)
  {
    $sql = "INSERT INTO pmTimers (user_id, start_time, project_id) ".
      "VALUES ('$user_id', '".get_gmt_time()."', '$project_id')";
    return $this->query($sql);
  }

/**
* Remove timer (stop)
*
* @param	int		$user_id	User ID
* @return	int 	= 0 if fail
*/
  function stop_timer($user_id)
  {
    $sql = "DELETE FROM pmTimers WHERE user_id='$user_id'";
    return $this->query($sql);
  }

/**
* Get timer
*
* @param	int		$user_id	User ID
* @return	mixed 	if success return a record in pmTimer table, if fail return false
*/
  function get_timer($user_id)
  {
    $sql = "SELECT start_time, project_id FROM pmTimers WHERE user_id='$user_id'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

/**
* Add a new project
*
* @param	int		$user_id				User ID, who creates this project
* @param	string	$name					Project name
* @param	string	$Description			Description
* @param	int		$contact_id				Client ID
* @param	string	$comments				Comments
* @param	string	$start_date				Start date of project
* @param	string	$end_date				End date of project
* @param	string	$responsible_user_id	Project manager ID
* @param	string	$probability		Project name
* @return	int 	= 0 if fail
*/
  function add_project($user_id, $name, $description, $contact_id, $comments,
      $start_date, $end_date, $status, $responsible_user_id,
      $probability, $fee_id, $budget, $acl_read, $acl_write, $task_template_id, $cat_id)
  {
    $project_id = $this->nextid("pmProjects");
    if ($project_id > 0)
    {
      $sql = "INSERT INTO pmProjects (id, user_id, name, description, ".
        "contact_id, acl_read, acl_write, comments, ctime, mtime,".
        " start_date, end_date, status, res_user_id, probability, ".
        "fee_id, budget, task_template_id, cat_id) ".
        "VALUES ('$project_id', '$user_id', '$name', '$description',".
        " '$contact_id', '$acl_read', '$acl_write', '$comments', ".
        "'".get_gmt_time()."', '".get_gmt_time()."', '$start_date', ".
        "'$end_date', '$status', '$responsible_user_id', ".
        "'$probability', '$fee_id', '$budget', '$task_template_id', '$cat_id')";

      $this->query($sql);
      return $project_id;
    }
    return false;
  }

  function update_project($project_id, $name, $description, $contact_id, $comments, $start_date, $end_date, $responsible_user_id, $probability, $fee_id, $budget)
  {
    $sql = "UPDATE pmProjects SET name='$name', description='$description', comments='$comments', mtime='".get_gmt_time()."', contact_id='$contact_id', start_date='$start_date', end_date='$end_date', res_user_id='$responsible_user_id', probability='$probability', fee_id='$fee_id', budget='$budget' WHERE id='$project_id'";
    return $this->query($sql);
  }

  function get_project($project_id)
  {
    $sql = "SELECT * FROM pmProjects WHERE id='$project_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function get_project_by_name($name)
  {
    $sql = "SELECT * FROM pmProjects WHERE name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function delete_project($project_id)
  {
    $sql = "DELETE FROM pmHours WHERE project_id='$project_id'";
    if ($this->query($sql))
    {
      $sql = "DELETE FROM pmProjects WHERE id='$project_id'";
      return $this->query($sql);
    }else
    {
      return false;
    }
  }

  function get_projects($sort='name', $direction='ASC')
  {
    $sql = "SELECT * FROM pmProjects ORDER BY $sort $direction";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_authorized_projects($user_id, $sort='name', $direction='ASC', $start=0, $offset=0, $filter='', $catalog, $contact_id=0)
  {
    if ($filter < STATUS_RUNNING)
      $filter = '';
    elseif ($filter == STATUS_RUNNING)
      $filter = ' AND status >= '.STATUS_BEGIN;
    else
      $filter = ' AND status = '.$filter;

    $sql = "SELECT DISTINCT pmProjects.* FROM pmProjects ".
      "INNER JOIN acl ON (pmProjects.acl_read = acl.acl_id OR pmProjects.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) WHERE ((".
      "users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id."))".$filter;

    if ($catalog > 0)
      $sql .= " AND cat_id=$catalog";

    if ($contact_id > 0)
      $sql .= " AND contact_id='$contact_id' ";

    $sql .= " GROUP BY pmProjects.id ORDER BY $sort $direction";

    if ($offset > 0)
    {
      $sql .= " LIMIT $start, $offset";

      $sql2 = "SELECT DISTINCT pmProjects.id FROM pmProjects ".
        "INNER JOIN acl ON (pmProjects.acl_read = acl.acl_id OR pmProjects.acl_write = acl.acl_id) ".
        "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) WHERE ((".
        "users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
        "acl.group_id = 0 AND acl.user_id = ".$user_id."))";

      if ($filter != '') $sql2 .= $filter;
      if ($contact_id > 0) $sql2 .= " AND contact_id='$contact_id'";

      $sql2 .= " GROUP BY pmProjects.id";
      $this->query($sql2);

      $count = $this->num_rows();
      if ($count > 0) $this->query($sql);
      return $count;

    } else {
      $this->query($sql);
      return $this->num_rows();
    }
  }

  function get_writable_projects($user_id)
  {
    $sql = "SELECT DISTINCT pmProjects.* FROM pmProjects, acl, users_groups WHERE (".
      "pmProjects.acl_write = acl.acl_id".
      ") AND ( ( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id." ))";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_user_projects($user_id)
  {
    $this->query("SELECT * FROM pmProjects WHERE user_id='$user_id'");
    return $this->num_rows();
  }

  function get_client_projects($contact_id)
  {
    $this->query("SELECT * FROM pmProjects WHERE contact_id='$contact_id'");
    return $this->num_rows();
  }

  function add_hours($project_id, $user_id, $start_time, $end_time, $break_time, $unit_value, $comments)
  {
    $id = $this->nextid("pmHours");
    if ($id > 0)
    {
      $sql  = "INSERT INTO pmHours (id, user_id, project_id, start_time, end_time, break_time, unit_value, comments) ";
      $sql .= "VALUES ('$id', '$user_id', '$project_id', '$start_time', '$end_time', '$break_time', '$unit_value', '$comments')";
      return $this->query($sql);
    }
    return false;
  }

  function get_working_hours($working_hours_id)
  {
    $sql = "SELECT * FROM pmHours WHERE id='$working_hours_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function check_hours($user_id, $start_time, $end_time)
  {
    $sql = "SELECT * FROM pmHours WHERE user_id='$user_id' AND start_time <= '$start_time' AND end_time > '$start_time'";
    $this->query($sql);
    if ($this->next_record())
    {
      return false;
    }

    $sql = "SELECT * FROM pmHours WHERE user_id='$user_id' AND start_time <= '$end_time' AND end_time > '$end_time'";
    $this->query($sql);
    if($this->next_record())
    {
      return false;
    }
    return true;
  }

  function get_hours($start_time=0, $end_time=0, $project_id=0, $user_id=0, $contact_id=0)
  {
    if ($start_time == 0 && $end_time == 0)
    {
      $sql = "SELECT pmHours.*, pmFees.value AS fee_value, pmFees.time AS fee_time, pmProjects.contact_id, pmProjects.name, pmProjects.description FROM pmHours INNER JOIN pmProjects ON (pmProjects.id=pmHours.project_id) LEFT JOIN pmFees ON (pmFees.id=pmProjects.fee_id)";
      $where = false;
    }else
    {
      $sql = "SELECT pmHours.*, pmFees.value AS fee_value, pmFees.time AS fee_time, pmProjects.contact_id, pmProjects.name, pmProjects.description FROM pmHours INNER JOIN pmProjects ON (pmProjects.id=pmHours.project_id) LEFT JOIN pmFees ON (pmFees.id=pmProjects.fee_id) WHERE pmHours.start_time >= '$start_time' AND pmHours.start_time < '$end_time'";
      $where = true;
    }

    if ($project_id > 0)
    {
      if ($where)
      {
        $sql .= " AND";
      }else
      {
        $sql .= " WHERE";
        $where = true;
      }
      $sql .= " pmHours.project_id='$project_id'";
    }

    if ($user_id > 0)
    {
      if ($where)
      {
        $sql .= " AND";
      }else
      {
        $sql .= " WHERE";
        $where = true;
      }
      $sql .= " pmHours.user_id='$user_id'";
    }

    if ($contact_id > 0)
    {
      if ($where)
      {
        $sql .= " AND";
      }else
      {
        $sql .= " WHERE";
        $where = true;
      }
      $sql .= " pmProjects.contact_id='$contact_id'";
    }


    $sql .= " ORDER BY pmHours.start_time ASC";

    $this->query($sql);
    return $this->num_rows();
  }

  function delete_hours($hours_id)
  {
    if ($hours_id > 0)
    {
      $sql = "DELETE FROM pmHours WHERE id='$hours_id'";
      return $this->query($sql);
    }else
    {
      return false;
    }
  }

  function get_project_hours($project_id, $start_time=0, $end_time=0)
  {
    if ($start_time == 0 && $end_time == 0)
    {
      $sql = "SELECT pmHours.*, pmFees.value AS fee_value, pmFees.time AS fee_time FROM pmHours INNER JOIN pmProjects ON (pmProjects.id=pmHours.project_id) LEFT JOIN pmFees ON (pmFees.id=pmProjects.fee_id) WHERE pmHours.project_id='$project_id' ORDER BY pmHours.start_time ASC";
    }else
    {
      $sql = "SELECT pmHours.*, pmFees.value AS fee_value, pmFees.time AS fee_time FROM pmHours INNER JOIN pmProjects ON (pmProjects.id=pmHours.project_id) LEFT JOIN pmFees ON (pmFees.id=pmProjects.fee_id)  WHERE pmHours.project_id='$project_id' AND pmHours.start_time >= '$start_time' AND pmHours.start_time < '$end_time' ORDER BY pmHours.start_time ASC";
    }
    $this->query($sql);
    return $this->num_rows();
  }

  function get_user_hours($user_id, $start_time=0, $end_time=0)
  {
    if ($start_time == 0 && $end_time == 0)
    {
      $sql = "SELECT pmHours.*, pmFees.value AS fee_value, pmFees.time AS fee_time, pmProjects.name AS project_name FROM pmHours LEFT JOIN pmProjects ON (pmHours.project_id = pmProjects.id) LEFT JOIN pmFees ON (pmFees.id=pmHours.fee_id)  WHERE pmHours.user_id='$user_id' ORDER BY start_time ASC";
    }else
    {
      $sql = "SELECT pmHours.*, pmFees.value AS fee_value, pmFees.time AS fee_time, pmProjects.name AS project_name FROM pmHours LEFT JOIN pmProjects ON (pmHours.project_id = pmProjects.id) LEFT JOIN pmFees ON (pmFees.id=pmHours.fee_id)  WHERE pmHours.user_id='$user_id' AND pmHours.start_time >= '$start_time' AND pmHours.start_time < '$end_time' ORDER BY pmHours.start_time ASC";
    }
    $this->query($sql);
    return $this->num_rows();
  }

  function get_fees()
  {
    $sql = "SELECT * FROM pmFees";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_fee($fee_id)
  {
    $sql = "SELECT * FROM pmFees WHERE id='$fee_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function add_fee($name, $value, $time)
  {
    $id = $this->nextid("pmFees");
    if ($id > 0)
    {
      $sql = "INSERT INTO pmFees (id, name, value, time) VALUES ('$id', '$name', '$value', '$time')";
      return $this->query($sql);
    }
    return false;
  }

  function delete_fee($fee_id)
  {
    $sql = "DELETE FROM pmFees WHERE id='$fee_id'";
    return $this->query($sql);
  }

  function update_fee($fee_id, $name, $value, $time)
  {
    $sql = "UPDATE pmFees SET name='$name', value='$value', time='$time' WHERE id='$fee_id'";
    return $this->query($sql);
  }

  function delete_user($user_id)
  {
    $projects = new projects();
    $this->get_user_projects($user_id);
    while($this->next_record())
    {
      $projects->delete_project($user_id);
    }
  }
}
?>
