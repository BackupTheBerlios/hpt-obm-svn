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
<table border="0" cellpadding="0" cellspacing="0" width="100%" height="24" class="FooterBar">
<tr class="LeftBar" height="1">
	<td>
	
	</td>
</tr>
<tr>
	<td class="FooterBar" valign="top" align="left" nowrap>HPT - Open Business Management version 1.0&nbsp;&nbsp;</td>
	
</tr>
</table>
</body>
</html>
