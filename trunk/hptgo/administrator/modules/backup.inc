<?php
echo '<b>Backup</b><br/><br/>';
echo '&nbsp;&nbsp;';
new checkbox('a','b','Backup database');echo '<br/>';
echo '&nbsp;&nbsp;';
new checkbox('a','b','Backup filesystem');echo '<br/>';
echo '&nbsp;&nbsp;';
new checkbox('a','b','Backup source code');echo '<br/>';
echo '<br/>&nbsp;&nbsp;';
new button('Backup',"javascript:backup();");
echo '<br/>';
echo '<br/>';
echo '<b>Restore</b><br/><br/>';
echo '&nbsp;&nbsp;';
echo '<input type="file" name="upload"/><br/>';
echo '<br/>&nbsp;&nbsp;';
new button('Restore',"javascript:restore();");
    echo '<br/><br/>';
    $button = new button($cmdClose,"javascript:document.location='$return_to'");
?>
<script language="javascript">
function restore()
{
  if (confirm("Are you sure?")) {
    alert("Unimplemented :-(");
    document.forms[0].task.value='restore';
    document.forms[0].submit();
    document.forms[0].task.value='';
  }
}
function backup()
{
document.forms[0].task.value='backup';
document.forms[0].submit();
document.forms[0].task.value='';
}
</script>
