<?php
//$projects_module_url = isset($projects_module_url) ? $projects_module_url : $GO_MODULES->url;

//$cat_id = $_REQUEST['cat_id'];
//if (!isset($cat_id)) $cat_id = 0;

$task = $_REQUEST['task'];
$company_id = $_REQUEST['company_id'];
$db = new db();

switch ($task) {
  case 'ok_status':
  	$category_id = $_REQUEST['category_id'];
	$company_id = $_REQUEST['company_id'];
	
	$db->query("DELETE FROM ab_cate_companies WHERE company_id='$company_id'");	
	for ($i=0; $i < count($category_id); $i++)
		$db->query("INSERT INTO ab_cate_companies VALUES('$company_id','$category_id[$i]')");
  break;
}

$db->query("SELECT category_id FROM ab_cate_companies WHERE company_id = $company_id");
if ($db->num_rows() > 0)
	while ($db->next_record())
		$arrCategory[]=$db->f("category_id");

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<input type="hidden" name="company_id" value="'.$_POST['company_id'].'" />';
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
  echo $spliter;
  while ($db->next_record())
  {
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
	echo "<td></td>";  
	$delitem = sprintf( ($count_child > 0)? $ab_ConfirmDeleteStatus2 : $ab_ConfirmDeleteStatus, $name);	
    echo '</tr>';

    echo $spliter;
	
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
			
			$checked = '';
			if (isset($arrCategory) && in_array("$id",$arrCategory))
				$checked = 'checked';

			echo "<td><input type='checkbox' name='category_id[]' value='$id' $checked ></td>";			
    		if ($count == 1)
      			echo '<td width="36" align="center">&nbsp;</a></td>';
    		echo '</tr>';

    		echo $spliter;
		}
	}
	
	$value++;
  }
}
else {
  echo '<tr><td colspan="99">&nbsp;</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '</table></td></tr></table>';
echo '<br />';

echo '<br><br>&nbsp;&nbsp;';
$button = new button($cmdCapnhat, 'javascript:ok_status()');
echo '&nbsp;&nbsp;';
$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
?>

<script type="text/javascript">

function ok_status()
{
	document.forms[0].task.value = 'ok_status';
    document.forms[0].submit();
}
</script>
