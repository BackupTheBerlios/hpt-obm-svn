# phpMyAdmin SQL Dump
# version 2.5.5-pl1
# http://www.phpmyadmin.net
#
# Host: localhost
# Generatie Tijd: 19 Apr 2004 om 10:46
# Server versie: 3.23.58
# PHP Versie: 4.3.4
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_calendars`
#

DROP TABLE IF EXISTS `cal_calendars`;
CREATE TABLE `cal_calendars` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `start_hour` tinyint(4) NOT NULL default '0',
  `end_hour` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_events`
#

DROP TABLE IF EXISTS `cal_events`;
CREATE TABLE `cal_events` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `start_time` int(11) NOT NULL default '0',
  `end_time` int(11) NOT NULL default '0',
  `all_day_event` enum('0','1') NOT NULL default '0',
  `contact_id` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `description` text NOT NULL,
  `location` varchar(100) NOT NULL default '',
  `background` varchar(7) NOT NULL default '',
  `repeat_type` enum('0','1','2','3','4','5') NOT NULL default '0',
  `repeat_forever` enum('0','1') NOT NULL default '0',
  `repeat_every` tinyint(4) NOT NULL default '0',
  `repeat_end_time` int(11) NOT NULL default '0',
  `mon` enum('0','1') NOT NULL default '0',
  `tue` enum('0','1') NOT NULL default '0',
  `wed` enum('0','1') NOT NULL default '0',
  `thu` enum('0','1') NOT NULL default '0',
  `fri` enum('0','1') NOT NULL default '0',
  `sat` enum('0','1') NOT NULL default '0',
  `sun` enum('0','1') NOT NULL default '0',
  `month_time` enum('0','1','2','3','4','5') NOT NULL default '0',
  `reminder` int(11) NOT NULL default '0',
  `ctime` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `start_time` (`start_time`),
  KEY `end_time` (`end_time`),
  KEY `repeat_end_time` (`repeat_end_time`),
  KEY `acl_read` (`acl_read`),
  KEY `acl_write` (`acl_write`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_events_calendars`
#

DROP TABLE IF EXISTS `cal_events_calendars`;
CREATE TABLE `cal_events_calendars` (
  `calendar_id` int(11) NOT NULL default '0',
  `event_id` int(11) NOT NULL default '0',
 PRIMARY KEY  (`calendar_id`,`event_id`),
 KEY `calendar_id` (`calendar_id`,`event_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_holidays`
#

DROP TABLE IF EXISTS `cal_holidays`;
CREATE TABLE `cal_holidays` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `calendar_id` int(11) NOT NULL default '0',
  `date` int(10) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `region` varchar(4) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_participants`
#

DROP TABLE IF EXISTS `cal_participants`;
CREATE TABLE `cal_participants` (
  `id` int(11) NOT NULL default '0',
  `event_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `email` varchar(100) NOT NULL default '',
  `user_id` int(11) NOT NULL default '0',
  `status` enum('0','1','2') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_reminders`
#

DROP TABLE IF EXISTS `cal_reminders`;
CREATE TABLE `cal_reminders` (
  `user_id` int(11) NOT NULL default '0',
  `event_id` int(11) NOT NULL default '0',
  `remind_time` int(11) NOT NULL default '0',
  PRIMARY KEY  (`user_id`,`event_id`,`remind_time`),
 KEY `user_id` (`user_id`),
  KEY `remind_time` (`remind_time`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_settings`
#

DROP TABLE IF EXISTS `cal_settings`;
CREATE TABLE `cal_settings` (
  `user_id` int(11) NOT NULL default '0',
  `default_cal_id` int(11) NOT NULL default '0',
  `default_view_id` int(11) NOT NULL default '0',
  `show_days` tinyint(4) NOT NULL default '0',
  `hide_completed_todos` enum('0','1') NOT NULL default '0',
  `show_todos` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_todos`
#

DROP TABLE IF EXISTS `cal_todos`;
CREATE TABLE `cal_todos` (
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
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_views`
#

DROP TABLE IF EXISTS `cal_views`;
CREATE TABLE `cal_views` (
  `id` int(11) NOT NULL default '0',
  `standard` enum('0','1') NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `start_hour` tinyint(4) NOT NULL default '0',
  `end_hour` tinyint(4) NOT NULL default '0',
  `type` varchar(10) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cal_views_calendars`
#

DROP TABLE IF EXISTS `cal_views_calendars`;
CREATE TABLE `cal_views_calendars` (
  `view_id` int(11) NOT NULL default '0',
  `calendar_id` int(11) NOT NULL default '0'
) TYPE=MyISAM;

DROP TABLE IF EXISTS `cal_config`;
CREATE TABLE cal_config (
  user_id int(11) unsigned NOT NULL default '0',
  calendar_id int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (calendar_id,user_id)
) TYPE=MyISAM;

DROP TABLE IF EXISTS `cal_view_subscriptions`;
CREATE TABLE cal_view_subscriptions (
  user_id int(11) unsigned NOT NULL default '0',
  view_id int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (view_id,user_id)
) TYPE=MyISAM;

      
