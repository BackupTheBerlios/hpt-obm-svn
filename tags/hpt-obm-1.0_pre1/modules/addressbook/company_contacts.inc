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

if (isset($_REQUEST['delete_contact_id']) && $_REQUEST['delete_contact_id'] > 0)
{
	$contact = $ab->get_contact($_REQUEST['delete_contact_id']);
	if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $contact['acl_write']))
	{
		$ab->add_contact_to_company($_REQUEST['delete_contact_id'], 0);
	}else
	{
		$feedback = '<p class="Error">'.$strAccessDenied.'</p>';
	}
}

//define the items to show
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;

//determine sorting
$contact_sort = isset($_COOKIE['contact_sort']) ? $_COOKIE['contact_sort'] : 'name';
$contact_direction = isset($_COOKIE['contact_direction']) ? $_COOKIE['contact_direction'] : 'ASC';

if ($contact_direction == "DESC")
{
	$sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
	$new_sort_direction = "ASC";
}else
{
	$sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
	$new_sort_direction = "DESC";
}

echo '<input type="hidden" name="first" value="'.$first.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="delete_contact_id" />';
echo '<input type="hidden" name="new_sort_field" value="'.$contact_sort.'" />';
echo '<input type="hidden" name="new_sort_direction" value="'.$contact_direction.'" />';

if ($write_permission) {
	echo '<a class="normal" href="'.$GO_MODULES->url.'contact.php?company_id='.$company_id.'&addressbook_id='.$addressbook_id.'&return_to='.rawurlencode($link_back.'&active_tab='.$tabtable->active_tab).'">'.$ab_add_new.'</a>';
	echo '&nbsp;|&nbsp;';
	echo '<a class="normal" href="'.$ab->select_contacts('', $GO_MODULES->url.'add_employees.php?company_id='.$company_id, 'true', 'false', 'false', 'true', 'false', 'id').'">'.$ab_add_existing.'</a>';
}


echo '<table border="0" cellspacing="0" cellpadding="2">';
echo '<tr><td>';
$count = $ab->get_company_contacts($company_id, $contact_sort, $contact_direction, $first, $max_rows);
$str_count = $count == 1 ? $ab_employee_count_single : $ab_employee_count_multiple;
echo '</td><td colspan="99" class="small" align="right">'.$count.' '.$str_count.'</td></tr>';
echo '<tr height="20">';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'name\',\''.$new_sort_direction.'\');">'.$strName.'</a>';
if ($contact_sort == 'name')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'email\',\''.$new_sort_direction.'\');">'.$strEmail.'</a>';
if ($contact_sort == 'email')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'home_phone\',\''.$new_sort_direction.'\');">'.$strPhone.'</a>';
if ($contact_sort == 'home_phone')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'work_phone\',\''.$new_sort_direction.'\');">'.$strWorkphone.'</a>';
if ($contact_sort == 'work_phone')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2">&nbsp;</td></tr>';

if ($count > 0)
{
	while($ab->next_record())
	{
		$middle_name = $ab->f('middle_name') == '' ? '' : $ab->f('middle_name').' ';
		$name = $ab->f('last_name').' '.$middle_name.$ab->f('first_name');

		echo '<tr><td><a class="normal" href="'.$GO_MODULES->url.'contact.php?contact_id='.$ab->f('id').'&return_to='.rawurlencode($link_back.'&active_tab='.$tabtable->active_tab).'">'.$name.'</a></td>';
		echo '<td>'.mail_to($ab->f('email'), $ab->f('email')).'</td>';
		echo '<td>'.$ab->f('home_phone').'</a></td>';
		echo '<td>'.$ab->f('work_phone').'</td>';
		if ($GO_SECURITY->has_permission($GO_SECURITY->user_id,$ab->f('acl_write')))
			echo "<td><a href='javascript:div_confirm_action(\"javascript:delete_contact(".$ab->f('id').")\",\"".div_confirm_id($strDeletePrefix."'".$name."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$name."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
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
	echo '<tr><td colspan="99">'.$ab_no_employees.'</td></tr>';
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table>';
echo '<br />';
$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
?>

<script type="text/javascript">

function _sort(field, direction)
{
	document.company_form.new_sort_field.value=field;
	document.company_form.new_sort_direction.value=direction;
	document.company_form.submit();
}

function delete_contact(id)
{
	document.company_form.delete_contact_id.value=id;
	document.company_form.submit();
}

function change_list(first, max_rows)
{
	document.company_form.first.value=first;
	document.company_form.max_rows.value=max_rows;
	document.company_form.submit();
}
</script>
