<?php

function draw_button($url,$title,$selected=FALSE){
	global $name;
	if ($selected) $class="<font color=red>"; else $class="<font color=white>";
	return "<td><a href=\"$url\">$class$title</font></a></td>";
}
?>
