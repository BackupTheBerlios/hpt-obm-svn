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


$selected_modules = array();
$selected_count = 0;
echo '<br />';
if (isset($feedback))
{
	echo $feedback;
}

echo '<h1>'.$menu_modules.'</h1>'.$admin_modules_text.'<br /><br /><table border="0" width="600" cellspacing="5">';

$column_count = 0;
$module_count = count($modules);
while($module = array_shift($modules))
{
	if ($module['name'] != 'CVS')
	{
		$version = '';
		
		$lang_var = isset($lang_modules[$module['name']]) ? $lang_modules[$module['name']] : $module['name'];
		echo '<tr><td>';		
		$tabtable = new tabtable($module['name'], $lang_var, '100%', '');
		$tabtable->print_head();
		
		echo '<table border="0">';	
		$info_file = $module['path'].'/module.info';
		$module_info = $GO_MODULES->get_module_info($module['name']);
				
		if ($module_info)
		{
				echo '<tr><td nowrap valign="top">'.$strDescription.':</td><td>'.text_to_html(trim(htmlspecialchars($module_info['description']))).'</td></tr>';
				echo '<tr><td nowrap>'.$strVersion.':</td><td>'.trim($module_info['version']).'</td></tr>';
				echo '<tr><td nowrap>'.$strStatus.':</td><td>'.trim($module_info['status']).'</td></tr>';
				echo '<tr><td nowrap>'.$strReleaseDate.':</td><td>'.db_date_to_date(trim($module_info['release_date'])).'</td></tr>';			
				echo '<tr><td valign="top" nowrap>'.$strAuthors.':</td><td>';
				while ($author = array_shift($module_info['authors']))
				{
					echo trim(htmlspecialchars($author['name']).' &lt;'.mail_to($author['email']).'&gt;').'<br />';
				}
				echo '</td></tr>';
		}else
		{
			echo '<tr><td></td><td>'.$strNoModuleInfo.'</td></tr>';
		}
		echo '</table>';
		echo '<div align="right">';
		if ($sql_module = $GO_MODULES->get_module($module['name']))
		{
			$button = new button($strPermissions, "javascript:popup('module_permissions.php?module_id=".
						urlencode($module['name'])."','450','450')");
			echo '&nbsp;&nbsp;';
			$button = new button($cmdUninstall, "javascript:uninstall('".$module['name']."', '".$lang_var."');");
		}else
		{
			$button = new button($cmdInstall, "javascript:install('".$module['name']."', '".trim($version)."');");
		}
		echo '</div>';		
		$tabtable->print_foot();
		echo '</td></tr>';
	}
}
echo '</table>';
$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
?>
<script type="text/javascript">
function uninstall(module_id, module_name)
{
	if (confirm("<?php echo $strConfirmUninstallPrefix; ?> '"+module_name+"' <?php echo $strConfirmUninstallSuffix; ?>"))
	{
		document.forms[0].task.value = 'uninstall';
		document.forms[0].module_id.value = module_id;	
		document.forms[0].submit();
	}
}

function install(module_id, version)
{
	document.forms[0].version.value = version;
	document.forms[0].task.value = 'install';
	document.forms[0].module_id.value = module_id;	
	document.forms[0].submit();
}
</script>
