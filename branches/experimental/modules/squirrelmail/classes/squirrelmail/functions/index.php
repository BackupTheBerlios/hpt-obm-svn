<?php

/**
 * index.php
 *
 * Copyright (c) 1999-2004 The SquirrelMail Project Team
 * Licensed under the GNU GPL. For full terms see the file COPYING.
 *
 * This file simply takes any attempt to view source files and sends those
 * people to the login screen. At this point no attempt is made to see if
 * the person is logged or not.
 *
 * @version $Id: index.php,v 1.11 2004/08/24 00:40:28 jervfors Exp $
 * @package squirrelmail
 */

/** Redirect back to the login page
 * @ignore */
header("Location:../index.php");

/* pretty impressive huh? */

?>