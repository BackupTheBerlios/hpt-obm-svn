<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This function creates a Group-Office.php file from the template Group-Office.tpl and fills it with values from a $GO_CONFIG object
 */

function save_config($config)
{
  if ( !$fp = fopen($config->root_path.'Group-Office.tpl', 'r') ) {
    exit( "Failed to open config template" );
  }
  $config_data = fread($fp, filesize($config->root_path.'Group-Office.tpl'));
  if (strlen( $config_data ) == 0 ) {
    exit( "Failed to read from config template" );
  }
  fclose($fp);

  $config_data = str_replace('%title%', smartstrip($config->title), $config_data);
  $config_data = str_replace('%slash%', $config->slash, $config_data);
  $config_data = str_replace('%host%', $config->host, $config_data);
  $config_data = str_replace('%full_url%', $config->full_url, $config_data);
  $config_data = str_replace('%root_path%', smartstrip($config->root_path), $config_data);
  $config_data = str_replace('%language%', $config->language, $config_data);
  $first_weekday = (isset($config->first_weekday) && $config->first_weekday != '') ? $config->first_weekday : '0';
  $config_data = str_replace('%first_weekday%', $first_weekday, $config_data);
  $config_data = str_replace('%tmpdir%', $config->tmpdir, $config_data);
  $config_data = str_replace('%theme%', $config->theme, $config_data);

  $allow_themes = ($config->allow_themes === true) ? 'true' : 'false';
  $config_data = str_replace('%allow_themes%', $allow_themes, $config_data);

  $allow_password_change = ($config->allow_password_change === true) ? 'true' : 'false';
  $config_data = str_replace('%allow_password_change%', $allow_password_change, $config_data);

  $config_data = str_replace('%mailer%', $config->mailer, $config_data);
  $config_data = str_replace('%smtp_server%', $config->smtp_server, $config_data);
  $config_data = str_replace('%smtp_port%', $config->smtp_port, $config_data);
  $config_data = str_replace('%max_attachment_size%', $config->max_attachment_size, $config_data);

  $config_data = str_replace('%file_storage_path%', $config->file_storage_path, $config_data);
  $config_data = str_replace('%email_connectstring_options%', $config->email_connectstring_options, $config_data);

  if (!is_string($config->create_mode))
  {
    $config->create_mode = decoct((string)$config->create_mode);
  }
  if (strlen($config->create_mode) == 3)
  {
    $config->create_mode = '0'.$config->create_mode;
  }
  $config_data = str_replace('%create_mode%', $config->create_mode, $config_data);
  $config_data = str_replace('%max_file_size%', $config->max_file_size, $config_data);
  $config_data = str_replace('%webmaster_email%', $config->webmaster_email, $config_data);
  $config_data = str_replace('%db_type%', $config->db_type, $config_data);
  $config_data = str_replace('%db_host%', $config->db_host, $config_data);
  $config_data = str_replace('%db_name%', $config->db_name, $config_data);
  $config_data = str_replace('%db_user%', $config->db_user, $config_data);
  $config_data = str_replace('%db_pass%', $config->db_pass, $config_data);
  $login_image = str_replace($config->host, '', $config->login_image);
  $config_data = str_replace('%login_image%', $login_image, $config_data);
  $config_data = str_replace('%composer_width%', $config->composer_width, $config_data);
  $config_data = str_replace('%composer_height%', $config->composer_height, $config_data);
  $config_data = str_replace('%refresh_rate%', $config->refresh_rate, $config_data);
  $config_data = str_replace('%max_users%', $config->max_users, $config_data);
  $config_data = str_replace('%mime_types_file%', $config->mime_types_file, $config_data);
  $config_data = str_replace('%auth_sources%', $config->auth_sources, $config_data);

  if (!$fp = fopen($config->root_path.'Group-Office.php', 'w+'))
  {
    exit("Failed to open config file");
  }elseif(!fwrite($fp, $config_data))
  {
    exit("Failed to write to config file");
  }else
  {
    return fclose($fp);
  }
}

function print_head()
{
  header('Content-Type: text/html; charset='.$charset);
  echo '<html><head><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /><title>Cài đặt HPT Open Business Management</title></head><body style="font-family: Arial,Helvetica">';
  echo '<form method="post" action="install.php">';
  echo '<table align="center" style="background: #f1f1f1;border-width: 1px;border-color: black;border-style: solid;font-family: Arial,Helvetica; font-size: 12px; width: 500px;">';
  echo '<tr><td align="center"><h2><img src="lib/hpt-obm.gif" border="0" /><br />Cài đặt HPT Open Business Management</h2></td></tr>';
  echo '<tr><td><table style="border-width: 0px;padding: 20px;font-family: Arial,Helvetica; width: 500px;font-weight: normal; font-size: 12px;"><tr><td>';
}

function print_foot()
{
  echo '</td></tr></table></td></tr></table></form></body></html>';
}

//destroy session when user closes browser
ini_set('session.cookie_lifetime','0');


//get the path of this script
if (isset($_SERVER['SCRIPT_FILENAME']) && $_SERVER['SCRIPT_FILENAME'] != '')
{
  $script_path = stripslashes($_SERVER['SCRIPT_FILENAME']);
}else
{
  $script_path = stripslashes(str_replace('\\','/',$_SERVER['PATH_TRANSLATED']));
}
if ($script_path == '')
{
  print_head();
  echo '<b>Fatal error:</b> Could not get the path of the this script. The server variable \'SCRIPT_FILENAME\' and \'PATH_TRANSLATED\' are not set.';
  echo '<br /><br />Hãy điều chỉnh và nhấn Refresh để cập nhật lại trang này. If you are not able to correct this try the manual installation described in the file \'INSTALL\'';
  print_foot();
  exit();
}

//the root_path of Group-Office is the path of this script without install.php
$root_path = str_replace('install.php', '',$script_path);

//check ifconfig exists and if the config file is writable
$config_exists = file_exists('Group-Office.php');
if ($config_exists && !is_writable('Group-Office.php'))
{
  print_head();
  echo 'Tập tin cấu hình Group-Office.php đã có tại '.$root_path.' nhưng không thể hiệu chỉnh. Nếu bạn muốn thay đổi cấu hình, bạn phải cho phép ghi vào Group-Office.php trong suốt tiến trình cài đặt.';
  echo '<br /><br />Hãy điều chỉnh và nhấn Refresh để cập nhật trang này.';
  echo '<br /><br />Với Linux, thực hiện lệnh: <br/><font color="#003399"><i>$ chmod 777 '.$root_path.'Group-Office.php<br /></i></font>';
  print_foot();
  exit();
}elseif(!$config_exists && !is_writable($root_path))
{
  print_head();
  echo 'Không thể tạo tập tin cấu hình trong '.$root_path.'. Nếu bạn muốn cài đặt HPT-OBM bạn phải tạm thời cho phép ghi vào '.$root_path.'.';
  echo '<br /><br />Hãy điều chỉnh và nhấn Refresh để cập nhật lại trang này.';
  echo '<br /><br />Với Linux, thực hiện lệnh: <br/><font color="#003399"><i>$ chmod 777 '.$root_path.'<br /></i></font>';
  print_foot();
  exit();
}elseif(!function_exists('mysql_connect'))
{
  print_head();
  echo '<b>Lỗi nghiêm trọng:</b> Phần mở rộng PHP MySQL chưa được cài đặt.'.
    'Vui lòng xem <a href="http://www.php.net" target="_blank">'.
    'http://www.php.net</a> để biết thêm thông tin chi tiết cách cài đặt phần mở rộng này.';

  print_foot();
  exit();

}


//if we can write and config file doesn't exist create a default config file
if (!$config_exists)
{
  $tpl_file = str_replace('install.php', 'Group-Office.tpl',$script_path);
  $GO_CONFIG_file = str_replace('install.php', 'Group-Office.php',$script_path);
  if ( !$fp = fopen($tpl_file, 'r') ) {
    exit( "Failed to open config template" );
  }
  $config_data = fread($fp, filesize($tpl_file));
  if (strlen( $config_data ) == 0 ) {
    exit( "Failed to read from config template" );
  }
  fclose($fp);

  $config_data = str_replace('%title%', 'HPT Open Business Management', $config_data);
  $config_data = str_replace('%slash%', '/', $config_data);

  $host= str_replace('//','/',substr(str_replace(str_replace('\\','/',$_SERVER['DOCUMENT_ROOT']),"",$root_path),0,-1));
  $full_url = 'http://'.$_SERVER['HTTP_HOST'].$host;

  $config_data = str_replace('%host%', $host, $config_data);
  $config_data = str_replace('%full_url%', $full_url, $config_data);
  $config_data = str_replace('%root_path%', addslashes($root_path), $config_data);
  $config_data = str_replace('%language%', 'vn', $config_data);
  $config_data = str_replace('%first_weekday%', '1', $config_data);
  $config_data = str_replace('%tmpdir%', (getenv('TEMP') ? getenv('TEMP') : '/tmp/'), $config_data);
  $config_data = str_replace('%theme%', 'crystal', $config_data);
  $config_data = str_replace('%allow_themes%', 'true', $config_data);
  $config_data = str_replace('%allow_password_change%', 'true', $config_data);
  $config_data = str_replace('%mailer%', 'mail', $config_data);  
  $config_data = str_replace('%smtp_server%', '', $config_data);
  $config_data = str_replace('%smtp_port%', '', $config_data);
  $config_data = str_replace('%max_attachment_size%', '2000000', $config_data);
  $config_data = str_replace('%email_connectstring_options%', '', $config_data);
  $config_data = str_replace('%file_storage_path%', '/home/groupoffice', $config_data);
  $config_data = str_replace('%create_mode%', '0755', $config_data);
  $config_data = str_replace('%max_file_size%', '2000000', $config_data);
  $config_data = str_replace('%webmaster_email%', 'webmaster@example.com', $config_data);
  $config_data = str_replace('%db_type%', 'mysql', $config_data);
  $config_data = str_replace('%db_host%', 'localhost', $config_data);
  $config_data = str_replace('%db_name%', 'groupoffice', $config_data);
  $config_data = str_replace('%db_user%', 'groupoffice', $config_data);
  $config_data = str_replace('%db_pass%', '', $config_data);
  $config_data = str_replace('%login_image%', 'lib/GOCOM.gif', $config_data);
  $config_data = str_replace('%composer_width%', '750', $config_data);
  $config_data = str_replace('%composer_height%', '550', $config_data);
  $config_data = str_replace('%refresh_rate%', '60', $config_data);
  $config_data = str_replace('%max_users%', '0', $config_data);
  $config_data = str_replace('%mime_types_file%', '/etc/mime.types', $config_data);
  $config_data = str_replace('%auth_sources%', $root_path.'auth_sources.inc', $config_data);

  if (!$fp = fopen($GO_CONFIG_file, 'w+'))
  {
    exit("Failed to open config file");
  }elseif(!fwrite($fp, $config_data))
  {
    exit("Failed to write to config file");
  }else
  {
    fclose($fp);
  }
}

//config file exists now so require it to get the properies.
require('Group-Office.php');

if ($_SERVER['REQUEST_METHOD'] =='POST')
{
  switch($_POST['task'])
  {
    case 'administrator':
      $pass1=trim($_POST['pass1']);
      $pass2=trim($_POST['pass2']);
      $email=trim($_POST['email']);
      $username=trim($_POST['username']);


      if ($pass1 == '' || $username=='')
      {
	$feedback = '<font color="red">Vui lòng nhập mật khẩu và tên người dùng.</font>';
      }elseif( strlen($pass1) < 4)
      {
	$feedback = '<font color="red">Mật khẩu không thể ngắn hơn 4 ký tự!</font>';
      }elseif($pass1 != $pass2)
      {
	$feedback = '<font color="red">Mật khẩu không giống nhau!</font>';
      }elseif(!eregi("^([a-z0-9]+)([._-]([a-z0-9]+))*[@]([a-z0-9]+)([._-]([a-z0-9]+))*[.]([a-z0-9]){2}([a-z0-9])?$", $email))
      {
	$feedback = '<font color="red">Địa chỉ E-mail không hợp lệ!</font>';
      }else
      {
	$GO_USERS->get_users();
	$new_user_id = $GO_USERS->nextid("users");

	$GO_GROUPS->query("DELETE FROM db_sequence WHERE seq_name='groups'");
	$GO_GROUPS->query("DELETE FROM groups");

	$admin_group_id = $GO_GROUPS->add_group($new_user_id, 'Admins');
	$root_group_id = $GO_GROUPS->add_group($new_user_id, 'Everyone');

	$new_user_id = $GO_USERS->add_user(
	    smart_addslashes($username),	// Username
	    smart_addslashes($pass1),			// Password
	    '',	// First Name
	    '',			// Middle Name
	    '',			// Last Name
	    '',			// Initials
	    '',			// Title
	    'M',			// Sex
	    '',			// Birthday
	    smart_addslashes($email),			// eMail Address
	    '',			// Work Phone
	    '',			// Home Phone
	    '',			// Fax
	    '',			// Cellular
	    '',			// Country
	    '',			// State
	    '',			// City
	    '',			// ZIP
	    '',			// Address
	    '',			// Company
	    '',			// Work Country
	    '',			// Work State
	    '',			// Work City
	    '',			// Work ZIP
	    '',			// Work Address
	    '',			// Work Fax
	    '',			// Homepage
	    '',			// Department
	    '',			// Function
	    '',			// Language
	    '',			// Theme
	    true,		// Visible to everybody?      
	    $new_user_id
	      );
	    $old_umask = umask(000);
	    mkdir($GO_CONFIG->file_storage_path.'users', $GO_CONFIG->create_mode);
	    mkdir($GO_CONFIG->file_storage_path.'common', $GO_CONFIG->create_mode);
	    mkdir($GO_CONFIG->file_storage_path.'users/'.smartstrip($username), $GO_CONFIG->create_mode);
	    umask($old_umask);

	    //grant administrator privileges
	    $GO_GROUPS->add_user_to_group($new_user_id, $GO_CONFIG->group_root);    

	    $_SESSION['completed']['administrator'] = true;
      }
      break;

    case 'license':
      $_SESSION['completed']['license'] = true;
      break;

    case 'release_notes':
      $_SESSION['completed']['release_notes'] = true;
      break;

    case 'database_connection':
      $db = new db();
      $db->Halt_On_Error = 'no';
      if(@$db->connect($_POST['db_name'], $_POST['db_host'], $_POST['db_user'], $_POST['db_pass']))
      {
	$GO_CONFIG->db_host = $_POST['db_host'];
	$GO_CONFIG->db_name = $_POST['db_name'];
	$GO_CONFIG->db_user = $_POST['db_user'];
	$GO_CONFIG->db_pass = $_POST['db_pass'];
	if (save_config($GO_CONFIG))
	{
	  $_SESSION['completed']['database_connection'] = true;
	}

      }else
      {
	$feedback ='<font color="red">Lỗi kết nối với cơ sở dữ liệu</font>';
      }
      break;

    case 'database_structure':
      $db = new db();
      $db->Halt_On_Error = 'no';
      if (!$db->connect($GO_CONFIG->db_name, $GO_CONFIG->db_host, $GO_CONFIG->db_user, $GO_CONFIG->db_pass))
      {
	print_head();
	echo 'Không thể kết nối với cơ sở dữ liệu!';
	echo '<br /><br />Hãy điều chỉnh và nhấn Refresh để cập nhật lại trang này.';
	print_foot();
	exit();
      }else
      {
	if($_POST['upgrade'] == 'true')
	{
	  $old_version = intval(str_replace('.', '', $_POST['db_version']));
	  $new_version = intval(str_replace('.', '', $GO_CONFIG->version));
	  require('lib/updates.inc');
	  if (!isset($updates[$old_version]))
	  {
	    //invalid version, abort upgrade
	    $feedback = '<font color="red">Số phiên bản bạn nhập không hợp lệ</font>';
	  }else
	  {
	    for ($cur_ver=$old_version;$cur_ver<$new_version;$cur_ver++)
	    {
	      if (isset($updates[$cur_ver]))
	      {
		while($query = array_shift($updates[$cur_ver]))
		{
		  //echo $query.'<br />';
		  @$db->query($query);
		}
	      }

	      if (file_exists($GO_CONFIG->root_path.'lib/scripts/'.$cur_ver.'.inc'))
	      {
		echo 'Đang chạy chương trình cập nhật cho phiên bản '.$cur_ver.'...<br>';
		require_once($GO_CONFIG->root_path.'lib/scripts/'.$cur_ver.'.inc');
	      }
	    }
	    $db_version = $GO_CONFIG->version;
	    $_SESSION['completed']['database_structure'] = true;
	    //store the version number for future upgrades
	    $GO_CONFIG->save_setting('version', $GO_CONFIG->version);
	    //Upgrade completed
	  }
	}else
	{
	  if(!isset($_POST['fresh_install']))
	  {
	    //delete all existing users
	    $delete=new GO_USERS();
	    $delete->Halt_On_Error = 'no';

	    $GO_USERS->get_users();
	    while($GO_USERS->next_record())
	    {
	      $delete->delete_user($GO_USERS->f('id'));
	    }

	    $GO_MODULES->get_modules();

	    $delete_module = new GO_MODULES();
	    $delete_module->Halt_On_Error = 'no';
	    while($GO_MODULES->next_record())
	    {
	      $delete_module->delete_module($GO_MODULES->f('id'));
	    }
	  }
	  //create new empty database
	  //table is empty create the structure
	  $queries = get_sql_queries("lib/sql/groupoffice.sql");
	  while ($query = array_shift($queries))
	  {
	    $db->query($query);
	  }
	  $queries = get_sql_queries("lib/sql/filetypes.sql");
	  while ($query = array_shift($queries))
	  {
	    $db->query($query);
	  }
	  //store the version number for future upgrades
	  $GO_CONFIG->save_setting('version', $GO_CONFIG->version);
	  $db_version = $GO_CONFIG->version;
	  $_SESSION['completed']['database_structure'] = true;
	}
      }
      break;

    case 'userdir':
      if (!is_writable($_POST['userdir']))
      {
	$feedback = '<font color="red">Đường dẫn bạn nhập không cho phép ghi.<br />Hãy điều chỉnh và thử lại lần nữa.</font>';
      }else
      {
	if (substr($_POST['userdir'], -1) != '/') $_POST['userdir'] = $_POST['userdir'].'/';
	$GO_CONFIG->file_storage_path=$_POST['userdir'];
	$GO_CONFIG->create_mode=$_POST['create_mode'];
	$GO_CONFIG->max_file_size=$_POST['max_file_size'];

	if (save_config($GO_CONFIG))
	{
	  $_SESSION['completed']['userdir'] = true;
	}
      }

      break;

    case 'tmpdir':
      $tmpdir=$_POST['tmpdir'];
      if (!is_writable($tmpdir))
      {
	$feedback = '<font color="red">Đường dẫn bạn nhập không cho phép ghi.<br />Hãy điều chỉnh và thử lại lần nữa.</font>';
      }else
      {
	if (substr($tmpdir, -1) != '/') $tmpdir = $tmpdir.'/';
	$GO_CONFIG->tmpdir=$tmpdir;
	if (save_config($GO_CONFIG))
	{
	  $_SESSION['completed']['tmpdir'] = true;
	}
      }

      break;

    case 'title':
      if ($_POST['title'] == '')
      {
	$feedback = 'Bạn chưa nhập tiêu đề.';

      }elseif(!eregi("^([a-z0-9]+)([._-]([a-z0-9]+))*[@]([a-z0-9]+)([._-]([a-z0-9]+))*[.]([a-z0-9]){2}([a-z0-9])?$", $_POST['webmaster_email']))
      {
	$feedback = 'Bạn đã nhập địa chỉ e-mail không hợp lệ.';
      }else
      {
	$GO_CONFIG->webmaster_email = $_POST['webmaster_email'];
	$GO_CONFIG->title = $_POST['title'];
	if (save_config($GO_CONFIG))
	{
	  $_SESSION['completed']['title'] = true;
	}
      }
      break;

    case 'url':
      $host = smartstrip(trim($_POST['host']));
      $full_url = smartstrip(trim($_POST['full_url']));
      if ($host != '' && $full_url != '')
      {
	if ($host != '/')
	{
	  if (substr($host , -1) != '/') $host  = $host.'/';
	  if (substr($host , 0, 1) != '/') $host  = '/'.$host;
	}

	if(substr($full_url,-1) != '/') $full_url = $full_url.'/';

	$GO_CONFIG->login_image = str_replace($GO_CONFIG->host, $host, $GO_CONFIG->login_image);

	$GO_CONFIG->host = $host;
	$GO_CONFIG->full_url = $full_url;
	if (save_config($GO_CONFIG))
	{
	  $_SESSION['completed']['url'] = true;
	}

      }else
      {
	$feedback = '<font color="red">Bạn chưa nhập cả hai.</font>';
      }
      break;

    case 'theme':
      $GO_CONFIG->theme = $_POST['theme'];
      $GO_CONFIG->allow_themes = ($_POST['allow_themes'] == 'true') ? true : false;
      if (save_config($GO_CONFIG))
      {
	$_SESSION['completed']['theme'] = true;
      }
      break;

      case 'allow_password_change';
      $GO_CONFIG->allow_password_change = ($_POST['allow_password_change'] == 'true') ? true : false;
      if (save_config($GO_CONFIG))
      {
	$_SESSION['completed']['allow_password_change'] = true;
      }

      break;

    case 'language':

      $GO_CONFIG->language = $_POST['language'];
      if (save_config($GO_CONFIG))
      {
	$_SESSION['completed']['language'] = true;
      }
      break;

    case 'smtp':
      $GO_CONFIG->mailer = $_POST['mailer'];
      $GO_CONFIG->smtp_port = isset($_POST['smtp_port']) ? trim($_POST['smtp_port']) : '';
      $GO_CONFIG->smtp_server= isset($_POST['smtp_server']) ? trim($_POST['smtp_server']) : '';
      $GO_CONFIG->max_attachment_size= trim($_POST['max_attachment_size']);
      $GO_CONFIG->email_connectstring_options = trim($_POST['email_connectstring_options']);
      if (save_config($GO_CONFIG))
      {
	$_SESSION['completed']['smtp'] = true;
      }
      break;

  }
}else
{
  unset($_SESSION);
}

//Store all options in config array during install
$_SESSION['completed'] = isset($_SESSION['completed']) ? $_SESSION['completed'] : array();

if (!isset($_SESSION['completed']['license']))
{
  if(file_exists('LICENSE.GPL'))
  {
    $license = 'LICENSE.GPL';
  }else
  {
    $license = 'LICENSE.PRO';
  }

  print_head();
  echo '<input type="hidden" name="task" value="license" />';
  echo 'Bạn có đồng ý với những điều khoản được nêu trong giấy phép này hay không?<br /><br />';
  echo '<iframe style="width: 670; height: 250; background: #ffffff;" src="'.$license.'"></iframe>';
  echo '<br /><br /><div align="center"><input type="submit" value="Tôi đồng ý với những điều khoản này" /></div>';
  print_foot();
  exit();
}


if (!isset($_SESSION['completed']['release_notes']))
{
  print_head();
  echo '<input type="hidden" name="task" value="release_notes" />';
  echo 'Vui lòng đọc thông tin về chương trình<br /><br />';
  echo '<iframe style="width: 670; height: 250; background: #ffffff;" src="RELEASE"></iframe>';
  echo '<br /><br /><div align="center"><input type="submit" value="Tiếp tục" /></div>';
  print_foot();
  exit();
}


//Get the database parameters first
//if option database_connection is set then we have succesfully set up database
if (!isset($_SESSION['completed']['database_connection']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="database_connection" />
    Hãy tạo cơ sở dữ liệu và điền thông tin vào bên dưới để kết nối với cơ sở dữ liệu.<br />
    Người dùng cơ sở dữ liệu cần có quyền thực hiện các câu lệnh select, insert, update, delete và được quyền khoá bảng.

    <br /><br />

    <font color="#003399"><i>
    $ mysql -u root -p<br />
    $ mysql&#62; CREATE DATABASE groupoffice;<br />
    <table width="100%" border="0">
    $ mysql&#62; GRANT ALL PRIVILEGES ON groupoffice.* TO groupoffice@localhost<br />
    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&#62; IDENTIFIED BY "mật khẩu của bạn" WITH GRANT OPTION;<br />
    </i></font>

    <br /><br />
    <table width="100%" style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>
    Máy chủ:
    </td>
    <td>
    <input type="text" size="40" name="db_host" value="<?php echo $GO_CONFIG->db_host; ?>" />
    </td>
    </tr>
    <tr>
    <td>
    Tên CSDL:
    </td>
    <td>
    <input type="text" size="40" name="db_name" value="<?php echo $GO_CONFIG->db_name; ?>" />
    </td>
    </tr>

    <tr>
    <td>
    Tên người dùng:
    </td>
    <td>
    <input type="text" size="40" name="db_user" value="<?php echo $GO_CONFIG->db_user; ?>"  />
    </td>
    </tr>
    <tr>
    <td>
    Mật khẩu:
    </td>
    <td>
    <input type="password" size="40" name="db_pass" value="<?php echo $GO_CONFIG->db_pass; ?>"  />
    </td>
    </tr>
    <tr>
    <td colspan="2" align="center">
    <br />
    <input type="submit" value="Tiếp tục" />
    </td>
    </tr>
    </table>

    <?php
    print_foot();
  exit();
}

//database connection is setup now
//next step isto check if the table structure is present.

if(!isset($_SESSION['completed']['database_structure']))
{
  $db = new db();
  $db->Halt_On_Error = 'no';
  if (!@$db->connect($GO_CONFIG->db_name, $GO_CONFIG->db_host, $GO_CONFIG->db_user, $GO_CONFIG->db_pass))
  {
    print_head();
    echo 'Không thể kết nối với cơ sở dữ liệu!';
    echo '<br /><br />Hãy điều chỉnh và nhấn Refresh để cập nhật lại trang này.';
    print_foot();
    exit();
  }else
  {
    $settings_exist = false;
    $db->query("SHOW TABLES");
    if ($db->num_rows() > 0)
    {
      //structure exists see if the settings table exists
      while ($db->next_record())
      {
	if ($db->f(0) == 'settings')
	{
	  $settings_exist = true;
	  break;
	}
      }
    }
    if ($settings_exist)
    {
      $db->query("SELECT value FROM settings WHERE name='version'");
      if ($db->next_record())
      {
	$db_version=str_replace('.','', $db->f('value'));
	require('lib/updates.inc');
	if (!isset($updates[$db_version]))
	{
	  $db_version = false;
	}
      }else
      {
	$db_version = false;
      }
      print_head();
      if (isset($feedback))
      {
	echo $feedback.'<br /><br />';
      }
      ?>
	<script type="text/javascript">
	function delete_database()
	{
	  if (confirm("Bạn có chắc muốn xóa mọi thông tin về người dùng và dữ liệu trong HPT-OBM không?"))
	  {
	    document.forms[0].upgrade.value="false";
	    document.forms[0].submit();
	  }

	}
      </script>
	<input type="hidden" name="task" value="database_structure" />
	<input type="hidden" name="upgrade" value="true" />
	HPT-OBM tìm ra một phiên bản HPT-OBM đã được cài đặt. Bạn có muốn giữ nguyên dữ liệu và thực hiện nâng cấp không?
	<?php
	if (!$db_version)
	{
	  echo '<br /><br />HPT-OBM không thể xác định số hiệu phiên bản HPT-OBM cũ của bạn.'.
	    'Trình cài đặt cần số hiệu phiên bản cũ để xác định cần phải cập nhật những gì.<br />'.
	    'Vui lòng nhập số hiệu phiên bản bên dưới nếu bạn muốn thực hiện nâng cấp.';
	}
      ?>
	<br /><br />
	<table width="100%" style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
	<?php
	if (!$db_version)
	{
	  echo '<tr><td>Version:</td><td>';
	  $db_version = isset($db_version) ? $db_version : $GO_CONFIG->db_version;
	  echo '<input type="text" size="4" maxlength="4" name="db_version" value="'.$db_version.'" /></td></tr>';
	}else
	{
	  echo '<input type="hidden" name="db_version" value="'.$db_version.'" />';
	}
	?>
	  <tr>
	  <td colspan="2" align="center">
	  <input type="submit" value="Có" />
	  &nbsp;&nbsp;
	<input type="button" onclick="javasscript:delete_database()" value="Không" />
	  </td>
	  </tr>
	  </table>
	  <?php
	  print_foot();
	exit();
    }else
    {
      print_head();
      echo '<input type="hidden" name="upgrade" value="false" />'.
	'<input type="hidden" name="fresh_install" value="true" />'.
	'<input type="hidden" name="task" value="database_structure" />';


      echo 'HPT-OBM đã kết nối thành công với cơ sở dữ liệu!<br />'.
	'Hãy nhấn \'Tiếp tục\' để tạo bảng cơ sở cho HPT-OBM.'.
	'Việc này có thể mất một ít thời gian. Đừng ngắt đột ngột tiến trình này.<br /><br />';
      echo '<div align="center"><input type="submit" value="Tiếp tục" /></div>';
      print_foot();
      exit();
    }
  }
}

//database structure exists now and is up to date
//now we get the userdir

if (!isset($_SESSION['completed']['userdir']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="userdir" />
    HPT-OBM cần một nơi để lưu trữ dữ liệu. Hãy tạo một thư mục cho phép ghi để lưu trữ dữ liệu và nhập đường dẫn đó vào bên dưới.<br />
    Người dùng chạy webserver cần được phép ghi vào thư mục này (Trong Linux, hãy đặt quyền 0777 và owner là người dùng chạy webserver - Bạn có thể sẽ cần quyền root để thực hiện điều này).
    <br />Ngoài ra cần nhập kích thước tối đa cho phép đưa tập tin lên server (và quyền truy cập tập tin dạng bát phân với Linux).
    <br /><br />
    Trong Linux, thực hiện lệnh:<br/>
    <font color="#003399"><i>
    $ su<br />
    $ mkdir /home/groupoffice<br />
    $ chown apache:apache /home/groupoffice<br />
    </i></font>

    <br /><br />
    <table width="100%" style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td colspan="2">Thư mục lưu trữ dữ liệu:</td>
    </tr>
    <tr>
    <?php
    $userdir = isset($_POST['userdir']) ? $_POST['userdir'] : $GO_CONFIG->file_storage_path;
  ?>
    <td colspan="2"><input type="text" size="50" name="userdir" value="<?php echo $userdir; ?>" /></td>
    </tr>
    <tr>
    <td>
    Kích thước tập tin đưa lên tối đa:
    </td>
    </tr>
    <td>
    <input type="text" size="50" name="max_file_size" value="<?php echo $GO_CONFIG->max_file_size; ?>"  />
    </td>
    </tr>
    <tr>
    <td>
    Create mode:
    </td>
    </tr>
    <tr>
    <td>
    <?php
    $create_mode_string = decoct((string)$GO_CONFIG->create_mode);
  if (strlen($create_mode_string) == 3)
  {
    $create_mode_string = '0'.$create_mode_string;
  }
  ?>
    <input type="text" size="4" name="create_mode" value="<?php echo $create_mode_string; ?>" />
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}

//database structure exists now and is up to date
//now we get the tempdir

if (!isset($_SESSION['completed']['tmpdir']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="tmpdir" />
    HPT-OBM cần chỗ để lưu thông tin tạm thời như session và các tập tin tải lên. Hãy tạo một thư mục cho phép ghi và nhập vào bên dưới.<br />
    Trong Linux, thư mục /tmp là một lựa chọn tốt.
    <br /><br />
    <table width="100%" style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>Thư mục tạm:</td>
    </tr>
    <tr>
    <?php
    $tmpdir = isset($_POST['tmpdir']) ? $_POST['tmpdir'] : $GO_CONFIG->tmpdir;
  ?>
    <td><input type="text" size="50" name="tmpdir" value="<?php echo $tmpdir; ?>" /></td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}

if (!isset($_SESSION['completed']['url']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="url" />
    Hãy nhập URL tương đối và tuyệt đối.<br /><br />
    <font color="#003399"><i>
    Ví dụ:<br />
    URL tương đối: /hpt-obm/<br />
    Absolute URL: http://www.hptvietnam.com.vn/hpt-obm/</i>
    </font>
    <br /><br />
    <table width="100%" style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>
    URL tương đối:
    </td>
    <td>
    <?php
    $host = isset($_POST['host']) ? $_POST['host'] : $GO_CONFIG->host;
  ?>
    <input type="text" size="40" name="host" value="<?php echo $host; ?>" />
    </td>
    </tr>
    <tr>
    <td>URL tuyệt đối:</td>
    <td>
    <?php
    $full_url = isset($_POST['full_url']) ? $_POST['full_url'] : $GO_CONFIG->full_url;
  ?>
    <input type="text" size="40" name="full_url" value="<?php echo $full_url; ?>" />
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}


//the title of Group-Office
if (!isset($_SESSION['completed']['title']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="title" />
    Hãy nhập tên phiên bản HPT-OBM của bạn và địa chỉ e-mail của webmaster cho ứng dụng.<br />
    Địa chỉ e-mail sẽ nhận thông tin về những người dùng vừa đăng ký.
    <br /><br />
    <table style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>Tên:</td>
    </tr>
    <tr>
    <?php
    $title = isset($_POST['title']) ? $_POST['title'] : $GO_CONFIG->title;
  $webmaster_email = isset($_POST['webmaster_email']) ? $_POST['webmaster_email'] : $GO_CONFIG->webmaster_email;
  ?>
    <td><input type="text" size="50" name="title" value="<?php echo $title; ?>" /></td>
    </tr>
    <tr>
    <td>
    E-mail Webmaster:
    </td>
    </tr>
    <tr>
    <td>
    <input type="text" size="50" name="webmaster_email" value="<?php echo $webmaster_email; ?>" />
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}

//theme
if (!isset($_SESSION['completed']['theme']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="theme" />
    Hãy chọn kiểu dáng mặc định cho HPT-OBM và cho biết người dùng có được quyền đổi kiểu dáng hay không.
    <br /><br />
    <table style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>Kiểu dáng mặc định:</td>
    <td>
    <?php
    $themes = $GO_THEME->get_themes();
  $dropbox = new dropbox();
  $dropbox->add_arrays($themes, $themes);
  $dropbox->print_dropbox("theme", $GO_CONFIG->theme);
  ?>
    </td>
    </tr>
    <tr>
    <td align="right">Cho phép:</td>
    <td>
    <?php
    $allow_themes = isset($_POST['allow_themes']) ? $_POST['allow_themes'] : $GO_CONFIG->allow_themes;
  $allow_themes = $allow_themes ? 'true' : 'false';
  $dropbox = new dropbox();
  $dropbox->add_value('true', 'Có');
  $dropbox->add_value('false', 'Không');
  $dropbox->print_dropbox('allow_themes', $allow_themes);
  ?>
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}

//allow_password_change
if (!isset($_SESSION['completed']['allow_password_change']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="allow_password_change" />
    Bạn có muốn cho phép người dùng đổi mật khẩu của họ không?
    <br /><br />
    <table style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td align="right">Cho phép:</td>
    <td>
    <?php
    $allow_password_change = isset($_POST['allow_password_change']) ? $allow_password_change : $GO_CONFIG->allow_password_change;
  $allow_password_change = $allow_password_change ? 'true' : 'false';
  $dropbox = new dropbox();
  $dropbox->add_value('true', 'Có');
  $dropbox->add_value('false', 'Không');
  $dropbox->print_dropbox('allow_password_change', $allow_password_change);
  ?>
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}
//language
if (!isset($_SESSION['completed']['language']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="language" />
    Hãy chọn ngôn ngữ mặc định cho HPT-OBM. HPT-OBM sẽ đặt các thiết lập khác dựa trên thiết lập này. 
    <br /><br />
    <table style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>Ngôn ngữ:</td>
    <td>
    <?php
    $dropbox= new dropbox();
  $languages = $GO_LANGUAGE->get_languages();
  while($language = array_shift($languages))
  {
    $dropbox->add_value($language['code'], $language['description']);
  }
  $dropbox->print_dropbox("language", $GO_CONFIG->language);
  ?>
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Tiếp tục" />
    </div>
    <?php
    print_foot();
  exit();
}

if (!isset($_SESSION['completed']['smtp']))
{
  print_head();
  if (isset($feedback))
  {
    echo $feedback.'<br /><br />';
  }
  ?>
    <input type="hidden" name="task" value="smtp" />
    HPT-OBM có thể gửi và nhận e-mail. Vui lòng cho biết thiết lập SMTP server của bạn. <br />
    Hãy để trong chỗ này nếu bạn dùng hàm php mail(), nhưng bạn sẽ không thể dùng dùng CC và BCC header!
    <br />
    <br />
    <table style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
    <tr>
    <td>
    Mailer:
    </td>
    <td>
    <?php
    $dropbox = new dropbox();
  $dropbox->add_value('mail', 'PHP Mail() Function');
  $dropbox->add_value('sendmail', 'Use local sendmail');
  $dropbox->add_value('qmail', 'Use local Qmail');
  $dropbox->add_value('smtp', 'Use remote SMTP');
  $dropbox->print_dropbox('mailer', $GO_CONFIG->mailer, 'onchange="javascript:change_mailer()"');
  ?>
    </td>
    </tr>
    <tr>
    <td>
    SMTP server:
    </td>
    <td>
    <input type="text" size="40" name="smtp_server" value="<?php echo $GO_CONFIG->smtp_server; ?>"  />
    </td>
    </tr>
    <tr>
    <td>
    Cổng SMTP:
    </td>
    <td>
    <input type="text" size="40" name="smtp_port" value="<?php echo $GO_CONFIG->smtp_port; ?>" />
    </td>
    </tr>
    <tr>
    <td>
    Kích thước tập tin đính kèm tối đa:
    </td>
    <td>
    <input type="text" size="40" name="max_attachment_size" value="<?php echo $GO_CONFIG->max_attachment_size; ?>" />
    </td>
    </tr>
    <tr>
    <td colspan="2">
    <br />
    Vài máy chủ cần tùy chọn chuỗi kết nối khi kết nối với máy chủ IMAP hoặc POP-3 bằng phần mở rộng PHP IMAP. Ví dụ các hệ thống Red Hat cần '/notls' hoặc '/novalidate-cert'. Nếu bạn không rõ, hãy để trống phần này.
    <br /><br />
    </td>
    </tr>
    <tr>
    <td>
    Tùy chọn kết nối:
    </td>
    <td>
    <input type="text" size="40" name="email_connectstring_options" value="<?php echo $GO_CONFIG->email_connectstring_options; ?>" />
    </td>
    </tr>
    </table><br />
    <div align="center">
    <input type="submit" value="Kết nối" />
    </div>
    <script type="text/javascript">
    function change_mailer()
    {
      if(document.forms[0].mailer.value=='smtp')
      {
	document.forms[0].smtp_server.disabled=false;
	document.forms[0].smtp_port.disabled=false;
      }else
      {
	document.forms[0].smtp_server.disabled=true;
	document.forms[0].smtp_port.disabled=true;
      }  
    }
  change_mailer();
  </script>
    <?php
    print_foot();
  exit();
}


//check if we need to add the administrator account

if(!isset($_SESSION['completed']['administrator']))
{
  if (!$GO_USERS->get_user(1))
  {
    print_head();
    if (isset($feedback))
    {
      echo $feedback.'<br /><br />';
    }
    ?>
      <input type="hidden" name="task" value="administrator" />
      HPT-OBM cần một tài khoản cho quản trị hệ thống. Tên người dùng là 'admin'. Hãy nhập mật khẩu cho 'admin'.
      <br /><br />
      <table style="border-width: 0px;font-family: Arial,Helvetica; font-size: 12px;">
      <tr>
      <td>Tên người dùng:</td>
      <td>
      <?php 
      $username = isset($_POST['username']) ? smartstrip(htmlspecialchars($_POST['username'])) : 'admin';
    ?>
      <input name="username" type="text" value="<?php echo $username; ?>" />
      </tr>
      <tr>
      <td>
      Mật khẩu:
      </td>
      <td>
      <input type="password" name="pass1" />
      </td>
      </tr>
      <tr>
      <td>
      Xác nhận mật khẩu:
      </td>
      <td>
      <input type="password" name="pass2" />
      </td>
      </tr>
      <tr>
      <td>
      E-mail:
      </td>
      <td>
      <?php $email = isset($email)? $email : $GO_CONFIG->webmaster_email;?>
      <input type="text" size="40" name="email" value="<?php echo $email; ?>" />
      </td>
      </tr>
      </table><br />
      <div align="center">
      <input type="submit" value="Tiếp tục" />
      </div>
      <?php
      print_foot();
    exit();
  }
}


print_head();

?>
Quá trình cài đặt đã hoàn tất!<br />
<br />
Vui lòng kiểm tra lại và đảm bảo là không ai được phép sửa chữa tập tin Group-Office.php từ lúc này.<br />
<br />
Trong Linux, hãy thực hiện:<br/>
<font color="#003399"><i>
$ chown -R someuser:someuser <?php echo $GO_CONFIG->root_path; ?><br />
$ chmod 755 <?php echo $GO_CONFIG->root_path; ?><br />
$ chmod 644 <?php echo $GO_CONFIG->root_path; ?>Group-Office.php

</i></font>
<br />
<br />
Trong Windows, hãy đặt thuộc tính Read-Only cho Group-Office.php
<br />
<br />
Sau khi hoàn tất, hãy đăng nhập vào HPT-OBM với tài khoản của quản trị hệ thống.<br />
<ul>
<li>Chọn phần Cấu hình, Thêm/bớt chức năng và chọn cài đặt những module bạn cần</li>
<li>Chọn phần Cấu hình, Quản lý nhóm để tạo các nhóm mới.</li>
<li>Chọn phần Cấu hình, Quản lý người dùng để thêm các người dùng mới.</li>
</ul>
<br />
<br />
Bạn cũng có thể cấu hình xác thực độc lập như IMAP, POP-3, hoặc máy chủ LDAP.
Hãy xem 'auth_sources.inc' để biết thêm thông tin chi tiết.
<br />
<br />
<div align="center">
<input type="button" value="Mở HPT-OBM!" onclick="javascript:window.location='<?php echo $GO_CONFIG->host; ?>';" />
</div>
<?php
print_foot();
session_destroy();
?>
