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

class acl
{
  var $form_name;
  var $acl_id;
  var $read_only;
  var $group_detail;
  var $selected_groups_name;
  var $selected_users_name;
  var $ACL;

  /**
   * Constructor.
   *
   * @param	int		$acl_id
   * @param	string	$acl_table
   */
  function acl($acl_id,$acl_table='acl')
  {
    global $GO_SECURITY;

    $task = isset($_POST['task']) ? $_POST['task'] : '';

    $this->ACL = new GO_SECURITY();
    $this->ACL->acl_table = $acl_table;
    if ($acl_id > 0)
    {
      $this->acl_id = $acl_id;
      //$this->read_only = $read_only == 'true' ? true : false;
      $this->read_only = !$this->ACL->user_owns_acl($GO_SECURITY->user_id, $acl_id);
      $this->group_detail = isset($_POST['groupdetail']) ? $_POST['groupdetail'] : '0';
      $this->form_name = 'acl_control_'.$acl_id;
      $this->selected_groups_name = "selected_groups_".$acl_id;
      $this->selected_users_name = "selected_users_".$acl_id;

      if ($_SERVER['REQUEST_METHOD'] == "POST")
      {
	switch($task)
	{
	  case 'delete_users':
	    $this->delete_users();
	    break;

	  case 'delete_groups':
	    $this->delete_groups();
	    break;

	  case 'save_add_groups':
	    $this->add_groups();
	    break;

	  case 'save_add_users':
	    $this->add_users();
	    break;
	}
      }
      switch($task)
      {
	case "add_groups":
	  $this->print_user_groups();
	break;

	case "add_users":
	  $this->print_users();
	break;

	default:
	$this->print_acl();
	break;
      }

    }else
    {
      die('<b>Fatal error:</b> acl id not set');
    }
  }

  /**
   * Print add users form.
   */
  function print_users()
  {
    global $GO_SECURITY, $GO_CONFIG, $GO_USERS,
    $cmdAdd, $cmdCancel, $cmdShowAll, $cmdSearch;

    echo '<form name="'.$this->form_name.'" method="post" action="'.$_SERVER['PHP_SELF'].'">';
    echo '<input type="hidden" name="task" value="search" />';
    echo '<input type="hidden" name="acl_id" value="'.$this->acl_id.'" />';
    echo '<input type="hidden" name="acl_table" value="'.$this->ACL->acl_table.'" />';
    //echo '<input type="hidden" name="read_only" value="'.$this->read_only.'" />';

    echo '<table border="0" cellpadding="0" cellspacing="3"><tr><td>';

    $search_field = isset($_POST['search_field']) ? $_POST['search_field'] : 'first_name';

    $dropbox = new dropbox();
    foreach ( $GO_USERS->get_search_fields() as $fields ) {
      $dropbox->add_value( $fields[0], $fields[1] );
    }
    $dropbox->print_dropbox('search_field', $search_field);

    echo '</td><td><input type="text" name="query" size="31" maxlength="255" class="textbox" value="';
    if (isset($_POST['query'])) echo smartstrip($_POST['query']);
    echo '"></td></tr>';
    echo '<tr><td colspan="2">';
    echo '<table><tr><td>';
    $button = new button($cmdSearch, 'javascript:search()');
    echo '</td><td>';
    $button = new button($cmdShowAll, "javascript:document.".$this->form_name.".query.value='';search()");
    echo '</td><td>';
    $button = new button($cmdCancel, 'javascript:return_to_acl()');
    echo '</td></tr></table>';
    echo '</td></tr></table>';


    if (isset($_POST['query']))
    {
      if ($_POST['query'] != '')
      {
	$GO_USERS->search('%'.smart_addslashes($_POST['query']).'%', smart_addslashes($search_field), $GO_SECURITY->user_id);
      }else
      {
	$GO_USERS->get_authorized_users($GO_SECURITY->user_id);
      }

      echo '<select name="'.$this->selected_users_name.'[]" multiple="true" style="width: 250px;height: 100px;" class="textbox">';

      while ($GO_USERS->next_record())
      {
	$middle_name = $GO_USERS->f('middle_name') == '' ? '' : $GO_USERS->f('middle_name').' ';
	$name = $GO_USERS->f('last_name').' '.$middle_name.$GO_USERS->f('first_name');
	echo '<option value="'.$GO_USERS->f('id').'">'.htmlspecialchars($name).'</option>';
      }
      echo '</select>';
      echo '<table><tr><td>';
      $button = new button($cmdAdd, 'javascript:add_users()');
      echo '</td></tr></table>';
    }

    echo '<script type="text/javascript">

      function search()
      {
	document.'.$this->form_name.'.task.value="add_users";
	document.'.$this->form_name.'.submit();
      }
    function add_users()
    {
      document.'.$this->form_name.'.task.value="save_add_users";
      document.'.$this->form_name.'.submit();
    }

    function return_to_acl()
    {
      document.'.$this->form_name.'.task.value="";
      document.'.$this->form_name.'.submit();
    }

    var nav4 = window.Event ? true : false;
    function processkeypress(e)
    {
      if(nav4)
      {
	var whichCode = e.which;
      }else
      {
	var whichCode = event.keyCode;
      }

      if (whichCode == 13)
      {
	search();
	return true;
      }
    }
    if (window.Event) //if Navigator 4.X
    {
      document.captureEvents(Event.KEYPRESS)
    }
    document.onkeypress = processkeypress;
    document.forms[0].query.focus();
    </script>
      ';

    echo '</form>';
  }

  /**
   * Print add groups form
   */
  function print_user_groups()
  {
    global $GO_SECURITY, $GO_CONFIG, $cmdAdd, $cmdCancel, $_SERVER, $query, 
    $GO_GROUPS, $cmdShowAll, $cmdSearch,$add_group_showall,$add_group_showauth;

    echo '<form name="'.$this->form_name.'" method="post" action="'.
      $_SERVER['PHP_SELF'].'">';
    echo '<input type="hidden" name="task" value="" />';
    echo '<input type="hidden" name="acl_id" value="'.$this->acl_id.'" />';
    echo '<input type="hidden" name="acl_table" value="'.$this->ACL->acl_table.'" />';
    echo '<input type="hidden" name="read_only" value="'.
      $this->read_only.'" />';
    echo '<input type="hidden" name="show_all" value="'.(isset($_REQUEST['show_all']) ? $_REQUEST['show_all'] : 'false').'" />';
    if (isset($_REQUEST['show_all']) && $_REQUEST['show_all'] == 'true')
      echo '<a href="javascript:document.'.$this->form_name.'.show_all.value=\'false\';refresh_groups();">'.$add_group_showauth.'</a><br/>';
    else
      echo '<a href="javascript:document.'.$this->form_name.'.show_all.value=\'true\';refresh_groups();">'.$add_group_showall.'</a><br/>';
    

    echo '<select name="'.$this->selected_groups_name.'[]" multiple="true" style="width: 250px;height: 100px;" class="textbox">';
    if (isset($_REQUEST['show_all']) && $_REQUEST['show_all'] == 'true')
      $GO_GROUPS->get_all_groups();
    else
      $GO_GROUPS->get_authorised_groups($GO_SECURITY->user_id);
    while ($GO_GROUPS->next_record())
    {
      echo '<option value="'.$GO_GROUPS->f('id').'">'.htmlspecialchars($GO_GROUPS->f('name')).'</option>';
    }
    echo '</select>';
    echo '<table><tr><td>';
    $button = new button($cmdAdd, 'javascript:add_groups()');
    echo '</td><td>';
    $button = new button($cmdCancel, 'javascript:return_to_acl()');
    echo '</td></tr></table>';

    echo '</form>';

    echo '<script type="text/javascript">

      function refresh_groups()
      {
	document.'.$this->form_name.'.task.value="add_groups";
	document.'.$this->form_name.'.submit();
      }

      function add_groups()
      {
	document.'.$this->form_name.'.task.value="save_add_groups";
	document.'.$this->form_name.'.submit();
      }

    function return_to_acl()
    {
      document.'.$this->form_name.'.task.value="";
      document.'.$this->form_name.'.submit();
    }
    </script>
      ';
  }

  /** 
   * Print list of users in acl
   */
  function print_groups_in_acl()
  {
    global $GO_SECURITY, $GO_CONFIG, $cmdAdd, $cmdDelete;
    $disabled = $this->read_only ? 'disabled' : '';

    echo '<select name="'.$this->selected_groups_name.'[]"'.
      ' multiple="true" style="width: 220px;height: 250px;"'.
      ' class="textbox" '.$disabled.'>';

    $this->ACL->get_groups_in_acl($this->acl_id);
    $group_db = new GO_GROUPS();

    while ($this->ACL->next_record())
      // echo '<option value="'.$this->ACL->f('id').'">'
      //	.$this->ACL->f('name').'</option>';
    {
      if ( $this->ACL->f("name") )
      {
	if (!$this->hide_self ||
	    ($this->ACL->f("id") != $this->ACL->group_id))
	{
	  echo '<option value="'.$this->ACL->f('id').'">'.
	    htmlspecialchars($this->ACL->f('name')).'</option>';
	  $group_id = $this->ACL->f('id');
	}
      } else
      {
	if ( !$this->hide_self ||
	    ( $this->ACL->f("user_id" != $this->ACL->group_id ) ) )
	{
	  require_once($GO_CONFIG->class_path.'profiles.class.inc');
	  $profiles = new profiles();
	  if ($profile = $profiles->get_group_profile($this->ACL->
		f('group_id')))
	  {
	    echo '<option value="'.$this->ACL->f('group_id').
	      '">'.htmlspecialchars($profile["name"]).'</option>';
	    $group_id = $this->ACL->f('group_id');
	  }
	}
      }

      if (isset($group_id) && $this->group_detail == '1')
      {
	$group_db->get_users_in_group($group_id);
	while ($group_db->next_record())
	{
	  $name = $group_db->f('last_name') .' '. $group_db->f('middle_name') .' '. $group_db->f('first_name');
	  echo '<option disabled>&nbsp;&nbsp;'.htmlspecialchars($name).'</option>';
	}
      }
      //$this->ACL->get_groups_in_acl_with_users($this->acl_id);

    }
    echo '</select>';

    if (!$this->read_only)
    {
      echo '<table><tr><td>';
      $button = new button($cmdAdd, 'javacript:add_groups()');
      echo '</td><td>';
      $button = new button($cmdDelete, 'javascript:delete_groups(document.forms[0].'.$this->selected_groups_name.')');
      echo '</td></tr></table>';
    }
  }

  /** 
   * Print list of users in acl
   */
  function print_users_in_acl()
  {
    global $GO_SECURITY, $GO_USERS, $cmdAdd, $cmdDelete;

    $disabled = $this->read_only ? 'disabled' : '';

    $this->ACL->get_users_in_acl($this->acl_id);
    echo '<select name="'.$this->selected_users_name.
      '[]" multiple="true" style="width: 220px;height: 250px;"'.
      ' class="textbox" '.$disabled.'>';

    while ($this->ACL->next_record())
    {
      if ( $profile = $GO_USERS->get_user( $this->ACL->f('user_id') ) )
      {
	$middle_name = $profile["middle_name"] == '' ?
	  '' : $profile["middle_name"].' ';
	$name = $profile["last_name"].' '.$middle_name.$profile["first_name"];
	echo '<option value="'.$this->ACL->f('user_id').'">'
	  .htmlspecialchars($name).'</option>';
      }
    }
    echo '</select>';
    if (!$this->read_only)
    {
      echo '<table><tr><td>';
      $button = new button($cmdAdd, 'javascript:add_users()');
      echo '</td><td>';
      $button = new button($cmdDelete, 'javascript:delete_users(document.forms[0].'.$this->selected_users_name.')');
      echo '</td></tr></table>';
    }
  }

  /** 
   * Print list of users in acl (test mode). That is, print all authorised users.
   */
   /* -- comment since it's not been used anymore
  function test_mode_print_users_in_acl()
  {
    global $GO_SECURITY, $GO_USERS, $cmdAdd, $cmdDelete;

    $this->ACL->get_all_users_in_acl($this->acl_id,true);
    $db = &$this->ACL;
    echo '<select name="'.$this->selected_users_name.
      '[]" multiple="true" style="background: none;width: 220px;height: 250px;"'.
      ' class="textbox" disabled="1">';

    while ($db->next_record())
    {
	$middle_name = $db->f("middle_name") == '' ?
	  '' : $db->f("middle_name").' ';
	$name = $db->f("last_name").' '.$middle_name.$db->f("first_name");
	echo '<option value="'.$db->f('id').'">'
	  .htmlspecialchars($name).'</option>';
    }
    echo '</select>';
  }
  */
  /**
   * Print the ACL form.
   */
  function print_acl()
  {
    echo '<script type="text/javascript">
		function add_groups()
		{
		document.'.$this->form_name.'.task.value="add_groups";
		document.'.$this->form_name.'.submit();
		}

		function add_users()
		{
		document.'.$this->form_name.'.task.value="add_users";
		document.'.$this->form_name.'.submit();
		}
		function delete_groups()
		{
		document.'.$this->form_name.'.task.value="delete_groups";
		document.'.$this->form_name.'.submit();
		}

		function delete_users()
		{
		  document.'.$this->form_name.'.task.value="delete_users";
		  document.'.$this->form_name.'.submit();
		}
		function group_detail_toggle()
		{
		  document.'.$this->form_name.'.groupdetail.value= (document.'.$this->form_name.'.groupdetail.value == "1" ? "0" : "1");
		  document.'.$this->form_name.'.submit();
		}

		</script>
		  ';
    global $acl_control_auth_groups, $acl_control_auth_users;
    global $acl_control_auth_groups_detail_on,$acl_control_auth_groups_detail_off;
    echo '<form name="'.$this->form_name.'" method="post" action="'.$_SERVER['PHP_SELF'].'">';
    echo '<input type="hidden" name="task" value="" />';
    echo '<input type="hidden" name="acl_id" value="'.$this->acl_id.'" />';
    echo '<input type="hidden" name="acl_table" value="'.$this->ACL->acl_table.'" />';
    echo '<input type="hidden" name="read_only" value="'.$this->read_only.'" />';
    echo '<input type="hidden" name="groupdetail" value="'.$this->group_detail.'" />';
    echo '<table border="0" cellspacing="0" cellpadding="0">';
    echo '<tr><td><b>'.$acl_control_auth_users.'</b></td><td width="20">&nbsp;</td>';
    echo '<td><b>'.$acl_control_auth_groups.'</b>&nbsp;<a href="javascript:group_detail_toggle()">'.($this->group_detail == '1' ? $acl_control_auth_groups_detail_off : $acl_control_auth_groups_detail_on).'</a></td><td width="20">&nbsp;</td>';
//    echo '<td><h3>'.$acl_all_auth_users.'</td></tr>';
    echo '</tr>';
    echo '<tr><td>';
    $this->print_users_in_acl();
    echo '</td><td width="20">&nbsp;</td><td>';
    $this->print_groups_in_acl();
/*
    echo '</td><td width="20">&nbsp;</td><td valign="top">';
    $this->test_mode_print_users_in_acl();
*/
    echo '</td></tr>';
    echo '</table>';
    /*
		$this->print_groups_in_acl();
		echo '<br />';
		echo '<h3>'.$acl_control_auth_users.'</h3>';
		$this->print_users_in_acl();
    */
    echo '</form>';
  }

  /**
   * Post process delete groups operation. Delete selected groups from the acl.
   */
  function delete_groups()
  {
    global $GO_SECURITY, $GO_CONFIG;

    $selected_groups = isset($_POST[$this->selected_groups_name])
      ? $_POST[$this->selected_groups_name] : array();

    for ($i=0;$i<count($selected_groups);$i++)
    {
      $this->ACL->delete_group_from_acl($selected_groups[$i],
					  $this->acl_id);

      if($GO_CONFIG->dav_switch){
	global $GO_DAV;
	if ( $GO_DAV->check_acl($this->acl_id) )
	  $GO_DAV->grouping($selected_groups[$i],
			    $this->acl_id,"delete");
      }

    }
  }

  /**
   * Post process delete users operation. Delete selected users from the acl.
   */
  function delete_users()
  {
    global $GO_SECURITY, $GO_CONFIG;

    $selected_users = isset($_POST[$this->selected_users_name]) 
      ? $_POST[$this->selected_users_name] : array();

    for ($i=0;$i<count($selected_users);$i++)
    {
      if ($selected_users[$i] != $GO_SECURITY->user_id)
      {
	$this->ACL->delete_user_from_acl($selected_users[$i],
					   $this->acl_id);

	if($GO_CONFIG->dav_switch) {
	  global $GO_DAV;
	  if ( $GO_DAV->check_acl($this->acl_id) )
	    $GO_DAV->delete_user($selected_users[$i],
				 $this->acl_id);
	}

      }
    }
  }

  /**
   * Post process add groups operation. Add selected groups.
   */
  function add_groups()
  {
    global $GO_SECURITY, $GO_CONFIG;

    $selected_groups = isset($_POST[$this->selected_groups_name])
      ? $_POST[$this->selected_groups_name] : array();

    for ($i=0;$i<count($selected_groups);$i++)
    {
      if (!$this->ACL->group_in_acl($selected_groups[$i], 
				      $this->acl_id))
      {
	$this->ACL->add_group_to_acl($selected_groups[$i],
				       $this->acl_id);

	if($GO_CONFIG->dav_switch) {
	  global $GO_DAV;
	  if ( $GO_DAV->check_acl($this->acl_id) )
	    $GO_DAV->grouping($selected_groups[$i],
			      $this->acl_id,"add");
	}

      }
    }
  }

  /**
   * Post process add users operation. Add selected users.
   */
  function add_users()
  {
    global $GO_SECURITY, $GO_CONFIG;

    $selected_users = isset($_POST[$this->selected_users_name]) 
      ? $_POST[$this->selected_users_name] : array();

    for ($i=0;$i<count($selected_users);$i++)
    {
      if (!$this->ACL->user_in_acl($selected_users[$i], 
				     $this->acl_id))
      {
	$this->ACL->add_user_to_acl($selected_users[$i],
				      $this->acl_id);

	if($GO_CONFIG->dav_switch) {
	  global $GO_DAV;
	  if ( $GO_DAV->check_acl($this->acl_id) )
	    $GO_DAV->add_user($selected_users[$i],$this->acl_id);
	}

      }
    }
  }
}
?>
