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

if($task ==  'start_timer')
{
  $projects->start_timer($GO_SECURITY->user_id, $_REQUEST['project_id']);
}

if (isset($_REQUEST['delete_project_id']) && $_REQUEST['delete_project_id'] > 0)
{
  $pid = $_REQUEST['delete_project_id'];
  $project = $projects->get_project($pid);
  if ($GO_SECURITY->user_id == $project['res_user_id'])
  {
    if($projects->delete_project($pid))
    {
      $GO_SECURITY->delete_acl($project['acl_write']);
      $GO_SECURITY->delete_acl($project['acl_read']);
      $projects->query("DELETE FROM task WHERE task_project_id=$pid");
      $project_folder = $GO_CONFIG->file_storage. 'projects/' . $pid;
      require_once($GO_CONFIG->class_path.'filesystem.class.inc');
      $fs = new filesystem();
      $fs->delete($project_folder);
    }
  }else
  {
    $feedback = '<p class="Error">'.$strAccessDenied.'</p>';
  }
}

$pm_settings = $projects->get_settings($GO_SECURITY->user_id);

$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;

//check if we are included from the addressbook and if we have a contact related to us
$projects_module_url = isset($projects_module_url) ? $projects_module_url : $GO_MODULES->url;
$contact_id = isset($contact_id) ? $contact_id : 0;
echo '<a href="'.$projects_module_url.'project.php?contact_id='.$contact_id.'&return_to='.rawurlencode($link_back).'" class="normal">'.$pm_new_project.'</a><br /><br />';

if ($timer = $projects->get_timer($GO_SECURITY->user_id))
{
  echo '<table border="0" class="alert"><tr><td><img src="'.$GO_THEME->images['alert'].'" border="0" /></td>'.
    '<td><a class="normal" href="'.$projects_module_url.'project.php?task=stop_timer&project_id='.$timer['project_id'].'&return_to='.rawurlencode($link_back).'&contact_id='.$contact_id.'">'.$pm_clocked_in.'</a></td></tr></table>';
}
if(isset($_REQUEST['show_projects']) && $_REQUEST['show_projects'] != $pm_settings['show_projects'])
{
  $projects->set_show_projects($GO_SECURITY->user_id, $_REQUEST['show_projects']);
  $pm_settings['show_projects'] = $_REQUEST['show_projects'];  
}

if(isset($_REQUEST['show_catalog']) && $_REQUEST['show_catalog'] != $pm_settings['show_catalog'])
{
  $projects->set_show_catalog($GO_SECURITY->user_id, $_REQUEST['show_catalog']);
  $pm_settings['show_catalog'] = $_REQUEST['show_catalog'];  
}

if(isset($_REQUEST['new_pr_sort_order']) && $_REQUEST['new_pr_sort_order'] != $pm_settings['sort_order'])
{
  $projects->set_sorting($GO_SECURITY->user_id, 
  $_REQUEST['new_pr_sort_field'], $_REQUEST['new_pr_sort_order']);
  $pm_settings['sort_order'] = $_REQUEST['new_pr_sort_order'];
  $pm_settings['sort_field'] = $_REQUEST['new_pr_sort_field'];  
}
if ($pm_settings['sort_order'] == "DESC")
{
  $sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $new_pr_sort_order = "ASC";
}else
{
  $sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $new_pr_sort_order = "DESC";
}

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<tr><td colspan="5"><table border="0"><tr><td colspan="2">'.$pm_filter.':</td><td colspan="3">';

echo '<input type="hidden" name="first" value="'.$first.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="post_action" value="projects" />';
echo '<input type="hidden" name="new_pr_sort_field" value="'.$pm_settings['sort_field'].'" />';
echo '<input type="hidden" name="new_pr_sort_order" value="'.$pm_settings['sort_order'].'" />';
echo '<input type="hidden" name="delete_project_id" value="0" />';

$show_catalog = $_REQUEST['show_catalog'];
if (!isset($show_catalog)) $show_catalog = 0;

$db = new db();
$db->query('SELECT * FROM pmCatalog');
$catalog = new dropbox();
$catalog->add_value(0, $cmdShowAll);
while ($db->next_record())
  $catalog->add_value($db->f('id'), $db->f('name'));
$catalog->print_dropbox('show_catalog', $show_catalog, 'onchange="javascript:document.forms[0].submit()"');

$dropbox = new dropbox();
$dropbox->add_value(STATUS_DROP - 2, $cmdShowAll);
$dropbox->add_value(STATUS_RUNNING, $pm_status_running);
$dropbox->add_value(STATUS_BEGIN, $pm_status_begin);
if ($show_catalog) {
  $db->query('SELECT * FROM pmStatus WHERE cat_id='.$show_catalog);
  while ($db->next_record())
    $dropbox->add_value($db->f('value'), $db->f('name'));
}
$dropbox->add_value(STATUS_DONE, $pm_status_done);
$dropbox->add_value(STATUS_DROP, $pm_status_drop);

$dropbox->print_dropbox('show_projects', $pm_settings['show_projects'], 'onchange="javascript:document.forms[0].submit()"');

$count = $projects->get_authorized_projects($GO_SECURITY->user_id, $pm_settings['sort_field'], $pm_settings['sort_order'], $first, $max_rows, $pm_settings['show_projects'], $pm_settings['show_catalog'], $contact_id);
echo '</td></tr></table></td><td colspan="99" class="small" align="right">'.$count.' '.$pm_projects_count.'</td></tr>';

$contacts_module = $GO_MODULES->get_module('addressbook');
$has_contacts_module = ($contacts_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $contacts_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $contacts_module['acl_write'])));

echo '<tr height="20">';
echo '<td class="TableHead2" width="16"></td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'name\',\''.$new_pr_sort_order.'\');">'.$strName.'</a>';
if ($pm_settings['sort_field'] == 'name')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'start_date\',\''.$new_pr_sort_order.'\');">'.$pm_start_date.'</a>';
if ($pm_settings['sort_field'] == 'start_date')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'end_date\',\''.$new_pr_sort_order.'\');">'.$pm_end_date.'</a>';
if ($pm_settings['sort_field'] == 'end_date')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'status\',\''.$new_pr_sort_order.'\');">'.$pm_status.'</a>';
if ($pm_settings['sort_field'] == 'status')
{
  echo $sort_arrow;
}
echo '</td>';

// TODO:
// Work around to display Project's progress using hacking way :-D
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:_sort(\'probability\',\''.$new_pr_sort_order.'\');">'.$pm_progress.'</a>';
if ($pm_settings['sort_field'] == 'probability')
{
  echo $sort_arrow;
}
echo '</td>';

if ($has_contacts_module && $contact_id == 0)
{
  echo '<td class="TableHead2">'.$pm_client.'</td>';
}
echo '<td class="TableHead2" colspan="3"></td></tr>';


if ($count > 0)
{
  $time = get_time();

  while ($projects->next_record())
  {
    $pid = $projects->f('id');

    $pm_status_values = array();
    $db->query('SELECT * FROM pmStatus WHERE cat_id='.$projects->f('cat_id'));
    while ($db->next_record())
      $pm_status_values[$db->f('value')] = $db->f('name');
    // Add/modify 3 special status
    $pm_status_values[STATUS_DROP] = $pm_status_drop;
    $pm_status_values[STATUS_DONE] = $pm_status_done;
    $pm_status_values[STATUS_BEGIN] = $pm_status_begin;

    $db->query('SELECT count(task_id) as total_task '.
               'FROM task WHERE task.task_project_id='.$pid);
    $db->next_record();
    $total_task = $db->f('total_task');
    $db->query('SELECT count(task_approved) as total_task_approved '.
               'FROM task WHERE task_project_id='.$pid.' AND task_approved=1');
    $db->next_record();
    $total_task_approved = $db->f('total_task_approved');
    $progress = round($total_task_approved * 100 / $total_task);

    echo '<tr height="18"><td>';
    $pstate = $projects->f('status');
    if ($pstate > 0 && $projects->f('end_date') < get_time())
    {
      echo '<img border="0" src="'.$GO_THEME->images['alert'].'" alt="'.$pm_alert.'" />';
    }else
    {
      $names = array('drop', 'ok', 'offer', 'ongoing');
      $p = $pstate > STATUS_BEGIN ? STATUS_IMPLEMENT : $pstate + 1;
      echo '<img border="0" src="'.$GO_THEME->images[$names[$p]].
           '" alt="'.$pm_status_values[$p].'" />';
    }

    $project_name = ($projects->f('description') == '') ? htmlspecialchars($projects->f('name')) : htmlspecialchars($projects->f('name').' ('.$projects->f('description').')');
    echo '</td><td nowrap><a href="'.$projects_module_url.'project.php?project_id='.$projects->f('id').'&return_to='.rawurlencode($link_back).'" class="normal">'.$project_name.'</a></td>';
    echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'], $projects->f('start_date')).'</td>';
    echo '<td nowrap>'.date($_SESSION['GO_SESSION']['date_format'], $projects->f('end_date')).'</td>';
    echo '<td nowrap>'.$pm_status_values[(int) $projects->f('status')].'</td>';
    echo '<td nowrap>'.$progress.'%</td>';
    if ($has_contacts_module && $contact_id == 0)
    {
      echo "<td nowrap>".show_contact($projects->f('contact_id'))."&nbsp;&nbsp;&nbsp;</td>";
    }

    /* TODO: add clocking here

    if (!$timer)
    {
      echo '<td><a href="'.$projects_module_url.'project.php?task=book&project_id='.$projects->f("id").'&return_to='.rawurlencode($link_back).'&contact_id='.$contact_id.'" title="'.$pm_enter_data.'"><img src="'.$GO_THEME->images['enter_data'].'" border="0"></a></td>';
      echo '<td><a href="'.$projects_module_url.'index.php?task=start_timer&project_id='.$projects->f("id").'&contact_id='.$contact_id.'&link_back='.urlencode($link_back).'" title="'.$pm_start_timer.'"><img src="'.$GO_THEME->images['clock'].'" border="0"></a></td>';
    }elseif($timer['project_id'] == $projects->f('id'))
    {
      echo '<td></td><td><a href="'.$projects_module_url.'project.php?task=stop_timer&project_id='.$projects->f("id").'&return_to='.rawurlencode($link_back).'&contact_id='.$contact_id.'" title="'.$pm_stop_timer.'"><img src="'.$GO_THEME->images['clockout'].'" border="0"></a></td>';
    }else
    */
    {
    echo '<td colspan="2">&nbsp;</td>';
    }
    echo "<td><a href='javascript:confirm_action(\"javascript:delete_project(".$projects->f("id").")\",\"".rawurlencode($strDeletePrefix."'".$project_name."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$project_name."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

  }
  $links = '';
  $max_links=10;
  if ($max_rows != 0)
  {
    if ($count > $max_rows)
    {
      $links = '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td>';
      $next_start = $first+$max_rows;
      $previous_start = $first-$max_rows;
      if ($first != 0)
      {
        $links .= '<a href="javascript:change_list(0, '.$max_rows.');">&lt;&lt;</a>&nbsp;';
        $links .= '<a href="javascript:change_list('.$previous_start.', '.$max_rows.');">'.$cmdPrevious.'</a>&nbsp;';
      }else
      {
        $links .= '<font color="#cccccc">&lt;&lt; '.$cmdPrevious.'</font>&nbsp;';
      }

      $start = ($first-(($max_links/2)*$max_rows));

      $end = ($first+(($max_links/2)*$max_rows));

      if ($start < 0)
      {
        $end = $end - $start;
        $start=0;
      }
      if ($end > $count)
      {
        $end = $count;
      }
      if ($start > 0)
      {
        $links .= '...&nbsp;';
      }

      for ($i=$start;$i<$end;$i+=$max_rows)
      {
        $page = ($i/$max_rows)+1;
        if ($i==$first)
        {
          $links .= '<b><i>'.$page.'</i></b>&nbsp;';
        }else
        {
          $links .= '<a href="'.$_SERVER['PHP_SELF'].'?post_action=projects&first='.$i.'">'.$page.'</a>&nbsp;';
        }
      }

      if ($end < $count)
      {
        $links .= '...&nbsp;';
      }

      $last_page = floor($count/$max_rows)*$max_rows;

      if ($count > $next_start)
      {
        $links .= '<a href="javascript:change_list('.$next_start.', '.$max_rows.');">'.$cmdNext.'</a>&nbsp;';
        $links .= '<a href="javascript:change_list('.$last_page.', '.$max_rows.');">&gt;&gt;</a>';
      }else
      {
        $links .= '<font color="#cccccc">'.$cmdNext.' &gt;&gt;</font>';
      }
      $links .= '</td><td align="right"><a class="normal" href="javascript:change_list(0, 0);">'.$cmdShowAll.'</a></td></tr></table>';

      echo '<tr height="20"><td colspan="99">'.$links.'</td></tr>';
    }
  }

}else
{
  echo '<tr><td colspan="99">'.$pm_no_projects.'</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table>';
?>

<script type="text/javascript">

function _sort(field, direction)
{
  document.forms[0].new_pr_sort_field.value=field;
  document.forms[0].new_pr_sort_order.value=direction;
  document.forms[0].submit();
}

function delete_project(id)
{
  document.forms[0].delete_project_id.value=id;
  document.forms[0].submit();
}

function change_list(first, max_rows)
{
  document.forms[0].first.value=first;
  document.forms[0].max_rows.value=max_rows;
  document.forms[0].submit();
}
</script>
