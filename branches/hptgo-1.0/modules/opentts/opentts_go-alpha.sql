# phpMyAdmin SQL Dump
# version 2.5.3-rc3
# http://www.phpmyadmin.net
#
# Host: localhost
# Generation Time: May 02, 2004 at 03:10 PM
# Server version: 3.23.58
# PHP Version: 4.3.4
# 
# Database : `go-alpha`
# 

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_activities`
#

DROP TABLE IF EXISTS `nuke_opentts_activities`;
CREATE TABLE `nuke_opentts_activities` (
  `activity_id` tinyint(3) NOT NULL default '0',
  `activity_name` varchar(50) NOT NULL default ''
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_activities`
#

INSERT INTO `nuke_opentts_activities` VALUES (0, 'Inactive');
INSERT INTO `nuke_opentts_activities` VALUES (1, 'Active');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_categories`
#

DROP TABLE IF EXISTS `nuke_opentts_categories`;
CREATE TABLE `nuke_opentts_categories` (
  `category_id` int(11) NOT NULL default '0',
  `category_name` varchar(100) NOT NULL default '',
  PRIMARY KEY  (`category_id`)
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_categories`
#

INSERT INTO `nuke_opentts_categories` VALUES (1, 'general');
INSERT INTO `nuke_opentts_categories` VALUES (2, 'bug report');
INSERT INTO `nuke_opentts_categories` VALUES (3, 'wish list');
INSERT INTO `nuke_opentts_categories` VALUES (4, 'how to');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_colors_tables`
#

DROP TABLE IF EXISTS `nuke_opentts_colors_tables`;
CREATE TABLE `nuke_opentts_colors_tables` (
  `clr_tbl_id` int(11) NOT NULL default '0',
  `fnt_clr` varchar(20) NOT NULL default '',
  `bck_clr` varchar(20) NOT NULL default ''
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_colors_tables`
#

INSERT INTO `nuke_opentts_colors_tables` VALUES (0, 'black', 'white');
INSERT INTO `nuke_opentts_colors_tables` VALUES (1, 'white', 'black');
INSERT INTO `nuke_opentts_colors_tables` VALUES (2, 'red', 'yellow');
INSERT INTO `nuke_opentts_colors_tables` VALUES (4, 'blue', 'white');
INSERT INTO `nuke_opentts_colors_tables` VALUES (5, 'green', 'white');
INSERT INTO `nuke_opentts_colors_tables` VALUES (3, 'red', 'blue');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_config`
#

DROP TABLE IF EXISTS `nuke_opentts_config`;
CREATE TABLE `nuke_opentts_config` (
  `varname` varchar(128) NOT NULL default '',
  `definition` text,
  UNIQUE KEY `varname` (`varname`)
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_config`
#

INSERT INTO `nuke_opentts_config` VALUES ('tts_title', 'JCE HELP DESK');
INSERT INTO `nuke_opentts_config` VALUES ('assigned_subject', 'from JCE Helpdesk [ticket #$Ticket_Number]');
INSERT INTO `nuke_opentts_config` VALUES ('powered_by_riunx', '@ powered by <a href="www.riunx.com">RIUNX</a>');
INSERT INTO `nuke_opentts_config` VALUES ('welcome_message', '<h3>Ticket Tracking System</3>');
INSERT INTO `nuke_opentts_config` VALUES ('hlpdsk_theme', 'Default');
INSERT INTO `nuke_opentts_config` VALUES ('timezone', '+2');
INSERT INTO `nuke_opentts_config` VALUES ('assigned_msg_body', 'Ticket $Ticket_Number : $t_subject $t_description\r\n<a href=\\"modules.php?name=Helpdesk&file=showline&Ticket_Number=$Ticket_Number\\">Ticket $Ticket_Number</a>');
INSERT INTO `nuke_opentts_config` VALUES ('issuer_subject', 'from JCE Helpdesk [ticket #$Ticket_Number]');
INSERT INTO `nuke_opentts_config` VALUES ('issuer_msg_body', 'Greetings, This message has been automatically generated in response to the creation of a trouble ticket regarding: $t_subject. There is no need to reply to this message at this time. Your ticket has been assigned an ID of [Tracker #$Ticket_Number]: http://portal.jce.ac.il/portal/modules.php?name=Helpdesk . Please include the string: [Tracker #$Ticket_Number] in the subject line of all future correspondence about this issue. To do so, you may reply to this message. Thank you, $hlpdsk_email');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_groups`
#

DROP TABLE IF EXISTS `nuke_opentts_groups`;
CREATE TABLE `nuke_opentts_groups` (
  `gid` int(11) NOT NULL default '0',
  `group_name` varchar(127) NOT NULL default '',
  `description` varchar(255) default NULL,
  `permissions` varchar(255) default NULL,
  UNIQUE KEY `gid` (`gid`,`group_name`)
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_groups`
#

INSERT INTO `nuke_opentts_groups` VALUES (1, 'clients', NULL, NULL);
INSERT INTO `nuke_opentts_groups` VALUES (2, 'agents', NULL, NULL);
INSERT INTO `nuke_opentts_groups` VALUES (3, 'manager', NULL, NULL);
INSERT INTO `nuke_opentts_groups` VALUES (4, 'auditor', NULL, NULL);
INSERT INTO `nuke_opentts_groups` VALUES (5, 'administrators', 'TTS administrators', NULL);
INSERT INTO `nuke_opentts_groups` VALUES (0, 'anonymous', NULL, NULL);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_groups_members`
#

DROP TABLE IF EXISTS `nuke_opentts_groups_members`;
CREATE TABLE `nuke_opentts_groups_members` (
  `uid` int(11) NOT NULL default '0',
  `gid` int(11) NOT NULL default '0',
  `uid_default` smallint(3) NOT NULL default '0',
  PRIMARY KEY  (`uid`,`gid`)
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_groups_members`
#

INSERT INTO `nuke_opentts_groups_members` VALUES (1, 5, 0);
INSERT INTO `nuke_opentts_groups_members` VALUES (1, 3, 0);
INSERT INTO `nuke_opentts_groups_members` VALUES (1, 2, 0);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_lang`
#

DROP TABLE IF EXISTS `nuke_opentts_lang`;
CREATE TABLE `nuke_opentts_lang` (
  `definition` varchar(128) NOT NULL default '',
  `english` varchar(255) default NULL,
  `hebrew` varchar(255) default NULL,
  `spanish` varchar(255) default NULL,
  UNIQUE KEY `definition` (`definition`),
  KEY `definition_2` (`definition`)
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_lang`
#

INSERT INTO `nuke_opentts_lang` VALUES ('_CATEGORY', 'Category', 'קטגוריה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_SEARCH', 'Search', 'חיפוש', 'Buscar');
INSERT INTO `nuke_opentts_lang` VALUES ('_MAIL_THIS', 'Mail this', 'שלח בדואל', 'envie por email');
INSERT INTO `nuke_opentts_lang` VALUES ('_DUE_DATE', 'Due date', 'תאריך יעד', 'Fecha programada');
INSERT INTO `nuke_opentts_lang` VALUES ('_POST_DATE', 'Post date', 'תאריך פתיחה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_LIMIT', 'Limit', 'גבול', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_ON_FIELD', 'on Field', NULL, 'En campo');
INSERT INTO `nuke_opentts_lang` VALUES ('_ORDER_BY', 'order by', 'סדר', 'Ordenar por');
INSERT INTO `nuke_opentts_lang` VALUES ('_SHOW_ALL_TICKETS', 'Show All (inlude Closed Tickets)', 'הראה את כל הכרטיסים', 'Mostrar todos(inclusive boletos cerrados)');
INSERT INTO `nuke_opentts_lang` VALUES ('_QUERY', 'Query', 'חקירה', 'Pregunta');
INSERT INTO `nuke_opentts_lang` VALUES ('_TOTAL_RECORDS_FOUND', 'Total Records found', 'סך-הכל ממצאים נמצאו', 'Total de registros encontrados');
INSERT INTO `nuke_opentts_lang` VALUES ('_STATUS', 'Status', 'מעמד', 'Status');
INSERT INTO `nuke_opentts_lang` VALUES ('_PRIORITY', 'Priority', 'עדיפות', 'Prioridad');
INSERT INTO `nuke_opentts_lang` VALUES ('_ISSUER', 'Issuer', 'מנפיק', 'Remitente');
INSERT INTO `nuke_opentts_lang` VALUES ('_SUBJECT', 'Subject', 'נושא', 'Sujeto');
INSERT INTO `nuke_opentts_lang` VALUES ('_ASSIGNED', 'Assigned', 'הקציב', 'Asignado');
INSERT INTO `nuke_opentts_lang` VALUES ('_STAGE', 'Stage', 'רמה', 'Nivel');
INSERT INTO `nuke_opentts_lang` VALUES ('_DATE', 'Date', 'תאריך', 'Fecha');
INSERT INTO `nuke_opentts_lang` VALUES ('_TIMER', 'Timer', 'קוצב-זמן', 'Cronometro');
INSERT INTO `nuke_opentts_lang` VALUES ('_TICKET_NR', 'Ticket Nr.', 'מספר כרטיס או דו"ח', 'Boleto nr.');
INSERT INTO `nuke_opentts_lang` VALUES ('_ADDING_TICKET', 'ADDING A NEW TICKET', 'הוסף כרטיס חדש', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_DESCRIPTION', 'Description', 'תאור', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_ESTIMATED_TIME', 'Estimated time', 'הערכת זמן', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_ASSIGN_TO', 'Assign to', 'הקציב ל-', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_NOTIFY_BY', 'Notify by', 'הודיע ל', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_PRIV_MSG', 'Private message', 'הודעה פרטית', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_EMAIL', 'Email', 'דואר-אלקטרוני', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_COMMENTS', 'Comments', 'הערה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_NOTE_ENTRY_1', '*Task number will be assigned on Submit.', 'מספר משימה יוקצה לשליחה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_NOTE_ENTRY_2', '**Please enter detailed information.', '**בבקשה הכנס פרטי מידע', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TICKET_ADDED', 'Ticket Added', 'הוסף כרטיס', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_ADDED_SUCCESSFULY', 'Added successfuly!', 'הוסף בהצלחה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_CONTINUE', 'Continue', 'המשך', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TASK_ADDED', 'New task Added', 'הוסף משימה חדשה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TASK_NR', 'Task nr.', 'מספר משימה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_ADD_TASK', 'Add Task', 'הוסף משימה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_CLOSE_TICKET', 'Close Ticket', 'סגור כרטיס', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TTS_DETAILS', 'TTS - Your ticket Detail', 'את פרטי הכרטיס', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TIME', 'Time', 'זמן', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TICKET_CLOSED', 'Ticket closed', 'TTS -סגור כרטיס', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_REASSIGN', 'Reassign to', 'הקצה מחדש', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_CHANGE_STATUS', 'Change status to task', 'שנה מעמד בשביל המשימה', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TICKET_MODIFIED', 'Ticket status mofified!', 'התאם מצב כרטיס', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_REASSIGNED_TO', 'Re-assigned to', 'הקצה מחדש ל-', NULL);
INSERT INTO `nuke_opentts_lang` VALUES ('_TICKET_REASSIGNED', 'Ticket Task Re-assigned!', 'הקצה מחדש משימת-כרטיס', NULL);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_menu`
#

DROP TABLE IF EXISTS `nuke_opentts_menu`;
CREATE TABLE `nuke_opentts_menu` (
  `menu_id` int(11) NOT NULL auto_increment,
  `title` varchar(30) NOT NULL default '',
  `file` varchar(30) NOT NULL default '',
  `link` varchar(30) NOT NULL default '',
  UNIQUE KEY `menu_id` (`menu_id`)
) TYPE=MyISAM AUTO_INCREMENT=7 ;

#
# Dumping data for table `nuke_opentts_menu`
#

INSERT INTO `nuke_opentts_menu` VALUES (1, 'List', 'my_tickets.php', 'my_tickets.php');
INSERT INTO `nuke_opentts_menu` VALUES (2, 'Search', 'queries.php', 'queries.php');
INSERT INTO `nuke_opentts_menu` VALUES (3, 'New', 'entry.php', 'entry.php');
INSERT INTO `nuke_opentts_menu` VALUES (4, 'Admin', 'admin.php', 'admin.php');
INSERT INTO `nuke_opentts_menu` VALUES (5, 'Start', 'index.php', 'index.php');
INSERT INTO `nuke_opentts_menu` VALUES (6, 'Stat', 'statistics.php', 'statistics.php');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_permissions`
#

DROP TABLE IF EXISTS `nuke_opentts_permissions`;
CREATE TABLE `nuke_opentts_permissions` (
  `gid` bigint(11) NOT NULL default '0',
  `action_id` int(11) NOT NULL default '0',
  `description` varchar(255) NOT NULL default ''
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_permissions`
#

INSERT INTO `nuke_opentts_permissions` VALUES (3, 35, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 34, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 33, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 32, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 31, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 30, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 29, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 28, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 27, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 26, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 25, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 24, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 23, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 35, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 22, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 21, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 20, '');
INSERT INTO `nuke_opentts_permissions` VALUES (1, 21, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 34, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 19, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 33, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 18, '');
INSERT INTO `nuke_opentts_permissions` VALUES (5, 24, '');
INSERT INTO `nuke_opentts_permissions` VALUES (5, 22, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 32, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 28, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 17, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 24, '');
INSERT INTO `nuke_opentts_permissions` VALUES (1, 19, '');
INSERT INTO `nuke_opentts_permissions` VALUES (1, 12, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 23, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 21, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 16, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 20, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 15, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 14, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 13, '');
INSERT INTO `nuke_opentts_permissions` VALUES (5, 2, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 19, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 18, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 12, '');
INSERT INTO `nuke_opentts_permissions` VALUES (2, 11, '');
INSERT INTO `nuke_opentts_permissions` VALUES (1, 28, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 12, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 11, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 10, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 9, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 8, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 7, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 5, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 4, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 3, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 2, '');
INSERT INTO `nuke_opentts_permissions` VALUES (3, 1, '');
INSERT INTO `nuke_opentts_permissions` VALUES (5, 6, '');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_permissions_users`
#

DROP TABLE IF EXISTS `nuke_opentts_permissions_users`;
CREATE TABLE `nuke_opentts_permissions_users` (
  `uid` int(11) NOT NULL default '0',
  `action_id` int(11) NOT NULL default '0'
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_permissions_users`
#

INSERT INTO `nuke_opentts_permissions_users` VALUES (3, 6);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_priorities`
#

DROP TABLE IF EXISTS `nuke_opentts_priorities`;
CREATE TABLE `nuke_opentts_priorities` (
  `priority_id` tinyint(4) NOT NULL default '0',
  `priority_name` varchar(100) NOT NULL default '',
  `select_id` tinyint(4) NOT NULL default '0'
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_priorities`
#

INSERT INTO `nuke_opentts_priorities` VALUES (1, 'Low', 1);
INSERT INTO `nuke_opentts_priorities` VALUES (2, 'Normal', 1);
INSERT INTO `nuke_opentts_priorities` VALUES (3, 'High', 1);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_projects`
#

DROP TABLE IF EXISTS `nuke_opentts_projects`;
CREATE TABLE `nuke_opentts_projects` (
  `project_id` int(11) unsigned NOT NULL auto_increment,
  `project_name` varchar(100) NOT NULL default '',
  `project_description` varchar(255) default NULL,
  `privacy` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`project_id`)
) TYPE=MyISAM AUTO_INCREMENT=2 ;

#
# Dumping data for table `nuke_opentts_projects`
#

INSERT INTO `nuke_opentts_projects` VALUES (1, 'GO-TTS', NULL, 0);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_stages`
#

DROP TABLE IF EXISTS `nuke_opentts_stages`;
CREATE TABLE `nuke_opentts_stages` (
  `stage_id` tinyint(3) NOT NULL default '1',
  `stage_name` varchar(100) NOT NULL default 'undefined',
  PRIMARY KEY  (`stage_id`)
) TYPE=MyISAM;

#
# Dumping data for table `nuke_opentts_stages`
#

INSERT INTO `nuke_opentts_stages` VALUES (1, 'open');
INSERT INTO `nuke_opentts_stages` VALUES (2, 'closed');

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_status`
#

DROP TABLE IF EXISTS `nuke_opentts_status`;
CREATE TABLE `nuke_opentts_status` (
  `status_id` int(11) NOT NULL auto_increment,
  `status_name` varchar(100) NOT NULL default '',
  `show_by_default` tinyint(3) unsigned default '1',
  PRIMARY KEY  (`status_id`)
) TYPE=MyISAM AUTO_INCREMENT=7 ;

#
# Dumping data for table `nuke_opentts_status`
#

INSERT INTO `nuke_opentts_status` VALUES (1, 'waiting', 1);
INSERT INTO `nuke_opentts_status` VALUES (2, 'Open', 1);
INSERT INTO `nuke_opentts_status` VALUES (3, 'In Progress', 1);
INSERT INTO `nuke_opentts_status` VALUES (4, 'Done', 1);
INSERT INTO `nuke_opentts_status` VALUES (5, 'Cancelled', 1);

# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_tasks`
#

DROP TABLE IF EXISTS `nuke_opentts_tasks`;
CREATE TABLE `nuke_opentts_tasks` (
  `task_id` int(11) NOT NULL auto_increment,
  `ticket_id` int(11) NOT NULL default '0',
  `sender_id` int(11) NOT NULL default '0',
  `comment` varchar(255) NOT NULL default '',
  `post_date` varchar(25) NOT NULL default '',
  `email_issuer` int(11) NOT NULL default '0',
  `email_agent` int(11) NOT NULL default '0',
  KEY `sender_id` (`sender_id`),
  KEY `task_id` (`task_id`,`ticket_id`)
) TYPE=MyISAM AUTO_INCREMENT=1 ;

#
# Dumping data for table `nuke_opentts_tasks`
#


# --------------------------------------------------------

#
# Table structure for table `nuke_opentts_tickets`
#

DROP TABLE IF EXISTS `nuke_opentts_tickets`;
CREATE TABLE `nuke_opentts_tickets` (
  `ticket_number` int(11) NOT NULL auto_increment,
  `t_assigned` varchar(255) NOT NULL default '',
  `t_from` varchar(255) NOT NULL default '',
  `t_stage` int(3) NOT NULL default '0',
  `t_priority` tinyint(3) NOT NULL default '1',
  `t_category` tinyint(3) NOT NULL default '1',
  `t_subject` varchar(255) NOT NULL default '',
  `t_description` text NOT NULL,
  `t_comments` varchar(255) NOT NULL default '',
  `post_date` varchar(12) NOT NULL default '',
  `due_date` varchar(12) NOT NULL default '',
  `change_date` varchar(12) NOT NULL default '',
  `t_status` varchar(255) NOT NULL default '',
  `t_sms` varchar(255) NOT NULL default '',
  `t_email` varchar(255) NOT NULL default '',
  `transac_id` varchar(128) default NULL,
  `activity_id` tinyint(3) NOT NULL default '0',
  `project_id` int(11) default '1',
  `end_date` varchar(12) NOT NULL default '',
  `complete` int(3) NOT NULL default '0',
  PRIMARY KEY  (`ticket_number`),
  KEY `t_category` (`t_category`)
) TYPE=MyISAM AUTO_INCREMENT=2 ;

#
# Dumping data for table `nuke_opentts_tickets`
#

INSERT INTO `nuke_opentts_tickets` VALUES (1, '1', '1', 1, 1, 2, 'testing', '', '', '1083499089', '1083499089', '1083499089', '1', '', '', '11083499089', 0, 1, '1083499089', 0);
