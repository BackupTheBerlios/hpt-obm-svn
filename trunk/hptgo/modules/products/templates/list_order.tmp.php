<form name="frmOrderList" method="post">
<input type="hidden" name="task">
<input name="txt_name" type="hidden">
<input type="hidden" name="id">
<input type="hidden" name="close_win">
<?php //echo 'Giá tiền :'.$_SESSION['cart']->total.'  - Số lượng :'.$_SESSION['cart']->itemcount(); ?>
<table width="100%"  border="0" cellspacing="0" cellpadding="0">
  <tr class="TableHead2" height="20">
    <td width="20" align="center">#</td>
	<td><?php echo $sc_order_number?></td>
    <td><?php echo $sc_client?></td>
	<td><?php echo $sc_employee?></td>
	<td>&nbsp;</td>
  </tr>
<?php
	$i=0;
	while ($pro->next_record())
	{
		$id = $pro->f('order_number');
		$click_del = "javascript:click_del(document.frmOrderList, \"$id\" , \"$id\" , \"".sprintf($sc_ConfirmDeleteOrder, $id)."\")";
//		<input type="checkbox" name="order_id[]" value="checkbox">
		$profile = $GO_USERS->get_user($pro->f('seller'));
?>  
  <tr>
  	<td width="20" align="center"><?php echo ++$i;?></td>
    <td><a href="javascript:click_txt(document.frmOrderList,'edit',document.frmOrderList.id,'<?php echo $id?>',false)"><?php echo $id?></a></td>
    <td><?php echo $pro->f('attn')?></td>
	<td><?php echo $profile['last_name'].' '.$profile['middle_name'].' '.$profile['first_name']?></td>
    <td><a href='<?php echo $click_del?>'><?php if ($GO_MODULES->write_permissions) echo $trash?></a>&nbsp;</td>
  </tr>
<?php
		echo $spliter;
	}
	echo $spliter.$spliter;
?>  
</table>
<br>
<?php
	$button = new button($cmdClose, "javascript:click_but(document.frmOrderList,'close',true)");
?>
</form>
<script language="javascript" src="../../lib/action.js"></script>