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