<?php
require("../../Group-Office.php");

//authenticate the user
//if $GO_SECURITY->authenticate(true); is used the user needs admin permissons

$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'vdict'
$GO_MODULES->authenticate('vdict');

require($GO_THEME->theme_path."header.inc");

$tabtable = new tabtable('Dictionary', 'Dictionary', '100%', '400');
$tabtable->add_tab('ev', 'English-Vietnamese');

$tabtable->print_head();
$word = $_REQUEST['word'];
$wlist = $_SESSION['GO_SESSION']['wordlist'];
if (isset($wlist)) {
	for ($i = count($wlist) - 1; $i >= 0; $i--)
		if ($wlist[$i] == $word) {
			unset($wlist[$i]);
			break;
		}
	if (count($wlist) >= 23)
		array_pop($wlist);
	array_unshift($wlist, $word);
}
else {
	$wlist[] = $word;
}
$_SESSION['GO_SESSION']['wordlist'] = $wlist;

echo '<form name="events" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<table border="0" cellspacing="0" cellpadding="4">';
// Left panel
echo '<tr><td>';
echo '<table border="0" cellspacing="4" cellpadding="0">';
echo '<tr><td>Word:</td><td>';
echo '<input type="text" class="textbox" name="word" value="'.$word.'">';
echo '</td></tr>';
echo '<tr><td colspan="2">';
$wordlist = new dropbox();
if (isset($wlist)) {
	foreach ($wlist as $w)
		$wordlist->add_value($w, $w);
}
$wordlist->print_dropbox('list', $word, 'onchange="change_word(this.value)" ondblclick="javascript:frm.submit()"', true, 23, 180);
echo '</td></tr>';
echo '</table>';
echo '</td>';
// Right panel
echo '<td width="100%" height="100%">';
echo '<iframe src="contents.php'.(isset($word) ? '?word='.$word : '').'" height="100%" width="100%" class="textbox"></iframe>';
echo '</td></tr>';
echo '</table>';
echo '</form>';
?>
<script language="JavaScript">
<!--
var frm = document.forms[0];

function change_word(w)
{
	frm.word.value = w;
}
//-->
</script>
<?php
$tabtable->print_foot();
require($GO_THEME->theme_path."footer.inc");
?>
