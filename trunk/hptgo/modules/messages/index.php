<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

require("../../Group-Office.php");
$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('messages');
#require($GO_LANGUAGE->get_language_file('messages'));

$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$mode = isset($_REQUEST['mode']) ? $_REQUEST['mode'] : 'messages';
$receipient_id = isset($_REQUEST['receipient']) ? $_REQUEST['receipient'] : '';
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$db = new db();
switch ($task) {
 case 'send':
   if ($_REQUEST['message'] == '')
     $feedback .= 'Message is empty';
   else
     if ($receipient_id > 0) {
       if ($db->query("SELECT * FROM messages_users WHERE user_id='$receipient_id'") &&
	   $db->next_record())
	 $has_permission = $GO_SECURITY->has_permission($GO_SECURITY->user_id,$db->f('acl_write'));
       else
	 $has_permission = true;
       if ($has_permission) {
	 $msg_id = $db->nextid("messages_messages");
	 $db->query("INSERT INTO messages_messages (id,user_id,sender_id,ctime, text) VALUES ('$msg_id','$receipient_id','{$GO_SECURITY->user_id}','".get_gmt_time()."','".smart_addslashes($_REQUEST['message'])."')");
	 $db->query("INSERT INTO messages_new (id) VALUES ('$msg_id')");
       } else
	 $feedback .= "You are not allowed to send message to the receipient";
     } else
       $feedback .= 'You did not specify receipient';
   break;

 case 'delete':
   if (is_array($_REQUEST['msgs'])) {
     $db->query("DELETE FROM messages_messages WHERE id in (".implode(',',$_REQUEST['msgs']).")");
     $db->query("DELETE FROM messages_new WHERE id in (".implode(',',$_REQUEST['msgs']).")");
   }
   break;
}


$page_title = $lang_modules['messages'];
require($GO_THEME->theme_path."header.inc");

?>
<table border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td class="ModuleIcons">
      <a href="javascript:submit_mode('messages');"><img src="<?php echo $GO_THEME->images['ab_browse']; ?>" border="0" height="32" width="32" /><br /><?php echo 'Messages'; ?></a>
    </td>

    <td class="ModuleIcons">
      <a href="javascript:submit_mode('send');"><img src="<?php echo $GO_THEME->images['ab_browse']; ?>" border="0" height="32" width="32" /><br /><?php echo 'New message'; ?></a>
    </td>

    <td class="ModuleIcons">
      <a href="javascript:submit_mode('properties');"><img src="<?php echo $GO_THEME->images['ab_browse']; ?>" border="0" height="32" width="32" /><br /><?php echo 'Properties'; ?></a>
    </td>
  </tr>
</table>
<br/>
<?php

echo '<form name="template" method="post" action="'.$_SERVER['PHP_SELF'].'" />';
echo '<input type="hidden" name="mode" value="'.$mode.'" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="close" value="false" />';
//echo '<input type="hidden" name="receipient" value="'.$receipient_id.'" />';

switch ($mode) {
 case 'send':
   if (isset($feedback))
     echo "<b>$feedback</b></br>";
   
   $select = new select('user', 'template', 'receipient', $receipient_id);
   $select->print_link('Send to');echo ':&nbsp;';
   $select->print_field();
   echo '<br/>';
   echo '<br/>';
   echo 'Message:';
   echo '<br/>';
   echo '<textarea  cols="70" rows="4" class="textbox" name="message"></textarea>';
   echo '<br/>';
   echo '<br/>';
   $button = new button();
   echo $button->get_button('Send',"document.forms[0].task.value='send';document.forms[0].submit();");
   break;

 case 'messages':
   echo '<br/>';
   echo '<br/>';
   echo '<table width="100%" cellpadding="0" cellspacing="0">';
   echo '<tr><td class="TableHead2"><input name="dummy" value="dummy" onclick="javascript:invert_selection()" type="checkbox"></td><td class="TableHead2" width="30%">Sender</td><td class="TableHead2" width="100%">Message</td></tr>';
   $db->query("SELECT * FROM messages_messages WHERE user_id='{$GO_SECURITY->user_id}'ORDER BY ctime DESC");
   while ($db->next_record()) {
     $sender = show_profile($db->f('user_id'), '', 'normal', $link_back);
     $message = htmlspecialchars($db->f('text'));
     echo "<tr id=\"".$db->f('id')."\"><td><input id=\"C_".$db->f('id')."\" type=\"checkbox\" name=\"msgs[]\" value=\"".$db->f('id')."\" onclick=\"javascript:item_click(this)\" /></td><td>$sender</td><td>$message</td></tr>";
     echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
   }
   echo '</table>';

   $button = new button();
   echo '<br/>';
   echo $button->get_button('Delete',"document.forms[0].task.value='delete';document.forms[0].submit();");
   break;

 case 'properties':
   if (!$db->query("SELECT * from messages_users WHERE user_id={$GO_SECURITY->user_id}") ||
       !$db->next_record()) {
     $acl_write = $GO_SECURITY->get_new_acl('message write: '.$GO_SECURITY->user_id);
     if ($acl_write > 0)
       $db->query("INSERT INTO messages_users (user_id,acl_write) VALUES ('{$GO_SECURITY->user_id}','$acl_write')");
     $db->query("SELECT * from messages_users WHERE user_id={$GO_SECURITY->user_id}");
     $db->next_record();
   }
   $tabtable = new tabtable('messages_tab', 'Messages', '100%', '400','120','',true);
   $tabtable->add_tab('properties', 'Properties');
   $tabtable->add_tab('permissions', 'Permissions');
   $tabtable->print_head();

   switch ($tabtable->get_active_tab_id()) {
   case 'properties':
     break;

   case 'permissions':
    print_acl($db->f('acl_write'));
    echo '<br /><br />&nbsp;&nbsp;&nbsp;&nbsp;';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."';");
    break;
   }

   $tabtable->print_foot();
   break;
}
echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
<script type="text/javascript" language="javascript">
  <!--
mainform = document.forms[0];
function item_click(check_box)
{
  var item = get_object(check_box.value);
  if (check_box.checked)
  {
    item.style.backgroundColor = '#FFFFCC';
  }else
  {
    item.style.backgroundColor = '#FFFFFF';
  }
}

function invert_selection()
{
  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
      document.forms[0].elements[i].onclick();
    }
  }
}

function submit_mode(mode)
{
  mainform.mode.value = mode;
  mainform.submit();
}

//-->
</script>