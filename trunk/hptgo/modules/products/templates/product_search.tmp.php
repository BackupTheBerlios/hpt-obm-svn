<form name="frmProSeach" method="post">
	<input name="task" type="hidden">
	<input name="close_win" type="hidden">
<?php
	$fielddrop = new dropbox();
	$res = $pro->get_product_fields_name();
	for ($i=0; $i<count($res); $i++)
		if (isset($sc_fields[$res[$i]]))
			$fielddrop->add_value($sc_fields[$res[$i]],$sc_fields[$res[$i]]);

	$pro->get_categories();
	$catedrop = new dropbox();
	$catedrop->add_value(0,$sc_all);
	while ($pro->next_record())
		$catedrop->add_value($pro->f('category_id'),$pro->f('category_name'));
?>
<table width="30%"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td><?php echo $sc_search_on?></td>
    <td>
<?php
	$fielddrop->print_dropbox('field_search','');
	echo ' '.$sc_in.' ';
	$catedrop->print_dropbox('cate_search','');
?>	
	</td>
  </tr>
  <tr>
    <td><?php echo $sc_search_phrase?></td>
    <td><input type="text" name="phrase_search"></td>
  </tr>
</table>
<?php
	$button = new button($cmdSearch, "javascript:click_but(frmProSearch,'search','false')");
?>
<br><br>
</form>