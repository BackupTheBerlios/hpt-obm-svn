<?php
/*
   Copyright HPT Corporation 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 27 September 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require("../../Group-Office.php");

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('projects');
if (!$GO_MODULES->write_permissions) {
  header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
  exit();
}

require($GO_LANGUAGE->get_language_file('projects'));
require($GO_THEME->theme_path."header.inc");

$task_pre_list = '';
if (isset($_REQUEST['template_id']) && isset($_REQUEST['task_id'])) {
  $template_id = $_REQUEST['template_id'];
  $task_id = $_REQUEST['task_id'];
  $db = new db();
  $db->query('SELECT * FROM task_template_'.$template_id.' ORDER BY task_order');
  if ($db->num_rows() > 0) {
    $tasks = array();
    $tasks[0] = array(0, $pm_status_begin, '');
    while ($db->next_record())
      $tasks[$db->f('task_id')] =
        array($db->f('task_order'), $db->f('task_name'), $db->f('task_predecessors'));

    if ($task_id <= 0) {
      $predecessors = $_REQUEST['predecessors'];
      if (!isset($predecessors) || $predecessors=='') {
        $task_pre_list = '';
        $predecessors = array();
      }
      else {
        $task_pre_list = "'".$predecessors."'";
        $predecessors = explode(",", $predecessors);
      }
      display_tasklist($tasks, '', $predecessors);
    }
    else {
      $plist = array();
      $task_pre_list = $tasks[$task_id][2];
      $pre = $predecessors = explode(",", $task_pre_list);
      while (true) {
        $xlist = $plist;
        foreach($pre as $p)
          $plist = array_unique(array_merge($plist, explode(",", $tasks[$p][2])));
        $dlist = array_diff($xlist, $plist);
        if (count($dlist) <= 0) break;
        $pre = $dlist;
      }
      foreach ($plist as $p) unset($tasks[$p]);
      unset($tasks[$task_id]);

      if (count(array_keys($tasks)) > 0) {
        $dlist = array();
        foreach ($tasks as $id => $data) {
          $pre = explode(",", $data[2]);
          if (in_array($task_id, $pre) || count(array_intersect($dlist, $pre)) > 0)
            $dlist[] = $id;
        }

        foreach ($dlist as $p) unset($tasks[$p]);
        display_tasklist($tasks, $task_pre_list, $predecessors);
      }
      $task_pre_list = "'".implode("','", explode(",", $task_pre_list))."'";
    }
  }
}

require($GO_THEME->theme_path."footer.inc");

function display_tasklist($tasks, $task_pre_list, $predecessors)
{
  echo '<style>body { background-color: #CCCCCC; margin: 0px; } a:hover { color: white; }</style>';
  echo '<input type="hidden" id="predecessors" value="'.$task_pre_list.'">';
  echo '<table border="0" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">';
  foreach ($tasks as $id => $data) {
    $chk = in_array($id, $predecessors) ? "checked" : "";
    echo '<tr>'.
           '<td><input type="checkbox" id="cb_'.$id.'" '.$chk.' onclick="toggle('.$id.',false)"/></td>'.
           '<td align="right">'.htmlspecialchars($data[0]).'.</td>'.
           '<td nowrap>&nbsp;<a href="javascript:toggle('.$id.',true)">'.htmlspecialchars($data[1]).'</a></td></td>'.
         '</tr>';
  }
  echo '</table>';
}
?>
<script type="text/javascript">
var pre_list = new Array(<?php echo $task_pre_list; ?>);
function toggle(id,do_toggle)
{
  cb = document.getElementById("cb_"+id);
  if (do_toggle)
  cb.checked = !cb.checked;

  if (cb.checked)
    pre_list[pre_list.length] = id;
  else
  for (i = 0; i < pre_list.length; i++)
    if (pre_list[i] == id) {
      pre_list[i] = null;
      break;
    }
  v = '';
  for (i = 0; i < pre_list.length - 1; i++)
    if (pre_list[i] != null) v += pre_list[i] + ',';
  if (pre_list[i] != null)  v += pre_list[i];
  document.getElementById("predecessors").value = v;
}
</script>
