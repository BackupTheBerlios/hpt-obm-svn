<?php
/*
Update queries for each version from 1.95
*/
$updates[205] = array();

/******************************************************************************
UPDATES FOR 2.04
*******************************************************************************/
$updates[204][] = "ALTER TABLE `cms_sites` ADD `domain` VARCHAR( 100 ) NOT NULL AFTER `name`";
$updates[204][] = "ALTER TABLE `users` ADD `DST` ENUM( '0', '1', '2' ) NOT NULL AFTER `timezone`";

$updates[204][] = "CREATE TABLE `cms_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(20) NOT NULL default '',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `fs_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(4) NOT NULL default '',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `no_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(20) NOT NULL default '',
  `show_notes` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `pm_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(20) NOT NULL default '',
  `show_projects` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `ab_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_contacts_field` varchar(20) NOT NULL default '',
  `sort_contacts_order` varchar(4) NOT NULL default '',
  `sort_companies_field` varchar(20) NOT NULL default '',
  `sort_companies_order` varchar(4) NOT NULL default '',
  `sort_users_field` varchar(20) NOT NULL default '',
  `sort_users_order` varchar(4) NOT NULL default '',
  `search_type` varchar(10) NOT NULL default '',
  `search_contacts_field` varchar(30) NOT NULL default '',
  `search_companies_field` varchar(30) NOT NULL default '',
  `search_users_field` varchar(30) NOT NULL default '',
  `search_addressbook_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `em_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` tinyint(4) NOT NULL default '0',
  `sort_order` enum('0','1') NOT NULL default '0',
  `send_format` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `cal_views` (
  `id` int(11) NOT NULL default '0',
  `standard` enum('0','1') NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `start_hour` tinyint(4) NOT NULL default '0',
  `end_hour` tinyint(4) NOT NULL default '0',
  `type` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `cal_views_calendars` (
  `view_id` int(11) NOT NULL default '0',
  `calendar_id` int(11) NOT NULL default '0'
) TYPE=MyISAM;";

$updates[204][] = "CREATE TABLE `cal_settings` (
  `user_id` int(11) NOT NULL default '0',
  `default_cal_id` int(11) NOT NULL default '0',
  `default_view_id` int(11) NOT NULL default '0',
  `show_days` tinyint(4) NOT NULL default '0',
  `hide_completed_todos` enum('0','1') NOT NULL default '0',
  `show_todos` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;";
$updates[204][] = "ALTER TABLE `users` DROP `samba_user`;";
$updates[204][] = "ALTER TABLE `modules` ADD `version` VARCHAR( 5 ) NOT NULL AFTER `id`;";

/******************************************************************************
UPDATES FOR 2.03
*******************************************************************************/
$updates[203][] = "CREATE TABLE `cal_todos` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `contact_id` int(11) NOT NULL default '0',
  `ctime` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  `start_time` int(11) NOT NULL default '0',
  `due_time` int(11) NOT NULL default '0',
  `completion_time` int(11) NOT NULL default '0',
  `remind_time` int(11) NOT NULL default '0',
  `remind_style` enum('0','1') NOT NULL default '0',
  `status` tinyint(4) NOT NULL default '0',
  `priority` enum('0','1','2') NOT NULL default '0',
  `res_user_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `description` text NOT NULL,
  `location` varchar(50) NOT NULL default '',
  `background` varchar(6) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`,`res_user_id`),
  KEY `remind_time` (`remind_time`)
) TYPE=MyISAM;";

$updates[203][] = "ALTER TABLE `cal_reminders` DROP INDEX `event_id`";
$updates[203][] = "ALTER TABLE `cal_reminders` ADD INDEX ( `remind_time` )";

/******************************************************************************
UPDATES FOR 2.02
*******************************************************************************/
$updates[202][] = "ALTER TABLE `ab_contacts` ADD `user_id` INT NOT NULL AFTER `id`";
$updates[202][] = "ALTER TABLE `ab_companies` ADD `user_id` INT NOT NULL AFTER `id`";
$updates[202][] = "ALTER TABLE `ab_companies` DROP PRIMARY KEY , ADD PRIMARY KEY ( `id` )";
$updates[202][] = "ALTER TABLE `ab_companies` ADD INDEX ( `addressbook_id` )";
$updates[202][] = "CREATE TABLE `bm_catagories` (
									`id` int(11) NOT NULL default '0',
									`user_id` int(11) NOT NULL default '0',
									`name` varchar(50) NOT NULL default '',
									`acl_read` int(11) NOT NULL default '0',
									`acl_write` int(11) NOT NULL default '0',
									PRIMARY KEY  (`id`)
								) TYPE=MyISAM;";
$updates[202][] = "ALTER TABLE `bookmarks` RENAME `bm_bookmarks`";
$updates[202][] = "ALTER TABLE `bm_bookmarks` ADD `catagory_id` INT NOT NULL AFTER `user_id`";
$updates[202][] = "ALTER TABLE `bm_bookmarks` ADD `acl_read` INT NOT NULL AFTER `new_window`";
$updates[202][] = "ALTER TABLE `bm_bookmarks` ADD `acl_write` INT NOT NULL AFTER `acl_read`";
$updates[202][] = "ALTER TABLE `cal_events` ADD `ctime` INT NOT NULL , ADD `mtime` INT NOT NULL";
$updates[202][] = "CREATE TABLE `ab_custom_contact_fields_cats` (
									`id` INT NOT NULL ,
									`name` VARCHAR( 50 ) NOT NULL ,
									`acl_id` INT NOT NULL ,
									PRIMARY KEY ( `id` )
									);";

$updates[202][] = "CREATE TABLE `ab_custom_company_fields_cats` (
									`id` INT NOT NULL ,
									`name` VARCHAR( 50 ) NOT NULL ,
									`acl_id` INT NOT NULL ,
									PRIMARY KEY ( `id` )
									);";
$updates[202][] = "ALTER TABLE `ab_custom_contact_fields_sort` ADD `catagory_id` INT NOT NULL FIRST";
$updates[202][] = "ALTER TABLE `ab_custom_contact_fields_sort` ADD INDEX ( `catagory_id` )";
$updates[202][] = "ALTER TABLE `ab_custom_company_fields_sort` ADD `catagory_id` INT NOT NULL FIRST";
$updates[202][] = "ALTER TABLE `ab_custom_company_fields_sort` ADD INDEX ( `catagory_id` )";

/******************************************************************************
UPDATES FOR 2.01
*******************************************************************************/
$updates[201] = array();

/******************************************************************************
UPDATES FOR 2.0
*******************************************************************************/
$updates[200][] = "ALTER TABLE `users` ADD `first_weekday` TINYINT NOT NULL";

/******************************************************************************
UPDATES FOR 1.98
*******************************************************************************/
$updates[198] = array();

/******************************************************************************
UPDATES FOR 1.97
*******************************************************************************/
$updates[197][] = "ALTER TABLE `users` ADD `language` VARCHAR( 20 ) NOT NULL, ADD `theme` VARCHAR( 20 ) NOT NULL";
$updates[197][] = "CREATE TABLE `tp_mailing_companies` (
  `group_id` int(11) NOT NULL default '0',
  `company_id` int(11) NOT NULL default '0',
  KEY `group_id` (`group_id`)
) TYPE=MyISAM;";
$updates[197][] = "CREATE TABLE `tp_mailing_contacts` (
  `group_id` int(11) NOT NULL default '0',
  `contact_id` int(11) NOT NULL default '0',
  KEY `group_id` (`group_id`)
) TYPE=MyISAM;";
$updates[197][] = "CREATE TABLE `tp_mailing_groups` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;";

$updates[197][] = "CREATE TABLE `tp_subscribed` (
  `user_id` int(11) NOT NULL default '0',
  `template_id` int(11) NOT NULL default '0',
  KEY `user_id` (`user_id`,`template_id`)
) TYPE=MyISAM;";

$updates[197][] = "CREATE TABLE `tp_templates` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `type` tinyint(4) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `content` longblob NOT NULL,
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;";

/******************************************************************************
UPDATES FOR 1.96
*******************************************************************************/

$updates[196][] = "CREATE TABLE `ab_custom_company_fields` (
  `id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;";

$updates[196][] = "CREATE TABLE `ab_custom_company_fields_sort` (
  `field` varchar(50) NOT NULL default '',
  `size` tinyint(4) NOT NULL default '0',
  `sort_index` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`field`)
) TYPE=MyISAM;";

$updates[196][] = "CREATE TABLE `ab_custom_contact_fields` (
  `id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;";

$updates[196][] = "CREATE TABLE `ab_custom_contact_fields_sort` (
  `field` varchar(50) NOT NULL default '',
  `size` tinyint(4) NOT NULL default '0',
  `sort_index` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`field`),
  KEY `sort_index` (`sort_index`)
) TYPE=MyISAM;";

$updates[196][] = "ALTER TABLE `users` ADD `start_module` VARCHAR( 50 ) NOT NULL ;";
$updates[196][] = "ALTER TABLE `emAccounts` ADD `auto_check` ENUM( '0', '1' ) NOT NULL ;";
$updates[196][] = "ALTER TABLE `ab_contacts` DROP `homepage` ,
DROP `work_address` ,
DROP `work_zip` ,
DROP `work_city` ,
DROP `work_state` ,
DROP `work_country`;";

$updates[196][] = "ALTER TABLE `ab_contacts` CHANGE `company` `company_id` INT NOT NULL;";
$updates[196][] = "CREATE TABLE `ab_companies` (
  `id` int(11) NOT NULL default '0',
  `addressbook_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `address` varchar(100) NOT NULL default '',
  `zip` varchar(10) NOT NULL default '',
  `city` varchar(50) NOT NULL default '',
  `state` varchar(50) NOT NULL default '',
  `country` varchar(50) NOT NULL default '',
  `phone` varchar(20) NOT NULL default '',
  `fax` varchar(20) NOT NULL default '',
  `email` varchar(75) NOT NULL default '',
  `homepage` varchar(100) NOT NULL default '',
  `bank_no` varchar(20) NOT NULL default '',
  `vat_no` varchar(30) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0'
) TYPE=MyISAM;";

/******************************************************************************
UPDATES FOR 1.95
*******************************************************************************/
$updates[195][] = "ALTER TABLE `ab_contacts` ADD `acl_read` INT NOT NULL;";
$updates[195][] = "ALTER TABLE `ab_contacts` ADD `acl_write` INT NOT NULL;";
$updates[195][] = "ALTER TABLE `acl_items` ADD `user_id` INT NOT NULL;";
$updates[195][] = "ALTER TABLE `pmProjects` ADD `probability` TINYINT NOT NULL ,
ADD `fee_id` INT NOT NULL ,
ADD `budget` INT NOT NULL ,
ADD `description` VARCHAR(50) NOT NULL;";

$updates[195][] = "ALTER TABLE `pmHours` DROP `fee_id`;";

$updates[195][] = "CREATE TABLE `no_notes` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `contact_id` int(11) NOT NULL default '0',
  `project_id` int(11) NOT NULL default '0',
  `file_path` varchar(255) NOT NULL default '0',
  `catagory_id` int(11) NOT NULL default '0',
  `res_user_id` int(11) NOT NULL default '0',
  `due_date` int(11) NOT NULL default '0',
  `ctime` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `content` text NOT NULL,
  `acl_read` INT NOT NULL,
  `acl_write` INT NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`,`contact_id`,`project_id`),
  KEY `file_path` (`file_path`)
) TYPE=MyISAM;";

$updates[195][] = "CREATE TABLE `no_catagories` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;";

$updates[195][] = "CREATE TABLE `cal_holidays` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `calendar_id` int(11) NOT NULL default '0',
  `region` varchar ( 4 ) NOT NULL default '',
  `date` int(10) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;";
$updates[195][] = "ALTER TABLE `users` CHANGE `birthday` `birthday` DATE DEFAULT '0000-00-00 00:00:00' NOT NULL;";
$updates[195][] = "ALTER TABLE `ab_contacts` CHANGE `birthday` `birthday` DATE DEFAULT '0000-00-00 00:00:00' NOT NULL;";
$updates[195][] = "ALTER TABLE `users` CHANGE `password` `password` VARCHAR( 64 ) NOT NULL;";
$updates[195][] = "ALTER TABLE `emAccounts` CHANGE `password` `password` VARCHAR( 64 ) NOT NULL;";

/******************************************************************************
OLDER VERSIONS NOT SUPPORTED BY THIS INSTALLER. DOWNLOAD 1.95 IF YOU WANT TO
UPGRADE FROM AN OLDER VERSION THEN 1.95.
*******************************************************************************/
?>
