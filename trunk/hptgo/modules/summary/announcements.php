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
$GO_MODULES->authenticate('summary', true);
require($GO_LANGUAGE->get_language_file('summary'));

$page_title=$lang_modules['summary'];
require($GO_MODULES->class_path."announcements.class.inc");
$announcements = new announcements();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$announcement_id = isset($_REQUEST['announcement_id']) ? $_REQUEST['announcement_id'] : 0;

$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

if (isset($_REQUEST['new_sort_field']) && $_REQUEST['new_sort_field'] != '')
{
	SetCookie("an_sort_field",$_REQUEST['new_sort_field'],time()+3600*24*365,"/","",0);
	$_COOKIE['an_sort_field'] = $_REQUEST['new_sort_field'];
}
if (isset($_REQUEST['new_sort_direction']) && $_REQUEST['new_sort_direction'] != '')
{
	SetCookie("an_sort_direction",$_REQUEST['new_sort_direction'],time()+3600*24*365,"/","",0);
	$_COOKIE['an_sort_direction'] = $_REQUEST['new_sort_direction'];
}

if (isset($_REQUEST['delete_announcement_id']) && $_REQUEST['delete_announcement_id'] > 0)
{
	$announcement = $announcements->get_announcement($_REQUEST['delete_announcement_id']);

	if($announcements->delete_announcement($_REQUEST['delete_announcement_id']))
	{
		$GO_SECURITY->delete_acl($announcement['acl_id']);
	}else
	{
		$feedback = '<p class="Error">'.$strAccessDenied.'</p>';
	}
}

$announcements_module_url = isset($announcements_module_url) ? $announcements_module_url : $GO_MODULES->url;

//define the items to show
$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;


//determine sorting
$an_sort_field = isset($_COOKIE['an_sort_field']) ? $_COOKIE['an_sort_field'] : 'title';
$an_sort_direction = isset($_COOKIE['an_sort_direction']) ? $_COOKIE['an_sort_direction'] : 'ASC';

if ($an_sort_direction == "DESC")
{
	$sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
	$new_sort_direction = "ASC";
}else
{
	$sort_arrow = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
	$new_sort_direction = "DESC";
}

require($GO_THEME->theme_path."header.inc");

echo '<a href="announcement.php" class="normal">'.$cmdAdd.'</a><br /><br />';

$count = $announcements->get_all_announcements($an_sort_field, $an_sort_direction, $first, $max_rows);
echo '<form name="announcements_form" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="first" value="'.$first.'" />';
echo '<input type="hidden" name="max_rows" value="'.$max_rows.'" />';
echo '<input type="hidden" name="delete_announcement_id" />';
echo '<input type="hidden" name="new_sort_field" value="'.$an_sort_field.'" />';
echo '<input type="hidden" name="new_sort_direction" value="'.$an_sort_direction.'" />';

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
$str_count = $count == 1 ? $sum_announcements_count_single : $sum_announcements_count;
echo '<tr><td colspan="99" class="small" align="right">'.$count.' '.$str_count.'</td></tr>';
echo '<tr height="20">';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'name\',\''.$new_sort_direction.'\');">'.$strName.'</a>';
if ($an_sort_field == 'title')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'name\',\''.$new_sort_direction.'\');">'.$sum_due_time.'</a>';
if ($an_sort_field == 'due_time')
{
	echo $sort_arrow;
}
echo '</td>';

echo '<td class="TableHead2" width="100" nowrap><a class="TableHead2" href="javascript:_sort(\'mtime\',\''.$new_sort_direction.'\');">'.$strModifiedAt.'</a>';
if ($an_sort_field == 'mtime')
{
	echo $sort_arrow;
}
echo '</td>';
echo '<td class="TableHead2">&nbsp;</td></tr>';

if ($count > 0)
{	
	while($announcements->next_record())
	{
		echo '<tr><td><a class="normal" href="announcement.php?announcement_id='.$announcements->f('id').'&return_to='.rawurlencode($link_back).'">'.htmlspecialchars($announcements->f('title')).'</a></td>';
		$due_time = $announcements->f('due_time') > 0 ? date($_SESSION['GO_SESSION']['date_format'], $announcements->f('due_time')) : '';
		echo '<td>'.$due_time.'</td>';
		echo '<td>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $announcements->f('mtime')+($_SESSION['GO_SESSION']['timezone']*3600)).'</td>';
		echo "<td><a href='javascript:confirm_action(\"javascript:delete_announcement(".$announcements->f('id').")\",\"".rawurlencode($strDeletePrefix."'".htmlspecialchars($announcements->f('title'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($announcements->f('title'))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>\n";
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
	echo '<tr><td colspan="99">'.$sum_no_announcements.'</td></tr>';
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table><br />';

$button = new button($cmdClose, "javascript:document.location='index.php';");
?>
</form>

<script type="text/javascript">

function _sort(field, direction)
{
	document.forms[0].new_sort_field.value=field;
	document.forms[0].new_sort_direction.value=direction;
	document.forms[0].submit();
}

function delete_announcement(id)
{
	document.forms[0].delete_announcement_id.value=id;
	document.forms[0].submit();
}

function change_list(first, max_rows)
{
	document.forms[0].first.value=first;
	document.forms[0].max_rows.value=max_rows;
	document.forms[0].submit();
}
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
