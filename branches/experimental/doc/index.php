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

require("../Group-Office.php");
$GO_SECURITY->authenticate();

require($GO_LANGUAGE->get_fallback_base_language_file('preferences'));

if (!file_exists($GO_CONFIG->root_path.'doc'.$GO_CONFIG->slash.$GO_LANGUAGE->language['language_file']))
{
  require($GO_CONFIG->class_path."filesystem.class.inc");
  $fs = new filesystem();

  require($GO_CONFIG->root_path.'/language/languages.inc');

  $page_title = $menu_manual;
  require($GO_THEME->theme_path."header.inc");
  echo '<table border="0" width="100%" cellpadding="10"><tr><td align="center"><table border="0">';
  echo '<tr><td><h2>'.$pref_language.':</h2><br /></td></tr>';
  $folders = $fs->get_folders_sorted($GO_CONFIG->root_path.'doc', 'basename', 'asc');
  while($folder = array_shift($folders))
  {
    if ($folder['name'] != 'CVS')
    {
    $language = isset($languages[$folder['name']]) ? $languages[$folder['name']]['description'] : $folder['name'];
      echo '<tr><td><a href="'.$GO_CONFIG->host.'doc'.$GO_CONFIG->slash.$folder['name'].'">'.$language.'</a></td></tr>';
    }
  }
  echo '</table></td></tr></table>';
  require($GO_THEME->theme_path."footer.inc");

}else
{
  header('Location: '.$GO_CONFIG->host.'doc'.'/'.$GO_LANGUAGE->language['language_file'].'/');
}
?>
