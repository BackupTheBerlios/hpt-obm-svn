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
$GO_MODULES->authenticate('squirrelmail');

require_once($GO_MODULES->class_path.'email.class.inc');

require($GO_LANGUAGE->get_language_file('squirrelmail'));
require($GO_THEME->theme_path."header.inc");
echo '<embed src="'.$GO_THEME->sounds['reminder'].'" hidden="true" autostart="true"><noembed><bgsound src="'.$GO_THEME->sounds['reminder'].'"></noembed>';

echo '<a class="normal" href="javascript:show_email()"><img style="margin: 10px;" align="absmiddle" width="32" height="32" src="'.$GO_THEME->images['email_notify'].'" border="0" />';

echo $ml_you_have.' '.$_SESSION['new_mail'].' ';

if ($_SESSION['new_mail'] > 1)
{
  echo $ml_new_mail_multiple;
}else
{
  echo $ml_new_mail_single;
}
echo '</a><br /><br />';
$_SESSION['notified_new_mail'] = $_SESSION['new_mail'];

$button = new button($cmdClose, "javascript:window.close()");
echo '</td></tr></table>';
?>
<script type="text/javascript">

function show_email()
{
  if (opener.parent.main)
  {
    opener.parent.main.location="<?php echo $GO_MODULES->url; ?>";
    opener.parent.main.focus();
  }else
  {
    window.open('<?php echo $GO_CONFIG->full_url.'index.php?return_to='.urlencode($GO_MODULES->url); ?>', '_blank','scrollbars=yes,resizable=yes,status=yes');
  }
  window.close();
}
</script>
<?php
require($GO_THEME->theme_path."footer.inc");
?>
