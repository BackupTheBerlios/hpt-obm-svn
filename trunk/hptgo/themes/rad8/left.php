<?php
require('../../Group-Office.php');
header('Content-Type: text/html; charset='.$charset);
?>

<script language="javascript">
var prevId='';
var prevFontWeight = 'normal';
var prevFontSize=10;

function get_object(name)
{
	if (document.getElementById)
		return document.getElementById(name);
 	else if (document.all)
  		return document.all[name];
 	else if (document.layers)
  		return document.layers[name];
	return false;
}

function Bold_Text(id)
{
	if (obj = get_object(id))
	{
		if ( prevId != '')
			if (pre_obj = get_object(prevId) )
			{
				pre_obj.style.fontWeight = prevFontWeight;
				pre_obj.style.fontSize = prevFontSize;
			}
		prevFontWeight = obj.style.fontWeight;
		prevFontSize = obj.style.fontSize;
		obj.style.fontWeight = 'bold';
		obj.style.fontSize = 11;
	}
	prevId = id;
}
</script>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>" />
<link href="<?php echo $GO_THEME->theme_url.'style.css'; ?>" rel="stylesheet" type="text/css" />
</head>
<body marginwidth="0" marginheight="0" leftmargin="0" topmargin="0">
<table border="0" cellpadding="0" cellspacing="0" width ="100%" height="100%" class="LeftBar">
<tr>
	<?php
		$modules = $GO_MODULES->get_modules_with_locations();
		$user = $GO_USERS->get_user($GO_SECURITY->user_id);

		foreach ($modules as $id => $module) {
			$module_map[$module['id']] = $id;
		}

		$a = explode(':',$user['modules']);
		$show_text = isset($a[1]) ? $a[1] == '1' : true;
		$module_list = $a[0];
		if ($module_list == '' && isset($module_map) && is_array($module_map))
			$module_list = implode(' ',array_keys($module_map));

		if ($module_list != '') {
		$module_list = explode(' ',$module_list);
		foreach ($module_list as $module)
		{
			$module = $modules[$module_map[$module]];
			echo "\n<!-- ".$module['id']." -->\n";
			if ($module['enable'] && 
			($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) ||
			$GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read'])))
			{
				$GO_THEME->images[$module['id']] = isset($GO_THEME->images[$module['id']]) ? $GO_THEME->images[$module['id']] : $GO_THEME->images['unknown'];
				if ($show_text)
				$lang_var = isset($lang_modules[$module['id']]) ? $lang_modules[$module['id']] : $module['id'];
//				echo '<td class="ModuleIcons" align="center" valign="top" nowrap><a  id="'.$module['id'].'" class="LeftBar" href="javascript:Bold_Text(\''.$module['id'].'\',\''.$module['url'].'\')"><img src="'.$GO_THEME->images[$module['id']].'" border="0" width="32" height="32" /><br />'.$lang_var.'</a></td>';	
				echo '<tr><td class="ModuleIcons" align="center" valign="top" wrap>&nbsp;<br><a id="'.$module['id'].'" class="LeftBar" onClick="javascript:Bold_Text(\''.$module['id'].'\')" href="'.$module['url'].'" target="main"><img src="'.$GO_THEME->images[$module['id']].'" border="0" width="32" height="32" /><br />'.$lang_var.'</a></td></tr>';	
				if ($show_text)
				if (strcasecmp($lang_modules[$_SESSION['GO_SESSION']['start_module']], $lang_var)==0) // set bold text for default screen
					echo "<script language='javascript'>Bold_Text('".$module['id']."');</script>";
			}
		}
		}
	?>
</tr>
<tr height="100%">
	<td>
	&nbsp;
	</td>
</tr>
</table>
</body>
</html>
