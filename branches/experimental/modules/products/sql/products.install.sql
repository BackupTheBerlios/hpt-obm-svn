DROP TABLE IF EXISTS sc_attach_categories;
CREATE TABLE sc_attach_categories (
  id int(11) unsigned NOT NULL auto_increment,
  name varchar(30) NOT NULL default '',
  PRIMARY KEY  (id)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_attachments;
CREATE TABLE sc_attachments (
  product_id int(11) unsigned NOT NULL default '0',
  attach_cate_id int(11) unsigned NOT NULL default '0',
  attach_id int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (product_id,attach_cate_id,attach_id)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_cate_products;
CREATE TABLE sc_cate_products (
  product_id int(11) unsigned NOT NULL default '0',
  category_id int(11) unsigned NOT NULL default '0',
  be_attachment tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (product_id,category_id,be_attachment)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_categories;
CREATE TABLE sc_categories (
  category_id int(11) unsigned NOT NULL auto_increment,
  category_name varchar(30) NOT NULL default '',
  parent_id int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (category_id)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_order_detail;
CREATE TABLE sc_order_detail (
  order_number varchar(30) NOT NULL default '0',
  product_id int(11) unsigned NOT NULL default '0',
  attach_cate_id int(11) unsigned NOT NULL default '0',
  attach_id int(11) unsigned NOT NULL default '0',
  quantity int(11) unsigned NOT NULL default '0',
  price double NOT NULL default '0',
  discount int(11) unsigned default NULL,
  PRIMARY KEY  (order_number,product_id,attach_cate_id,attach_id)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_orders;
CREATE TABLE sc_orders (
  order_number varchar(30) NOT NULL default '0',
  phone varchar(30) default NULL,
  sale_date date default NULL,
  fax varchar(30) NOT NULL default '0',
  company varchar(100) NOT NULL default '',
  attn varchar(50) NOT NULL default '',
  valid_date date NOT NULL default '0000-00-00',
  seller int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (order_number)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_products;
CREATE TABLE sc_products (
  product_id int(11) unsigned NOT NULL auto_increment,
  product_name varchar(30) default NULL,
  description text,
  price double NOT NULL default '0',
  part_number varchar(30) default NULL,
  VAT double default NULL,
  warranty_info text NOT NULL,
  PRIMARY KEY  (product_id)
) TYPE=MyISAM;

DROP TABLE IF EXISTS sc_templates;
CREATE TABLE sc_templates (
  category_id int(11) unsigned NOT NULL default '0',
  attach_cate_id int(11) unsigned NOT NULL default '0',
  PRIMARY KEY  (category_id,attach_cate_id)
) TYPE=MyISAM;
