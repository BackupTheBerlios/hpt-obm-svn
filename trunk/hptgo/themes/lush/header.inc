<?php
$load_start = getmicrotime();
$charset = isset($charset) ? $charset : 'iso-8859-1';
header('Content-Type: text/html; charset='.$charset);
?>
<html>
<head>
<link rel="shortcut icon" href="<?php echo $GO_CONFIG->host; ?>lib/favicon.ico" />
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset; ?>" />
<?php if (isset($GO_HEADER['head'])) echo $GO_HEADER['head']; ?>
<script language="javascript" type="text/javascript" src="<?php echo $GO_CONFIG->host; ?>javascript/common.js"></script>
<link rel="shortcut icon" href="<?php echo $GO_CONFIG->host; ?>lib/favicon.ico" />
<link href="<?php echo $GO_THEME->theme_url.'style.css'; ?>" rel="stylesheet" type="text/css" />
<link href="<?php echo $GO_THEME->theme_url.'htmlarea.css'; ?>" rel="stylesheet" type="text/css" />
<link href="<?php echo $GO_THEME->theme_url.'calendar.css'; ?>" type="text/css" rel="stylesheet" />
<title>
<?php
echo $GO_CONFIG->title;
if (isset($page_title))
{
	echo " - ".$page_title;
}

echo '</title>';
if(isset($GO_HEADER['auto_refresh']))
{
	echo '<meta http-equiv="refresh" content="'.$GO_HEADER['auto_refresh']['interval'].';';
	if (isset($GO_HEADER['auto_refresh']['url']))
	{
	 echo 'url='.$GO_HEADER['auto_refresh']['url'];
	}
	echo '">';
}
echo "</head>\r\n";
echo '<body ';
if (isset($GO_HEADER['body_arguments'])) echo ' '.$GO_HEADER['body_arguments'];
echo '><div id="overDiv" style="position:absolute; visibility:hidden; z-index:1000;"></div>';
?>
<!--Einde header-->
