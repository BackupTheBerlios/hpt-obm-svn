<?php $ab->enable_contact_selector(); ?>
<form name="frmOrder" method="post" action="">
	<input name="task" type="hidden">
	<input name="close_win" type="hidden">
	<input name="txt_name" type="hidden">
	<input name="txt_id" type="hidden">	
	<input name="is_order" type="hidden" value="true">
	<table>
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
		<td><input type="text" class="textbox" name="order_number" value="<?php echo $order['order_number']?>" style="width:300px"></td>
  	</tr>  
	<tr>
		<td><?php echo $sc_company?></td>
		<td><input type="text" class="textbox" name="company" value="<?php echo $order['company']?>" style="width:300px"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_attn?></td>
		<td><input type="text" class="textbox" name="attn" value="<?php echo $order['attn']?>" style="width:300px"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_fax?></td>
		<td><input type="text" class="textbox" name="fax" value="<?php echo $order['fax']?>" onKeyUp="inputNumber(this,'()-. ')" style="width:300px"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_phone?></td>
		<td><input type="text" class="textbox" name="phone" value="<?php echo $order['phone']?>" onKeyUp="inputNumber(this,'()-. ')" style="width:300px"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_cc?></td>
		<td><input type="text" class="textbox" name="cc" value="<?php echo $order['cc']?>" style="width:300px"></td>
  	</tr>  
 	<tr>
		<td><?php echo $sc_subject?></td>
		<td><input type="text" class="textbox" name="subject" value="<?php echo $order['subject']?>" style="width:300px"></td>
  	</tr>  
  </table>
  <br>
  <table width="30%"  border="1" cellspacing="0" cellpadding="0">
    <tr class="TableHead2" height="20">
      <td width="1%" align="center" nowrap>#</td>
      <td align="center" width="99%" nowrap><?php echo $sc_product_name?> </td>
      <td align="center" width="1%" nowrap><?php echo $sc_price?></td>
	  <td align="center" width="1%" nowrap><?php echo $sc_quantity?></td>
	  <td align="center" width="1%" nowrap><?php echo $sc_total?></td>
	  <td align="center" width="1%" nowrap><?php echo $sc_inc_dec?></td>
	  <td align="center" width="1%" nowrap><?php echo $sc_VAT?></td>
	  <td align="center" width="1%" nowrap><?php echo $sc_sum?></td>
	  <td align="center" width="1%" nowrap>&nbsp;</td>
    </tr>
<?php
	$i=0;
	$ptotal = 0;
	
	$attach_cate = new products();
	$attach = new products();
	
	while ($pro->next_record())
	{
		$pprice = $pro->f('price');
		$pid = $pro->f('product_id');
		$pname = $pro->f('product_name');

		if ($task == 'new')
			$pquantity = $pitems[$pid];
		else
			$pquantity = $pro->f('quantity');
		
		$click_del = "javascript:click_delete(document.frmOrder, $pid , \"$pname\" , \"".sprintf($sc_ConfirmDeleteProductFromCart, $pname)."\", \"_cart\")";
?>
    <tr>
      <td align="center"> <?php echo ++$i?> </td>
      <td nowrap> <?php echo '<b>'.$pname.'<b><br>';?> </td>
<?php
		$attach->get_order_product($pid);
		if ($attach->num_rows() == 0)
		{
			if (empty($pquantity)) $pquantity =1;
			$ptotal += ($pprice*$pquantity);
			$pVAT = 0;
			$pamount = $ptotal + ($ptotal * $pVAT / 100);
			$psumamount += $pamount;
?>	  
	  <td align="right"> 
        <input type="text" class="textbox" id="price<?php echo $i?>" name="price[]2" style="text-align:right; width:90px" value="<?php echo $pprice?>" readonly>		
        <input type="hidden" name="product[]" value="<?php echo $pid?>">
		<input type="hidden" name="cate[]">
	  <input type="hidden" name="attach[]">	  </td>
      <td align="center"> <input type="text" class="textbox" id="quantity<?php echo $i?>" name="quantity[]" style="text-align:right; width:60px" value="<?php echo $pquantity?>" onKeyUp="inputNumber(this,'');<?php echo "CalcMoney(price".$i.",quantity".$i.",sum".$i.",incdec".$i.",VAT".$i.",amount".$i.",sum,total,amount,sumamount)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="sum" name="sum<?php echo $i?>" style="text-align:right; width:100px" value="<?php echo $pprice*$pquantity?>" readonly> </td>
	  <td align="center"> <input type="text" class="textbox" id="incdec<?php echo $i?>" name="incdec[]" style="text-align:right; width:60px" value="0" onKeyUp="inputNumber(this,'%.-');<?php echo "CalcMoney(price".$i.",quantity".$i.",sum".$i.",incdec".$i.",VAT".$i.",amount".$i.",sum,total,amount,sumamount)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="VAT<?php echo $i?>" name="VAT[]" style="text-align:right; width:60px" value="<?php echo $pVAT?>" onKeyUp="inputNumber(this,'.');<?php echo "CalcMoney(price".$i.",quantity".$i.",sum".$i.",incdec".$i.",VAT".$i.",amount".$i.",sum,total,amount,sumamount)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="amount" name="amount<?php echo $i?>" style="text-align:right" value="<?php echo $pamount?>" readonly> </td>
	  <td align="center" width="1%"><a href='<?php echo $click_del?>'><?php echo $trash?></a></td>
    </tr>	  
<?php 
		}
		else
		{
?>	  
	  <td align="right">&nbsp;</td>
      <td align="center">&nbsp;</td>
	  <td align="center">&nbsp;</td>
	  <td align="center">&nbsp;</td>
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
			$VAT_arr[$j][] = $attach->f('attachment_VAT');
		}
		for ($j=0; $j<count($cate_arr); $j++)
		{
			$dropbox = new dropbox();
			$pricebox = new dropbox();
			
			$dropbox->add_arrays($value_arr[$j],$text_arr[$j]);
			
			for ($k=0; $k<count($price_arr[$j]); $k++)
				$pricebox->add_value($price_arr[$j][$k],sprintf("%s ",$price_arr[$j][$k]));

			$pid = $value_arr[$j];
			$pprice = $price_arr[$j][0];
			$pVAT = $VAT_arr[$j][0];
			$pamount = $pprice * $pquantity + ($pprice * $pquantity * $pVAT / 100);			
			$psumamount += $pamount;
			$ptotal += ($pprice * $pquantity);
?>			
	<tr>
      <td align="center">&nbsp;  </td>
	  <td nowrap>
	  	&nbsp;&nbsp;-&nbsp;<?php echo $cate_arr[$j]?>&nbsp;&nbsp;&nbsp;
		<?php $dropbox->print_dropbox('attach[]','', 'onChange="document.frmOrder.price'.$i.$j.'.value=document.frmOrder.listprice'.$i.$j.'.options[this.selectedIndex].value;CalcMoney(price'.$i.$j.',quantity'.$i.$j.',sum'.$i.$j.',incdec'.$i.$j.',VAT'.$i.$j.',amount'.$i.$j.',sum,total,amount,sumamount)"');?>
		<?php $pricebox->print_dropbox('listprice'.$i.$j,'',' style="visibility:hidden" ');?>
		<input type="hidden" name="product[]" value="<?php echo $pro->f('product_id')?>">
		<input type="hidden" name="cate[]" value="<?php echo $cate_id_arr[$j]?>">
	  </td>
	  <td align="right"> <input type="text" class="textbox" id="price<?php echo $i.$j?>" name="price[]" style="text-align:right; width:90px"  value="<?php echo $pprice?>" readonly><input type="hidden" name="id[]" value="<?php echo $pid?>"></td>
      <td align="center"> <input type="text" class="textbox" id="quantity<?php echo $i.$j?>" name="quantity[]" style="text-align:right; width:60" value="<?php echo $pquantity?>" onKeyUp="inputNumber(this,'');<?php echo "CalcMoney(price".$i.$j.",quantity".$i.$j.",sum".$i.$j.",incdec".$i.$j.",VAT".$i.$j.",amount".$i.$j.",sum,total,amount,sumamount)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="sum" name="sum<?php echo $i.$j?>" style="text-align:right; width:100px" value="<?php echo $price_arr[$j][0]*$pquantity?>" readonly> </td>
	  <td align="center"> <input type="text" class="textbox" id="incdec<?php echo $i.$j?>" name="incdec[]" style="text-align:right; width:60px" value="0" onKeyUp="inputNumber(this,'%.-');<?php echo "CalcMoney(price".$i.$j.",quantity".$i.$j.",sum".$i.$j.",incdec".$i.$j.",VAT".$i.$j.",amount".$i.$j.",sum,total,amount,sumamount)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="VAT<?php echo $i.$j?>" name="VAT[]" style="text-align:right; width:60px" value="<?php echo $pVAT?>" onKeyUp="inputNumber(this,'.');<?php echo "CalcMoney(price".$i.$j.",quantity".$i.$j.",sum".$i.$j.",incdec".$i.$j.",VAT".$i.$j.",amount".$i.$j.",sum,total,amount,sumamount)"?>;"> </td>
	  <td align="center"> <input type="text" class="textbox" id="amount" name="amount<?php echo $i.$j?>" style="text-align:right" value="<?php echo $pamount?>" readonly> </td>
	  <td align="center" width="1%"><a href='<?php //echo $click_del?>'><?php //echo $trash?></a>&nbsp;</td>
    </tr>
<?php 
		}
	}
?>	
    <tr class="TableHead2" height="20">
      <td colspan="4" align="center"> <?php echo $sc_sum?> </td>
	  <td align="center" width="1%"> <input type="text" class="textbox" name="total" style="text-align:right; width:100px" value="<?php echo $ptotal?>" readonly> </td>
	  <td colspan="2" align="center" width="1%">&nbsp;</td>
	  <td align="center" width="1%"> <input type="text" class="textbox" name="sumamount" style="text-align:right" value="<?php echo $psumamount?>" readonly> </td>
	  <td align="center" width="1%">*</td>
    </tr>
    <tr>
      <td colspan="7" align="right"> <?php echo $sc_inc_dec_quotation?>&nbsp;&nbsp; </td>
	  <td align="center" width="1%"> <input type="text" class="textbox" name="incdecquotation" style="text-align:right" value="0" onKeyUp="inputNumber(this,'%.-');<?php echo "CalcQuotationAmount()"?>;"> </td>
	  <td align="center" width="1%">*</td>
    </tr>
    <tr class="TableHead2" height="20">
      <td colspan="7" align="right"> <?php echo $sc_amount_quotation?>&nbsp; </td>
	  <td align="center" width="1%"> <input type="text" class="textbox" name="quotationamount" style="text-align:right" value="<?php echo $psumamount?>" readonly> </td>
	  <td align="center" width="1%">*</td>
    </tr>
  </table>
</form>
<br><br>
<?php 
	$click = "click_but(document.frmOrder,'clear',true)";
	$button = new button($sc_clear_cart, $click);

	echo '&nbsp;&nbsp;';
	$click = "if (check_not_bland(document.frmOrder,Array(document.frmOrder.order_number, document.frmOrder.seller),Array('$sc_AskInput $sc_order_number', '$sc_AskInput $sc_employee'))) click_but(document.frmOrder,'save',true)";
	$button = new button($cmdOk, $click);

//	echo '&nbsp;&nbsp;';
//	$click = "if (check_not_bland(document.frmOrder,Array(document.frmOrder.order_number, document.frmOrder.seller),Array('$sc_AskInput $sc_order_number', '$sc_AskInput $sc_employee'))) click_but(document.frmOrder,'apply',false)";
//	$button = new button($cmdApply, $click);

	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "document.location = 'index.php'");
?>

<script language="javascript">
	function CalcQuotationAmount()
	{
		var frm = document.frmOrder;
		
		if (frm.incdecquotation.value.lastIndexOf('%')>-1)
		{	
			inc = frm.incdecquotation.value.replace('%','');
			frm.quotationamount.value = frm.sumamount.value*1 + frm.sumamount.value * inc / 100;
		}
		else 
			frm.quotationamount.value = frm.sumamount.value*1 + frm.incdecquotation.value*1;
	}

	function CalcMoney(quality, price, sum,  incdec, VAT, amount, sumArr, total, amountArr, sumamount)
	{
		sum.value = quality.value * price.value;

		if (incdec.value.lastIndexOf('%')>-1)
		{	
			inc = incdec.value.replace('%','');
			amount.value = sum.value*1 + sum.value * inc / 100 + (sum.value*1 + sum.value * inc / 100) * VAT.value / 100;
		}
		else 
			amount.value = sum.value*1 + incdec.value*1 + (sum.value*1 + incdec.value*1) * VAT.value / 100;

		if (sumArr.length==1)
			total.value = sumArr.value;
		else
		{
			for (m=0,i=0;i<sumArr.length;i++)
				m += sumArr[i].value*1;
			total.value = m;
		}
		
		if (amountArr.length==1)
			sumamount.value = amountArr.value;
		else
		{
			for (m=0,i=0;i<amountArr.length;i++)
				m += amountArr[i].value*1;
			sumamount.value = m;
		}
		
		CalcQuotationAmount();
	}
</script>	
<script language="javascript" src="../../lib/action.js"></script>