<?php $excel_processor_url = "http://172.16.32.245:8081/subscribe/{$order['order_number']}+1";?>
<?php $ab->enable_contact_selector(); ?>
<form name="frmOrder" method="post" action="">
	<input name="task" type="hidden">
	<input name="close_win" type="hidden">
	<input name="txt_name" type="hidden">
	<input name="txt_id" type="hidden">		
	<input name="is_order" type="hidden" value="true">
	<table>
 	<tr>
		<td>
			<?php 
				if (empty($order['seller'])) $order['seller'] = $GO_SECURITY->user_id;
				$select = new select('user', 'frmOrder', 'seller', $order['seller']);
				$select->print_link($sc_employee);
			?>
		</td>
		<td><?php $select->print_field();?></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_order_number?></td>
		<td><input type="text" class="textbox" name="order_number" value="<?php echo $order['order_number']?>" readonly></td>
  	</tr>  
	<tr>
		<td><?php echo $sc_company?></td>
		<td><input type="text" class="textbox" name="company" value="<?php echo $order['company']?>"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_attn?></td>
		<td><input type="text" class="textbox" name="attn" value="<?php echo $order['attn']?>"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_fax?></td>
		<td><input type="text" class="textbox" name="fax" value="<?php echo $order['fax']?>" onKeyUp="inputNumber(this,'()-. ')"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_phone?></td>
		<td><input type="text" class="textbox" name="phone" value="<?php echo $order['phone']?>" onKeyUp="inputNumber(this,'()-. ')"></td>
  	</tr>  

	  	<tr>
		<td><?php echo $sc_sale_date?></td>
		<td>
			<?php 
			if (!isset($order['sale_date']))
				$today = get_today();
			else
				$today = db_date_to_date($order['sale_date']);
			$datepicker->print_date_picker('sale_date', $_SESSION['GO_SESSION']['date_format'], $today);
			?>
		</td>
  	</tr>
  	<tr>
		<td><?php echo $sc_valid_date?></td>
		<td>
			<?php 
			if (!isset($order['valid_date']))
				$today = get_today();
			else
				$today = db_date_to_date($order['valid_date']);
			$datepicker->print_date_picker('valid_date', $_SESSION['GO_SESSION']['date_format'], $today);
			?>
		</td>
  	</tr>
  </table>
  <br>
  <table width="70%"  border="1" cellspacing="0" cellpadding="0">
    <tr class="TableHead2" height="20">
      <td width="1%" align="center" nowrap>#</td>
      <td align="center" width="40%" nowrap><?php echo $sc_product_name?> </td>
      <td align="center" width="10%" nowrap><?php echo $sc_price?></td>
	  <td align="center" width="10%" nowrap><?php echo $sc_quantity?></td>
	  <td align="center" width="10%" nowrap><?php echo $sc_total?></td>
	  <td align="center" width="1%" nowrap>&nbsp;</td>
    </tr>
<?php
	$i=0;
	$ptotal = 0;
	
	$attach_cate = new products();
	$attach = new products();
	
	while ($pro_order->next_record())
	{
		$pprice = $pro_order->f('price');
		$pid = $pro_order->f('product_id');
		$apid = $pid;
		$pname = $pro_order->f('product_name');
//		if ($task == 'new')
//			$pquantity = $pitems[$pid];
//		else
			$pquantity = $order_quantity_arr[$apid.'0'];

		
		$click_del = "javascript:click_delete(document.frmOrder, $pid , \"$pname\" , \"".sprintf($sc_ConfirmDeleteProductFromCart, $pname)."\",\"_detail\")";
?>
    <tr>
      <td align="center"> <?php echo ++$i?> </td>
      <td> <?php echo '<b>'.$pname.'<b><br>';?> </td>
<?php
		$attach->get_order_product($pid);
		if ($attach->num_rows() == 0)
		{
?>	  
	  <td align="right"> 
	  	<input type="text" class="textbox" id="price<?php echo $i?>" name="price[]" style="text-align:right" value="<?php echo $pprice?>" onKeyUp="inputNumber(this,'.');<?php echo "CalcMoney(quantity".$i.",sum".$i.",this,sum,total)"?>;">
		<input type="hidden" name="product[]" value="<?php echo $pid?>">
		<input type="hidden" name="cate[]">
		<input type="hidden" name="attach[]">
	  </td>
      <td align="center"> <input type="text" class="textbox" id="quantity<?php echo $i?>" name="quantity[]" style="text-align:right" value="<?php echo $pquantity?>" onKeyUp="inputNumber(this,'');<?php echo "CalcMoney(this,sum".$i.",price".$i.",sum,total)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="sum" name="sum<?php echo $i?>" style="text-align:right" value="<?php echo $pprice*$pquantity?>" readonly> </td>
	  <td align="center" width="1%"><a href='<?php echo $click_del?>'><?php echo $trash?></a></td>
    </tr>	  
<?php 
			$ptotal += ($pprice*$pquantity);
		}
		else
		{
?>	  
	  <td align="right">&nbsp;</td>
      <td align="center">&nbsp;</td>
	  <td align="center">&nbsp;</td>
	  <td align="center" width="1%"><a href='<?php echo $click_del?>'><?php echo $trash?></a></td>
    </tr>	  
<?php 
		}
		$first = true;
		$j = -1;
		unset($cate_arr);
		unset($cate_id_arr);
		unset($text_arr);
		unset($value_arr);
		unset($price_arr);
		while ($attach->next_record())
		{
			if ($cate_arr[$j] != $attach->f('name'))
			{
				$j++;
				$cate_arr[] = $attach->f('name');
				$cate_id_arr[] = $attach->f('attach_cate_id');
				
				$text_arr[$j][] = $sc_no_select;
				$value_arr[$j][] = 0;
				$price_arr[$j][] = 0;
			}
			$text_arr[$j][] = $attach->f('attachment_name');
			$value_arr[$j][] = $attach->f('attachment_id');
			$price_arr[$j][] = $attach->f('attachment_price');
		}

		for ($j=0; $j<count($cate_arr); $j++)
		{
			$dropbox = new dropbox();
			$pricebox = new dropbox();
			
			$dropbox->add_arrays($value_arr[$j],$text_arr[$j]);
			
			for ($k=0; $k<count($price_arr[$j]); $k++)
				$pricebox->add_value($price_arr[$j][$k],sprintf("%s ",$price_arr[$j][$k]));
//$order_price_arr[$apid.$acate] = $pro->f('price_id');
//				$order_quantity_arr[$apid.$acate] = $pro->f('quantity_id');

			$pid = $value_arr[$j];
			$pprice = $order_price_arr[$apid.$cate_id_arr[$j]];
			$pquantity = $order_quantity_arr[$apid.$cate_id_arr[$j]];
			$ptotal += ($pprice*$pquantity);			
?>			
	<tr>
      <td align="center">&nbsp;  </td>
	  <td>
	  	&nbsp;&nbsp;-&nbsp;<?php echo $cate_arr[$j]; ?>&nbsp;&nbsp;&nbsp; 
		<?php $dropbox->print_dropbox('attach[]', $order_attach_arr[$apid.$cate_id_arr[$j]], 'onChange="document.frmOrder.price'.$i.$j.'.value=document.frmOrder.listprice'.$i.$j.'.options[this.selectedIndex].value;CalcMoney(quantity'.$i.$j.',sum'.$i.$j.',price'.$i.$j.',sum,total)"');?>
		<?php $pricebox->print_dropbox('listprice'.$i.$j,'',' style="visibility:hidden" ');?>
		<input type="hidden" name="product[]" value="<?php echo $pro_order->f('product_id')?>">
		<input type="hidden" name="cate[]" value="<?php echo $cate_id_arr[$j]?>">
	  </td>
	  <td align="right"> <input type="text" class="textbox" id="price<?php echo $i.$j?>" name="price[]" style="text-align:right" value="<?php echo $pprice?>" onKeyUp="inputNumber(this,'.');<?php echo "CalcMoney(quantity".$i.$j.",sum".$i.$j.",this,sum,total)"?>;"><input type="hidden" name="id[]" value="<?php echo $pid?>"><input type="hidden" name="id[]" value="<?php echo $pid?>"></td>
      <td align="center"> <input type="text" class="textbox" id="quantity<?php echo $i.$j?>" name="quantity[]" style="text-align:right" value="<?php echo $pquantity?>" onKeyUp="inputNumber(this,'');<?php echo "CalcMoney(this,sum".$i.$j.",price".$i.$j.",sum,total)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="sum" name="sum<?php echo $i.$j?>" style="text-align:right" value="<?php echo $pprice*$pquantity?>" readonly> </td>
	  <td align="center" width="1%"><a href='<?php //echo $click_del?>'><?php //echo $trash?></a>&nbsp;</td>
    </tr>
<?php 
		}
	}
?>	
    <tr class="TableHead2" height="20">
      <td colspan="4" align="center"> <?php echo $sc_sum?> </td>
	  <td align="center" width="1%"> <input type="text" class="textbox" name="total" style="text-align:right" value="<?php echo $ptotal?>" readonly> </td>
	  <td align="center" width="1%">*</td>
    </tr>
  </table>
</form>
<br><br>
<?php 
//	$click = "if (check_not_bland(document.frmOrder,Array(document.frmOrder.order_number, document.frmOrder.seller),Array('$sc_AskInput $sc_order_number', '$sc_AskInput $sc_employee'))) click_but(document.frmOrder,'save',true)";
//	$button = new button($cmdOk, $click);

	$click = "click_but(document.frmOrder,'clear',true)";
	$button = new button($sc_clear_cart, $click);

	echo '&nbsp;&nbsp;';
	$click = "if (check_not_bland(document.frmOrder,Array(document.frmOrder.order_number, document.frmOrder.seller),Array('$sc_AskInput $sc_order_number', '$sc_AskInput $sc_employee'))) click_but(document.frmOrder,'update',false)";
	$button = new button($cmdCapnhat, $click);

	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "document.location = 'index.php'");
	echo '&nbsp;&nbsp;';
	$button = new button($sc_export, "document.location = '$excel_processor_url'");

?>

<script language="javascript">
	function CalcMoney(amount, sum, price, sumArr, total)
	{
		sum.value = amount.value * price.value;

		if (sumArr.length==1)
			total.value = sumArr.value;
		else
		{
			for (m=0,i=0;i<sumArr.length;i++)
				m += parseInt(sumArr[i].value);
			total.value = m;
		}
	}
</script>	
<script language="javascript" src="../../lib/action.js"></script>
