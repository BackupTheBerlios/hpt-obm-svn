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

define('TASK_LIST_TAB', 2);

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('projects');
require($GO_LANGUAGE->get_language_file('projects'));

//check for the addressbook module
$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || 
    !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) ||
      $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
  $ab_module = false;
}else
{
  require_once($ab_module['class_path'].'addressbook.class.inc');
  $ab = new addressbook();
}


$page_title=$lang_modules['projects'];
require($GO_MODULES->class_path."projects.class.inc");
$projects = new projects();

require($GO_MODULES->class_path."task.class.inc");

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$project_id = isset($_REQUEST['project_id']) ? $_REQUEST['project_id'] : 0;

$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
/*
if ($_SESSION['return_to']['pid'] != $project_id ||
    $_SESSION['return_to']['mod'] != $_SESSION['GO_SESSION']['active_module']) {
  $return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
  $_SESSION['return_to']['pid'] = $project_id;
  $_SESSION['return_to']['url'] = $return_to;
  $_SESSION['return_to']['mod'] = $_SESSION['modules']['active_module'];
}
else {
  $return_to = $_SESSION['return_to']['url'];
}
*/

switch ($task)
{
  case 'save_hours':
    $_COOKIE['unit_value_cookie'] = isset($_COOKIE['unit_value_cookie']) ? $_COOKIE['unit_value_cookie'] : '';
    $unit_value = isset($_GET['unit_value']) ? $_GET['unit_value'] : $_COOKIE['unit_value_cookie'];

    SetCookie("registration_method_cookie",$_GET['registration_method'],time()+3600*24*365,"/","",0);
    SetCookie("unit_value_cookie",$unit_value,time()+3600*24*365,"/","",0);

    //translate the given date stamp to unix time
    $start_date_array = explode('-',$_GET['book_start_date']);
    $start_year = $start_date_array[2];

    if ($_SESSION['GO_SESSION']['date_format'] == "m-d-Y")
    {
      $start_month = $start_date_array[0];
      $start_day = $start_date_array[1];
    }else
    {
      $start_month = $start_date_array[1];
      $start_day = $start_date_array[0];
    }

    $start_time = mktime($_GET['start_hour'], $_GET['start_min'], 0, $start_month, $start_day, $start_year)-($_SESSION['GO_SESSION']['timezone']*3600);

    //if user gave a number of units calulate ending time
    if ($_GET['registration_method'] == 'units')
    {
      $end_time = $start_time + $unit_value*60*$_GET['units'];
      $break_time=0;
    }else
    {
      //translate the given date stamp to unix time
      $end_date_array = explode('-',$_GET['book_end_date']);
      $end_year = $end_date_array[2];
      if ($_SESSION['GO_SESSION']['date_format'] == "m-d-Y")
      {
        $end_month = $end_date_array[0];
        $end_day = $end_date_array[1];
      }else
      {
        $end_month = $end_date_array[1];
        $end_day = $end_date_array[0];
      }
      $break_time = ($_GET['break_hours']*3600)+($_GET['break_mins']*60);
      $end_time = mktime($_GET['end_hour'], $_GET['end_min'], 0, $end_month, $end_day, $end_year)-($_SESSION['GO_SESSION']['timezone']*3600);;
      $unit_value=0;
    }

    if ($end_time < $start_time)
    {
      $feedback = '<p class="Error">'.$pm_invalid_period.'</p>';

    }elseif(!$projects->check_hours($_GET['pm_user_id'], $start_time, $end_time))
    {
      $feedback = '<p class="Error">'.$pm_already_booked.'</p>';
    }else
    {
      if (!$projects->add_hours($_GET['project_id'], $_GET['pm_user_id'],
            $start_time, $end_time, $break_time, $unit_value,
            smart_addslashes($_GET['book_comments'])))
      {
        $feedback = '<p class="Error">'.$strSaveError.'</p>';
      }else
      {
        $feedback = '<p class="Success">'.$pm_add_hours_success.'</p>';
        if ($_GET['close'] == 'true')
        {
          header('Location: '.$return_to);
          exit();
        }
      }
    }
    break;

  case 'change_catalog':
    break;

  case 'save_project':
    //translate the given date stamp to unix time
    $start_date = date_to_unixtime($_GET['start_date']);
    $end_date = date_to_unixtime($_GET['end_date']);

    $name = smart_addslashes(trim($_GET['name']));
    $task_template_id = $_GET['task_template_id'];
    if ($project_id > 0)
    {
      if ($name == '')
      {
        $feedback = '<p class="Error">'.$error_missing_field.'</p>';
      }else
      {
        $existing_project = $projects->get_project_by_name($name);

        if($existing_project && $existing_project['id'] != $project_id)
        {
          $feedback = '<p class="Error">'.$pm_project_exists.'</p>';
        }elseif(!$projects->update_project($_GET['project_id'], $name,
              smart_addslashes($_GET['description']),
              $_GET['contact_id'],
              smart_addslashes($_GET['comments']),
              $start_date,
              $end_date,
              $_GET['responsible_user_id'],
              $_GET['probability'],
              $_GET['fee_id'],
              $_GET['budget']))
        {
          $feedback = '<p class="Error">'.$strSaveError.'</p>';
        }else
        {
          if ($_GET['close'] == 'true')
          {
            header('Location: '.$return_to);
            exit();
          }
        }
      }
    }else
    {
      if ($name == '' || $task_template_id == 0)
      {
        $feedback = '<p class="Error">'.$error_missing_field.'</p>';
      }elseif($projects->get_project_by_name($name))
      {
        $feedback = '<p class="Error">'.$pm_project_exists.'</p>';
      }else
      {
        $acl_read = $GO_SECURITY->get_new_acl('Project read: '.$name);
        $acl_write = $GO_SECURITY->get_new_acl('Project write: '.$name);
        if ($acl_read > 0 && $acl_write > 0)
        {
          if ($GO_SECURITY->add_user_to_acl($GO_SECURITY->user_id, $acl_write))
          {
            if (!$project_id = $projects->add_project($GO_SECURITY->user_id,
                  $name,
                  smart_addslashes($_GET['description']),
                  $_GET['contact_id'],
                  smart_addslashes($_GET['comments']),
                  $start_date, $end_date, STATUS_BEGIN,
                  $_GET['responsible_user_id'],
                  $_GET['probability'], $_GET['fee_id'],
                  $_GET['budget'], $acl_read, $acl_write,
                  $_GET['task_template_id'],
                  $_GET['catalog']))
            {
              $GO_SECURITY->delete_acl($acl_read);
              $GO_SECURITY->delete_acl($acl_write);
              $feedback = '<p class="Error">'.$strSaveError.'</p>';
            }else
            {
              $is_brandnew_project = true;
              $task = 'write_permissions';
              $active_tab = 1;
            }
          }else
          {
            $GO_SECURITY->delete_acl($acl_read);
            $GO_SECURITY->delete_acl($acl_write);
            $feedback = '<p class="Error">'.$strSaveError.'</p>';
          }
        }else
        {
          $feedback ='<p class="Error">'.$strAclError.'</p>';
        }
      }
    }
    break;

  case 'save_task_list':
    $is_new_project = false;
    $project = $projects->get_project($project_id);
    $responsible_user_id = $project['res_user_id'];
    $user = $GO_USERS->get_user($responsible_user_id);
    $project_folder = $GO_CONFIG->file_storage_path . 'projects/';
    if (!file_exists($project_folder) && !mkdir($project_folder)) {
      error_log("GO: Create $project_folder error");
      unset($project_folder);
    }
    else {
      $project_folder = $project_folder . "$project_id";
      if (!file_exists($project_folder)) {
        if (!mkdir($project_folder)) {
          error_log("GO: Create $project_folder error");
          unset($project_folder);
        }
        else {
          $is_new_project = true;
        }
      }
    }
    $tid = 1;
    $max_task_id = $_REQUEST["max_task_id"];
    while ($tid <= $max_task_id) {
      $tpid = $_REQUEST["task_$tid"];
      if (!isset($tpid)) {
        $tid = $tid + 1;
        continue;
      }

      $tid_time = $_REQUEST["task_time_$tid"];
      if (!isset($tid_time) || $tid_time == 0)
        $tid_time = '';
      else
        $tid_time = ', task_time='.$tid_time;

      $task_folder = "$project_folder/$tid";
      if ($is_new_project) {
        if (!mkdir($task_folder))
          error_log("GO: Create $task_folder error");
      }

      $projects->query("SELECT * FROM task ".
                       "WHERE task_id=$tid AND task_project_id=$project_id");
      $projects->next_record();
      $old_person_id = $projects->f('task_person_id');

      $projects->query("UPDATE task SET task_person_id=$tpid $tid_time ".
                       "WHERE task_id=$tid AND task_project_id=$project_id");

      if ($tpid != $old_person_id && $tpid != $responsible_user_id) {
        notify_relevant_members($project_id,$tid,$tpid,true);
        if ($old_person_id != $responsible_user_id)
          notify_relevant_members($project_id,$tid,$old_person_id,false);
      }
      // Write
      $acl_write_pfolder = $GO_SECURITY->get_acl_id("write: $task_folder");
      if (!$acl_write_pfolder)
        $acl_write_pfolder = $GO_SECURITY->get_new_acl("write: $task_folder");
      else
        $GO_SECURITY->clear_acl($acl_write_pfolder);
      $GO_SECURITY->add_user_to_acl($tpid, $acl_write_pfolder);

      // Read
      $acl_read_pfolder = $GO_SECURITY->get_acl_id("read: $task_folder");
      if (!$acl_read_pfolder)
        $acl_read_pfolder = $GO_SECURITY->get_new_acl("read: $task_folder");
      else
        $GO_SECURITY->clear_acl($acl_read_pfolder);
      $GO_SECURITY->add_user_to_acl($tpid, $acl_read_pfolder);

      $tid = $tid + 1;
    }
    break;

  case 'save_task_approve':
    $tid = $_REQUEST['task_id'];
    $tapp = $_REQUEST['task_approved'] == "on" ? true : false;
    $tasks = load_project_task($projects, $project_id);
    $t = $tasks[$tid];

    if ($t->approved != $tapp) $t->set_approve($tapp);
    $active_tab = TASK_LIST_TAB;
    break;

  case 'save_task_status':
    $tid = $_REQUEST['task_id'];
    $tstat = $_REQUEST['task_status'];
    $tcomm = trim($_REQUEST['task_comments']);
    $tcuser = $_REQUEST['task_cuser'];
    $tdays = $_REQUEST['task_time'];

    $tasks = load_project_task($projects, $project_id);
    $t = $tasks[$tid];

    if (isset($tcomm) && $tcomm != '') {
      $new_comments = "\xFF" . strftime("%d/%m/%y %H:%M", time()) .
                      "|" . $tcuser . "|" . $tcomm;
      $t->add_comments($new_comments);
    }

    if (isset($tstat)) $t->set_status($tstat);
    if (isset($tdays)) $t->set_days($tdays);

    $active_tab = TASK_LIST_TAB;
    break;

  case 'stop_timer':
    $timer = $projects->get_timer($GO_SECURITY->user_id);
    $timer_start_time = $timer['start_time']+($_SESSION['GO_SESSION']['timezone']*3600);
    $timer_end_time = get_time();

    $projects->stop_timer($GO_SECURITY->user_id);

    $_COOKIE['registration_method_cookie'] = 'endtime';
    SetCookie("registration_method_cookie",'endtime',time()+3600*24*365,"/","",0);

    $active_tab = 1;
    break;

  case 'book':
    $active_tab = 1;
    break;

  case 'show_task_list':
  case 'show_task_gantt':
  case 'show_task_status':
  case 'show_task_doc':
    $active_tab = TASK_LIST_TAB;
    break;

  case 'drop_project':
    $projects->query("UPDATE pmProjects SET status=".STATUS_DROP.",probability=0".
                     " WHERE id=$project_id");
    $active_tab = 0;
    break;

  case 'recover_project':
    $projects->query("SELECT max(task_level) AS level FROM task ".
                     "WHERE task_project_id=$project_id AND task_approved=1");
    $pstatus = STATUS_OFFER;
    if ($projects->num_rows() > 0 && $projects->next_record())
      $pstatus = $projects->f('level');
    if (!isset($pstatus)) $pstatus = STATUS_OFFER;
    $projects->query("UPDATE pmProjects SET status=$pstatus,probability=100 ".
                     "WHERE id=$project_id");
    $active_tab = 0;
    break;
}

if ($project_id > 0)
{
  $project = $projects->get_project($project_id);
  $project_name = ($project['description'] == '') ? $project['name'] : $project['name'].' ('.$project['description'].')';

  $tabtable = new tabtable('project_tab', $project_name, '700', '400', '145', '', true);
  $tabtable->add_tab('properties', $strProperties);

  $write_permissions = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $project['acl_write']);
  $read_permissions = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $project['acl_read']);

  if (!$write_permissions && !$read_permissions)
  {
    header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
    exit();
  }

  /*
  if ($write_permissions)
  {
    $tabtable->add_tab('book', $pm_enter_data);
  }
  */

  $tabtable->add_tab('write_permissions', $strWriteRights);
  $tabtable->add_tab('task', $pm_task);
  //$tabtable->add_tab('load', $pm_load);

  $tabtable->add_tab('read_permissions', $strReadRights);

  if ($notes_module = $GO_MODULES->get_module('notes'))
  {
    if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_read']) ||
        $GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_write']))
    {
      $tabtable->add_tab('notes', $lang_modules['notes']);
    }
  }
  $is_owner = ($project['user_id'] == $GO_SECURITY->user_id) ? true : false;
}else
{
  $tabtable = new tabtable('project_tab', $pm_new_project, '600', '400', '145', '', true);
  $project = false;
}

if ($project && $task != 'save_project')
{
  $name = $project['name'];
  $contact_id = $project['contact_id'];
  $comments = $project['comments'];

  $start_date = date($_SESSION['GO_SESSION']['date_format'], $project['start_date']);
  $end_date = date($_SESSION['GO_SESSION']['date_format'], $project['end_date']);
  $status = $project['status'];
  $responsible_user_id = $project['res_user_id'];
  $probability = $project['probability'];
  $fee_id = $project['fee_id'];
  $budget = $project['budget'];
  $description = $project['description'];

  if (isset($active_tab))
  {
    $tabtable->set_active_tab($active_tab);
  }
}else
{
  $name = isset($_GET['name']) ? smartstrip($_GET['name']) : '';
  $contact_id = isset($_GET['contact_id']) ? $_GET['contact_id'] : '0';
  $comments = isset($_GET['comments']) ? smartstrip($_GET['comments']) : '';
  $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : date($_SESSION['GO_SESSION']['date_format'], get_time());
  $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : date($_SESSION['GO_SESSION']['date_format'], get_time());
  $status = isset($_GET['status']) ? $_GET['status'] : '-3';
  $responsible_user_id = isset($_GET['responsible_user_id']) ? $_GET['responsible_user_id'] : $GO_SECURITY->user_id;
  $fee_id = isset($_GET['fee_id']) ? $_GET['fee_id'] : 0;
  $probability = isset($_GET['probability']) ? $_GET['probability'] : 0;
  $budget = isset($_GET['budget']) ? $_GET['budget'] : 0;
  $description = isset($_GET['description']) ? smartstrip($_GET['description']) : '';
}

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();

$page_title = $lang_modules['projects'];
require($GO_THEME->theme_path."header.inc");
echo '<form method="get" action="'.$_SERVER['PHP_SELF'].'" name="projects_form">';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="project_id" value="'.$project_id.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';

$tabtable->print_head();
switch($tabtable->get_active_tab_id())
{
  case 'read_permissions':
    print_acl($project['acl_read'].'&project_acl=1');
    echo '<br />';
    echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;

  case 'write_permissions':
    print_acl($project['acl_write'].'&project_acl=1');
    echo '<br />';
    echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
    if (!isset($is_brandnew_project) || !$is_brandnew_project)
      $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    else
      $button = new button($cmdNext, "javascript:change_tab('".TASK_LIST_TAB."');");
    break;

  case 'book':
    require('book.inc');
    break;

  case 'task':
    require('task.inc');
    break;

  case 'load':
    $fixed_project_id = $project_id;
    require('load.inc');
    break;

  case 'notes':
    $contact_id=0;
    echo '<input type="hidden" name="sort_cookie_prefix" value="no_" />';
    require($GO_LANGUAGE->get_language_file('notes'));
    require_once($notes_module['class_path'].'notes.class.inc');
    $notes = new notes();
    $notes_module_url = $notes_module['url'];
    $link_back .= '&active_tab=3';
    require($notes_module['path'].'notes.inc');
    echo '<br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."'");
    break;

  default:
    if (isset($feedback)) echo $feedback;
    ?>

      <table border="0" cellspacing="0" cellpadding="4">
      <tr>
      <td><?php echo $strName; ?>:</td>
      <td>
      <?php
      if ($project_id < 1 || $is_owner)
      {
        echo '<input type="text" class="textbox" style="width: 250px;" name="name" value="'.htmlspecialchars($name).'" maxlength="50" />';
      }else
      {
        echo htmlspecialchars($project['name']);
      }
      $db = new db();
      if ($project_id <= 0) {
        $db->query('SELECT DISTINCT pmCatalog.* FROM pmCatalog,task_templates WHERE pmCatalog.id=task_templates.cat_id');
        $catalog = new dropbox();
        if (isset($cat_id))
	  $cat_id = $_REQUEST['catalog'];
        while ($db->next_record()) {
          $catalog->add_value($db->f('id'), $db->f('name'));
	  if (!isset($cat_id)) $cat_id = $db->f('id');
	}
        $db->query("SELECT * FROM task_templates WHERE cat_id=$cat_id ORDER BY id");
        $task_templates = new dropbox();
        $task_templates->add_value(0, ' ');
        while ($db->next_record())
          $task_templates->add_value($db->f('id'), $db->f('name'));
        echo "<tr><td>$pm_category:</td><td>";
        $catalog->print_dropbox('catalog', $cat_id, 'onchange="javascript:_change_catalog()"');
        $task_templates->print_dropbox('task_template_id', 0);
        echo '</td></tr>';
      }
      else {
        $db->query('SELECT * FROM pmCatalog WHERE id='.$project['cat_id']);
        $db->next_record();
        $catalog = $db->f('name');
        $db->query('SELECT * FROM task_templates '.
                   'WHERE id='.$project['task_template_id']);
        $db->next_record();
        echo "<tr><td>$pm_category:</td><td><b>$catalog&nbsp;&nbsp;-&nbsp;&nbsp;".$db->f('name').'</b></td></tr>';
      }
      ?>
        </td>
        <tr>
        <tr>
        <td><?php echo $pm_description; ?>:</td>
        <td>
        <?php
        if ($project_id < 1 || $is_owner)
        {
          echo '<input type="text" class="textbox" style="width: 250px;" name="description" value="'.htmlspecialchars($description).'" maxlength="50" />';
        }else
        {
          echo htmlspecialchars($project['description']);
        }
        ?>
          </td>
          <tr>
          <?php
          $progress = 0;
          $pm_status_values = array();
          if ($project_id > 0) {
            $db->query('SELECT count(task_id) as total_task '.
                       'FROM task WHERE task.task_project_id='.$project_id);
            $db->next_record();
            $total_task = $db->f('total_task');
            $db->query('SELECT count(task_approved) as total_task_approved '.
                       'FROM task WHERE task_project_id='.$project_id.' AND task_approved=1');
            $db->next_record();
            $total_task_approved = $db->f('total_task_approved');
            $progress = round($total_task_approved * 100 / $total_task);
            $db->query('SELECT * FROM pmStatus WHERE cat_id='.$projects->f('cat_id'));
            while ($db->next_record())
              $pm_status_values[$db->f('value')] = $db->f('name');
          }
          // Add/modify 3 special status
          $pm_status_values[STATUS_DROP] = $pm_status_drop;
          $pm_status_values[STATUS_DONE] = $pm_status_done;
          $pm_status_values[STATUS_BEGIN] = $pm_status_begin;
          if ($project_id < 1 || $project['user_id'] == $GO_SECURITY->user_id)
          {
            if($ab_module)
            {
              $select = new select('contact', 'projects_form', 'contact_id', $contact_id);
              echo '<tr><td>';
              $select->print_link($pm_client);
              echo ':</td><td>';
              $select->print_field();
              echo '</td></tr>';
            }else
            {
              echo '<input type="hidden" value="0" name="contact_id" />';
              echo $pm_no_contact;
            }
            $select = new select('user', 'projects_form', 'responsible_user_id', $responsible_user_id);
            echo '<tr><td>';
            $select->print_link($pm_employee);
            echo ':</td><td>';
            $select->print_field();
            echo '</td></tr>';

            echo '<tr><td>'.$pm_start_date.':</td><td>';
            $datepicker->print_date_picker('start_date', $_SESSION['GO_SESSION']['date_format'], $start_date, '', '', 'onchange="javascript:is_valid_date(\'end_date\', this.value)"');
            echo '</td></tr>';
            echo '<tr><td>'.$pm_end_date.':</td><td>';
            $datepicker->print_date_picker('end_date', $_SESSION['GO_SESSION']['date_format'], $end_date, '', '', 'onchange="javascript:is_valid_date(\'start_date\', this.value)"');
            echo '</td></tr>';
            if ($project_id > 0) {
              $pstate = $projects->f('status');
              echo '<tr><td>'.$pm_status.':</td><td><b>'.$pm_status_values[$pstate].'</b></td></tr>';
              echo '<tr><td>'.$pm_progress.':</td><td>'.$progress.'%</td></tr>';
            }

            echo '<tr><td>'.$pm_budget.':</td><td><input type="text" class="textbox" size="10" name="budget" value="'.$budget.'" maxlength="50" style="text-align: right;" /> '.$_SESSION['GO_SESSION']['currency'].'</td></tr>';
          }else
          {
            echo '<tr><td>'.$pm_client.':</td><td>';
            if($contact_id > 0)
            {
              echo show_contact($contact_id);
            }else
            {
              echo $pm_no_contact;
            }
            echo '</td></tr>';

            echo '<tr><td>'.$pm_start_date.':</td><td>';
            echo $start_date;
            echo '</td></tr>';
            echo '<tr><td>'.$pm_end_date.':</td><td>';
            echo $end_date;
            echo '</td></tr>';
            echo '<tr><td>'.$pm_status.'</td><td><b>';
            $status = $pm_status_values[$projects->f('status')];
            echo $status.'</b></td></tr>';
            echo '<tr><td>'.$pm_progress.':</td><td>'.$progress.'%</td></tr>';
            echo '<tr><td>'.$pm_budget.':</td><td>'.$budget.' '.$_SESSION['GO_SESSION']['currency'].'</td></tr>';
          }

          $fee_count = $projects->get_fees();
          if ($fee_count > 0)
          {
            echo '<tr><td>'.$pm_fee.'</td><td>';

           
            $dropbox=new dropbox();

            while ($projects->next_record())
            {
              $dropbox->add_value($projects->f('id'),$projects->f('name').' ('.htmlentities($_SESSION['GO_SESSION']['currency']).'&nbsp;'.number_format($projects->f('value'), 2, $_SESSION['GO_SESSION']['decimal_seperator'],$_SESSION['GO_SESSION']['thousands_seperator']).'&nbsp;/&nbsp;'.$projects->f('time').'&nbsp;'.$pm_mins.')');
            }
            $disabled = ($project_id < 1 || $project['user_id'] == $GO_SECURITY->user_id) ? '' : 'disabled';
            $dropbox->print_dropbox('fee_id', $fee_id, $disabled);
          }else
          {
            echo '<input type="hidden" name="fee_id" value="0" />';
          }

          if ($project_id > 0)
          {
            echo '<tr><td>'.$strOwner.':</td><td>'.show_profile($project['user_id']).'</td></tr>';
            echo '<tr><td>'.$strCreatedAt.':</td><td>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $project['ctime']+($_SESSION['GO_SESSION']['timezone']*3600)).'</td><tr>';
            echo '<tr><td>'.$strModifiedAt.':</td><td>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $project['mtime']+($_SESSION['GO_SESSION']['timezone']*3600)).'</td><tr>';
          }
          ?>
                  <tr>
                  <td valign="top"><?php echo $strComments; ?>:</td>
                <td>
                <?php
                if ($project_id < 1 || $project['user_id'] == $GO_SECURITY->user_id)
                {
                  $disabled = (isset($pstate) && $pstate == STATUS_DROP) ? 'disabled' : '';
                  echo '<textarea name="comments" cols="50" rows="4" class="textbox" '.$disabled.'>'.$comments.'</textarea>';
                }else
                {
                  echo text_to_html($comments);
                }
                ?>
                  </td>
                  </tr>
                  <tr>
                  <td colspan="2">
                  <?php
                  $pstat = $projects->f('status');
                  if (($project_id < 1 || $is_owner) && (!isset($pstat) || $pstat != STATUS_DROP))
                  {
                    $button = new button($cmdOk, "javascript:_save('save_project', 'true');");
                    echo '&nbsp;&nbsp;';
                    $button = new button($cmdApply, "javascript:_save('save_project', 'false')");
                    echo '&nbsp;&nbsp;';
                  }
                $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
                ?>
                  </td>
                  <td colspan="3" align="right">
                  <?php
                  if (!isset($is_owner) || !$is_owner)
                    echo "&nbsp;";
                  elseif ($projects->f('status') != STATUS_DROP)
                    $button = new button($cmdDrop, 'javascript:_drop_project()');
                  else
                    $button = new button($cmdRecover, 'javascript:_recover_project()');
                  ?>
                  </td>
                  </tr>
                  </table>

                  <?php
                  break;
}
$tabtable->print_foot();
echo '</form>';
?>
<script type="text/javascript">
function _change_catalog()
{
  document.projects_form.task.value = 'change_catalog';
  document.projects_form.close.value = true;
  document.projects_form.submit();
}

function _drop_project()
{
  if (confirm("<?php echo $pm_ConfirmDrop; ?>")) {
    document.projects_form.task.value = 'drop_project';
    document.projects_form.close.value = true;
    document.projects_form.submit();
  }
}

function _recover_project()
{
  if (confirm("<?php echo $pm_ConfirmRecover; ?>")) {
    document.projects_form.task.value = 'recover_project';
    document.projects_form.close.value = true;
    document.projects_form.submit();
  }
}

function _save(task, close)
{
  document.projects_form.task.value = task;
  document.projects_form.close.value = close;
  document.projects_form.submit();
}
function remove_client()
{
  document.projects_form.contact_id.value = 0;
  document.projects_form.contact_name.value = '';
  document.projects_form.contact_name_text.value = '';
}

function remove_user()
{
  document.projects_form.responsible_user_id.value = 0;
  document.projects_form.user_name.value = '';
  document.projects_form.user_name_text.value = '';
}

function is_valid_date(date_field_to_validate, date)
{
  switch (date_field_to_validate)
  {
    case 'start_date':
      if(document.projects_form.start_date.value > date)
          {
            document.projects_form.start_date.value = date;
          }
        break;
    case 'end_date':
      if(document.projects_form.end_date.value < date)
          {
            document.projects_form.end_date.value = date;
          }
        break;
  }
}

</script>
<?php
require($GO_THEME->theme_path."footer.inc");
function notify_relevant_members($project_id,$task_id,$person_id,$assigned = true)
{
      global $GO_CONFIG;
      
      $db = new db();
      $sql = "SELECT users.* FROM".
        " users LEFT JOIN users_groups ON (users.id = users_groups.user_id)".
        " WHERE users_groups.group_id='".$GO_CONFIG->group_root."'";
      if ($db->query($sql) && $db->num_rows() && $db->next_record()) {
        require_once($GO_CONFIG->class_path."phpmailer/class.phpmailer.php");
        require_once($GO_CONFIG->class_path."phpmailer/class.smtp.php");
        $mail = new PHPMailer();
        $mail->PluginDir = $GO_CONFIG->class_path.'phpmailer/';
        $mail->SetLanguage($php_mailer_lang, $GO_CONFIG->class_path.'phpmailer/language/');

        switch($GO_CONFIG->mailer) {
          case 'smtp':
            $mail->Host = $GO_CONFIG->smtp_server;
            $mail->Port = $GO_CONFIG->smtp_port;
            $mail->IsSMTP();			  
            break;			
          case 'qmail':
            $mail->IsQmail();
            break;			
          case 'sendmail':
            $mail->IsSendmail();
            break;
          case 'mail':
            $mail->IsMail();
            break;
        }

        $mail->Sender   = $db->f('email');
        $mail->From     = $db->f('email');
        $mail->FromName = $GO_CONFIG->title;
        $mail->AddReplyTo($db->f('email'),$GO_CONFIG->title);
        $mail->WordWrap = 50;
        $mail->IsHTML(true);

        $db->query("SELECT * FROM task WHERE task_id=".$task_id." AND task_project_id=".$project_id);
        $db->next_record();
        $task_name = $db->f('task_name');
        $task_person_id = $db->f('task_person_id');
	$task_duration = $db->f('task_time');

        $db->query('SELECT * '.
          'FROM pmProjects '.
  	  'WHERE id="'.$project_id.'" ');
        $db->next_record();

        $task_url = $GO_CONFIG->full_url.'modules/projects/project.php?task=show_task_status&project_id='.$project_id.'&task_id='.$task_id.'&task_status='.$status;
        $project_url = $GO_CONFIG->full_url.'modules/projects/project.php?project_id='.$project_id;


        global $pm_task_status_values;
        $new_status = $pm_task_status_values[$status];
        $project_name = $db->f('name');
	$project_description = $db->f('description');
        global $subjectTaskAssigneeChanged,$mailTaskAssigneeLeft,$mailTaskAssigneeJoined;
        $mail->Subject = sprintf($subjectTaskAssigneeChanged,$task_name,$project_name);
	if ($assigned)
          $mail_body  = sprintf($mailTaskAssigneeJoined,$project_name,$project_description,$task_name,$task_duration,$task_url);
	else
          $mail_body  = sprintf($mailTaskAssigneeLeft,$project_name,$project_description,$task_name);

        $mail->Body = $mail_body;
        $mail->ClearAllRecipients();
        if ($status == TASK_DONE)
          $db->query('SELECT users.* '.
            'FROM users,pmProjects '.
  	    'WHERE users.id=pmProjects.user_id '.
  	    'AND pmProjects.id="'.$project_id.'"');
        else
          $db->query('SELECT * '.
            'FROM users '.
  	    'WHERE id="'.$task_person_id.'"');
        $db->next_record();
        $mail->AddAddress($db->f('email'));

        // HACK: For some reasons, admin@hptvietnam.com.vn is not accepted by mail.hptvietnam.com.vn :(
        $mail->From = $db->f('email');
        $mail->Sender = $db->f('email');

        //if (!$mail->Send()) echo "Failed: ".$mail->ErrorInfo;
        $mail->Send();
        //$mail->Send();
      }
}
?>
