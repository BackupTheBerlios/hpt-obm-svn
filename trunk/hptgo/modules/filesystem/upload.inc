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
?>
<input type="hidden" name="task" value="upload" />
<input type="hidden" name="MAX_FILE_SIZE" value="<?php echo $GO_CONFIG->max_file_size; ?>" />

<?php
$tabtable = new tabtable('upload_tab', $fbUpload, '600', '300');
$tabtable->print_head();
?>
<table border="0" cellpadding="4" cellspacing="0">
<?php if(isset($feedback)) echo '<tr><td>'.$feedback.'</td></tr>';
?>
<tr>
	<?php
    if (!(strpos($path,$GO_CONFIG->file_storage_path.'projects/') === 0))
      echo '<td><h3>'.$fbPath.': '.$page_title.'</3><br /><br />'.$fbFiles.':<br/><br/>';
	?>
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
<!--
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" /><br />
	<input type="file" name="file[]" maxlength="100" class="textbox" size="70" />
-->
	</td>
</tr>
<tr>
	<td>
	<?php
	echo $fbMaxUploadSize.": ".format_size($GO_CONFIG->max_file_size)." (".number_format($GO_CONFIG->max_file_size, 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator'])." bytes)";
	if ( $GO_CONFIG->user_quota != 0 ) {
	  $absolute_path = split( "/", $page_title );
//	  echo $fbUserQuotaFree.": ".$absolute_path;
	  
	  exec( "du ".$GO_CONFIG->file_storage_path.$absolute_path[1]." -s", $retval );
	  sscanf( $retval[0], "%d", &$usedspace );
	  echo "<br>$fbFreeQuotaSpace: ".format_size( ($GO_CONFIG->user_quota - $usedspace)*1024 );
	}
	?>
	</td>
</tr>
<tr>
	<td id="status" class="Success">&nbsp;</td>
</tr>
<tr>
	<td>
		<br />
		<?php
		$button = new button($cmdOk, "javascript:upload()");
		echo '&nbsp;&nbsp;';
		$button = new button($cmdCancel, "javascript:document.location='".$_SERVER['PHP_SELF']."?path=".$urlencoded_path."';");
		?>
		</td>
</tr>
</table>
<?php
$tabtable->print_foot();
?>
<script type="text/javascript">
<!--
function upload()
{
	var status = null;
	if (status = get_object("status"))
	{
		status.innerHTML = "<?php echo $fbPleaseWait; ?>";
	}
	document.forms[0].submit();
}
-->
</script>
