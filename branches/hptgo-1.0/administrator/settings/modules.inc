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

require($GO_CONFIG->class_path.'xpath.class.inc');
$xpath = new XPath();

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
		if (file_exists($info_file) && function_exists('xml_parser_create'))
		{
			$xpath->reset();
			if ($xpath->importFromFile($info_file))
			{
				if ($description = $xpath->getData("/module[1]/description[1]"))
				{
					echo '<tr><td valign="top" nowrap>'.$strDescription.':</td><td>'.text_to_html(trim(htmlspecialchars($description))).'</td></tr>';
				}
				if ($version = $xpath->getData("/module[1]/version[1]"))
				{
					echo '<tr><td nowrap>'.$strVersion.':</td><td>'.trim($version).'</td></tr>';
				}
				if ($status = $xpath->getData("/module[1]/status[1]"))
				{
					echo '<tr><td nowrap>'.$strStatus.':</td><td>'.trim($status).'</td></tr>';
				}
				if ($releaseDate = $xpath->getData("/module[1]/releaseDate[1]"))
				{
					echo '<tr><td nowrap>'.$strReleaseDate.':</td><td>'.db_date_to_date(trim($releaseDate)).'</td></tr>';
				}
				
				echo '<tr><td valign="top" nowrap>'.$strAuthors.':</td><td>';
				$i=1;
				while ($author = $xpath->getAttributes("/module[1]/authors[1]/author[$i]"))
				{
					echo trim(htmlspecialchars($author['name']).' &lt;'.mail_to($author['email']).'&gt;').'<br />';
					$i++;
				}
				echo '</td></tr>';
			}			
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
