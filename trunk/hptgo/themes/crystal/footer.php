<?php
require('../../Group-Office.php');
header('Content-Type: text/html; charset='.$charset);
?>

<html>
<head>
<link href="<?php echo $GO_THEME->theme_url.'style.css'; ?>" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>" />
<script language="javascript">

var prevId='';
var prevFontWeight = 'normal';
var prevFontSize=10;
function ABC(id, url){
	parent.main.document.location=url; 	
	if (document.getElementById){
		if (prevId != ''){
			document.getElementById(prevId).style.fontWeight = prevFontWeight;
			document.getElementById(prevId).style.fontSize = prevFontSize;
		}
		prevFontWeight = document.getElementById(id).style.fontWeight;
		prevFontSize = document.getElementById(id).style.fontSize;
		document.getElementById(id).style.fontWeight = 'bold';
		document.getElementById(id).style.fontSize = 11;
		
	}
	else if (document.all){
		if (prevId != ''){
			document.all[prevId].style.fontWeight = prevFontWeight;
			document.all[prevId].style.fontSize = prevFontSize;
		}
		prevFontWeight = document.all[id].style.fontWeight;
		prevFontSize = document.all[id].style.fontSize;
		document.all[id].style.fontWeight = 'bold';
		document.all[id].style.fontSize = 11;		
	}
	prevId = id;
}
</script>
</head>
<body marginwidth="0" marginheight="0" leftmargin="0" topmargin="0">
<table border="0" cellpadding="0" cellspacing="0" width="100%" class="FooterBar">
<tr>
	<td align="right">
	<table border="0">
	<tr>
		<?php
			$modules = $GO_MODULES->get_modules_with_locations();
			while ($module = array_shift($modules))
			{
				if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $module['acl_read']))
				{
					$GO_THEME->images[$module['id']] = isset($GO_THEME->images[$module['id']]) ? $GO_THEME->images[$module['id']] : $GO_THEME->images['unknown'];
					$lang_var = isset($lang_modules[$module['id']]) ? $lang_modules[$module['id']] : $module['id'];
					//$module_ids[] = $module['id'];
					echo '<td class="ModuleIcons" align="center" valign="top" nowrap><a  id="'.$module['id'].'" class="FooterBar" href="javascript:ABC(\''.$module['id'].'\',\''.$module['url'].'\')"><img src="'.$GO_THEME->images[$module['id']].'" border="0" width="32" height="32" /><br />'.$lang_var.'</a></td>';
				}
			}
		?>
	</tr>
	</table>
	</td>
	<td width="100%" height="50" align="right">
	<a target="main" href="http://www.hptvietnam.com.vn" title="<?php echo $menu_about; ?>">
	<img src="<?php echo $GO_THEME->images['go_header']; ?>" border="0" height="40" width="200" />
	</a>
	</td>
</tr>
</table>
</body>
</html>
