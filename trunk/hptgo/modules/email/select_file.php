<?php
/*
Copyright Intermesh 2003
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 08 July 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

require_once('../../Group-Office.php');
require($GO_LANGUAGE->get_language_file('filesystem'));
$module = $GO_MODULES->get_module('email');
$GO_HANDLER = $module['url'].'attach_online.php';
$GO_CONFIG->window_mode = 'popup';
$target_frame = '_self';
$module = $GO_MODULES->get_module('filesystem');
require($module['path'].'index.php');
?>