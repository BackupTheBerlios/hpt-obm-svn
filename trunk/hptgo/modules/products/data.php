<?php
require_once('../../Group-Office.php');
require_once('classes/products.class.php');
$pro = new products();

$order_id = $_REQUEST['order_id'];
$pro->get_entire_order($order_id,$order);

$to = $order['company'];
$attn = $order['attn'];
$cc = '';
$fax = $order['fax'];
$tel = $order['phone'];
$subj = '';
$effective = $order['sale_date'];
$quotation = $order['valid_date'];
$row_count = 0;			// initial value

while($pro->next_record())
{
  $apid = $pro->f('product_id');
  $acate = $pro->f('attach_cate_id');
  $order_attach_arr[$apid.$acate] = $pro->f('attach_id');
  $order_price_arr[$apid.$acate] = $pro->f('price');
  $order_quantity_arr[$apid.$acate] = $pro->f('quantity');
}

$pro_order = new products();
$list_id = $pro_order->get_order_productid_list($order_id);
$pro_order->get_buy_products($list_id);


$i=0;
$ptotal = 0;
	
$attach_cate = new products();
$attach = new products();

while ($pro_order->next_record()) {
  // price
  $pprice = $pro_order->f('price');
  // product_id
  $pid = $pro_order->f('product_id');
  // attachment_id
  $apid = $pid;
  // product name
  $pname = $pro_order->f('product_name');
  // quantity
  $pquantity = $order_quantity_arr[$apid.'0'];
  $product = array('id' => 0,
		   'pn' => $pro_order->f('part_number'),
		   'description' => $pro_order->f('description'),
		   'uprice' => $pro_order->f('price'),
		   'quantity' => $pquantity,
		   'vat' => $pro_order->f('VAT')
		   );
  $productlist[] = $product;

  /*
  $attach->get_order_product($pid);
  if ($attach->num_rows() == 0) {
    $ptotal += ($pprice*$pquantity);
  }

  $first = true;
  $j = -1;
  unset($cate_arr);
  unset($cate_id_arr);
  unset($text_arr);
  unset($value_arr);
  unset($price_arr);
  while ($attach->next_record()) {
    if ($cate_arr[$j] != $attach->f('name')) {
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

  for ($j=0; $j<count($cate_arr); $j++) {
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
  }
  */
}

$row_count = count($productlist);
$info = array($to,$attn,$cc,$fax,$tel,$subj,$effective,$quotation,$row_count);
echo "To:,Attn:,C/C:,Fax,Tel,Subj,Effective,Quotation,Count\r\n";
echo implode(',',$info)."\r\n";
echo "Index,P/N,Description,U/Price,Q'ty,VAT\r\n";
$counter = 1;
foreach ($productlist as $product) {
  $product['id'] = $counter++;
  echo implode(',',$product)."\r\n";
}
?>
