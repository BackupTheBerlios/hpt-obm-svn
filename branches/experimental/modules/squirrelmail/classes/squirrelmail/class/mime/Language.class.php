<?php

/**
 * Language.class.php
 *
 * Copyright (c) 2003-2004 The SquirrelMail Project Team
 * Licensed under the GNU GPL. For full terms see the file COPYING.
 *
 * This contains functions needed to handle mime messages.
 *
 * @version $Id: Language.class.php,v 1.5 2004/05/22 13:41:17 jervfors Exp $
 * @package squirrelmail
 */

/**
 * Undocumented class
 * @package squirrelmail
 */
class Language {
    function Language($name) {
       $this->name = $name;
       $this->properties = array();
    }
}

?>
