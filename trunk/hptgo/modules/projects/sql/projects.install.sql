# phpMyAdmin SQL Dump
# version 2.5.5-pl1
# http://www.phpmyadmin.net
#
# Host: localhost
# Generatie Tijd: 19 Apr 2004 om 10:49
# Server versie: 3.23.58
# PHP Versie: 4.3.4
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Tabel structuur voor tabel `pmFees`
#

DROP TABLE IF EXISTS `pmFees`;
CREATE TABLE `pmFees` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `value` float NOT NULL default '0',
  `time` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `pmHours`
#

DROP TABLE IF EXISTS `pmHours`;
CREATE TABLE `pmHours` (
  `id` int(11) NOT NULL default '0',
  `project_id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `start_time` int(11) NOT NULL default '0',
  `end_time` int(11) NOT NULL default '0',
  `break_time` int(11) NOT NULL default '0',
  `unit_value` int(11) NOT NULL default '0',
  `comments` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `pmMaterials`
#

DROP TABLE IF EXISTS `pmMaterials`;
CREATE TABLE `pmMaterials` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `value` double NOT NULL default '0',
  `description` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `pmProjects`
#

DROP TABLE IF EXISTS `pmProjects`;
CREATE TABLE `pmProjects` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `description` varchar(50) NOT NULL default '',
  `contact_id` int(11) NOT NULL default '0',
  `res_user_id` int(11) NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `comments` text NOT NULL,
  `ctime` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  `start_date` int(11) NOT NULL default '0',
  `end_date` int(11) NOT NULL default '0',
  `status` tinyint(10) NOT NULL default '0',
  `probability` tinyint(4) NOT NULL default '0',
  `fee_id` int(11) NOT NULL default '0',
  `budget` int(11) NOT NULL default '0',
  `task_template_id` int(11) NOT NULL default '0',
  `cat_id` int(11) NOT NULL default '0',
  KEY `id` (`id`),
  KEY `contact_id` (`contact_id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `pmTimers`
#

DROP TABLE IF EXISTS `pmTimers`;
CREATE TABLE `pmTimers` (
  `user_id` int(11) NOT NULL default '0',
  `start_time` int(11) NOT NULL default '0',
  `project_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `pm_settings`
#

DROP TABLE IF EXISTS `pm_settings`;
CREATE TABLE `pm_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(20) NOT NULL default '',
  `show_projects` tinyint(4) NOT NULL default '0',
  `show_catalog` int(11) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Table structure for table `pmCatalog`
#

CREATE TABLE `pmCatalog` (
  `id` int(11) NOT NULL auto_increment,
  `name` char(100) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM COMMENT='Project catalog';

# --------------------------------------------------------

#
# Table structure for table `pmStatus`
#

CREATE TABLE `pmStatus` (
  `cat_id` int(11) NOT NULL default '0',
  `value` int(11) NOT NULL default '0',
  `name` char(50) NOT NULL default '',
  PRIMARY KEY  (`cat_id`,`value`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Table structure for table `task_templates`
#

CREATE TABLE `task_templates` (
  `id` int(11) NOT NULL auto_increment,
  `cat_id` int(11) NOT NULL default '0',
  `name` char(128) default NULL,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM AUTO_INCREMENT=12 ;

# --------------------------------------------------------

#
# Table structure for table `task`
#

CREATE TABLE `task` (
  `task_project_id` int(8) NOT NULL default '0',
  `task_id` int(8) NOT NULL default '0',
  `task_name` varchar(255) default NULL,
  `task_predecessors` varchar(255) default NULL,
  `task_time` int(4) default NULL,
  `task_person_id` tinyint(4) default NULL,
  `task_status` tinyint(4) default NULL,
  `task_comment` blob,
  `task_approved` char(1) default NULL,
  `task_level` tinyint(4) default NULL,
  `task_approved_date` datetime default NULL,
  PRIMARY KEY  (`task_project_id`,`task_id`)
) TYPE=MyISAM;

