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
$GO_MODULES->authenticate('calendar');
require($GO_LANGUAGE->get_language_file('calendar'));

require($GO_MODULES->path.'classes/calendar.class.inc');
$cal = new calendar();

$post_action = isset($_REQUEST['post_action']) ? $_REQUEST['post_action'] : '';
$task = isset($_POST['task']) ? $_POST['task'] : '';
$return_to = isset($_REQUEST['return_to']) ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = isset($_REQUEST['link_back']) ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : 0;

switch ($task)
{
  case 'delete_calendar':
    $calendar = $cal->get_calendar($_POST['delete_calendar_id']);

    if($GO_SECURITY->has_permission($GO_SECURITY->user_id, $calendar['acl_write']))
    {
      if ($cal->delete_calendar($_POST['delete_calendar_id']))
      {
	$holidays->delete_holidays($GO_SECURITY->user_id, $_POST['delete_calendar_id']);
	$GO_SECURITY->delete_acl($calendar['acl_write']);
	$GO_SECURITY->delete_acl($calendar['acl_read']);
      }
    }
    break;

  case 'save_calendar':
    $cal->set_default_calendar($GO_SECURITY->user_id, $_POST['default_calendar_id']);
    if ($_POST['close_action'] == 'true')
    {
      header('Location: '.$return_to);
      exit();
    }
    break;
}

$tabtable = new tabtable('calendar', $sc_calendars, '100%', '400', '120', '', true);
require($GO_THEME->theme_path.'header.inc');

echo '<form name="events" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="calendar_id" value="'.$calendar_id.'" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="delete_calendar_id" value="" />';
echo '<input type="hidden" name="task" value="'.$task.'" />';
echo '<input type="hidden" name="close_action" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

$tabtable->print_head();
?>
<table border="0" cellpadding="10">
<tr>
<td>
<table border="0" cellpadding="3" cellspacing="0">
<tr><td colspan="5" height="25"><a href="calendar.php" class="normal"><?php echo $cmdAdd; ?></a></td></tr>
  <?php
if (isset($feedback))
{
  echo '<tr><td colspan="5" height="25">'.$feedback.'</td></tr>';
}

echo '<tr><td><h3>'.$strName.'</td><td><h3>'.$sc_owner.'</h3></td><td>&nbsp;</td><td>&nbsp;</td></tr>';

$settings = $cal->get_settings($GO_SECURITY->user_id);

$calendar_count = $cal->get_authorised_calendars($GO_SECURITY->user_id);
if ($calendar_count > 0)
{
  while ($cal->next_record())
  {
    echo '<tr>';
    echo '<td nowrap><a href="index.php?calendar_id='.$cal->f("id").'" class="normal">'.$cal->f("name").'</a>&nbsp;</td>';
    echo '<td nowrap>'.show_profile($cal->f("user_id")).'&nbsp;</td>';
    echo '<td><a href="calendar.php?calendar_id='.$cal->f("id").'&return_to='.rawurlencode($link_back).'" title="'.$strEdit.' \''.htmlspecialchars(addslashes($cal->f("name"))).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
    echo "<td><a href='javascript:delete_calendar(\"".$cal->f("id")."\",\"".rawurlencode($strDeletePrefix."'".addslashes($cal->f("name"))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cal->f("name"))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
  }
}
echo '</table>';
echo '<br /><br />';
$button = new button($cmdClose,"javascript:document.location='".$return_to."'");
?>
</td>
</tr>
</table>

  <script type="text/javascript">
function delete_calendar(calendar_id, message)
{
  if (confirm(unescape(message)))
  {
    document.forms[0].delete_calendar_id.value = calendar_id;
    document.forms[0].task.value='delete_calendar';
    document.forms[0].submit();
  }
}
function save_calendar(close_me)
{
  document.forms[0].close_action.value=close_me;
  document.forms[0].task.value = 'save_calendar';
  document.forms[0].submit();
}
</script>
<?php
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path.'footer.inc');
?>
