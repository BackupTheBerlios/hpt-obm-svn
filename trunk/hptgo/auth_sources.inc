<?php
/*
Copyright Intermesh 2004
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 30 March 2004

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

-------------------------------------------------------------------------------

Group-Office authentication configuration file

In this file you can create additional authentication sources
for Group-Office.
This is an example file. If this file contains passwords or
othe sensitive information please put it in a safe place.
Protect it with an .htaccess file or put it outside the public
web root!
*/


/*
Normal Group-Office authentication
Enabled by default.
If you disable this make sure you have an administrator
set up.
*/

$auth_sources[] = 
	array(
	'type' => 'sql',
	'name' => 'Group-Office database',
	'user_manager' => 'sql'
	);

/*
An LDAP server


$auth_sources[] = 
	array(
	'type' => 'ldap',
	'name' => 'Intermesh LDAP server',
	'user_manager' => 'sql',
	'groups' => array('Intermesh'),
	'modules_read' => array('email', 'filesystem', 'addressbook', 'calendar', 'bookmarks', 'notes', 'summary'),
	'modules_write' => array(),
	'host' => 'ldap.intermesh.nl',
	'user' => 'admin',
	'pass' => 'some_pass',
	'basedn' => 'dc=intermesh,dc=nl',
	'peopledn' => 'ou=People,dc=intermesh,dc=nl',
	'groupsdn' => 'ou=Groups,dc=intermesh,dc=nl'
	);


An IMAP server


$auth_sources[] = 
	array(
	'type' => 'email',							
	'name' => 'intermesh.nl',
	'user_manager' => 'sql',
	'proto' => 'imap',
	'domain' => 'intermesh.nl',
	'host' => 'mail.intermesh.nl',
	'port' => '143',
	'mbroot' => 'INBOX',
	'add_domain_to_username' => true,
	'create_email_account' => true,
	'groups' => array('Intermesh'),
	'modules_read' => array('email', 'filesystem', 'addressbook', 'calendar', 'bookmarks', 'notes', 'summary'),
	'modules_write' => array()
	);
*/
?>
