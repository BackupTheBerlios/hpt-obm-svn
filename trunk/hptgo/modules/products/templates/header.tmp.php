<?php 
if ($GO_MODULES->write_permissions)
{
?>
	<a href="category.php"><?php echo $sc_add_category;?></a>&nbsp;&nbsp;&nbsp;
	<a href="attach_category.php"><?php echo $sc_add_attach_category;?></a>&nbsp;&nbsp;&nbsp;
	<a href="index.php?task=add"><?php echo $sc_add_products;?></a>&nbsp;&nbsp;&nbsp;
<?php
}
	if (count($_SESSION['cart']->items)>0) 
		echo '<a href="order.php?task=new">'.$sc_cart.'</a>&nbsp;&nbsp;&nbsp;';
?>
<a href="order.php"><?php echo $sc_order_list;?></a>&nbsp;&nbsp;&nbsp;
<br><br>
