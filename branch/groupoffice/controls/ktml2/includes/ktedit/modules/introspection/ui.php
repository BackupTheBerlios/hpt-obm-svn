<?php 

$objectName = $HTTP_GET_VARS['ktmlname'];
$dirDepth = $HTTP_GET_VARS['dirDepth'];
$language = $HTTP_GET_VARS['language'];
$relativeImagePath = $dirDepth."images/editor_images";	
$evstr = "onmouseover=\"hndlr_buttonMouseOver(this);\" onmousedown=\"hndlr_buttonMouseDown(this);\"	onmouseup=\"hndlr_buttonMouseUp(this);\" onmouseout=\"hndlr_buttonMouseOut(this);\"	onfocus=\"\" ";

include("../../../../includes/ktedit/modules/introspection/languages/".$language.".inc.php");

?>
<html>
<head>
</head>
<body onLoad="parent.<?php echo $objectName; ?>_transferContent()">

	<div id="Properties_img_<?php echo $objectName; ?>" style="display:none;height: 75px; font-size: 12px; text-align: justify">
			<fieldset class="ktml_fieldset"><legend class="ktml_legend" align="right"><?php echo (isset($KT_Messages["Image Inspector"])) ? $KT_Messages["Image Inspector"] : "Image Inspector"; ?></legend>
		<table border="0" class="introspector" width="100%" cellspacing="0" cellpadding="0">
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Width"])) ? $KT_Messages["Introspection Width"] : "Width:"; ?></td>
				<td nowrap="true" align="right"><div style="display:inline"><input type="text" class="ktml_input" id="Properties_img_width_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onBlur="if (!util_checkFld(this, 3000)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('width', this.value);" onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" rowspan="2" align="left">
					<img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); } else {ktml_<?php echo $objectName; ?>.properties.boundImgSize(this, '<?php echo $relativeImagePath; ?>'); return false; }" 
						src="<?php echo $relativeImagePath; ?>/ubound.gif"
						border="0" <?php echo $evstr; ?>
	    				alt="<?php echo (isset($KT_Messages["Constrain"])) ? $KT_Messages["Constrain"] : "Constrain proportions"; ?>"
						title="<?php echo (isset($KT_Messages["Constrain"])) ? $KT_Messages["Constrain"] : "Constrain proportions"; ?>"
					/>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection HSpace"])) ? $KT_Messages["Introspection HSpace"] : "Hspace:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_img_hspace_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 100)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('hspace', this.value);" onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Align"])) ? $KT_Messages["Introspection Align"] : "Align:"; ?></td>
				<td nowrap="true" >
					<select class="ktml_select" id="Properties_img_align_<?php echo $objectName; ?>" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('align', this.value);">
							<option value=""><?php echo (isset($KT_Messages["Introspection Option Default"]))? $KT_Messages["Introspection Option Default"] : "Default"; ?></option>
							<option value="baseline"><?php echo (isset($KT_Messages["Introspection Option Baseline"]))? $KT_Messages["Introspection Option Baseline"] : "Baseline"; ?></option>
							<option value="top"><?php echo (isset($KT_Messages["Introspection Option Top"]))? $KT_Messages["Introspection Option Top"] : "Top"; ?></option>
							<option value="middle"><?php echo (isset($KT_Messages["Introspection Option Middle"]))? $KT_Messages["Introspection Option Middle"] : "Middle"; ?></option>
							<option value="bottom"><?php echo (isset($KT_Messages["Introspection Option Bottom"]))? $KT_Messages["Introspection Option Bottom"] : "Bottom"; ?></option>
							<option value="texttop"><?php echo (isset($KT_Messages["Texttop"]))? $KT_Messages["Texttop"] : "Texttop"; ?></option>
							<option value="absmiddle"><?php echo (isset($KT_Messages["AbsoluteMiddle"]))? $KT_Messages["AbsoluteMiddle"] : "AbsoluteMiddle"; ?></option>
							<option value="absbottom"><?php echo (isset($KT_Messages["AbsoluteBottom"]))? $KT_Messages["AbsoluteBottom"] : "AbsoluteBottom"; ?></option>
							<option value="left"><?php echo (isset($KT_Messages["Introspection Option Left"]))? $KT_Messages["Introspection Option Left"] : "Left"; ?></option>
							<option value="right"><?php echo (isset($KT_Messages["Introspection Option Right"]))? $KT_Messages["Introspection Option Right"] : "Right"; ?></option>
					</select>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Border"])) ? $KT_Messages["Introspection Border"] : "Border:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_img_border_<?php echo $objectName; ?>" maxlength="4" size="4" onkeyup="PropertiesUI_intValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onBlur="if (!util_checkFld(this, 100)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('border', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" rowspan="2" align="right" valign="bottom"><a target="_blank" href="http://www.interakt.ro/products/KTML" title="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>"><img border="0" alt="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>" src="<?php echo $relativeImagePath; ?>/aboutktml.gif"/></a></td>
			</tr>
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Height"])) ? $KT_Messages["Introspection Height"] : "Height:"; ?></td>
				<td nowrap="true" align="right"><div style="display:inline"><input type="text" class="ktml_input" id="Properties_img_height_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onBlur="if (!util_checkFld(this, 3000)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('height', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection VSpace"])) ? $KT_Messages["Introspection VSpace"] : "VSpace:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_img_vspace_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 100)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('vspace', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection AltText"])) ? $KT_Messages["Introspection AltText"] : "Alt Text:"; ?></td>
				<td nowrap="true" colspan="3"><div style="display:inline"><input id="Properties_img_alt_<?php echo $objectName; ?>" type="text" class="ktml_input" size="26" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('image', './', 'modules/introspection'); }" onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('alt', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
	  		</tr>
		</table>
			</fieldset>
	</div>
	
	<div id="Properties_table_<?php echo $objectName; ?>" style="display:none;height: 75px; font-size: 12px; text-align: justify">
			<fieldset class="ktml_fieldset"><legend class="ktml_legend" align="right"><?php echo (isset($KT_Messages["Table Inspector"])) ? $KT_Messages["Table Inspector"] : "Table Inspector"; ?></legend>				
		<table border="0" class="introspector" width="100%" cellspacing="0" cellpadding="0">
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Width"])) ? $KT_Messages["Introspection Width"] : "Width:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_width_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 1000)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('width', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection CellPadd"])) ? $KT_Messages["Introspection CellPadd"] : "CellPadd:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_cellpadding_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 30)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('cellPadding', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Border"])) ? $KT_Messages["Introspection Border"] : "Border:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_border_<?php echo $objectName; ?>" maxlength="4" size="4" onkeyup="PropertiesUI_intValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onBlur="if (!util_checkFld(this, 30)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('border', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection BGColor"])) ? $KT_Messages["Introspection BGColor"] : "BGColor:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_bgcolor_<?php echo $objectName; ?>" size="8" onkeyup="PropertiesUI_colorValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onBlur="ktml_<?php echo $objectName; ?>.properties.prop_changed('bgColor', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div> <img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); } else { ktml_<?php echo $objectName; ?>.properties.chooseBgColor(this, 'Properties_table_bgcolor_<?php echo $objectName; ?>'); return false;}" src="<?php echo $relativeImagePath; ?>/bgcolor.gif" class="toolbaritem_flat" border="0" <?php echo $evstr; ?> kttype="btn"/></td>
				<td nowrap="true" rowspan="2" align="right" valign="bottom"><a target="_blank" href="http://www.interakt.ro/products/KTML" title="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>"><img border="0" alt="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>" src="<?php echo $relativeImagePath; ?>/aboutktml.gif"/></a></td>
			</tr>
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Height"])) ? $KT_Messages["Introspection Height"] : "Height:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_height_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 1000)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('height', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection CellSpace"])) ? $KT_Messages["Introspection CellSpace"] : "CellSpace:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_cellspacing_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 30)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('cellSpacing', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
					<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Align"])) ? $KT_Messages["Introspection Align"] : "Align:"; ?></td>
				<td nowrap="true" >
					<select  class="ktml_select" id="Properties_table_align_<?php echo $objectName; ?>" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('align', this.value);">
						<option value=""><?php echo (isset($KT_Messages["Introspection Option Default"])) ? $KT_Messages["Introspection Option Default"] : "Default"; ?></option>
						<option value="left"><?php echo (isset($KT_Messages["Introspection Option Left"])) ? $KT_Messages["Introspection Option Left"] : "Left"; ?></option>
						<option value="right"><?php echo (isset($KT_Messages["Introspection Option Right"])) ? $KT_Messages["Introspection Option Right"] : "Right"; ?></option>
						<option value="center"><?php echo (isset($KT_Messages["Introspection Option Center"])) ? $KT_Messages["Introspection Option Center"] : "Center"; ?></option>
					</select>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection BrdColor"])) ? $KT_Messages["Introspection BrdColor"] : "BrdColor:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_table_brdcolor_<?php echo $objectName; ?>" size="8" onkeyup="PropertiesUI_colorValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); }" onBlur="ktml_<?php echo $objectName; ?>.properties.prop_changed('borderColor', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div> <img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('table', './', 'modules/introspection'); } else {ktml_<?php echo $objectName; ?>.properties.chooseBrdColor(this, 'Properties_table_brdcolor_<?php echo $objectName; ?>'); return false; }" src="<?php echo $relativeImagePath; ?>/bgcolor.gif" class="toolbaritem_flat" border="0" <?php echo $evstr; ?> kttype="btn"/></td>
			</tr>
		</table>
			</fieldset>
	</div>
	
	<div id="Properties_td_<?php echo $objectName; ?>" style="display:none;height: 75px; font-size: 12px; text-align: justify">
			<fieldset class="ktml_fieldset" ><legend class="ktml_legend" align="right"><?php echo (isset($KT_Messages["Cell Inspector"])) ? $KT_Messages["Cell Inspector"] : "Cell Inspector"; ?></legend>				
		<table border="0" class="introspector" width="100%" cellspacing="0" cellpadding="0">
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Width"])) ? $KT_Messages["Introspection Width"] : "Width:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_td_width_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 1000)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('width', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Align"])) ? $KT_Messages["Introspection Align"] : "Align:"; ?></td>
				<td nowrap="true" >
					<select  class="ktml_select" id="Properties_td_align_<?php echo $objectName; ?>" 
							onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); }" 
							onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('align', this.value);">
						<option value=""><?php echo (isset($KT_Messages["Introspection Option Default"])) ? $KT_Messages["Introspection Option Default"] : "Default"; ?></option>
						<option value="left"><?php echo (isset($KT_Messages["Introspection Option Left"])) ? $KT_Messages["Introspection Option Left"] : "Left"; ?></option>
						<option value="right"><?php echo (isset($KT_Messages["Introspection Option Right"])) ? $KT_Messages["Introspection Option Right"] : "Right"; ?></option>
						<option value="center"><?php echo (isset($KT_Messages["Introspection Option Center"])) ? $KT_Messages["Introspection Option Center"] : "Center"; ?></option>
					</select>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection BGColor"])) ? $KT_Messages["Introspection BGColor"] : "BGColor:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_td_bgcolor_<?php echo $objectName; ?>" size="8" onkeyup="PropertiesUI_colorValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); }" onBlur="ktml_<?php echo $objectName; ?>.properties.prop_changed('bgColor', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div> <img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); } else {ktml_<?php echo $objectName; ?>.properties.chooseBgColor(this, 'Properties_td_bgcolor_<?php echo $objectName; ?>'); return false; }" src="<?php echo $relativeImagePath; ?>/bgcolor.gif" class="toolbaritem_flat" border="0" <?php echo $evstr; ?> kttype="btn"/></td>
					<td nowrap="true" >&#160;</td>
					<td nowrap="true" >&#160;</td>
					<td nowrap="true" rowspan="2" align="center">
					<?php
						//if (isset($moduleexists['tableedit']) && $moduleexists['tableedit'] && (in_array("TableEdit",$display) || $display[0] =="ALL" )) {
							include ("../../../../includes/ktedit/modules/tableedit/ui.php");
					?>
					<!--script LANGUAGE="JavaScript" src="<!?php echo $dirDepth; ?includes/ktedit/modules/tableedit/scripts.js.php"></script-->
					<?php
						//} 
					?>
					</td>
					<!--
					<td nowrap="true" rowspan="2">
						<a target="_blank" href="http://www.interakt.ro/products/KTML" title="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>"><img border="0" alt="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>" src="<?php echo $relativeImagePath; ?>/aboutktml.gif" width="50"/></a>
					</td>
					-->
			</tr>
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Height"])) ? $KT_Messages["Introspection Height"] : "Height:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_td_height_<?php echo $objectName; ?>" maxlength="4" size="4" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); }" onChange="if (!util_checkFld(this, 1000)) { return false;};ktml_<?php echo $objectName; ?>.properties.prop_changed('height', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection VAlign"])) ? $KT_Messages["Introspection VAlign"] : "VAlign:"; ?></td>
				<td nowrap="true" >
					<select  class="ktml_select"  id="Properties_td_valign_<?php echo $objectName; ?>" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); }" onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('vAlign', this.value);">
						<option value=""><?php echo (isset($KT_Messages["Introspection Option Default"])) ? $KT_Messages["Introspection Option Default"] : "Default"; ?></option>
						<option value="top"><?php echo (isset($KT_Messages["Introspection Option Top"])) ? $KT_Messages["Introspection Option Top"] : "Top"; ?></option>
						<option value="bottom"><?php echo (isset($KT_Messages["Introspection Option Bottom"])) ? $KT_Messages["Introspection Option Bottom"] : "Bottom"; ?></option>
						<option value="baseline"><?php echo (isset($KT_Messages["Introspection Option Baseline"])) ? $KT_Messages["Introspection Option Baseline"] : "Baseline"; ?></option>
						<option value="middle"><?php echo (isset($KT_Messages["Introspection Option Center"])) ? $KT_Messages["Introspection Option Center"] : "Center"; ?></option>
					</select>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection BrdColor"])) ? $KT_Messages["Introspection BrdColor"] : "BrdColor:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_td_brdcolor_<?php echo $objectName; ?>" size="8" onkeyup="PropertiesUI_colorValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); }" onBlur="ktml_<?php echo $objectName; ?>.properties.prop_changed('borderColor', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div> <img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); } else {ktml_<?php echo $objectName; ?>.properties.chooseBrdColor(this, 'Properties_td_brdcolor_<?php echo $objectName; ?>'); return false; }" src="<?php echo $relativeImagePath; ?>/bgcolor.gif" class="toolbaritem_flat" border="0" <?php echo $evstr; ?> kttype="btn"/></td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection NoWrap"])) ? $KT_Messages["Introspection NoWrap"] : "NoWrap:"; ?></td>
				<td nowrap="true" ><input type="checkbox"  id="Properties_td_nowrap_<?php echo $objectName; ?>" onClick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('td', './', 'modules/introspection'); } else { ktml_<?php echo $objectName; ?>.properties.prop_changed('noWrap', this.checked); }"/></td>
			</tr>
		</table>
			</fieldset>
	</div>
	
	<div id="Properties_tr_<?php echo $objectName; ?>" style="display:none;height: 75px; font-size: 12px; text-align: justify">
			<fieldset class="ktml_fieldset" ><legend class="ktml_legend" align="right"><?php echo (isset($KT_Messages["Row Inspector"])) ? $KT_Messages["Row Inspector"] : "Row Inspector"; ?></legend>				
		<table border="0" class="introspector" width="100%" cellspacing="0" cellpadding="0">
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection Align"])) ? $KT_Messages["Introspection Align"] : "Align:"; ?></td>
				<td nowrap="true" >
					<select  class="ktml_select" id="Properties_tr_align_<?php echo $objectName; ?>" 
							onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('tr', './', 'modules/introspection'); }"
							onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('align', this.value);">
						<option value=""><?php echo (isset($KT_Messages["Introspection Option Default"])) ? $KT_Messages["Introspection Option Default"] : "Default"; ?></option>
						<option value="left"><?php echo (isset($KT_Messages["Introspection Option Left"])) ? $KT_Messages["Introspection Option Left"] : "Left"; ?></option>
						<option value="right"><?php echo (isset($KT_Messages["Introspection Option Right"])) ? $KT_Messages["Introspection Option Right"] : "Right"; ?></option>
						<option value="center"><?php echo (isset($KT_Messages["Introspection Option Center"])) ? $KT_Messages["Introspection Option Center"] : "Center"; ?></option>
					</select>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection BGColor"])) ? $KT_Messages["Introspection BGColor"] : "BGColor:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_tr_bgcolor_<?php echo $objectName; ?>" size="8" onkeyup="PropertiesUI_colorValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('tr', './', 'modules/introspection'); }" onBlur="ktml_<?php echo $objectName; ?>.properties.prop_changed('bgColor', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div> <img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('tr', './', 'modules/introspection'); } else {ktml_<?php echo $objectName; ?>.properties.chooseBgColor(this, 'Properties_tr_bgcolor_<?php echo $objectName; ?>'); return false; }" src="<?php echo $relativeImagePath; ?>/bgcolor.gif" class="toolbaritem_flat" border="0" <?php echo $evstr; ?> kttype="btn"/></td>
				<td nowrap="true" rowspan="2" align="right" valign="bottom"><a target="_blank" href="http://www.interakt.ro/products/KTML" title="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>"><img border="0" alt="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>" src="<?php echo $relativeImagePath; ?>/aboutktml.gif"/></a></td>
			</tr>
			<tr>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection VAlign"])) ? $KT_Messages["Introspection VAlign"] : "VAlign:"; ?></td>
				<td nowrap="true" >
					<select  class="ktml_select" id="Properties_tr_valign_<?php echo $objectName; ?>" 
							onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('tr', './', 'modules/introspection'); }"
							onChange="ktml_<?php echo $objectName; ?>.properties.prop_changed('vAlign', this.value);">
						<option value=""><?php echo (isset($KT_Messages["Introspection Option Default"])) ? $KT_Messages["Introspection Option Default"] : "Default"; ?></option>
						<option value="top"><?php echo (isset($KT_Messages["Introspection Option Top"])) ? $KT_Messages["Introspection Option Top"] : "Top"; ?></option>
						<option value="bottom"><?php echo (isset($KT_Messages["Introspection Option Bottom"])) ? $KT_Messages["Introspection Option Bottom"] : "Bottom"; ?></option>
						<option value="baseline"><?php echo (isset($KT_Messages["Introspection Option Baseline"])) ? $KT_Messages["Introspection Option Baseline"] : "Baseline"; ?></option>
						<option value="middle"><?php echo (isset($KT_Messages["Introspection Option Center"])) ? $KT_Messages["Introspection Option Center"] : "Center"; ?></option>
					</select>
				</td>
				<td nowrap="true" ><?php echo (isset($KT_Messages["Introspection BrdColor"])) ? $KT_Messages["Introspection BrdColor"] : "BrdColor:"; ?></td>
				<td nowrap="true" ><div style="display:inline"><input type="text" class="ktml_input" id="Properties_tr_brdcolor_<?php echo $objectName; ?>" size="8" onkeyup="PropertiesUI_colorValue(this);" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('tr', './', 'modules/introspection'); }" onBlur="ktml_<?php echo $objectName; ?>.properties.prop_changed('borderColor', this.value);"  onkeypress="return util_preventEvent2(this, event);"/></div> <img onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('tr', './', 'modules/introspection'); } else {ktml_<?php echo $objectName; ?>.properties.chooseBrdColor(this, 'Properties_tr_brdcolor_<?php echo $objectName; ?>'); return false; }" src="<?php echo $relativeImagePath; ?>/bgcolor.gif" class="toolbaritem_flat" border="0" <?php echo $evstr; ?> kttype="btn"/></td>
			</tr>
		</table>
			</fieldset>
	</div>
</body>
</html>