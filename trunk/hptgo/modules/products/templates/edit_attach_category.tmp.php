<?php 
	if (!$GO_MODULES->write_permissions) goURL('index.php');
?>
<form name="frmAttachCategory" method="post">
<input name="txt_name" type="hidden">
<input name="task" type="hidden">
<input name="close_win" type="hidden">
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr class="TableHead2" height="20">
		<td align="center" width="20"> # </td>
		<td width="40%" nowrap><?php echo $sc_category_name?> </td>
		<td></td>
	</tr>
<?php
//	$pro2 = new products();
	
//	if ($pro->num_rows()>0) $pro->seek();
	$i=0;
	while ($pro->next_record())
	{
		$id = $pro->f('id');
		$name = $pro->f('name');
		
//		$pro2->get_categories($pro->f('category_id'));
//		$count = $pro2->num_rows();
		
		$click_del = "javascript:click_del(document.frmAttachCategory, $id , \"$name\" , \"".sprintf($sc_ConfirmDeleteCategory2, $name)."\")";
?>	
	<tr>
		<td align="center"> <?php echo ++$i;?> </td>
		<td nowrap>
			<a href="<?php echo "javascript:set_edit(document.frmAttachCategory,'$id','$name')"?>"> 
				<?php echo $name?> 
			</a>
		</td>
		<td><a href='<?php echo $click_del?>'>
			<?php echo $trash?>
		</a></td>
	</tr>
<?php	
		echo $spliter;
	}
	echo $spliter;
	echo $spliter;
?>	
</table>
<br>
<table width="100%"  border="0">
  <tr>
    <td width="1%" nowrap><?php echo $sc_category_name?> : </td>
    <td>
		<input type="text" class="textbox" name="category"> 
		<input type="hidden" name="id">
	</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>
<?php 
	$click = "if (check_not_bland(document.frmCategory,Array(document.frmAttachCategory.category),Array('$sc_AskInput $sc_category_name'))) click_but(document.frmAttachCategory,'add',false)";
	$button = new button($cmdAdd, $click);
	echo '&nbsp;&nbsp;';
	$click = "if (check_not_bland(document.frmCategory,Array(document.frmAttachCategory.category),Array('$sc_AskInput $sc_category_name'))) click_but(document.frmAttachCategory,'update',false)";
	$button = new button($cmdCapnhat, $click);
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location = 'index.php'");
?>
	</td>
  </tr>
</table>
</form>

<script language="javascript" src="../../lib/action.js"></script>
<script language="javascript">
function set_edit(frm, id, name)
{
  frm.category.value = name;
  frm.id.value = id;
//  frm.parent_id.selectedIndex = parent;
/*  if (count_child > 0)
  	frm.parent_id.disabled = true;
  else
  	frm.parent_id.disabled = false;*/
}
</script>
