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
  PRIMARY KEY  (`id`),
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
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;
