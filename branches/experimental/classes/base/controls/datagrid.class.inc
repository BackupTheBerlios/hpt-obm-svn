<?php
/*
   Copyright Intermesh 2004
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

/**
 * Control to manupilate sql table directly from user.
 */
class datagrid extends db
{
  var $table_name = '';
  var $fields = array();
  var $id_field = 0;
  var $order_field = '';
  var $order_direction = '';
	
  var $start_row = 0;
  var $max_rows = 0;
	
  var $row_count = 0;

  var $delete_tables = array();
	
  function datagrid($table_name, $id_field_name='', $order_field='', $order_direction='ASC', $delete_tables='')
  {
    if($delete_tables != '')
    {
      $this->delete_tables = $delete_tables;
    }
    $this->db();
    $this->table_name = $table_name;
    $this->id_field_name = $id_field_name;
    $this->order_field = $order_field;
    $this->order_direction = $order_direction;

    $this->start_row = isset($_POST['start_row']) ? $_POST['start_row'] : '0';
    $this->max_rows = isset($_POST['max_rows']) ? $_POST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
		

    $this->query("SHOW FIELDS FROM `".$this->table_name."`");

    $i=0;
    while($this->next_record())
    {
      if($this->f(0) == $id_field_name)
      {
	$this->id_field = $i;
      }
    	
      $this->fields[$i] = $this->f(0);
      $i++;
    }

    if($_SERVER['REQUEST_METHOD'] == 'POST')
    {
      if($_POST['task'] == 'update')
      {
	$this->update();
      }
    	
      if($_POST['task'] == 'insert')
      {
	$this->insert();
      }
      if($_POST['delete_row_id'] > 0)
      {
	$this->delete(smart_addslashes($_POST['delete_row_id']));
      }
    }
  }

  function print_datagrid()
  {
    return $this->header().$this->data().$this->footer();
  }

  /**
   * Print the header
   */
  function header()
  {
    $headers = '<form method="post" action="'.$_SERVER['PHP_SELF'].'" name="'.$this->table_name.'">'.
      '<input type="hidden" name="task" value="" />'.
      '<input type="hidden" name="delete_row_id" value="0" />'.
      '<input type="hidden" name="start_row" value="'.$this->start_row.'" />'.
      '<input type="hidden" name="max_rows"  value="'.$this->max_rows.'" />'.
      '<table border="0" cellpadding="3" cellspacing="0"><tr>';

    for($i=0;$i<count($this->fields);$i++)
    {
      if($i!=$this->id_field)
      {
	$headers .= '<td class="TableHead2">'.$this->fields[$i].'</td>';
      }
    }
    $headers .= '<td class="TableHead2">&nbsp;</td></tr>';		
    return $headers;
  }

  /**
   * Print the footer.
   */
  function footer()
  {
    $footer = $this->pagination();

    $footer .= '</table><br />';		
    $footer .= '<input type="button" class="button" style="width: 100px;" '.
      ' value="Update" onclick="document.'.
      $this->table_name.'.task.value=\'update\';document.'.
      $this->table_name.'.submit();" '.
      'onmouseover="javascript:this.className=\'button_mo\';" '.
      'onmouseout="javascript:this.className=\'button\';" />'.
      '&nbsp;&nbsp;'.
								
      '<input type="button" class="button" style="width: 100px;" '.
      ' value="Insert" onclick="document.'.
      $this->table_name.'.task.value=\'insert\';document.'.
      $this->table_name.'.submit();" '.
      'onmouseover="javascript:this.className=\'button_mo\';" '.
      'onmouseout="javascript:this.className=\'button\';" />';						
								
    $footer .= '</form>';
		
    $footer .= '<script type="text/javascript">'.
      'function delete_row(id, text)'.
      '{if(confirm(text))'.
      '{document.'.$this->table_name.'.delete_row_id.value=id;'.
      'document.'.$this->table_name.'.submit();}}'.
								
      'function change_list(start_row, max_rows)'.
      '{document.'.$this->table_name.'.start_row.value=start_row;'.
      'document.'.$this->table_name.'.max_rows.value=max_rows;'.
      'document.'.$this->table_name.'.submit();}</script>';

		
		
    return $footer;
  }

  /**
   * Return pagination content.
   *
   * @return html code
   */
  function pagination()
  {
    global $GO_THEME, $cmdPrevious, $cmdNext, $cmdShowAll;
  	
    $links = '';
    $max_links=10;
    if ($this->max_rows != 0)
    {
      if ($this->row_count > $this->max_rows)
      {
	$links = '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td>';
	$next_first = $this->start_row+$this->max_rows;
	$previous_first = $this->start_row-$this->max_rows;
	if ($this->start_row != 0)
	{
	  $links .= '<a href="javascript:change_list(0, '.$this->max_rows.');">&lt&lt</a>&nbsp;';
	  $links .= '<a href="javascript:change_list('.$previous_first.', '.$this->max_rows.');">'.$cmdPrevious.'</a>&nbsp;';
	}else
	{
	  $links .= '<font color="#cccccc">&lt&lt '.$cmdPrevious.'</font>&nbsp;';
	}
	
	$start = ($this->start_row-(($max_links/2)*$this->max_rows));	
	$end = ($this->start_row+(($max_links/2)*$this->max_rows));
	
	if ($start < 0)
	{
	  $end = $end - $start;
	  $start=0;
	}
	if ($end > $this->row_count)
	{
	  $end = $this->row_count;
	}
	if ($start> 0)
	{
	  $links .= '...&nbsp;';
	}
	
	for ($i=$start;$i<$end;$i+=$this->max_rows)
	{
	  $page = ($i/$this->max_rows)+1;
	  if ($i==$this->start_row)
	  {
	    $links .= '<b><i>'.$page.'</i></b>&nbsp;';
	  }else
	  {
	    $links .= '<a href="javascript:change_list('.$i.', '.$this->max_rows.');">'.$page.'</a>&nbsp;';
	  }
	}
	
	if ($end < $this->row_count)
	{
	  $links .= '...&nbsp;';
	}
	
	$last_page = floor($this->row_count/$this->max_rows)*$this->max_rows;
	
	if ($this->row_count > $next_first)
	{
	  $links .= '<a href="javascript:change_list('.$next_first.', '.$this->max_rows.');">'.$cmdNext.'</a>&nbsp;';
	  $links .= '<a href="javascript:change_list('.$last_page.', '.$this->max_rows.');">&gt&gt</a>';
	}else
	{
	  $links .= '<font color="#cccccc">'.$cmdNext.' &gt&gt</font>';
	}
	$links .= '</td><td align="right"><a class="normal" href="javascript:change_list(0, 0);">'.$cmdShowAll.'</a></td></tr></table>';	
	$links = '<tr height="20"><td colspan="99">'.$links.'</td></tr>'.
	  '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
      }
      return $links;
    }
  }

  /**
   * Return sql table.
   *
   * @return html code.
   */
  function data()
  {
    global $GO_THEME, $strDeleteRow;
    $sql = "SELECT * FROM `".$this->table_name."`";
  	
    if($this->max_rows > 0)
    {
      $this->query($sql);
      $this->row_count = $this->num_rows(); 		
    }
  	
    if($this->order_field != '')
    {
      $sql .= " ORDER BY `".$this->order_field."` ".$this->order_direction;
    }
  	
    if($this->max_rows > 0)
    {
      $sql .= " LIMIT ".$this->start_row.",".$this->max_rows;
      $this->query($sql);
    }else
    {
      $this->query($sql);
      $this->row_count = $this->num_rows();
    }
  	
    $data = '';
    while($this->next_record())
    {
      $data .= '<tr>';
  		
      $column_count = count($this->Record)/2;
  		
      for($i=0;$i<$column_count;$i++)
      {
	if($i == $this->id_field)
	{
	  $id = $this->f($i);
  				
	  $data .= '<input type="hidden" name="'.$this->fields[$i].'[]" value="'.$this->f($i).'" />';
	}else
	{
	  $data .= '<td><input type="text" name="'.$this->fields[$i].'[]" class="textbox" value="'.$this->f($i).'" /></td>';
	}
      }
  		
      $data .= '<td><a href=\'javascript:delete_row("'.$id.
	'","'.rawurlencode($strDeleteRow).'")\'"><img src="'.
	$GO_THEME->images['delete'].'" border="0"></a></td>';
  		
      $data .= '</tr>';	
    }
    return $data;
  }

  /**
   * Do sql command UPDATE
   */
  function update()
  {
    for($i=0;$i<count($_POST[$this->id_field_name]);$i++)
    {
      $sql = "UPDATE `".$this->table_name."` SET ";
  	 		
      for($n=0;$n<count($this->fields);$n++)
      {
	if($n != $this->id_field)
	{
	  $sql .= "`".$this->fields[$n]."`='".smart_addslashes($_POST[$this->fields[$n]][$i])."',";
	}
      }
      $sql = substr($sql,0,-1);
  	 		
      $sql .= " WHERE `".$this->id_field_name."`='".$_POST[$this->id_field_name][$i]."'";
      $this->query($sql);
    }
  }

  /**
   * Do sql command INSERT
   */
  function insert()
  {
    $next_id = $this->nextid($this->table_name);
  	
    $sql = "INSERT INTO `".$this->table_name."` (".$this->id_field_name.") ".
      "VALUES ('$next_id')";
    return $this->query($sql);
  }

  /**
   * Do sql command DELETE
   */
  function delete($id)
  {
    $sql = "DELETE FROM `".$this->table_name."` WHERE ".$this->id_field_name."='$id'"; 		
    $this->query($sql);
		
    for($i=0;$i<count($this->delete_tables);$i++)
    {
      $sql = "DELETE FROM `".$this->delete_tables[$i]['name']."` WHERE `".$this->delete_tables[$i]['id_field']."`='$id'"; 		
      $this->query($sql);
    }
  }
}
?>
