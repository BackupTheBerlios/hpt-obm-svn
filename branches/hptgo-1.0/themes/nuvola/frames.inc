<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo $GO_CONFIG->title; ?>
</title>
<link rel="shortcut icon" href="<?php echo $GO_CONFIG->host; ?>lib/favicon.ico" />
</head>
<frameset rows="0, 24,100%, 55" frameborder="0" framespacing="0">
<frame name="checker" src="<?php echo $GO_CONFIG->host; ?>checker.php" scrolling="no" noresize>
<frame name="header" src="<?php echo $GO_THEME->theme_url; ?>header.php" scrolling="no" noresize>
<frame name="main" src="<?php echo $link; ?>" noresize>
<frame name="footer" src="<?php echo $GO_THEME->theme_url; ?>footer.php" scrolling="no" noresize>
</frameset>
</html>
