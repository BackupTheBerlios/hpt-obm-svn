<?php
	/* Class name  : products
	*  Last update : 13 September 2004
	*  This class was written and developed by Tran Kien Duc(trankien_duc@yahoo.com)
	*  Copyright 2004 TranKienDuc
	*/ 

	class products extends db 
	{
		var $cate_tab = 'sc_categories';
		var $cate_id_fie = 'category_id';
		
		var $no_attach = 0;
		var $is_attach = 1;
		var $have_attach = 2;
		
		var $in_order_detail = 100;
		var $over_size = 200;
		var $err_duplicate = 300;
		var $err_child_of_itsself = 301;
		
		var $upload_dir = 'upload';
		var $maxsize = 500000;
		
		function products()
		{
			$this->db();
		}
		
//The methods for common
	function check_duplicate($field_name, $table_name, $value, $id_name='', $id='')
	{
		$sql = "SELECT *
				FROM $table_name
				WHERE $field_name = '$value' ";
		if ($id_name!='') $sql .= "AND $id_name <> '$id'";
		if (!$this->query($sql)) return true;

		if  ($this->num_rows() > 0) return true;
		return false;
	}
		
// The methods for categories
		function add_category($name, $parent_id, $attach_categories)
		{
			if ($this->check_duplicate('category_name', 'sc_categories', $name)) return $this->err_duplicate;

			$sql = "INSERT INTO sc_categories(category_name,parent_id) 
					VALUES ('$name', '$parent_id')";
			if (!$this->query($sql)) return false;
			
			$id = mysql_insert_id();
			for ($i=0; $i<count($attach_categories); $i++)
			{
				$sql = "INSERT INTO sc_templates(category_id,attach_cate_id)
						VALUES ('$id','".$attach_categories[$i]."')";
				if (!$this->query($sql)) return false;						
			}
			return true;
		}
		
		function update_category($id, $name, $parent_id, $attach_categories)
		{
			if ($this->check_duplicate('category_name', 'sc_categories', $name, 'category_id', $id)) return $this->err_duplicate;
			
			if ($id == $parent_id) return $this->err_child_of_itsself;
			$sql = "UPDATE sc_categories 
					SET category_name = '$name', parent_id = '$parent_id'
					WHERE category_id = '$id'";
			if (!$this->query($sql)) return false;

			$sql = "DELETE FROM sc_templates
					WHERE category_id = '$id'";
			if (!$this->query($sql)) return false;						
		
			for ($i=0; $i<count($attach_categories); $i++)
			{
				$sql = "INSERT INTO sc_templates(category_id,attach_cate_id)
						VALUES ('$id','".$attach_categories[$i]."')";
				if (!$this->query($sql)) return false;						
			}
			return true;
		}
		
		function delete_category($id)
		{
			$sql = "SELECT cp.* 
					FROM sc_categories c 
					INNER JOIN sc_cate_products cp ON c.category_id = cp.category_id 
					WHERE c.parent_id = '$id' 
						OR c.category_id = '$id'";
			$this->query($sql);
			if ($this->num_rows() > 0) return false;

			$sql = "DELETE FROM sc_categories
					WHERE category_id = '$id' OR parent_id = '$id'";
			if (!$this->query($sql)) return false;

			$sql = "DELETE FROM sc_templates
					WHERE category_id = '$id'";
			if (!$this->query($sql)) return false;						
			
			return true;
		}

		function get_categories($id=-1, $parent_id=true)
		{
			$sql = "SELECT * 
					FROM sc_categories ";
			if ($id == -2) 
				$sql.=' WHERE parent_id > 0';
			else
				if ($parent_id) 
					$sql.='WHERE parent_id = "'.$id.'"';
				else
					$sql.='WHERE category_id = "'.$id.'"';
			if ($this->query($sql)) return true;
			return false;
		}

//The methods for attach categories
		function add_attach_category($name)
		{
			if ($this->check_duplicate('name', 'sc_attach_categories', $name)) return $this->err_duplicate;
			
			$sql = "INSERT INTO sc_attach_categories(name) 
					VALUES ('$name')";
			if ($this->query($sql)) return true;
			return false;
		}
		
		function update_attach_category($id, $name)
		{
			if ($this->check_duplicate('name', 'sc_attach_categories', $name, 'id', $id)) return $this->err_duplicate;
			
			$sql = "UPDATE sc_attach_categories 
					SET name = '$name'
					WHERE id = '$id'";
			if ($this->query($sql)) return true;
			return false;
		}
		
		function delete_attach_category($id)
		{
			$sql = "SELECT category_id 
					FROM sc_cate_products
					WHERE category_id = '$id'
					union
					SELECT attach_cate_id
					FROM sc_order_detail
					WHERE attach_cate_id = '$id'";
			if (!$this->query($sql)) return false;
			if ($this->num_rows() > 0) return false;

			$sql = "DELETE FROM sc_attach_categories
					WHERE id = '$id'";
			if ($this->query($sql)) return true;
			return false;
		}

		function get_attach_categories($id = -1, $category_id = false)
		{
			$sql = "SELECT * 
					FROM sc_attach_categories ac";

			if ($category_id) $sql .= " INNER JOIN sc_templates t ON t.attach_cate_id = ac.id
										WHERE t.category_id = '$id'";
			if ($this->query($sql)) return true;
			return false;
		}
		
//The methods for products
		function get_cate_for_product($id)
		{
			$sql = "SELECT * 
					FROM sc_cate_products 
					WHERE product_id = '$id'";
			if (!$this->query($sql)) return false;
			
			$this->next_record();
			$be_attach = $this->f('be_attachment');
			
			$sql = "SELECT * ";
			if (!$be_attach)
				$sql .= " FROM sc_categories
							WHERE category_id = '".$this->f(category_id)."'";
			else
				$sql .= " FROM sc_attach_categories
							WHERE category_id = '".$this->f(category_id)."'";
			if (!$this->query($sql)) return false;
			
			$this->next_record();
			
			return true;
		}
	
		function get_product_fields_name()
		{
			$result = $this->metadata('sc_products');
			for ($i=0; $i < count($result); $i++)
				$res[] = $result[$i]['name'];
			return $res;
		}
		
		function upload_img(&$img)
		{
			if ($img['size'] > 0)
				if (!uploadFile($img, $this->maxsize, $this->upload_dir))
					return $this->over_size;
			$img = $this->upload_dir."/".$img['name'];				
			return 1;
		}		

		function add_product($part_number, $name, $category_id, $be_attachment, $price, $VAT, $description, $warranty_info, $attach_cate, $attachs)
		{
			$sql = "INSERT INTO sc_products(part_number, product_name, price, VAT, description, warranty_info)
					VALUES('$part_number', '$name', '$price', '$VAT', '$description', '$warranty_info')";
			if (!$this->query($sql)) return false;

			$id = mysql_insert_id();
			$be_attachment = $be_attachment?1:0;
			$sql = "INSERT INTO sc_cate_products(product_id, category_id, be_attachment)
					VALUES('$id', '$category_id', '$be_attachment')";
			if (!$this->query($sql)) return false;

			if (!$be_attachment)
			{
				for ($i=0; $i<count($attach_cate); $i++)
				{
					for ($j=0; $j<count($attachs[$attach_cate[$i]]); $j++)
					{
						$sql = "INSERT INTO sc_attachments(product_id, attach_cate_id, attach_id)
								VALUES('$id','".$attach_cate[$i]."', '".$attachs[$attach_cate[$i]][$j]."')";
						if (!$this->query($sql)) return false;
					}
				}
			}
			return true;
		}
		
		function update_product($id, $part_number, $name, $category_id, $be_attachment, $price, $VAT, $description, $warranty_info, $attach_cate, $attachs)
		{
//--->xoa cu			
			$sql = "UPDATE sc_products
					SET part_number = '$part_number',
						product_name = '$name',
						price = '$price',
						VAT = '$VAT',
						description = '$description',
						warranty_info = '$warranty_info'
					WHERE product_id = '$id'";
			if (!$this->query($sql)) return false;

			$be_attachment = $be_attachment?1:0;			
			$sql = "UPDATE sc_cate_products
					SET category_id = '$category_id',
						be_attachment = '$be_attachment'
					WHERE product_id = '$id'";
			if (!$this->query($sql)) return false;					

			$sql = "DELETE FROM sc_attachments
					WHERE product_id = '$id'";
			if (!$this->query($sql)) return false;
			
			if (!$be_attachment)
			{
				for ($i=0; $i<count($attach_cate); $i++)
				{
					for ($j=0; $j<count($attachs[$attach_cate[$i]]); $j++)
					{
						$sql = "INSERT INTO sc_attachments(product_id, attach_cate_id, attach_id)
								VALUES('$id','".$attach_cate[$i]."', '".$attachs[$attach_cate[$i]][$j]."')";
						if (!$this->query($sql)) return false;
					}
				}
			}
			return true;
		}
		
		function delete_product($id)
		{
			$sql = "SELECT *
					FROM sc_order_detail
					WHERE product_id = '$id' OR attach_id = '$id'";
			if (!$this->query($sql)) return false;
			if ($this->num_rows() > 0) return $this->in_order_detail;
			
			$sql = "DELETE FROM sc_products
					WHERE product_id = '$id'";
			if (!$this->query($sql)) return false;

			$sql = "DELETE FROM sc_cate_products
					WHERE product_id = '$id'";
			if (!$this->query($sql)) return false;
			
			$sql = "DELETE FROM sc_attachments
					WHERE product_id = '$id' OR attach_id = '$id'";
			if (!$this->query($sql)) return false;
			
			return true;
		}
	
		function get_products($id = -1, $category_id = false, $with_attach=false)
		{
			$sql = "SELECT p.* , p.product_id as id, c.category_id, c.category_name
					FROM sc_products p
					INNER JOIN sc_cate_products cp ON cp.product_id = p.product_id
					INNER JOIN sc_categories c ON cp.category_id = c.category_id ";

			if (!$category_id && $id>-1) $sql.= "WHERE p.product_id = '$id' ";
			if ($category_id) $sql.= "WHERE cp.category_id = '$id' ";
			
/*			if ($with_attach) 
				$sql .= "union
						select p.*, p.product_id as id, ac.id, ac.name
						from sc_products p
						inner join sc_cate_products cp on p.product_id = cp.product_id
						inner join sc_attach_categories ac on cp.category_id = ac.id ";
*/
 			if ($this->query($sql)) return true;
			return false;
		}
		
		function get_list_attach_for_product($id)
		{
			$sql = "SELECT * 
					FROM sc_attachments
					WHERE product_id = '".$id."'";
			if (!$this->query($sql)) return false;
			return true; 
		}
		
		function get_attachments($id=-1, $attach_cate_id = true)
		{
			$sql = "SELECT * 
					FROM sc_products p 
					INNER JOIN sc_cate_products cp ON p.product_id = cp.product_id ";
			if ($id != -1 && $attach_cate_id) 
				$sql .= " WHERE cp.category_id = '".$id."' 
						AND cp.be_attachment = 1";
			if (!$attach_cate_id)
				$sql .= " WHERE p.product_id = '".$id."' 
						AND cp.be_attachment = 1";
			if (!$this->query($sql)) return false;
			return true;
		}
		
		function get_buy_products($in_clause)
		{
			$sql = "SELECT product_id, product_name, price, VAT
					FROM sc_products
					WHERE product_id IN (".$in_clause.")";
			if ($this->query($sql)) return true;
			return false;
		}
		
		function get_order_product($product_id)
		{
			$sql = "SELECT *, att.product_name as attachment_name, att.product_id as attachment_id, att.price as attachment_price, att.VAT as attachment_VAT
					FROM sc_attachments a
						INNER JOIN sc_attach_categories ac ON a.attach_cate_id = ac.id
						INNER JOIN sc_products p ON p.product_id = a.product_id
						INNER JOIN sc_products att ON att.product_id = a.attach_id
					WHERE p.product_id = '".$product_id."'";
			if (!$this->query($sql)) return false;
			
			return true;
		}
		
		function get_order_productid_list($order_id)
		{
			$sql = "SELECT product_id
					FROM sc_order_detail a
					WHERE order_number = '".$order_id."'
					GROUP BY product_id";
			if (!$this->query($sql)) return false;
			
			while($this->next_record())
				$list .= ",'" . $this->f('product_id') . "'";

			return substr($list, 1);
		}
//This methods for order
		function add_order($seller, $order_number, $company, $attn, $cc, $subject, $phone, $fax, $sale_date, $valid_date, $priceadjustment, $product_id, $attach_cate_id, $attach_id, $quantity, $price, $VAT, $incdecprice)
		{
			$sql = "SELECT order_number
					FROM sc_orders 
					WHERE order_number = '".$order_number."'";
			if (!$this->query($sql) || $this->num_rows() > 0) return false;

			$sql = "INSERT INTO sc_orders(seller, order_number, company, attn, sale_date, valid_date, phone, fax, cc, subject, priceadjustment)
					VALUES ('$seller', '$order_number', '$company', '$attn', '$sale_date', '$valid_date', '$phone', '$fax','$cc', '$subject', '$priceadjustment')";
			if (!$this->query($sql)) return false;
			
			for ($i=0; $i<count($product_id); $i++)
			{
				$sql = "INSERT INTO sc_order_detail(order_number, product_id, attach_cate_id, attach_id, quantity, price, discount, VAT)
						VALUES ('".$order_number."', '".$product_id[$i]."', '".$attach_cate_id[$i]."', '".$attach_id[$i]."','".$quantity[$i]."','".$price[$i]."','".$incdecprice[$i]."','".$VAT[$i]."')";
				if (!$this->query($sql)) return false;				
			}
			return true;
		}		
		
		function update_order($seller, $order_number, $company, $attn, $cc, $subject, $phone, $fax, $sale_date, $valid_date, $priceadjustment, $product_id, $attach_cate_id, $attach_id, $quantity, $price, $VAT, $incdecprice)
		{
			$sql = "SELECT order_number
					FROM sc_orders 
					WHERE order_number = '".$order_number."'";
			if (!$this->query($sql) || $this->num_rows() > 1) return false;

			$sql = "UPDATE sc_orders
					SET seller='$seller', 
						company='$company', 
						attn='$attn', 
						sale_date='$sale_date', 
						valid_date='$valid_date', 
						phone='$phone', 
						fax='$fax',
						cc = '$cc',
						subject = '$subject',
						priceadjustment = '$priceadjustment'
					WHERE order_number = '$order_number'";
			if (!$this->query($sql)) return false;
			
			$sql = "DELETE FROM sc_order_detail
					WHERE order_number = '$order_number'";
			if (!$this->query($sql)) return false;
								
			for ($i=0; $i<count($product_id); $i++)
			{
				$sql = "INSERT INTO sc_order_detail(order_number, product_id, attach_cate_id, attach_id, quantity, price, discount, VAT)
						VALUES ('".$order_number."', '".$product_id[$i]."', '".$attach_cate_id[$i]."', '".$attach_id[$i]."','".$quantity[$i]."','".$price[$i]."','".$incdecprice[$i]."','".$VAT[$i]."')";
				if (!$this->query($sql)) return false;				
			}
			return true;
		}		
		
		function delete_order($id)
		{
			$sql = "DELETE FROM sc_orders
					WHERE order_number = '$id'";
			if (!$this->query($sql)) return false;

			$sql = "DELETE FROM sc_order_detail
					WHERE order_number = '$id'";
			if (!$this->query($sql)) return false;
		}
		
		function delete_product_from_order($order_number, $product_id)
		{
			$sql = "DELETE FROM sc_order_detail
					WHERE order_number = '$order_number' AND product_id = '$product_id'";
			if (!$this->query($sql)) return false;
			
			$sql = "SELECT * 
					FROM sc_order_detail
					WHERE order_number = '$order_number'";
			if (!$this->query($sql)) return false;

			if ($this->num_rows() == 0)
			{
				$sql = "DELETE FROM sc_orders
						WHERE order_number = '$order_number'";
				if (!$this->query($sql)) return false;
				return true;
			}
		
			return false;
		}
		
		function get_orders($has_id=false, $id='')
		{
			$sql = "SELECT o.*
					FROM sc_orders o";
			if ($has_id) $sql.="WHERE so.order_number ='".$id."'";
			if (!$this->query($sql)) return false;
			return true;
		}

//Get entire order with order detail
		function get_entire_order($id, &$order)
		{
			$sql = "SELECT * 
					FROM sc_orders
					WHERE order_number = '".$id."'";
			if (!$this->query($sql)) return false;
			if ($this->next_record()) 
			{
				$order['order_number'] = $this->f('order_number');
				$order['seller'] = $this->f('seller');
				$order['company'] = $this->f('company');
				$order['attn'] = $this->f('attn');
				$order['phone'] = $this->f('phone');
				$order['fax'] = $this->f('fax');
				$order['sale_date'] = $this->f('sale_date');
				$order['valid_date'] = $this->f('valid_date');
				$order['cc'] = $this->f('cc');
				$order['subject'] = $this->f('subject');
				$order['priceadjustment'] = $this->f('priceadjustment');
			}

			$sql = "SELECT od.*, p.product_name
					FROM sc_order_detail od
					INNER JOIN sc_products p ON od.product_id = p.product_id
					WHERE od.order_number = '".$id."'";
			if (!$this->query($sql)) return false;
			
			return true;
		}
	};
	
/* Class name  : Cart
*  Last update : 27 January 2004
*  This class was written and developed by Tran Kien Duc(trankien_duc@yahoo.com)
*  Copyright 2004 Tran Kien Duc
*/ 

class Cart extends db
{
	var $items;		// array of items
	var $total;		// total of the cart
	var $SQL;
	var $id;
	var $price;

	function Cart($ProIDFieldName, $PriceFieldName, $ProTableName) {
	// object constructor
		$this->db();
		
		$this->init($ProIDFieldName, $PriceFieldName, $ProTableName);
		$this->SQL = "SELECT ".$ProIDFieldName. ", ".$PriceFieldName." 
					FROM ".$ProTableName."
					WHERE ".$ProIDFieldName." IN ";
		$this->id = $ProIDFieldName;
		$this->price = $PriceFieldName;
	}

	function init($ProIDFieldName, $PriceFieldName, $ProTableName) {
	// this function is called to initialize (and reset) a shopping cart

		$this->items = array();
		$this->total = 0;
		$this->SQL = "SELECT ".$ProIDFieldName. ", ".$PriceFieldName." 
					FROM ".$ProTableName."
					WHERE ".$ProIDFieldName." IN ";
		$this->id = $ProIDFieldName;
		$this->price = $PriceFieldName;
	}

	function add($productid, $qty) {
	// add an item to the shopping cart and update the total price

		if (isset($productid)) {
			if (!isset($this->items[$productid])) 
				$this->items[$productid] = 0;
//			setdefault($this->items[$productid], 0);
			$this->items[$productid] += $qty;
		}
	}
	
	function addcalc($productid, $qty)
	{
		$this->add($productid, $qty);
		return $this->recalc_total();
	}

	function set($productid, $qty) {
	// set the quantity of a product in the cart to a specified value

		if (isset($productid)) {
			$this->items[$productid] = (int) $qty;
		}
	}
	
	function setcalc($productid, $qty)
	{
		$this->set($productid, $qty);
		return $this->recalc_total();		
	}

	function remove($productid) {
	// this function will remove a given product from the cart

		if (isset($productid)) {
			unset($this->items[$productid]);
		}
	}

	function removecalc($productid)
	{
		$this->remove($productid);
		return $this->recalc_total();		
	}

	function cleanup() {
	// this function will clean up the cart, removing items with invalid product
   //  id's (non-numeric ones) and products with quantities less than 1

		foreach ($this->items as $productid => $qty) {
			if ($qty < 1) {
				unset($this->items[$productid]);
			}
		}
	}
	
	function cleanall()
	{
		foreach ($this->items as $productid => $qty)
			unset($this->items[$productid]);
		$this->recalc_total();
	}

	function itemcount() {
	// returns the number of individual items in the shopping cart (note, this
	// takes into account the quantities of the items as well)

		$count = 0;
		foreach ($this->items as $productid => $qty) {
			$count += $qty;
		}

		return $count;
	}

	function get_productid_list() {
	/* return a comma delimited list of all the products in the cart, this will
	 * be used for queries, eg. SELECT id, price FROM products WHERE id IN ....*/

		$productid_list = "";

		foreach ($this->items as $productid => $qty) {
			$productid_list .= ",'" . $productid . "'";
		}

		// need to strip off the leading comma
		return substr($productid_list, 1);
	}

	function recalc_total() {
	/* recalculate the total for the shopping cart, we will also do some cleanup
	 * and remove invalid items from the cart.  we have to query the database to
	 * get the prices, so instead of making one query for each product in the
	 * basket, we will gather up all the ID's we are interested in and run one
	 * query to get all the products we care about (using $in_clause) */

		$this->total = 0;

		$in_clause = $this->get_productid_list();
		if (empty($in_clause)) {
			return;
		}
		
		//$qid = $this->query($this->SQL."($in_clause)");
		$this->query($this->SQL."($in_clause)");
		
//		while ($product = mysql_fetch_array($qid)) {
		while ($this->next_record())
		{
//			$this->total += $this->items[$product[$this->id]] * $product[$this->price];
			$this->total += $this->items[$this->f($this->id)] * $this->f($this->price);
		}
//		mysql_free_result($qid);

		return $this->total;
	}
}
?>
