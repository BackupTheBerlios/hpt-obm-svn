<?php
$error_feedbacks = array();
$info_feedbacks = array();

$detail_config = false;
if (file_exists('config.inc.php') && is_readable('config.inc.php'))
{
	$cfg = parse_ini_file('config.inc.php');
}
else
	$no_config = true;
// Default configuration
if (!isset($cfg['cfg_root']))
	$cfg['cfg_root'] = get_default_root();
if (!isset($cfg['cfg_password']))
	$cfg['cfg_password'] = '';
if (!isset($cfg['db_hostname']))
	$cfg['db_hostname'] = '';
if (!isset($cfg['db_username']))
	$cfg['db_username'] = '';
if (!isset($cfg['db_password']))
	$cfg['db_password'] = '';
if (!isset($cfg['db_name']))
	$cfg['db_name'] = '';
if (!isset($cfg['db_prefix']))
	$cfg['db_prefix'] = '';

if (isset($no_config))
{
	header_config();
	exit;
}

$cfg_root = isset($_POST['cfg_root']) ? $_POST['cfg_root'] : $cfg['cfg_root'];
if (!file_exists($cfg_root.'/Group-Office.php'))
{
	$info_feedbacks[] = 'Wrong cfg_root!';
	session_start();
}
else
{
	$detail_config = true;
	require($cfg_root.'/Group-Office.php');
	$params = new GO_PARAMS();

	$defparams = array(
		'cfg_relative_url' => str_replace('setup.php','',$_SERVER['SCRIPT_NAME']),
		'cfg_absolute_url' => 'http://'.$_SERVER['HTTP_HOST'].str_replace('setup.php','',$_SERVER['SCRIPT_NAME']),
		'cfg_language' => 'vn',
		'cfg_title' => 'HPT-OBM',
		'cfg_webmaster_email' => 'webmaster@example.com',
		'cfg_tmpdir' => '/tmp',
		'cfg_max_users' => 20,
		'cfg_refresh_rate' => 5,
		'cfg_user_quota' => 100,
		'cfg_mailer' => 'smtp',
		'cfg_smtp_port' => 25,
		'cfg_smtp_server' => 'smtp.example.com',
		'cfg_max_attachment_size' => 100000,
		'cfg_file_storage' => ''
	);
	foreach ($defparams as $key => $val)
		if (!isset($params->params[$key]) ||
			$params->params[$key] == '')
			$params->params[$key] = $defparams[$key];
}

if ((!isset($_SESSION['logged']) || $_SESSION['logged'] != 1) &&
	isset($_POST['cfg_passwd']) &&
	$_POST['cfg_passwd'] == $cfg['cfg_password'])
	$_SESSION['logged'] = 1;

if ((!isset($_SESSION['logged']) || $_SESSION['logged'] != 1))
{
	print_head();
?>
Please enter config password:
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<input type="password" name="cfg_passwd" />
<input type="submit" />
</form>
<?php
	print_foot();
	exit;
}

header_config();

function header_config()
{
	global $cfg,$error_feedbacks,$info_feedbacks,$params,$detail_config;

	$cfg_password = isset($_POST['cfg_password']) ? $_POST['cfg_password'] : $cfg['cfg_password'];
	$cfg_root = isset($_POST['cfg_root']) ? $_POST['cfg_root'] : $cfg['cfg_root'];
	$db_hostname = isset($_POST['db_hostname']) ? $_POST['db_hostname'] : $cfg['db_hostname'];
	$db_name = isset($_POST['db_name']) ? $_POST['db_name'] : $cfg['db_name'];
	$db_username = isset($_POST['db_username']) ? $_POST['db_username'] : $cfg['db_username'];
	$db_password = isset($_POST['db_password']) ? $_POST['db_password'] : $cfg['db_password'];
	$db_type = 'mysql';
	$db_prefix = '';

	// Process the form
	if (isset($_POST['write']) || isset($_POST['download']) || isset($_POST['view']))
	{
		$cfg_path = $cfg_root.'/config.inc.php';
		$tpl_path = $cfg_root.'/config.inc.tpl.php';
		$values = array(
			'cfg_password' => $cfg_password,
			'cfg_root' => $cfg_root,
			'db_hostname' => $db_hostname,
			'db_name' => $db_name,
			'db_username' => $db_username,
			'db_password' => $db_password,
			'db_type' => $db_type,
			'db_prefix' => $db_prefix);

		if (isset($_POST['write']) &&
			(!file_exists($cfg_path) || is_writable($cfg_path)) &&
			($fp = @fopen($cfg_path,"wt")) != FALSE)
		{
			fwrite($fp,array_to_ini($values));
			fclose($fp);
			// Once we write the config file successfully. Try to use it.
			$_SESSION['logged'] = 0;
			header('Location: '.$_SERVER['PHP_SELF']);
			return;
		}
		else
		{
			$error_feedbacks[] = 'Could not write to config.inc.php. Please check access permission or download it and write it manually';
		}

		if (isset($_POST['download']))
		{
			header('Content-Type: text/plain');
			header('Expires: '.gmdate('D, d M Y H:i:s') . ' GMT');
			header('Content-Disposition: attachment; filename="config.inc.php"');
			header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
			header('Pragma: public, no-cache');
			header('Content-Transfer-Encoding: binary');
			echo array_to_ini($values);
			exit;
		}

		if (isset($_POST['view']))
		{
			header('Content-Type: text/plain');
			header('Expires: '.gmdate('D, d M Y H:i:s') . ' GMT');
			header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
			header('Pragma: public, no-cache');
			header('Content-Transfer-Encoding: binary');
			echo array_to_ini($values);
			exit;
		}
	}

	if (isset($_POST['save']))
	{
		$param_names = array(
			'cfg_relative_url',
			'cfg_absolute_url',
			'cfg_language',
			'cfg_title',
			'cfg_webmaster_email',
			'cfg_tmpdir',
			'cfg_max_users',
			'cfg_refresh_rate',
			'cfg_user_quota',
			'cfg_mailer',
			'cfg_smtp_port',
			'cfg_smtp_server',
			'cfg_max_attachment_size',
			'cfg_file_storage'
		);
		foreach ($param_names as $name)
			if (isset($_POST[$name]))
				$params->params[$name] = $_POST[$name];
		$params->update();
		$params->reload();
	}

// Okay, you are authenticated
print_head();
?>
<h1><center>Base Configuration</center></h1>
<form action="<?=$_SERVER['PHP_SELF']?>" method="post">
<table border="0" width="100%">
<tr><td>Config password</td><td><input type="text" name="cfg_password" value="<?=$cfg_password?>"/></td></tr>
<tr><td>Root path</td><td><input type="text" name="cfg_root" value="<?=$cfg_root?>"/></td></tr>
<tr><td>Database hostname</td><td><input type="text" name="db_hostname" value="<?=$db_hostname?>"/></td></tr>
<tr><td>Database name</td><td><input type="text" name="db_name" value="<?=$db_name?>"/></td></tr>
<tr><td>Database username</td><td><input type="text" name="db_username" value="<?=$db_username?>"/></td></tr>
<tr><td>Database password</td><td><input type="text" name="db_password" value="<?=$db_password?>"/></td></tr>
<tr><td colspan="2"><center>
<input type="submit" name="write" value="Write"/>
&nbsp;&nbsp;
<input type="submit" name="download" value="Download"/>
&nbsp;&nbsp;
<input type="submit" name="view" value="View"/></center></td></tr>
</table>

<?php if ($detail_config) : ?>
<h1><center>Detail Configuration</center></h1>
<input type="hidden" name="detail_config" value="<?=$detail_config?>"/>
<table border="0" width="100%">
<tr><td>Relative URL</td><td><input type="text" name="cfg_relative_url" value="<?=$params->params['cfg_relative_url']?>"/></td></tr>
<tr><td>Absolute URL</td><td><input type="text" name="cfg_absolute_url" value="<?=$params->params['cfg_absolute_url']?>"/></td></tr>
<tr><td>Document location</td><td><input type="text" name="cfg_file_storage" value="<?=$params->params['cfg_file_storage']?>"/></td></tr>
<tr><td>Default language</td><td><input type="text" name="cfg_language" value="<?=$params->params['cfg_language']?>"/></td></tr>
<tr><td>Title</td><td><input type="text" name="cfg_title" value="<?=$params->params['cfg_title']?>"/></td></tr>
<tr><td>Webmaster email</td><td><input type="text" name="cfg_webmaster_email" value="<?=$params->params['cfg_webmaster_email']?>"/></td></tr>
<tr><td>Temporary directory</td><td><input type="text" name="cfg_tmpdir" value="<?=$params->params['cfg_tmpdir']?>"/></td></tr>
<tr><td>Max users</td><td><input type="text" name="cfg_max_users" value="<?=$params->params['cfg_max_users']?>"/></td></tr>
<tr><td>Refresh rate</td><td><input type="text" name="cfg_refresh_rate" value="<?=$params->params['cfg_refresh_rate']?>"/></td></tr>
<!-- tr><td>Enable DAV</td><td><input type="text" name="cfg_dav_switch" value="<?=$params->params['cfg_dav_switch']?>"/></td></tr -->
<tr><td>User quota</td><td><input type="text" name="cfg_user_quota" value="<?=$params->params['cfg_user_quota']?>"/></td></tr>
<tr><td>Mailer type</td><td><input type="text" name="cfg_mailer" value="<?=$params->params['cfg_mailer']?>"/></td></tr>
<tr><td>Mail server</td><td><input type="text" name="cfg_smtp_server" value="<?=$params->params['cfg_smtp_server']?>"/></td></tr>
<tr><td>Mail server port</td><td><input type="text" name="cfg_smtp_port" value="<?=$params->params['cfg_smtp_port']?>"/></td></tr>
<tr><td>Max attachment size</td><td><input type="text" name="cfg_max_attachment_size" value="<?=$params->params['cfg_max_attachment_size']?>"/></td></tr>
<tr><td colspan="2"><center>
<input type="submit" name="save" value="Save"/></center></td></tr>
</table>
<?php endif ?>

</form>
<?php
print_foot();
}


function print_head()
{
	global $error_feedbacks,$info_feedbacks;
?>
<html><head><title>HPT-OBM Installation</title></head><body style="font-family: Arial,Helvetica">
<form method="post" action="<?=$_SERVER["PHP_SELF"]?>">
<table align="center" style="background: #f1f1f1;border-width: 1px;border-color: black;border-style: solid;font-family: Arial,Helvetica; font-size: 12px; width: 500px;">
<tr><td align="center"><h2><img src="lib/intermesh.gif" border="0" /><br />HPT-OBM installation</h2></td></tr>
<?php
	foreach ($error_feedbacks as $msg)
		echo '<tr><td><b>'.htmlspecialchars($msg).'</b></td></tr>';
	foreach ($info_feedbacks as $msg)
		echo '<tr><td><i>'.htmlspecialchars($msg).'</i></td></tr>';
?>
<tr><td><table style="border-width: 0px;padding: 20px;font-family: Arial,Helvetica; width: 500px;font-weight: normal; font-size: 12px;"><tr><td>

<?php
}

function print_foot()
{
?>
</td></tr></table></td></tr></table></form></body></html>
<?php
}

function get_default_root()
{
	return str_replace('setup.php','',$_SERVER['SCRIPT_FILENAME']);
}

function array_to_ini($assoc_array)
{
	$content = '';
	foreach($assoc_array as $key => $item) {
		if(is_array($item)) {
			$content .= "\n[{$key}]\n";
			foreach ($item as $key2 => $item2) {
				if(is_numeric($item2) || is_bool($item2))
				$content .= "{$key2} = {$item2}\n";
				else
				$content .= "{$key2} = \"{$item2}\"\n";
			}
		} else {
			if(is_numeric($item) || is_bool($item))
			$content .= "{$key} = {$item}\n";
			else
			$content .= "{$key} = \"{$item}\"\n";
		}
	}
	return $content;
}

?>
