
<!--Start Footer-->
<?php
	if (isset($confirm_texts) && is_array($confirm_texts)) {
		foreach ($confirm_texts as $id => $text) {
			echo '<div id="div_confirm_'.$id.'" style="visibility: hidden;display:none">'.htmlspecialchars($text).'</div>'."\n";
		}
	}
?>

</body>
</html>
