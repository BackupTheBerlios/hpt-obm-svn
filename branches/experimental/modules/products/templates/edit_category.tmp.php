<?php 
	if (!$GO_MODULES->write_permissions) goURL('index.php');
?>

<form name="frmCategory" method="post">
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
	$pro2 = new products();
	
	if ($pro->num_rows()>0) $pro->seek();
	$i=0;
	while ($pro->next_record())
	{
		$id = $pro->f('category_id');
		$name = $pro->f('category_name');
		
		$pro2->get_categories($pro->f('category_id'));
		$count = $pro2->num_rows();
		
		$click_del = "javascript:click_del(document.frmCategory, $id , \"$name\" , \"".sprintf($count==0?$sc_ConfirmDeleteCategory2:$sc_ConfirmDeleteCategory, $name)."\")";
//			<a href="<?php echo "javascript:set_edit(document.frmCategory,'$id','$name',0,'$count')"  "> 		
?>	
	<tr class="HiLi">
		<td align="center"> <?php echo ++$i;?> </td>
		<td nowrap>
			<a href="<?php echo "javascript:click_txt(document.frmCategory,'edit',document.frmCategory.id,'$id','$count')"?>"> 
				<?php echo $name?> 
			</a>
		</td>
		<td><a href='<?php echo $click_del?>'>
			<?php echo $trash?>
		</a></td>
	</tr>
<?php
		echo $spliter;
		$i2=0;
		while ($pro2->next_record())
		{
				$id = $pro2->f('category_id');
				$name = $pro2->f('category_name');
				$parent = $pro2->f('parent_id');
				$click_del = "javascript:click_del(frmCategory, $id , \"$name\" , \"".sprintf($sc_ConfirmDeleteCategory2, $name)."\")";
//	<a href="<?php echo "javascript:set_edit(document.frmCategory,'$id','$name','$i',0)"  "> 				
?>
	<tr>
		<td align="center">  </td>
		<td nowrap> 
			<?php echo ++$i2;?>&nbsp;&nbsp;
			<a href="<?php echo "javascript:click_txt(document.frmCategory,'edit', document.frmCategory.id, '$id', 'false')"?>"> 
				<?php echo $name?> 
			</a>
		</td>
		<td><a href='<?php echo $click_del?>'><?php echo $trash?></a></td>
	</tr>
<?php	
			echo $spliter;
		}
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
		<input type="text" class="textbox" name="category" value="<?php echo $edit_name?>"> 
		<?php $dropbox->print_dropbox('parent_id',$edit_parent, $disabled);?>
		<input type="hidden" name="id" value="<?php echo $edit_id?>">
	</td>
  </tr>
  <tr>
    <td width="1%" nowrap><?php echo $sc_be_attach?> : </td>  
  	<td>
<?php
	$choice->print_choice_list('frmCategory','template_id');
	$choice->select_all();
?>	
  	</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>
<?php 
	$click = "if (check_not_bland(document.frmCategory,Array(document.frmCategory.category),Array('$sc_AskInput $sc_category_name'))) { ".$choice->onSubmit_event()." click_but(document.frmCategory,'add',true);}";
	$button = new button($cmdAdd, $click);
	echo '&nbsp;&nbsp;';
	$click = "if (check_not_bland(document.frmCategory,Array(document.frmCategory.category),Array('$sc_AskInput tên Danh mục'))) { ".$choice->onSubmit_event()." click_but(document.frmCategory,'update',false);}";
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
function set_edit(frm, id, name, parent, count_child)
{
  frm.category.value = name;
  frm.id.value = id;
  frm.parent_id.selectedIndex = parent;

  if (count_child > 0)
  	frm.parent_id.disabled = true;
  else
  	frm.parent_id.disabled = false;
}
</script>
