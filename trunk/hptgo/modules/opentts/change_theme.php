<?php

/************************************************************************/
/* TTS: Ticket tracking system                                          */
/* ============================================                         */
/*                                                                      */
/* Copyright (c) 2002 by Meir Michanie                                  */
/* http://www.riunx.com                                                 */
/*                                                                      */
/* This program is free software. You can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License.       */
/************************************************************************/
if (!eregi("modules.php", $PHP_SELF)) {
    die ("You can't access this file directly...");
}
include_once("mainfile.php");
if (isset($submit)){
	write_config($theme_selected);
}else{
	select_themes();
}

function get_themes(){
global $name;
$themes= null;
if ($handle = opendir("modules/$name/themes")) {
   echo "Themes:\n";

   /* This is the correct way to loop over the directory. */
   while (false !== ($file = readdir($handle))) { 
	if (is_dir("modules/$name/themes/$file")  and $file!="." and  $file !=".." and $file!="CVS"){
	       $themes[]="$file";
	}
   }
   closedir($handle); 
}
return $themes;
}

function select_themes(){
global $name;
	if (is_file("modules/$name/configure.php")){
		$file=file("modules/$name/configure.php");
		foreach ($file as $line){
			if (strstr($line,"\$hlpdsk_theme")){
				$theme_selected=substr(strstr($line,'"'),1,-3);
				$show= "theme selected<br>$theme_selected<br>";
			}
		}
		$themes=get_themes();
		foreach($themes as $value){
			if ($value==$theme_selected){
				$options.="<option value=\"$value\" selected>$value</option>\n";				
			}else{
				$options.="<option value=\"$value\">$value</option>\n";
			}
		}
		$theme_select= "<select name='theme_selected'>$options</select>";
		echo "<form action='modules.php?name=$name&file=admin&func=change_theme' method='POST'>";
		echo "<table>"
			."<tr>"
			."<td>$theme_select</td>"
			."</tr>"
			."</table>";
		echo "<input name=submit type=submit></form>";
	}
		
}
#tts version counter.
function change_theme($theme_selected){
global $name;
$hlpdsk_theme="$theme_selected";
$file=file("modules/$name/configure.php");
$fp1=fopen ("modules/$name/configure.php","w");
foreach ($file as $line){
	if (strstr($line,"\$hlpdsk_theme")){
		fwrite($fp1,"\$hlpdsk_theme=\"$theme_selected\";\n");
	}else{
		fwrite($fp1,$line);
	}
}
fclose($fp1);
}
?>
