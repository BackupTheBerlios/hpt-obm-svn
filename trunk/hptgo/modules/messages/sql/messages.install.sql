DROP TABLE IF EXISTS `messages_messages`;
CREATE TABLE `messages_messages` (
  `id` int(11) NOT NULL default '0',
  `user_id` int(11) NOT NULL default '0',
  `sender_id` int(11) NOT NULL default '0',
  `ctime` int(11) NOT NULL default '0',
  `text` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

DROP TABLE IF EXISTS `messages_new`;
CREATE TABLE `messages_new` (
  `id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) TYPE=MyISAM;

DROP TABLE IF EXISTS `messages_users`;
CREATE TABLE `messages_users` (
  `user_id` int(11) NOT NULL default '0',
  `acl_write` int(11) NOT NULL default '0',
  PRIMARY KEY  (`user_id`)
) TYPE=MyISAM;
