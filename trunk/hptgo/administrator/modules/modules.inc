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

echo '<table border="0" cellspacing="0">'.
	'<tr><td class="TableHead2">'.$mod_installed.'</td>'.
	'<td class="TableHead2">'.'Version'.'</td>'.
	'<td class="TableHead2">'.'Description'.'</td>'.
	'<td class="TableHead2"><input type="checkbox" onclick="javascript:invert_selection(\'enable\')" />Enable</td>'.
	'<td class="TableHead2"><input type="checkbox" onclick="javascript:invert_selection(\'install\')" />Install</td>'.
	'<td class="TableHead2"><input type="checkbox" onclick="javascript:invert_selection(\'uninstall\')" />Uninstall</td>'.
	'<td class="TableHead2">&nbsp;</td></tr>';

$column_count = 0;

$installed_modules = $GO_MODULES->get_modules_with_locations();
$installed_module_count = count($installed_modules);
foreach ($installed_modules as $id => $module)
$installed_modules_map[$module['id']] = $id;
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
		echo '<td><a href="module.php?module_id='.$module['name'].'&return_to='.$link_back.'" ><img style="margin-right: 5px;" src="'.$GO_THEME->images[$module['name']].'" border="0" width="32" height="32" align="absmiddle" />'.$lang_var.'</a></td><td>'.$module_info['version'].'</td><td '.$overlib->print_overlib(htmlspecialchars($tooltip)).'>'.cut_string(htmlspecialchars($module_info['description']),60).'</td><td>';
	//echo '<input type="checkbox" id="enable_'.$module['name'].'" name="enable[]" value="'.$module['name'].'" checked />';
	echo '-';
	echo '</td><td>';
	echo '<input type="checkbox" id="install_'.$module['name'].'" name="install[]" value="'.$module['name'].'"/>';
	echo'</td><td>-</td>';
		echo '</tr>';
		echo '<tr><td height="1" colspan="100"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
	}
	else
	{
	if (isset($installed_modules_map[$module['name']]))
{		
	$module = $installed_modules[$installed_modules_map[$module['name']]];
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

	
	echo '<td><a href="module.php?module_id='.$module['id'].'&return_to='.$link_back.'">'.
				'<img style="margin-right: 5px;" src="'.$GO_THEME->images[$module['id']].'" border="0" width="32" height="32" align="absmiddle" />'.$lang_var.'</a></td>'.
		'<td>'.(htmlspecialchars($module_info['version'])).'</td>'.
		'<td '.$overlib->print_overlib(htmlspecialchars($tooltip)).'>'.cut_string(htmlspecialchars($module_info['description']),60).'</td>'.
  '<td>';
	echo '<input type="checkbox" id="enable_'.$module['id'].'" name="enable[]" value="'.$module['id'].'" '.($module['enable'] ? 'checked' : '').'/>';
  echo '</td><td>-</td><td>';
	echo '<input type="checkbox" id="uninstall_'.$module['id'].'" name="uninstall[]" value="'.$module['id'].'" />';
	echo '</td></tr>';
	echo '<tr><td height="1" colspan="100"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

}
	}
}


$column_count = 0;

echo '</table>';

echo '</td></tr></table>';
echo '<br/>';
$button = new button($cmdSave, "javascript:process()");
echo '&nbsp;&nbsp;';
$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
?>
<script type="text/javascript">
function process()
{
	s = 'install';
	installs = new Array();
	install_count = 0;
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' &&
		document.forms[0].elements[i].checked &&
		document.forms[0].elements[i].id.substr(0,s.length) == s)
			installs[install_count++] = document.forms[0].elements[i].value;
	}
	
	s = 'uninstall';
	uninstalls = new Array();
	uninstall_count = 0;
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' &&
		document.forms[0].elements[i].checked &&
		document.forms[0].elements[i].id.substr(0,s.length) == s)
			uninstalls[uninstall_count++] = document.forms[0].elements[i].value;
	}

	msg = '';
	if (install_count)
	{
		msg = msg + 'You are about to install: '+installs;
		msg = msg + '\n';
	}

	if (uninstall_count)
	{
		msg = msg + 'You are about to uninstall: '+uninstalls;
	}
	if (msg == '' ||
		confirm(msg+'\nIs it ok?'))
	{
		document.forms[0].task.value='process';
		document.forms[0].submit();
	}
}

function invert_selection(s)
{
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' &&
		//document.forms[0].elements[i].name == s)
		document.forms[0].elements[i].id.substr(0,s.length) == s)
		{
			document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
		}
	}
}

</script>
