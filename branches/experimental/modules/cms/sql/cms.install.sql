# phpMyAdmin SQL Dump
# version 2.5.5-pl1
# http://www.phpmyadmin.net
#
# Host: localhost
# Generatie Tijd: 06 Jul 2004 om 13:12
# Server versie: 3.23.58
# PHP Versie: 4.3.6
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_files`
#

DROP TABLE IF EXISTS `cms_files`;
CREATE TABLE `cms_files` (
  `id` int(11) NOT NULL default '0',
  `folder_id` int(11) NOT NULL default '0',
  `extension` varchar(10) NOT NULL default '',
  `size` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `content` longblob NOT NULL,
  `title` varchar(100) NOT NULL default '',
  `description` text NOT NULL,
  `keywords` text NOT NULL,
  `priority` int(11) NOT NULL default '0',
  `hot_item` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_folders`
#

DROP TABLE IF EXISTS `cms_folders`;
CREATE TABLE `cms_folders` (
  `id` int(11) NOT NULL default '0',
  `parent_id` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  `name` char(50) NOT NULL default '',
  `disabled` enum('0','1') NOT NULL default '0',
  `priority` int(11) NOT NULL default '0',
  `disable_multipage` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`),
  KEY `parent_id` (`parent_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_search_files`
#

DROP TABLE IF EXISTS `cms_search_files`;
CREATE TABLE `cms_search_files` (
  `search_word_id` int(11) NOT NULL default '0',
  `file_id` int(11) NOT NULL default '0',
  KEY `file_id` (`file_id`),
  KEY `search_word_id` (`search_word_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_search_words`
#

DROP TABLE IF EXISTS `cms_search_words`;
CREATE TABLE `cms_search_words` (
  `id` int(11) NOT NULL default '0',
  `site_id` int(11) NOT NULL default '0',
  `search_word` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `site_id` (`site_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_settings`
#

DROP TABLE IF EXISTS `cms_settings`;
CREATE TABLE `cms_settings` (
  `user_id` int(11) NOT NULL default '0',
  `sort_field` varchar(20) NOT NULL default '',
  `sort_order` varchar(20) NOT NULL default '',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_sites`
#

DROP TABLE IF EXISTS `cms_sites`;
CREATE TABLE `cms_sites` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `root_folder_id` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `domain` varchar(100) NOT NULL default '',
  `description` text NOT NULL,
  `keywords` text NOT NULL,
  `template_id` int(11) NOT NULL default '0',
  `publish_style` enum('0','1','2') NOT NULL default '0',
  `publish_path` varchar(100) NOT NULL default '',
  `display_type` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_subscribed`
#

DROP TABLE IF EXISTS `cms_subscribed`;
CREATE TABLE `cms_subscribed` (
  `user_id` int(11) NOT NULL default '0',
  `site_id` int(11) NOT NULL default '0',
  KEY `user_id` (`user_id`),
  KEY `site_id` (`site_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_template_files`
#

DROP TABLE IF EXISTS `cms_template_files`;
CREATE TABLE `cms_template_files` (
  `id` int(11) NOT NULL default '0',
  `template_id` int(11) NOT NULL default '0',
  `name` varchar(100) NOT NULL default '',
  `extension` varchar(10) NOT NULL default '',
  `size` int(11) NOT NULL default '0',
  `mtime` int(11) NOT NULL default '0',
  `content` mediumblob NOT NULL,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_template_items`
#

DROP TABLE IF EXISTS `cms_template_items`;
CREATE TABLE `cms_template_items` (
  `id` int(11) NOT NULL default '0',
  `template_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `content` text NOT NULL,
  `main` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `template_id` (`template_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `cms_templates`
#

DROP TABLE IF EXISTS `cms_templates`;
CREATE TABLE `cms_templates` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `root_folder_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `style` text NOT NULL,
  `additional_style` text NOT NULL,
  `restrict_editor` enum('0','1') NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `qn_actions`
#

DROP TABLE IF EXISTS `qn_actions`;
CREATE TABLE `qn_actions` (
  `id` int(11) NOT NULL default '0',
  `questionnaire_id` int(11) NOT NULL default '0',
  `type` varchar(20) NOT NULL default '0',
  `extension` varchar(10) NOT NULL default '',
  `text` text NOT NULL,
  `content` longblob NOT NULL,
  `next_action_id` int(11) NOT NULL default '0',
  `max_length` int(11) NOT NULL default '0',
  `send_to_viewer` enum('0','1') NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `questionnaire_id` (`questionnaire_id`),
  KEY `next_question_id` (`next_action_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `qn_answers`
#

DROP TABLE IF EXISTS `qn_answers`;
CREATE TABLE `qn_answers` (
  `id` int(11) NOT NULL default '0',
  `action_id` int(11) NOT NULL default '0',
  `next_action_id` int(11) NOT NULL default '0',
  `type` varchar(10) NOT NULL default '',
  `text` text NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `question_id` (`action_id`),
  KEY `next_action_id` (`next_action_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `qn_emails`
#

DROP TABLE IF EXISTS `qn_emails`;
CREATE TABLE `qn_emails` (
  `id` int(11) NOT NULL default '0',
  `action_id` int(11) NOT NULL default '0',
  `type` tinyint(4) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `content` longblob NOT NULL,
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `qn_questionnaires`
#

DROP TABLE IF EXISTS `qn_questionnaires`;
CREATE TABLE `qn_questionnaires` (
  `id` int(11) NOT NULL default '0',
  `site_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `start_action_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`site_id`)
) TYPE=MyISAM;
