# phpMyAdmin SQL Dump
# version 2.5.5-pl1
# http://www.phpmyadmin.net
#
# Host: localhost
# Generatie Tijd: 19 Apr 2004 om 10:48
# Server versie: 3.23.58
# PHP Versie: 4.3.4
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Tabel structuur voor tabel `no_catagories`
#

DROP TABLE IF EXISTS `no_catagories`;
CREATE TABLE `no_catagories` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `no_notes`
#

DROP TABLE IF EXISTS `no_notes`;
CREATE TABLE `no_notes` (
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
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`,`contact_id`,`project_id`),
  KEY `file_path` (`file_path`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `no_settings`
#

DROP TABLE IF EXISTS `no_settings`;
CREATE TABLE `no_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(20) NOT NULL default '',
  `show_notes` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;
