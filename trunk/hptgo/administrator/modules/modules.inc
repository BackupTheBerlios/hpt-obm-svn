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



echo '<br />';
if (isset($feedback))
{
	echo $feedback;
}

echo '<h1>'.$menu_modules.'</h1>'.$admin_modules_text.'<br /><br />';

echo '<table width="100%"><tr><td valign="top">';
echo '<table border="0"><tr><td class="TableHead2" colspan="2">'.$mod_available.'</td></tr>';

$column_count = 0;

$modules = $fs->get_folders($GO_CONFIG->module_path);
$module_count = count($modules);
while($module = array_shift($modules))
{
	if ($module['name'] != 'CVS' && !$sql_module = $GO_MODULES->get_module($module['name']))
	{
		$version = '';
		
		$lang_var = isset($lang_modules[$module['name']]) ? $lang_modules[$module['name']] : $module['name'];
	

		$info_file = $module['path'].'/module.info';
		$module_info = $GO_MODULES->get_module_info($module['name']);
		
		$version = $module_info['version'];
		
		if ($module_info)
		{
				$tooltip = '<table border="0">'.
					'<tr><td valign="top">'.$strDescription.':</td><td>'.text_to_html(trim(htmlspecialchars($module_info['description']))).'</td></tr>'.
					'<tr><td>'.$strVersion.':</td><td>'.trim($module_info['version']).'</td></tr>'.
					'<tr><td>'.$strStatus.':</td><td>'.trim($module_info['status']).'</td></tr>'.
					'<tr><td>'.$strReleaseDate.':</td><td>'.db_date_to_date(trim($module_info['release_date'])).'</td></tr>'.
					'<tr><td valign="top">'.$strAuthors.':</td><td>';
				while ($author = array_shift($module_info['authors']))
				{
					$tooltip .= trim(htmlspecialchars($author['name']).' &lt;'.mail_to($author['email']).'&gt;').'<br />';
				}
				$tooltip .= '</td></tr></table>';
		}else
		{
			$tooltip = $strNoModuleInfo;
		}
		
		echo '<tr>';
		
		$GO_THEME->images[$module['name']] = isset($GO_THEME->images['mod_'.$module['name']]) ? $GO_THEME->images['mod_'.$module['name']] : $GO_THEME->images['unknown'];
		echo '<td><a href="#" '.$overlib->print_overlib(htmlspecialchars($tooltip)).'><img style="margin-right: 5px;" src="'.$GO_THEME->images[$module['name']].'" border="0" width="32" height="32" align="absmiddle" />'.$lang_var.'</a></td><td align="right">';

		$button = new button($cmdInstall, "javascript:install('".$module['name']."', '".trim($version)."', '');");

		echo '</td></tr>';
		echo '<tr><td height="1" colspan="3"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
	}
}
echo '</table>';

echo '</td><td valign="top">';

echo '<table border="0" cellspacing="0">'.
	'<tr><td class="TableHead2">'.$mod_installed.'</td>'.
	'<td class="TableHead2">&nbsp;</td></tr>';

$column_count = 0;

$modules = $GO_MODULES->get_modules_with_locations();
$module_count = count($modules);
while($module = array_shift($modules))
{		
	$lang_var = isset($lang_modules[$module['id']]) ? $lang_modules[$module['id']] : $module['id'];


	$info_file = $module['path'].'/module.info';
	$module_info = $GO_MODULES->get_module_info($module['id']);
	
	if ($module_info)
	{
			$tooltip = '<table border="0">'.
				'<tr><td valign="top">'.$strDescription.':</td><td>'.text_to_html(trim(htmlspecialchars($module_info['description']))).'</td></tr>'.
				'<tr><td>'.$strVersion.':</td><td>'.trim($module_info['version']).'</td></tr>'.
				'<tr><td>'.$strStatus.':</td><td>'.trim($module_info['status']).'</td></tr>'.
				'<tr><td>'.$strReleaseDate.':</td><td>'.db_date_to_date(trim($module_info['release_date'])).'</td></tr>'.
				'<tr><td valign="top">'.$strAuthors.':</td><td>';
			while ($author = array_shift($module_info['authors']))
			{
				$tooltip .= trim(htmlspecialchars($author['name']).' &lt;'.mail_to($author['email']).'&gt;').'<br />';
			}
			$tooltip .= '</td></tr></table>';
	}else
	{
		$tooltip = $strNoModuleInfo;
	}
	
	echo '<tr>';
	
	$GO_THEME->images[$module['id']] = isset($GO_THEME->images['mod_'.$module['id']]) ? $GO_THEME->images['mod_'.$module['id']] : $GO_THEME->images['unknown'];

	
	echo '<td><a href="#" '.$overlib->print_overlib(htmlspecialchars($tooltip)).'>'.
				'<img style="margin-right: 5px;" src="'.$GO_THEME->images[$module['id']].'" border="0" width="32" height="32" align="absmiddle" />'.$lang_var.'</a></td>'.
  '<td align="right">';


	$button = new button($strPermissions, "javascript:popup('module_permissions.php?module_id=".
				urlencode($module['id'])."','450','450')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdUninstall, "javascript:uninstall('".$module['id']."', '".$lang_var."');");
	
	echo '</td></tr>';
	echo '<tr><td height="1" colspan="3"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

}
echo '</table>';

echo '</td></tr></table>';

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
