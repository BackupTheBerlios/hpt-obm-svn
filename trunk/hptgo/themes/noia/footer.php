<?php
require('../../Group-Office.php');
header('Content-Type: text/html; charset='.$charset);
$GO_SECURITY->authenticate();
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>">
<script language="javascript" type="text/javascript" src="<?php echo $GO_CONFIG->host; ?>javascript/common.js"></script>
<script language="JavaScript" type="text/javascript" src="<?php echo $GO_CONFIG->host; ?>javascript/remind.js"></script>
<title><?php echo $GO_CONFIG->title; ?>
</title>
<link href="<?php echo $GO_THEME->theme_url.'style.css'; ?>" rel="stylesheet" type="text/css" />
</head>
<body marginwidth="0" marginheight="0" leftmargin="0" topmargin="0">
<table border="0" cellpadding="0" cellspacing="0" width="100%" height="26" class="FooterBar">
<tr class="LeftBar" height="1">
	<td colspan="3">
	
	</td>
</tr>

<tr>
	<td align="right" width="20">
	<img align="right" src="<?php echo $GO_THEME->images['small_user_icon']; ?>" border="0" height="16" width="16">
	</td>
	
	<td class="FooterBar" valign="middle" align="left" nowrap>
	&nbsp;<b><?php echo htmlspecialchars($_SESSION['GO_SESSION']['name']); ?></b>
	</td>

	<td class="FooterBar" valign="middle" align="right" nowrap>
	HPT - Open Business Management version 1.0 &nbsp;
	</td>
	
</tr>
</table>
</body>
</html>
