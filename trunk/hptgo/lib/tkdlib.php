<?php 
/*
   Copyright Intermesh 2003
   Author: Tran Kien Duc <trankien_duc@yahoo.com>
   Version: 1.0 Release date: 13 July 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
	function print_config_title($page=0)
	{
		global $GO_SECURITY;
		
		$db = new db();
		$db->query("SELECT order_fields FROM ab_config WHERE page=".$page." AND user_id='".$GO_SECURITY->user_id."'"); 
		if ($db->next_record())
			$order = explode(",",$db->f('order_fields'));

		return $order;
	}
	
	function print_config_content($order, $ab, $page)
	{
		global $strSexes, $GO_USERS;
	
		$db = new db();

		for ($i=0; $i<count($order); $i++)
		{
			switch ($order[$i])
			{
				case 'email' :
					if ($page==0)
						echo "<td nowrap>".mail_to(empty_to_stripe($ab->f("email")),
	    							empty_to_stripe($ab->f("email")),
	    							'normal',
	    							true,
	    							$ab->f("id"))."&nbsp;</td>\n";
					if ($page==1)
						echo '<td>'.mail_to($ab->f('email'), $ab->f('email')).'</td>';
					if ($page==2)
							echo "<td>".mail_to(empty_to_stripe($GO_USERS->f("email")))."&nbsp;</td>\n";
				break;
				case 'sex':
					echo '<td nowrap> '.$strSexes[$ab->f('sex')].' </td>';
				break;
				case 'birthday':
				case 'relation_date':
					$day = ($ab->f($order[$i])>0?db_date_to_date($ab->f($order[$i])):'');
					echo '<td nowrap> '.empty_to_stripe($day).' </td>';
				break;
				case 'company_id':
				case 'parent':
					$db->query('SELECT name FROM ab_companies WHERE id = '.$ab->f($order[$i]));
					echo '<td nowrap> '.empty_to_stripe($db->next_record()?$db->f('name'):'').' </td>';
				break;
				case '':break;
				default: echo "<td nowrap> ".empty_to_stripe($ab->f($order[$i]))."&nbsp; </td>\n";
			}
		}
	}

	function goURL($url)
	{
		echo '<script language="javascript"> document.location = "'.$url.'" </script>';
	}
 
	function account_manager($acl_id)
	{
		global $GO_SECURITY, $GO_USERS, $strAccountManager;
		  		
		$disabled = $read_only ? 'disabled' : '';

		$GO_SECURITY->get_users_in_acl($acl_id);

		$first = true;
		while ($GO_SECURITY->next_record())
		{
			$acc = '';
			if ($first)
			{
				$acc = $strAccountManager."&nbsp;:&nbsp;";
				$first = false;
			}
				 
			if ( $profile = $GO_USERS->get_user( $GO_SECURITY->f('user_id') ) )
		  	{
				$middle_name = $profile["middle_name"] == '' ?'' : $profile["middle_name"].' ';
	      		$name = $profile["last_name"].' '.$middle_name.$profile["first_name"];

    	  		echo '<tr><td align="right" nowrap>'.$acc.'</td><td nowrap>&nbsp;&nbsp;'.$name.'</td></tr>';
		  	}
		}
	}

//----------------------------------- CHANGE ITEM (UP, DOWN) --------------------------------

	class move_item
	{
		function change($a, $b)
		{
			return "javascript:change('$a','$b');";
		}
		
		function item($id, $str, $value, $checked = false)
		{
			$str = '<td><div id="item'.$id.'">'.$str.'</div></td><td><input type="checkbox" id="check" name="com[]" value="'.$value.'" '.($checked?'checked':'').'><input type="hidden" id="order_all" name="order_all[]" value="'.$value.'"></td>';
			return $str;
		}
	
		function move_item()
		{	
			$str = <<<EOF
				<script language="javascript">
					function change(a, b)
					{
						a--;
						b--;
						temp = document.forms[0].order_all[a].value;
						document.forms[0].order_all[a].value = document.forms[0].order_all[b].value;
						document.forms[0].order_all[b].value = temp;
					
						temp = document.forms[0].check[a].checked;
						document.forms[0].check[a].checked = document.forms[0].check[b].checked;
						document.forms[0].check[b].checked = temp;
					
						temp = document.forms[0].check[a].value;
						document.forms[0].check[a].value = document.forms[0].check[b].value;
						document.forms[0].check[b].value = temp;

						a = 'item' + ++a;
						b = 'item' + ++b;
						
						if (document.all)
						{
							temp = document.all[a].innerHTML;
							document.all[a].innerHTML = document.all[b].innerHTML;
							document.all[b].innerHTML = temp;
						}
						else if (document.layers)
						{
							temp = document.layers[a].innerHTML;
							document.layers[a].innerHTML = document.layers[b].innerHTML;
							document.layers[b].innerHTML = temp;
						}
						else
						{
							temp = document.getElementById(a).innerHTML;
							document.getElementById(a).innerHTML = document.getElementById(b).innerHTML;
							document.getElementById(b).innerHTML = temp;
						}
					}
			</script>
EOF;
		echo $str;
		}
	};

//------------------------------ CALENDAR ---------------------------------

	class tkdCalendar
	{
		function init($calendarpath, $langfile, $themepath)
		{
			$str = <<<EOF
				<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
				<script type="text/javascript" src="$calendarpath/calendar.js"></script>
				<script type="text/javascript" src="$calendarpath/lang/$langfile"></script>
				<script type="text/javascript" src="$calendarpath/calendar-setup.js"></script>
				<link href="$themepath/calendar.css" type="text/css" rel="stylesheet" />
EOF;
			echo $str;
		}
		
		function show($inputname, $datevalue="", $buttonname, $buttonvalue,   $onchange="", $ifFormat="", $firstDay = 1)
		{
			if (empty($ifFormat))
				$ifFormat = $_SESSION['GO_SESSION']['date_format'];

			if (empty($datevalue))
			{
				$local_time = get_time();
	    		$year = isset($_REQUEST['year']) ? $_REQUEST['year'] : date("Y", $local_time);
	    		$month = isset($_REQUEST['month']) ? $_REQUEST['month'] : date("m", $local_time);
	    		$day = isset($_REQUEST['day']) ? $_REQUEST['day'] : date("j", $local_time);
				
				$datevalue = date($ifFormat, mktime(0, 0, 0, $month, $day, $year));
			}
			$ifFormat = str_replace("m","%m",$ifFormat);
			$ifFormat = str_replace("d","%d",$ifFormat);
			$ifFormat = str_replace("Y","%Y",$ifFormat);
			
			$inputid = $inputname . "_id";
			$buttonid = $buttonname . "_id";
	  		$str = <<<EOF
				<input class="textbox" type="text" id="$inputid" name="$inputname" value="$datevalue" onchange="$onchange" />
	  			<input type="button" name="$buttonname" id="$buttonid" value="$buttonvalue"/>
	  			<script type="text/javascript">
      				var calendar = Calendar.setup(
					{
						firstDay  : $firstDay,
						inputField  : "$inputid",
						ifFormat    : "$ifFormat", date :  "$datevalue", button      : "$buttonid"});
				</script>
EOF;
			echo $str;
		}
	};
	
?>