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

//load Group-Office
require("../../Group-Office.php");


//authenticate the user
$GO_SECURITY->authenticate();

//see if the user has access to this module
//for this to work there must be a module named 'example'
$GO_MODULES->authenticate('cms');

//load the CMS module class library
require($GO_MODULES->class_path.'cms.class.inc');
$cms = new cms();

//get the language file
require($GO_LANGUAGE->get_language_file('cms'));

require($GO_THEME->theme_path.'header.inc');

echo '<input type="hidden" name="template_file_id" />';
echo '<input type="hidden" name="task" />';
echo '<table border="0" cellpadding="3" cellspacing="0"><tr>';
echo '<td width="16">&nbsp;</td>';
echo '<td width="100" nowrap><h3>'.$strName.'</h3></td>';
echo '<td width="100" nowrap><h3>'.$strType.'</h3></td></tr>';

//list the files
$total_size = 0;
$count_files = $cms->get_template_files($_REQUEST['template_id']);
while ($cms->next_record())
{
  $total_size += $cms->f('size');
  $short_name = cut_string($cms->f('name'), 30);

  echo '<tr class="Table1">';
  echo '<td><img width="16" height="16" border="0" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$cms->f('extension').'" /></td>';
  echo '<td nowrap>&nbsp;<a href="javascript:paste_url(\''.$GO_MODULES->url.'template_file.php?template_file_id='.$cms->f('id').'\')" title="'.$cms->f('name').'">'.$short_name.'</a>&nbsp;&nbsp;</td>';
  echo '<td nowrap>'.$cms->f('content_type_friendly').'&nbsp;&nbsp;</td></tr>';
}
echo '<tr><td colspan="99" height="18">&nbsp;'.$count_files.' '.$cms_items.'</td></tr>';

echo '</table>
<script type="text/javascript">

function paste_url(url)
{
  var textarea = opener.document.forms[0].'.$_REQUEST['SET_FIELD'].';

  if (document.all)
  {
    textarea.value=url+"\r\n"+textarea.value
  }else
  {
    textarea.value=textarea.value.substring(0,textarea.selectionStart)+url+textarea.value.substring(textarea.selectionEnd,textarea.value.length);
  }
  window.close();
}

</script>';

require($GO_THEME->theme_path.'footer.inc');
?>
