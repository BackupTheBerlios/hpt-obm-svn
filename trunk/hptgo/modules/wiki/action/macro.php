<?php
// $Id: macro.php,v 1.1.1.1 2004/08/03 11:50:16 vutp Exp $

require('parse/macros.php');
require('parse/html.php');

// Execute a macro directly from the URL.
function action_macro()
{
  global $ViewMacroEngine, $macro, $parms;

  if(!empty($ViewMacroEngine[$macro]))
  {
    print $ViewMacroEngine[$macro]($parms);
  }
}
?>
