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
if (!Security::is_action_allowed("set_money")){$money='0.00';}

$t_status="1";
$t_stage="1";
$post_date=time();
$change_date=$post_date;
if (!isset($_POST['due_date_d_m_y'])) $due_date=$change_date;
	else {
		$due_date_d_m_y=$_POST['due_date_d_m_y'];
		$due_date_h=$_POST['due_date_h'];
		$due_date_i=$_POST['due_date_i'];
		$due_date=strtotime("$due_date_d_m_y $due_date_h:$due_date_i:00");
	}
if (!isset($_POST['end_date_d_m_y'])) $end_date=$change_date;
        else {
		$end_date_d_m_y=$_POST['end_date_d_m_y'];
                $end_date_h=$_POST['end_date_h'];
                $end_date_i=$_POST['end_date_i'];
		$end_date=strtotime("$end_date_d_m_y $end_date_h:$end_date_i:00");
	}



if (Security::is_action_allowed("imperson") and isset($_POST['t_from']) ) {
	$t_from= Security::get_uid($_POST['t_from']);
	if ($t_from<>whoami()){
		$t_subject="(submitted by ".whatsmyname(whoami()).") : ". $t_subject;
		}
}else{
	$t_from= Security::get_uid();
}

if (!isset($_POST['t_assigned'])) $t_assigned=Security::get_default_agent_id();
if (!isset($_POST['t_priority'])) $t_priority=1;
if(!isset($_POST['project_id'])) $project_id=1;
$my_ticket= new Ticket();
if(isset($_POST['t_status'])) $my_ticket->status_id=$_POST['t_status'];
if(isset($_POST['t_assigned'])) $my_ticket->assigned_id=$_POST['t_assigned'];
#if(isset($_POST['t_from']))     $my_ticket->issuer=$_POST['t_from'];
$my_ticket->issuer=$t_from;
if(isset($_POST['stage_id'])) $my_ticket->stage_id=$_POST['t_stage'];
if(isset($_POST['t_category'])) $my_ticket->category_id=$_POST['t_category'];
if(isset($_POST['t_priority'])) $my_ticket->priority_id=Security::sqlsecure($_POST['t_priority']);
if(isset($_POST['t_subject']) && $_POST['t_subject']) $my_ticket->subject=$_POST['t_subject'];
if(isset($_POST['t_description']) && $_POST['t_description']) $my_ticket->description=$_POST['t_description'];
if(isset($_POST['due_date'])) $my_ticket->due_date="{$_POST['due_date']}";
if(isset($_POST['end_date'])) $my_ticket->end_date="{$_POST['end_date']}";
if(isset($_POST['complete'])) $my_ticket->complete="{$_POST['complete']}";
if(isset($_POST['post_date'])) $my_ticket->post_date="{$_POST['post_date']}";
if(isset($_POST['change_date'])) $my_ticket->change_date="{$_POST['change_date']}";
if(isset($_POST['t_priv_msg'])) $my_ticket->notify_priv_msg=$_POST['t_priv_msg'];
if(isset($_POST['t_email'])) $my_ticket->notify_email=$_POST['t_email'];
if(isset($_POST['project_id'])) $my_ticket->project_id=$_POST['project_id'];

$my_ticket->sql_insert();
$Ticket_Number= $my_ticket->ticket_nr;
#email ticket
/*
$assign_msg_subject=Opentts::loadvar("assigned_subject");
eval("\$assign_msg_subject=\"$assign_msg_subject\";");
$assign_msg_body=Opentts::loadvar("assigned_msg_body");
eval("\$assign_msg_body=\"$assign_msg_body\";");
$issuer_msg_subject=Opentts::loadvar("issuer_subject");
eval("\$issuer_msg_subject=\"$issuer_msg_subject\";");
$issuer_msg_body=Opentts::loadvar("issuer_msg_body");
eval("\$issuer_msg_body=\"$issuer_msg_body\";");
$t_cc = trim($t_cc);
$separator=",";
$t_ccarray = explode($separator,$t_cc);

if ($t_sms) {
$t_from_uname=$t_from;
$t_assigned_uname=$t_assigned;
priv_msg("$t_from","$t_assigned","$assign_msg_subject", "$assign_msg_body");
priv_msg("$t_assigned","$t_from","$reply_msg_subject", "$reply_msg_body");
}
if ($t_email){
$agent_email=User::fetch_email_by_uid($t_assigned);
$issuer_email=User::fetch_email_by_uid($t_from);
$email= new Email(Security::get_uname($t_from),$issuer_email,$agent_email, $assign_msg_subject, $assign_msg_body);
$email= new Email(Security::get_uname($t_assigned),$agent_email,$issuer_email, $issuer_msg_subject, $issuer_msg_body);
}
*/
#
$textmenu=menu("Show_Tickets",'');
eval($textmenu);
OpenTable();
echo "
<P>&nbsp;</P>


<H1 align=center>"._TICKET_ADDED."   $Ticket_Number</H1>";


echo "
	<DIV align=center>
<P align=center>
<TABLE border=1 cellPadding=1 cellSpacing=1 width=\"75%\" bgcolor=Silver bordercolor=Black>

  <TR>
    <TD>
      <H2 align=center>"._TICKET_NR.": $Ticket_Number &nbsp;&nbsp;  $t_status</H2>
      <P align=center>
      <TABLE border=1 cellPadding=1 cellSpacing=1 width=\"75%\" align=center>
        <FORM action=\"\" method=post id=form0 name=form0>

        <TR>
                <TD align=center>
                "._ADDED_SUCCESSFULY."
                </TD>
        </TR>
        <TR>


        <TD align=center><INPUT type=\"button\" value=\""._CONTINUE."\" id=button1 name=button1 onclick=\"window.location='showline.php?Ticket_Number=$Ticket_Number'\"></td>
        </TR></form></TABLE></td>
        </TR></TABLE></P>
<script language=\"Javascript\" type=\"text/javascript\">
        <!--
        function gotoThread(){
        window.location.href=\"showline.php?Ticket_Number=$Ticket_Number\";
        }
        window.setTimeout(\"gotoThread()\", 3000);
        //-->
        </script>

";
CloseTable();
?>
