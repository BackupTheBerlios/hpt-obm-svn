<?php
	$preload_files = 'modules/products/classes/products.class.php';
	require("../../Group-Office.php");
	require('../../lib/tkdlib.php');

	$GO_SECURITY->authenticate();
	$GO_MODULES->authenticate('products');
	require($GO_LANGUAGE->get_language_file('products'));

	$datepicker = new date_picker();
	$GO_HEADER['head'] = $datepicker->get_header();

	require($GO_THEME->theme_path."header.inc");

	$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
	$move_task_up = '<img src="'.$GO_THEME->images['task_up'].'" border="0">';
	$move_task_dn = '<img src="'.$GO_THEME->images['task_dn'].'" border="0">';
	$move_space = '<img src="'.$GO_THEME->images['task_space'].'" border="0" width="16" height="12">';
	$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

require('../addressbook/classes/addressbook.class.inc');
	$ab = new addressbook();

	$pro = new products();
	
	$task = $_REQUEST['task'];
	switch ($task)
	{
		case 'clear':
			 $_SESSION['cart']->cleanall();
		case 'close':
			if ($_POST['close_win']=='true') goURL('index.php');
		break;
		case 'update':
			$seller = $_POST['seller'];
			$order_number = $_POST['order_number'];
			$company = $_POST['company'];
			$attn = $_POST['attn'];
			$phone = $_POST['phone'];
			$fax = $_POST['fax'];
			$sale_date = date_to_db_date($_POST['sale_date']);
			$valid_date = date_to_db_date($_POST['valid_date']);
			$cc = $_POST['cc'];
			$subject = $_POST['subject'];
			$adjustment = $_POST['incdecquotation'];

			$product = $_POST['product'];
			$cate = $_POST['cate'];
			$attach = $_POST['attach'];
				
			$quantity = $_POST['quantity'];
			$price = $_POST['price'];
			$incdec = $_POST['incdec'];
			$VAT = $_POST['VAT'];
				
			if ($pro->update_order($seller, $order_number, $company, $attn, $cc, $subject, $phone, $fax, $sale_date, $valid_date, $adjustment, $product, $cate, $attach, $quantity, $price, $VAT, $incdec)) 
				goURL('order.php');
		break;
		case 'save':
		case 'apply':
			$quantity = $_POST['quantity'];
			$id = $_POST['id'];
			for ($i=0; $i<count($id); $i++)
				$_SESSION['cart']->setcalc($id[$i],$quantity[$i]);
			$_SESSION['cart']->cleanup();			

			if ($task == 'save') 
			{
				$seller = $_POST['seller'];
				$order_number = $_POST['order_number'];
				$company = $_POST['company'];
				$attn = $_POST['attn'];
				$phone = $_POST['phone'];
				$fax = $_POST['fax'];
				$sale_date = date_to_db_date($_POST['sale_date']);
				$valid_date = date_to_db_date($_POST['valid_date']);
				$cc = $_POST['cc'];
				$subject = $_POST['subject'];
				$adjustment = $_POST['incdecquotation'];
				
				$product = $_POST['product'];
				$cate = $_POST['cate'];
				$attach = $_POST['attach'];
				
				$quantity = $_POST['quantity'];
				$price = $_POST['price'];
				$incdec = $_POST['incdec'];
				$VAT = $_POST['VAT'];
				
				if ($pro->add_order($seller, $order_number, $company, $attn, $cc, $subject, $phone, $fax, $sale_date, $valid_date, $adjustment, $product, $cate, $attach, $quantity, $price, $VAT, $incdec)) 
				{
					$_SESSION['cart']->cleanall();
					break;
				}

				$_POST['close_win'] == 'f';
			}
		break;
		case 'delete_cart':
			$seller = $_POST['seller'];
			$order_number = $_POST['order_number'];
			$company = $_POST['company'];
			$attn = $_POST['attn'];
			$phone = $_POST['phone'];
			$fax = $_POST['fax'];
			$sale_date = date_to_db_date($_POST['sale_date']);
			$valid_date = date_to_db_date($_POST['valid_date']);
			$_SESSION['cart']->remove($_POST['txt_id']);
		case 'new':
			if ($_SESSION['cart']->itemcount() == 0) goURL('index.php');
			
			$pitems = $_SESSION['cart']->items;
			$pro->get_buy_products($_SESSION['cart']->get_productid_list());
			require('templates/edit_order.tmp.php');
		break;
		case 'delete_detail':
			if ($pro->delete_product_from_order($_POST['order_number'],$_POST['txt_id']))
			{
				$pro->get_orders();
				require('templates/list_order.tmp.php');
				break;
			}
			$_POST['id'] = $_POST['order_number'];
		case 'edit':
			$pro->get_entire_order($_POST['id'], $order);
			while($pro->next_record())
			{
				$apid = $pro->f('product_id');
				$acate = $pro->f('attach_cate_id');
				$order_attach_arr[$apid.$acate] = $pro->f('attach_id');
				$order_price_arr[$apid.$acate] = $pro->f('price');
				$order_quantity_arr[$apid.$acate] = $pro->f('quantity');
				$order_discount_arr[$apid.$acate] = $pro->f('discount');
				$order_VAT_arr[$apid.$acate] = $pro->f('VAT');
			}
			
			$pro_order = new products();
			$list_id = $pro_order->get_order_productid_list($_POST['id']);
			$pro_order->get_buy_products($list_id);

			require('templates/edit_update_order.tmp.php');			
		break;
		case 'delete':
			$pro->delete_order($_POST['id']);
		default:
			$pro->get_orders(false,'',$_POST['sort_fld'],$_POST['direction']);
			require('templates/list_order.tmp.php');
	}
	if ($_POST['close_win']=='true') goURL('order.php');

	require($GO_THEME->theme_path."footer.inc"); 		
?>
