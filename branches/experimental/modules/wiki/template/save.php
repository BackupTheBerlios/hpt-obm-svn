<?php
// $Id: save.php,v 1.1.1.1 2004/08/03 11:50:16 vutp Exp $

// The save template is passed an associative array with the following
// elements:
//
//   page      => A string containing the name of the wiki page being saved.
//   text      => A string containing the wiki markup for the given page.

function template_save($args)
{
// You might use this to put up some sort of "thank-you" page like Ward
//   does in WikiWiki, or to display a list of words that fail spell-check.
// For now, we simply redirect to the view action for this page.
// Removed since headers are sent by Group-Office Framework
//  header('Location: ' . viewURL($args['page']));
}
?>
