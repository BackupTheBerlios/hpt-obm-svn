	<script>
		function showHelp(el, event) {
			if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(el, event)) { 
				<?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('linkinspector', './', 'modules/linkintrospector'); 
				return false; 
			} else { 
				return true; 
			}
		}
	</script>	


	<div id="Properties_none_<?php echo $objectName; ?>" style="display:block;height: 75px; font-size: 12px; text-align: justify">
				<fieldset class="ktml_fieldset"><legend class="ktml_legend" align="right"><?php echo (isset($KT_Messages["Property Inspector"])) ? $KT_Messages["Property Inspector"] : "Property Inspector"; ?></legend>
	  		<table class="introspector" width="100%" cellspacing="0" cellpadding="0">
	  			<tr>
					<td style="font-size: 12px; padding: 5px">
					<?php echo (isset($KT_Messages["Panel description"])) ? $KT_Messages["Panel description"] : 
					'This is the "Propery Inspector" panel. Special options will appear in this space once you select elements like TABLE, TR, TD, A, IMG.'; 
					?>
					</td>
					<td align="left" valign="bottom"><a target="_blank" href="http://www.interakt.ro/products/KTML" title="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>"><img border="0" alt="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>" src="<?php echo $relativeImagePath; ?>/aboutktml.gif"/></a></td>
				</tr>
			</table>
				</fieldset>
		</div>
		
		
	  	<div id="Properties_a_<?php echo $objectName; ?>" style="display:none;height: 75px; font-size: 12px; text-align: justify">
				<fieldset class="ktml_fieldset"><legend class="ktml_legend" align="right"><?php echo (isset($KT_Messages["Link Inspector"])) ? $KT_Messages["Link Inspector"] : "Link Inspector"; ?></legend>
	  		<table class="introspector" width="100%" cellspacing="0" cellpadding="0">
	  			<tr>
	  				<td align="right" style="padding-right: 2px;">
						<?php echo (isset($KT_Messages["Link Name"])) ? $KT_Messages["Link Name"] : "Name:"; ?>					
					</td>
						<td >
						<div style="display:inline">
								<input type="text" style="width:120px" class="ktml_input" id="Properties_a_name_<?php echo $objectName; ?>" 
								onclick="showHelp(this, event)" 
								onFocus="if(!ktml_<?php echo $objectName; ?>.toolbar.helpMode) { ktml_<?php echo $objectName; ?>.propertieslink.name_focus('<?php if (isset($KT_Messages["Link2Anchor"])) { echo $KT_Messages["Link2Anchor"]; } else { echo "Link2Anchor"; } ?>'); }" 
								onChange="ktml_<?php echo $objectName; ?>.propertieslink.name_changed(this.value);"
								onkeydown="return util_preventEvent2(this, event);"
								onkeypress="return util_preventEvent2(this, event);"
								/>
						</div>
				    </td>
					<td align="right" style="padding-right: 2px;" nowrap="true"><?php echo (isset($KT_Messages["Link Title"])) ? $KT_Messages["Link Title"] : "Title:"; ?></td>
					<td>
						<div style="display:inline">
								<input type="text" style="width:120px" class="ktml_input" id="Properties_a_title_<?php echo $objectName; ?>" 
								onclick="showHelp(this, event)" 
								onChange="ktml_<?php echo $objectName; ?>.propertieslink.title_changed(this.value);"
								onkeydown="return util_preventEvent2(this, event);"
								onkeypress="return util_preventEvent2(this, event);"
								/>
						</div>
					</td>
	  				<td align="right" style="padding-right: 2px;"><?php echo (isset($KT_Messages["Link Target"])) ? $KT_Messages["Link Target"] : "Link:"; ?></td>
	  				<td>
	  					<select 
								onkeydown="return util_preventEvent2(this, event);"
								class="ktml_select" onkeypress="return util_preventEvent2(this, event);"
								style="width:100px"
								onclick="showHelp(this, event)"
								id="Properties_a_target_<?php echo $objectName; ?>" 
								onChange="ktml_<?php echo $objectName; ?>.propertieslink.target_changed(this.value);"
							>
								<option value=""><?php echo (isset($KT_Messages["Link Option normal"])) ? $KT_Messages["Link Option normal"] : "normal"; ?></option>
								<option value="_blank"><?php echo (isset($KT_Messages["Link Option newwindow"])) ? $KT_Messages["Link Option newwindow"] : "new window"; ?></option>
								<option value="_top"><?php echo (isset($KT_Messages["Link Option currentwindow"])) ? $KT_Messages["Link Option currentwindow"] : "current window"; ?></option>
								<option value="_self"><?php echo (isset($KT_Messages["Link Option currentframe"])) ? $KT_Messages["Link Option currentframe"] : "current frame"; ?></option>
								<option value="_parent"><?php echo (isset($KT_Messages["Link Option parentframe"])) ? $KT_Messages["Link Option parentframe"] : "parent frame"; ?></option>
	  					</select>
	  				</td>
					<td rowspan="2" align="right" valign="bottom"><a target="_blank" href="http://www.interakt.ro/products/KTML" title="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>"><img border="0" alt="<?php echo (isset($KT_Messages["About Ktml"])) ? $KT_Messages["About Ktml"] : "About Ktml"; ?>" src="<?php echo $relativeImagePath; ?>/aboutktml.gif"/></a></td>
	  			</tr>
	  			<tr>
	  				<td align="right" style="padding-right: 2px;"><?php echo (isset($KT_Messages["Link Href"])) ? $KT_Messages["Link Href"] : "Href:"; ?></td>
	  				<td colspan="3">
							<div style="display:inline;">
							<input type="text" class="ktml_input" style="width:300px" id="Properties_a_href_<?php echo $objectName; ?>" 
								onclick="showHelp(this, event)" 
								onFocus="if(!ktml_<?php echo $objectName; ?>.toolbar.helpMode) { ktml_<?php echo $objectName; ?>.propertieslink.href_focus('<?php if (isset($KT_Messages["Anchor2Link"])) { echo $KT_Messages["Anchor2Link"]; } else { echo "Anchor2Link"; } ?>'); }"

								onChange="ktml_<?php echo $objectName; ?>.propertieslink.href_changed(this.value);"
								onkeydown="return util_preventEvent2(this, event);"
								onkeypress="return util_preventEvent2(this, event);"
								/>
							</div> 
	  				</td>
					<td colspan="2" style="padding-left:10px">
							<img kttype="btn" <?php echo $evstr; ?> alt="<?php echo (isset($KT_Messages["Link Browse"])) ? $KT_Messages["Link Browse"] : "Browse:"; ?>" title="<?php echo (isset($KT_Messages["Link Browse"])) ? $KT_Messages["Link Browse"] : "Browse:"; ?>" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('linkinspector', './', 'modules/linkintrospector'); } else { ktml_<?php echo $objectName; ?>.propertieslink.chooseFileLink(this, 'Properties_a_href_<?php echo $objectName; ?>',<?php echo $counter; ?>); return false;}" src="<?php echo $relativeImagePath; ?>/dir.gif" class="toolbaritem_flat" border="0"
							/>
							<img kttype="btn" <?php echo $evstr; ?> alt="<?php echo (isset($KT_Messages["Link Remove"])) ? $KT_Messages["Link Remove"] : "Remove:"; ?>" title="<?php echo (isset($KT_Messages["Link Remove"])) ? $KT_Messages["Link Remove"] : "Remove:"; ?>" onclick="if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('linkinspector', './', 'modules/linkintrospector'); } else { ktml_<?php echo $objectName; ?>.logic_doFormat('RemoveLink'); return false; }" src="<?php echo $relativeImagePath; ?>/linkremove.gif" class="toolbaritem_flat" border="0"
							/>
					</td>
	  			</tr>
	  		</table>
				</fieldset>
			</div>
