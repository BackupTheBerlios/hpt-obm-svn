<?php
require("../../Group-Office.php");

//authenticate the user
//if $GO_SECURITY->authenticate(true); is used the user needs admin permissons

$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'vdict'
$GO_MODULES->authenticate('vdict');

require($GO_THEME->theme_path."header.inc");
require('db/ev.php');

$tabtable = new tabtable('Dictionary', 'Dictionary', '100%', '400');
$tabtable->add_tab($dict_id, $dict_description);
$tabtable->print_head();

$wtabtable = new tabtable('wlist_table', 'Word', '200', '300', '30', '', false, 'left', 'top', '', 'vertical');
echo '<table border="0" cellspacing="0" cellpadding="4">';
// Left panel
echo '<tr><td>';
echo '<table border="0" cellspacing="0" cellpadding="0">';
echo '<tr><td>';
foreach ($dict_dbhash as $label => $file)
	$wtabtable->add_tab($file, $label);
$active_tab = isset($_REQUEST['active_tab']) ? $_REQUEST['active_tab'] : null;
if (isset($active_tab))
	$wtabtable->set_active_tab($active_tab);
$wtabtable->print_head();
echo '<table border="0" cellspacing="0" cellpadding="4">';
echo '<tr><td>';
echo '<input id="word" type="text" class="textbox" name="word" value="" style="width:160" onchange="word_lookup()">';
echo '</td></tr><tr><td>';
echo '<select name="list" class="textbox" id="wordlist" onchange="change_word(this)" ondblclick="show_word(this)" multiple="true" size="23" style="width:160">';
$fp = fopen('db/'.$wtabtable->get_active_tab_id(), "r");
while (!feof($fp)) {
	$w = fgets($fp, 512);
	echo '<option>'.$w.'</option>';
}
fclose($fp);
echo '</select></td></tr></table>';
$wtabtable->print_foot();
echo '</td></tr>';
echo '</table>';
echo '</td>';
// Right panel
echo '<td width="100%" height="100%">';
echo '<iframe src="contents.php" height="100%" width="100%" class="textbox"></iframe>';
echo '</td></tr>';
echo '</table>';
?>
<script language="JavaScript">
<!--
var agent = navigator.userAgent.toLowerCase();
var is_ie = ((agent.indexOf("msie") != -1) && (agent.indexOf("opera") == -1));
var word_field = document.getElementById("word");
var word_list = document.getElementById("wordlist");

word_field.focus();

function change_word(sel)
{
	if (is_ie) sel = word_list;
	word_field.value = sel.options[sel.selectedIndex].text;
}

function show_word(sel)
{
	if (is_ie) sel = word_list;
	frames[0].location.href = "contents.php?word=" +
		sel.options[sel.selectedIndex].text;
}

word_field.onkeypress = function ref_word(e)
{
	if (is_ie) e = window.event;
	if (e.keyCode == 13)
		frames[0].location.href = "contents.php?word=" + word_field.value;
	else {
		/* TODO:
		   lookup and set word position
		*/
	}
}

var cur_word = '';
var cur_index = 0;

function word_lookup()
{
	w = word_field.value;
	if (cur_word == '') {
		l = 0;
		r = word_list.options.length - 1;
	}
	else
	if (cur_word > w) {
		l = 0;
		r = cur_index;
	}
	else {
		l = cur_index;
		r = word_list.options.length - 1;
	}

	while (true) {
		if (r - l < 2) {
			if (word_list.selectedIndex >= 0)
				word_list.options[word_list.selectedIndex].selected = false;
			if (w > word_list.options[l].text) {
				word_list.selectedIndex = cur_index = r;
				word_list.options[r].selectedIndex = r;
				word_list.options[r].selected = true;
			}
			else {
				word_list.selectedIndex = cur_index = l;
				word_list.options[l].selectedIndex = l;
				word_list.options[l].selected = true;
			}
			break;
		}
		m = (l + r) >> 1;
		if (w == word_list.options[m].text) {
			if (word_list.selectedIndex >= 0)
				word_list.options[word_list.selectedIndex].selected = false;
			word_list.selectedIndex = cur_index = m;
			word_list.options[m].selectedIndex = m;
			word_list.options[m].selected = true;
			break;
		}
		else
		if (w < word_list.options[m].text)
			r = m;
		else
			l = m;
	}
}
//-->
</script>
<?php
$tabtable->print_foot();
require($GO_THEME->theme_path."footer.inc");
?>
