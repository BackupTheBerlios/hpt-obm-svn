<?php

require_once($GO_CONFIG->class_path.'GraphViz.php');
require_once($GO_MODULES->class_path.'task.class.inc');
require_once($GO_LANGUAGE->get_language_file('projects'));


function build_template_image($template_id,$type='png')
{
  global $GO_CONFIG,$pm_status_begin;

  $db = new db();
  $db->query('SELECT * FROM task_template_'.$template_id.' ORDER BY task_id');
  $count = $db->num_rows();

  $tasks = array();
  while ($db->next_record()) {
    $id = $db->f('task_id');
    $name = $db->f('task_name');
    $level = $db->f('task_level');
    $level = $level >= 0 ? 0 : -$level;
    $time = $db->f('task_time');
    $pre = $db->f('task_predecessors');
    $tasks[$id] = array('id' => $id,
			'name' => $name,
			'level' => $level,
			'time' => $time,
			'pre' => $pre);
  }

  $graphviz = new Image_GraphViz();

  $graphviz->addNode('node_0',
		     array('label' => $pm_status_begin,
			   'fontname' => 'arial',
			   'shape' => 'box'));
  foreach ($tasks as $task) {
    if ($task['level'] == 0)
      $graphviz->addNode('node_'.$task['id'],
			 array('URL' => "javascript:set_edit('{$task['name']}', '{$task['pre']}', '{$task['time']}', {$task['id']})",
			       'label' => $task['name'],
			       'fontname' => 'arial',
			       'shape' => 'box'));
    else
      $graphviz->addNode('node_'.$task['id'],
			 array('URL' => "javascript:set_edit('{$task['name']}', '{$task['pre']}', '{$task['time']}', {$task['id']})",
			       'label' => $task['name'],
			       'fontname' => 'arial'));
    //  echo "Node ".$task['id'];
  }

  foreach ($tasks as $task) {
    $a = explode(',',$task['pre']);
    foreach($a as $i) {
      $graphviz->addEdge(array('node_'.(int)$i => 'node_'.$task['id']));
      //echo "Edge ".$task['id']." - ".(int)$i."<br/>";
    }
  }

  $graphviz->addAttributes(array('fontpath' => $GO_CONFIG->root_path.'fonts'));
  $graphviz->addAttributes(array('fontname' => 'arial'));
  $graphviz->image($type);
  //echo $graphviz->parse();
}
?>