<?php

function draw_button($url,$title,$image,$selected=FALSE){
        global $name,$GO_LANGUAGE,$GO_THEME;
        require($GO_LANGUAGE->get_language_file('opentts'));
        return "<td class=\"ModuleIcons\"><a href=\"$url\" class=\"small\"><img src=\"".$GO_THEME->theme_url."/images/buttons/$image\" border=\"0\" height=\"32\" width=\"32\" ><br>".$$title."</font></a></td>";
	
}

function draw_extra_button(){
        global $name,$GO_LANGUAGE,$GO_THEME;
	require($GO_LANGUAGE->get_language_file('opentts'));
	return "<td class=\"ModuleIcons\"><form ACTION=showline.php Method=GET><input name=Ticket_Number type=text size=3 valign=center><img src=\"images/blank.png\" border=\"0\" height=\"32\" width=\"1\" valign=center><br>$helpdesk_go_to_ticket</form></td>";
}

function show_hidden(){
	global $_SESSION,$GO_LANGUAGE;
	require($GO_LANGUAGE->get_language_file('opentts'));
	$returntd = "<td  class=\"ModuleIcons\"><form name=form_hidden method=POST action='my_tickets.php'>";
	$returntd .= "<input type=hidden name=\"hidden_box\" value='on' valign=center>";
	if (isset($_SESSION['search_hidden'])){
        	$hidden_check='checked';
	}else{
        	$hidden_check='';
	}
	$returntd .= "<input type=checkbox name=\"search_hidden\" $hidden_check onClick='document.form_hidden.submit()'><img src=\"images/blank.png\" border=\"0\" height=\"32\" width=\"1\" valign=center><br>$helpdesk_show_hidden</form></td>";
	return $returntd;
}
?>
