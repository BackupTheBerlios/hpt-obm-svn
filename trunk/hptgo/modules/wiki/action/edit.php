<?php
// $Id: edit.php,v 1.1.1.1 2004/08/03 11:50:16 vutp Exp $

require('parse/html.php');
require(TemplateDir . '/edit.php');

// Edit a page (possibly an archive version).
function action_edit()
{
  global $page, $pagestore, $ParseEngine, $version;

  $pg = $pagestore->page($page);
  $pg->read();

  if(!$pg->mutable)
    { die(ACTION_ErrorPageLocked); }

  $archive = 0;
  if($version != '')
  {
    $pg->version = $version;
    $pg->read();
    $archive = 1;
  }

  template_edit(array('page'      => $page,
                      'text'      => $pg->text,
                      'timestamp' => $pg->time,
                      'nextver'   => $pg->version + 1,
                      'archive'   => $archive));
}
?>
