<?php
/*
   Copyright HPT Commerce 2004
   Author: Nguyen Thai Ngoc Duy <duyntn@hptvietnam.com.vn>
   Version: 1.0 Release date: 21 September 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
if (isset($_REQUEST['active_tab']))
  $page = $_REQUEST['active_tab'];

$user = $GO_SECURITY->user_id;

$db = new db();

switch ($task) {
	case 'save':
		$com = $_REQUEST['com'];
		$show_text = isset($_REQUEST['show_text']) && $_REQUEST['show_text'] == '1' ? '1' : '0';

		if ($_REQUEST['com'] > 0) {
			$com = $_REQUEST['com'];
			$com_string = implode(" ", $com);
			$db->query("UPDATE users SET modules='$com_string:$show_text' WHERE id='{$GO_SECURITY->user_id}'");
		}
    echo '<script type="text/javascript">';

    if ($_POST['close'] == 'true')
    {
      echo 'parent.location="'.$GO_CONFIG->host.'index.php?return_to='.urlencode($return_to).'";';
    }else
    {
      echo 'parent.location="'.$GO_CONFIG->host.'index.php?return_to='.urlencode($return_to).'";';
    }
    echo '</script>';
		break;
}


$modules = $GO_MODULES->get_modules_with_locations();
$user = $GO_USERS->get_user($GO_SECURITY->user_id);

foreach ($modules as $id => $module) {
	if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) ||
		$GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_write']))
		$module_map[$module['id']] = $id;
}

$user_modules_properties = explode(':',$user['modules']);
$com_string = $user_modules_properties[0];
$show_text = isset($user_modules_properties[1]) ? $user_modules_properties[1] == '1' : true;

if ($com_string == '') {
	$res = isset($module_map) && is_array($module_map) ? array_keys($module_map) : array();
	$com = $res;
} else {
	$com = explode(' ',$com_string);
	foreach ($com as $j)
		if (isset($module_map[$j]))
			$res[] = $j;
	$com = $res;
	foreach (array_keys($module_map) as $module)
		if (!in_array($module,$res))
			$res[] = $module;
}

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<input type="hidden" name="id" value="0" />';
echo '<input type="hidden" name="addressbook_id" value="'.(isset($_REQUEST['addressbook_id']) ? $_REQUEST['addressbook_id'] : '').'" />';
echo '<input type="hidden" name="treeview" value="'.(isset($_REQUEST['treeview']) ? $_REQUEST['treeview'] : '').'" />';
echo '<input type="hidden" name="first" value="'.(isset($_REQUEST['first']) ? $_REQUEST['first'] : '').'" />';
echo '<input type="hidden" name="max_rows" value="'.(isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : '').'" />';
echo '<tr><td>';
echo '<table border="0" cellpadding="2" cellspacing="0">';
echo '<tr><td>&nbsp;</td></tr>';
echo '<tr><td><input type="checkbox" name="show_text" value="1" '.($show_text == '1' ? 'checked' : '').'/>'.$pref_show_text.'</td></tr>';
echo '<tr><td>&nbsp;</td></tr>';

$count = count($res);
$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="16" height="12">';
$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

echo '<tr class="TableHead2" height="20">';
echo '<td width="340" colspan="99" nowrap>'.$pref_module_name.'</td></tr>';

require_once($GO_CONFIG->root_path.'lib/tkdlib.php');
$mi = new move_item();

$value = 1;

if ($count > 0)
{
	for ($i = 0; $i < $count; $i++)
	{
		$name = isset($lang_modules[$res[$i]]) ? $lang_modules[$res[$i]] : $res[$i];
		if ( isset($name) )
		{
			$icon = isset($GO_THEME->images['mod_'.$res[$i]]) ? $GO_THEME->images['mod_'.$res[$i]] : $GO_THEME->images['unknown'];
			if ($value > 1)		
			{
				echo '<td>'.($value==2?$move_space:'<a href="'.$mi->change($value-1,$value-2).'">'.$move_task_up.'</a>').' <a href="'.$mi->change($value-1,$value).'">'.$move_task_dn.'</a></td>';			
    			echo '</tr>';
    			echo $spliter;			
			}
			
			echo '<tr>';
	
			if (in_array($res[$i],$com))
				echo $mi->item($value, '<table border=0><tr><td><img src="'.$icon.'"/><td valign="center">'.$name.'</td></tr></table>', $res[$i],true);
			else
				echo $mi->item($value, '<table border=0><tr><td><img src="'.$icon.'"/><td valign="center">'.$name.'</td></tr></table>', $res[$i]);
			
			$value++;			
		}
	}
	if ($value > 2)
	{
		echo '<td><a href="'.$mi->change($value-1,$value-2).'">'.$move_task_up.'</a></td>';			
    	echo '</tr>';
    	echo $spliter;			
	}
}
else {
  echo '<tr><td colspan="99">&nbsp;</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '<input type="hidden" name="count_child" value="" />';

echo '</table></td></tr></table>';
echo '<br />';

echo '<br><br>&nbsp;&nbsp;';
$button = new button($cmdSave, "javascript:save_modules('false')");
?>

<script type="text/javascript">
function default_status()
{
    document.forms[0].task.value = 'default_status';
    document.forms[0].submit();
}
function save_modules(close)
{
  document.forms[0].task.value='save';
  document.forms[0].close.value=close;
  document.forms[0].submit();
}
</script>
