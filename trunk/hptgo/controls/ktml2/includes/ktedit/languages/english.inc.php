<?php
	// Copyright 2001-2004 Interakt Online. All rights reserved.
	
	//main toolbar
	$KT_Messages["Cut"] = "Cut";
	$KT_Messages["Copy"] = "Copy";
	$KT_Messages["Paste"] = "Paste";
	$KT_Messages["InsertImage"] = "Insert Image";
	$KT_Messages["UploadImage"] = "Upload Image";
	$KT_Messages["InsertTable"] = "Insert Table";
	$KT_Messages["InsertLink"] = "Insert Link";
	$KT_Messages["Link"] = "Link";
	$KT_Messages["File"] = "File";
	$KT_Messages["ToggleVisible"] = "Show/Hide Table Border";
	$KT_Messages["ToggleEditMode"] = "Show Code/Design";
	$KT_Messages["Find"] = "Find";
	$KT_Messages["RemoveLink"] = "Remove Link";
	$KT_Messages["Bold"] = "Bold";
	$KT_Messages["Italic"] = "Italic";
	$KT_Messages["Underline"] = "Underline";
	$KT_Messages["AlignLeft"] = "Align Left";
	$KT_Messages["AlignCenter"] = "Align Center";
	$KT_Messages["AlignRight"] = "Align Right";
	$KT_Messages["AlignJustify"] = "Align Justify";
	$KT_Messages["BackgroundColor"] = "Background Color";
	$KT_Messages["ForegroundColor"] = "Foreground Color";
	$KT_Messages["Undo"] = "Undo";
	$KT_Messages["Redo"] = "Redo";
	$KT_Messages["BulletList"] = "Bullet List";
	$KT_Messages["NumberedList"] = "Numbered List";
	$KT_Messages["Indent"] = "Indent";
	$KT_Messages["Outdent"] = "Outdent";
	$KT_Messages["HorizontalLine"] = "Horizontal Rule";
	$KT_Messages["FontType"] = "Font Type";
	$KT_Messages["FontSize"] = "Font Size";
	$KT_Messages["Font Advanced"] = "Font Advanced";
	$KT_Messages["KTML Properties Window"] = "KTML Properties Window";
	$KT_Messages["Size"] = "Size";
	$KT_Messages["About"] = "About";
	$KT_Messages["Help"] = "Help";
	$KT_Messages["Spellcheck"] = "Spellcheck";

	$KT_Messages["normal"] = "normal";
	$KT_Messages["new window"] = "new window";
	$KT_Messages["current window"] = "current window";
	$KT_Messages["current frame"] = "current frame";
	$KT_Messages["parent frame"] = "parent frame";
	
	$KT_Messages["Select Font..."] = "Select Font...";
	$KT_Messages["Select Size..."] = "Size...";

	//messages for file management
	$KT_Messages["Upload Image"] = "Upload Image";
	$KT_Messages["Delete Image"] = "Delete Image";
	$KT_Messages["Make Directory"] = "Make Folder";
	$KT_Messages["Delete Directory"] = "Delete Folder";
	$KT_Messages["Upload File"] = "Upload File";
	$KT_Messages["Delete File"] = "Delete File";	
	$KT_Messages["Target:"] = "Target:";
	$KT_Messages["File:"] = "File:";
	
	$KT_Messages["Enter Full Url"] = "Enter Full URL";
	$KT_Messages["Alternate Text"] = "Alternative  Text";	
	$KT_Messages["Layout"] = "Layout";	
	$KT_Messages["Alignment"] = "Alignment";
	$KT_Messages["Left"] = "Left";
	$KT_Messages["Right"] = "Right";
	$KT_Messages["Top"] = "Top";
	$KT_Messages["Middle"] = "Middle";
	$KT_Messages["Bottom"] = "Bottom";
	
	$KT_Messages["Width"] = "Width";	
	$KT_Messages["Height"] = "Height";	
	
	$KT_Messages["Border Thickness"] = "Border Thickness:";
	
	$KT_Messages["AbsoluteMiddle"] = "AbsoluteMiddle";
	$KT_Messages["AbsoluteBottom"] = "AbsoluteBottom";
	$KT_Messages["Default"] = "Default";
	$KT_Messages["Baseline"] = "Baseline";
	$KT_Messages["Texttop"] = "Texttop";
	
	$KT_Messages["Spacing"] = "Spacing";
	$KT_Messages["Horizontal"] = "Horizontal";
	$KT_Messages["Vertical"] = "Vertical";
	$KT_Messages["OK"] = "OK";
	$KT_Messages["Cancel"] = "Cancel";

	$KT_Messages["Select a file to upload"] = "Select a file to upload";	
	$KT_Messages["Select a file to delete"] = "Select a file to delete";	
	$KT_Messages["Leave Image"] = "Leave Image";	
	$KT_Messages["Custom"] = "Custom";	
	$KT_Messages["Sharpen"] = "Sharpen";	
	$KT_Messages["Upload"] = "Upload";	
	$KT_Messages["Accepted Image Types"] = "Accepted Image Types";	
	$KT_Messages["Accepted File Types"] = "Accepted File Types";	
	$KT_Messages["Other Media Files"] = "Other Media Files";
	$KT_Messages["Download"] = "INSERT FILE";
	$KT_Messages["Unselect Folder"] = "Unselect Folder";
	$KT_Messages["Up One Level"] = "Up One Level";
	$KT_Messages["Enter folder name"] = "Please enter a folder name:";
	$KT_Messages["New Folder"] = "New Folder";
	$KT_Messages["Folder name invalid"] = "Folder name is not valid!";
	$KT_Messages["Cannot delete main"] = "Cannot delete main directory !";
	$KT_Messages["Are you sure dir"] = "Are you sure that you want to delete directory ";
	$KT_Messages["Are you sure del"] = "Are you sure you want to delete ";
	$KT_Messages["resize to this"] = "Resize to these dimensions:";
	$KT_Messages["CleanHTMLContent"] = "Clean HTML Content";
	$KT_Messages["About Ktml"] = "About Ktml";
	$KT_Messages["No frame support"] = "Your browser does not support frames!";
	$KT_Messages["No credentials"] = 'You don\'t have credentials to access this part of the editor. Please click <a href=# onclick="window.close()">here</a> to close this window';
	
	$KT_Messages["You are here:"] = "You are here:";
	
	//popups
	
	$KT_Messages["Enter Table Information"] = "Enter Table Information"; 
	$KT_Messages["InsertTable"] = "Insert Table";
	$KT_Messages["Rows"] = "Rows";
	$KT_Messages["Cols"] = "Cols";
	$KT_Messages["Border"] = "Border";
	
?>
<?php
	echo '<script>
		function KT_sprintf(str1, str2) {
			return str1.replace(/%s/, str2);;
		}
	
		var KT_js_messages = new Array();
		KT_js_messages["Color picker"] = "Color picker";
		KT_js_messages["Enter a color:"] = "Enter a color:";
		KT_js_messages["Selected"] = "Selected:";
		KT_js_messages["OK"] = "OK";
		KT_js_messages["Cancel"] = "Cancel";

		KT_js_messages["Table rowspanalready1"] = "RowSpan is already set to 1.";
		KT_js_messages["Table nocellbelowmerge"] = "There is not a cell below this one to merge with.";
		KT_js_messages["Table nocellbelow"] = "No cell below";
		KT_js_messages["Table diffcolspans"] = "Can\'t merge cells with different colspans.";
		KT_js_messages["Table diffrowspans"] = "Can\'t merge cells with different rowspans.";
		KT_js_messages["Table cantdivide"] = "Cell can\'t be divided.";
		KT_js_messages["Table onlycell"] = "You can\'t remove the only remaining cell in a row.";

		KT_js_messages["hidden missing"] = "The hidden field related to this KTML control has no ID set!\n Check http://www.interakt.ro/products/technical_17.html for details.";
		KT_js_messages["not active context"] = "This is not the active editor context! ";
		KT_js_messages["move pointer"] = "Please move the selection pointer outside the %s element to be able to switch to code view.";
		KT_js_messages["select something"] = "Please select some text or an image.";
		KT_js_messages["No text selected"] = "No text selected";
		KT_js_messages["Remove Tag"] = "Remove Tag";
		KT_js_messages["File exists"] = "The file already exists!!!\r\nPlease enter a new name for the file:";
		KT_js_messages["Bad name"] = "The file name is not compliant!\r\nPlease enter a new name for the file";
		KT_js_messages["Invalid value"] = "Invalid value";
		KT_js_messages["Value to big"] = "Value too big! Must be less then %s";
		KT_js_messages["No form"] = "KTML is not inside a form! You will not be able to sumbit your work.";
		KT_js_messages["Anchor2Link"] = "Do you want to transform this anchor into a link? (the old Anchor will be deleted).";
	</script>';
?>
<?php
//modules 
	$idx = -1;
	if (isset($counter)) {
		$idx = $counter;
	} else if (isset($HTTP_GET_VARS['ktmlid'])) {
		//$idx = $HTTP_GET_VARS['ktmlid'];
	} 
	if (($idx >= 0) && (@$HTTP_SESSION_VARS['KTML2security'][$idx]['moduleexists'])) {
		foreach ($HTTP_SESSION_VARS['KTML2security'][$idx]['moduleexists'] as $mod=>$modexist) {
			if ($modexist == 1 && file_exists(@$dirDepth."includes/ktedit/modules/".$mod."/languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$counter]['language']))? $HTTP_SESSION_VARS['KTML2security'][$counter]['language']:"english").".inc.php")) {
				include(@$dirDepth."includes/ktedit/modules/".$mod."/languages/".((isset($HTTP_SESSION_VARS['KTML2security'][$counter]['language']))? $HTTP_SESSION_VARS['KTML2security'][$counter]['language']:"english").".inc.php");
			}
		}
	}
?>
