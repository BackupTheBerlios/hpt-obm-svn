<?php
/*
Copyright Intermesh 2003
Author: Merijn Schering <mschering@intermesh.nl>
Version: 1.0 Release date: 08 July 2003

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.
*/

require("../Group-Office.php");
$GO_SECURITY->authenticate();
require($GO_LANGUAGE->get_language_file('bookmarks'));

require($GO_THEME->theme_path."simple_header.inc");
if ($_SERVER['REQUEST_METHOD'] == "POST")
{
	require($GO_CONFIG->class_path."bookmarks.class.inc");
	$bookmarks = new bookmarks();
	$URL = trim($_REQUEST['URL']);
	$name = trim($_REQUEST['name']);
	$invalid[] = "\"";
	$invalid[] = "&";
	$invalid[] = "?";

	if (!validate_input($name,$invalid))
	{
		$feedback = "<p class=\"Error\">".$invalid_chars.": \" & ?</p>";
	}else
	{
		if ($URL != "" && $name != "")
		{
			if (!eregi('(^http[s]*:[/]+)(.*)', $URL))
			$URL= "http://".$URL;
			if ($_REQUEST['bookmark_id'])
			{
				if($bookmarks->update_bookmark($_REQUEST['bookmark_id'], $URL, $name, $_REQUEST['new_window']))
				{
					echo "<script type=\"text/javascript\">\nopener.location=opener.location\nwindow.close()\n</script>";
				}else
				{
					$feedback = "<p class=\"Error\">".$strSaveError."</p>";
				}
			}else
			{
				if($bookmarks->add_bookmark($GO_SECURITY->user_id, $URL, $name, $_REQUEST['new_window']))
				{
					echo "<script type=\"text/javascript\">\nopener.location=opener.location\nwindow.close()\n</script>";
				}else
				{
					$feedback = "<p class=\"Error\">".$strSaveError."</p>";
				}
			}
		}else
		{
			$feedback = "<p class=\"Error\">".$error_missing_field."</p>";
		}
	}
}
?>

<form name="add" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<?php
$check = true;
if (isset($_REQUEST['bookmark_id']))
{
	echo "<input type=\"hidden\" value=\"".$_REQUEST['bookmark_id']."\" name=\"bookmark_id\" />\n";
        if ($_REQUEST['new_window'] != 1)
        {
                $check =false;
        }
}
?>
<table border="0" cellpadding="10" cellspacing="0" align="center" valign="center">
<tr>
	<td>
	<table border="0" cellpadding="0" cellspacing="3">
	<tr>
		<td colspan="2" valign="top">
		<h1><?php echo $bm_save_title; ?></h1>
		<?php if (isset($feedback)) echo $feedback; ?>
		</td>
	</tr>
	<tr>
		<td>URL:</td>
		<td><input type="text" class="textbox" size="50" name="URL" maxlength="200" value="<?php if(isset($_REQUEST['bURL'])) echo $_REQUEST['bURL']; ?>" /></td>
	</tr>
	<tr>
		<td><?php echo $strName; ?>:</td>
		<td><input type="text" class="textbox" size="50" name="name" maxlength="50" value="<?php if(isset($_REQUEST['bname'])) echo smartstrip($_REQUEST['bname']); ?>" /></td>
	</tr>
	<tr>
		<td colspan="2">
		<?php 
		$checkbox = new checkbox('new_window', 'true', $bm_new_window, $check);
		?> 
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center" height="20" valign="bottom">
		<?php
		$button = new button($cmdOk, 'javascript:document.forms[0].submit()');
		echo '&nbsp;&nbsp;';
		$button = new button($bm_current_page, 'javascript:get_current_page()');
		echo '&nbsp;&nbsp;';
		$button = new button($cmdCancel, 'javascript:window.close()');
		?>
		</td>
	</tr>
	</table>
	</td>
</tr>
</table>
</form>
<script type="text/javascript" language="javascript">
<!--
	function get_current_page()
	{
		document.add.URL.value=opener.location;
		document.add.name.value=opener.document.title;
	}
	document.forms[0].URL.focus();
-->
</script>
<?php
require($GO_THEME->theme_path."simple_footer.inc");
?>

