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
# Tabel structuur voor tabel `fsShares`
#

DROP TABLE IF EXISTS `fsShares`;
CREATE TABLE `fsShares` (
  `user_id` int(11) NOT NULL default '0',
  `path` varchar(200) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`path`),
  KEY `path` (`path`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `fs_settings`
#

DROP TABLE IF EXISTS `fs_settings`;
CREATE TABLE `fs_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(4) NOT NULL default '',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Table structure for table `fsSystemFolders`
#

DROP TABLE IF EXISTS `fsSystemFolders`;
CREATE TABLE `fsSystemFolders` (
  `path` varchar(200) NOT NULL default ''
) TYPE=MyISAM COMMENT='Table to determine which folder is system folder';
