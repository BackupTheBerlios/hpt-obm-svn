# phpMyAdmin SQL Dump
# version 2.5.5-pl1
# http://www.phpmyadmin.net
#
# Host: localhost
# Generatie Tijd: 19 Apr 2004 om 10:45
# Server versie: 3.23.58
# PHP Versie: 4.3.4
# 
# Database : `groupoffice`
# 

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_addressbooks`
#

DROP TABLE IF EXISTS `ab_addressbooks`;
CREATE TABLE `ab_addressbooks` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `user_id` (`user_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_companies`
#

DROP TABLE IF EXISTS `ab_companies`;
CREATE TABLE `ab_companies` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
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
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `addressbook_id` (`addressbook_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_contacts`
#

DROP TABLE IF EXISTS `ab_contacts`;
CREATE TABLE `ab_contacts` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `addressbook_id` int(11) NOT NULL default '0',
  `source_id` int(11) NOT NULL default '0',
  `group_id` int(11) NOT NULL default '0',
  `first_name` varchar(50) NOT NULL default '',
  `middle_name` varchar(50) NOT NULL default '',
  `last_name` varchar(50) NOT NULL default '',
  `initials` varchar(10) NOT NULL default '',
  `title` varchar(10) NOT NULL default '',
  `sex` enum('M','F') NOT NULL default 'M',
  `birthday` date NOT NULL default '0000-00-00',
  `email` varchar(100) NOT NULL default '',
  `company_id` int(11) NOT NULL default '0',
  `department` varchar(50) NOT NULL default '',
  `function` varchar(50) NOT NULL default '',
  `home_phone` varchar(20) NOT NULL default '',
  `work_phone` varchar(20) NOT NULL default '',
  `fax` varchar(20) NOT NULL default '',
  `work_fax` varchar(20) NOT NULL default '',
  `cellular` varchar(20) NOT NULL default '',
  `country` varchar(50) NOT NULL default '',
  `state` varchar(50) NOT NULL default '',
  `city` varchar(50) NOT NULL default '',
  `zip` varchar(10) NOT NULL default '',
  `address` varchar(100) NOT NULL default '',
  `comment` varchar(50) NOT NULL default '',
  `color` varchar(6) NOT NULL default '',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`),
  KEY `user_id` (`addressbook_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_custom_company_fields`
#

DROP TABLE IF EXISTS `ab_custom_company_fields`;
CREATE TABLE `ab_custom_company_fields` (
  `id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_custom_company_fields_cats`
#

DROP TABLE IF EXISTS `ab_custom_company_fields_cats`;
CREATE TABLE `ab_custom_company_fields_cats` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `acl_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_custom_company_fields_sort`
#

DROP TABLE IF EXISTS `ab_custom_company_fields_sort`;
CREATE TABLE `ab_custom_company_fields_sort` (
  `catagory_id` int(11) NOT NULL default '0',
  `field` varchar(50) NOT NULL default '',
  `size` tinyint(4) NOT NULL default '0',
  `sort_index` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`field`),
  KEY `catagory_id` (`catagory_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_custom_contact_fields`
#

DROP TABLE IF EXISTS `ab_custom_contact_fields`;
CREATE TABLE `ab_custom_contact_fields` (
  `id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_custom_contact_fields_cats`
#

DROP TABLE IF EXISTS `ab_custom_contact_fields_cats`;
CREATE TABLE `ab_custom_contact_fields_cats` (
  `id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `acl_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_custom_contact_fields_sort`
#

DROP TABLE IF EXISTS `ab_custom_contact_fields_sort`;
CREATE TABLE `ab_custom_contact_fields_sort` (
  `catagory_id` int(11) NOT NULL default '0',
  `field` varchar(50) NOT NULL default '',
  `size` tinyint(4) NOT NULL default '0',
  `sort_index` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`field`),
  KEY `sort_index` (`sort_index`),
  KEY `catagory_id` (`catagory_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_groups`
#

DROP TABLE IF EXISTS `ab_groups`;
CREATE TABLE `ab_groups` (
  `id` int(11) NOT NULL default '0',
  `addressbook_id` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`),
  KEY `id` (`id`),
  KEY `user_id` (`addressbook_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_settings`
#

DROP TABLE IF EXISTS `ab_settings`;
CREATE TABLE `ab_settings` (
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
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `ab_subscribed`
#

DROP TABLE IF EXISTS `ab_subscribed`;
CREATE TABLE `ab_subscribed` (
  `user_id` int(11) NOT NULL default '0',
  `addressbook_id` int(11) NOT NULL default '0',
  `standard` enum('0','1') NOT NULL default '0',
  KEY `user_id` (`user_id`,`addressbook_id`,`standard`),
  KEY `addressbook_id` (`addressbook_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `tp_mailing_companies`
#

DROP TABLE IF EXISTS `tp_mailing_companies`;
CREATE TABLE `tp_mailing_companies` (
  `group_id` int(11) NOT NULL default '0',
  `company_id` int(11) NOT NULL default '0',
  KEY `group_id` (`group_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `tp_mailing_contacts`
#

DROP TABLE IF EXISTS `tp_mailing_contacts`;
CREATE TABLE `tp_mailing_contacts` (
  `group_id` int(11) NOT NULL default '0',
  `contact_id` int(11) NOT NULL default '0',
  KEY `group_id` (`group_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `tp_mailing_groups`
#

DROP TABLE IF EXISTS `tp_mailing_groups`;
CREATE TABLE `tp_mailing_groups` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `tp_subscribed`
#

DROP TABLE IF EXISTS `tp_subscribed`;
CREATE TABLE `tp_subscribed` (
  `user_id` int(11) NOT NULL default '0',
  `template_id` int(11) NOT NULL default '0',
  KEY `user_id` (`user_id`,`template_id`)
) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabel structuur voor tabel `tp_templates`
#

DROP TABLE IF EXISTS `tp_templates`;
CREATE TABLE `tp_templates` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `type` tinyint(4) NOT NULL default '0',
  `name` varchar(50) NOT NULL default '',
  `content` longblob NOT NULL,
  `acl_read` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;
