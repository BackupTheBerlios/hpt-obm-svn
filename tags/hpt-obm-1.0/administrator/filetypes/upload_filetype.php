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

require("../../Group-Office.php");
$GO_SECURITY->authenticate(true);
require($GO_LANGUAGE->get_base_language_file('filetypes'));
//load file management class
require($GO_CONFIG->class_path."filetypes.class.inc");
$filetypes = new filetypes();

if ($_SERVER['REQUEST_METHOD'] == "POST")
{
  if (is_uploaded_file($_FILES['uploaded_file']))
  {
    if (eregi("gif", $_FILES['uploaded_file']['type']))
    {
      $filetype = $filetypes->get_type($_POST['extension']);
      $filetypes->update_filetype($extension, $filetype['friendly'], $uploaded_file);
      echo "<script type=\"text/javascript\">\nopener.document.location.reload();\nwindow.close();\n</script>";
      exit();
    }
  }else
  {
    $feedback = "<p class=\"Error\">".$ft_no_file."</p>";
  }
}

$page_title="File upload";
require($GO_THEME->theme_path."simple_header.inc");
?>
<script type="text/javascript">
  <!--
function upload()
{
  var status = null;
  if (status = get_object("status"))
  {
    status.innerHTML = "<?php echo $ft_please_wait; ?>";
  }
  document.forms[0].submit();
}
-->
</script>
<form name="upload" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>" enctype="multipart/form-data">
<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_file_size; ?>" />
<input type="hidden" name="extension" value="<?php echo $_REQUEST['extension']; ?>" />

<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
<td class="Header" height="50">
&nbsp;<i><?php echo $ft_upload; ?></i>
</td>
</tr>
<tr>
<td>
<div class="menuBar">
&nbsp;
</div>
</td>
</tr>
</table>
<table border="0" cellpadding="10">
<tr>
<td>
<table border="0" class="normal">
  <?php
if (isset($feedback))
{
  echo "<tr><td colspan=\"2\">".$feedback."</td></tr>\n";
}
?>
<tr>
<td colspan="2">
<?php echo $ft_upload_text; ?><br /><br /></td>
</tr>
<tr>
<td><?php echo $ft_filename; ?>:</td>
<td><input type="file" name="uploaded_file" maxlength="100" /></td>

</tr>
<tr>
<td id="status" colspan="2" class="Success">
<br />
</td>
</tr>

<tr height="100">
<td colspan="2" align="center" class="cmd">
<br />
<a class="cmd" href="javascript:upload()"><?php echo $cmdOk; ?>&nbsp;|&nbsp;<a class="cmd" href="javascript:window.close()"><?php echo $cmdCancel; ?></a></a>
</tr>
</table>
</td>
</tr>
</table>
</form>
<?php
require($GO_THEME->theme_path."simple_footer.inc");
?>
