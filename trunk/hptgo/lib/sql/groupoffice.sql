# phpMyAdmin SQL Dump
# version 2.5.5-pl1
# http://www.phpmyadmin.net
#
# Host: localhost
# Generatie Tijd: 06 Apr 2004 om 08:52
# Server versie: 3.23.58
# PHP Versie: 4.3.4
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Tabel structuur voor tabel `acl`
#

DROP TABLE IF EXISTS `acl`;
CREATE TABLE `acl` (
  `acl_id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `group_id` int(11) NOT NULL default '0',
  KEY `acl_id` (`acl_id`),
  KEY `user_id` (`user_id`),
  KEY `group_id` (`group_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `acl_items`
#

DROP TABLE IF EXISTS `acl_items`;
CREATE TABLE `acl_items` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `description` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `db_sequence`
#

DROP TABLE IF EXISTS `db_sequence`;
CREATE TABLE `db_sequence` (
  `seq_name` char(20) NOT NULL default '',
  `nextid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`seq_name`),
  KEY `seq_name` (`seq_name`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `filetypes`
#

DROP TABLE IF EXISTS `filetypes`;
CREATE TABLE `filetypes` (
  `extension` varchar(10) NOT NULL default '',
  `mime` varchar(50) NOT NULL default '',
  `friendly` varchar(50) NOT NULL default '',
  `image` blob NOT NULL,
  PRIMARY KEY  (`extension`),
  KEY `extension` (`extension`)
) TYPE=MyISAM COMMENT='filetypes';

# --------------------------------------------------------

#
# Tabel structuur voor tabel `groups`
#

DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `user_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `modules`
#

DROP TABLE IF EXISTS `modules`;
CREATE TABLE `modules` (
  `id` varchar(20) NOT NULL default '',
  `version` varchar(5) NOT NULL default '',
  `path` varchar(50) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `settings`
#

DROP TABLE IF EXISTS `settings`;
CREATE TABLE `settings` (
  `name` varchar(20) NOT NULL default '',
  `value` text NOT NULL,
  PRIMARY KEY  (`name`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `users`
#

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` int(11) NOT NULL default '0',
  `auth_source` tinyint(4) NOT NULL default '0',
  `username` varchar(50) NOT NULL default '',
  `password` varchar(64) NOT NULL default '',
  `authcode` varchar(20) NOT NULL default '',
  `first_name` varchar(50) NOT NULL default '',
  `middle_name` varchar(50) NOT NULL default '',
  `last_name` varchar(50) NOT NULL default '',
  `initials` varchar(10) NOT NULL default '',
  `title` varchar(10) NOT NULL default '',
  `sex` enum('M','F') NOT NULL default 'M',
  `birthday` date NOT NULL default '0000-00-00',
  `email` varchar(100) NOT NULL default '',
  `company` varchar(50) NOT NULL default '',
  `department` varchar(50) NOT NULL default '',
  `function` varchar(50) NOT NULL default '',
  `home_phone` varchar(20) NOT NULL default '',
  `work_phone` varchar(20) NOT NULL default '',
  `fax` varchar(20) NOT NULL default '',
  `cellular` varchar(20) NOT NULL default '',
  `country` varchar(50) NOT NULL default '',
  `state` varchar(50) NOT NULL default '',
  `city` varchar(50) NOT NULL default '',
  `zip` varchar(10) NOT NULL default '',
  `address` varchar(100) NOT NULL default '',
  `homepage` varchar(100) NOT NULL default '',
  `work_address` varchar(100) NOT NULL default '',
  `work_zip` varchar(10) NOT NULL default '',
  `work_country` varchar(50) NOT NULL default '',
  `work_state` varchar(50) NOT NULL default '',
  `work_city` varchar(50) NOT NULL default '',
  `work_fax` varchar(20) NOT NULL default '',
  `acl_id` int(11) NOT NULL default '0',
  `date_format` varchar(20) NOT NULL default 'd-m-Y H:i',
  `time_format` varchar(10) NOT NULL default '',
  `thousands_seperator` char(1) NOT NULL default '.',
  `decimal_seperator` char(1) NOT NULL default ',',
  `currency` char(3) NOT NULL default '€',
  `mail_client` tinyint(4) NOT NULL default '1',
  `logins` int(11) NOT NULL default '0',
  `lastlogin` int(11) NOT NULL default '0',
  `registration_time` int(11) NOT NULL default '0',
  `max_rows_list` tinyint(4) NOT NULL default '15',
  `timezone` tinyint(4) NOT NULL default '0',
  `DST` ENUM( '0', '1', '2' ) NOT NULL default '0',
  `start_module` varchar(50) NOT NULL default '',
  `language` varchar(20) NOT NULL default '',
  `theme` varchar(20) NOT NULL default '',
  `first_weekday` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `users_groups`
#

DROP TABLE IF EXISTS `users_groups`;
CREATE TABLE `users_groups` (
  `group_id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  KEY `group_id` (`group_id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;
