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

$calendar_id = isset($_REQUEST['calendar_id']) ? $_REQUEST['calendar_id'] : 0;

if (isset($_POST['delete_view_id']))
{
  $cal->delete_view($_POST['delete_view_id']);
}

$tabtable = new tabtable('group_views_tab', $cal_views, '100%', '400', '120', '', true);
require($GO_THEME->theme_path.'header.inc');

echo '<form name="events" method="post" action="'.$_SERVER['PHP_SELF'].'">';
echo '<input type="hidden" name="calendar_id" value="'.$calendar_id.'" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="delete_view_id" value="" />';

$tabtable->print_head();

echo '<p><a href="view.php" class="normal">'.$cmdAdd.'</a></p>';

$cal->get_views($GO_SECURITY->user_id);
echo '<table border="0">';
while($cal->next_record())
{
  echo '<tr><td><a href="index.php?view_id='.$cal->f('id').'" class="normal">'.$cal->f('name').'</a></td>';
  //echo '<td>'.$cal->f('type').'</td>';
  echo '<td>&nbsp;<a href="view.php?view_id='.$cal->f("id").'&return_to='.rawurlencode($_SERVER['REQUEST_URI']).'" title="'.$strEdit.' \''.htmlspecialchars(addslashes($cal->f("name"))).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
  echo "<td>&nbsp;<a href='javascript:delete_view(\"".$cal->f("id")."\",\"".rawurlencode($strDeletePrefix."'".addslashes($cal->f("name"))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cal->f("name"))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
}
echo '</table><br />';

$button = new button($cmdClose, "javascript:document.location='$return_to';");

$tabtable->print_foot();

echo '</form>';
?>
  <script type="text/javascript">
function delete_view(view_id, message)
{
  if (confirm(unescape(message)))
  {
    document.forms[0].delete_view_id.value = view_id;
    document.forms[0].submit();
  }
}
</script>
<?php
require($GO_THEME->theme_path.'footer.inc');
?>
