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

#get new ticket number
/*
$Task_Number="1";
$due_date=strtotime($due_date);
$priority_id=$t_priority;
$status_id=$t_status;
*/	
$func='';
if (isset($_GET['func'])) $func=$_GET['func'];
$Ticket_Number=$_GET['Ticket_Number'];
if (Ticket::ticket_related($Ticket_Number) or Security::is_action_allowed("change_all_tickets")){
	if(Security::is_action_allowed("change_status") && isset($_POST['t_status']) ){
			change_status($Ticket_Number,$_POST['t_status']);
		}		
	if(Security::is_action_allowed("change_priority") && isset($_POST['t_priority'])){
	                change_priority($Ticket_Number,$_POST['t_priority']);
		}
	if(Security::is_action_allowed("change_category") && isset($_POST['t_category'])){
	                change_category($Ticket_Number,$_POST['t_category']);
		}
	if(Security::is_action_allowed("change_stage") && isset($_POST['t_stage'])){
	                change_stage($Ticket_Number,$_POST['t_stage']);
		}

	if(Security::is_action_allowed("change_due_date") && isset( $_POST['due_date_d_m_y'])){
			
			#$due_date=strtotime("$due_date_d_m_y $due_date_h:$due_date_i:00" );
			change_due_date($Ticket_Number,$_POST['due_date_d_m_y'],$_POST['due_date_h'],$_POST['due_date_i']);
		}
        if(Security::is_action_allowed("change_assigned") && isset( $_POST['t_assigned'])){
                        change_assigned($Ticket_Number,$_POST['t_assigned']);
                }
        if(Security::is_action_allowed("change_activity") && isset($_POST['activity_id'])){
                        change_activity($Ticket_Number,$_POST['activity_id']);
                }
         if(Security::is_action_allowed("enter_new_task") && isset(  $_POST['comment'] )){
                        add_task($Ticket_Number,$_POST['comment']);
                }
	if(Security::is_action_allowed("change_subject") && isset($_POST['t_subject'])){
                        change_subject($Ticket_Number,$_POST['t_subject']);
                }
	if(Security::is_action_allowed("change_project") && isset($_POST['project_id'])){
                        change_project($Ticket_Number,$_POST['project_id']);
                }
	if(Security::is_action_allowed("change_issuer") && isset($_POST['t_from'])){
                        change_issuer($Ticket_Number,$_POST['t_from']);
                }

	show_html($Ticket_Number,'');
}
#
function add_task($Ticket_Number,$comment){
global $func;
        $sender=whoami();
	
        Task::insert_new_task($Ticket_Number,$sender,$comment);
	if ($func=='add_task'){
	notify_change($Ticket_Number,$comment);
	}
	
}
function change_subject($Ticket_Number,$t_subject){
        $last_subject=Ticket::get_ticket_subject($Ticket_Number);
        if($last_subject<>$t_subject){
        $result=Ticket::change_subject($Ticket_Number,$t_subject);
        $comment="Ticket subject changed from \'$last_subject\'  to: \'$t_subject\' ";
        add_task($Ticket_Number,$comment);
        notify_change($Ticket_Number,$comment);
        }
}

function change_project($Ticket_Number,$project_id){
	$last_project_id=Ticket::get_ticket_project_id($Ticket_Number);
        if ($last_project_id<>$project_id){
        $last_project_name=Ticket::get_project_name($last_project_id);
        $new_project_name=Ticket::get_project_name($project_id);
        $result=Ticket::change_project_id($Ticket_Number,$project_id);
        $sender=whoami();
        $comment="Project changed from \'$last_project_name\' to \'$new_project_name\'";
        add_task($Ticket_Number,$comment);
        notify_change($Ticket_Number,$comment);
        }

}


function change_assigned($Ticket_Number,$t_assigned){
	$last_assigned_uid=Ticket::get_ticket_assigned_uid($Ticket_Number);
	if($last_assigned_uid<>$t_assigned){
	$last_assigned=Security::get_uname($last_assigned_uid);
        $result=Ticket::change_assigned($Ticket_Number,$t_assigned);
	$new_assigned=Security::get_uname($t_assigned);
	$comment="Ticket re-assigned from \'$last_assigned\'  to: \'$new_assigned\' ";
        add_task($Ticket_Number,$comment);
	notify_change($Ticket_Number,$comment);
	}
}

function change_issuer($Ticket_Number,$issuer){
        $last_issuer_uid=Ticket::get_ticket_issuer_uid($Ticket_Number);
	$new_issuer_uid=Security::get_uid($issuer);
        if($last_issuer_uid<>$new_issuer_uid){
        $last_issuer=Security::get_uname($last_issuer_uid);
        $result=Ticket::change_issuer($Ticket_Number,$new_issuer_uid);
        $new_issuer=Security::get_uname($new_issuer_uid);
        $comment="Ticket re-issued from \'$last_issuer\'  to: \'$new_issuer\' ";
        add_task($Ticket_Number,$comment);
        notify_change($Ticket_Number,$comment);
        }
}


function change_due_date($Ticket_Number,$due_date_d_m_y,$due_date_h,$due_date_i){
	$due_date=strtotime("$due_date_d_m_y $due_date_h:$due_date_i:00");
	$last_due_date=Ticket::get_due_date($Ticket_Number);
	if($last_due_date<> $due_date){
	$last_due_date=date("Y/m/d H:i",Ticket::get_due_date($Ticket_Number));
	$result=Ticket::change_due_date($Ticket_Number,$due_date);
	$print_due_date=date("Y/m/d H:i",$due_date);
	$comment="due date changed from \'$last_due_date\' to \'$print_due_date\'";
	add_task($Ticket_Number,$comment);
	notify_change($Ticket_Number,$comment);
	}
}
function change_priority($Ticket_Number,$priority_id){
	$last_priority_id=Ticket::get_ticket_priority_id($Ticket_Number);
	if ($last_priority_id<>$priority_id){
	$last_priority=Ticket::get_priority_name($last_priority_id);
	$result=Ticket::change_priority($Ticket_Number,$priority_id);
	$new_priority=Ticket::get_priority_name($priority_id);
	$comment="priority changed from \'$last_priority\' to \'$new_priority\'";
	add_task($Ticket_Number,$comment);
	notify_change($Ticket_Number,$comment);
	}
}

function change_category($Ticket_Number,$category_id){
        $last_category_id=Ticket::get_ticket_category_id($Ticket_Number);
        if ($last_category_id<>$category_id){
        $last_category=Ticket::get_category_name($last_category_id);
        $result=Ticket::change_category($Ticket_Number,$category_id);
        $new_category=Ticket::get_category_name($category_id);
        $comment="category changed from \'$last_category\' to \'$new_category\'";
        add_task($Ticket_Number,$comment);
        notify_change($Ticket_Number,$comment);
        }
}

function change_stage($Ticket_Number,$stage_id){
        $last_stage_id=Ticket::get_ticket_stage_id($Ticket_Number);
        if ($last_stage_id<>$stage_id){
        $last_stage=Ticket::get_stage_name($last_stage_id);
        $result=Ticket::change_stage($Ticket_Number,$stage_id);
        $new_stage=Ticket::get_stage_name($stage_id);
        $comment="Stage changed from \'$last_stage\' to \'$new_stage\'";
        add_task($Ticket_Number,$comment);
        notify_change($Ticket_Number,$comment);
        }
}


function change_status($Ticket_Number,$status_id){
	$last_status_id=Ticket::get_ticket_status_id($Ticket_Number);
	if ($last_status_id<>$status_id){
	$last_status=Ticket::get_status_name($last_status_id);
	$new_status=Ticket::get_status_name($status_id);
        $result=Ticket::change_status($Ticket_Number,$status_id);
	$sender=whoami();
	$comment="status changed from \'$last_status\' to \'$new_status\'";
	add_task($Ticket_Number,$comment);
	notify_change($Ticket_Number,$comment);
	}
}

function change_activity($Ticket_Number,$activity){
        $last_activity=Ticket::get_ticket_activity_id($Ticket_Number);
	if ($last_activity<>$activity){
        $last_activity=Ticket::get_activity_name($last_activity);
        $new_activity=Ticket::get_activity_name($activity);
        $result=Ticket::change_activity_id($Ticket_Number,$activity);
        $sender=whoami();
	$comment="activity changed from \'$last_activity\' to \'$new_activity\'";
        add_task($Ticket_Number,$comment);
	notify_change($Ticket_Number,$comment);
	}
}

function show_html($Ticket_Number,$result){
global $name;
$textmenu=menu("Show_Tickets",'');
eval($textmenu);
OpenTable();
echo "
<P>&nbsp;</P>


<H1 align=center>"._TICKET_MODIFIED."   $Ticket_Number</H1>";


echo "
	<DIV align=center>
<P align=center>
<TABLE border=1 cellPadding=1 cellSpacing=1 width=\"75%\" bgcolor=Silver bordercolor=Black>

  <TR>
    <TD>
      <H2 align=center>"._TICKET_NR.": $Ticket_Number &nbsp;&nbsp;</H2>
      <P align=center>
      <TABLE border=1 cellPadding=1 cellSpacing=1 width=\"75%\" align=center>
        <FORM action=\"\" method=post id=form0 name=form0>

        <TR>
                <TD align=center>
               $result 
                </TD>
        </TR>
        <TR>


        <TD align=center><INPUT type=\"button\" value=\""._CONTINUE."\" id=button1 name=button1 onclick=\"window.location='showline.php?Ticket_Number=$Ticket_Number'\"></td>
        </TR></form></TABLE></td></TR></TABLE></P>";

echo " <script language=\"Javascript\" type=\"text/javascript\">
        <!--
        function gotoThread(){
        window.location.href=\"showline.php?Ticket_Number=$Ticket_Number\";
        }
        window.setTimeout(\"gotoThread()\", 3000);
        //-->
        </script>
";

CloseTable();
}


function notify(){
#email ticket
$assign_msg_subject=Opentts::loadvar("agent_subject");
eval("\$assign_msg_subject=\"$assign_msg_subject\";");
$assign_msg_body=Opentts::loadvar("agent_email_body");
eval("\$assign_msg_body=\"$assign_msg_body\";");
$reply_msg_subject=Opentts::loadvar("client_email_subject");
eval("\$reply_msg_subject=\"$reply_msg_subject\";");
$reply_msg_body=Opentts::loadvar("client_email_body");
eval("\$reply_msg_body=\"$reply_msg_body\";");
if ($t_sms) {
priv_msg("$t_from","$t_assigned","$assign_msg_subject", "$assign_msg_body");
priv_msg("$t_assigned","$t_from","$reply_msg_subject", "$reply_msg_body");
}
if ($t_email){
$agent_email=User::fetch_email_by_uid($t_assigned);
$issuer_email=User::fetch_email_by_uid($t_from);
mail("$agent_email", "$assign_msg_subject", "$assign_msg_body");
mail("$issuer_email", "$reply_msg_subject", "$reply_msg_body");
}
}

function notify_change($ticket_id,$message){
	global $t_email,$t_sms,$name;
	 $t_from=Task::get_task_issuer($ticket_id);
        $t_assigned=Task::get_task_assigned($ticket_id);
	#

	#
#email ticket
	$assign_msg_subject="$name -> Ticket Number: $ticket_id " ;
	$assign_msg_body=$message;
	$reply_msg_subject=$assign_msg_subject;
	$reply_msg_body=$message;
	if ($t_sms) {
		priv_msg("$t_from","$t_assigned","$assign_msg_subject", "$assign_msg_body");
		priv_msg("$t_assigned","$t_from","$reply_msg_subject", "$reply_msg_body");
	}
	if ($t_email){
		$agent_email=User::fetch_email_by_uid($t_assigned);
		$issuer_email=User::fetch_email_by_uid($t_from);
		#mail("$agent_email", "$assign_msg_subject", "$assign_msg_body");
		#mail("$issuer_email", "$reply_msg_subject", "$reply_msg_body");
		Email::Email("$name","$agent_email","$issuer_email", "$assign_msg_subject", "$assign_msg_body");
		Email::Email("$name","$issuer_email","$agent_email", "$reply_msg_subject", "$reply_msg_body");
	}
}

?>
