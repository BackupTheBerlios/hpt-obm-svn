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

if (!$GO_MODULES->write_permissions) {
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}

$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
$editor = '<img src="'.$GO_THEME->images['edit'].'" border="0">';
if (isset($_REQUEST['cat_id'])) $cat_id = $_REQUEST['cat_id'];

if (isset($_REQUEST['task'])) $task = $_REQUEST['task'];
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
    $db->query("SELECT * FROM pmProjects WHERE cat_id='$cat_id'");
    if ($db->num_rows() == 0) {
      $db->query("DELETE FROM pmStatus WHERE cat_id='$cat_id'");
      $db->query("DELETE FROM pmCatalog WHERE id='$cat_id'");
      $cat_id = 0;
    }
    else {
      echo '<p class="Error">'.$pm_CannotDelete.'</p>';
    }
    break;
  case 'update_catalog':
    $catalog_name = $_REQUEST['catalog_name'];
    if (isset($catalog_name) && $catalog_name != '')
      $db->query("UPDATE pmCatalog SET name='$catalog_name' WHERE id='$cat_id'");
    break;
  case 'change_status_order':
    $status_value = $_REQUEST['status_value'];
    if ($status_value > 1) {
      $status_change = $_REQUEST['status_change'];
      $related_status = $status_value + $status_change;
      $db->query("UPDATE pmStatus SET value='999' WHERE value='$status_value'");
      $db->query("UPDATE pmStatus SET value='$status_value' WHERE value='$related_status'");
      $db->query("UPDATE pmStatus SET value='$related_status' WHERE value='999'");
    }
    break;
  case 'delete_status':
    $status_value = $_REQUEST['status_value'];
    if ($status_value > 1) {
      $db->query("DELETE FROM pmStatus ".
                 "WHERE cat_id='$cat_id' AND value='$status_value'");
      $db->query("UPDATE pmStatus SET value=value-1 ".
                 "WHERE cat_id='$cat_id' AND value>'$status_value'");
    }
    break;
  case 'update_status':
    $status_value = $_REQUEST['status_value'];
    if ($status_value > 1) {
      $status_name = $_REQUEST['status_name'];
      $db->query("UPDATE pmStatus SET name='$status_name' ".
                 "WHERE cat_id='$cat_id' AND value='$status_value'");
    }
    break;
  case 'add_status':
    $status_name = $_REQUEST['status_name'];
    $status_value = $_REQUEST['status_value'] + 0;
    if (isset($status_name)) $status_name = trim($status_name);
    if ($status_value > 1) {
      $db->query("UPDATE pmStatus SET value=value+1 ".
                 "WHERE cat_id='$cat_id' AND value>='$status_value'");
      $db->query("INSERT INTO pmStatus VALUES('$cat_id', '$status_value', '$status_name')");
    }
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
  if (!isset($cat_id) || $cat_id == 0) $cat_id = $db->f('id');
  $catalog->add_value($db->f('id'), $db->f('name'));
  if ($cat_id == $db->f('id'))
    $catalog_name = $db->f('name');
}
echo '<input type="hidden" name="catalog_name" value="'.$catalog_name.'" />';
$catalog->print_dropbox('cat_id', $cat_id, 'onchange="javascript:frm.submit()"', false, '10', '200');
echo '</td>';
echo '<td><a href="javascript:edit_category()" title="'.$pm_EditCategory.'">'.$editor.'</a></td>';
echo '<td><a href="javascript:delete_category()" title="'.$pm_DeleteCategory.'">'.$trash.'</a></td>';
echo '</tr></table>';
echo '<table border="0" cellpadding="2" cellspacing="0" width="400">';

if ($count > 0) {
  $db->query("SELECT * FROM pmStatus WHERE cat_id='$cat_id' ORDER BY value");
  $count = $db->num_rows();
}

$pm_insert_status = 'Insert new status';

$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="16" height="12">';
$insert_btn = '<td align="center">'.
              '<span id="insert_[id]">'.
                 '<a href="javascript:insert_status([id])">'.
                 '<img src="'.$GO_THEME->images['add_project_task'].'"'.
                 ' border="0" height="8" width="8" title="'.$pm_insert_status.'" /></a>'.
              '</span></td>';
$spliter_cell = '<td colspan="99" height="1">'.
                '<img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" />'.
                '</td>';
$spliter_line = '<tr><td>&nbsp;</td>'.$spliter_cell.'</tr>';
$spliter = '<tr>'.$insert_btn.$spliter_cell.'</tr>';
$empty_row = '<tr id="row_[id]" style="visibility: hidden; display: none;">'.
             '<td align="center"><a href="javascript:submit_status()">'.
             '<img src="'.$GO_THEME->images['task_pre_save'].'" border="0" title="Save" />'.
             '</td>'.
             '<td colspan="97"><span id="status_new_[id]">&nbsp;</span></td>'.
             '<td align="center"><a href="javascript:submit_status()">'.
             '<img src="'.$GO_THEME->images['task_pre_save'].'" border="0" title="Save" />'.
             '</td>'.
             '<td align="center">'.
             '<a href="javascript:hide_editor()">'.
             '<img src="'.$GO_THEME->images['task_edit_cancel'].'" border="0" title="Cancel" />'.
             '</a></td>'.
             '</td>'.
             '</tr>'.
             '<tr id="spliter_[id]" style="visibility: hidden; display: none;">'.
             '<td>&nbsp;</td>'.$spliter_cell.
             '</tr>';

echo '<tr class="TableHead2" height="20">';
echo '<td align="center" width="20">#</td>';
echo '<td width="320" colspan="99" nowrap>'.$strName.'</td></tr>';

echo '<tr><td align="center">1</td><td colspan="99">'.$pm_status_begin.'</td></tr>';
echo str_replace('[id]', 2, $spliter);
echo str_replace('[id]', 2, $empty_row);
if ($count > 0)
{
  while ($db->next_record()) {
    $value = $db->f('value');
    $name = $db->f('name');
    $delitem = sprintf($pm_ConfirmDeleteStatus, $name);
    $delitem_hint = "$strDeleteItem '$name'";
    echo '<tr>';
    echo '<td align="center">'.$value.'</td>';
    echo '<td colspan="97" nowrap><span id="status_'.$value.'">'.
         "<a href='javascript:set_edit(\"$name\", $value)'>$name</a>".
         '</span></td>';
    if ($count == 1)
      $move_up_link = $move_dn_link = '';
    elseif ($value == 2) {
      $move_up_link = $move_space;
      $move_dn_link = '<a href="javascript:move_status('.$value.', 1)">'.$move_task_dn.'</a>';
    }
    elseif ($value == $count+1) {
      $move_up_link = '<a href="javascript:move_status('.$value.', -1)">'.$move_task_up.'</a>';
      $move_dn_link = $move_space;
    }
    else {
      $move_up_link = '<a href="javascript:move_status('.$value.', -1)">'.$move_task_up.'</a>';
      $move_dn_link = '<a href="javascript:move_status('.$value.', 1)">'.$move_task_dn.'</a>';
    }
    echo '<td width="36" align="center">'.$move_up_link.'&nbsp;'.$move_dn_link.'</td>';
    echo "<td><a href='javascript:delete_status(\"$delitem\",$value)'>$trash</a></td>";
    echo '</tr>';

    echo str_replace('[id]', $value + 1, $spliter);
    echo str_replace('[id]', $value + 1, $empty_row);
  }
}
echo '<tr><td>&nbsp;</td><td colspan="99">'.$pm_status_done.'</td></tr>';
echo $spliter_line;
echo '<tr><td>&nbsp;</td><td colspan="99">'.$pm_status_drop.'</td></tr>';
echo $spliter_line;
echo '<input type="hidden" name="last_value" value="'.((isset($value) ? $value : 0)+1).'" />';
echo '</table></td></tr></table>';
echo '<input id="status_edit" type="text" class="textbox" '.
     'style="position: absolute; visibility: hidden; display: none; width: 300px;" '.
     'name="status_name" value="" maxlength="50" /><br /><br />';
?>

<script type="text/javascript">
var agent = navigator.userAgent.toLowerCase();
var is_ie = ((agent.indexOf("msie") != -1) && (agent.indexOf("opera") == -1));
var frm = document.forms[0];

function key_hanlder(e)
{
  if (is_ie) e = window.event;
  switch (e.keyCode) {
    case 13:
      submit_status();
      break;
    case 27:
      hide_editor();
      break;
  }
  return true;
}

document.onkeypress = key_hanlder;

function move_status(status_value, action)
{
  frm.task.value = 'change_status_order';
  frm.status_value.value = status_value;
  frm.status_change.value = action;
  frm.submit();
}

function delete_status(status_name, status_value)
{
  if (confirm(status_name)) {
    frm.task.value = 'delete_status';
    frm.status_value.value = status_value;
    frm.submit();
  }
}

var row = null;
var spliter = null;
var insert_btn = null;
var status_edit = document.getElementById('status_edit');

function get_position(element)
{
  var elem = element, tagname = "", x = 0, y = 0;

  while ((typeof(elem) == "object") && (typeof(elem.tagName) != "undefined"))
  {
    y += elem.offsetTop;
    x += elem.offsetLeft;
    tagname = elem.tagName.toUpperCase();

    if (tagname == "BODY") elem = 0;
    if (typeof(elem) == "object")
      if (typeof(elem.offsetParent) == "object")
        elem=elem.offsetParent;
  }

  position = new Object();
  position.x = x;
  position.y = y;
  return position;
}

function show_editor(container, editor, value)
{
  p = get_position(container);
  editor.style.top = p.y + "px";
  editor.style.left = p.x + "px";
  editor.style.display = '';
  editor.style.visibility = 'visible';
  editor.value = value;
  editor.focus();
  if (value != null && value != '')
    editor.select();
}

function hide_editor()
{
  task_action = null;
  status_edit.blur();
  status_edit.style.display = 'none';
  status_edit.style.visibility = 'hidden';
  status_edit.value = '';
  if (insert_btn) {
    insert_btn.style.visibility = 'visible';
    insert_btn = null;
    row.style.display = 'none';
    row.style.visibility = 'hidden';
    row = null;
    spliter.style.display = 'none';
    spliter.style.visibility = 'hidden';
    spliter = null;
  }
}

function insert_status(p)
{
  hide_editor();

  frm.status_value.value = p;
  frm.task.value = 'add_status';

  insert_btn = document.getElementById("insert_"+p);
  insert_btn.style.visibility = 'hidden';

  row = document.getElementById("row_"+p);
  row.style.display = '';
  row.style.visibility = 'visible';

  spliter = document.getElementById("spliter_"+p);
  spliter.style.display = '';
  spliter.style.visibility = 'visible';

  show_editor(document.getElementById("status_new_"+p), status_edit, "", null, 0);
}

function set_edit(name, p)
{
  hide_editor();

  frm.status_value.value = p;
  frm.task.value = 'update_status';

  show_editor(document.getElementById("status_"+p), status_edit, name, null, 0);
}

function submit_status()
{
  if (frm.status_name.value != '')
    frm.submit();
}

function edit_category()
{
  name = prompt("<?php echo $pm_EditCategory; ?>", frm.catalog_name.value);
  if (name != '') {
    frm.task.value = 'update_catalog';
    frm.catalog_name.value = name;
    frm.submit();
  }
}

function add_category()
{
  name = prompt("<?php echo $pm_AddCategory; ?>", "");
  if (name != '') {
    frm.task.value = 'add_catalog';
    frm.catalog_name.value = name;
    frm.submit();
  }
}

function delete_category()
{
  if (confirm("<?php echo $pm_ConfirmDeleteCategory;?>" + "'" + frm.catalog_name.value + "' ?")) {
    frm.task.value = 'delete_catalog';
    frm.submit();
  }
}

</script>
