<?php
/**
* Copyright Intermesh 2004
* Author: Merijn Schering <mschering@intermesh.nl>
* Version: 2.05 Release date: 23 May 2004
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*/


/**
* This class holds the main configuration options of Group-Office
*
* @package  GO_CONFIG
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 1.0
*/

class GO_CONFIG
{
#FRAMEWORK VARIABLES

/**
* Enable debugging mode
*
* @var     bool
* @access  public
*/
  var $debug = false;


/**
* Slash to use '/' for linux and '\\' for windows
*
* @var     string
* @access  public
*/
  var $slash = "/";

/**
* Default language
*
* @var     string
* @access  public
*/
  var $language = "vn";

/**
* Default theme
*
* @var     string
* @access  public
*/
  var $theme = "nuvola";

/**
* Enable theme switching by users
*
* @var     bool
* @access  public
*/
  var $allow_themes = true;

/**
* Enable password changing by users
*
* @var     bool
* @access  public
*/
  var $allow_password_change = true;

/**
* The Group-Office version number
*
* @var     string
* @access  public
*/
  var $version = "2.05";

/**
* Relative hostname with slash on both start and end
*
* @var     string
* @access  public
*/
  var $host = '/~pclouds/opm2/';

/**
* Full URL to reach Group-Office with slash on end
*
* @var     string
* @access  public
*/
  var $full_url = 'http://172.16.32.242/~pclouds/opm2/';

/**
* Title of Group-Office
*
* @var     string
* @access  public
*/
  var $title = "Group-Office";

/**
* The e-mail of the webmaster
*
* @var     string
* @access  public
*/
  var $webmaster_email = "webmaster@example.com";

/**
* The path to the root of Group-Office with slash on end
*
* @var     string
* @access  public
*/
  var $root_path = "/home/pclouds/public_html/opm2/";

/**
* The path to store temporary files with a slash on end
*
* @var     string
* @access  public
*/
  var $tmpdir = "/tmp/";

/**
* The maximum number of users
*
* @var     int
* @access  public
*/
  var $max_users = 0;

/**
* Refresh interval in seconds for the mail & event checker
*
* @var     string
* @access  public
*/
  var $refresh_rate = '60';

/**
* The database type to use. Currently only MySQL is supported
*
* @var     string
* @access  public
*/
  var $db_type = "mysql";
/**
* The host of the database
*
* @var     string
* @access  public
*/
  var $db_host = "localhost";
/**
* The name of the database
*
* @var     string
* @access  public
*/
  var $db_name = "opm2";
/**
* The username to connect to the database
*
* @var     string
* @access  public
*/
  var $db_user = "root";
/**
* The password to connect to the database
*
* @var     string
* @access  public
*/
  var $db_pass = "";

#group configuration
/**
* The administrator user group ID
*
* @var     string
* @access  public
*/
  var $group_root = "1";
/**
* The everyone user group ID
*
* @var     string
* @access  public
*/
  var $group_everyone = "2";


/**
* The path to external authentication sources file
*
* @var     string
* @access  public
*/
  var $auth_sources = '/home/pclouds/public_html/opm2/auth_sources.inc';

#FILE BROWSER VARIABLES

/**
* The path to the mime.types file in Linux
*
* @var     string
* @access  public
*/
  var $mime_types_file = '/etc/mime.types';
/**
* The path to the location where the files of the file browser module are stored
*
* This path should NEVER be inside the document root of the webserver
* this directory should be writable by apache. Also choose a partition that
* has enough diskspace.
*
* @var     string
* @access  public
*/
  var $file_storage_path = "/home/pclouds/opmfiles/";

/**
* The permissions mode to use when creating files and folders
*
* @var     hexadecimal
* @access  public
*/
  var $create_mode = 0755;

#WebDAV VARIABLES

/**
* Enable WebDAV Switch
*
* @var     bool
* @access  public
*/
  var $dav_switch = true;

/**
* Path to Apache DAV library with slash on end
*
* @var     string
* @access  public
*/
  var $dav_apachedir = "/home/pclouds/public_html/opm2/lib/dav/apache/";
/**
* Path to Apache drafts with slash on end
*
* @var     string
* @access  public
*/
  var $dav_drafts = "/home/pclouds/public_html/opm2/lib/dav/drafts/";
  
/**
* DAV authentication source
*
* When it is empty, use the MySQL-Authentication
* For the test: ldap://localhost/dc=tgm,dc=ac,dc=at?uid
*
* @var     string
* @access  public
*/
  var $dav_auth = "";
/**
* The alias as defined in Apache configuration to access the DAV files
* with slash at start and end.
*
* @var     string
* @access  public
*/
  var $dav_alias = "/~pclouds/opm2/dav/";
/**
* The name of the access file (usually .htaccess)
*
* @var     string
* @access  public
*/
  var $dav_accessfilename = ".htaccess";
/**
* The name of the folder to put the symblic links to the Group-Office shares
*
* @var     string
* @access  public
*/
  var $name_of_sharedir = "Shares";

/**
* The maximum file size the filebrowser attempts to upload. Be aware that 
* the php.ini file must be set accordingly (http://www.php.net).
*
* @var     string
* @access  public
*/
  var $max_file_size = "2000000";

/**
* The maximum amount of diskspace that a user may use in Kb 
*
* @var     int
* @access  public
*/
  var $user_quota = 0;	# kb

#EMAIL VARIABLES
/**
* The E-mail mailer type to use. Valid options are: smtp, qmail, sendmail, mail 
*
* @var     int
* @access  public
*/
  var $mailer = 'smtp';
/**
* The SMTP host to use when using the SMTP mailer 
*
* @var     string
* @access  public
*/
  var $smtp_server = "mail.hptvietnam.com.vn";
/**
* The SMTP port to use when using the SMTP mailer 
*
* @var     string
* @access  public
*/
  var $smtp_port = "25";

/**
* Connection string options to append to the hostname when connecting to IMAP
* servers using the PHP imap extension. Some distributions require /notls here.
*
* @var     string
* @access  public
*/
  var $email_connectstring_options = '';
  
/**
* The maximum size of e-mail attachments the browser attempts to upload.
* Be aware that the php.ini file must be set accordingly (http://www.php.net).
*
* @var     string
* @access  public
*/
 var $max_attachment_size = "2000000";

/**
* Image to display at the login window leave blank to disable.
*
* @var     string
* @access  public
*/
  var $login_image = 'lib/GOCOM.gif';

/**
* The width of the E-mail composer's popup window
*
* @var     string
* @access  public
*/
  var $composer_width = '750';
/**
* The height of the E-mail composer's popup window
*
* @var     string
* @access  public
*/
  var $composer_height = '550';


  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////      Do not change underneath this      ///////////////////
  ////////////////////////////////////////////////////////////////////////////////


/**
* Date formats to be used. Changing these will probably break things!
*
* @var     string
* @access  public
*/
  var $date_formats = array(
      'd-m-Y',
      'm-d-Y'
      );
/**
* Time formats to be used.
*
* @var     string
* @access  public
*/
  var $time_formats = array(
      'G:i',
      'g:i a'
      );

/**
* Relative path to the modules directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $module_path = 'modules';
/**
* Relative URL to the administrator directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $administrator_url = 'administrator';
/**
* Relative URL to the configuration directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $configuration_url = 'configuration';
/**
* Relative path to the classes directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $class_path = 'classes';
/**
* Relative path to the controls directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $control_path = 'controls';
/**
* Relative URL to the controls directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $control_url = 'controls';
/**
* Relative path to the themes directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $theme_path = 'themes';
/**
* Relative path to the language directory with no slash at start and end
*
* @var     string
* @access  private
*/
  var $language_path = 'language';
/**
* Relative path to the default icon when displaying an unknown filetype
*
* @var     string
* @access  private
*/
  var $default_filetype_icon = 'lib/icons/default.gif';

/**
* Database object
*
* @var     object
* @access  private
*/
  var $db;

/**
* The window mode of Group-Office
*
* @var     string
* @access  public
*/
  var $window_mode = 'normal';

/**
* Constructor. Initialises all public variables.
*
* @access public
* @return void
*/
  function GO_CONFIG()
  {
    /**
     * Configuration parameters. Thess should be overridden in config.php
     * $GO_CONFIG->debug = false;
     * $GO_CONFIG->slash = "/";
     * $GO_CONFIG->language = "vn";
     * $GO_CONFIG->theme = "nuvola";
     * $GO_CONFIG->allow_themes = true;
     * $GO_CONFIG->allow_password_change = true;
     * $GO_CONFIG->version = "2.05";
     * $GO_CONFIG->host = '/~pclouds/opm2/';
     * $GO_CONFIG->full_url = 'http://172.16.32.242/~pclouds/opm2/';
     * $GO_CONFIG->title = "Group-Office";
     * $GO_CONFIG->webmaster_email = "webmaster@example.com";
     * $GO_CONFIG->root_path = "/home/pclouds/public_html/opm2/";
     * $GO_CONFIG->tmpdir = "/tmp/";
     * $GO_CONFIG->max_users = 0;
     * $GO_CONFIG->refresh_rate = '60';
     * $GO_CONFIG->db_type = "mysql";
     * $GO_CONFIG->db_host = "localhost";
     * $GO_CONFIG->db_name = "opm2";
     * $GO_CONFIG->db_user = "root";
     * $GO_CONFIG->db_pass = "";
     * $GO_CONFIG->group_root = "1";
     * $GO_CONFIG->group_everyone = "2";
     * $GO_CONFIG->auth_sources = '/home/pclouds/public_html/opm2/auth_sources.inc';
     * $GO_CONFIG->mime_types_file = '/etc/mime.types';
     * $GO_CONFIG->file_storage_path = "/home/pclouds/opmfiles/";
     * $GO_CONFIG->create_mode = 0755;
     * $GO_CONFIG->dav_switch = true;
     * $GO_CONFIG->dav_apachedir = "/home/pclouds/public_html/opm2/lib/dav/apache/";
     * $GO_CONFIG->dav_drafts = "/home/pclouds/public_html/opm2/lib/dav/drafts/";
     * $GO_CONFIG->dav_auth = "";
     * $GO_CONFIG->dav_alias = "/~pclouds/opm2/dav/";
     * $GO_CONFIG->dav_accessfilename = ".htaccess";
     * $GO_CONFIG->name_of_sharedir = "Shares";
     * $GO_CONFIG->max_file_size = "2000000";
     * $GO_CONFIG->user_quota = 0;	# kb
     * $GO_CONFIG->mailer = 'smtp';
     * $GO_CONFIG->smtp_server = "mail.hptvietnam.com.vn";
     * $GO_CONFIG->smtp_port = "25";
     * $GO_CONFIG->email_connectstring_options = '';
     * $GO_CONFIG->max_attachment_size = "2000000";
     * $GO_CONFIG->login_image = 'lib/GOCOM.gif';
     * $GO_CONFIG->composer_width = '750';
     * $GO_CONFIG->composer_height = '550';
     * $GO_CONFIG->date_formats = array( 'd-m-Y', 'm-d-Y' );
     * $GO_CONFIG->time_formats = array( 'G:i', 'g:i a' );
     * $GO_CONFIG->module_path = 'modules';
     * $GO_CONFIG->administrator_url = 'administrator';
     * $GO_CONFIG->configuration_url = 'configuration';
     * $GO_CONFIG->class_path = 'classes';
     * $GO_CONFIG->control_path = 'controls';
     * $GO_CONFIG->control_url = 'controls';
     * $GO_CONFIG->theme_path = 'themes';
     * $GO_CONFIG->language_path = 'language';
     * $GO_CONFIG->default_filetype_icon = 'lib/icons/default.gif';
     * $GO_CONFIG->window_mode = 'normal';
     *
     */
    require_once('config.php');
    
#path to classes
    $this->class_path = $this->root_path.$this->class_path.$this->slash;

#path to controls
    $this->control_path = $this->root_path.$this->control_path.$this->slash;

#url to controls
    $this->control_url = $this->host.$this->control_url.$this->slash;

#path to modules
    $this->module_path = $this->root_path.$this->module_path.$this->slash;

#url to administrator apps
    $this->administrator_url = $this->host.$this->administrator_url.$this->slash;

#url to user configuration apps
    $this->configuration_url = $this->host.$this->configuration_url.$this->slash;

#filetype icon
    $this->default_filetype_icon = $this->root_path.$this->default_filetype_icon;

#login image
    if ($this->login_image != '')
    {
      $this->login_image = $this->host.$this->login_image;
    }

    //database class library
    require_once($this->root_path.'database/'.$this->db_type.".class.inc");
    $this->db = new db();
    $this->db->Host = $this->db_host;
    $this->db->Database = $this->db_name;
    $this->db->User = $this->db_user;
    $this->db->Password = $this->db_pass;   
  }

  //
  //see /configuration/preferences/index.php for an example
  
  /**
	* Gets a custom saved setting from the database
	*
	* @param 	string $name Configuration key name
	* @access public
	* @return string Configuration key value
	*/
  function get_setting($name)
  {
    $this->db->query("SELECT * FROM settings WHERE name='$name'");
    if ($this->db->next_record())
    {
      return $this->db->f('value');
    }
    return false;
  }

  /**
	* Saves a custom setting to the database
	*
	* @param 	string $name Configuration key name
	* @param 	string $value Configuration key value
	* @access public
	* @return bool Returns true on succes
	*/
  function save_setting($name, $value)
  {
    if ($this->get_setting($name)===false)
    {
      return $this->db->query("INSERT INTO settings (name, value) VALUES ('$name', '$value')");
    }else
    {
      return $this->db->query("UPDATE settings SET value='$value' WHERE name='$name'");
    }
  }
  /**
	* Deletes a custom setting from the database
	*
	* @param 	string $name Configuration key name
	* @access public
	* @return bool Returns true on succes
	*/
  function delete_setting($name)
  {
    return $this->db->query("DELETE FROM settings WHERE name='$name'");
  }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////       Group-Office initialisation        //////////////////
////////////////////////////////////////////////////////////////////////////////


//load configuration
$GO_CONFIG = new GO_CONFIG();


//setting session save path is required for some server configuration
session_save_path($GO_CONFIG->tmpdir);

if (isset($preload_files)) {
  if (is_array($preload_files)) {
    foreach ($preload_files as $file) {
      include($file);
    }
  } else {
    include($preload_files);
  }
}

// Need to load before Group-Office.php because it needs to be loaded before session_start is called
//require_once('modules/products/classes/products.class.php');

//start session
session_start();

require($GO_CONFIG->class_path.'base/base.security.class.inc');
//require external auth_sources file
if ($GO_CONFIG->auth_sources != '')
{
  require($GO_CONFIG->auth_sources);
}else
{
  $auth_sources = array();
}

$_SESSION['auth_source'] = isset($_SESSION['auth_source']) ? $_SESSION['auth_source'] : '0';
$auth_source = isset($_REQUEST['auth_source_key']) ? $_REQUEST['auth_source_key'] : $_SESSION['auth_source'];

if (isset($auth_sources[$auth_source]))
{
  $_SESSION['auth_source'] = $auth_source;

  if ( ( $auth_sources[$_SESSION['auth_source']]['type'] == "ldap" ) |
      ( $auth_sources[$_SESSION['auth_source']]['user_manager'] == "ldap" ) )
  {
    require_once($GO_CONFIG->root_path.'database/ldap.class.inc');
    $GO_LDAP = new ldap();
  }

  require($GO_CONFIG->class_path.'base/'.$auth_sources[$_SESSION['auth_source']]['type'].'.auth.class.inc');
  require($GO_CONFIG->class_path.'base/'.$auth_sources[$_SESSION['auth_source']]['user_manager'].'.security.class.inc');
  require($GO_CONFIG->class_path.'base/'.$auth_sources[$_SESSION['auth_source']]['user_manager'].'.groups.class.inc');
  require($GO_CONFIG->class_path.'base/'.$auth_sources[$_SESSION['auth_source']]['user_manager'].'.users.class.inc');
}else
{
  require($GO_CONFIG->class_path.'base/sql.auth.class.inc');
  require($GO_CONFIG->class_path.'base/sql.security.class.inc');
  require($GO_CONFIG->class_path.'base/sql.groups.class.inc');
  require($GO_CONFIG->class_path.'base/sql.users.class.inc');	
}
require($GO_CONFIG->class_path.'base/modules.class.inc');
require($GO_CONFIG->class_path.'base/controls.class.inc');
require($GO_CONFIG->root_path.'functions.inc');
require($GO_CONFIG->class_path."base/language.class.inc");
require($GO_CONFIG->class_path.'base/theme.class.inc');

$GO_LANGUAGE = new GO_LANGUAGE();
$GO_THEME = new GO_THEME();
$GO_MODULES = new GO_MODULES();
$GO_AUTH = new GO_AUTH();
$GO_SECURITY = new GO_SECURITY();
$GO_USERS = new GO_USERS();
$GO_GROUPS = new GO_GROUPS();

if ($GO_CONFIG->dav_switch)
{
  require($GO_CONFIG->class_path.'dav.class.inc');
  $GO_DAV = new dav();
}

if (isset($_REQUEST['SET_LANGUAGE']))
{
  $GO_LANGUAGE->set_language($_REQUEST['SET_LANGUAGE']);
}
require($GO_LANGUAGE->get_base_language_file('common'));
define('GO_LOADED', true);
?>
