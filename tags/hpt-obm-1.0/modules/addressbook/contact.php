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
$GO_MODULES->authenticate('addressbook');
require($GO_LANGUAGE->get_language_file('addressbook'));

$custom_fields_plugin = $GO_MODULES->get_plugin('custom_fields');

$page_title=$contact_profile;
require($GO_MODULES->class_path."addressbook.class.inc");
$ab = new addressbook();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? 
$_REQUEST['return_to'] : null;
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? 
$_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$contact_id = isset($_REQUEST['contact_id']) ? $_REQUEST['contact_id'] : '0';

$company_id =isset($_REQUEST['company_id']) ? $_REQUEST['company_id'] : 0;
$addressbook_id = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : 
$ab->get_default_addressbook($GO_SECURITY->user_id);

switch($task)
{
  case 'save':
    $require = 'edit_contact.inc';
    $first_name = smart_addslashes(trim($_POST['first_name']));
    $middle_name = smart_addslashes(trim($_POST['middle_name']));
    $last_name = smart_addslashes(trim($_POST['last_name']));
    $initials = smart_addslashes($_POST["initials"]);
    $title = smart_addslashes($_POST["title"]);
    $birthday = smart_addslashes($_POST["birthday"]);
    $email = smart_addslashes($_POST["email"]);
    $work_phone = smart_addslashes($_POST["work_phone"]);
    $home_phone = smart_addslashes($_POST["home_phone"]);
    $fax = smart_addslashes($_POST["fax"]);
    $work_fax = smart_addslashes($_POST["work_fax"]);
    $cellular = smart_addslashes($_POST["cellular"]);
    $country = smart_addslashes($_POST["country"]);
    $state = smart_addslashes($_POST["state"]);
    $city = smart_addslashes($_POST["city"]);
    $zip = smart_addslashes($_POST["zip"]);
    $address = smart_addslashes($_POST["address"]);
    $department = smart_addslashes($_POST["department"]);
    $function = smart_addslashes($_POST["function"]);
    $comment = smart_addslashes($_POST["comment"]);

    if ($first_name == '' && $last_name == '')
    {
      $feedback = "<p class=\"Error\">".$error_missing_field."</p>";
    }else
    {
      $company_name = isset($_POST['company_name']) ?
	smart_addslashes(trim($_POST['company_name'])) : '';

      if (isset($_POST['company_name']) && $company_name == '')
      {
	$company_id = 0;
      }elseif($company_name != '' &&
	  !$new_company_id = $ab->get_company_id_by_name($company_name, 
	    $addressbook_id))
      {
	$acl_read = $GO_SECURITY->get_new_acl('company read');
	$acl_write = $GO_SECURITY->get_new_acl('company write');

	if ($acl_read > 0 && $acl_write > 0 &&
	    $company_id = $ab->add_company($addressbook_id, 
	      $GO_SECURITY->user_id, $company_name, '','', '',
	      '', '', '', '', '', '', '', '', '','', ''
	      , $acl_read, $acl_write,0))
	{
	  if($addressbook = $ab->get_addressbook($addressbook_id))
	  {
	    $GO_SECURITY->copy_acl($addressbook['acl_read'], $acl_read);
	    $GO_SECURITY->copy_acl($addressbook['acl_write'], $acl_write);
	  }

	}else
	{
	  $GO_SECURITY->delete_acl($acl_read);
	  $GO_SECURITY->delete_acl($acl_write);
	  $feedback = "<p class=\"Error\">".$strSaveError."</p>";
	}

      }elseif(isset($new_company_id) && $new_company_id != $company_id)
      {
	$company_id = $new_company_id;
      }

      //translate the given birthdayto gmt unix time
      $birthday = date_to_db_date($_POST['birthday']);

      $group_id = isset($_POST['group_id']) ? $_POST['group_id'] : '0';
      if ($_POST['contact_id'] > 0)
      {
	if ($ab->update_contact($_POST['contact_id'], $_POST['addressbook_id'],
	      $first_name, $middle_name, $last_name,$initials, $title,
	      $_POST['sex'], $birthday, $email, $work_phone, $home_phone,
	      $fax, $cellular, $country, $state, $city, $zip, $address,
	      $company_id, $work_fax, $department, $function, $comment,
	      $group_id, $_POST['color']))
	{
	  if ($_POST['close'] == 'true')
	  {
	    header('Location: '.$return_to);
	    exit();
	  }
	}else
	{
	  $feedback = "<p class=\"Error\">".$strSaveError."</p>";
	}
      }else
      {
	$acl_read = $GO_SECURITY->get_new_acl('contact read');
	$acl_write = $GO_SECURITY->get_new_acl('contact write');

	if ($acl_read > 0 && $acl_write > 0 &&
	    $contact_id = $ab->add_contact($_POST['source_id'], $GO_SECURITY->user_id,
	      $_POST['addressbook_id'], $first_name, $middle_name, $last_name,
	      $_POST['initials'], $_POST['title'], $_POST['sex'], $birthday,
	      $email, $work_phone, $home_phone,  $fax, $cellular, $country,
	      $state, $city, $zip, $address, $company_id, $work_fax,
	      $department, $function, $comment, $group_id, $_POST['color'],
	      $acl_read, $acl_write))
	{
	  if($addressbook = $ab->get_addressbook($addressbook_id))
	  {
	    $GO_SECURITY->copy_acl($addressbook['acl_read'], $acl_read);
	    $GO_SECURITY->copy_acl($addressbook['acl_write'], $acl_write);
	  }

	  $link_back .= '&contact_id='.$contact_id;

	  if ($_POST['close'] == 'true')
	  {
	    header('Location: '.$return_to);
	    exit();
	  }
	}else
	{
	  $GO_SECURITY->delete_acl($acl_read);
	  $GO_SECURITY->delete_acl($acl_write);
	  $feedback = "<p class=\"Error\">".$strSaveError."</p>";
	}
      }
    }

    break;

  case 'save_custom_fields':
    if (isset($_POST['fields']))
    {
      require_once($custom_fields_plugin['path'].'classes/custom_fields.class.inc');
      $cf = new custom_fields('ab_custom_contact_fields');

      $cf->update_record($contact_id, $_POST['fields'], $_POST['values']);

      if ($_POST['close'] == 'true')
      {
	header('Location: '.$return_to);
	exit();
      }
    }
    break;

  case 'start_timer':
    $active_tab = 1;
    break;

  default:
    $require = 'edit_contact.inc';
    break;
}
if ($contact_id > 0)
{
  $contact = $ab->get_contact($contact_id);
  $write_permission = $GO_SECURITY->has_permission($GO_SECURITY->user_id, $contact["acl_write"]);
  if (!$write_permission  && !$GO_SECURITY->has_permission($GO_SECURITY->user_id, $contact["acl_read"]))
  {
    Header("Location: ".$GO_CONFIG->host."error_docs/403.php");
    exit();
  }
  if (!$write_permission)
  {
    $require = 'show_contact.inc';
  }
  $birthday = $contact['birthday'] > 0 ? db_date_to_date($contact['birthday']) : '';
  $addressbook_id = isset($_POST['addressbook_id']) ? $_POST['addressbook_id'] : $contact['addressbook_id'];
}

if (isset($_REQUEST['user_id']) && $_REQUEST['user_id'] > 0)
{
  $contact = $GO_USERS->get_user($_REQUEST['user_id']);

  if ($ab->user_is_contact($GO_SECURITY->user_id, $_REQUEST['user_id']))
  {
    $feedback = "<p class='Error'>".$contact_exist_warning."</p>";
    $contact['source_id'] = "";
  }else
  {
    $contact['source_id'] = $_REQUEST['user_id'];
  }
  $contact['addressbook_id'] = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : $ab->get_default_addressbook($GO_SECURITY->user_id);
  $addressbook_id = isset($_POST['addressbook_id']) ? $_POST['addressbook_id'] : $contact['addressbook_id'];
  $contact['company_name'] = $contact['company'];
  $contact['company_id'] = $ab->get_company_id_by_name($contact['company'], $contact['addressbook_id']);
  $contact['group_id'] = isset($_REQUEST['group_id']) ? $_REQUEST['group_id'] : '';
  $birthday = $contact['birthday'] > 0 ? db_date_to_date($contact['birthday']) : '';
  $require = 'edit_contact.inc';
}elseif (($contact_id == 0 || $task != '') && $task != 'save_custom_fields')
{
  $require = 'edit_contact.inc';
  $contact['addressbook_id'] = isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : $addressbook_id;
  $contact['first_name'] = isset($_REQUEST['first_name']) ? smartstrip($_REQUEST['first_name']) : '';
  $contact['middle_name'] = isset($_REQUEST['middle_name']) ? smartstrip($_REQUEST['middle_name']) : '';
  $contact['last_name'] = isset($_REQUEST['last_name']) ? smartstrip($_REQUEST['last_name']) : '';
  $contact['initials'] = isset($_REQUEST['initials']) ? smartstrip($_REQUEST['initials']) : '';
  $contact['title'] = isset($_REQUEST['title']) ? smartstrip($_REQUEST['title']) : '';
  $contact['sex'] = isset($_REQUEST['sex']) ? smartstrip($_REQUEST['sex']) : 'M';
  $birthday = isset($_REQUEST['birthday']) ? smartstrip($_REQUEST['birthday']) : '';
  $contact['email'] = isset($_REQUEST['email']) ? smartstrip($_REQUEST['email']) : '';
  $contact['work_phone'] = isset($_REQUEST['work_phone']) ? smartstrip($_REQUEST['work_phone']) : '';
  $contact['home_phone'] = isset($_REQUEST['home_phone']) ? smartstrip($_REQUEST['home_phone']) : '';
  $contact['fax'] = isset($_REQUEST['fax']) ? smartstrip($_REQUEST['fax']) : '';
  $contact['cellular'] = isset($_REQUEST['cellular']) ? smartstrip($_REQUEST['cellular']) : '';
  $contact['country'] = isset($_REQUEST['country']) ? smartstrip($_REQUEST['country']) : '';
  $contact['state'] = isset($_REQUEST['state']) ? smartstrip($_REQUEST['state']) : '';
  $contact['city'] = isset($_REQUEST['city']) ? smartstrip($_REQUEST['city']) : '';
  $contact['zip'] = isset($_REQUEST['zip']) ? smartstrip($_REQUEST['zip']) : '';
  $contact['address'] = isset($_REQUEST['address']) ? smartstrip($_REQUEST['address']) : '';
  $contact['department'] = isset($_REQUEST['department']) ? smartstrip($_REQUEST['department']) : '';
  $contact['function'] = isset($_REQUEST['function']) ? smartstrip($_REQUEST['function']) : '';
  $contact['comment'] = isset($_REQUEST['comment']) ? smartstrip($_REQUEST['comment']) : '';
  $contact['color'] = isset($_REQUEST['color']) ? smartstrip($_REQUEST['color']) : '000000';
  $contact['source_id'] = isset($_REQUEST['source_id']) ? $_REQUEST['source_id'] : '';
  $contact['group_id'] = isset($_REQUEST['group_id']) ? $_REQUEST['group_id'] : '';
  $contact['company_name'] = isset($_REQUEST['company_name']) ? $_REQUEST['company_name'] : '';

  if ($company_id && $company= $ab->get_company($company_id))
  {
    $contact['company_name'] = $company['name'];
    $contact['company_id'] = $company_id ;
  }else
  {
    $contact['company_id'] = isset($contact['company_id']) ? $contact['company_id'] : 0;
  }
}

if($task =='update')
{
  $contact = $GO_USERS->get_user($contact['source_id']);
  $contact["source_id"] = $_POST['source_id'];
  $contact['comment'] = $_POST['comment'];
  $contact['group_id'] = $_POST['group_id'];
  $contact['addressbook_id'] = $_POST['addressbook_id'];
  $contact['company_id']  = $_POST['company_id'];
  $birthday = $contact['birthday'] > 0 ? db_date_to_date($contact['birthday']) : '';
}

$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();
$overlib = new overlib();
$GO_HEADER['head'] .= $overlib->get_header();

require($GO_THEME->theme_path."header.inc");

echo '<form name="add" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" value="'.$contact["source_id"].'" name="source_id" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="contact_id" value="'.$contact_id.'" />';
echo '<table border="0"><tr>';

$title = $contact_id > 0 ? $ab_contact : $contacts_add;

$tabtable= new tabtable('contact_table', $title, '100%', '400', '120', '', true, 'left', 'top', 'add', 'vertical');

if ($contact_id > 0)
{
  $tabtable->add_tab('profile', $contact_profile);

  if ($custom_fields_plugin)
  {
    require_once($custom_fields_plugin['path'].'classes/custom_fields.class.inc');
    $cf = new custom_fields('ab_custom_contact_fields');

    if ($cf->get_catagories($GO_SECURITY->user_id) > 0)
    {
      while($cf->next_record())
      {
	$tabtable->add_tab($cf->f('id'), $cf->f('name'));
      }
    }
  }

  $projects_module = $GO_MODULES->get_module('projects');
  if ($projects_module)
  {
    if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $projects_module['acl_read']) ||
	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $projects_module['acl_write']))
    {
      $tabtable->add_tab('projects', $lang_modules['projects']);
    }
  }
  $notes_module = $GO_MODULES->get_module('notes');
  if ($notes_module)
  {
    if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_write']))
    {
      $tabtable->add_tab('notes', $lang_modules['notes']);

      echo '<td  class="ModuleIcons" nowrap>';
      echo '<a href="'.$notes_module['url'].'note.php?contact_id='.$contact_id.'&return_to='.rawurlencode($link_back).'"><img src="'.$GO_THEME->images['ab_notes'].'" border="0" height="32" width="32" /><br />'.$ab_new_note.'</td>';
    }
  }

  if ($contact['email'] != '')
  {
    echo '<td class="ModuleIcons" nowrap>';
    echo mail_to($contact['email'], '<img src="'.$GO_THEME->images['ab_email'].'" border="0" height="32" width="32" /><br />'.$ab_send_message, 'small', true, $contact_id);
    echo '</td>';
  }
  $calendar_module = $GO_MODULES->get_module('calendar');
  if ($calendar_module)
  {
    if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar_module['acl_write']))
    {
      echo '<td class="ModuleIcons" nowrap>';
      echo '<a href="'.$calendar_module['url'].'event.php?contact_id='.$contact_id.'&return_to='.
	rawurlencode($link_back).'"><img src="'.$GO_THEME->images['cal_compose'].
	'" border="0" height="32" width="32" /><br />'.$ab_new_event.'</td>';
      echo '</td>';

      $tabtable->add_tab('calendar', $ab_events);
      $tabtable->add_tab('todos', $ab_todos);
    }
  }
  $templates_plugin = $GO_MODULES->get_plugin('templates');
  if ($templates_plugin)
  {
    require_once($templates_plugin['path'].'classes/templates.class.inc');

    $tp = new templates();
    if ($tp->has_oo_templates($GO_SECURITY->user_id))
    {
      echo '<td class="ModuleIcons" nowrap>';
      echo '<a target="_blank" href="'.$GO_MODULES->url.
	'templates/download_oo_template.php?contact_id='.$contact_id.
	'"><img src="'.$GO_THEME->images['new_letter'].
	'" border="0" height="32" width="32" /><br />'.$ab_oo_doc.'</td>';
      echo '</td>';
    }
  }
  $tabtable->add_tab('read_permissions', $strReadRights);
  $tabtable->add_tab('write_permissions', $strWriteRights);
}
echo '</tr></table>';
$active_tab = isset($_REQUEST['active_tab']) ? $_REQUEST['active_tab'] : null;
if (isset($active_tab))
{
  $tabtable->set_active_tab($active_tab);
}

$link_back = cleanup_url($link_back.'&'.$tabtable->get_link_back());
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

$tabtable->print_head();

if ($tabtable->get_active_tab_id() > 0)
{
  $catagory_id = $tabtable->get_active_tab_id();
  $active_tab_id = 'custom_fields';
}else
{
  $active_tab_id =$tabtable->get_active_tab_id();
}

switch($active_tab_id)
{
  case 'read_permissions':
    print_acl($contact['acl_read']);
    echo '<br />';
    echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;

  case 'write_permissions':
    print_acl($contact['acl_write']);
    echo '<br />';
    echo '<br />&nbsp;&nbsp;&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;

  case 'custom_fields':
    require('custom_fields/custom_fields.inc');
    break;

  case 'projects':
    echo '<input type="hidden" name="sort_cookie_prefix" value="pm_" />';
    require($GO_LANGUAGE->get_language_file('projects'));
    require_once($projects_module['class_path'].'projects.class.inc');
    $projects = new projects();

    $projects_module_url = $projects_module['url'];
    require($projects_module['path'].'projects.inc');
    echo '<br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."'");
    break;

  case 'notes':
    echo '<input type="hidden" name="sort_cookie_prefix" value="no_" />';
    require($GO_LANGUAGE->get_language_file('notes'));
    require_once($notes_module['class_path'].'notes.class.inc');
    $notes = new notes();
    $notes_module_url = $notes_module['url'];
    require($notes_module['path'].'notes.inc');
    echo '<br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."'");
    break;

  case 'calendar':
    echo '<script type="text/javascript">
      function goto_date(day, month, year)
      {
	document.forms[0].day.value = day;
	document.forms[0].month.value = month;
	document.forms[0].year.value = year;
	document.forms[0].submit();
      }
    </script>';
    echo '<input type="hidden" name="sort_cookie_prefix" value="cal_" />';

    require($GO_LANGUAGE->get_language_file('calendar'));
    require_once($calendar_module['class_path'].'calendar.class.inc');
    $cal = new calendar();
    $calendar_module_url = $calendar_module['url'];

    $print = false;
    $calendar_id = 0;

    //get the local times
    $local_time = get_time();
    $year = isset($_REQUEST['year']) ? $_REQUEST['year'] : date("Y", $local_time);
    $month = isset($_REQUEST['month']) ? $_REQUEST['month'] : date("m", $local_time);
    $day = isset($_REQUEST['day']) ? $_REQUEST['day'] : date("j", $local_time);
    $hour = isset($_REQUEST['hour']) ? $_REQUEST['hour'] : date("H", $local_time);
    $min = isset($_REQUEST['min']) ? $_REQUEST['min'] : date("i", $local_time);
    $local_browse_time = mktime($hour, $min, 0, $month, $day, $year);

    //recalculate date
    $year = date("Y", $local_browse_time);
    $month = date("m", $local_browse_time);
    $day = date("j", $local_browse_time);
    $hour = date("H", $local_browse_time);
    $min = date("i", $local_browse_time);

    //get the current date properties
    $current_year = date("Y", $local_time);
    $current_month = date("m", $local_time);
    $current_day = date("j", $local_time);
    $current_hour = date("H", $local_time);
    $current_min = date("i", $local_time);
    $current_date = date(DB_DATE_FORMAT, $local_time);

    echo '<input type="hidden" name="day" value="'.$day.'" />';
    echo '<input type="hidden" name="month" value="'.$month.'" />';
    echo '<input type="hidden" name="year" value="'.$year.'" />';

    require($calendar_module['path'].'list_view.inc');
    echo '<br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."'");
    break;

  case 'todos':
    require_once($calendar_module['class_path'].'todos.class.inc');
    $todos = new todos();
    $calendar_module_url = $calendar_module['url'];
    echo '<table border= "0"><tr><td>';
    require($calendar_module['path'].'todos.inc');
    echo '</td></tr></table>';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."'");
    break;

  default:
    require($require);
    break;
}
$tabtable->print_foot();

echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>

