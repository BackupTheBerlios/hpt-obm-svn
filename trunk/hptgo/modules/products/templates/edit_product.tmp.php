<?php 
	if (!$GO_MODULES->write_permissions) goURL('index.php');
?>

<form name="frmProduct" method="post" enctype="multipart/form-data">
	<input name="id" type="hidden" value="<?php echo $_POST['id']?>">
	<input name="task" type="hidden">
	<input name="close_win" type="hidden">
<table width="100%"	border="0" cellspacing="0" cellpadding="0">
<tr>
	<td width="1%" nowrap>
	<table width="100%" border="0" cellspacing="0" cellpadding="0">
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_product_name?> :&nbsp;</td>
	    <td><input type="text" class="textbox" name="product" value="<?php echo $name?>"></td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_part_number?> : &nbsp;</td>
	    <td><input type="text" class="textbox" name="part_number" value="<?php echo $part_number?>"></td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_price?> : &nbsp;</td>
	    <td><input type="text" class="textbox" name="price" value="<?php echo $price?>" onKeyUp="javascript:inputNumber(this,'.')"></td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_VAT?> : &nbsp;</td>
	    <td><input type="text" class="textbox" name="VAT" value="<?php echo $VAT?>" onKeyUp="javascript:inputNumber(this,'.')"></td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_be_attachment?> : &nbsp;</td>
	    <td>
		<input type="checkbox" name="is_attach" <?php echo $is_attachment==(bool)$pro->is_attach?'checked':''?> onClick="click_but(document.frmProduct,'<?php echo $change_case?>','')">
		</td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_category?> : &nbsp;</td>
	    <td>
<?php
/*	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_image?> : &nbsp;</td>
	    <td>
			<input type="hidden" name="oldimage" value="<?php echo $image?>">
			<input type="file" name="image" class="textbox" onChange="frmProduct.img.src = this.value; oldimage.value = '##change##'">
		</td>
	  </tr>*/

	$catedrop->print_dropbox('category_id',$category_id, 'onChange=click_but(document.frmProduct,"'.$change_case.'","")');
?>		
		</td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_information?> : &nbsp;</td>
	    <td><textarea name="description" style="height:70;width:50%"><?php echo $description?></textarea></td>
	  </tr>
	  <tr>
    	<td width="1%" align="right" nowrap><?php echo $sc_warranty?> : &nbsp;</td>
	    <td><textarea name="warranty" style="height:70;width:50%"><?php echo $warranty?></textarea></td>
	  </tr>

<?php
	$choice = new choice_list();
if (!$is_attachment)
{
	$pro2 = new products();
	$pro2->get_attach_categories($category_id,true);
	while ($pro2->next_record())
	{
?>
	  <tr>
	  	<td width="1%" align="right" nowrap><?php echo $pro2->f('name')?> : &nbsp;</td>
		<td>
<?php
	$choice->clear();
	$cate_id = $pro2->f('id');
	$pro->get_attachments($cate_id);

	while ($pro->next_record())
	{
		if (isset($list_attach[$cate_id]))
			if (in_array($pro->f('product_id'),$list_attach[$cate_id]))
				$choice->add_option($pro->f('product_name'),$pro->f('product_id'),true);
			else
				$choice->add_option($pro->f('product_name'),$pro->f('product_id'));
		else
			$choice->add_option($pro->f('product_name'),$pro->f('product_id'));
	}
	$choice->print_choice_list('frmProduct','attach_'.$pro2->f('id'));
?>		
			<input type="hidden" name="attachment_categories[]" value="<?php echo $pro2->f('id')?>">
		</td>
	  </tr>
<?php 
	}
}
	$choice->select_all();	
?>	  
	  <tr>
    	<td>&nbsp;</td>
	    <td><br>
<?php 
	if ($task!='add' && $task!='apply' && $task!='add_change')
	{
		$click = "if (check_not_bland(document.frmProduct,Array(document.frmProduct.product, document.frmProduct.price),Array('$sc_AskInput $sc_product_name','$sc_AskInput $sc_part_number', '$sc_AskInput $sc_price'))) {".$choice->onSubmit_event()." click_but(document.frmProduct,'update',true)}";
		$button = new button($cmdCapnhat, $click);
	}
	else
	{
		$click = "if (check_not_bland(document.frmProduct,Array(document.frmProduct.product, document.frmProduct.price),Array('$sc_AskInput $sc_product_name', '$sc_AskInput $sc_price'))) {".$choice->onSubmit_event()." click_but(document.frmProduct,'save',true);}";
		$button = new button($cmdOk, $click);
		echo '&nbsp;&nbsp;';
		$click = "if (check_not_bland(document.frmProduct,Array(document.frmProduct.product, document.frmProduct.price),Array('$sc_AskInput $sc_product_name', '$sc_AskInput $sc_price'))) {".$choice->onSubmit_event()." click_but(document.frmProduct,'apply',false);}";
		$button = new button($cmdApply, $click);
	}
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location ='index.php'");
?>
		</td>
	  </tr>
	</table>
	
	</td>
	<td valign="top">
	</td>
</tr>	
</form>

<script language="javascript" src="../../lib/action.js"></script>
