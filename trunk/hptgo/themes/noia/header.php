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
<table border="0" cellpadding="0" cellspacing="0" width="100%" class="HeaderBar">
<tr height="50">

	<td width="205" height="50" align="left">
	<a target="main" href="http://www.hptvietnam.com.vn" title="<?php echo $menu_about; ?>">
	<img src="<?php echo $GO_THEME->images['go_header']; ?>" border="0" height="40" width="200" />
	</a>
	</td>
	
	<td class="bigtitle" width="*"height="50" valign="middle" align="center">
	<?php echo $title; ?>
	</td>
	
	<td width="200" height="50" align="right">
		<table width="100%">
		<tr>
			<td class="ModuleIcons1" id="user_area" align="right" nowrap><br><img src="<?php echo $GO_THEME->images['small_user_icon']; ?>" border="0" height="16" width="16" align="middle"/>&nbsp;<b><?php echo htmlspecialchars($_SESSION['GO_SESSION']['name']); ?></b>
			</td>
		</tr>
		
		<tr>
			<td align="right" nowrap width="100%">

			<a class="ModuleIcons1" href="<?php echo $GO_CONFIG->host;?>configuration/" target="main">
			<img src="<?php echo $GO_THEME->images['configuration']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_configuration; ?>
			</a>

			<a class="ModuleIcons1" href="javascript:popup('<?php echo $GO_CONFIG->host; ?>doc/Vietnamese/index.html', 500, 500);">
			<img src="<?php echo $GO_THEME->images['help']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_help; ?>
			</a>

			<a class="ModuleIcons1" href="<?php echo $GO_CONFIG->host; ?>index.php?task=logout" target="_parent">
			<img src="<?php echo $GO_THEME->images['logout']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_logout; ?>
			</a>
			
			</td>
		</tr>
		</table>
	</td>
</tr>
<tr class="LeftBar" height="1" width="100%">
	<td colspan="3">
	
	</td>
</tr>
</table>
</body>
</html>
