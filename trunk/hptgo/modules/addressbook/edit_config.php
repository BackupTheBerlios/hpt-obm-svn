<?php
	$task = $_REQUEST['task'];
	$page = $_REQUEST['active_tab'];
	
	$user = $GO_SECURITY->user_id;

	$db = new db();
	
	switch ($page)
	{
		case $constCompaniesPage:$res = $db->metadata("ab_companies",false); break;
		case $constMembersPage:$res = $db->metadata("users",false); break;
		default:
			$page = $$constContactsPage;		
			$res = $db->metadata("ab_contacts",false); 
	}
	
switch ($task) {
  case 'default_status':
	$db->query("DELETE FROM ab_config WHERE page = '$page' AND user_id = '$user'");  
  break;
  case 'ok_status':
		$com = $_REQUEST['com'];
		$order_all = $_REQUEST['order_all'];
		
		if ($_REQUEST['com'] > 0)
			$com = implode(",", $_REQUEST['com']);
		if ($_REQUEST['order_all'] > 0)
			$order_all = implode(",", $_REQUEST['order_all']);
		
		$db->query("DELETE FROM ab_config WHERE page = '$page' AND user_id = '$user'");
		if (!empty($order_all))
			$db->query("INSERT INTO ab_config VALUES('$page', '$user', '$com', '$order_all')");
	case 'return_to':		
		require('../../lib/tkdlib.php');
		switch ($page)
		{
			case $constContactsPage:
				goURL("index.php?post_action=browse&addressbooks=".$user);
			break;
			case $constCompaniesPage:
				goURL("index.php?post_action=companies&addressbooks=".$user);
			break;
			case $constMembersPage:
				goURL("index.php?post_action=members&addressbooks=".$user);
			break;
		}
}

$db->query("SELECT order_fields, order_all FROM ab_config WHERE page = '$page' AND user_id = '$user'");

if ($db->next_record())
{
	$com = explode(",",$db->f('order_fields'));
	$s = $db->f('order_all');

	if (!empty($s))
		$order_all = explode(",",$db->f('order_all'));
}

if ($db->num_rows() == 0)	
{
	switch ($page)
	{
		case $constContactsPage : 
			$com[] = "email";
			$com[] = "home_phone";
			$com[] = "work_phone";
		break;
		case $constCompaniesPage :
			$com[] = "city";
			$com[] = "email";
			$com[] = "homepage";
			$com[] = "phone";
		break;
		case $constMembersPage :
			$com[] = "email";
			$com[] = "company";
		break;
	}
}

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<input type="hidden" name="id" value="0" />';
echo '<tr><td>';
echo '<table border="0" cellpadding="2" cellspacing="0">';
echo '<tr><td>&nbsp;</td></tr>';

$count = count($res);

$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="16" height="12">';
$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

echo '<tr class="TableHead2" height="20">';
echo '<td align="center" width="20">#</td>';
echo '<td width="320" colspan="99" nowrap>'.$ab_category_name.'</td></tr>';

require "../../lib/tkdlib.php";
$mi = new move_item();

$value = 1;

if (count($order_all) > 0)
{
	$iorder = 0;
	for ($i = 0; $i < count($order_all); $i++)
	{
		if ($value > 1)		
		{
			echo '<td>'.($value==2?$move_space:'<a href="'.$mi->change($value-1,$value-2).'">'.$move_task_up.'</a>').' <a href="'.$mi->change($value-1,$value).'">'.$move_task_dn.'</a></td>';			
    		echo '</tr>';
    		echo $spliter;			
		}
	
//-----------------------------------------------	
			echo '<tr>';
    		echo '<td align="center"><b>'.$value.'</b></td>';

			if ($order_all[$i] == $com[$iorder])
			{
				echo $mi->item($value, $strCom[$order_all[$i]], $order_all[$i], true);
				$iorder++;
			}
			else
				echo $mi->item($value, $strCom[$order_all[$i]], $order_all[$i], false);
			
		$value++;			
	}
	if ($value > 2)
	{
		echo '<td><a href="'.$mi->change($value-1,$value-2).'">'.$move_task_up.'</a></td>';			
    	echo '</tr>';
    	echo $spliter;			
	}
}
else 
if ($count > 0)
{
	for ($i = 0; $i < $count; $i++)
	{
//echo "<script language=javascript>alert('".$res[$i]['name']."--".$strCom[$res[$i]['name']]."');<script>";	
		if ( isset($strCom[$res[$i]['name']]) )// && !$haveCom[$res[$i]['name']] )
		{
			if ($value > 1)		
			{
				echo '<td>'.($value==2?$move_space:'<a href="'.$mi->change($value-1,$value-2).'">'.$move_task_up.'</a>').' <a href="'.$mi->change($value-1,$value).'">'.$move_task_dn.'</a></td>';			
    			echo '</tr>';
    			echo $spliter;			
			}
			
			echo '<tr>';
    		echo '<td align="center"><b>'.$value.'</b></td>';
	
			if (in_array($res[$i]['name'],$com))
				echo $mi->item($value, $strCom[$res[$i]['name']], $res[$i]['name'],true);
			else
				echo $mi->item($value, $strCom[$res[$i]['name']], $res[$i]['name']);

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
$button = new button($cmdOk, 'javascript:ok_status()');
echo '&nbsp;&nbsp;';
//$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
$button = new button($cmdClose, "javascript:return_to()");
echo '&nbsp;&nbsp;';
$button = new button($cmdDefault, 'javascript:default_status()');
?>

<script type="text/javascript">
function default_status()
{
    document.forms[0].task.value = 'default_status';
    document.forms[0].submit();
}

function ok_status()
{
    document.forms[0].task.value = 'ok_status';
    document.forms[0].submit();
}

function return_to()
{
	document.forms[0].task.value = 'return_to';
	document.forms[0].submit();
/*    switch (page)
	{
		case 1:
			document.location = "index.php?post_action=companies&addressbooks=" + user;
		break;
		case 2:
			document.location = "index.php?post_action=members&addressbooks=" + user;
		break;		
		case default:
			document.location = "index.php?post_action=browse&addressbooks=" + user;
	}
*/
}
</script>
