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
	write_config();
}else{
	show_vars();
}
function get_themes(){
global $name;
$themes= null;
if ($handle = opendir("modules/$name/themes")) {
   echo "Directory handle: $handle\n";
   echo "Files:\n";

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

function show_vars(){
global $name;
	if (is_file("modules/$name/configure.php")){
		$file=file("modules/$name/configure.php");
		$strfile=join("",$file);
		#eval("\n>\n$file\n<?php\n ");
		echo "<table>"
			."<tr>"
			."<td><textarea name=config_file cols=80 rows=25>$strfile</textarea></td>"
			."</tr>"
			."</table>";
		foreach ($file as $line){
			if (strstr($line,"\$hlpdsk_theme")){
				$theme_selected=substr(strstr($line,'"'),1,-3);
				echo "theme selected<br>$theme_selected<br>";
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
		echo "<select>$options</select>";
	}else{
		write_config();
	}
		
}
#tts version counter.
function write_config(){
global $name;
$tts_version="2.0.1-alpha";

$php_nuke_version="6";

if ($php_nuke_version=="6"){
	# for phpnuke 6.8 uncomment this:

	$nuke_user_id_fieldname="user_id";
	$nuke_username_fieldname="username";
	$nuke_user_website="user_website";
	$nuke_user_email_fieldname="user_email";

	#-----
	}else{

	# for older uncomment this.
	$nuke_user_id_fieldname="uid";
	$nuke_username_fieldname="uname";
	$nuke_user_website="url";
	$nuke_user_email_fieldname="email";

	#-----
}
/*
Default values
*/

$hlpdsk_prefix="_opentts";

$hlpdsk_email="support@riunx.com";
$hlpdsk_gmt_diff="+2";
$tts_demo=FALSE;
$rtl_dir="ltr";
$charset="windows-1255";
$hlpdsk_theme="Aqua";
$classes_dir="$name";

$fp1=fopen ("modules/$name/configure.php","w");
fwrite($fp1,"<?php\n");
fwrite ($fp1,"\$nuke_user_id_fieldname=\"$nuke_user_id_fieldname\";\n");
fwrite ($fp1,"\$nuke_username_fieldname=\"$nuke_username_fieldname\";\n");
fwrite ($fp1,"\$nuke_user_website=\"$nuke_user_website\";\n");
fwrite ($fp1,"\$nuke_user_email_fieldname=\"$nuke_user_email_fieldname\";\n");

fwrite ($fp1,"\$hlpdsk_prefix=\"$hlpdsk_prefix\";\n");
fwrite ($fp1,"\$hlpdsk_email=\"$hlpdsk_email\";\n");
fwrite ($fp1,"\$hlpdsk_gmt_diff=\"$hlpdsk_gmt_diff\";\n");
fwrite ($fp1,"\$tts_demo=\"$tts_demo\";\n");
fwrite ($fp1,"\$rtl_dir=\"$rtl_dir\";\n");
fwrite ($fp1,"\$charset=\"$charset\";\n");
fwrite ($fp1,"\$hlpdsk_theme=\"$hlpdsk_theme\";\n");
fwrite ($fp1,"\$classes_dir=\"$classes_dir\";\n");
fwrite($fp1,"\n?>\n");
fclose($fp1);
}
?>
