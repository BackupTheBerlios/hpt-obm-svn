# phpMyAdmin SQL Dump
# version 2.5.4
# http://www.phpmyadmin.net
#
# Host: localhost
# Generation Time: Dec 23, 2003 at 05:45 PM
# Server version: 3.23.58
# PHP Version: 4.3.3
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Table structure for table `pr_presentations`
#

DROP TABLE IF EXISTS `pr_presentations`;
CREATE TABLE `pr_presentations` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `publish_path` varchar(100) NOT NULL default '',
  `style` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Table structure for table `pr_slides`
#

DROP TABLE IF EXISTS `pr_slides`;
CREATE TABLE `pr_slides` (
  `id` int(11) NOT NULL default '0',
  `presentation_id` int(11) NOT NULL default '0',
  `time` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `content` text NOT NULL,
  `background` varchar(6) NOT NULL default '',
  `priority` int(11) NOT NULL default '0',
  `mon` enum('0','1') NOT NULL default '0',
  `tue` enum('0','1') NOT NULL default '0',
  `wed` enum('0','1') NOT NULL default '0',
  `thu` enum('0','1') NOT NULL default '0',
  `fri` enum('0','1') NOT NULL default '0',
  `sat` enum('0','1') NOT NULL default '0',
  `sun` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `presentation_id` (`presentation_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Table structure for table `pr_subscribed`
#

DROP TABLE IF EXISTS `pr_subscribed`;
CREATE TABLE `pr_subscribed` (
  `user_id` int(11) NOT NULL default '0',
  `presentation_id` int(11) NOT NULL default '0',
  KEY `user_id` (`user_id`,`presentation_id`),
  KEY `presentation_id` (`presentation_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Table structure for table `pr_templates`
#

DROP TABLE IF EXISTS `pr_templates`;
CREATE TABLE `pr_templates` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `content` text NOT NULL,
  `background` varchar(6) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;
