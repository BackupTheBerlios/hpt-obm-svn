<?php
require('../../Group-Office.php');
$GO_SECURITY->authenticate();
header('Content-Type: text/html; charset='.$charset);
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>">
<script language="javascript" type="text/javascript" src="<?php echo $GO_CONFIG->host; ?>javascript/common.js"></script>
<title><?php echo $GO_CONFIG->title; ?>
</title>
<link href="<?php echo $GO_THEME->theme_url.'style.css'; ?>" rel="stylesheet" type="text/css" />
</head>
<body marginwidth="0" marginheight="0" leftmargin="0" topmargin="0">
<table border="0" cellpadding="0" cellspacing="0" width="100%" height="23">
<tr>
	<td class="HeaderBar" align="left" nowrap>&nbsp;&nbsp;<b><?php echo htmlspecialchars($_SESSION['GO_SESSION']['name']); ?></b><td>
	<td class="HeaderBar" align="right" width="100%">

			<a class="HeaderBar" href="<?php echo $GO_CONFIG->host; ?>configuration/" target="main">
			<img src="<?php echo $GO_THEME->images['configuration']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_configuration; ?>
			</a>

			<a class="HeaderBar" href="javascript:popup('<?php echo $GO_CONFIG->host; ?>doc/index.php', 500, 500);">
			<img src="<?php echo $GO_THEME->images['help']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_help; ?>
			</a>

			<a class="HeaderBar" href="<?php echo $GO_CONFIG->host; ?>index.php?task=logout" target="_parent">
			<img src="<?php echo $GO_THEME->images['logout']; ?>" width="16" height="16" border="0" align="absmiddle" />
			<?php echo $menu_logout; ?>
			</a>

	</td>
</tr>
</table>
</body>
</html>
