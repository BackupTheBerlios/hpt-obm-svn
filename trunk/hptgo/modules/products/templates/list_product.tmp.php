<form name="frmProList" method="post" action="">
<input type="hidden" name="task">
<input type="hidden" name="id">
<input type="hidden" name="list_id" value="<?php echo $_POST['list_id']?>">
<input type="hidden" name="txt_name">
<input type="hidden" name="close_win">
<?php echo $sc_amount.' :'.$_SESSION['cart']->total.'  - '.$sc_quantity.' :'.$_SESSION['cart']->itemcount(); ?>
<table width="100%"  border="0" cellspacing="0" cellpadding="5">
<tr>
	<td valign="top" width="15%">
	
	<table width="100%"  border="0" cellspacing="0" cellpadding="0">
	  <tr class="TableHead2" height="20">
		<td nowrap><?php echo $sc_category?></td>
	  </tr>
	
<?php 
	$pro->get_categories(0);
	$num = $pro->num_rows();
		
	$attach = new products();
	$attach->get_attach_categories();
	$att_num = $attach->num_rows();
	
	for ($i=1; $pro->next_record(); $i++)
	{
		$id = $pro->f('category_id');
		$click = "javascript:click_txt(document.frmProList,'expand',document.frmProList.id,'$id','')";

		$pro2 = new products();
		$pro2->get_categories($id);
		$num2 = $pro2->num_rows();

		
		if ($num2 > 0)
		{
			$no_exp = true;
			if (count($exp)>0)
			{
				if (in_array($id,$exp))
				{
					$img = $image['mnode'];
					if ($i==$num && $att_num==0) $img = $image['mlastnode'];
					$a = "<a href=\"javascript:click_txt(document.frmProList,'shrink',document.frmProList.id,'".$pro->f('category_id')."','')\">";
					$aE = '</a>';
				}
				else
				{
					$img = $image['pnode'];
					if ($i==$num && $att_num==0) $img = $image['plastnode'];
					$a = "<a href=\"$click\">";
					$aE = '</a>';
				}
			}
			else
			{
				$img = $image['pnode'];
				if ($i==$num && $att_num==0) $img = $image['plastnode'];
				$a = "<a href=\"$click\">";
				$aE = '</a>';
			}
		}
		else
		{
			$img = $image['emptynode'];
			if ($i==$num && $att_num==0) $img = $image['emptylastnode'];
			$a = '';
			$aE = '';
		}
		
		echo "<tr><td nowrap>$a $img ".$pro->f('category_name')." $aE</td></tr>";

		if (count($exp)>0)
			if (in_array($id,$exp))
			{
				for ($i2=1; $pro2->next_record(); $i2++)
				{
					$img = $image['emptynode'];
					if ($i2==$num2) $img = $image['emptylastnode'];
					$img2 = $image['vertline'];
					if ($i==$num && $att_num==0) $img2=$image['blank'];

					$id = $pro2->f('category_id');
					$click = "javascript:click_txt(document.frmProList,\"list\",document.frmProList.list_id,\"$id\",\"\")";
					echo "<tr><td nowrap><a href='$click'>$img2$img".$pro2->f('category_name')." </a></td></tr>";
				}
			}
	}
	
	
	for ($i=1; $attach->next_record(); $i++)
	{
		$img = $image['emptynode'];
//		alert($attach->num_rows());
		if ($i==$att_num) $img = $image['emptylastnode'];
		$a = '';
		$aE = '';
		
		$click = "javascript:click_txt(document.frmProList,\"list_attach\",document.frmProList.list_id,\"".$attach->f('id')."\",\"\")";
		echo "<tr><td nowrap><a href='$click'>$img".$attach->f('name')." </a></td></tr>";
	}
?>
	</table>
	
	</td>
	<td valign="top" width="85%">

<table width="100%"  border="0" cellspacing="0" cellpadding="0">
  <tr class="TableHead2" height="20">
    <td width="1">&nbsp;</td>
	<td><?php echo $sc_part_number?></td>
	<td><?php echo $sc_product_name?></td>
    <td><?php echo $sc_category?></td>
	<td><?php echo $sc_price?></td>
    <td>&nbsp;</td>
	<td>&nbsp;</td>
  </tr>
<?php
	if ($task == 'list_attach')
	{
		$pro->get_attachments($_POST['list_id']);
		$task = 'edit_attach';
	}
	else
	{
		if (!empty($_POST['list_id']))
			$pro->get_products($_POST['list_id'],true);
		else
			$pro->get_products(-1,false,true);
		$task = 'edit';
	}
	
	while ($pro->next_record())
	{
		$id = $pro->f('product_id');
		$name = $pro->f('product_name');

		$cate = $pro->f('category_name');
		if (empty($cate)) 
			$cate=$pro->f('attach_cate');
		
		$click_del = "javascript:click_del(document.frmProList, $id , \"$name\" , \"".sprintf($sc_ConfirmDeleteProduct, $name)."\")";
?>  
  <tr>
  	<td width="1"><input type="checkbox" name="buy_id[]" value="<?php echo $id?>"> </td>
	<td><?php echo $pro->f('part_number')?></td>
    <td><a href="javascript:click_txt(document.frmProList,'<?php echo $task?>',document.frmProList.id,'<?php echo $id?>','')"><?php echo $name?> </a></td>
    <td><?php echo $cate?> </td>
	<td><?php echo $pro->f('price')?> </td>
	<td><a href="javascript:click_txt(document.frmProList,'buy',document.frmProList.id,'<?php echo $id?>','')"><b><?php echo $sc_buy?></b></a></td>
    <td><a href='<?php echo $click_del?>'><?php if ($GO_MODULES->write_permissions) echo $trash ?></a>&nbsp;</td>
  </tr>
<?php
		echo $spliter;
	}
	echo $spliter.$spliter;
?>  
	<tr><td colspan="99">&nbsp;</td></tr>
	<tr>
	  <td colspan="99"><a href="javascript:click_but(document.frmProList,'buy_all',false)"><b><?php echo $sc_buy_all?></b></a></td>
	</tr>
</table>

</td>
</tr>
</table>
</form>
<script language="javascript" src="../../lib/action.js"></script>