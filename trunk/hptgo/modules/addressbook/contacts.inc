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

if (!isset($_SESSION['contacts_expanded']))
$_SESSION['contacts_expanded'][]=-1;

if($task == 'sort')
{
  $ab->set_contacts_sorting($GO_SECURITY->user_id, 
      $_REQUEST['new_sort_field'], $_REQUEST['new_sort_order']);
  $ab_settings['sort_contacts_order'] = $_REQUEST['new_sort_order'];
  $ab_settings['sort_contacts_field'] = $_REQUEST['new_sort_field'];  
}

if ($ab_settings['sort_contacts_order'] == "DESC")
{
  $image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $new_sort_order = "ASC";
}else
{
  $image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $new_sort_order = "DESC";
}

switch($task)
{
  case "delete":
    for ($i=0;$i<sizeof($_POST['contact']);$i++)
    {
      $contact = $ab->get_contact($_POST['contact'][$i]);

      if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $contact['acl_write']))
      {
	$ab->delete_contact($_POST['contact'][$i]);
	$GO_SECURITY->delete_acl($contact['acl_write']);
	$GO_SECURITY->delete_acl($contact['acl_read']);
      }
    }
  break;

  case "move":
    if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $addressbook['acl_write']))
    {
      if (!isset($_POST['group_id']))
      {
	$_POST['group_id']="0";
      }

      if (isset($_POST['contact']))
      {
	for ($i=0;$i<sizeof($_POST['contact']);$i++)
	{
	  $ab->move_contact_to_group($_POST['contact'][$i], $_POST['group_id']);
	}
      }
    }

  break;

  case "expand":

    if (isset($_POST['expand_id']))
    {
      $key = array_search($_POST['expand_id'], $_SESSION['contacts_expanded']);
      if (!$key)
      {
	$_SESSION['contacts_expanded'][]=$_POST['expand_id'];
      }else
      {
	unset($_SESSION['contacts_expanded'][$key]);
      }
    }
  break;
}


echo '<input type="hidden" name="post_action" value="browse" />';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" value="'.$new_sort_order.'" name="new_sort_order" />';
echo '<input type="hidden" name="new_sort_field" />';
echo '<input type="hidden" name="expand_id" />';

if (isset($feedback)) echo $feedback;

echo "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">\n";
echo '<tr><td colspan="99">';
echo '<table border="0"><tr>';
if ($ab->get_subscribed_addressbooks($GO_SECURITY->user_id) > 1)
{
  echo '<td>'.$ab_addressbook.':</td><td>';
  $subscribed_addressbooks = new dropbox();
  while ($ab->next_record())
  {
    $subscribed_addressbooks->add_value($ab->f('id'), $ab->f('name'));
  }
  $subscribed_addressbooks->print_dropbox('addressbook_id', $addressbook_id, 'onchange="javascript:document.forms[0].submit()"');
  echo '</td>';
}else
{
  echo '<input type="hidden" name="addressbook_id" value="'.$addressbook_id.'" />';
}

echo '<td align="right" width="100%">';
$group_count = $ab->get_groups($addressbook_id);
if ($group_count > 0)
{
  $groups = array();

  $dropbox = new dropbox();
  $dropbox->add_value('',$contacts_move_to_group);
  $dropbox->add_value('0',$contacts_other);

  while ($ab->next_record())
  {
    $dropbox->add_value($ab->f('id'), $ab->f('name'));
    $group['name'] = $ab->f('name');
    $group['id'] = $ab->f('id');
    $groups[] = $group;
  }
  if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $addressbook['acl_write']))
  {
    $dropbox->print_dropbox('group_id','','onchange="javascript:move_to_group()"');
  }
}
echo '</td></tr></table>';
echo '</td></tr>';

echo '<tr><td class="TableHead2">&nbsp;</td>';
echo '<td class="TableHead2" width="16"><input type="checkbox" name="dummy" value="dummy" onclick="javascript:invert_selection()" /></td>';
echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"javascript:sort('name')\">".$strName;
if ($ab_settings['sort_contacts_field'] == "name")
echo $image_string;
echo "</a></td>\n";
echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"javascript:sort('email')\">".$strEmail;
if ($ab_settings['sort_contacts_field'] == "email")
echo $image_string;
echo "</a></td>\n";
echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"javascript:sort('home_phone')\">".$strPhone;
if ($ab_settings['sort_contacts_field'] == "home_phone")
echo $image_string;
echo "</a></td>\n";
echo "<td class=\"TableHead2\" nowrap><a class=\"TableHead2\" href=\"javascript:sort('work_phone')\">".$strWorkphone;
if ($ab_settings['sort_contacts_field'] == "work_phone")
echo $image_string;
echo "</a></td>\n";
echo "</tr>\n";

if(isset($groups))
{
  for($i=0;$i<sizeof($groups);$i++)
  {
    if (in_array($groups[$i]['id'], $_SESSION['contacts_expanded']))
    {
      echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(".$groups[$i]['id'].")\"><img src=\"".$GO_THEME->images['min_node']."\" border=\"0\" /></a></td><td><input type=\"checkbox\" name=\"dummy\" value=\"dummy\" onclick=\"javascript:select_group('".$groups[$i]['id']."')\" /></td><td colspan=\"4\">".$groups[$i]['name']."</td></tr>";
      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

      if ($ab->get_contacts_group($addressbook_id, $groups[$i]['id'], $ab_settings['sort_contacts_field'], $ab_settings['sort_contacts_order']) > 0)
      {
	echo '<input type="hidden" name="group_start_'.$groups[$i]['id'].'" />';
	while ($ab->next_record())
	{
	  $checked = "";
	  $class = 'Table1';
	  if (isset($contact))
	  {
	    $key = array_search($ab->f("id"), $contact);
	    if (is_int($key))
	    {
	      unset($contact[$key]);
	      $checked = "checked";
	      $class = 'Table2';
	    }
	  }

	  if ($ab->f('color') != '')
	  {
	    $style = ' style="color: '.$ab->f('color').';"';
	  }else
	  {
	    $style = '';
	  }

	  $middle_name = $ab->f('middle_name') == '' ? '' : $ab->f('middle_name').' ';
	  $name = $ab->f('last_name').' '.$middle_name.$ab->f('first_name');

	  echo "<tr id=\"".$ab->f("id")."\" class=\"".$class."\"><td></td>\n";
	  echo "<td><input id=\"".$name."\" type=\"checkbox\" onclick=\"javacript:item_click(this)\" name=\"contact[]\" value=\"".$ab->f("id")."\" ".$checked." /></td><td><a".$style." class=\"normal\" href=\"contact.php?contact_id=".$ab->f("id")."&return_to=".rawurlencode($_SERVER['REQUEST_URI'])."\" title=\"".$strShowProfile."\">".empty_to_stripe($name)."</a>&nbsp;</td>\n";
	  echo "<td>".mail_to(empty_to_stripe($ab->f("email")), empty_to_stripe($ab->f("email")),'normal',true, $ab->f("id"))."&nbsp;</td>\n";
	  echo "<td>".empty_to_stripe($ab->f("home_phone"))."&nbsp;</td>\n";
	  echo "<td>".empty_to_stripe($ab->f("work_phone"))."&nbsp;</td>\n";
	  echo "</tr>\n";
	  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
	}
	echo '<input type="hidden" name="group_end_'.$groups[$i]['id'].'" />';
      }else
      {
	echo "<tr><td colspan=\"99\" height=\"18\">".$contacts_empty_group."</td></tr>";
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
      }
    }else
    {
      echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(".$groups[$i]['id'].")\"><img src=\"".$GO_THEME->images['plus_node']."\" border=\"0\" /></a></td><td>&nbsp;</td><td colspan=\"4\" width=\"100%\">".$groups[$i]['name']."</td></tr>";
      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
    }
  }
}

if ($group_count > 0)
{
  if (in_array(0, $_SESSION['contacts_expanded']))
  {
    echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(0)\"><img src=\"".$GO_THEME->images['min_node']."\" border=\"0\" /></a><td><input type=\"checkbox\" name=\"dummy\" value=\"dummy\" onclick=\"javascript:select_group('0')\" /></td><td colspan=\"4\">".$contacts_other."</td></tr>";
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

    $ab->get_contacts_group($addressbook_id,0, $ab_settings['sort_contacts_field'], $ab_settings['sort_contacts_order']);
    if ($ab->num_rows() > 0)
    {
      echo '<input type="hidden" name="group_start_0" />';
      while ($ab->next_record())
      {
	$checked = "";
	$class = 'Table1';
	if (isset($contact))
	{
	  $key = array_search($ab->f("id"), $contact);
	  if (is_int($key))
	  {
	    unset($contact[$key]);
	    $checked = "checked";
	    $class = 'Table2';
	  }
	}
	if ($ab->f('color') != '')
	{
	  $style = ' style="color: '.$ab->f('color').';"';
	}else
	{
	  $style = '';
	}

	$middle_name = $ab->f('middle_name') == '' ? '' : $ab->f('middle_name').' ';
	$name = $ab->f('last_name').' '.$middle_name.$ab->f('first_name');
	echo "<tr id=\"".$ab->f("id")."\" class=\"".$class."\"><td></td>\n";
	echo "<td><input id=\"".addslashes($name)."\" onclick=\"javacript:item_click(this)\" type=\"checkbox\" name=\"contact[]\" value=\"".$ab->f("id")."\" ".$checked." /></td><td><a".$style." class=\"normal\" href=\"contact.php?contact_id=".$ab->f("id")."&return_to=".rawurlencode($_SERVER['REQUEST_URI'])."\" title=\"".$strShowProfile."\">".empty_to_stripe($name)."</a>&nbsp;</td>\n";
	echo "<td>".mail_to(empty_to_stripe($ab->f("email")), empty_to_stripe($ab->f("email")),'normal',true, $ab->f("id"))."&nbsp;</td>\n";
	echo "<td>".empty_to_stripe($ab->f("home_phone"))."&nbsp;</td>\n";
	echo "<td>".empty_to_stripe($ab->f("work_phone"))."&nbsp;</td>\n";
	echo "</tr>\n";
	echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
      }
      echo '<input type="hidden" name="group_end_0" />';
    }else
    {
      if ($group_count > 0)
	$text = $contacts_empty_group;
      else
	$text = $contacts_no_contacts;

      echo "<tr><td colspan=\"99\" height=\"18\" class=\"normal\">".$text."</td></tr>";
      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
    }
  }else
  {
    echo "<tr class=\"Table4\"><td><a href=\"javascript:expand_group(0)\"><img src=\"".$GO_THEME->images['plus_node']."\" border=\"0\" /></a><td>&nbsp;</td><td colspan=\"4\" width=\"100%\">".$contacts_other."</td></tr>";
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
  }
}else
{
  if ($ab->get_contacts_group($addressbook_id, 0, $ab_settings['sort_contacts_field'], $ab_settings['sort_contacts_order']) > 0)
  {
    echo '<input type="hidden" name="group_start_0" />';
    while ($ab->next_record())
    {
      $class = 'Table1';
      $checked = "";
      if (isset($contact))
      {
	$key = array_search($ab->f("id"), $contact);
	if (is_int($key))
	{
	  unset($contact[$key]);
	  $checked = "checked";
	  $class = 'Table2';
	}
      }
      if ($ab->f('color') != '')
      {
	$style = ' style="color: '.$ab->f('color').';"';
      }else
      {
	$style = '';
      }
      $middle_name = $ab->f('middle_name') == '' ? '' : $ab->f('middle_name').' ';
      $name = $ab->f('last_name').' '.$middle_name.$ab->f('first_name');

      echo "<tr id=\"".$ab->f("id")."\" class=\"".$class."\"><td></td>\n".
	"<td width=\"16\"><input id=\"".addslashes($name)."\" ".
	"onclick=\"javacript:item_click(this)\"  type=\"checkbox\"".
	"name=\"contact[]\" value=\"".$ab->f("id")."\" ".$checked.
	" /></td><td><a".$style." class=\"normal\" ".
	"href=\"contact.php?contact_id=".$ab->f("id")."&return_to=".
	rawurlencode($_SERVER['REQUEST_URI'])."\" title=\"".
	$strShowProfile."\">".empty_to_stripe($name)."</a>&nbsp;</td>\n".
	"<td>".mail_to(empty_to_stripe($ab->f("email")),
	    empty_to_stripe($ab->f("email")),
	    'normal',
	    true,
	    $ab->f("id"))."&nbsp;</td>\n".

      "<td>".empty_to_stripe($ab->f("home_phone"))."&nbsp;</td>\n".
      "<td>".empty_to_stripe($ab->f("work_phone"))."&nbsp;</td>\n".
      "</tr>\n";

      echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
    }
    echo '<input type="hidden" name="group_end_0" />';
  }else
  {
    if ($group_count > 0)
      $text = $contacts_empty_group;
    else
      $text = $contacts_no_contacts;

    echo "<tr><td colspan=\"99\" height=\"18\" class=\"normal\" width=\"100%\">".$text."</td></tr>";
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
  }
}

/*
   uncommenting this remebers selected contacts in closed groups
   people got confused
   if (isset($contact))
   {
   while($adress = array_pop($contact))
   {
   echo '<input type="hidden" name="contact[]" value="'.$adress.'" />';
   }
   }*/
echo '</table>';
?>
<script type="text/javascript" language="javascript">
  <!--
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

function select_group(group_id)
{
  var add = false;

  for (var i = 0; i < document.forms[0].elements.length; i++)
  {
    if (document.forms[0].elements[i].name == 'group_start_'+group_id)
    {
      add = true;
    }

    if (document.forms[0].elements[i].name == 'group_end_'+group_id)
    {
      add = false;
    }

    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy' && add==true)
    {
      document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
      item_click(document.forms[0].elements[i]);
    }
  }
}

function confirm_delete()
{
  var count = 0;
  var name = new String;
  for (var i=0;i<document.forms[0].elements.length;i++)
  {
    if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
    {
      if (document.forms[0].elements[i].checked == true)
      {
	count++;
	name = document.forms[0].elements[i].id;
      }
    }
  }
  switch (count)
  {
    case 0:
      alert("<?php echo $contacts_no_select; ?>");
      break;

    case 1:
      if (confirm("<?php echo $strDeletePrefix; ?> '"+name+"' <?php echo $strDeleteSuffix; ?>"))
      {
	document.forms[0].task.value="delete";
	document.forms[0].submit();
      }
      break;

    default:
      if (confirm("<?php echo $strDeletePrefix.$strThis; ?> "+count+" <?php echo $contacts_contacts.$strDeleteSuffix; ?>"))
      {
	document.forms[0].task.value="delete";
	document.forms[0].submit();
      }
      break;
  }
}

function move_to_group()
{
  document.forms[0].task.value="move";
  document.forms[0].submit();
}
function sort(column)
{
  document.forms[0].task.value = 'sort';
  document.forms[0].new_sort_field.value = column;
  document.forms[0].submit();
}

function expand_group(group_id)
{
  document.forms[0].expand_id.value = group_id;
  document.forms[0].task.value = "expand";
  document.forms[0].submit();
}

//-->
</script>
