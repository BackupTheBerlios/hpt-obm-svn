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
if(!isset($no_settings))
{
  $no_settings = $notes->get_settings($GO_SECURITY->user_id);
}  

if (isset($_REQUEST['delete_note_id']) && $_REQUEST['delete_note_id'] > 0)
{
  $note = $notes->get_note($_REQUEST['delete_note_id']);
  if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $note['acl_write']))
  {
    if($notes->delete_note($_REQUEST['delete_note_id']))
    {
      $GO_SECURITY->delete_acl($note['acl_read']);
      $GO_SECURITY->delete_acl($note['acl_write']);
    }
  }else
  {
    $feedback = '<p class="Error">'.$strAccessDenied.'</p>';
  }
}

$notes_module_url = isset($notes_module_url) ? $notes_module_url : $GO_MODULES->url;

//define the items to show_notes
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;

if(isset($_REQUEST['show_notes']) && $_REQUEST['show_notes'] != $no_settings['show_notes'])
{
  $notes->set_show_notes($GO_SECURITY->user_id, $_REQUEST['show_notes']);
  $no_settings['show_notes'] = $_REQUEST['show_notes'];  
}

//define load parameters
$project_id = isset($project_id) ? $project_id : 0;
$contact_id = isset($contact_id) ? $contact_id : 0;
$file_path = isset($file_path) ? addslashes($file_path) : '';

if(isset($_REQUEST['new_no_sort_order']) && $_REQUEST['new_no_sort_order'] != $no_settings['sort_order'])
{
  $notes->set_sorting($GO_SECURITY->user_id, 
  $_REQUEST['new_no_sort_field'], $_REQUEST['new_no_sort_order']);
  $no_settings['sort_order'] = $_REQUEST['new_no_sort_order'];
  $no_settings['sort_field'] = $_REQUEST['new_no_sort_field'];  
}

if ($no_settings['sort_order'] == "DESC")
{
  $sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $new_no_sort_order = "ASC";
}else
{
  $sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $new_no_sort_order = "DESC";
}

echo '<a href="'.$notes_module_url.'note.php?contact_id='.$contact_id.'&project_id='.$project_id.'&file_path='.urlencode($file_path).'&return_to='.urlencode($link_back).'" class="normal">'.$cmdAdd.'</a><br /><br />';

$count = $notes->get_notes($GO_SECURITY->user_id, $project_id, $contact_id, $file_path, $no_settings['show_notes'], $no_settings['sort_field'], $no_settings['sort_order'], $first, $max_rows);

echo '<input type="hidden" name="first" value="'.$first.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="delete_note_id" />';
echo '<input type="hidden" name="new_no_sort_field" value="'.$no_settings['sort_field'].'" />';
echo '<input type="hidden" name="new_no_sort_order" value="'.$no_settings['sort_order'].'" />';
echo '<input type="hidden" name="show_notes" value="'.$no_settings['show_notes'].'" />';

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
$str_count = $count == 1 ? $no_notes_count_single : $no_notes_count;
echo '<tr><td>';
$dropbox = new dropbox();
$dropbox->add_value(SHOW_ALL_NOTES, $no_show_all);
$dropbox->add_value(SHOW_OWN_NOTES, $no_show_own);
$dropbox->add_value(SHOW_RESPONSIBLE_NOTES, $no_show_responsible);
$dropbox->print_dropbox('show_notes', $no_settings['show_notes'], 'onchange="javascript:document.forms[0].submit();"');

echo '</td><td colspan="99" class="small" align="right">'.$count.' '.$str_count.'</td></tr>';
echo '<tr height="20">';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'name\',\''.$new_no_sort_order.'\');">'.$strName.'</a>';
if ($no_settings['sort_field'] == 'name')
{
  echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'catagory_name\',\''.$new_no_sort_order.'\');">'.$no_catagory.'</a>';
if ($no_settings['sort_field'] == 'catagory_name')
{
  echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'due_date\',\''.$new_no_sort_order.'\');">'.$no_due_date.'</a>';
if ($no_settings['sort_field'] == 'due_date')
{
  echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'mtime\',\''.$new_no_sort_order.'\');">'.$strModifiedAt.'</a>';
if ($no_settings['sort_field'] == 'mtime')
{
  echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2">&nbsp;</td></tr>';

if ($count > 0)
{
  $notes_module_url = isset($notes_module_url) ? $notes_module_url : $GO_MODULES->url;
  while($notes->next_record())
  {
    $catagory_name = ($notes->f('catagory_name') == '') ? $no_none : htmlspecialchars($notes->f('catagory_name'));
    echo '<tr><td><a class="normal" href="'.$notes_module_url.'note.php?note_id='.$notes->f('id').'&return_to='.rawurlencode($link_back).'">'.htmlspecialchars($notes->f('name')).'</a></td>';
    echo '<td>'.$catagory_name.'</td>';
    $due_date = $notes->f('due_date') > 0 ? date($_SESSION['GO_SESSION']['date_format'], $notes->f('due_date')) : '';
    echo '<td>'.$due_date.'</td>';
    echo '<td>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $notes->f('mtime')+($_SESSION['GO_SESSION']['timezone']*3600)).'</td>';
    echo "<td><a href='javascript:confirm_action(\"javascript:delete_note(".$notes->f('id').")\",\"".rawurlencode($strDeletePrefix."'".htmlspecialchars($notes->f('name'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($notes->f('name'))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
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
	  $links .= '<a href="javascript:change_list('.$i.', '.$max_rows.');">'.$page.'</a>&nbsp;';
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
      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
    }
  }

}else
{
  echo '<tr><td colspan="99">'.$no_no_notes.'</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table>';
?>

<script type="text/javascript">

function _sort(field, direction)
{
  document.forms[0].new_no_sort_field.value=field;
  document.forms[0].new_no_sort_order.value=direction;
  document.forms[0].submit();
}

function delete_note(id)
{
  document.forms[0].delete_note_id.value=id;
  document.forms[0].submit();
}

function change_list(first, max_rows)
{
  document.forms[0].first.value=first;
  document.forms[0].max_rows.value=max_rows;
  document.forms[0].submit();
}
</script>
