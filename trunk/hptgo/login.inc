<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo $charset?>">
<link rel="stylesheet" type="text/css" href="<?php echo $GO_CONFIG->host; ?>lib/login.css" />
<script language="javascript" type="text/javascript" src="<?php echo $GO_CONFIG->host; ?>javascript/common.js"></script>
<link rel="shortcut icon" href="<?php echo $GO_CONFIG->host; ?>lib/favicon.ico" />
<title><?php echo $GO_CONFIG->title; ?></title>
</head>
<body>
<form method="post" action="<?php echo $GO_CONFIG->host; ?>index.php" name="login" target="_parent">
<input type="hidden" name="task" value="login" />
<?php
if (isset($_GET['return_to']))
{
  echo '<input type="hidden" name="return_to" value="'.$_GET['return_to'].'" />';
}
?>
<table border="0" width="100%" height="95%" cellpadding="0" cellspacing="0">
<tr>
<td align="center" valign="center">
<table id="header" border="0">
<tr>
<td id="header-logo">
<img src="<?php echo $GO_CONFIG->login_image; ?>" border="0" align="absmiddle" />
</td>
</tr>
<tr>
<td id="header-border"></td>
</tr>
<tr>
<td align="center">
<table cellspacing="2" border="0" cellpadding="1" valign="middle" align="center" style="margin: 20px;">
<tr>
<td colspan="2" align="center"><h1><?php echo $GO_CONFIG->title; ?></h1></td>
</tr>
<tr>
<td colspan="2" align="center">
<h3><?php echo $login_text; ?></h3>
<br />
<?php if (isset($feedback)) echo $feedback; ?>
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $login_username; ?>:&nbsp;
</td>
<td>
  <?php
if (count($auth_sources) > 1)
{
  $dropbox = new dropbox();
  for($i=0;$i<count($auth_sources); $i++)
  {
    $dropbox->add_value($i, $auth_sources[$i]['name']);
  }
  echo '
    <table border="0" cellpadding="1" cellspacing="0">
    <tr><td><input type="text" class="textbox" name="username" value="" size="30" /></td><td>@</td><td>
    ';
  $dropbox->print_dropbox('auth_source_key', $_REQUEST['auth_source_key']);
  echo '</td></tr></table>';
}else
{
  echo '<input type="hidden" name="auth_source_key" value="0" />';
  echo '<input type="text" class="textbox" name="username" value="" size="30" />';
}				
?>
</td>

</tr>
<tr>
<td align="right" nowrap>
<?php echo $login_password; ?>:&nbsp;
</td>

<td>
<input class="textbox" type="password" name="password" value="" size="30" />
</td>
</tr>
<tr>
<td align="right" nowrap>
<?php echo $login_language; ?>:&nbsp;
</td>
<td>
<?php
$dropbox= new dropbox();
$languages = $GO_LANGUAGE->get_languages();
while($language = array_shift($languages))
{
  if	(($language['code'] == 'en') || ($language['description'] == 'English') ||
  		 ($language['code'] == 'vn') || ($language['description'] == 'Vietnamese')) 
	$dropbox->add_value($language['code'], $language['description']);
}
$dropbox->print_dropbox("SET_LANGUAGE", $GO_LANGUAGE->language['code'], 'onchange="javascript:set_language(this)"');
?>
</td>

</tr>
<tr>
<td colspan="2">
<br />
<?php
$checkbox = new checkbox('remind', 'true', $login_remind, false);
?>
</td>
</tr>

<tr>
<td colspan="2" align="center">
<br />
<?php
$button = new button($cmdLogin, 'javascript:document.forms[0].submit();');
?>
</td>
</tr>
</table>
</td>
</tr>
<tr>
<td id="FooterBar">
<a style="color: white;" target="_blank" href="http://www.hptvietnam.com.vn">HPT Commerce&nbsp;&nbsp;-&nbsp;&nbsp;HPT Open Business Management <?php echo $GO_CONFIG->version; ?></a>
</td>
</tr>
</table>
</td>
</tr>
</table>

<script type="text/javascript" language="javascript">
var nav4 = window.Event ? true : false;
function processkeypress(e)
{
  if(nav4)
  {
    var whichCode = e.which;
  }else
  {
    var whichCode = event.keyCode;
  }

  if (whichCode == 13)
  {
    window.document.forms[0].submit();
    return true;
  }
}
if (window.Event) //if Navigator 4.X
{
  document.captureEvents(Event.KEYPRESS)
}
document.onkeypress = processkeypress;
document.forms[0].username.focus();

function set_language(dropbox)
{
  document.location='<?php echo $_SERVER['PHP_SELF']; ?>?SET_LANGUAGE='+dropbox.value;
}
</script>
</form>
</body>
</html>
