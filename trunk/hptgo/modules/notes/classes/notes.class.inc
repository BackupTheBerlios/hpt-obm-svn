<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 2 Januari 2004

   Part of the Group-Office Professional license
 */

define('SHOW_ALL_NOTES', 1);
define('SHOW_OWN_NOTES', 2);
define('SHOW_RESPONSIBLE_NOTES', 3);

class notes extends db
{
  function notes()
  {
    $this->db();
  }

  function get_settings($user_id)
  {
    $this->query("SELECT * FROM no_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO no_settings ".
	  "(user_id, sort_field, sort_order, show_notes) VALUES ".
	  "('$user_id', 'name', 'ASC','1')");
      return $this->get_settings($user_id);
    }		
  }

  function set_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE no_settings SET sort_field='$sort_field' , ".
      "sort_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function set_show_notes($user_id, $show_notes)
  {
    $sql = "UPDATE no_settings SET show_notes='$show_notes'  WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function add_note($user_id, $contact_id, $project_id, $file_path,
      $catagory_id, $res_user_id, $due_date, $name,
      $content, $acl_read, $acl_write)
  {
    $note_id = $this->nextid("no_notes");

    if ($note_id > 0)
    {
      $sql = "INSERT INTO no_notes (id, user_id, contact_id, project_id,".
	"file_path, catagory_id, res_user_id, due_date, name, content,".
	" ctime, mtime, acl_read, acl_write) ".
	"VALUES ('$note_id', '$user_id', '$contact_id', '$project_id', ".
	"'$file_path', '$catagory_id', '$res_user_id', '$due_date',".
	" '$name', '$content', '".get_gmt_time()."', '".get_gmt_time().
	"', '$acl_read', '$acl_write')";

      if($this->query($sql))
      {
	return $note_id;
      }
    }
    return false;
  }

  function get_note($note_id)
  {
    $sql = "SELECT * FROM no_notes WHERE id='$note_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function delete_note($note_id)
  {
    $sql = "DELETE FROM no_notes WHERE id='$note_id'";
    return $this->query($sql);
  }


  function update_note($note_id, $name, $catagory_id, $res_user_id,
      $due_date, $content)
  {
    $sql = "UPDATE no_notes SET res_user_id='$res_user_id', ".
      "due_date='$due_date', catagory_id='$catagory_id', ".
      "name='$name', content='$content', catagory_id='$catagory_id',".
      " mtime='".get_gmt_time()."' WHERE id='$note_id'";

    return $this->query($sql);
  }

  function get_note_by_name($name)
  {
    $sql = "SELECT * FROM no_notes WHERE name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function get_new_notes()
  {
    $sql = "SELECT DISTINCT no_notes.* FROM no_notes ".
      "INNER JOIN acl ON (no_notes.acl_read = acl.acl_id OR no_notes.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
      " WHERE ((users_groups.user_id = '".$_SESSION['GO_SESSION']['user_id']."' AND acl.user_id = '0' ) OR (".
      "acl.group_id = '0' AND acl.user_id = '".$_SESSION['GO_SESSION']['user_id']."')) ".
      "AND mtime>'".$_SESSION['GO_SESSION']['lastlogin']."'";

    $this->query($sql);
    return $this->num_rows();
  }

  function get_notes($user_id=0, $project_id=0, $contact_id=0, $file_path='',
      $show, $sort='name', $direction='ASC', $start=0,
      $offset=0)
  {
    $sql = "SELECT DISTINCT no_notes.*, no_catagories.name AS catagory_name FROM no_notes ".
      "INNER JOIN acl ON (no_notes.acl_read = acl.acl_id OR no_notes.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
      "LEFT JOIN no_catagories ON (no_notes.catagory_id=no_catagories.id)".
      " WHERE ((users_groups.user_id = '".$user_id."' AND acl.user_id = '0' ) OR (".
      "acl.group_id = '0' AND acl.user_id = '".$user_id."'))";


    if ($contact_id != 0)
    {
      $sql .= " AND no_notes.contact_id='$contact_id'";
    }

    if ($project_id != 0)
    {
      $sql .= " AND no_notes.project_id='$project_id'";
    }

    if ($file_path != '')
    {
      $sql .= " AND no_notes.file_path='$file_path'";
    }

    switch ($show)
    {
      case SHOW_OWN_NOTES:
	$sql .= " AND no_notes.user_id='$user_id'";
	break;

      case SHOW_RESPONSIBLE_NOTES:

	$sql .= " AND no_notes.res_user_id='$user_id'";
	break;
    }

    $sql .= " ORDER BY $sort $direction";

    if ($offset > 0)
    {
      $this->query($sql);

      $count = $this->num_rows();
      if ($count > 0)
      {
	$this->query($sql." LIMIT $start, $offset");
      }
      return $count;

    }else
    {
      $this->query($sql);
      return $this->num_rows();
    }
  }

  function add_catagory($name)
  {
    $catagory_id = $this->nextid("no_catagories");

    if ($catagory_id > 0)
    {
      $sql = "INSERT INTO no_catagories (id, name) ".
	"VALUES ('$catagory_id', '$name')";
      return $this->query($sql);
    }
    return false;
  }

  function update_catagory($catagory_id, $name)
  {
    $sql = "UPDATE no_catagories SET  name='$name' WHERE id='$catagory_id'";
    return $this->query($sql);
  }

  function get_catagory_by_name($name)
  {
    $sql = "SELECT * FROM no_catagories WHERE name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function get_catagories()
  {
    $sql = "SELECT * FROM no_catagories ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function delete_catagory($catagory_id)
  {
    $sql = "DELETE FROM no_catagories WHERE id='$catagory_id'";
    return $this->query($sql);
  }
  function get_catagory($catagory_id)
  {
    $sql = "SELECT * FROM no_catagories WHERE id='$catagory_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  function delete_user($user_id)
  {
    $sql = "DELETE FROM no_notes WHERE user_id='$user_id'";
    $this->query($sql);
  }
}
?>
