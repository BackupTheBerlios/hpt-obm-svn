<?php

require("../../Group-Office.php");


$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('projects');

require_once($GO_MODULES->class_path.'graph.class.inc');

if (!$GO_SECURITY->has_admin_permission($GO_SECURITY->user_id)) {
	header('Location: '.$GO_CONFIG->host.'error_docs/403.php');
	exit();
}

$template_id = $_REQUEST['template_id'];
build_template_image($template_id,'png');
?>