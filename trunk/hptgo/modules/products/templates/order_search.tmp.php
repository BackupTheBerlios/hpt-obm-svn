<form name="frmSearch" method="post">
	<input name="task" type="hidden">
	<input name="close_win" type="hidden">

<?php
	$fielddrop = new dropbox();
	$res = $pro->get_order_fields_name();
	for ($i=0; $i<count($res); $i++)
		if (isset($sc_fields[$res[$i]]))
			$fielddrop->add_value($res[$i],$sc_fields[$res[$i]]);
?>
<table width="30%"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td nowrap><?php echo $sc_search_on?></td>
    <td>
<?php
	$fielddrop->print_dropbox('search_fld',$_POST['search_fld']);
?>	
	</td>
  </tr>
  <tr>
    <td nowrap><?php echo $sc_search_phrase?></td>
    <td><input type="text" name="search_value"></td>
  </tr>
</table>
<br>
<?php
	$button = new button($cmdSearch, "javascript:click_but(frmSearch,'search','false')");
?>
<br><br>
</form>