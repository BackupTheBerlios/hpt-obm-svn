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

if (isset($_POST['task']) && $_POST['task'] == 'delete_companies')
{
	for ($i=0;$i<sizeof($_POST['companies']);$i++)
	{
		$company = $ab->get_company($_POST['companies'][$i]);

		if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $company['acl_write']))
		{
			$ab->delete_company($_POST['companies'][$i]);
			$GO_SECURITY->delete_acl($company['acl_write']);
			$GO_SECURITY->delete_acl($company['acl_read']);
		}
	}
}

//define the items to show

$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;

if(isset($_REQUEST['new_sort_order']) && $_REQUEST['new_sort_order'] != $ab_settings['sort_companies_order'])
{
  $ab->set_companies_sorting($GO_SECURITY->user_id, 
						  $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
  $ab_settings['sort_companies_order'] = $_REQUEST['new_sort_order'];
  $ab_settings['sort_companies_field'] = $_REQUEST['new_sort_field'];  
}

if ($ab_settings['sort_companies_order'] == "DESC")
{
	$sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
	$new_sort_order = "ASC";
}else
{
	$sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
	$new_sort_order = "DESC";
}

echo '<input type="hidden" name="first" value="'.$first.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="task" />';
echo '<input type="hidden" name="new_sort_field" value="'.$ab_settings['sort_companies_field'].'" />';
echo '<input type="hidden" name="new_sort_order" value="'.$ab_settings['sort_companies_order'].'" />';

echo '<table border="0" cellspacing="0" cellpadding="0" width="100%">';


echo '<tr><td colspan="4">';

if ($ab->get_subscribed_addressbooks($GO_SECURITY->user_id) > 1)
{
	echo '<table border="0"><tr><td>'.$ab_addressbook.':</td><td>';
	$subscribed_addressbooks = new dropbox();
	while ($ab->next_record())
	{
		$subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));
	}
	$subscribed_addressbooks->print_dropbox('addressbook_id', $addressbook_id, 'onchange="javascript:document.forms[0].submit()"');
	echo '&nbsp;&nbsp;&nbsp;';

	echo '<input type="hidden" name="treeview" value="'.$is_treeview.'"/>';
	echo '<a href="javascript:document.forms[0].treeview.value=\''.(1-$is_treeview).'\';document.forms[0].submit()">'.($is_treeview ? $strListView : $strTreeView).'</a>';
	echo '</td></tr></table>';
}else
{
	echo '<input type="hidden" name="addressbook_id" value="'.$addressbook_id.'" />';
	echo '<input type="hidden" name="treeview" value="'.$is_treeview.'"/>';
	echo '<a href="javascript:document.forms[0].treeview.value=\''.(1-$is_treeview).'\';document.forms[0].submit()">'.($is_treeview ? $strListView : $strTreeView).'</a>';
}


$count = $ab->get_companies($addressbook_id, ($ab_settings['sort_companies_field'] == 'new' ? 'company_id' : $ab_settings['sort_companies_field']), $ab_settings['sort_companies_order'], $first, $max_rows);
$str_count = $count == 1 ? $ab_companies_count_single : $ab_companies_count;
echo '</td><td colspan="99" class="small" align="right">'.$count.' '.$str_count.'</td></tr>';
echo '<tr height="20">';

echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:_sort(\'name\',\''.$new_sort_order.'\');">'.$strName.'</a>';
if ($ab_settings['sort_companies_field'] == 'name')
{
	echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:_sort(\'city\',\''.$new_sort_order.'\');">'.$strCity.'</a>';
if ($ab_settings['sort_companies_field'] == 'city')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:_sort(\'email\',\''.$new_sort_order.'\');">'.$strEmail.'</a>';
if ($ab_settings['sort_companies_field'] == 'email')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:_sort(\'homepage\',\''.$new_sort_order.'\');">'.$strHomepage.'</a>';
if ($ab_settings['sort_companies_field'] == 'homepage')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" nowrap><a class="TableHead2" href="javascript:_sort(\'phone\',\''.$new_sort_order.'\');">'.$strPhone.'</a>';
if ($ab_settings['sort_companies_field'] == 'phone')
{
	echo $sort_arrow;
}
echo '</td>';

// check for new companies
$myab = new addressbook();
if ($myab->query("SELECT * FROM ab_addressbooks WHERE id='$addressbook_id'") &&
    $myab->next_record())
{
	$owner = $myab->f('user_id');
	if ($GO_SECURITY->user_id == $owner &&
	    $myab->query("SELECT company_id FROM ab_new_companies WHERE addressbook_id='$addressbook_id'"))
	{
		$new_companies = array();
		while ($myab->next_record())
			$new_companies[$myab->f('company_id')] = 1;
		if (isset($_REQUEST['reviewed']) && (int)$_REQUEST['reviewed'] != 0)
		{
			$id = (int)$_REQUEST['reviewed'];
			if (isset($new_companies[$id]))
			{
				@$myab->query("DELETE FROM ab_new_companies WHERE addressbook_id='$addressbook_id' AND company_id='$id'");
				unset($new_companies[$id]);
			}
			else
			{
				@$myab->query("REPLACE INTO ab_new_companies (addressbook_id,company_id) VALUES('$addressbook_id','$id')");
				$new_companies[$id] = 1;
			}
		}
		echo '<td class="TableHead2"><a class="TableHead2" href="javascript:_sort(\'company_id\',\''.$new_sort_order.'\');">'.$strNew.'</a>';
		if ($ab_settings['sort_companies_field'] == 'company_id')
		{
			echo $sort_arrow;
		}
		echo '</td>';
		echo '<input type="hidden" name="reviewed" value="" />';
	}
}
unset($myab);


echo '</tr>';

if ($count > 0)
{
	while($ab->next_record())
	{
		echo '<tr id="'.$ab->f("id").'" '.(isset($new_companies[$ab->f('id')]) ? 'class="TableInside2"': '').' >';
		echo '<td width="16"><input id="'.addslashes($ab->f('name')).'" type="checkbox" onclick="javacript:item_click(this)" name="companies[]" value="'.$ab->f("id").'" /></td>';
		echo '<td><a class="normal" href="'.$GO_MODULES->url.'company.php?company_id='.$ab->f('id').'&return_to='.rawurlencode($link_back).'">'.htmlspecialchars($ab->company_name()).'</a></td>';
		echo '<td>'.$ab->f('city').'</td>';
		echo '<td>'.mail_to($ab->f('email'), $ab->f('email')).'</td>';
		echo '<td><a class="normal" target="_blank" href="'.$ab->f('homepage').'">'.$ab->f('homepage').'</a></td>';
		echo '<td>'.$ab->f('phone').'</td>';
		if (isset($new_companies))
		{
			echo '<td align="center"><input type="checkbox" onchange="javacript:document.forms[0].reviewed.value=\''.$ab->f('id').'\';document.forms[0].submit()" '.(isset($new_companies[$ab->f('id')]) ? 'checked' : '').' /></td>';
		}
		echo '</tr>';
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
	echo '<tr><td colspan="99">'.$ab_no_companies.'</td></tr>';
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table>';
?>

<script type="text/javascript">

function _sort(field, direction)
{
	document.forms[0].new_sort_field.value=field;
	document.forms[0].new_sort_order.value=direction;
	document.forms[0].submit();
}

function delete_company(id)
{
	document.forms[0].delete_company_id.value=id;
	document.forms[0].submit();
}

function change_list(first, max_rows)
{
	document.forms[0].first.value=first;
	document.forms[0].max_rows.value=max_rows;
	document.forms[0].submit();
}

function invert_selection()
{
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
		{
			document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
			document.forms[0].elements[i].onclick();
		}
	}
}

function item_click(check_box)
{
	var item = get_object(check_box.value);
	if (check_box.checked)
	{
		item.style.backgroundColor = '#FFFFCC';
	}else
	{
		item.style.backgroundColor = '#FFFFFF';
	}
}

function confirm_delete()
{
	var count = 0;
	var name = new String;
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
		{
			if (document.forms[0].elements[i].checked == true)
			{
				count++;
				name = document.forms[0].elements[i].id;
			}
		}
	}
	switch (count)
	{
		case 0:
			alert("<?php echo $contacts_no_select; ?>");
		break;

		case 1:
			if (confirm("<?php echo $strDeletePrefix; ?> '"+name+"' <?php echo $strDeleteSuffix; ?>"))
			{
				document.forms[0].task.value='delete_companies';
				document.forms[0].submit();
			}
		break;

		default:
			if (confirm("<?php echo $strDeletePrefix.$strThis; ?> "+count+" <?php echo $contacts_contacts.$strDeleteSuffix; ?>"))
			{
				document.forms[0].task.value='delete_companies';
				document.forms[0].submit();
			}
		break;
	}
}

</script>