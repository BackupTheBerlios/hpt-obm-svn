<table border="0" class="normal" cellpadding="2" cellspacing="0">
<tr>
        <td colspan="2">
        <br><?php if (isset($feedback)) echo $feedback; ?>
        <?php echo $security_text; ?><br><br>
        </td>
</tr>
<?php
if (isset($val->error["currentpassword"]))
{
?>
<tr>
        <td class="Error" colspan="2">
          <?php echo $val->error["currentpassword"]; ?>
        </td>
</tr>
<?php } ?>
<tr>
	<td align="right" nowrap><?php echo $security_current_password; ?>:&nbsp;</td>
	<td ><input size="40" class="textbox" type="password" name="currentpassword" maxlength="20"></td>
</tr>
<tr>
        <td>&nbsp;</td>
</tr>
<?php
if (isset($val->error["newpass1"]))
{
?>
<tr>
        <td class="Error" colspan="2">
                <?php echo $val->error["newpass1"]; ?>
        </td>
</tr>
<?php } ?>
<tr>
	<td align="right" nowrap><?php echo $security_new_password; ?>:&nbsp;</td>
	<td ><input size="40" class="textbox" type="password" name="newpass1" maxlength="20"></td>
</tr>
<?php
if (isset($val->error["newpass2"]))
{
?>
<tr>
        <td class="Error" colspan="2">
                <?php echo $val->error["newpass2"]; ?>
        </td>
</tr>
<?php } ?>
<tr>
	<td align="right" nowrap><?php echo $security_confirm_password; ?>:&nbsp;</td>
	<td ><input size="40" class="textbox" type="password" name="newpass2" maxlength="20"></td>
</tr>
<tr>
        <td>&nbsp;</td>
</tr>
<tr height="25">
	<td colspan="99"><?php
	$button = new button($cmdOk, "javascript:_save('change_password', 'true')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdApply, "javascript:_save('change_password', 'false')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
	?></td>
</tr>
</table>
<script type="text/javascript">
function _save(task, close)
{
	document.forms[0].task.value = task;
	document.forms[0].close.value = close;
	document.forms[0].submit();
}
</script>

