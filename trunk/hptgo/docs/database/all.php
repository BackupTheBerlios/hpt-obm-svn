<?php
	header('Content-Type: text/xml');
	$fp = popen('xsltproc all.xsl modules.xml','r');
	while ($buf = fread($fp,1024))
		echo $buf;
	pclose($fp);
?>
