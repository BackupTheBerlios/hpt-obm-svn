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
<tr height="57">

	<td width="205" height="55" align="left">
	<a target="main" href="http://www.hptvietnam.com.vn" title="<?php echo $menu_about; ?>">
	<img src="<?php echo $GO_THEME->images['go_header']; ?>" border="0" height="40" width="200" />
	</a>
	</td>
	
	<td class="bigtitle" width="*"height="55" valign="middle" align="center">
	<?php echo $title; ?>
	</td>
	
	<td width="200" height="55" valign="bottom" align="right">
		<table border="0" width="100%">
		
		<tr height="*">
			<td align="right" valign="bottom" nowrap width="100%">
			<?php
			if($GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
			{
			?>			
			<a class="HeaderBar" href="<?php echo $GO_CONFIG->host; ?>administrator/" target="main">
			<img src="<?php echo $GO_THEME->images['admin']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_admin; ?>
			</a>			
			<?php
			}
			?>
				<a class="ModuleIcons1" href="javascript:popup('<?php echo $GO_CONFIG->host; ?>doc/Vietnamese/index.html', 500, 500);">
				<img src="<?php echo $GO_THEME->images['help']; ?>" width="16" height="16" border="0" align="absmiddle" />
				<?php echo $menu_help; ?>
				</a>
			</td>
		</tr>
		
		<tr valign="middle">
			<td align="right" valign="bottom" nowrap width="100%">
				<a class="ModuleIcons1" href="<?php echo $GO_CONFIG->host; ?>index.php?task=logout" target="_parent">
				<img src="<?php echo $GO_THEME->images['logout']; ?>" width="16" height="16" border="0" align="absmiddle" />
				<?php echo $menu_logout; ?>&nbsp;<b><?php echo htmlspecialchars($_SESSION['GO_SESSION']['username']); ?></b>
				</a>
				
				<a class="ModuleIcons1" href="<?php echo $GO_CONFIG->host;?>configuration/" target="main">
				<img src="<?php echo $GO_THEME->images['configuration']; ?>" width="16" height="16" border="0" align="absmiddle" />
				<?php echo $menu_configuration; ?>
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
