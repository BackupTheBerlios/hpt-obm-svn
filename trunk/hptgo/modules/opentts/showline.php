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
$Ticket_Number=$_GET['Ticket_Number'];
require_once("language/lang-$language.php");

global $name;
$textmenu=menu("Show_Tickets",'');
eval($textmenu);
OpenTable();
echo "
<br>

<H1 align=center>"._TTS_DETAILS."</H1>";
#
showrecords();
if(isset($printer)){;}else{
showtasks();
show_new_task($Ticket_Number);
#
}
CloseTable();

//functions

function showtasks(){
	global $Ticket_Number,$name,$tts,$prefix,$hlpdsk_prefix,$hlpdsk_theme;
	#if (!Security::is_action_allowed("view_all_tasks")) $query_condition = " and (sender_id='".whoami()."' or rcpt_id='".whoami()."')";
	if (!Security::is_action_allowed("view_all_tasks")) $query_condition = " and (sender_id='".whoami()."')";else $query_condition = '';

	if (Security::is_action_allowed("view_tasks")){ 
	$query="select * from {$prefix}{$hlpdsk_prefix}_tasks where ticket_id='$Ticket_Number' $query_condition order by task_id asc";
	#if ($result=sql_query($query,$dbi)){
	if ($tts->query($query)){
		$file="themes/$hlpdsk_theme/showline_task.html";
		$file=addslashes (implode("",file($file)));
		$_POST_DATE=_POST_DATE;
		$_ISSUER='Issuer:';
		$_ASSIGNED='Assigned:';
		$_SENDER='Sender:';
		$_COMMENT='Comment:';
		$_MIDDLE='';
		while($tts->next_record()){
		$POST_DATE="<tr><td><font class=content>".date("Y/m/d",$tts->f('post_date'))."</td>";
			$SENDER="<td><font class=content>".Security::get_uname($tts->f('sender_id'))."</td>";
			$comment=nl2br(str_replace("<","\<",$tts->f('comment')));
			$COMMENT="<td><font class=content>{$comment}</td></tr>";
			$_ACTION="";
			$_MAIL_THIS="";
			$_MIDDLE.=$POST_DATE.$SENDER.$COMMENT;
		}
		eval("\$content=stripslashes(\"$file\");");
		echo $content;
	}
	}
}

function show_new_task($ticket_number){
	global	$name,$hlpdsk_theme;
	if (Security::is_action_allowed("enter_new_task")){
	$sender=Security::get_uname();
	$_SENDER="Sender: $sender";
	$_EMAIL="Email parties: <input type=checkbox name=t_email value=1>";
	$_COMMENT="Comment:<br><textarea name=comment cols=80 rows=12></textarea>";
	#$_START_POST="<form name=new_task method=POST action=\"task&func=new_task&ticket_number=$ticket_number\">";
	$_START_POST="<form name=new_task method=POST action=\"change_ticket.php?func=add_task&Ticket_Number=$ticket_number\">";
	$_END_POST="<input name=submit value=submit type=submit></form>";	
	$_POST_DATE='';
	$_MAIL_THIS='';
	$file="themes/$hlpdsk_theme/showline_new_task.html";
	$file=addslashes (implode("",file($file)));		
	eval("\$file=stripslashes(\"$file\");");
	echo $file;
	}
} 


function showrecords(){
global $Ticket_Number,$name,$tts,$prefix,$hlpdsk_prefix,$hlpdsk_theme;
if (!Security::is_action_allowed("view_all_tickets")) $query_condition = " and (t_from='".whoami()."' or t_assigned='".whoami()."')"; else $query_condition = '';
$querytext="select * from {$prefix}{$hlpdsk_prefix}_tickets where Ticket_Number='$Ticket_Number' $query_condition ";
$tts->query($querytext); 
$recordcount=$tts->num_rows();
$row=0;
if ($recordcount=0) exit;
#while ($data = sql_fetch_object($result, $dbi)):
while ($tts->next_record()):
		$post_date=$tts->f('post_date');
		$due_date=$tts->f('due_date');
		$end_date=$tts->f('end_date');
		$complete=$tts->f('complete');
		$t_from=$tts->f('t_from');
		$t_stage=$tts->f('t_stage');
		$t_category=$tts->f('t_category');
		$t_priority=$tts->f('t_priority');
		$t_subject=htmlspecialchars($tts->f('t_subject'));
		$t_description=htmlspecialchars($tts->f('t_description'));
		$t_description=str_replace("\n"," <br> ",$t_description);
		$t_assigned=$tts->f('t_assigned');
		$t_email=$tts->f('t_email');
		$t_sms=$tts->f('t_sms');
		$t_status=$tts->f('t_status');
		$change_date=htmlspecialchars($tts->f('change_date'));
		$activity_id=$tts->f('activity_id');
		$project_id=$tts->f('project_id');
$due_date=date("Y/m/d H:i",$due_date);
$end_date=date("Y/m/d H:i",$end_date);
if ($t_sms=="on") $t_sms=" CHECKED";
if ($t_email=="on") $t_email=" CHECKED";
$action_changes="<center><INPUT type=\"submit\" value=\"Submit\"  name=submit>".
	"<INPUT type=\"reset\" value=\"Reset\"  name=reset></form></center>";
$_TICKET_NR="Ticket Number:";
$_POST_DATE="Post date:". date("Y/m/d H:i",$post_date);
#$_ISSUER=_ISSUER.":".Security::get_uname($t_from);
$t_from=Security::get_uname($t_from);
if (Security::is_action_allowed("imperson")){
$_ISSUER="Issuer:<INPUT  name=t_from style=\"HEIGHT: 22px; WIDTH: 100px\" value=\"$t_from\">"
."<a href=\"javascript:popup('/~meirm/groupoffice-1.94/modules/addressbook/select.php?multiselect=false&SET_HANDLER=/~meirm/groupoffice-1.94/modules/opentts/add_client.php&pass_value=id','550','400')\">...</a>";
}else{
$_ISSUER="Issuer:$t_from";
}


$stage_name=get_cross_value("{$prefix}{$hlpdsk_prefix}_stages","stage_name"," where stage_id='$t_stage'");
$category_name=get_cross_value("{$prefix}{$hlpdsk_prefix}_categories","category_name"," where category_id='$t_category'");
$project_name=get_cross_value("{$prefix}{$hlpdsk_prefix}_projects","project_name"," where project_id='$project_id'");
$select_complete="<select name='complete'>"
        ."<option value='0' >0%</option>"
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

$t_assigned_name=Security::whatsmyname($t_assigned);
if (Security::is_action_allowed("change_subject")){
	$_SUBJECT="Subject: <input name=t_subject value=\"$t_subject\" max=128 size=80>";	
}else{
	$_SUBJECT="Subject:$t_subject";
}
$_DESCRIPTION="Description:<br>$t_description";
$_CHANGE_DATE="Change date:".date("Y/m/d H:i",$change_date);
$_NOTIFY_BY='Notify by';
$_PRIV_MSG='mesg';
$_EMAIL='email';


$post_changes="<form name=\"change_status\" method=\"POST\" action=\"change_ticket.php?Ticket_Number=$Ticket_Number&func=change_status\">";
if (Security::is_action_allowed("change_project")){
	 $project_name= select_option("$project_id",fill_select("project_id","{$prefix}{$hlpdsk_prefix}_projects","project_id","project_name"," order by project_id"));
	$_PROJECT="Project:$project_name";
}else{
	$project_name=get_cross_value("{$prefix}{$hlpdsk_prefix}_projects","project_name"," where project_id='$project_id'");
        $_PROJECT="Project:$project_name";

}

if (Security::is_action_allowed("change_assigned")){
	$myagents= new Agents();
	$myagents->sql_fetch_array();
	$t_assigned =select_option( "$t_assigned",$myagents->fill_select("t_assigned"));

	$_ASSIGN_TO="Assigned:$t_assigned&nbsp;";
}else{
	$_ASSIGN_TO="Assigned:".Security::get_uname($t_assigned);
}
$_JAVASCRIPT="<script language='javascript' src=\"JavaScript/popcalendar.js\"></script>\n"
."<SCRIPT LANGUAGE=\"JavaScript\" SRC=\"JavaScript/lw_layers.js\"></SCRIPT><SCRIPT LANGUAGE=\"JavaScript\" SRC=\"JavaScript/lw_menu.js\"></SCRIPT>";
if (Security::is_action_allowed("change_end_date")){
        $time=strtotime($end_date);
        $end_date_d_m_y=date("Y/m/d",$time);
        $end_date_h=date("H",$time);
        $end_date_i=date("i",$time);
$end_date="<input type=button onclick='popUpCalendar(this, change_status.end_date_d_m_y, \"yyyy/mm/dd\")' value='select' style='font-size:11px'>\n";

        $end_date.="End date:<table dir=ltr><tr><td><INPUT  name=\"end_date_d_m_y\" size=10 value=\"$end_date_d_m_y\"></td>";
        $end_date.="<td><select name=\"end_date_h\"  onFocus=\"self.status='Hour: when should the ToDo or Phone call be started, it shows up from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\">00</option>
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
$end_date.=select_option($end_date_h,$options);
        $end_date.="<td>:</td><td> <select name=\"end_date_i\"  onFocus=\"self.status='Minute: when should the ToDo or Phone call be started, it shows up from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\">00</option>
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
$end_date.=select_option($end_date_i,$options);


        $_END_DATE="$end_date&nbsp;";
}else{
        $_END_DATE="End Date:$end_date";
}
if (Security::is_action_allowed("change_complete")){
        $_PERCENTAGE_COMPLETE="Completed:".select_option( "$complete","$select_complete");
}else{
        $_PERCENTAGE_COMPLETE="Completed:$complete %";
}

if (Security::is_action_allowed("change_due_date")){
	$time=strtotime($due_date);
	$due_date_d_m_y=date("Y/m/d",$time);
	$due_date_h=date("H",$time);
	$due_date_i=date("i",$time);
$due_date.="<input type=button onclick='popUpCalendar(this, change_status.due_date_d_m_y, \"yyyy/mm/dd\")' value='select' style='font-size:11px'>\n";

	$due_date.="Due date:<table dir=ltr><tr><td><INPUT  name=\"due_date_d_m_y\" size=10 value=\"$due_date_d_m_y\"></td>";
        $due_date.="<td><select name=\"due_date_h\"  onFocus=\"self.status='Hour: when should the ToDo or Phone call be started, it shows up from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\">00</option>
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
$due_date.=select_option($due_date_h,$options);
        $due_date.="<td>:</td><td> <select name=\"due_date_i\"  onFocus=\"self.status='Minute: when should the ToDo or Phone call be started, it shows up from that date in the filter open or own open (startpage)'; return true;\" onBlur=\"self.status=''; return true;\">";
$options="
<option value=\"00\">00</option>
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
$due_date.=select_option($due_date_i,$options);


	$post="<form name=\"change_status\" method=\"POST\" action=\"change_ticket.php?Ticket_Number=$Ticket_Number&func=change_due_date\">";
	$_DUE_DATE="$due_date&nbsp;";
}else{
	$_DUE_DATE="Due date:$due_date";
}
if (Security::is_action_allowed("change_activity")){
        $activity=select_option("$activity_id",fill_select("activity_id","{$prefix}{$hlpdsk_prefix}_activities","activity_id","activity_name"," "));
        $_ACTIVITY="Activity:$activity&nbsp;";
}else{
        $_ACTIVITY="Activity:$activity";
}

if (Security::is_action_allowed("change_status")){
	$t_status_sel=select_option("$t_status",fill_select("t_status","{$prefix}{$hlpdsk_prefix}_status","status_id","status_name"," "));
	$_STATUS="Status:<br>$t_status_sel";
}else{
	$status_name=get_cross_value("{$prefix}{$hlpdsk_prefix}_status","status_name"," where status_id='$t_status'");
	$_STATUS="Status:$status_name";
}
if (Security::is_action_allowed("change_priority")){
	$t_priorities= select_option("$t_priority",fill_select("t_priority","{$prefix}{$hlpdsk_prefix}_priorities","priority_id","priority_name"," "));
$_PRIORITY="Priority:<br>$t_priorities<br>";
}else{
	$t_priority_name=get_cross_value("{$prefix}{$hlpdsk_prefix}_priorities","priority_name"," where priority_id=$t_priority");
	$_PRIORITY="Priority:$t_priority_name";
}
if (Security::is_action_allowed("change_category")){
        $t_category= select_option("$t_category",fill_select("t_category","{$prefix}{$hlpdsk_prefix}_categories","category_id","category_name"," "));
	$_CATEGORY="Category:<br>$t_category<br>";
}else{
	$_CATEGORY="Category:$category_name";
}
if (Security::is_action_allowed("change_stage")){
        $t_stage= select_option("$t_stage",fill_select("t_stage","{$prefix}{$hlpdsk_prefix}_stages","stage_id","stage_name"," "));
	$_STAGE="Stage:<br>$t_stage<br>";
}else{
	$_STAGE="Stage:$stage_name";
}

$mailto_subject= "?subject=".addslashes("Ticket Task  $Ticket_Number: "). addslashes($t_subject);
$mailto_body= "&body=".addslashes("Ticket/Task:   $Ticket_Number / "  ).addslashes($t_description);
$mailto= $mailto_subject . $mailto_body;
$_MAIL_THIS="<a href=\"mailto:$mailto\">Send email</a>";

# for JCE requierments
/*
if (Security::is_action_allowed("plugin_view_student_details")){

$_ISSUER_DETAILS=<<<EOF
<a href="javascript:void(0);" onclick="window.open('plugins/stud_details&uid=$t_from','new','width=200,height=200,location=yes');"> . . . </a>
EOF;

}else{

$_ISSUER_DETAILS="";

}

*/
$_ESTIMATED_TIME=$_MONEY='';
$_ACTION="";
$_MAIL_THIS="";
#
$file="themes/$hlpdsk_theme/showline_ticket.html";
$file=addslashes (implode("",file($file)));
#$file=implode("",file($file));
eval("\$content=stripslashes(\" $file\");");
echo $content;

 $row++;
endwhile;

}
?>
