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
require("../../Group-Office.php");
$GO_SECURITY->authenticate();
require($GO_LANGUAGE->get_base_language_file('groups'));

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$group_users = isset($_REQUEST['group_users']) ? $_REQUEST['group_users'] : array();

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
  if (isset($_REQUEST['search_field']))
  {
    SetCookie("user_search_field",$_REQUEST['search_field'],time()+3600*24*365,"/","",0);
    $_COOKIE['user_search_field'] = $_REQUEST['search_field'];
  }

  if ($_POST['group_id'] != 0 && !$GO_GROUPS->user_owns_group($GO_SECURITY->user_id, $_POST['group_id']))
  {
    $feedback = '<p class="Error">'.$strAccessDenied.'</p>';
    $task = '';
  }else
  {
    switch ($task)
    {
      case 'set_leader':
        if (count($group_users) == 1) {
          $GO_GROUPS->set_group_leader($_POST['group_id'], $group_users[0]);
        }
        break;

      case 'delete_users':
        $group = $GO_GROUPS->get_group($_POST['group_id']);
	for ($i=0;$i<count($group_users);$i++)
	{
	  $GO_GROUPS->delete_user_from_group($group_users[$i],$_POST['group_id']);
	  if ($group_users[$i] == $group['leader_id'])
	    $GO_GROUPS->set_group_leader($_POST['group_id'],0);
	}
	break;

      case 'save_add_users':
	for ($i=0;$i<count($group_users);$i++)
	{
	  if (!$GO_GROUPS->is_in_group($group_users[$i], $_POST['group_id']))
	  {
	    $GO_GROUPS->add_user_to_group($group_users[$i],$_POST['group_id']);
	  }
	}
	break;

      case 'save_group_name':
	$group_name = smart_addslashes(trim($_POST['group_name']));

	if ($group_name != "")
	{
	  if (validate_input($group_name))
	  {
	    if ($_POST['group_id'] == '0')
	    {
	      if (!$GO_GROUPS->get_group_by_name($group_name))
	      {
		if (!$_REQUEST['group_id'] = $GO_GROUPS->add_group($GO_SECURITY->user_id, $group_name))
		{
		  $_REQUEST['group_id'] = 0;
		  $feedback = "<p class=\"Error\">".$add_group_fail."</p>";
		}else
		{
		  if ($_POST['close'] == 'true')
		  {
		    header('Location: '.$GO_CONFIG->host.'configuration/groups/index.php');
		    exit();
		  }
		}
	      }else
	      {
		$feedback = "<p class=\"Error\">".$add_group_exists."</p>";
	      }
	    }else
	    {
	      $existing_group = $GO_GROUPS->get_group_by_name($group_name);

	      if($existing_group && $existing_group['id'] != $_POST['group_id'])
	      {
		$feedback = "<p class=\"Error\">".$add_group_exists."</p>";
	      }else
	      {
		$GO_GROUPS->update_group($_POST['group_id'], $group_name);

		if ($_POST['close'] == 'true')
		{
		  header('Location: '.$GO_CONFIG->host.'configuration/groups/index.php');
		  exit();
		}
	      }
	    }
	  }else
	  {
	    $feedback = "<p class=\"Error\">".$invalid_chars.": \\ / ? & \"</p>";
	  }

	}else
	{
	  $feedback = "<p class=\"Error\">".$add_group_no_name."</p>";
	}
	break;
    }
  }
}

if (!isset($_REQUEST['group_id']))
{
  $group_name = $groups_new_group;
  $group_id = 0;
}else
{
  $group = $GO_GROUPS->get_group($_REQUEST['group_id']);
  $group_name = $group['name'];
  $group_id = $_REQUEST['group_id'];
  if ($group['leader_id'] == 0)
    $group_leader = '';
  else {
    $leader = $GO_GROUPS->get_group_leader($group_id, $group['leader_id']);
    if (!$leader)
      $group_leader = '';
    else {
      $middle_name = $leader['middle_name'];
      if ($middle_name)
        $middle_name = ' '.$middle_name.' ';
      else
        $middle_name = ' ';
      $group_leader = $leader['last_name'] . $middle_name . $leader['first_name'];
    }
  }
}

if ($group_id == $GO_CONFIG->group_everyone)
{
  $feedback = '<p class="Error">'.$groups_everyone.'</p>';
  $enabled = false;
  $disabled = 'disabled';
}elseif($group_id != 0 && !$GO_GROUPS->user_owns_group($GO_SECURITY->user_id, $group_id))
{
  $enabled = false;
  $disabled = 'disabled';
}else
{
  $disabled = '';
  $enabled = true;
}

$page_title = $groups_title;
require($GO_THEME->theme_path."header.inc");

$tabtable = new tabtable('group_tab', $group_name, '600', '300');
?>
<form name="group" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="group_id" value="<?php echo $group_id; ?>" />
<input type="hidden" name="task" value="" />
<input type="hidden" name="close" value="false" />
<?php
$tabtable->print_head();
if ($task == 'add_users')
{
  echo '<table border="0" cellpadding="0" cellspacing="3">';

  if (isset($feedback))
  {
    echo '<tr><td>'.$feedback.'</td></tr>';
  }
  echo '<tr><td>';

  $_COOKIE['user_search_field'] = isset($_COOKIE['user_search_field']) ? $_COOKIE['user_search_field'] : 'first_name';
  $search_field = isset($_POST['search_field']) ? $_POST['search_field'] : $_COOKIE['user_search_field'];

  $dropbox = new dropbox();
  $dropbox->add_value('first_name', $strFirstName);
  $dropbox->add_value('last_name', $strLastName);
  $dropbox->add_value('email', $strEmail);
  $dropbox->add_value('company', $strCompany);
  $dropbox->add_value('department',$strDepartment);
  $dropbox->add_value('function',$strFunction);
  $dropbox->add_value('address',$strAddress);
  $dropbox->add_value('city', $strCity);
  $dropbox->add_value('zip',$strZip);
  $dropbox->add_value('state',$strState);
  $dropbox->add_value('country', $strCountry);
  $dropbox->add_value('work_address',$strWorkAddress);
  $dropbox->add_value('work_cip',$strWorkZip);
  $dropbox->add_value('work_city',$strWorkCity);
  $dropbox->add_value('work_state',$strWorkState);
  $dropbox->add_value('work_country',$strWorkCountry);
  $dropbox->print_dropbox('search_field', $search_field);

  echo '</td><td><input type="text" name="query" size="31" maxlength="255" class="textbox" value="';
  if (isset($_REQUEST['query'])) echo smartstrip($_REQUEST['query']);
  echo '"></td></tr>';
  echo '<tr><td colspan="2">';
  echo '<table><tr><td>';
  $button = new button($cmdSearch, 'javascript:add_users()');
  echo '</td><td>';
  $button = new button($cmdShowAll, "javascript:document.group.query.value='';add_users()");
  echo '</td><td>';
  $button = new button($cmdCancel, 'javascript:return_to_group()');
  echo '</td></tr></table>';
  echo '</td></tr></table>';

  if (isset($_REQUEST['query']))
  {
    echo '<table border="0" cellpadding="3" cellspacing="0"><tr><td>';
    if ($_REQUEST['query'] != '')
    {
      $GO_USERS->search('%'.smart_addslashes($_REQUEST['query']).'%', smart_addslashes($search_field), $GO_SECURITY->user_id);
    }else
    {
      $GO_USERS->get_authorized_users($GO_SECURITY->user_id);
    }

    echo '<select name="group_users[]" multiple="true" style="width: 250px;height: 200px;" class="textbox">';

    while ($GO_USERS->next_record())
    {
      $middle_name = $GO_USERS->f('middle_name') == '' ? '' : $GO_USERS->f('middle_name').' ';
      //$name = $GO_USERS->f('first_name').' '.$middle_name.$GO_USERS->f('last_name');
      $name = $GO_USERS->f('last_name').' '.$middle_name.$GO_USERS->f('first_name');

      echo '<option value="'.$GO_USERS->f('id').'">'.$name.'</option>';
    }
    echo '</select>';
    echo '<table><tr><td>';
    $button = new button($cmdAdd, 'javascript:save_add_users()');
    echo '</td></tr></table>';
    echo '</td></tr></table>';
  }
}else
{
  ?>
    <table border="0" cellpadding="3" cellspacing="0">
    <tr>
    <td colspan="2">
    <?php
    if (isset($feedback)) echo $feedback;
  ?>
    </td>
    </tr>
    <tr>
    <td>
    <?php echo $strName; ?>:
    </td>
    <td>
    <input type="text" class="textbox" maxlength="50" name="group_name" value="<?php echo stripslashes(htmlspecialchars($group_name)); ?>" size="30" <?php echo $disabled; ?> />
    </td>
    </tr>
	<?php
	if ($group_id > 0)
	{
      if ($group_id > $GO_CONFIG->group_everyone) {
        echo "<tr><td>$strGroupLeader:</td>";
        echo "<td><input type=\"text\" class=\"textbox\" maxlength=\"50\" name=\"group_leader\" value=\"$group_leader\" size=\"30\" disabled /></td></tr>";
	  }
      echo '<tr><td valign="top">'.$groups_members.':</td><td>';
      $GO_GROUPS->get_users_in_group($group_id, "name", "ASC");
      echo '<select name="group_users[]" multiple="true" style="width: 250px;height: 100px;" class="textbox" '.$disabled.'>';

      while ($GO_GROUPS->next_record())
      {
	if ( $GO_GROUPS->f('last_name')) {
	  $middle_name = $GO_GROUPS->f('middle_name') == '' ? '' : $GO_GROUPS->f('middle_name').' ';
	  //$name = $GO_GROUPS->f('first_name').' '.$middle_name.$GO_GROUPS->f('last_name');
	  $name = $GO_GROUPS->f('last_name').' '.$middle_name.$GO_GROUPS->f('first_name');
	  echo '<option value="'.$GO_GROUPS->f('id').'">'.$name.'</option>';
	} else {
	  //TODO: get_users_in_group should return all info!
	  //move profiles into abstract LDAP classes
	  require_once($GO_CONFIG->class_path.'profiles.class.inc');
	  $profiles = new profiles();
	  if ($profile = $profiles->get_profile($GO_GROUPS->f('user_id'))) {
	    $middle_name = $profile["middle_name"] == '' ? '' : $profile["middle_name"].' ';
	    //$name = $profile["first_name"].' '.$middle_name.$profile["last_name"];
	    $name = $profile["last_name"].' '.$middle_name.$profile["first_name"];
	    echo '<option value="'.$GO_GROUPS->f('user_id').'">'.$name.'</option>';
	  }
	}
      }
      echo '</select>';
      if ($enabled)
      {
	echo '<table><tr><td>';
	$button = new button($cmdAdd, 'javascript:add_users()', 60);
	echo '</td><td>';
	$button = new button($cmdDelete, 'javascript:delete_users()', 60);
	echo '</td><td>';
	$button = new button($cmdSetLeader, 'javascript:set_leader()', 115);
	echo '</td></tr></table>';
      }
      echo '</td></tr></table>';
    }
  echo '<br /><table><tr><td>';
  if ($enabled)
  {
    $button = new button($cmdOk, 'javascript:save_close_group_name()');
    echo '</td><td>';
    $button = new button($cmdApply, 'javascript:save_group_name()');
    echo '</td><td>';
  }
  $button = new button($cmdClose, "javascript:document.location='index.php'");
  echo '</td></tr>';
  echo '</table>';

}
$tabtable->print_foot();
?>

  <script type="text/javascript">
function set_leader()
{
  document.group.task.value='set_leader';
  document.group.submit();
}

function delete_users()
{
  document.group.task.value='delete_users';
  document.group.submit();
}

function save_add_users()
{
  document.group.task.value='save_add_users';
  document.group.submit();
}
function add_users()
{
  document.group.task.value='add_users';
  document.group.submit();
}
function save_group_name()
{
  document.group.task.value='save_group_name';
  document.group.submit();
}
function save_close_group_name()
{
  document.group.close.value='true';
  document.group.task.value='save_group_name';
  document.group.submit();
}
function return_to_group()
{
  document.group.task.value='';
  document.group.submit();
}
</script>
</form>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
