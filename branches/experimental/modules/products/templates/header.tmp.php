<table border="0" cellpadding="5" cellspacing="0">
	<tr>
<?php 
if ($GO_MODULES->write_permissions)
{
?>
	<td align="center"><a href="category.php">
		<img src="<?php echo $GO_THEME->images['product_category']?>" border="0"><br>
		<?php echo $sc_add_category;?>
	</a></td>
	<td align="center"><a href="attach_category.php">
		<img src="<?php echo $GO_THEME->images['accessory_category']?>" border="0"><br>
		<?php echo $sc_add_attach_category;?>
	</a></td>
	<td align="center"><a href="index.php?task=add">
		<img src="<?php echo $GO_THEME->images['new_product']?>" border="0"><br>
		<?php echo $sc_add_products;?>
	</a></td>
<?php
}
	if (count($_SESSION['cart']->items)>0) 
		echo '<td align="center"><a href="order.php?task=new"><img src="'.$GO_THEME->images['create_quotation'].'" border="0"><br>'.$sc_cart.'</a></td>';
?>
		<td align="center"><a href="order.php">
			<img src="<?php echo $GO_THEME->images['quotation']?>" border="0"><br>
			<?php echo $sc_order_list;?>
		</a></td>
	</tr>
</table>
<br>