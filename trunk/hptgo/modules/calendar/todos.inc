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
require_once($GO_LANGUAGE->get_language_file('calendar'));
$todos = new todos();

$calendar_module_url = isset($calendar_module_url) ? $calendar_module_url : $GO_MODULES->url;

if(!isset($cal_settings))
{
  require_once($calendar_module['class_path'].'calendar.class.inc');
  $cal = new calendar();
  $cal_settings = $cal->get_settings($GO_SECURITY->user_id);
}

$ab_module = $GO_MODULES->get_module('addressbook');
if (!$ab_module || !($GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_read']) || $GO_SECURITY->has_permission($GO_SECURITY->user_id, $ab_module['acl_write'])))
{
	$ab_module = false;
}else
{
	require_once($ab_module['path'].'classes/addressbook.class.inc');
	$ab = new addressbook();
}



$contact_id = isset($contact_id) ? $contact_id : 0;

//define the items to show
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;

echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';
echo '<input type="hidden" name="first" value="'.$first.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="completed_todo_id" />';
echo '<input type="hidden" name="delete_todo_id" />';

if (isset($_POST['delete_todo_id']) && $_POST['delete_todo_id'] > 0)
{
	$todos->delete_todo($_POST['delete_todo_id']);
}

if (isset($_POST['completed_todo_id']) && $_POST['completed_todo_id'] > 0)
{
	if ($todo = $todos->get_todo($_POST['completed_todo_id']))
	{
		if ($todos->f('completion_time') > 0)
		{
			$todos->set_uncompleted($_POST['completed_todo_id']);
		}else
		{
			$todos->set_completed($_POST['completed_todo_id']);
		}
	}
}

$hide_completed_todos = isset($_REQUEST['hide_completed_todos']) ? 1 : 0;
if (isset($_REQUEST['show_todos']) && 
	(
	  ($hide_completed_todos != $cal_settings['hide_completed_todos']) ||
	  ($_REQUEST['show_todos'] != $cal_settings['show_todos'])
	)
  )
{
  $cal->set_todo_view($GO_SECURITY->user_id, $hide_completed_todos, $_REQUEST['show_todos']);
  $cal_settings['hide_completed_todos'] = $hide_completed_todos;  
  $cal_settings['show_todos'] = $_REQUEST['show_todos'];  
}

echo 	'<table border="0"><tr><td>'.
		'<table border="0" width="100%" height="16" cellpadding="0" '.
		' cellspacing="0"><td width="100%">'.
		'<h3>'.$cal_todos.'</h3>'.
		'</td><td width="16"><a title="'.$cal_add_todo.'" href="'.$calendar_module_url.'todo.php?return_to='.
		urlencode($link_back).'&contact_id='.$contact_id.'"><img src="'.
		$GO_THEME->images['cal_add_todo'].'" border="0"></a></td></tr>'.
		'</table>'.
		'</td></tr><tr><td>';

$checkbox = new checkbox('hide_completed_todos',
						'1',
						$cal_hide_completed,
						$cal_settings['hide_completed_todos'],
						'',
						'onclick="javascript:document.forms[0].submit();"');

echo 	'</td></tr><tr><td>';
$dropbox = new dropbox();
$dropbox->add_value(SHOW_ALL, $cal_show_all);
$dropbox->add_value(SHOW_OWN, $cal_show_own);
$dropbox->add_value(SHOW_RESPONSIBLE, $cal_show_responsible);
$dropbox->print_dropbox('show_todos', $cal_settings['show_todos'], 'onchange="javascript:document.forms[0].submit();"', false, 10, 200);

echo '<td></tr></table>';

switch ($cal_settings['show_todos'])
{
	case SHOW_ALL:
		$load_user_id = $GO_SECURITY->user_id;
		$load_res_user_id = $GO_SECURITY->user_id;
	break;

	case SHOW_OWN:
		$load_user_id = $GO_SECURITY->user_id;
		$load_res_user_id = 0;
	break;

	case SHOW_RESPONSIBLE:
		$load_user_id = 0;
		$load_res_user_id = $GO_SECURITY->user_id;
	break;
}

$count = $todos->get_todos($cal_settings['hide_completed_todos'], $load_user_id, $load_res_user_id,
						$contact_id, $first, $max_rows);

$local_time = get_time();

if ($count == 0)
{
	echo '<p align="center">'.$cal_no_todos.'</p>';
}else
{
	echo '<table border="0" cellspacing="0">';

	echo '<tr><td height="1" colspan="4"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
	while($todos->next_record())
	{
		$start_time = $todos->f('start_time')+($_SESSION['GO_SESSION']['timezone']*3600);
		$due_time = $todos->f('due_time')+($_SESSION['GO_SESSION']['timezone']*3600);
		$check = '';
		if ($todos->f('completion_time') > 0)
		{
			$class = 'todo_completed';
			$check = 'checked';
		}elseif($due_time < $local_time)
		{
			$class = 'todo_late';
		}else
		{
			$class= 'todo';
		}

		switch ($todos->f("priority"))
		{
		  case "0":
			$priority = '<img src="'.$GO_THEME->images['low_priority'].'" width="8" height="16" />';
		  break;

		  case "2":
			$priority = '<img src="'.$GO_THEME->images['high_priority'].'" width="8" height="16" />';
		  break;

		  default:
			$priority = '';
		  break;
		}

		$title = 	$sc_start_at.': '.date($_SESSION['GO_SESSION']['date_format'].' '.
									$_SESSION['GO_SESSION']['time_format'], $start_time)."<br />".
							$cal_due_at.': '.date($_SESSION['GO_SESSION']['date_format'].' '.
									$_SESSION['GO_SESSION']['time_format'], $due_time);
		if ($todos->f('completion_time') > 0)
		{
			$title .= 	"<br />".$cal_completed.': '.
									date($_SESSION['GO_SESSION']['date_format'].' '.
									$_SESSION['GO_SESSION']['time_format'],
									$todos->f('completion_time')+
									($_SESSION['GO_SESSION']['timezone']*3600));
		}

		if ($ab_module && $todos->f('contact_id') > 0 &&
											$contact = $ab->get_contact($todos->f('contact_id')))
		{
			if ($title != '')
			{
				$title .= "<br />";
			}
			$middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
			$contact_name = $contact['last_name'].' '.$middle_name.$contact['first_name'];
			$title .= $sc_client.": ".htmlspecialchars($contact_name);
			if ($contact['company_name'] != '')
			{
				$title .= " (".htmlspecialchars($contact['company_name']).")";
			}
		}
		if ($todos->f('location') != '')
		{
			if ($title != '')
			{
				$title .= "<br />";
			}
			$title .= $sc_location.": ".htmlspecialchars($todos->f('location'));
		}
		if ($todos->f('description') != '')
		{
			if ($title != '')
			{
				$title .= "<br />";
			}
			$title .= $sc_description.': '.htmlspecialchars($todos->f('description'));
		}
		$title .= "<br />".$cal_status.': '.$todos->f('status').'%';


		echo 	'<tr style="background-color: #'.$todos->f('background').'"><td width="20"><input type="checkbox" name="todo[]" value="'.
					$todos->f('id').'" onclick="javascript:'.
					'set_completed(this.value);" '.$check.'></td>'.
					'<td width="8">'.$priority.'</td>'.
					'<td width="100%"><a class="'.$class.'" href="'.$calendar_module_url.'todo.php?todo_id='.$todos->f('id').
					'&return_to='.urlencode($link_back).'" '.$overlib->print_overlib($title).'">'.$todos->f('name').'</a></td>'.
					"<td><a href='javascript:confirm_action(\"javascript:delete_todo(".
					$todos->f('id').")\",\"".($strDeletePrefix."%27".
					htmlspecialchars($todos->f('name'))."%27".$strDeleteSuffix).
					"\")' title=\"".$strDeleteItem." '".
					htmlspecialchars($todos->f('name'))."'\"><img src=\"".
					$GO_THEME->images['delete']."\" border=\"0\"></a></td>".
					'</tr>';

	}

	$links = '';
	$max_links=10;
	if ($max_rows != 0)
	{
		if ($count > $max_rows)
		{
			$links = '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td align="center">';
			$next_start = $first+$max_rows;
			$previous_start = $first-$max_rows;
			if ($first != 0)
			{
				$links .= '<a href="javascript:change_list(0, '.$max_rows.');">&lt&lt</a>&nbsp;';
				$links .= '<a href="javascript:change_list('.$previous_start.', '.$max_rows.');">'.$cmdPrevious.'</a>&nbsp;';
			}else
			{
				$links .= '<font color="#cccccc">&lt&lt '.$cmdPrevious.'</font>&nbsp;';
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
				$links .= '<a href="javascript:change_list('.$last_page.', '.$max_rows.');">&gt&gt</a>';
			}else
			{
				$links .= '<font color="#cccccc">'.$cmdNext.' &gt&gt</font>';
			}
			$links .= '<br /><a class="normal" href="javascript:change_list(0, 0);">'.$cmdShowAll.'</a></td></tr></table>';

			echo '<tr><td height="1" colspan="4"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
			echo '<tr height="20"><td colspan="3">'.$links.'</td></tr>';
		}
	}

	echo '</table>';
}
?>
<script type="text/javascript">
function change_list(first, max_rows)
{
	document.forms[0].first.value=first;
	document.forms[0].max_rows.value=max_rows;
	document.forms[0].submit();
}

function set_completed(todo_id)
{
	document.forms[0].completed_todo_id.value=todo_id;
	document.forms[0].submit();
}

function delete_todo(id)
{
	document.forms[0].delete_todo_id.value=id;
	document.forms[0].submit();
}
</script>
