<?php
/*
   Copyright HPT Commerce 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 08 August 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

$projects_module_url = isset($projects_module_url) ? $projects_module_url : $GO_MODULES->url;
echo '<a href="javascript:add_category()" class="normal">'.$pm_new_category.'</a><br /><br />';

if (!$GO_SECURITY->has_admin_permission($GO_SECURITY->user_id)) {
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}

$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$editor = '<img src="'.$GO_THEME->images['edit'].'" border="0">';
$cat_id = $_REQUEST['cat_id'];
if (!isset($cat_id)) $cat_id = 0;

$task = $_REQUEST['task'];
$db = new db();

switch ($task) {
  case 'add_catalog':
    $catalog_name = $_REQUEST['catalog_name'];
    if (isset($catalog_name) && $catalog_name != '') {
      $db->query("SELECT * FROM pmCatalog WHERE name='$catalog_name'");
      if ($db->num_rows() > 0)
        echo sprintf($pm_CategoryExists, $catalog_name);
      else
        $db->query("INSERT INTO pmCatalog(id, name) VALUES('', '$catalog_name')");
    }
    break;
  case 'delete_catalog':
    $db->query("SELECT * FROM pmProjects WHERE cat_id=$cat_id");
    if ($db->num_rows() == 0) {
      $db->query("DELETE FROM pmStatus WHERE cat_id=$cat_id");
      $db->query("DELETE FROM pmCatalog WHERE id=$cat_id");
      $cat_id = 0;
    }
    else {
      echo '<p class="Error">'.$pm_CannotDelete.'</p>';
    }
    break;
  case 'update_catalog':
    $catalog_name = $_REQUEST['catalog_name'];
    if (isset($catalog_name) && $catalog_name != '')
      $db->query("UPDATE pmCatalog SET name='$catalog_name' WHERE id=$cat_id");
    break;
  case 'change_status_order':
    $status_value = $_REQUEST['status_value'];
    if ($status_value > 1) {
      $status_change = $_REQUEST['status_change'];
      $related_status = $status_value + $status_change;
      $db->query('UPDATE pmStatus SET value=999 WHERE value='.$status_value);
      $db->query('UPDATE pmStatus SET value='.$status_value.' WHERE value='.$related_status);
      $db->query('UPDATE pmStatus SET value='.$related_status.' WHERE value=999');
    }
    break;
  case 'delete_status':
    $status_value = $_REQUEST['status_value'];
    if ($status_value > 1) {
      $db->query("DELETE FROM pmStatus ".
                 "WHERE cat_id=$cat_id AND value=$status_value");
      $status_value = $status_value + 1;
      $db->query("UPDATE pmStatus SET value=value-1 ".
                 "WHERE cat_id=$cat_id AND value=$status_value");
    }
    break;
  case 'update_status':
    $status_value = $_REQUEST['status_value'];
    if ($status_value > 1) {
      $status_name = $_REQUEST['status_name'];
      $db->query("UPDATE pmStatus SET name='$status_name' ".
                 "WHERE cat_id=$cat_id AND value=$status_value");
    }
    break;
  case 'add_status':
    $last_value = $_REQUEST['last_value'];
    $status_name = $_REQUEST['status_name'];
    $db->query("INSERT INTO pmStatus VALUES($cat_id, $last_value, '$status_name')");
    break;
}

echo '<table border="0" cellspacing="0" cellpadding="1" width="100%">';
echo '<tr><td><table border="0" cellpadding="2" cellspacing="0"><tr><td colspan="2">'.$pm_category.':</td><td colspan="5">';
echo '<input type="hidden" name="post_action" value="category" />';
echo '<input type="hidden" name="status_value" value="0" />';
echo '<input type="hidden" name="status_change" value="0" />';

$db->query('SELECT * FROM pmCatalog');
$count = $db->num_rows();
$catalog = new dropbox();
$catalog_name = '';
while ($db->next_record()) {
  if ($cat_id == 0) $cat_id = $db->f('id');
  $catalog->add_value($db->f('id'), $db->f('name'));
  if ($cat_id == $db->f('id'))
    $catalog_name = $db->f('name');
}
echo '<input type="hidden" name="catalog_name" value="'.$catalog_name.'" />';
$catalog->print_dropbox('cat_id', $cat_id,
  'onchange="javascript:document.forms[0].submit()"', false, '10', '200');
echo '</td>';
echo '<td><a href="javascript:edit_category()" title="'.$pm_EditCategory.'">'.$editor.'</a></td>';
echo "<td><a href=\"javascript:delete_category('$pm_ConfirmDeleteCategory')\" title=\"$pm_DeleteCategory\">$trash</a></td>";
echo '</tr></table>';
echo '<table border="0" cellpadding="2" cellspacing="0">';

if ($count > 0) {
  $db->query('SELECT * FROM pmStatus WHERE cat_id='.$cat_id.' ORDER BY value');
  $count = $db->num_rows();
}

$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="16" height="12">';
$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

echo '<tr class="TableHead2" height="20">';
echo '<td align="center" width="20">#</td>';
echo '<td width="320" colspan="99" nowrap>'.$strName.'</td></tr>';

$value = 1;
if ($count > 0)
{
  echo '<tr><td align="center">1</td><td colspan="99">'.$pm_status_begin.'</td></tr>';
  echo $spliter;
  while ($db->next_record())
  {
    $value = $db->f('value');
    $name = $db->f('name');
    $delitem = sprintf($pm_ConfirmDeleteStatus, $name);
    $delitem_hint = "$strDeleteItem '$name'";
    echo '<tr>';
    echo '<td align="center">'.$value.'</td>';
    echo "<td colspan=\"97\" nowrap><a href='javascript:set_edit(\"$name\", $value)'>$name</a></td>";
    if ($count == 1)
      echo '<td width="36" align="center">&nbsp;</a></td>';
    elseif ($value == 2)
      echo '<td width="36" align="center">'.$move_space.'<a href="javascript:move_status('.$value.', 1)" title="'.$delitem_hint.'">'.$move_task_dn.'</a></td>';
    elseif ($value == $count+1)
      echo '<td width="36" align="center"><a href="javascript:move_status('.$value.', -1)">'.$move_task_up.'</a>'.$move_space.'</td>';
    else
      echo '<td width="36" align="center"><a href="javascript:move_status('.$value.', -1)">'.$move_task_up.' <a href="javascript:move_status('.$value.', 1)">'.$move_task_dn.'</td>';
    echo "<td><a href='javascript:delete_status(\"$delitem\",$value)'>$trash</a></td>";
    echo '</tr>';

    echo $spliter;
  }
  echo '<tr><td>&nbsp;</td><td colspan="99">'.$pm_status_done.'</td></tr>';
  echo $spliter;
  echo '<tr><td>&nbsp;</td><td colspan="99">'.$pm_status_drop.'</td></tr>';
  echo $spliter;
}
else {
  echo '<tr><td colspan="99">&nbsp;</td></tr>';
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}
echo '<input type="hidden" name="last_value" value="'.($value+1).'" />';
echo '</table></td></tr></table>';
echo '<br />';
echo 'Trạng thái:<br />';
echo '<input type="text" class="textbox" style="width: 350px;" name="status_name" value="" maxlength="50" /><br /><br />&nbsp;&nbsp;';
$button = new button('Add', 'javascript:add_status()');
echo '&nbsp;&nbsp;';
$button = new button('Update', 'javascript:update_status()');
?>

<script type="text/javascript">

function move_status(status_value, action)
{
  document.forms[0].task.value = 'change_status_order';
  document.forms[0].status_value.value = status_value;
  document.forms[0].status_change.value = action;
  document.forms[0].submit();
}

function delete_status(status_name, status_value)
{
  if (confirm(status_name)) {
    document.forms[0].task.value = 'delete_status';
    document.forms[0].status_value.value = status_value;
    document.forms[0].submit();
  }
}

function set_edit(name, status_value)
{
  document.forms[0].status_name.value = name;
  document.forms[0].status_value.value = status_value;
}

function add_status()
{
  if (document.forms[0].status_name.value != '') {
    document.forms[0].task.value = 'add_status';
    document.forms[0].submit();
  }
}

function update_status()
{
  if (document.forms[0].status_name.value != '') {
    document.forms[0].task.value = 'update_status';
    document.forms[0].submit();
  }
}

function edit_category()
{
  name = prompt("<?php echo $pm_EditCategory; ?>", document.forms[0].catalog_name.value);
  if (name != '') {
    document.forms[0].task.value = 'update_catalog';
    document.forms[0].catalog_name.value = name;
    document.forms[0].submit();
  }
}

function add_category()
{
  name = prompt("<?php echo $pm_AddCategory; ?>", "");
  if (name != '') {
    document.forms[0].task.value = 'add_catalog';
    document.forms[0].catalog_name.value = name;
    document.forms[0].submit();
  }
}

function delete_category(msg)
{
  if (confirm(msg + "'" + document.forms[0].catalog_name.value + "' ?")) {
    document.forms[0].task.value = 'delete_catalog';
    document.forms[0].submit();
  }
}

</script>
