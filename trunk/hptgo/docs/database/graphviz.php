<?php
	$input = $_REQUEST['input'];
	$pipe1 = "xsltproc db2dot.xsl $input";
	if (isset($_REQUEST['all']))
		$pipe1 .= " --stringparam all 1";
	if (isset($_REQUEST['table']))
		$pipe1 .= " --stringparam main_table ".$_REQUEST['table'];
	$pipe2 = 'xsltproc dotml2dot.xsl -';
	$pipe3 = 'dot -Tpng /dev/stdin';
	$fp = popen("$pipe1|$pipe2|$pipe3",'r');
	header('Content-Type: image/png');
	while ($buf = fread($fp,1024))
	{
		echo $buf;
	}
	pclose($fp);
?>
