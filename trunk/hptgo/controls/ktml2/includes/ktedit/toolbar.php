<?php
// Copyright 2001-2004 Interakt Online. All rights reserved.
// verify if the  editor images directory exists
// 
if (file_exists($dirDepth."images/editor_images")) {
	$relativeImagePath = $dirDepth."images/editor_images";	
} else {
	$relativeImagePath = "img/ktml_images";
}
// $relativeImagePath
// general actions string
$evstr = "onmouseover=\"hndlr_buttonMouseOver(this);\" onmousedown=\"hndlr_buttonMouseDown(this);\"	onmouseup=\"hndlr_buttonMouseUp(this);\" onmouseout=\"hndlr_buttonMouseOut(this);\"	onfocus=\"\" ";
?>

<script>
function showToolbarButton(bName, bId, helpId, command) {
	document.write('<img <?php echo $evstr; ?> class="toolbaritem_flat" src="<?php echo $relativeImagePath ?>/'+bId+'.gif" alt="'+bName+'" title="'+bName+'" kttype="btn" id="'+bId+'" cid="'+command+'" onclick="if ( pageLoded() &amp;&amp; ktml_<?php echo $objectName; ?>.util_checkFocus(this, true)) { if (ktml_<?php echo $objectName; ?>.toolbar.checkHelp(this, event)) { ktml_<?php echo $objectName; ?>.toolbar.showHelp(\''+helpId+'\'); } else { ktml_<?php echo $objectName; ?>.logic_doFormat(\''+command+'\') }}" />');
}
</script>

<table cellpadding="0" cellspacing="0" class="toolbar" width="100%" onselectstart="return false;" oncontextmenu="return false;">
	<tr class="ktml_bg">
		<td valign="bottom" colspan="2" nowrap="true">
		<span name="Property_<?php echo $objectName; ?>" id="Property_<?php echo $objectName; ?>" unselectable="on" onselectstart="return false;" ondragstart="return false;" ondragover="return false;" ondrop="return false;" onbeforeeditfocus="return false">
		<?php if (in_array("Cut",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Cut"])) ? $KT_Messages["Cut"] : "Cut"; ?>", "cut", "cutcopypaste", "Cut")</script>
		<?php } ?>
		<?php if (in_array("Copy",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Copy"])) ? $KT_Messages["Copy"] : "Copy"; ?>", "copy", "cutcopypaste", "Copy")</script>
		<?php } ?>
		<?php if (in_array("Paste",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Paste"])) ? $KT_Messages["Paste"] : "Paste"; ?>", "paste", "cutcopypaste", "Paste")</script>
		<?php } ?>
		<?php if (in_array("Undo",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Undo"])) ? $KT_Messages["Undo"] : "Undo"; ?>", "undo", "undoredo", "Undo")</script>
		<?php } ?>
		<?php if (in_array("Redo",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Redo"])) ? $KT_Messages["Redo"] : "Redo"; ?>", "redo", "undoredo", "Redo")</script>
		<?php } ?>
		<?php if (in_array("Insert Image",$display) || $display[0] =="ALL" ) {?>    
		<img <?php echo $evstr; ?> class="toolbaritem_flat" 
			 src="<?php echo $relativeImagePath ?>/image.gif" 
			 alt="<?php echo (isset($KT_Messages["InsertImage"])) ? $KT_Messages["InsertImage"] : "Insert Image"; ?>" 
			 title="<?php echo (isset($KT_Messages["InsertImage"])) ? $KT_Messages["InsertImage"] : "Insert Image"; ?>" 
			 kttype="btn" id="insertimage" cid="InsertImage"
			 onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.util_checkFocus(this, true)) { if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('insertimage'); } else { <?php echo 'ktml_'.$objectName; ?>.logic_openInsertImage(<?php echo $counter; ?>); }}" />
		<?php } ?>
		<?php if (in_array("Insert Table",$display) || $display[0] =="ALL" ) {?>
		<img <?php echo $evstr; ?> class="toolbaritem_flat" 
			 src="<?php echo $relativeImagePath ?>/instable.gif" 
			 alt="<?php echo (isset($KT_Messages["InsertTable"])) ? $KT_Messages["InsertTable"] : "Insert Table"; ?>" 
			 title="<?php echo (isset($KT_Messages["InsertTable"])) ? $KT_Messages["InsertTable"] : "Insert Table"; ?>" 
			 kttype="btn" id="inserttable" cid="instable"
			 onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.util_checkFocus(this, true)) { if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('inserttable'); } else { <?php echo 'ktml_'.$objectName; ?>.logic_InsertTable(<?php echo $counter; ?>); }}" />
		<?php } ?>
		<?php if (in_array("Bullet List",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["BulletList"])) ? $KT_Messages["BulletList"] : "Bullet List"; ?>", "bulletlist", "bulletlist", "InsertUnorderedList")</script>
		<?php } ?>
		<?php if (in_array("Numbered List",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["NumberedList"])) ? $KT_Messages["NumberedList"] : "Numbered List"; ?>", "numberlist", "numberlist", "InsertOrderedList")</script>
		<?php } ?>
		<?php if (in_array("Indent",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Indent"])) ? $KT_Messages["Indent"] : "Indent"; ?>", "indent", "indentation", "Indent")</script>
		<?php } ?>
		<?php if (in_array("Outdent",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Outdent"])) ? $KT_Messages["Outdent"] : "Outdent"; ?>", "outdent", "indentation", "Outdent")</script>
		<?php } ?>
		<?php if (in_array("HR",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["HorizontalLine"])) ? $KT_Messages["HorizontalLine"] : "Horizontal Line"; ?>", "hr", "horline", "InsertHorizontalRule")</script>
		<?php } ?>
			<?php if (in_array("Align Left",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["AlignLeft"])) ? $KT_Messages["AlignLeft"] : "Align Left"; ?>", "alignleft", "paralign", "JustifyLeft")</script>
		<?php } ?>
		<?php if (in_array("Align Center",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["AlignCenter"])) ? $KT_Messages["AlignCenter"] : "Align Center"; ?>", "aligncenter", "paralign", "JustifyCenter")</script>
		<?php } ?>
		<?php if (in_array("Align Right",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["AlignRight"])) ? $KT_Messages["AlignRight"] : "Align Right"; ?>", "alignright", "paralign", "JustifyRight")</script>
		<?php } ?>
		<?php if (in_array("Align Justify",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["AlignJustify"])) ? $KT_Messages["AlignJustify"] : "Align Justify"; ?>", "alignjust", "paralign", "JustifyFull")</script>
		<?php } ?>    		
		<?php if (in_array("Toggle Vis/Invis",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["ToggleVisible"])) ? $KT_Messages["ToggleVisible"] : "Toggle Visible"; ?>", "togglevisible", "toggleinvis", "toggleInvisibles")</script>
		<?php } ?>
		<?php if (in_array("Toggle WYSIWYG",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["ToggleEditMode"])) ? $KT_Messages["ToggleEditMode"] : "Toggle Edit Mode"; ?>", "html", "toggleedit", "toggleEditMode")</script>
		<?php } ?>
		<?php if (in_array("Clean Word",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["CleanHTMLContent"])) ? $KT_Messages["CleanHTMLContent"] : "CleanHTMLContent - word"; ?>", "cleanword", "cleanup", "CleanHTML")</script>
		<?php } ?>
		<img <?php echo $evstr; ?> class="toolbaritem_flat" 
			 src="<?php echo $relativeImagePath ?>/about.gif" 
			 alt="<?php echo (isset($KT_Messages["About"])) ? $KT_Messages["About"] : "About"; ?>" 
			 title="<?php echo (isset($KT_Messages["About"])) ? $KT_Messages["About"] : "About"; ?>" 
			 kttype="btn" id="about" cid="about"
			 onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.util_checkFocus(this, true)) { ui_openAboutBox(); }" />
		<img <?php echo $evstr; ?> class="toolbaritem_flat" 
			 src="<?php echo $relativeImagePath ?>/help.gif" 
			 alt="<?php echo (isset($KT_Messages["Help"])) ? $KT_Messages["Help"] : "Help"; ?>" 
			 title="<?php echo (isset($KT_Messages["Help"])) ? $KT_Messages["Help"] : "Help"; ?>" 
			 kttype="btn" id="<?php echo 'ktml_'.$objectName; ?>_help" cid="help"
			 onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.toolbar.getHelpMode()) {<?php echo 'ktml_'.$objectName; ?>.toolbar.setHelpMode(false); } else { <?php echo 'ktml_'.$objectName; ?>.toolbar.setHelpMode(true); }" />
		<br/>
		<?php if (in_array("Heading List",$display) || $display[0] =="ALL" ) {?>
	       <select class="ktfonts ktml_select" 
	        	name="property_heading"	cid="property_heading" kttype="slc" id="property_heading"style="width:80px"
				onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('html_tags');return false; }"
				onchange="<?php echo 'ktml_'.$objectName; ?>.logic_doFormat('InsertHeading',this.value)"
			>
			  <option value="<p>">Normal</option>
			  <option value="<p>">Paragraph</option>
			  <option value="<h1>">Heading 1</option>
			  <option value="<h2>">Heading 2</option>		
			  <option value="<h3>">Heading 3</option>
			  <option value="<h4>">Heading 4</option>
			  <option value="<h5>">Heading 5</option>
			  <option value="<h6>">Heading 6</option>
			  <option value="<address>">Address</option>
			  <option value="<pre>">Formatted</option>  
	        </select>
        <?php } ?>
		<?php if (in_array("Style List",$display) || $display[0] =="ALL" ) {?>
        <select class="ktfonts ktml_select" 
        	name="property_styler" cid="property_styler" kttype="slc" id="property_styler"	style="width:90px"
			onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('styler');return false; }"
			onchange="<?php echo 'ktml_'.$objectName; ?>.logic_doFormat('InsertStyle',this.value)">
        	<option value="">None</option>
        </select>
		<?php } ?>
        <?php if (in_array("Font Type",$display) || $display[0] =="ALL" ) {?>
        <select class="ktfonts ktml_select"
	       	name="property_font_type"  kttype="slc" id="property_font_type" cid="Fontname" style="width:160px"
			onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('fontface');return false; } else { return true; }"
        	onchange="<?php echo 'ktml_'.$objectName; ?>.logic_doFormat('FontName',this.value);" >
          <option value="" selected="true"><?php echo (isset($KT_Messages["Select Font..."])) ? $KT_Messages["Select Font..."] : "Select Font..."; ?></option>
          <option value="Arial,Helvetica,sans-serif">Arial, Helvetica, sans-serif</option>
          <option value="Times New Roman,Times,serif" >Times New Roman, Times, serif</option>
          <option value="Courier New,Courier,mono">Courier New, Courier, mono</option>
          <option value="Georgia">Georgia</option>
          <option value="Verdana,Helvetica">Verdana, Helvetica</option>
          <option value="System">System</option>
        </select>
        <?php } ?>
        <?php if (in_array("Font Size",$display) || $display[0] =="ALL" ) {?>
        <select class="ktfonts ktml_select"
        	name="property_size" id="property_size" style="width:50px" kttype="slc" cid="FontSize"
			onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('fontsize');return false; } else { return true; }"
	    	onchange="<?php echo 'ktml_'.$objectName; ?>.logic_doFormat('FontSize',this.value);" >
          <option value="" selected="true"><?php echo (isset($KT_Messages["Select Size..."])) ? $KT_Messages["Select Size..."] : "Select Size..."; ?></option>
          <option value="1">8px</option>
          <option value="2">11px</option>
          <option value="3">14px</option>
          <option value="4">16px</option>
          <option value="5">20px</option>
          <option value="6">24px</option>
        </select>
        <?php } ?>
		<?php if (in_array("Insert Link",$display) || $display[0] =="ALL" ) {?> 
			<script>showToolbarButton("<?php echo (isset($KT_Messages["InsertLink"])) ? $KT_Messages["InsertLink"] : "InsertLink"; ?>", "link", "insertlink", "InsertLink")</script>
		<?php } ?>
		<?php if (in_array("Bold",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Bold"])) ? $KT_Messages["Bold"] : "Bold"; ?>", "bold", "basicformat", "Bold")</script>
		<?php } ?>
		<?php if (in_array("Italic",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Italic"])) ? $KT_Messages["Italic"] : "Italic"; ?>", "italic", "basicformat", "Italic")</script>
		<?php } ?>
		<?php if (in_array("Underline",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["Underline"])) ? $KT_Messages["Underline"] : "Underline"; ?>", "underline", "basicformat", "Underline")</script>
		<?php } ?>
		<?php if (in_array("Foreground Color",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["ForegroundColor"])) ? $KT_Messages["ForegroundColor"] : "ForegroundColor"; ?>", "fgcolor", "colors", "ForeColor")</script>
		<?php } ?>
		<?php if (in_array("Background Color",$display) || $display[0] =="ALL" ) {?>
			<script>showToolbarButton("<?php echo (isset($KT_Messages["BackgroundColor"])) ? $KT_Messages["BackgroundColor"] : "BackgroundColor"; ?>", "bgcolor", "colors", "BackColor")</script>
		<?php } ?>

		<?php if (isset($moduleexists['spellchecker']) && $moduleexists['spellchecker']) {
			include ($dirDepth."includes/ktedit/modules/spellchecker/ui.php");
			echo "<script LANGUAGE=\"JavaScript\" src=\"".$dirDepth."includes/ktedit/modules/spellchecker/scripts.js\"></script>\n";
		} ?>
		<?php if (isset($moduleexists['templates']) && $moduleexists['templates']) {
			echo '<br/>';
			include ($dirDepth."includes/ktedit/modules/templates/ui.php");
			echo '<script LANGUAGE="JavaScript" src="'.$dirDepth.'includes/ktedit/modules/templates/scripts.js"></script>';
		} ?>
		</span>
		</td>
	</tr>
</table>
<div id="<?php echo 'ktml_'.$objectName; ?>_ccdiv" name="<?php echo 'ktml_'.$objectName; ?>_ccdiv" class="cc_container invisible ktml_bg" style="width: 340px; height: 230px;display: none;" >
</div>
<!-- end toolbar code -->
