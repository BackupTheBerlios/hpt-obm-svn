<?php
define(S_NONE,  0);
define(S_ITEM,  1);
define(S_IDIOM, 2);

function highlight($word, $s)
{
	$text = '';
	$s = split(' ', $s);
	foreach ($s as $w)
		if ($w == $word)
			$text .= $w.' ';
		else {
			$ww = $w;
			$ln = strlen($w);
			if ($ln > 2) {
				$tail = substr($w, -2);
				if ($tail == "'s" || $tail == "'d")
					$ww = substr($w, 0, $ln - 2);
				else {
					$tail = substr($w, -3);
					if ($tail == "'ve" || $tail == "'ll" || $tail == "'re" || $tail == "'em")
						$ww = substr($w, 0, $ln - 3);
				}
			}
			$text .= "<A HREF=\"javascript:show_word('".$ww."')\" class=\"normal\">".$w.'</A> ';
		}
	return $text;
}

function show_contents($word, $contents)
{
	$data = '';
	$session = S_NONE;
	foreach ($contents as $c) {
		switch ($c[0]) {
			case '*':
				if ($session == S_IDIOM)
					$data .= '</UL></OL>';
				elseif ($session == S_ITEM)
					$data .= '</UL>';
				$session = S_ITEM;
				$data .= '<H3>'.str_replace('$', '-', $c).'</H3><UL>';
				break;
			case '-':
				if ($session == S_NONE)
					$data .= '- '.substr($c, 1).'<BR> ';
				else {
					$c = split('[$]', substr($c, 1));
					if( !isset($c[1]) )
						$data .= '<LI>'.$c[0].'<BR> ';
					else {
						if( !isset($c[2]) ) {
							$data = '<LI>'.$c[0];
							$pp = strtok($c[1], ",");
							while( $pp ) {
								$data .= '<A HREF="?word='.$pp.'">'.$pp.'</A> ';
								$pp = strtok(",");
							}
							$data .= '<BR> ';
						}
						else {
							$data .= '<LI>'.$c[0].'<A HREF="?word='.$c[1].'">'.$c[1].'</A>'.
											$c[2].'<BR>';
						}
					}
				}
				break;
			case '=':
				$c = split('[+]', substr($c, 1));
				$data .= highlight($word, $c[0]).'<BR><EM>'.$c[1].'</EM><BR>';
				break;
			case '!':
				$data .= '</UL>';
				if ($session != S_IDIOM) {
					$session = S_IDIOM;
					$data .= '<H3>Idioms</H3><OL>';
				}
				$data .= '<LI>'.highlight($word, substr($c, 1)).'<UL>';
				break;
			default:
				$data .= $c . '<BR>';
		}
	}
	$data .= '</UL>';
	if ($session == S_IDIOM) $data .= '</OL>';
	echo $data;
}

require("../../Group-Office.php");

$GO_SECURITY->authenticate();

$GO_MODULES->authenticate('vdict');
require($GO_THEME->theme_path."header.inc");

$word = $_REQUEST['word'];

if (isset($word)) {
	$dict = dba_open("ev.db3", "r", "db4");
	if ($dict === false)
		echo "tiêu\n";
	else {
		$word = strtolower($word);
		$tail = substr($word, -2);
		$len = strlen($word);
		if ($tail == "'s" || $tail == "'d")
			$word = substr($word, 0, $len - 2);
		else {
			$tail = substr($word, -3);
			if ($tail == "'ve" || $tail == "'ll" || $tail == "'re" || $tail == "'em")
				$word = substr($word, 0, $len - 3);
		}

		$contents = dba_fetch($word, $dict);
		if ($contents === false) {
			if (substr($word, -1) == 's') {
				$word = substr($word, 0, strlen($word) - 1);
				$contents = dba_fetch($word, $dict);
			}
		}
		dba_close($dict);

		if ($contents !== false) {
			$contents = explode("\n", $contents);
			show_contents($word, $contents);
		}
	}
}
?>
<script language="JavaScript">
<!--
var frm = window.parent.document.forms[0];

function show_word(w)
{
	frm.word.value = w;
	frm.submit();
}
//-->
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
