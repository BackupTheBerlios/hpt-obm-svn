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

Opentts::menu("Admin");
OpenTable();
if(Security::is_action_allowed("admin")){
$func='';
if (isset($_POST['func'])){$func=$_POST['func'];};
if (isset($_GET['func'])){$func=$_GET['func'];};
switch($func) {
	default:
        show_options();
    break;

    case "show_default_agent_table":
        show_default_agent_table();
    break;

    case "set_default_agent":
        set_default_agent($_POST['choosen_one']);
    break;

    case "select_themes":
        select_themes();
    break;

    case "change_theme":
        change_theme($_POST['theme_selected']);
    break;


    case "edit_categories":
        edit_categories();
    break;

    case "edit_status":
        edit_status();
    break;

    case "update_status":
        update_status($_POST['Sel']);
    break;

    case "update_categories":
        update_categories($_POST['Sel']);
    break;

    case "edit_priorities":
        edit_priorities();
    break;

    case "update_priorities":
        update_priorities($_POST['Sel']);
    break;
case "edit_projects":
        edit_projects();
    break;

    case "update_projects":
        update_projects($_POST['Sel']);
    break;

    case "edit_permissions":
        edit_permissions();
    break;

    case "mod_permissions":
        mod_permissions($_POST['Sel'],$_POST['dest_group']);
    break;


    case "edit_permissions_users":
        edit_permissions_users();
    break;

    case "mod_permissions_users":
        mod_permissions_users($_POST['Sel'],$_POST['dest_group']);
    break;


    case "edit_actions":
        edit_actions();
    break;

    case "update_actions":
        update_actions($_POST['Sel']);
    break;

    case "edit_agents":
        edit_agents();
    break;


    case "update_agents":
        update_agents($_POST['Sel'],$_POST['dest_group']);
    break;

    case "mod_globals":
        mod_globals($_POST['mod_varname'],$_POST['mod_definition'],$_POST['mod_action']);
    break;

    case "mod_gid":
        mod_gid($_POST['mod_uid'],$_POST['mod_gid'],$_POST['mod_action']);
    break;

    case "hard_reset":
        reset_categories();
        reset_status();
        reset_priorities();
        reset_permissions();
    break;

        case "reset_demo":
        reset_demo();
        break;

    case "edit_globals":
        edit_globals();
    break;



}
}
CloseTable();

function select_themes(){
global $name;
        if (is_file("configure.php")){
                $file=file("configure.php");
                foreach ($file as $line){
                        if (strstr($line,"\$hlpdsk_theme")){
                                $theme_selected=substr(strstr($line,'"'),1,-3);
                                $show= "theme selected<br>$theme_selected<br>";
                        }
                }
                $themes=Configure::get_themes();
                foreach($themes as $value){
                        if ($value==$theme_selected){
                                $options.="<option value=\"$value\" selected>$value</option>\n";  
                        }else{
                                $options.="<option value=\"$value\">$value</option>\n";
                        }
                }
                $theme_select= "<select name='theme_selected'>$options</select>";
                echo "<form action='admin.php?func=change_theme' method='POST'>";
                echo "<table>"
                        ."<tr>"
                        ."<td>$theme_select</td>"
                        ."</tr>"
                        ."</table>";
                echo "<input name=submit type=submit></form>";
        }

}

function change_theme($theme_selected){
	global $name;
	Configure::change_theme($theme_selected);
	echo " <script language=\"Javascript\" type=\"text/javascript\">
        <!--
        function gotoThread(){
        window.location.href=\"admin.php\";
        }
        window.setTimeout(\"gotoThread()\", 100);
        //-->
        </script>
";

}
function show_default_agent_table(){
	echo "<center>". Security::show_default_agent_table()."</center>";
	echo "<br>";
}

function set_default_agent($uid){
echo "<center>". Security::set_default_agent($uid)."<br>".show_options()."</center>";
        echo "<br>";
}

function show_options(){
	global $name;
	$choices=array("select_themes","edit_projects","show_default_agent_table","edit_priorities","edit_categories","edit_status","edit_actions","edit_permissions","edit_agents","edit_globals","hard_reset","reset_demo");
	$post_url="admin.php";
	echo "<center>". Opentts::form_show_options($choices,$post_url)."</center>";
}

function edit_table($db_table,$db_field,$post_url,$ret_func,$db_order_field=""){
	global $name, $prefix,$hlpdsk_prefix;
	$post_url="admin.php";
	echo "<center>". Opentts::edit_table($db_table,$db_field,$post_url,$ret_func,$db_order_field)."</center>";
	echo "<br>";
}

function update_table($db_table,$db_field,$text_array,$key_field){
	global $prefix,$hlpdsk_prefix;
	show_options();
        echo "<center>". Opentts::update_table($db_table,$db_field,$text_array,$key_field)."</center>";
        echo "<br>";
}

function edit_agents(){
        global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme,$nuke_user_id_fieldname,$nuke_username_fieldname,$action;
        $total_actions=count($action);
        $dbarray_group=SQL::build_hash("{$prefix}{$hlpdsk_prefix}_groups","gid","group_name"," order by gid");
        $total_groups=count($dbarray_group);
        $options_actions=Common::fill_options("users","$nuke_user_id_fieldname","$nuke_username_fieldname",""," order by $nuke_username_fieldname");
        $options_groups=Common::fill_options("{$prefix}{$hlpdsk_prefix}_groups","gid","group_name",""," order by gid");
        $i=0;
        foreach($dbarray_group as $key=>$value){
                $array_gids[$i]=SQL::build_hash("{$prefix}{$hlpdsk_prefix}_groups_members,users","uid","$nuke_username_fieldname"," and $nuke_user_id_fieldname=uid and  gid=$key order by uid");
                $i++;
        }
        $array_groups="\n";
        for ($i=0;$i<$total_groups;$i++){
                $temp_array=$array_gids[$i];
                if (isset($temp_array)){
                        $array_groups.="\tnew Array(\n ";
                        foreach($temp_array as $key=>$value){
                                $array_groups.="\t\tnew Array(\"$value\",\"$key\")\n ,";
                        }
                        $array_groups=substr($array_groups,0,-1);
                        $array_groups.="\n\t) ,\n";
                }else{
                        $array_groups.="\tnull\n ,";
                }
        }
        $array_groups.=substr($array_groups,0,-2);
        $file="themes/$hlpdsk_theme/admin_agents.html";
        $file=addslashes (implode("",file($file)));
        eval("\$content=stripslashes(\" $file\");");
        echo $content;
}
function update_agents ($Sel,$dest_group){
        global $prefix,$hlpdsk_prefix,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_groups_members where gid=$dest_group";
	$tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_groups_members (gid,uid,uid_default) values ('$dest_group','$value','')";
			$tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }

}


function edit_actions(){
global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme,$action;
        $options=Common::array_fill_options($action);
        $form_action_func="update_actions";
        $file="themes/$hlpdsk_theme/admin_select.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;

}

function update_actions($Sel){
echo "Nothing done, (update_actions:Legacy function). Actions hardcoded at file actions.php";
}

function edit_projects(){
global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme;
        $options=Common::fill_options("{$prefix}{$hlpdsk_prefix}_projects","project_id","project_name",""," order by project_id");
        $form_action_func="update_projects";
        $file="themes/$hlpdsk_theme/admin_select.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;

}

function update_projects($Sel){
global $prefix,$hlpdsk_prefix,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_projects";
        $tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
                        $project_id=substr($value,0,strpos($value,"=>"));
                        $project_name=substr($value,strpos($value,"=>") + 2  );
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_projects (project_id,project_name) values ('$project_id','$project_name')";
                        $tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }
}

function edit_status(){
global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme;
        $options=Common::fill_options("{$prefix}{$hlpdsk_prefix}_status","status_id","status_name",""," order by status_id");
        $form_action_func="update_status";
        $file="themes/$hlpdsk_theme/admin_select.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;

}

function update_status($Sel){
global $prefix,$hlpdsk_prefix,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_status";
        $tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
                        $action_id=substr($value,0,strpos($value,"=>"));
                        $action_name=substr($value,strpos($value,"=>") + 2  );
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_status (status_id,status_name) values ('$status_id','$status_name')";
                        $tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }
}


function edit_categories(){
global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme;
        $options=Common::fill_options("{$prefix}{$hlpdsk_prefix}_categories","category_id","category_name",""," order by category_id");
$form_action_func="update_categories";
        $file="themes/$hlpdsk_theme/admin_select.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;

}

function update_categories($Sel){
global $prefix,$hlpdsk_prefix,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_categories";
        $tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
			$category_id=substr($value,0,strpos($value,"=>"));
			$category_name=substr($value,strpos($value,"=>") + 2  );
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_categories (category_id,category_name) values ('$category_id','$category_name')";
                        $tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }
}

function edit_priorities(){
global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme;
        $options=Common::fill_options("{$prefix}{$hlpdsk_prefix}_priorities","priority_id","priority_name",""," order by priority_id");
$form_action_func="update_priorities";
        $file="themes/$hlpdsk_theme/admin_select.html";

$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;

}

function update_priorities($Sel){
global $prefix,$hlpdsk_prefix,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_priorities";
        $tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
                        $category_id=substr($value,0,strpos($value,"=>"));
                        $category_name=substr($value,strpos($value,"=>") + 2  );
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_priorities (priority_id,priority_name) values ('$priority_id','$priority_name')";
                        $tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }
}


function edit_permissions(){
        global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme,$action;
        $total_actions=count($action);
        $dbarray_group=SQL::build_hash("{$prefix}{$hlpdsk_prefix}_groups","gid","group_name"," order by gid");
        $total_groups=count($dbarray_group);
        $options_actions=Common::array_fill_options($action);
        $options_groups=Common::fill_options("{$prefix}{$hlpdsk_prefix}_groups","gid","group_name","",$order=" order by gid");
        $i=0;
        foreach($dbarray_group as $key=>$value){
                $array_gids[$i]=SQL::build_hash("{$prefix}{$hlpdsk_prefix}_permissions","action_id","gid"," and  gid=$key order by action_id");
                $i++;
        }
        $array_groups="\n";
        for ($i=0;$i<$total_groups;$i++){
                $temp_array=$array_gids[$i];
                if (isset($temp_array)){
                        $array_groups.="\tnew Array(\n ";
                        foreach($temp_array as $key=>$value){
                                $array_groups.="\t\tnew Array(\"{$action[$key]}\",\"$key\")\n ,";
                        }
                        $array_groups=substr($array_groups,0,-1);
                        $array_groups.="\n\t) ,\n";
                }else{
                        $array_groups.="\tnull\n ,";
                }
        }
        $array_groups.=substr($array_groups,0,-2);
        $file="themes/$hlpdsk_theme/admin_perm_groups.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;
}

function mod_permissions($Sel,$dest_group){
        global $prefix,$hlpdsk_prefix,$dbi,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_permissions where gid=$dest_group";
	$tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_permissions (gid,action_id,description) values ('$dest_group','$value','')";
			$tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }
}
function edit_permissions_users(){
        global $prefix,$hlpdsk_prefix,$name,$hlpdsk_theme,$action,$nuke_user_id_fieldname,$nuke_username_fieldname;
        $total_actions=count($action);
        $dbarray_group=SQL::build_hash("{$prefix}_users","$nuke_user_id_fieldname","$nuke_username_fieldname"," order by user_id");
        $total_groups=count($dbarray_group);
        $options_actions=Common::array_fill_options($action);
        $options_groups=Common::fill_options("{$prefix}_users","$nuke_user_id_fieldname","$nuke_username_fieldname",""," order by user_id");
        $i=0;
        foreach($dbarray_group as $key=>$value){
                $array_gids[$i]=SQL::build_hash("{$prefix}{$hlpdsk_prefix}_permissions_users","action_id","uid"," and  uid=$key order by action_id");
                $i++;
        }
        $array_groups="\n";
        for ($i=0;$i<$total_groups;$i++){
                $temp_array=$array_gids[$i];
                if (isset($temp_array)){
                        $array_groups.="\tnew Array(\n ";
                        foreach($temp_array as $key=>$value){
                                $array_groups.="\t\tnew Array(\"{$action[$key]}\",\"$key\")\n ,";
                        }
                        $array_groups=substr($array_groups,0,-1);
                        $array_groups.="\n\t) ,\n";
                }else{
                        $array_groups.="\tnull\n ,";
                }
        }
        $array_groups.=substr($array_groups,0,-2);
        $file="themes/$hlpdsk_theme/admin_perm_users.html";
$file=addslashes (implode("",file($file)));
eval("\$content=stripslashes(\" $file\");");
echo $content;

}

function mod_permissions_users($Sel,$dest_group){
        global $prefix,$hlpdsk_prefix,$dbi,$tts;
        $Sel=split(",",$Sel);
        $query_del="delete from {$prefix}{$hlpdsk_prefix}_permissions_users where uid=$dest_group";
	$tts->query($query_del);
        echo "$query_del<br>\n";
        foreach($Sel as $key=>$value){
                if ($value<>""){
                        $query_ins= "insert into {$prefix}{$hlpdsk_prefix}_permissions_users (uid,action_id) values ('$dest_group','$value')";
			$tts->query($query_ins);
                        echo "$query_ins<br>\n";
                }
        }

}


function mod_gid($mod_uid,$mod_gid,$mod_action){
        Opentts::mod_gid($mod_uid,$mod_gid,$mod_action);
	edit_agents();
}

function reset_categories($print_result=TRUE){
	global $prefix,$hlpdsk_prefix,$tts;
	$query_array=array(
	"DELETE FROM {$prefix}{$hlpdsk_prefix}_categories",
	"INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (0, 'TTS new feature request')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (1, 'TTS Bug report')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (2, 'TTS sugestion')","INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (3, 'Contact request')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (4, 'TTS help')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (5, 'TTS documentation')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_categories VALUES (6, 'TTS general')");
	foreach($query_array as $key=>$query){
		$tts->query($query);
		if($print_result) echo "$result: $query<br>";
	}
	return TRUE;
}
    
function reset_status($print_result=TRUE){
        global $prefix,$hlpdsk_prefix,$tts;
        $query_array=array(
        "DELETE FROM {$prefix}{$hlpdsk_prefix}_status",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (1, 'Waiting')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (2, 'Help')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (3, 'Open')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (4, 'In process')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (5, 'Done')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (6, 'Closed')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_status VALUES (7, 'Cancelled')");
        foreach($query_array as $key=>$query){
                $tts->query($query);
                if($print_result) echo "$result: $query<br>";
        }
        return TRUE;
}

function reset_priorities($print_result=TRUE){
        global $prefix,$hlpdsk_prefix,$tts;
        $query_array=array(
        "DELETE FROM {$prefix}{$hlpdsk_prefix}_priorities",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_priorities VALUES (0, 'Do it now', 0, 0)",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_priorities VALUES (1, 'Low', 0, 0)",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_priorities VALUES (2, 'Medium', 0, 0)",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_priorities VALUES (3, 'High', 0, 0)",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_priorities VALUES (4, 'URGENT', 0, 0)");
        foreach($query_array as $key=>$query){
                $tts->query($query);
                if($print_result) echo "$result: $query<br>";
        }
        return TRUE;
}

function reset_demo($print_result=TRUE){
        global $prefix,$hlpdsk_prefix,$tts,$tts_demo;
	if(!$tts_demo){
		echo "works only if \$tts_demo=1 in configure.php\n<br>";
		 return FALSE;
	}
        $query_array=array(
        "UPDATE {$prefix}_users set uname='demo',name='demo',user_theme=NULL, pass='fe01ce2a7fbac8fafaed7c982a04e229', email='demo@riunx.com' where uid=3",
        "UPDATE {$prefix}_users set uname='agent',name='agent',user_theme=NULL, pass='fe01ce2a7fbac8fafaed7c982a04e229', email='agent@riunx.com' where uid=9",
        "UPDATE {$prefix}_users set uname='manager',name='manager',user_theme=NULL, pass='fe01ce2a7fbac8fafaed7c982a04e229', email='manager@riunx.com' where uid=10"
);
        foreach($query_array as $key=>$query){
                $tts->query($query);
                if($print_result) echo "$result: $query<br>";
        }
        return TRUE;
}

function reset_permissions($print_result=TRUE){
        global $prefix,$hlpdsk_prefix,$tts;
        $query_array=array(
        "DELETE FROM {$prefix}{$hlpdsk_prefix}_permissions",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'change_priority', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (5, 'admin', 'can see the admin page')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'edit_ticket', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'edit_entry', 'shows the full form.')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'imperson', 'post a message on behalf someone else.')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (2, 'query_search', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'query_search', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'view_agents_tickets', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (2, 'view_priority', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'view_priority', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'view_clients_tickets', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (2, 'view_clients_tickets', '')",
"INSERT INTO {$prefix}{$hlpdsk_prefix}_permissions VALUES (3, 'statistics', '')");
        foreach($query_array as $key=>$query){
                $tts->query($query);
                if($print_result) echo "$result: $query<br>";
        }
        return TRUE;
}
	
function edit_globals(){
        global $prefix,$hlpdsk_prefix;
        echo Tts_sql::show_query(
        "varname,definition",
        "{$prefix}{$hlpdsk_prefix}_config"
        ," order by varname",
        "Agents");
        echo "<br>";
        echo Opentts::mod_globals();
}
function mod_globals($mod_varname,$mod_definition,$mod_action){
	Opentts::mod_globals($mod_varname,$mod_definition,$mod_action);
	edit_globals();
}
?>

	
