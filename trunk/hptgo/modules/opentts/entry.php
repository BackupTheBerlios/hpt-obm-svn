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
require("../../Group-Office.php");

//authenticate the user
//if $GO_SECURITY->authenticate(true); is used the user needs admin permissons

$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('opentts');

//set the page title for the header file
$page_title = "Opentts";
require($GO_CONFIG->class_path."opentts.class.inc");
require($GO_THEME->theme_path."header.inc");

$tts= new go_opentts();
require_once("classes.php");

require_once("language/lang-$language.php");
$myagents= new Agents();
$myagents->sql_fetch_array();


$textmenu=menu("entry.php",'');
eval($textmenu);
if (Security::is_action_allowed("enter_new_ticket")){}else{die("you are not allowed");}

OpenTable();
	$t_from= Security::get_uname();
	/*
	$t_date=locale_date("Y-m-d","+2");
	$t_time=locale_date("Y-m-d","+2");
	$t_due_date=locale_date("Y-m-d","+2");
	$t_due_time=locale_date("H:i:s","+2");
	*/
	#$post_date=$due_date=date("Y/m/d H:i",time());
	$time=time();
        $post_date=date("Y/m/d H:i",$time);
        $end_date_d_m_y=$due_date_d_m_y=date("Y/m/d",$time);
        $end_date_h=$due_date_h=date("H",$time);
        $end_date_i=$due_date_i=round(date("i",$time)/100,1)*100;
        $complete="<select name='complete'>"
        ."<option value='0'>0%</option>"
        ."<option value='10'>10%</option>"
        ."<option value='20'>20%</option>"
        ."<option value='30'>30%</option>"
        ."<option value='40'>40%</option>"
        ."<option value='50'>50%</option>"
        ."<option value='60'>60%</option>"
        ."<option value='70'>70%</option>"
        ."<option value='80'>80%</option>"
        ."<option value='90'>90%</option>"
        ."<option value='100'>100%</option>"
        ."</select>";

        $t_categories=fill_select("t_category","{$prefix}{$hlpdsk_prefix}_categories","category_id","category_name"," order by category_name");
	$t_priorities= fill_select("t_priority","{$prefix}{$hlpdsk_prefix}_priorities","priority_id","priority_name"," ");
        $project_id=fill_select("project_id","{$prefix}{$hlpdsk_prefix}_projects","project_id","project_name"," order by project_id");
	# This part of code has to be reviewed
        if (Security::is_action_allowed("post_self")){
                if  (Security::belongs_to_gid(2)){
                        $t_assigned =select_option( whoami(),$myagents->fill_select("t_assigned"));
                }else{
                        $t_assigned =select_option( Security::get_default_agent_id(),$myagents->fill_select("t_assigned"));
                }
        }else{
                $t_assigned =select_option( Security::get_default_agent_id(),$myagents->fill_select("t_assigned"));
        }


	   #$t_assigned =select_option( Security::get_default_agent_id(),$myagents->fill_select("t_assigned"));

# end review here
//
$_ADDING_TICKET=_ADDING_TICKET;
if (Security::is_action_allowed("imperson")){
$_ISSUER=_ISSUER.":<INPUT  name=t_from style=\"HEIGHT: 22px; WIDTH: 100px\" value=\"$t_from\">"
."<a href=\"javascript:popup('../addressbook/select.php?multiselect=false&SET_HANDLER=/../opentts/add_client.php&pass_value=id','550','400')\">...</a>";
}else{
$_ISSUER=_ISSUER.":$t_from";
}
$_CATEGORY=_CATEGORY.":$t_categories";
$_SUBJECT=_SUBJECT."<INPUT  name=t_subject style=\"HEIGHT: 22px; WIDTH: 400px\" value=\"\">";
$_DESCRIPTION=_DESCRIPTION."<BR><TEXTAREA name=t_description style=\"HEIGHT: 145px; WIDTH: 400px\"></TEXTAREA>";
/*
if (Security::is_action_allowed("set_estimated_time")){
	$_ESTIMATED_TIME=_ESTIMATED_TIME.':<INPUT type="text" name="t_est_time" value="--:--"  maxlength=5 style=" WIDTH: 50px">';
}else{
	$_ESTIMATED_TIME="";
}
*/
if (Security::is_action_allowed("set_assigned")){
	$_ASSIGN_TO=_ASSIGN_TO.":$t_assigned";
}else{
	$_ASSIGN_TO="";
}
if (Security::is_action_allowed("set_priority")){
	$_PRIORITY=_PRIORITY.":$t_priorities";
}else{
	$_PRIORITY="";
}
if (Security::is_action_allowed("set_project")){
	$_PROJECT="Project:$project_id";
}else{
	$_PROJECT="";
}
if (Security::is_action_allowed("set_complete")){
        $_PERCENTAGE_COMPLETE="Percentage complete:$complete";
}else{
        $_PERCENTAGE_COMPLETE="Percentage complete:0 %";
}
$_JAVASCRIPT="<script language='javascript' src=\"JavaScript/popcalendar.js\"></script>\n"
."<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"JavaScript/lw_layers.js\"></SCRIPT><SCRIPT LANGUAGE=\"JavaScript\" SRC=\"JavaScript/lw_menu.js\"></SCRIPT>";
if (Security::is_action_allowed("set_end_date")){
$_END_DATE="<input type=button onclick='popUpCalendar(this, new_ticket.end_date_d_m_y, \"yyyy/mm/dd\")' value='select' style='font-size:11px'>\n";
        $_END_DATE.="End Date:<table dir=ltr><tr><td><INPUT  name=end_date_d_m_y size=10 value=\"$end_date_d_m_y\"></td>";
        $_END_DATE.="<td><select name=\"end_date_h\"  onFocus=\"self.status='Hour: when should the ToDo or Phone call be started, it shows up from that dat
e in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\" SELECTED>00</option>
<option value=\"01\">01</option>
<option value=\"02\">02</option>
<option value=\"03\">03</option>
<option value=\"04\">04</option>
<option value=\"05\">05</option>
<option value=\"06\">06</option>
<option value=\"07\">07</option>
<option value=\"08\">08</option>

<option value=\"09\">09</option>
<option value=\"10\">10</option>
<option value=\"11\">11</option>
<option value=\"12\">12</option>
<option value=\"13\">13</option>
<option value=\"14\">14</option>
<option value=\"15\">15</option>
<option value=\"16\">16</option>
<option value=\"17\">17</option>

<option value=\"18\">18</option>
<option value=\"19\">19</option>
<option value=\"20\">20</option>
<option value=\"21\">21</option>
<option value=\"22\">22</option>
<option value=\"23\">23</option>
</select></td>";
$_END_DATE.=select_option($end_date_h,$options);
$_END_DATE.="<td>:</td><td> <select name=\"end_date_i\"  onFocus=\"self.status='Minute: when should the ToDo or Phone call be started, it shows up
from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\" SELECTED>00</option>
<option value=\"05\">05</option>
<option value=\"10\">10</option>
<option value=\"15\">15</option>
<option value=\"20\">20</option>
<option value=\"25\">25</option>
<option value=\"30\">30</option>
<option value=\"35\">35</option>
<option value=\"40\">40</option>

<option value=\"45\">45</option>
<option value=\"50\">50</option>
<option value=\"55\">55</option>
</select></td></tr></table>
";
$_END_DATE.=select_option($end_date_i,$options);
}else{
        $_END_DATE="";
}

if (Security::is_action_allowed("set_due_date")){
$_DUE_DATE="<script language='javascript' src=\"JavaScript/popcalendar.js\"></script>\n"
."<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"JavaScript/lw_layers.js\"></SCRIPT><SCRIPT LANGUAGE=\"JavaScript\" SRC=\"JavaScript/lw_menu.js\"></SCRIPT>";
$_DUE_DATE.="<input type=button onclick='popUpCalendar(this, new_ticket.due_date_d_m_y, \"yyyy/mm/dd\")' value='select' style='font-size:11px'>\n";
	$_DUE_DATE.=_DUE_DATE.":<table dir=ltr><tr><td><INPUT  name=due_date_d_m_y size=10 value=\"$due_date_d_m_y\"></td>";
	$_DUE_DATE.="<td><select name=\"due_date_h\"  onFocus=\"self.status='Hour: when should the ToDo or Phone call be started, it shows up from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\" SELECTED>00</option>
<option value=\"01\">01</option>
<option value=\"02\">02</option>
<option value=\"03\">03</option>
<option value=\"04\">04</option>
<option value=\"05\">05</option>
<option value=\"06\">06</option>
<option value=\"07\">07</option>
<option value=\"08\">08</option>

<option value=\"09\">09</option>
<option value=\"10\">10</option>
<option value=\"11\">11</option>
<option value=\"12\">12</option>
<option value=\"13\">13</option>
<option value=\"14\">14</option>
<option value=\"15\">15</option>
<option value=\"16\">16</option>
<option value=\"17\">17</option>

<option value=\"18\">18</option>
<option value=\"19\">19</option>
<option value=\"20\">20</option>
<option value=\"21\">21</option>
<option value=\"22\">22</option>
<option value=\"23\">23</option>
</select></td>";
$_DUE_DATE.=select_option($due_date_h,$options);
	$_DUE_DATE.="<td>:</td><td> <select name=\"due_date_i\"  onFocus=\"self.status='Minute: when should the ToDo or Phone call be started, it shows up from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\" SELECTED>00</option>
<option value=\"05\">05</option>
<option value=\"10\">10</option>
<option value=\"15\">15</option>
<option value=\"20\">20</option>
<option value=\"25\">25</option>
<option value=\"30\">30</option>
<option value=\"35\">35</option>
<option value=\"40\">40</option>

<option value=\"45\">45</option>
<option value=\"50\">50</option>
<option value=\"55\">55</option>
</select></td></tr></table>
";
$_DUE_DATE.=select_option($due_date_i,$options);
}else{
	$_DUE_DATE="";
}
$_NOTIFY_BY=_NOTIFY_BY.": email <input type=checkbox name=t_email>";
$_PRIV_MSG=_PRIV_MSG;
$_EMAIL=_EMAIL;
$_COMMENTS=_COMMENTS;
$_NOTE_ENTRY_1=_NOTE_ENTRY_1;
$_NOTE_ENTRY_2=_NOTE_ENTRY_2;
$_POST_DATE=_POST_DATE.":$post_date";
$_POST="<form name=\"new_ticket\" method=\"POST\" action=\"entry_proc.php\">";
$_ACTION="<INPUT type=\"submit\" value=\"Submit\"  name=submit>".
        "<INPUT type=\"reset\" value=\"Reset\"  name=reset></form>";

//
$file="themes/$hlpdsk_theme/entry_ticket.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\"$file\");");
echo "<center>$content</center>";
CloseTable();
 ?>
