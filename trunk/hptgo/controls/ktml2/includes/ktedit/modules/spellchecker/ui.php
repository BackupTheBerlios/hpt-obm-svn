<?php if (in_array("SpellCheck",$display) || $display[0] =="ALL" ) {?>	
		<img <?php echo $evstr; ?> class="toolbaritem_flat" 
				 src="<?php echo $relativeImagePath ?>/wizard.gif" 
				 alt="<?php echo (isset($KT_Messages["Spellcheck"])) ? $KT_Messages["Spellcheck"] : "Spellcheck"; ?>" 
				 title="<?php echo (isset($KT_Messages["Spellcheck"])) ? $KT_Messages["Spellcheck"] : "Spellcheck"; ?>" 
				 kttype="btn" 
				 id="spellcheck"
				 cid="spellcheck"
				 onclick="if ( pageLoded() && <?php echo 'ktml_'.$objectName; ?>.util_checkFocus(this, true)) { if (<?php echo 'ktml_'.$objectName; ?>.toolbar.checkHelp(this, event)) { <?php echo 'ktml_'.$objectName; ?>.toolbar.showHelp('spelling');return false; } else { <?php echo 'ktml_'.$objectName; ?>.spellcheck.initsp(null, <?php echo $counter; ?>) }}" />&#160;
<?php } ?>
