<?php
	require('../../lib/tkdlib.php');
	$upload_dir = 'upload';
	$maxsize = 500000;

	$list_id = $_REQUEST['list_id'];
	$sort_fld = $_REQUEST['sort_fld'];
	$direction = ($_REQUEST['direction']=='ASC'?'DESC':'ASC');
	
	$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
	$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;
	$curpage = isset($_REQUEST['curpage']) ? $_REQUEST['curpage'] : 1;
//	$max_rows = 1;
	$first = $max_rows * ($curpage - 1);
//	alert($_GET['curpage']);

	if (!isset($_SESSION['cart']))$_SESSION['cart'] = new Cart('product_id','price','sc_products');
	$trash = '<img src="'.$GO_THEME->images['delete'].'" border="0">';
	$spliter = '<tr><td colspan="100" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';

	$image['mlastnode'] = '<img src="'.$GO_THEME->images['mlastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['emptylastnode'] = '<img src="'.$GO_THEME->images['emptylastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['plastnode'] = '<img src="'.$GO_THEME->images['plastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['mnode'] = '<img src="'.$GO_THEME->images['mnode'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['emptynode'] = '<img src="'.$GO_THEME->images['emptynode'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['pnode'] = '<img src="'.$GO_THEME->images['pnode'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['vertline'] = '<img src="'.$GO_THEME->images['vertline'].'" border="0" height="22" width="16" align="absmiddle" />';
	$image['blank'] = '<img src="'.$GO_THEME->images['blank'].'" border="0" height="22" width="16" align="absmiddle" />';
	
	$pro = new products();
	$exp = $_SESSION['exp'];
	$task = $_REQUEST['task'];
	switch ($task)
	{
		case 'update':
			$id = $_POST['id'];
			$name = $_POST['product'];
			$category_id = $_POST['category_id'];
			$is_attachment = $_POST['is_attach'];
			$price = $_POST['price'];
			$description = $_POST['description'];
			$warranty = $_POST['warranty'];
			$part_number = $_POST['part_number'];
			$VAT = $_POST['VAT'];			
			
			$cate = $_POST['attachment_categories'];
			for ($i=0; $i<count($cate); $i++)
			{
				$get_attach = $_POST['attach_'.$cate[$i]];
				$attachs[$cate[$i]] = $get_attach;
			}
			$pro->update_product($id,$part_number,$name,$category_id,$is_attachment, $price, $VAT, $description, $warranty,$cate,$attachs);
		break;
		case 'save':
		case 'apply':
			$name = $_POST['product'];
			$category_id = $_POST['category_id'];
			$is_attachment = $_POST['is_attach'];
			$price = $_POST['price'];
			$description = $_POST['description'];
			$warranty = $_POST['warranty'];
			$part_number = $_POST['part_number'];
			$VAT = $_POST['VAT'];

			$cate = $_POST['attachment_categories'];
		
			for ($i=0; $i<count($cate); $i++)
			{
				$get_attach = $_POST['attach_'.$cate[$i]];
				$attachs[$cate[$i]] = $get_attach;
			}

			$pro->add_product($part_number,$name,$category_id,$is_attachment, $price, $VAT, $description, $warranty ,$cate,$attachs);
		if ($task=='save') break;

		case 'edit_attach':
			$id = $_POST['id'];
			$pro->get_attachments($id,false);
			if ($pro->next_record())
			{
				$name = $pro->f('product_name');
				$category_id = $pro->f('category_id');
				$is_attachment = $pro->f('be_attachment');
				$price = $pro->f('price');
				$description = $pro->f('description');
				$part_number = $pro->f('part_number');
				$VAT = $pro->f('VAT');
				$warranty = $pro->f('warranty_info');
			}
		case 'edit':
			$change_case = 'change';				
			$id = $_POST['id'];					
			if ($task == 'edit')
			{
				$pro->get_list_attach_for_product($id);
				while ($pro->next_record())
					$list_attach[$pro->f('attach_cate_id')][] = $pro->f('attach_id');

				$pro->get_products($id);
				if ($pro->next_record())
				{
					$name = $pro->f('product_name');
					$category_id = $pro->f('category_id');
					$is_attachment = $pro->f('be_attachment');
					$price = $pro->f('price');
					$description = $pro->f('description');
					$part_number = $pro->f('part_number');
					$VAT = $pro->f('VAT');
					$warranty = $pro->f('warranty_info');
				}
			}
		case 'add_change':
		case 'change':
			if ($task=='change' || $task=='add_change' )
			{
				$change_case = $task;			
				$id = $_POST['id'];

				$pro->get_list_attach_for_product($id);
				while ($pro->next_record())
					$list_attach[$pro->f('attach_cate_id')][] = $pro->f('attach_id');
				
				$name = $_POST['product'];
				$category_id = $_POST['category_id'];
				$is_attachment = $_POST['is_attach'];
				$price = $_POST['price'];
				$description = $_POST['description'];
				$warranty = $_POST['warranty'];
				$part_number = $_POST['part_number'];
				$VAT = $_POST['VAT'];			
			
				$cate = $_POST['attachment_categories'];
			}
		case 'add': 			
			if (!isset($change_case)) $change_case = 'add_change';
			$catedrop = new dropbox();
			if ($is_attachment==true)
			{
				$pro->get_attach_categories();
				while($pro->next_record())
					$catedrop->add_value($pro->f('id'),$pro->f('name'));
			}
			else
			{
				$pro->get_categories(-2);
				$first = true;
				$hasa = false;
				while($pro->next_record())
				{
					if (first) //&& !isset($category_id))
					{
						$first_cate_id = $pro->f('category_id');
//						$category_id = $pro->f('category_id');
						$first = false;
					}
					if ($category_id == $pro->f('category_id')) $hasa = true;
						
					$catedrop->add_value($pro->f('category_id'),$pro->f('category_name'));
				}
				if (!$hasa) $category_id = $first_cate_id;
			}
			$pro->get_attach_categories();
			
			$dropbox = new dropbox();

			require('templates/edit_product.tmp.php');
		break;
		case 'shrink':
		case 'expand':

			$pid = $_POST['id'];
			if ($task=='expand')
			{
				if (count($exp)>0)
				{
					if (!in_array($pid,$exp))
						$exp[] = $pid;
				}
				else
					$exp[] = $pid;
			}
			else
			{
				if (count($exp)==1)
					unset($exp);
				else
				{
					if (($pos = array_search($pid,$exp))>-1)
						unset($exp[$pos]);
				}
			}
			$_SESSION['exp'] = $exp;

		case 'delete':
		case 'buy':
		case 'buy_all':
			switch ($task)
			{
				case 'delete':
					if ((int)$pro->delete_product($_POST['id'])==$pro->in_order_detail)
						alert(sprintf($sc_CannotDeleteProduct,$_POST['txt_name']));
				break;
				case 'buy': 
					$_SESSION['cart']->addcalc($_POST['id'],1);
				break;
				case 'buy_all':
					$buy_id = $_POST['buy_id'];
					for ($i=0; $i<count($buy_id); $i++)
						$_SESSION['cart']->addcalc($buy_id[$i],1);
				break;
			}
		default:
			if ($task == 'apply') unset($_POST);
			require('templates/header.tmp.php');	
//			require('templates/search.tmp.php');
			
			require('templates/list_product.tmp.php');
	}
	if ($_POST['close_win']=='true') goURL('index.php');
?>