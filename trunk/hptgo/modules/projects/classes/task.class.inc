<?PHP
define(TASK_WAITING, 3);
define(TASK_ONGOING, 2);
define(TASK_DONE,    1);
define(TASK_BYPASS,  0);

$today = 0;
$max_task_id = 0;
$is_new_project = true;

function update_all_tasks($projects, $project_id) {
  // Disapprove all bypassed tasks if parent tasks are not approved
  $projects->query("SELECT * FROM task".
                   " WHERE task_project_id=$project_id".
                   " AND task_status=".TASK_BYPASS.
                   " ORDER BY task_id");
  $t = array();
  $a = array();

  while ($projects->next_record()) {
    $id = $projects->f('task_id');
    $t[$id] = $projects->f('task_predecessors');
    $a[$id] = $projects->f('task_approved');
  }

  foreach ($t as $id => $pids) {
    if ($pids == '0') continue;

    $projects->query("SELECT task_approved FROM task".
                     " WHERE task_project_id=$project_id AND task_id IN ($pids)");

    $approved = true;
    while ($projects->next_record())
      if ($projects->f('task_approved') == 0) {
        $approved = false;
        break;
      }

    if ($approved) {
      if (!$a[$id])
        $projects->query("UPDATE task SET task_approved=1".
                         " WHERE task_project_id=$project_id AND task_id=$id");
    }
    else {
      if ($a[$id])
        $projects->query("UPDATE task SET task_approved=0".
                         " WHERE task_project_id=$project_id AND task_id=$id");
    }
  }

  unset($t); unset($a);
  // Reset done & ongoing tasks to waiting state if parent tasks are not approved
  $projects->query("SELECT * FROM task".
                   " WHERE task_project_id=$project_id".
                   " AND task_status IN (".TASK_DONE.",".TASK_ONGOING.")".
                   " ORDER BY task_id");
  $t = array();
  $a = array();

  while ($projects->next_record()) {
    $id = $projects->f('task_id');
    $t[$id] = $projects->f('task_predecessors');
  }

  foreach ($t as $id => $pids) {
    $a[] = $id;

    if ($pids == '0') continue;
    $projects->query("SELECT task_approved FROM task".
                     " WHERE task_project_id=$project_id AND task_id IN ($pids)");

    $approved = true;
    while ($projects->next_record())
      if ($projects->f('task_approved') == 0) {
        $approved = false;
        break;
      }

    if (!$approved) {
      $projects->query("UPDATE task SET task_approved=0, task_status=".TASK_WAITING.
                       " WHERE task_project_id=$project_id AND task_id=$id");
    }
  }

  // Set waiting tasks to ongoing if all parent tasks are approved
  unset($t);
  $a = join(",", $a);

  if (!isset($a) || $a == "")
    $a = "";
  else
    $a = " AND task_id NOT IN ($a)";

  $projects->query("SELECT * FROM task".
                   " WHERE task_project_id=$project_id".
                   " AND task_status=".TASK_WAITING. $a .
                   " ORDER BY task_id");
  $t = array();

  while ($projects->next_record()) {
    $id = $projects->f('task_id');
    $t[$id] = $projects->f('task_predecessors');
  }

  foreach ($t as $id => $pids) {
    if ($pids == '0') continue;
    $projects->query("SELECT task_approved FROM task".
                     " WHERE task_project_id=$project_id AND task_id IN ($pids)");
    $approved = true;
    while ($projects->next_record())
      if ($projects->f('task_approved') == 0) {
        $approved = false;
        break;
      }
    if ($approved)
      $projects->query("UPDATE task SET task_status=".TASK_ONGOING.
                       " WHERE task_project_id=$project_id AND task_id=$id");
  }
}

function load_project_task($projects, $project_id) {
  global $project_task_list;

  $tasks = array();
  $project_task_list = array();

  $projects->query("SELECT status, task_template_id FROM pmProjects WHERE id=$project_id");
  if (!$projects->next_record()) {
    $pstate = STATUS_OFFER;
    $template_id = 0;
  }else {
    $pstate = $projects->f('status');
    $template_id = $projects->f('task_template_id');
  }
  

  update_all_tasks($projects, $project_id);

  $projects->query("SELECT * FROM task WHERE task_project_id=$project_id ORDER BY task_id");

  // Copy from Template if first time
  if (!$projects->next_record() && $template_id > 0) {
    $projects->query("INSERT INTO task SELECT $project_id, task_template_$template_id.* ".
                     "FROM task_template_$template_id");
    $projects->query("SELECT * FROM task WHERE task_project_id=$project_id  ORDER BY task_id");
    $projects->next_record();
  }

  // Load all tasks
  while ($template_id >= 0) {
    global $max_task_id, $is_new_project;

    $t = new project_task($projects, $pstate);
    $tasks[$t->id] = $t;
    $project_task_list[$t->id] = &$tasks[$t->id];
    if ($t->id > $max_task_id) $max_task_id = $t->id;
    if ($t->days != 0) $is_new_project = false;
    if (!$projects->next_record()) break;
  }

  return $tasks;
}

class project_task extends db {
  var $project_status = 1;
  var $project_id = 0;
  var $id = 0;
  var $name = "";
  var $pids = "";
  var $level = -1;
  var $status = -1;
  var $person_id = 0;
  var $approved = false;
  var $days = 0;
  var $comments = "";
  var $approved_date;

  var $elapsed = -1;

  var $is_owner = false;
  var $is_manager = false;
  var $is_passed = false;

  var $doc_path = "";

  function project_task($projects, $pstate) {
    global $GO_SECURITY, $GO_USERS, $GO_CONFIG, $responsible_user_id;

    $this->db();
    $this->project_id = $projects->f('task_project_id');
    $this->id = $projects->f('task_id');
    $this->name = $projects->f('task_name');
    $this->pids = $projects->f('task_predecessors');
    $this->level = $projects->f('task_level');
    $this->status = $projects->f('task_status');
    $this->approved = $projects->f('task_approved');
    $this->person_id = $projects->f('task_person_id');
    $this->days = $projects->f('task_time');
    $this->comments = $projects->f('task_comment');
    $this->approved_date = $projects->f('task_approved_date');
    $this->project_status = $pstate;

    if ($this->person_id == 0)
      $this->person_id = $responsible_user_id;

    $this->is_owner = $GO_SECURITY->user_id == $this->person_id;
    $this->is_manager = $GO_SECURITY->user_id == $responsible_user_id;
    $this->is_passed = $this->status <= TASK_DONE;

    $user = $GO_USERS->get_user($responsible_user_id);
    $this->doc_path = $GO_CONFIG->file_storage_path . $user['username'] . '/.projects/' .
      $this->project_id . '/' . $this->id;
  }

  function print_info($member_ids, $member_names, $member_list) {
    global $GO_THEME, $pm_task_status_values, $responsible_user_id, $is_new_project;

    if ($this->level == -1)
      echo "<tr>";
    else
      echo "<tr class=\"TableInside2\">";

    $id = $this->id < 10 ? "0" . $this->id : $this->id;
    echo "<td align=\"center\">$id</td>";
    $this->print_name();
    $this->print_members($member_ids, $member_names, $member_list);
    $pids = $this->pids == '0' ? '&nbsp;' : $this->pids;
    echo "<td>$pids</td>";
    if ($this->is_manager && $is_new_project && ($this->status > TASK_DONE))
    {
      echo "<td><input class=\"textbox\" style=\"width: 30px;\" type=\"text\" name=\"task_time_".$this->id."\" value=\"0\" align=\"right\"></td>";
      echo "<td align=\"center\"> - </td>";
    }else
    {
      $this->get_elapsed();
      $progress = $this->elapsed."/".$this->days;
      if ($this->elapsed > $this->days)
        $progress = "<font color=\"red\"><b>".$progress."</b></font>";
      echo "<td>".$progress."</td>";
      if ($this->status == TASK_ONGOING)
        echo "<td><b>".$pm_task_status_values[$this->status]."</b></td>";
      else
        echo "<td>".$pm_task_status_values[$this->status]."</td>";
    }
    echo "<td align=\"center\">".($this->approved ? "<img border=\"0\" src=\"".$GO_THEME->images['checked']."\"/>" : "&nbsp;")."</td>";
    if ($this->level == -1) {
      $url = $_SERVER['PHP_SELF'] . "?task=show_task_doc".
             "&project_id=".$this->project_id.
             "&task_id=".$this->id."&task_status=show_doc";
      echo "<td align=\"center\"><a href=\"$url\">".
           "<img border=\"0\" src=\"".$GO_THEME->images['doc']."\"/></a></td>";
    }
    else
      echo "<td>&nbsp;</td>";
    echo "</tr>";
  }
  
  function print_name() {
    if (($this->is_manager || $this->is_owner) && $this->project_status != STATUS_DROP) {
      $url = $_SERVER['PHP_SELF'] . "?task=show_task_status".
             "&project_id=".$this->project_id.
             "&task_id=".$this->id.
             "&task_status=".$this->status;
      echo "<td><b><a href=\"$url\" class=\"normal\">".$this->name."</a></b></td>";
    }
    else echo "<td>".$this->name."</td>";
  }

  function print_members($member_ids, $member_names, $member_list) {
    echo "<td>";
    if (!$this->is_manager || $this->is_passed || $this->project_status == STATUS_DROP)
      echo $member_list[$this->person_id];
    else {
      $dropbox = new dropbox();
      $dropbox->add_arrays($member_ids, $member_names);
      $dropbox->print_dropbox("task_".$this->id, $this->person_id);
    }
    echo "</td>";
  }

  function print_status($member_list) {
    global $GO_SECURITY, $GO_THEME, $pm_task_status_values, $pm_task_comments, $pm_task_new_comments;

    echo "<tr>";
    echo "<td align=\"center\">".$this->id."</td>";
    echo "<td>".$this->name."</td>";
    echo "<td>".$member_list[$this->person_id]."</td>";
    echo "<td>".$this->pids."</td>";

    if ($this->is_manager && $this->status != TASK_BYPASS && !$this->approved)
      echo "<td><input class=\"textbox\" style=\"width: 30px;\" type=\"text\" name=\"task_time\" value=\"".$this->days."\" align=\"right\"></td>";
    else {
      $this->get_elapsed();
      $progress = $this->elapsed."/".$this->days;
      if ($this->elapsed > $this->days)
        $progress = "<font color=\"red\"><b>".$progress."</b></font>";
      echo "<td>".$progress."</td>";
    }
    echo "<td>";

    if (($this->is_manager && ($this->status == TASK_BYPASS || !$this->approved)) ||
         $this->status == TASK_ONGOING || ($this->status == TASK_DONE && !$this->approved)) {

      $dropbox = new dropbox();
      if ($this->is_manager) {
        switch ($this->status) {
          case TASK_WAITING:
            $dropbox->add_value(TASK_WAITING, $pm_task_status_values[TASK_WAITING]);
            $dropbox->add_value(TASK_BYPASS, $pm_task_status_values[TASK_BYPASS]);
            break;
          case TASK_ONGOING:
            $dropbox->add_value(TASK_ONGOING, $pm_task_status_values[TASK_ONGOING]);
            $dropbox->add_value(TASK_DONE, $pm_task_status_values[TASK_DONE]);
            $dropbox->add_value(TASK_BYPASS, $pm_task_status_values[TASK_BYPASS]);
            break;
          case TASK_DONE:
            $dropbox->add_value(TASK_ONGOING, $pm_task_status_values[TASK_ONGOING]);
            $dropbox->add_value(TASK_DONE, $pm_task_status_values[TASK_DONE]);
            break;
          case TASK_BYPASS:
            $approved = true;
            if ($this->pids == 0)
              $approved = true;
            else {
              $pids = explode(',', $this->pids);
              foreach ($pids as $pid) {
                $approved = $approved && $project_task_list[$pid]->approved;
                if (!$approved) break;
              }
            }
            if ($approved)
              $dropbox->add_value(TASK_ONGOING, $pm_task_status_values[TASK_ONGOING]);
            else
              $dropbox->add_value(TASK_WAITING, $pm_task_status_values[TASK_WAITING]);
            $dropbox->add_value(TASK_BYPASS, $pm_task_status_values[TASK_BYPASS]);
            break;
        }
      }
      else {
        $dropbox->add_value(TASK_ONGOING, $pm_task_status_values[TASK_ONGOING]);
        $dropbox->add_value(TASK_DONE, $pm_task_status_values[TASK_DONE]);
      }
      $dropbox->print_dropbox("task_status", $this->status);
    }
    else {
      echo $pm_task_status_values[$this->status];
      echo "<input type=\"hidden\" name=\"task_status\" value=\"".$this->status."\">";
    }
    echo "</td>";

    if ($this->is_manager && ($this->status == TASK_DONE || ($this->status == TASK_BYPASS && $this->pids == 0))) {
      $chk = $this->approved ? "checked" : "";
      echo "<td align=\"center\"><input type=\"checkbox\" name=\"task_approved\" ".
           "$chk onclick='javascript:_set_approve()'></td>";
    }
    else
      echo "<td align=\"center\">".($this->approved ? "<img border=\"0\" src=\"".$GO_THEME->images['checked']."\"/>" : "&nbsp;")."</td>";
    echo "</tr>";
    echo "<tr><td>&nbsp;</td></tr>";
    echo "</table>";
    echo "<table border=\"0\" width=\"100%\" cellpadding=\"0\" cellspacing=\"0\">";
    echo "<tr><td class=\"TableHead2\" nowrap>$pm_task_comments</td></tr>";

    $this->print_formated_comments();

    $user = $member_list[$GO_SECURITY->user_id];
    echo "<tr><td><input type=\"hidden\" name=\"task_id\" value=\"".$this->id."\">".
         "<input type=\"hidden\" name=\"project_id\" value=\"".$this->project_id."\">".
         "<input type=\"hidden\" name=\"task_cuser\" value=\"$user\"></td></tr>";

    if ($this->status >= TASK_ONGOING) {
      echo "<tr><td>$pm_task_new_comments:</td></tr>";
      echo "<tr><td><textarea class=\"textbox\" name=\"task_comments\"".
         " rows=\"4\" cols=\"60\" wrap=\"soft\"></textarea></td></tr>";
    }
    echo "</tr>";
  }

  function print_formated_comments() {
    $comments = $this->comments;
    $data = strtok($comments, "\xFF");
    while ($data) {
      if ($data == "") continue;
      list($time, $who, $text) = explode("|", $data);
      echo "<tr class=\"TableInside2\"><td><i>($time)</i>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".
        "<b>$who</b></td></tr>";
      $text = str_replace("\n", "<br>", $text);
      echo "<tr class=\"TableInside1\"><td>$text</td></tr>";
      $data = strtok("\xFF");
    }
  }

  function show_doc_folder() {
    global $GO_SECURITY, $GO_THEME, $pm_task_status_values,$GO_MODULES;

    $fs_module = $GO_MODULES->get_module('filesystem');
    echo "<tr>";
    echo "<td align=\"center\">".$this->id."</td>";
    echo "<td>".$this->name."</td>";
    echo "<td>".$member_list[$this->person_id]."</td>";
    echo "<td>".$this->pids."</td>";
    echo "<td>".$this->days."</td>";
    echo "<td>";
    echo $pm_task_status_values[$this->status];
    echo "</td>";
    echo "<td align=\"center\">".($this->approved ? "<img border=\"0\" src=\"".$GO_THEME->images['checked']."\"/>" : "&nbsp;")."</td>";
    echo "</tr>";
    echo "<tr><td>&nbsp;</td></tr>";
    echo "<tr><td colspan=\"8\"><iframe src=\"".$fs_module['url']."index.php?path=".$this->doc_path."\" width=720 height=300>".
         "</iframe></td></tr>";
  }

  function set_status($status) {
    if ($this->approved)
      $this->set_approve(false);

    $this->status = $status;
    // Update db here
    $this->query("UPDATE task SET task_status=" . $this->status .
                 " WHERE task_id=".$this->id." AND task_project_id=".$this->project_id);
  }

  function set_approve($approved) {
    $this->approved = $approved;

    if ($approved) {
      $today = getdate();
      $today = "'" . $today['year'] . '-' . $today['mon'] . '-' . $today['mday'] . "'";
    }
    else {
      $today = "NULL";
    }
    // Updata db here
    $this->query("UPDATE task SET task_approved_date=$today, task_approved=".($approved?'1':'0').
                 " WHERE task_id=".$this->id." AND task_project_id=".$this->project_id);

    // Update project status
    unset($s);
    if ($approved) {
      if ($this->level >= 0) $s = $this->level;
    }
    else {
      global $tasks;
      $s = STATUS_OFFER;
      foreach ($tasks as $id => $t) {
        if ($t->id == $this->id) break;
        if ($t->level >= 0) $s = $t->level;
      }
    }
    if (isset($s))
      $this->query("UPDATE pmProjects SET status=$s, probability=100 ".
                   'WHERE id='.$this->project_id);
  }

  function set_days($ndays) {
    $this->query("UPDATE task SET task_time=$ndays ".
                 "WHERE task_id=".$this->id." AND task_project_id=".$this->project_id);
  }

  function add_comments($new_comments) {
    smart_addslashes($new_comments);
    $this->query("UPDATE task SET task_comment=CONCAT(task_comment,'$new_comments') ".
                 "WHERE task_id=".$this->id." AND task_project_id=".$this->project_id);
  }

  function find_start_date() {
    global $tasks, $pstart_date;

    if ($this->pids == '0')
      return array(0, 0);

    $plan_stop_date = 0;
    $real_stop_date = 0;
    $pids = explode(',', $this->pids);

    foreach ($pids as $pid) {
      $sd = $tasks[$pid]->find_start_date();
      $psd = $sd[0] + $tasks[$pid]->days;
      if (isset($tasks[$pid]->approved_date))
        $rsd = (strtotime($tasks[$pid]->approved_date) - $pstart_date) / (24 * 3600);
      else
        $rsd = $sd[1];
      if ($psd > $plan_stop_date)
        $plan_stop_date = $psd;
      if ($rsd > $real_stop_date)
        $real_stop_date = $rsd;
    }

    return array($plan_stop_date, $real_stop_date);
  }

  function get_elapsed() {
    global $today, $pstart_date;

    if ($this->elapsed >= 0) return;

    if ($today == 0) {
      $today = getdate();
      $today = gmmktime(0, 0, 0, $today['mon'], $today['mday'], $today['year']);
    }

    list($start_date, $real_start_date) = $this->find_start_date();

    if ($this->status == TASK_ONGOING || ($this->status == TASK_DONE && !$this->approved))
      $this->elapsed = round(($today - $pstart_date) / (24 * 3600)) - $real_start_date;
    elseif ($this->approved)
      $this->elapsed = (round((strtotime($this->approved_date) - $pstart_date) / (24 * 3600)) -
                  $real_start_date);
    else
      $this->elapsed = 0;
  }
}
?>
