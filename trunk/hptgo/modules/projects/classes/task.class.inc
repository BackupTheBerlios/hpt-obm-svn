<?PHP
/*
   Copyright HPT Commerce 2004
   Author: Dao Hai Lam <lamdh@hptvietnam.com.vn>
   Version: 1.0 Release date: 08 August 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

define('TASK_WAITING', 3);
define('TASK_ONGOING', 2);
define('TASK_DONE',    1);
define('TASK_BYPASS',  0);

$today = 0;
$max_task_id = 0;
$last_task_id = 0;
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

  $projects->query("SELECT * FROM task WHERE task_project_id=$project_id ORDER BY task_order");

  // Copy from Template if first time
  if (!$projects->next_record() && $template_id > 0) {
    $projects->query("INSERT INTO task SELECT $project_id, task_template_$template_id.* ".
                     "FROM task_template_$template_id");
    $projects->query("SELECT * FROM task WHERE task_project_id=$project_id  ORDER BY task_order");
    $projects->next_record();
  }

  // Load all tasks
  $last_task = 0;
  while ($template_id >= 0) {
    global $max_task_id, $is_new_project, $last_task_id;

    $t = new project_task($projects, $pstate);
    $tasks[$t->id] = $t;
    $project_task_list[$t->id] = &$tasks[$t->id];
    if ($t->id > $max_task_id) $max_task_id = $t->id;
    if ($t->order > $last_task) {
      $last_task = $t->order;
      $last_task_id = $t->id;
    }
    if ($t->days != 0) $is_new_project = false;
    if (!$projects->next_record()) break;
  }

  return $tasks;
}

function datetime_string($dt) {
  global $months;

  $d = date("j", $dt);
  $m = date("m", $dt);
  if ($_SESSION['GO_SESSION']['date_format'] == 'd-m-Y')
    return $d.' '.$months[$m - 1];
  return $months[$m - 1].' '.$d;
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
  var $order;

  var $elapsed = -1;
  var $start_date = 0;
  var $real_start_date = 0;

  var $is_owner = false;
  var $is_manager = false;
  var $is_monitor = false;
  var $is_passed = false;

  var $doc_path = "";
  var $doc_url = '';

  function project_task($projects, $pstate) {
    global $GO_SECURITY, $GO_USERS, $GO_CONFIG, $responsible_user_id, $monitors;

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
    $this->order = $projects->f('task_order');
    $this->project_status = $pstate;

    if ($this->person_id == 0)
      $this->person_id = $responsible_user_id;

    $this->is_owner = $GO_SECURITY->user_id == $this->person_id;
    $this->is_manager = $GO_SECURITY->user_id == $responsible_user_id;
    $this->is_monitor = isset($monitors) && is_array($monitors) ? in_array($GO_SECURITY->user_id, array_keys($monitors)) : false;
    $this->is_passed = $this->status <= TASK_DONE;

    $this->doc_path = $GO_CONFIG->file_storage_path . 'projects/' .
      $this->project_id . '/' . $this->id;
    $this->doc_url = $_SERVER['PHP_SELF'].'?task=show_task_doc'.
             '&project_id='.$this->project_id.
             '&task_id='.$this->id.'&task_status=show_doc';
  }

  function get_pid_tips() {
    global $pm_status_begin, $tasks, $link_back, $return_to,$GO_SECURITY;

    if ($this->pids == '0') {
      $pids = '&nbsp;';
      $tips = '';
    } else {
      $nolist = array();
      $tips = '<span id="task_tips_'.$this->order.'" '.
                'style="position: absolute; visibility:hidden; display: none;">'.
                '<table border="0" cellspacing="1" cellpadding="0" bgcolor="black">'.
                '<tr><td>'.
                '<table border="0" cellspacing="0" cellpadding="2" bgcolor="#FFF7EB">';
      foreach (explode(',', $this->pids) as $p) {
        if ($p == 0) {
          $to = 0;
          $tn = $pm_status_begin;
        } else {
          $to = $tasks[$p]->order;
          $tn = $tasks[$p]->name;
        }
        $tips .= '<tr><td align="right">'.$to.'.</td><td>&nbsp;'.$tn.'</td></tr>';
        if ($p == 0 || $this->project_status == STATUS_DROP ||
            (!$this->is_manager && !$this->is_monitor &&
              $tasks[$p]->person_id != $GO_SECURITY->user_id))
          $nolist[] = $to;
        else {
          $nolist[] = '<a href="'.$_SERVER['PHP_SELF'].
                        '?task=show_task_status'.
                        '&project_id='.$this->project_id.
                        '&task_id='.$tasks[$p]->id.
                        '&task_status='.$tasks[$p]->status.
			'&return_to='.rawurlencode($return_to).
			'&return_to2='.rawurlencode($link_back).'"'.
                        ' class="normal">'.$to.'</a>';
        }
      }
      $tips .= '</table></td></tr></table></span>';
      $pids = '<span id="task_pre_'.$this->order.'" style="cursor: default;"'.
              ' onmouseover="return show_tips('.$this->order.')"'.
              ' onmouseout="return hide_tips()">'.
              implode(",", $nolist).
              '</span>';
    }
    return array($pids, $tips);
  }

  function print_info($member_ids, $member_names, $member_list) {
    global $GO_THEME, $pm_task_status_values, $pm_status_begin,
           $responsible_user_id, $is_new_project, $tasks, $link_back, $return_to;

    if ($this->level == -1)
      echo '<tr>';
    else
      echo '<tr class="ProjectTask0">';

    echo '<td align="center">'.$this->order.'</td>';
    $this->print_name();
    $this->print_members($member_ids, $member_names, $member_list);
    list($pids, $tips) = $this->get_pid_tips();
    echo '<td>'.$tips.$pids.'</td>';
    if ($this->is_manager && $is_new_project && ($this->status > TASK_DONE))
    {
      echo '<td><input class="textbox" style="width: 30px;" type="text" name="task_time_'.$this->id.'" value="0" align="right"></td>';
      echo '<td align="center"> - </td>';
    } else {
      $this->get_elapsed();
      $progress = $this->elapsed."/".$this->days;
      if ($this->elapsed > $this->days)
        $progress = '<font color="red"><b>'.$progress.'</b></font>';
      echo '<td>'.$progress.'</td>';
      if ($this->status == TASK_ONGOING)
        echo "<td><b>".$pm_task_status_values[$this->status]."</b></td>";
      else {
        if (isset($pm_task_status_values[$this->status]))
          echo "<td>".$pm_task_status_values[$this->status]."</td>";
	else
          echo "<td>&nbsp;</td>";
      }
    }
    echo '<td align="center">'.($this->approved ? '<img border="0" src="'.$GO_THEME->images['checked'].'"/>' : '&nbsp;').'</td>';
    $url = $this->doc_url.'&return_to='.rawurlencode($return_to).'&return_to2='.rawurlencode($link_back);

    if ($this->level == -1) {
      if ($this->is_owner || $this->is_manager || $this->is_monitor)
        echo '<td align="center"><a href="'.$url.'">'.
           '<img border="0" src="'.$GO_THEME->images['doc'].'"/></a></td>';
      else
        echo '<td align="center">'.
           '<img border="0" src="'.$GO_THEME->images['doc_disable'].'"/></td>';
    }
    else {
      if ($this->is_owner || $this->is_manager || $this->is_monitor)
        echo '<td align="center"><a href="'.$url.'">'.
           '<img border="0" src="'.$GO_THEME->images['doc_red'].'"/></a></td>';
      else
        echo '<td align="center">'.
           '<img border="0" src="'.$GO_THEME->images['doc_disable'].'"/></td>';
    }
    echo '</tr>';
  }
  
  function print_name() {
    global $link_back,$return_to;
    if (($this->is_manager || $this->is_monitor || $this->is_owner) &&
        $this->project_status != STATUS_DROP) {
      $url = $_SERVER['PHP_SELF'] . '?task=show_task_status'.
             '&project_id='.$this->project_id.
             '&task_id='.$this->id.
             '&task_status='.$this->status.
             '&return_to='.rawurlencode($return_to).
             '&return_to2='.rawurlencode($link_back);
      echo '<td><b><a href="'.$url.'" class="normal">'.$this->name.'</a></b></td>';
    }
    else echo '<td>'.htmlspecialchars($this->name).'</td>';
  }

  function print_members($member_ids, $member_names, $member_list) {
    echo '<td>';
    if (!$this->is_manager || $this->is_passed || $this->project_status == STATUS_DROP)
      echo $member_list[$this->person_id];
    else {
      $dropbox = new dropbox();
      $dropbox->add_arrays($member_ids, $member_names);
      $dropbox->print_dropbox('task_'.$this->id, $this->person_id);
    }
    echo '</td>';
  }

  function print_status($member_list) {
    global $GO_SECURITY, $GO_THEME, $monitors, $link_back, $return_to,
           $pm_task_status_values, $pm_task_comments, $pm_task_new_comments;

    echo '<tr>';
    echo '<td align="center">'.$this->id.'</td>';
    echo '<td>'.$this->name.'</td>';
    echo '<td>'.$member_list[$this->person_id].'</td>';
    list($pids, $tips) = $this->get_pid_tips();
    echo '<td>'.$tips.$pids.'</td>';

    if ($this->is_manager && $this->status != TASK_BYPASS && !$this->approved)
      echo '<td><input class="textbox" style="width: 30px;" type="text" name="task_time" value="'.$this->days.'" align="right"></td>';
    else {
      $this->get_elapsed();
      $progress = $this->elapsed."/".$this->days;
      if ($this->elapsed > $this->days)
        $progress = '<font color="red"><b>'.$progress.'</b></font>';
      echo '<td>'.$progress.'</td>';
    }
    echo '<td>';

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
      $dropbox->print_dropbox('task_status', $this->status);
      echo '<input type="hidden" name="saved_task_status" value="'.$this->status.'"/>';
    }
    else {
      echo $pm_task_status_values[$this->status];
      echo '<input type="hidden" name="task_status" value="'.$this->status.'">';
    }
    echo '</td>';

    if ($this->is_manager && ($this->status == TASK_DONE || ($this->status == TASK_BYPASS && $this->pids == 0))) {
      $chk = $this->approved ? 'checked' : '';
      echo '<td align="center"><input type="checkbox" name="task_approved" '.$chk.
           ' onclick="javascript:_set_approve()"></td>';
    }
    else
      echo '<td align="center">'.($this->approved ? '<img border="0" src="'.$GO_THEME->images['checked'].'"/>' : '&nbsp;').'</td>';
    if ($this->level >= 0)
      echo '<td align="center"><a href="'.$this->doc_url.'&return_to='.rawurlencode($return_to).'&return_to2='.rawurlencode($link_back).'">'.
           '<img border="0" src="'.$GO_THEME->images['doc_red'].'"/></a></td>';
    else
      echo '<td align="center"><a href="'.$this->doc_url.'&return_to='.rawurlencode($return_to).'&return_to2='.rawurlencode($link_back).'">'.
           '<img border="0" src="'.$GO_THEME->images['doc'].'"/></a></td>';
    echo '</tr>';
    $this->show_datetimes();
    echo '<tr><td>&nbsp;</td></tr>';
    echo '</table>';
    echo '<table border="0" width="100%" cellpadding="0" cellspacing="0">';
    echo '<tr><td class="TableHead2" nowrap>'.$pm_task_comments.'</td></tr>';

    $this->print_formated_comments();

    if ($this->is_monitor)
      $user = $monitors[$GO_SECURITY->user_id];
    else
      $user = $member_list[$GO_SECURITY->user_id];
    echo '<tr><td><input type="hidden" name="task_id" value="'.$this->id.'">'.
         '<input type="hidden" name="project_id" value="'.$this->project_id.'">'.
         '<input type="hidden" name="task_cuser" value="'.$user.'"></td></tr>';

    if ($this->status >= TASK_ONGOING) {
      echo '<tr><td>&nbsp;</td></tr>';
      echo "<tr><td>$pm_task_new_comments:</td></tr>";
      echo '<tr><td><textarea class="textbox" name="task_comments"'.
         ' rows="4" cols="60" wrap="soft"></textarea></td></tr>';
    }
    echo '</tr>';
  }

  function print_formated_comments() {
    $comments = trim($this->comments);
    if ($comments == '') $height = 'height="60"';
    $_SESSION['GO_SESSION']['task_comments'] = $comments;
    echo '<tr><td width="100%">'.
             '<iframe src="comments.php" width="100%" class="textbox" '.$height.'>'.
             '</iframe></td></tr>';
  }

  function show_doc_folder() {
    global $GO_SECURITY, $GO_THEME, $pm_task_status_values;

    echo '<tr>';
    echo '<td align="center">'.$this->id.'</td>';
    echo '<td>'.$this->name.'</td>';
    echo '<td>'.(isset($member_list) && is_array($member_list) ? $member_list[$this->person_id] : '&nbsp;').'</td>';
    list($pids, $tips) = $this->get_pid_tips();
    echo '<td>'.$tips.$pids.'</td>';
    echo '<td>'.$this->days.'</td>';
    echo '<td>';
    echo $pm_task_status_values[$this->status];
    echo '</td>';
    echo '<td align="center">'.($this->approved ? '<img border="0" src="'.$GO_THEME->images['checked'].'"/>' : '&nbsp;').'</td>';
    echo '</tr>';
    echo '<tr><td>&nbsp;</td></tr>';
    echo '<tr><td colspan="8" width="100%">'.
         '<iframe src="folder.php?path='.$this->doc_path.
         '" width="100%" height="300" class="textbox">'.
         '</iframe></td></tr>';
  }

  function show_datetimes() {
    global $pstart_date, $GO_THEME, $today,
           $pm_task_duration, $pm_task_planning, $pm_task_reality,
           $pm_task_advance, $pm_task_latency,
           $pm_task_start, $pm_task_stop,
           $pm_task_days, $pm_task_day;

    $this->get_elapsed();

    $day_sec = 24 * 3600;
    $start_date = datetime_string($pstart_date + $this->start_date * $day_sec);
    $stop_date = datetime_string($pstart_date + ($this->start_date + $this->days) * $day_sec);
    $real_start_date = ($this->status != TASK_WAITING && $this->status != TASK_BYPASS) ?
        datetime_string($pstart_date + $this->real_start_date * $day_sec) : '-';
    $real_stop_date = $this->approved ? datetime_string(strtotime($this->approved_date)) : '-';

    $advance = $latency = '-';
    if ($this->approved) {
        $approved_date = strtotime($this->approved_date);
        $diff = $approved_date - ($pstart_date + ($this->start_date + $this->days) * $day_sec);
        if ($diff < 0)
            $advance = (0 - $diff) / $day_sec;
        else
            $latency = $diff / $day_sec;
    }
    elseif ($this->status == TASK_ONGOING) {
        $rsd = $pstart_date + ($this->real_start_date + $this->days) * $day_sec;
        if ($today < $rsd) {
            $diff = $this->real_start_date - $this->start_date;
            if ($diff < 0)
                $advance = -$diff;
            else
                $latency = $diff;
        }
        elseif ($today > $rsd) {
          $sd = $pstart_date + $this->start_date * $day_sec;
            $latency = round(($today - $sd) / $day_sec) - $this->days;
        }
    }

    if (is_numeric($advance)) {
        if ($advance >= 2)
            $advance .= ' '.$pm_task_days;
        elseif ($advance > 0)
            $advance .= ' '.$pm_task_day;
        else
            $advance = '-';
    }

    if (is_numeric($latency)) {
        if ($latency >= 2)
            $latency .= ' '.$pm_task_days;
        elseif ($latency > 0)
            $latency .= ' '.$pm_task_day;
        else
            $latency = '-';
    }

    echo '<tr><td colspan="8">&nbsp;</td></tr>';
    echo '<tr><td colspan="8" align="center">';
    echo '<table border="0" cellspacing="0" cellpadding="0" bgcolor="#BBBBBB">';
    echo '<tr><td width="100%">';

    echo '<table border="0" cellspacing="1" cellpadding="0">';
    echo '<tr class="TableInside2" align="center">';
    echo '<td width="80">'.$pm_task_duration.'</td>';
    echo '<td width="100">'.$pm_task_planning.'</td>';
    echo '<td width="100">'.$pm_task_reality.'</td>';
    echo '<td width="100">'.$pm_task_advance.'</td>';
    echo '<td width="100">'.$pm_task_latency.'</td></tr>';
    echo '<tr bgcolor="white" align="center">';
    echo '<td>'.$pm_task_start.'</td><td>'.$start_date.'</td><td>'.$real_start_date.'</td>';
    echo '<td rowspan="2">'.$advance.'</td><td rowspan="2">'.$latency.'</td></tr>';
    echo '<tr bgcolor="white" align="center">';
    echo '<td>'.$pm_task_stop.'</td><td>'.$stop_date.'</td><td>'.$real_stop_date.'</td></tr>';
    echo '</table>';

    echo '</td></tr>';
    echo '</table>';
    echo '</td></tr>';
  }


  function set_status($status) {
    if ($status == $this->status) return;

    if ($this->approved)
      $this->set_approve(false);

    $old_status = $this->status;
    $this->status = $status;

    if ($status != TASK_BYPASS)
      $changed_date = '';
    else {
      $today = getdate();
      $today = "'" . $today['year'] . '-' . $today['mon'] . '-' . $today['mday'] . "'";
      $changed_date = ", task_approved_date=$today";
    }

    // Update db here
    $this->query("UPDATE task SET task_status=" . $this->status . $changed_date .
                 " WHERE task_id=".$this->id." AND task_project_id=".$this->project_id);


    if ($status == TASK_ONGOING || $status == TASK_DONE) {

      global $GO_CONFIG;
      
      $db = new db();
      $sql = "SELECT users.* FROM".
        " users LEFT JOIN users_groups ON (users.id = users_groups.user_id)".
        " WHERE users_groups.group_id='".$GO_CONFIG->group_root."'";
      if ($db->query($sql) && $db->num_rows() && $db->next_record()) {
        require_once($GO_CONFIG->class_path."phpmailer/class.phpmailer.php");
        require_once($GO_CONFIG->class_path."phpmailer/class.smtp.php");
        $mail = new PHPMailer();
        $mail->PluginDir = $GO_CONFIG->class_path.'phpmailer/';
        $mail->SetLanguage($php_mailer_lang, $GO_CONFIG->class_path.'phpmailer/language/');

        switch($GO_CONFIG->mailer) {
          case 'smtp':
            $mail->Host = $GO_CONFIG->smtp_server;
            $mail->Port = $GO_CONFIG->smtp_port;
            $mail->IsSMTP();			  
            break;			
          case 'qmail':
            $mail->IsQmail();
            break;			
          case 'sendmail':
            $mail->IsSendmail();
            break;
          case 'mail':
            $mail->IsMail();
            break;
        }

        $mail->Sender   = $db->f('email');
        $mail->From     = $db->f('email');
        $mail->FromName = $GO_CONFIG->title;
        $mail->AddReplyTo($db->f('email'),$GO_CONFIG->title);
        $mail->WordWrap = 50;
        $mail->IsHTML(true);

        $this->query("SELECT * FROM task WHERE task_id=".$this->id." AND task_project_id=".$this->project_id);
        $this->next_record();
        $task_name = $this->f('task_name');
        $task_person_id = $this->f('task_person_id');
	$task_duration = $this->f('task_time');

        $db->query('SELECT * '.
          'FROM pmProjects '.
  	  'WHERE id="'.$this->project_id.'" ');
        $db->next_record();

        $task_url = $GO_CONFIG->full_url.'index.php?return_to='.$GO_CONFIG->full_url.'modules/projects/project.php?task=show_task_status&project_id='.$this->project_id.'&task_id='.$this->id.'&task_status='.$status;
        $project_url = $GO_CONFIG->full_url.'index.php?return_to='.$GO_CONFIG->full_url.'modules/projects/project.php?project_id='.$this->project_id;


        global $pm_task_status_values;
        $new_status = $pm_task_status_values[$status];
        $project_name = $db->f('name');
	$project_description = $db->f('description');
        global $subjectTaskStatusChanged,$mailTaskStatusChangedToDone,$mailTaskStatusChangedToOngoing;
        $mail->Subject = sprintf($subjectTaskStatusChanged,$task_name,$project_name,$pm_task_status_values[$this->status]);
	if ($status == TASK_DONE)
          $mail_body  = sprintf($mailTaskStatusChangedToDone,$project_name,$project_description,$task_name,$task_url);
	else
          $mail_body  = sprintf($mailTaskStatusChangedToOngoing,$pm_task_status_values[$this->status],$project_name,$project_description,$task_name,$task_duration,$project_url);

        $mail->Body = $mail_body;
        $mail->ClearAllRecipients();
        if ($status == TASK_DONE)
          $db->query('SELECT users.* '.
            'FROM users,pmProjects '.
  	    'WHERE users.id=pmProjects.user_id '.
  	    'AND pmProjects.id="'.$this->project_id.'"');
        else
          $db->query('SELECT * '.
            'FROM users '.
  	    'WHERE id="'.$task_person_id.'"');
        $db->next_record();
        $mail->AddAddress($db->f('email'));

        // HACK: For some reasons, admin@hptvietnam.com.vn is not accepted by mail.hptvietnam.com.vn :(
        $mail->From = $db->f('email');
        $mail->Sender = $db->f('email');

        if (!$mail->Send()) echo "Failed: ".$mail->ErrorInfo;
        //$mail->Send();
      }
    }
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

  function find_stop_date() {
    global $tasks, $pstart_date;

    if ($this->pids == '0')
      return 0;

    $real_stop_date = 0;
    $pids = explode(',', $this->pids);

    foreach ($pids as $pid) {
      if (isset($tasks[$pid]->approved_date))
        $rsd = (strtotime($tasks[$pid]->approved_date) - $pstart_date) / (24 * 3600);
      elseif ($tasks[$pid]->status == TASK_ONGOING || $tasks[$pid]->status == TASK_DONE) {
        $today = getdate();
        $today = gmmktime(0, 0, 0, $today['mon'], $today['mday'], $today['year']);
        $rsd = round(($today - $pstart_date) / (24 * 3600));
      }
      else
        $rsd = $tasks[$pid]->find_stop_date();
      if ($rsd > $real_stop_date)
        $real_stop_date = $rsd;
    }

    return $real_stop_date + $this->days;
  }

  function get_elapsed() {
    global $today, $pstart_date;

    if ($this->elapsed >= 0) return;
    if ($this->status == TASK_BYPASS) {
      $this->elapsed = 0;
      return;
    }

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

    $this->start_date = $start_date;
    $this->real_start_date = $real_start_date;
  }
}
?>
