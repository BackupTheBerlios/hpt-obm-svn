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

if (!isset($_SESSION['expanded']))
{
	$_SESSION['expanded'][]=-1;
}

//expand or collaps the expand id which is passed when a user clicks a node
if (isset($_REQUEST['expand_path']))
{
	$expand_path = explode(',',$_REQUEST['expand_path']);
	foreach ($expand_path as $i => $k)
	{
		$key = array_search($k, $_SESSION['expanded']);
		if ($i+1 < count($expand_path))
		{
			if (!$key)
				$_SESSION['expanded'][] = $key;
		}
		else
		{
			if (!$key)
				$_SESSION['expanded'][] = $k;
			else
				unset($_SESSION['expanded'][$key]);
		}
	}
}

$image['mlastnode'] = '<img src="'.$GO_THEME->images['mlastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['emptylastnode'] = '<img src="'.$GO_THEME->images['emptylastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['plastnode'] = '<img src="'.$GO_THEME->images['plastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['mnode'] = '<img src="'.$GO_THEME->images['mnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['emptynode'] = '<img src="'.$GO_THEME->images['emptynode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['pnode'] = '<img src="'.$GO_THEME->images['pnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['vertline'] = '<img src="'.$GO_THEME->images['vertline'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['blank'] = '<img src="'.$GO_THEME->images['blank'].'" border="0" height="22" width="16" align="absmiddle" />';
$image['group'] = '<img src="'.$GO_THEME->images['workgroup'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['user'] = '<img src="'.$GO_THEME->images['workstation'].'" border="0" height="22" width="20" align="absmiddle" />';

$image['opened_folder'] = '<img src="'.$GO_THEME->images['folderopen'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['closed_folder'] = '<img src="'.$GO_THEME->images['folderclosed'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['opened_folder_shared'] = '<img src="'.$GO_THEME->images['shared_folder_open'].'" border="0" height="22" width="24" align="absmiddle" />';
$image['closed_folder_shared'] = '<img src="'.$GO_THEME->images['shared_folder_closed'].'" border="0" height="22" width="24" align="absmiddle" />';

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
}

$rowcount = 0;
function print_companies($parent, $path = '', $images='', $arguments='')
{
	global $image, $path, $GO_SECURITY, $GO_THEME,$group_folders;
	global $addressbook_id,$ab_settings;
	global $rowcount,$link_back;

	$ab = new addressbook();
	$count = $ab->get_companies_by_parent($addressbook_id,$parent,$ab_settings['sort_companies_field'], $ab_settings['sort_companies_order']);
	$path .= $path != '' ? ",$parent" : $parent;
	for ($i=0;$i<$count;$i++)
	{
		$ab->next_record();

/*
		if ($ab->f('parent') ||
		  $fs->has_read_permission($GO_SECURITY->user_id, $folders[$i]['path']))
//		if ($fs->has_read_permission($GO_SECURITY->user_id, $folders[$i]['path']))
		{
			$subfolders = $fs->get_folders($folders[$i]['path']);
			$subfolders_count = count($subfolders);
			$open = in_array($folders[$i]['path'], $_SESSION['expanded']);
		}else*/
		{
			$subfolders = array();
			$subfolders_count = 0;
			$open = false;
		}

		$subab = new addressbook();
		$subcount = $subab->get_companies_by_parent($addressbook_id,$ab->f('id'));
		//if ($subfolders_count > 0)
		if ($subcount)
		{
			if ($i < ($count-1))
			{
				$new_image = $image['vertline'];
				$node = $open ? $image['mnode'] : $image['pnode'];

			}else
			{
				$new_image = $image['blank'];
				$node = $open ? $image['mlastnode'] : $image['plastnode'];
			}
		}else
		{
			if ($i < ($count-1))
			{
				$new_image = $image['vertline'];
				$node = $image['emptynode'];
			}else
			{
				$new_image = $image['blank'];
				$node = $image['emptylastnode'];
			}
		}
		unset($subab);

		$open = in_array($ab->f('id'), $_SESSION['expanded']);
		if ($open)
		{
			$folder_image = $image['opened_folder'];
		}else
		{
			$folder_image = $image['closed_folder'];
		}

		$folder_image = $image['opened_folder'];

		$short_name = /*cut_string(*/$ab->f('name')/*, 30)*/;

		echo '<tr class = "'.($rowcount ++ % 2 == 0 ? "TableInside1" : "TableInside2").'"id="'.$ab->f("id").'">';
		echo '<td width="16"><input id="'.addslashes($ab->f('name')).'" type="checkbox" onclick="javacript:item_click(this)" name="companies[]" value="'.$ab->f("id").'" /></td>';
		if ($subcount)
			echo '<td><a href="'.$_SERVER['PHP_SELF'].'?post_action=companies&treeview=1&expand_path='.$path.','.$ab->f('id').'">'.$images.$node.'</a>';
		else
			echo '<td>'.$images.$node;

		echo $folder_image;
		echo '<a class="normal" href="'.$GO_MODULES->url.'company.php?company_id='.$ab->f('id').'&return_to='.rawurlencode($link_back).'">'.htmlspecialchars($ab->f('name')).'</a></td>';
		echo '<td>'.$ab->f('city').'</td>';
		echo '<td>'.mail_to($ab->f('email'), $ab->f('email')).'</td>';
		echo '<td><a class="normal" target="_blank" href="'.$ab->f('homepage').'">'.$ab->f('homepage').'</a></td>';
		echo '<td>'.$ab->f('phone').'</td></tr>';

		if ($open)
		{
			print_companies($ab->f('id'), $path,$images.$new_image, $arguments);
		}
	}
	unset($ab);
	return $count;
}

//$count = $ab->get_companies_by_parent($addressbook_id, $ab_settings['sort_companies_field'], $ab_settings['sort_companies_order'], $first, $max_rows);
//$str_count = $count == 1 ? $ab_companies_count_single : $ab_companies_count;
//echo '</td><td colspan="99" class="small" align="right">'.$count.' '.$str_count.'</td></tr>';
echo '</td><td colspan="99" class="small" align="right"></td></tr>';
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
echo '</tr>';

$count = print_companies(0);
if ($count == 0)
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
