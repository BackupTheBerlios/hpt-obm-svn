<?php
/*
   Copyright HPT Commerce 2004
   Author: Tran Kien Duc <ductk@hptvietnam.com.vn>
   Version: 1.0 Release date: 13 August 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

//$projects_module_url = isset($projects_module_url) ? $projects_module_url : $GO_MODULES->url;

$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
/*
$cat_id = $_REQUEST['cat_id'];
if (!isset($cat_id)) $cat_id = 0;
*/

$task = $_REQUEST['task'];
$db = new db();
switch ($task) {
   case 'delete_status':
	  $id = $_REQUEST['id'];
	  $db->query("SELECT category_id FROM ab_categories WHERE parent_id = '$id'");
	  if ($db->num_rows() > 0)
	  	while ($db->next_record())
		{
			if (!isset($db_child))
				$db_child = new db();
			$db_child->query("DELETE FROM ab_cate_companies WHERE category_id = ".$db->f("category_id"));
		}

	  $db->query("DELETE FROM ab_categories WHERE parent_id = '$id' OR category_id = '$id'");		
	  $db->query("DELETE FROM ab_cate_companies WHERE category_id = '$id'");
    break;
    case 'update_status':
	  $parent = $_REQUEST['parent'];
	  $id = $_REQUEST['id'];
      $category = $_REQUEST['category'];
	  
      $db->query("UPDATE ab_categories SET category='$category', parent_id = $parent ".
                 "WHERE category_id=$id");
    break;
  case 'add_status':
	$parent = $_REQUEST['parent'];
    $category = $_REQUEST['category'];
    $db->query("INSERT INTO ab_categories(category,parent_id) VALUES('$category', '$parent')");
    break;
}

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<input type="hidden" name="id" value="0" />';
echo '<tr><td>';
echo '<table border="0" cellpadding="2" cellspacing="0">';
echo '<tr><td>&nbsp;</td></tr>';

  $db->query('SELECT * FROM ab_categories WHERE parent_id = "0" ORDER BY category');
  $count = $db->num_rows();

$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="16" height="12">';
$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

echo '<tr class="TableHead2" height="20">';
echo '<td align="center" width="20">#</td>';
echo '<td width="320" colspan="99" nowrap>'.$ab_category_name.'</td></tr>';

$value = 1;
if ($count > 0)
{
//  echo $spliter;
  while ($db->next_record())
  {
//    $value = $db->f('parent_id');
	$name = $db->f('category');
	$id = $db->f('category_id');
	$parent = $db->f('parent_id');	
    $delitem_hint = "$strDeleteItem '$name'";

//------------------------
	if (!isset($db_child))
		$db_child = new db();
	$db_child->query("SELECT * FROM ab_categories WHERE parent_id = '$id' ORDER BY category");
	$count_child = $db_child->num_rows();
//------------------------	  
	
    echo '<tr class="HiLi">';
    echo '<td align="center"><b>'.$value.'</b></td>';
    echo "<td colspan=\"97\" nowrap><a href='javascript:set_edit($id, \"$name\", $parent, $count_child)'><b>$name</b></a></td>";
    if ($count == 1)
      echo '<td width="36" align="center">&nbsp;</a></td>';
	$delitem = sprintf( ($count_child > 0)? $ab_ConfirmDeleteStatus2 : $ab_ConfirmDeleteStatus, $name);	
    echo "<td><a href='javascript:delete_status(\"".div_confirm_id($delitem)."\",".$id.")'>$trash</a></td>";
    echo '</tr>';

//    echo $spliter;
	
	if ($count_child > 0)
	{
		$val_child = 0;
		while ($db_child->next_record())
		{
			$val_child++;
			$name = $db_child->f('category');
			$id = $db_child->f('category_id');
			$parent = $db_child->f('parent_id');	

    		$delitem = sprintf($ab_ConfirmDeleteStatus, $name);
    		$delitem_hint = "$strDeleteItem '$name'";
			
    		echo '<tr>';
    		echo '<td align="center"></td>';
    		echo "<td colspan=\"97\" nowrap><a href='javascript:set_edit($id, \"$name\", $value, 0)'>".$val_child."&nbsp;&nbsp;&nbsp;$name</a></td>";
    		if ($count == 1)
      			echo '<td width="36" align="center">&nbsp;</a></td>';
    		echo "<td><a href='javascript:delete_status(\"".div_confirm_id($delitem)."\",$id)'>$trash</a></td>";
    		echo '</tr>';

//    		echo $spliter;
		}
	}
	
	$value++;
  }
}
else {
  echo '<tr><td colspan="99">&nbsp;</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '<input type="hidden" name="count_child" value="" />';
echo $spliter;
echo '</table></td></tr></table>';
echo '<br />';
/*echo '<table width="100%"  border="0" cellspacing="0" cellpadding="0">';
echo '<tr><td>Danh mục :</td><td>Danh mục cha :</td></tr>';
echo '<tr><td><input type="text" class="textbox" name="category" value="" maxlength="50" /></td>'.
	 '<td>&nbsp;</td></tr>
</table>*/

echo "$ab_category : <br />";
echo '<input type="text" class="textbox" name="category" value="" maxlength="50" />&nbsp;&nbsp;';
//style="width: 350px;" 

$db->query('SELECT * FROM ab_categories WHERE parent_id = "0" ORDER BY category');
$count = $db->num_rows();
$catalog = new dropbox();
$catalog_name = '';
$catalog->add_value('0', $ab_TOP);
while ($db->next_record()) {
  if ($cat_id == 0) $cat_id = $db->f('category_id');
  $catalog->add_value($db->f('category_id'), $db->f('category'));
  if ($cat_id == $db->f('category_id'))
    $catalog_name = $db->f('category');
}

$catalog->print_dropbox('parent', $selected,
  '', false, '10', '200');

echo '<br><br>&nbsp;&nbsp;';
$button = new button($cmdAdd, 'javascript:add_status()');
echo '&nbsp;&nbsp;';
$button = new button($cmdCapnhat, 'javascript:update_status()');
echo '&nbsp;&nbsp;';
$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
?>

<script type="text/javascript">

function delete_status(msg, id)
{
  	if (div_confirm(msg)) {
    	document.forms[0].task.value = 'delete_status';
	    document.forms[0].id.value = id;
    	document.forms[0].submit();
	}
}

function set_edit(id, name, parent, count_child)
{
  document.forms[0].category.value = name;
  document.forms[0].id.value = id;
  document.forms[0].parent.selectedIndex = parent;
  if (count_child > 0)
  	document.forms[0].parent.disabled = true;
  else
  	document.forms[0].parent.disabled = false;
}

function add_status()
{
  if (document.forms[0].category.value != '') {
    document.forms[0].task.value = 'add_status';
    document.forms[0].submit();
  }
}

function update_status()
{
  if (document.forms[0].category.value != '') {
    document.forms[0].task.value = 'update_status';
	document.forms[0].parent.disabled = false;
    document.forms[0].submit();
  }
}

</script>
