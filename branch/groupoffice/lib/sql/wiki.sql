# phpMyAdmin SQL Dump
# version 2.5.4
# http://www.phpmyadmin.net
#
# Host: localhost
# Erstellungszeit: 12. Januar 2004 um 22:54
# Server Version: 4.0.16
# PHP-Version: 4.3.3
# 
# Datenbank: `groupoffice`
# 

# --------------------------------------------------------

#
# Tabellenstruktur für Tabelle `wiki_interwiki`
#

DROP TABLE IF EXISTS `wiki_interwiki`;
CREATE TABLE `wiki_interwiki` (
		`prefix` varchar(80) NOT NULL default '',
		`where_defined` varchar(80) NOT NULL default '',
		`url` varchar(255) NOT NULL default '',
		PRIMARY KEY  (`prefix`)
		) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabellenstruktur für Tabelle `wiki_links`
#

DROP TABLE IF EXISTS `wiki_links`;
CREATE TABLE `wiki_links` (
		`page` varchar(80) NOT NULL default '',
		`link` varchar(80) NOT NULL default '',
		`count` int(10) unsigned NOT NULL default '0',
		PRIMARY KEY  (`page`,`link`)
		) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabellenstruktur für Tabelle `wiki_pages`
#

DROP TABLE IF EXISTS `wiki_pages`;
CREATE TABLE `wiki_pages` (
		`title` varchar(80) NOT NULL default '',
		`version` int(10) unsigned NOT NULL default '1',
		`time` timestamp(14) NOT NULL,
		`supercede` timestamp(14) NOT NULL,
		`mutable` set('off','on') NOT NULL default 'on',
		`username` varchar(80) default NULL,
		`author` varchar(80) NOT NULL default '',
		`comment` varchar(80) NOT NULL default '',
		`body` text,
		PRIMARY KEY  (`title`,`version`)
		) TYPE=MyISAM;

INSERT INTO `wiki_pages` VALUES ('RecentChanges', 1, 20040118142744, 20040118142744, 'on', 'administrator', '127.0.0.1', '', '[[! *]]');

# --------------------------------------------------------

#
# Tabellenstruktur für Tabelle `wiki_rate`
#

DROP TABLE IF EXISTS `wiki_rate`;
CREATE TABLE `wiki_rate` (
		`ip` char(20) NOT NULL default '',
		`time` timestamp(14) NOT NULL,
		`viewLimit` smallint(5) unsigned default NULL,
		`searchLimit` smallint(5) unsigned default NULL,
		`editLimit` smallint(5) unsigned default NULL,
		PRIMARY KEY  (`ip`)
		) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabellenstruktur für Tabelle `wiki_remote_pages`
#

DROP TABLE IF EXISTS `wiki_remote_pages`;
CREATE TABLE `wiki_remote_pages` (
		`page` varchar(80) NOT NULL default '',
		`site` varchar(80) NOT NULL default '',
		PRIMARY KEY  (`page`,`site`)
		) TYPE=MyISAM;

# --------------------------------------------------------

#
# Tabellenstruktur für Tabelle `wiki_sisterwiki`
#

DROP TABLE IF EXISTS `wiki_sisterwiki`;
CREATE TABLE `wiki_sisterwiki` (
		`prefix` varchar(80) NOT NULL default '',
		`where_defined` varchar(80) NOT NULL default '',
		`url` varchar(255) NOT NULL default '',
		PRIMARY KEY  (`prefix`)
		) TYPE=MyISAM;

