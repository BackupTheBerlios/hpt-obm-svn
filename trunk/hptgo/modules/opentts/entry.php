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
$datepicker = new date_picker();
$GO_HEADER['head'] = $datepicker->get_header();

$GO_SECURITY->authenticate();
require($GO_LANGUAGE->get_language_file('opentts'));

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('opentts');
//set the page title for the header file
$page_title = "Opentts";
require($GO_THEME->theme_path."header.inc");

$tts= new db();
require_once("classes.php");

$myagents= new Agents();
$myagents->sql_fetch_array();


$textmenu=menu("entry.php",'');
eval($textmenu);
$tabtable = new tabtable('newticket_tabtable', $helpdesk_title_entry, '100%', '400');
$tabtable->print_head();
if (Security::is_action_allowed("enter_new_ticket")){
	$t_from= Security::get_uname();
	$time=time();
        $post_date=date("{$_SESSION['GO_SESSION']['date_format']} H:i",$time);
        $end_date_d_m_y=$due_date_d_m_y=date("{$_SESSION['GO_SESSION']['date_format']} H:i",$time);
        $end_date_h=$due_date_h=date("H",$time);
        $end_date_i=$due_date_i=round(date("i",$time)/100,1)*100;
        $complete="<select name='complete'  class=textbox>"
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


if (Security::is_action_allowed("imperson")){
$select = new select('user', 'new_ticket', 't_from',whoami());
            $tts_lang_issuer_value=$select->get_link("$tts_lang_issuer");
            $tts_lang_issuer_value=$select->get_field();
}else{
$tts_lang_issuer_value=opentts::get_fullname(whoami());
}
if (Security::is_action_allowed("set_assigned")){
$select = new select('user', 'new_ticket', 't_assigned',whoami());
            $tts_lang_assign_to=$select->get_link("$tts_lang_assign_to");
            $tts_lang_assign_to_value=$select->get_field();
}else{

$tts_lang_assign_to_value="$t_from";
}

$tts_lang_category_value="$t_categories";
$tts_lang_subject_value="<INPUT class=textbox name=t_subject style=\"HEIGHT: 22px; WIDTH: 400px\" value=\"\">";
$tts_lang_description_value="<textarea class=textbox  name=t_description cols=80 rows=12></textarea>";

if (Security::is_action_allowed("set_priority")){
	$tts_lang_priority_value="$t_priorities";
}else{
	$tts_lang_priority_value="";
}
if (Security::is_action_allowed("set_project")){
	$tts_lang_project_value="$project_id";
}else{
	$tts_lang_project_value="";
}
if (Security::is_action_allowed("set_complete")){
        $_PERCENTAGE_COMPLETE_VALUE="$complete";
}else{
        $_PERCENTAGE_COMPLETE_VALUE="0 %";
}
if (Security::is_action_allowed("set_end_date")){
$time=date($_SESSION['GO_SESSION']['date_format'],time());
$tts_lang_end_date_value=$datepicker->get_date_picker('end_date_d_m_y',$_SESSION['GO_SESSION']['date_format'], $time);
$tts_lang_end_date_value.="<select name=\"end_date_h\" class=textbox>";
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
</select>";
$tts_lang_end_date_value.=select_option($end_date_h,$options);
$tts_lang_end_date_value.=":<select name=\"end_date_i\" class=textbox>";
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
</select>
";
$tts_lang_end_date_value.=select_option($end_date_i,$options);
}else{
        $tts_lang_end_date_value="";
}

if (Security::is_action_allowed("set_due_date")){
$time=date($_SESSION['GO_SESSION']['date_format'],time());
$tts_lang_due_date_value=$datepicker->get_date_picker('due_date_d_m_y',$_SESSION['GO_SESSION']['date_format'], $time);

$tts_lang_due_date_value.="<select name=\"due_date_h\" class=textbox>";  
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
</select>";
$tts_lang_due_date_value.=select_option($due_date_h,$options);
	$tts_lang_due_date_value.=":<select name=\"due_date_i\" class=textbox>";  
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
</select>
";
$tts_lang_due_date_value.=select_option($due_date_i,$options);
}else{
	$tts_lang_due_date_value="";
}
$tts_lang_notify_by.=" email <input type=checkbox name=t_email>";

$tts_lang_post_date_value="$post_date";
$_POST="<form name=\"new_ticket\" method=\"POST\" action=\"entry_proc.php\">";
$button = new button();
$action_changes=$button->get_button($cmdOk, "javascript:document.new_ticket.submit()");
$action_changes.='&nbsp;'. $button->get_button($cmdReset, "javascript:document.new_ticket.reset()");


$_ACTION=$action_changes."</form>";

//
$file="themes/$hlpdsk_theme/entry_ticket.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\"$file\");");
echo "<center>$content</center>";
}
$tabtable->print_foot();
 ?>