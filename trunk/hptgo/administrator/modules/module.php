<?php
/*
   Copyright HPT Vietnam 2003
   Author: Nguyễn Thái Ngọc Duy <duyntn@hptvietnam.com.vn>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require("../../Group-Office.php");

$GO_SECURITY->authenticate();

$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

require($GO_LANGUAGE->get_base_language_file('modules'));

$page_title = $menu_modules;
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$module_id = isset($_REQUEST['module_id']) ? $_REQUEST['module_id'] : '';
$module = $GO_MODULES->get_module($module_id);

switch ($task) {
  case 'backup':
    // prototype: function module_backup($filename)
    // backup package is a zip file that includes the following files:
    // database.sql: sql database
    // module.info
    // filesystem directory: real files from filesystem module
    // code: source code dir
    // setup: (dir). Currently sql only
    $rootdir = tempnam($GO_CONFIG->tmpdir,"OBM");
    unlink($rootdir);
    mkdir($rootdir);
    mkdir("$rootdir/setup");
    mkdir("$rootdir/setup/sql");
    mkdir("$rootdir/src");
    mkdir("$rootdir/files");
    if (isset($_REQUEST['backup_db']) &&
        $_REQUEST['backup_db'] == 'yes' &&
	function_exists("module_backup")) {
      module_backup("$rootdir/database.sql"); // should check for errors
    }
    copy($module['path']."/module.info","$rootdir/module.info");
    if (isset($_REQUEST['backup_files']) &&
        $_REQUEST['backup_files'] == 'yes' &&
	function_exists("module_fs_backup")) {
      module_fs_backup("$rootdir/files");
    }

    if (isset($_REQUEST['backup_src']) &&
        $_REQUEST['backup_src'] == 'yes') {
      copy_folder($module['path'],"$rootdir/src");
    }
    copy($module['path']."/sql/$module_id.install.sql","$rootdir/setup/sql/$module_id.install.sql");
    copy($module['path']."/sql/$module_id.uninstall.sql","$rootdir/setup/sql/$module_id.uninstall.sql");

    require($GO_CONFIG->class_path.'pclzip.class.inc');
    $zip = new PclZip("{$GO_CONFIG->tmpdir}$module_id.zip");
    $zip->create(array($rootdir), PCLZIP_OPT_REMOVE_PATH, $rootdir);
    delete_folder($rootdir);

    $browser = detect_browser();
    //header('Content-Type: '.$type['mime']);
    header('Content-Length: '.filesize($GO_CONFIG->tmpdir."$module_id.zip"));
    header('Expires: '.gmdate('D, d M Y H:i:s') . ' GMT');
    if ($browser['name'] == 'MSIE')
    {
	header('Content-Type: application/octet-stream');
	header('Content-Disposition: attachment; filename="'.$module_id.'.zip"');
	header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
	header('Pragma: public');
    }
    else
    {
	//header('Content-Type: '.$type['mime']);
	header('Pragma: no-cache');
	header('Content-Disposition: attachment; filename="'.$module_id.'.zip"');
    }
    header('Content-Transfer-Encoding: binary');
    $fd = fopen($GO_CONFIG->tmpdir.$module_id.'.zip','rb');
    while (!feof($fd)) {
	print fread($fd, 32768);
    }
    fclose($fd);
    @unlink($GO_CONFIG->tmpdir.$module_id.'.zip');
    exit;
    break;
}

$tabtable = new tabtable('module', 'Module configuration', '100%', '300', '150',  '', true, 'left', 'top', 'module_form', $tab_direction='vertical',$module_id);


$tabtable->add_tab('information.inc', 'Information');
if ($module) {
  $tabtable->add_tab('read_permissions.inc', 'Read Permission');
  $tabtable->add_tab('write_permissions.inc', 'Admin Permission');
  $tabtable->add_tab('backup.inc', 'Backup/Restore');
  $tabtable->add_tab('update.inc', 'Update');
}

require($GO_THEME->theme_path."header.inc");
?>
<form action="<?php echo $_SERVER['PHP_SELF']; ?>" name="module_form" method="post" enctype="multipart/form-data" >
<input type="hidden" name="module_id" value="<?php echo $module_id; ?>"/>
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>"/>
<input type="hidden" name="task" />
<input type="hidden" name="close" value="false" />
<?php
$tabtable->print_head();
//require($tabtable->get_active_tab_id());
switch ($tabtable->get_active_tab_id()) {
  case 'information.inc':
    $module_info = $GO_MODULES->get_module_info($module_id);
?>
<table border="0">
<tr><td>Module name:</td><td><?php echo $module_id; ?></td></tr>
<tr><td>Version:</td><td><?php echo $module_info['version']; ?></td></tr>
<?php if ($module) { ?>
<tr><td>Path:</td><td><?php echo $module['path']; ?></td></tr>
<?php } ?>
<tr><td>Authors:</td><td><?php
  if (is_array($module_info['authors']))
  while ($author = array_shift($module_info['authors'])) {
    echo trim(htmlspecialchars($author['name']).' &lt;'.mail_to($author['email']).'&gt;').'<br />';
  }
  else
  {
    $author = $module_info['authors'];
    echo trim(htmlspecialchars($author['name']).' &lt;'.mail_to($author['email']).'&gt;').'<br />';
  }
?></td></tr>
<tr><td>Description:</td><td><?php echo htmlspecialchars($module_info['description']); ?></td></tr>
<tr><td>Status:</td><td><?php echo htmlspecialchars($module_info['status']); ?></td></tr>
<tr><td>Release date:</td><td><?php echo db_date_to_date($module_info['release_date']); ?></td></tr>
</table>
<?php
echo '<br/>';
    if (!$module)
      $button = new button($cmdInstall,"javascript:document.location='$return_to'");
    else
      $button = new button($cmdUninstall,"javascript:document.location='$return_to'");
    echo '&nbsp;&nbsp;';
    $button = new button($cmdClose,"javascript:document.location='$return_to'");
    break;

  case 'read_permissions.inc':
    print_acl($module['acl_read']);
    echo '<br/>';
    $button = new button($cmdClose,"javascript:document.location='$return_to'");
    break;
    
  case 'write_permissions.inc':
    print_acl($module['acl_write']);
    echo '<br/>';
    $button = new button($cmdClose,"javascript:document.location='$return_to'");
    break;
    
  default:
    require($tabtable->get_active_tab_id());
}
$tabtable->print_foot();
?>
</form>
<?php
require($GO_THEME->theme_path."footer.inc");

function delete_folder($folder)
{
	$dir = @opendir($folder);
	while ($item = readdir($dir))
	{
		if (is_dir("$folder/$item"))
		{
			if ($item != '.' && $item != '..')
				delete_folder("$folder/$item");
		}
		else
			@unlink("$folder/$item");
	}
	closedir($dir);
	@rmdir($folder);
}

function copy_folder($source_path, $destination_path)
{
	if (!file_exists($destination_path))
	{
		global $GO_CONFIG;
		if (!mkdir($destination_path, $GO_CONFIG->create_mode))
		{
			return false;
		}
	}

	$dir = opendir($source_path);
	while ($item = readdir($dir))
	{
		$newspath = "$source_path/$item";
		$newdpath = "$destination_path/$item";
		if (is_dir($newspath))
		{
			if ($item != '.' && $item != '..')
				copy_folder($newspath,$newdpath);
		}
		else
			copy($newspath,$newdpath);
	}
	closedir($dir);
	return true;
}
?>
