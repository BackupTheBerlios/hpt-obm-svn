<?php
// Copyright 2001-2003 Interakt Online. All rights reserved.
	
session_start();
?><?php
	include("../functions.inc.php");
	include_once("../languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']))? $HTTP_SESSION_VARS['KTML2security'][$HTTP_GET_VARS['counter']]['language']:"english").".inc.php"); 
?>
<html>
<head>
<title><?php echo (isset($KT_Messages["Enter Table Information"])) ? $KT_Messages["Enter Table Information"] : "Enter Table Information:"; ?></title>
<link rel="stylesheet" href="../styles/main.css" type="text/css"/> 
<script>
function openHelp(helpStr) {
	if (typeof dialogArguments != "undefined") {
		dialogArguments.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr);
	} else {
		window.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>].toolbar.showHelp(helpStr);
	}
}
function btn_onclick() {
	if (window.opener) {
		var ktml = window.opener.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>];
	} else {
		var ktml = dialogArguments.ktmls[<?php echo $HTTP_GET_VARS['counter']; ?>];
	}
	rowstext = document.getElementById("NumRows").value;
	colstext = document.getElementById("NumCols").value;
	brdtext = document.getElementById("Border").value;

	rows = parseInt(rowstext);
	cols = parseInt(colstext);
	var re = new RegExp('^[0-9]+$');
	if ((rows > 0) && (cols > 0) && brdtext.match(re)) {
		ktml.undo.addEdit();
		table = ktml.edit.createElement("table");
		table.setAttribute("border", brdtext);
		table.setAttribute("cellpadding", "2");
		table.setAttribute("cellspacing", "2");
		table.setAttribute("width", "75%");
		tbody = ktml.edit.createElement("tbody");
		for (var i=0; i < rows; i++) {
			tr =ktml.edit.createElement("tr");
			for (var j=0; j < cols; j++) {
				td =ktml.edit.createElement("td");
				td.innerHTML = '&nbsp;';
				//br =ktml.edit.createElement("br");
				//td.appendChild(br);
				tr.appendChild(td);
			}
			tbody.appendChild(tr);
		}
		table.appendChild(tbody);
		if (window.opener) {
			window.opener.util_insertNodeSel(ktml.edit, table);
		} else {
			dialogArguments.util_insertNodeSel(ktml.edit, table);
		}
		if (ktml.viewinvisibles) {
			ktml.utils_setInvisibles(false);
		}
		ktml.undo.addEdit();
	}	
	window.close();
}
</script>

</head>
<body class="body">
<table width="240" height="98%" border="0" cellpadding="0">
  <tr>
    <td valign="top"><fieldset class="ktml_fieldset">
    <legend class="ktml_legend"><?php echo (isset($KT_Messages["InsertTable"])) ? $KT_Messages["InsertTable"] : "Insert Table"; ?></legend>
    <table cellspacing="2" class="ktml_table">
      <tr>
        <td class="ktml_label"><label class="ktml_label"><?php echo (isset($KT_Messages["Rows"])) ? $KT_Messages["Rows"] : "Rows:"; ?></label>
        <td><input type="TEXT" class="ktml_input" id="NumRows" SIZE=3 name="NumRows" value="3">
        </tr>
      <tr>
        <td><label class="ktml_label"><?php echo (isset($KT_Messages["Cols"])) ? $KT_Messages["Cols"] : "Cols:"; ?></label>
        <td><input type="TEXT" class="ktml_input" id="NumCols" SIZE=3 name="NumCols" value="3">
        </tr>
      <tr>
        <td><label class="ktml_label"><?php echo (isset($KT_Messages["Border"])) ? $KT_Messages["Border"] : "Border:"; ?></label>
        <td><input type="TEXT" class="ktml_input" id="Border" SIZE=3 NAME="Border" value="1">
        </tr>
    </table>  
    </fieldset></td>
    <td width="80" align="right" valign="top">
		<input name="button2" type="button" class="ktml_button" id="button" onClick="btn_onclick()" value="<?php echo (isset($KT_Messages["Ok"])) ? $KT_Messages["Ok"] : "Ok"; ?>"/><br>
    	<input name="button" type="button" class="ktml_button" id="button3" onClick="window.close();" value="<?php echo (isset($KT_Messages["Cancel"])) ? $KT_Messages["Cancel"] : "Cancel"; ?>"/>
		<input name="helpbutton" type="button" class="ktml_button" id="buttonhelp" onClick="openHelp('inserttablepopup')" value="<?php echo (isset($KT_Messages["Help"])) ? $KT_Messages["Help"] : "Help"; ?>"/>
	</td>
  </tr>
</table>
</body>
</html>
