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

function Bold_Text(id,imgfile)
{
	if (obj = get_object(id))
	{
		if ( prevId != '')
			if (pre_obj = get_object(prevId) )
			{
				//pre_obj.style.fontWeight = prevFontWeight;
				pre_obj.style.color = prevcolor;
				document.getElementById("left_modon"+prevId).src="<?php echo "images/menu/modoff_begin.gif";?>";
				document.getElementById("right_modon"+prevId).src="<?php echo "images/menu/modoff_end.gif";?>";
				document.getElementById("bg_"+prevId).className="Menu_modoff";
			}
		//prevFontWeight = obj.style.fontWeight;
		prevcolor = obj.style.color;
		//obj.style.fontWeight = 'bold';
		obj.style.color = '#0869b5';
		document.getElementById("modicon").src=imgfile;
		document.getElementById("left_modon"+id).src="<?php echo "images/menu/modon_begin.gif";?>";
		document.getElementById("right_modon"+id).src="<?php echo "images/menu/modon_end.gif";?>";
		document.getElementById("bg_"+id).className="Menu_modon";
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

<table border="0" cellpadding="0" cellspacing="0" width="100%" class="HeaderBar">
<tr height="46">
	<td width="100%">
	
	<table border="0" cellpadding="0" cellspacing="0" height="46" width="100%">
	<tr>
	<td width="200">
	<a target="main" href="http://www.hptvietnam.com.vn" title="<?php echo $menu_about; ?>">
	<img src="<?php echo $GO_THEME->images['go_header']; ?>" border="0" height="40" width="200" />
	</a>
	</td>
	
	<td border="0" class="bigtitle" align="left" width="100%">
	<?php echo $title; ?>
	</td>
	
	<td class="HeaderBar" border="0" width="150" valign="top">
	<img id="modicon">
	</td>
	</tr>
	</table>
	
	</td>
</tr>

<tr height="54">
	<td height="54" align="left" colspan="3">
		<table border="0" cellpadding="0" cellspacing="0" height="54" width="100%">
		<tr height="28">
		
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
				if (!isset($module_map[$module])) continue;
				$module = $modules[$module_map[$module]];
				echo "\n<!-- ".$module['id']." -->\n";
				if ($module['enable'] && 
				($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']) ||
				$GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read'])))
				{
					$GO_THEME->images[$module['id']] = isset($GO_THEME->images[$module['id']]) ? $GO_THEME->images[$module['id']] : $GO_THEME->images['unknown'];
					if ($show_text)
					$lang_var = isset($lang_modules[$module['id']]) ? $lang_modules[$module['id']] : $module['id'];
//					echo '<td class="ModuleIcons" align="center" valign="top" nowrap><a  id="'.$module['id'].'" class="LeftBar" href="javascript:Bold_Text(\''.$module['id'].'\',\''.$module['url'].'\')"><img src="'.$GO_THEME->images[$module['id']].'" border="0" width="32" height="32" /><br />'.$lang_var.'</a></td>';	
					echo '<td border="0" align="center" valign="middle" nowrap height="28">';
					echo '<img id="left_modon'.$module['id'].'" src="images/menu/modoff_begin.gif">';
					echo '</td>';	
					
					echo '<td id="bg_'.$module['id'].'" class="Menu_modoff" border="0" align="center" valign="middle" nowrap height="28">';
					echo '<a id="'.$module['id'].'"  onClick="javascript:Bold_Text(\''.$module['id'].'\',\''.$GO_THEME->images[$module['id']].'\')" href="'.$module['url'].'" target="main">&nbsp;&nbsp;'.$lang_var.'&nbsp;&nbsp;</a>';
					echo '</td>';	
					
					echo '<td border="0" align="center" valign="middle" nowrap height="28">';
					echo '<img id="right_modon'.$module['id'].'" src="images/menu/modoff_end.gif">';
					echo '</td>';	
					if (strcasecmp($lang_modules[$_SESSION['GO_SESSION']['start_module']], $lang_var)==0) // set bold text for default screen
						echo "<script language='javascript'>Bold_Text('".$module['id']."','".$GO_THEME->images[$module['id']]."');</script>";
				}
			}
			}
		?>
<!--		<?php
				
					echo '<td id="bg_'.$module['id'].'" style="background: url(images/menu/out_bg.gif);" border="0" align="center" valign="middle" nowrap height="28">';
					echo '<a id="'.$module['id'].'"  onClick="javascript:Bold_Text(\''.$module['id'].'\',\''.$GO_THEME->images[$module['id']].'\')" href="'.$module['url'].'" target="main">&nbsp;<img src="images/menu/2rightarrow.png" border="0" width="16" height="22"/></a>';
					echo '</td>';
					echo '</td>';
		?> -->
			<td width="100%" style="background: url(images/menu/out_bg.gif);"></td>
				
		</tr>
		<tr height="26">
			<td width="100%" colspan="99" style="background: url(images/menu/sub_nav_bg.gif);">
			</td>
		</tr>
		</table>
	</td>
</tr>
</table>
</body>
</html>
