<?php 
/*
   Copyright (2004)
   Author: Tran Kien Duc <ductk@hptvietnam.com.vn>
   Version: 1.0 Release date: 13 July 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

	function print_input_file($frm, $name,$value,$class,$mo_class,$attributes='',$width='')
	{
		$button = "document.$frm.".$name."_button";
		$str =
			'&nbsp;<span style="position:relative">
				<input type="button" id="'.$name.'_button" name="'.$name.'_button" value="'.$value.'" class="'.$class.'" style="position:absolute;top:0px;left:0px;width:'.(empty($width)?'105':$width).'px;">

				<script language="javascript">
					var button = document.getElementById("'.$name.'_button");
				</script>
				
				<input type="file" id="'.$name.'" name="'.$name.'" style="position:absolute;top:0px;left:0px;width:0px;-moz-opacity:0;filter:alpha(opacity:0);opacity:0;" size="1" '.$attributes.' onmouseover="javascript:button.className=\''.$mo_class.'\';" onmouseout="javascript:button.className=\''.$class.'\';">
			</span>
			<script language="javascript">
			 	if (!document.all) button.style.marginTop = "-1px";
			</script>';
  		echo $str;
	}
 
 	function AddToME($ME,$str)
	{
		if ($ME[strlen($ME)-1]!="?" || $ME[strlen($ME)-1]!="&")
			return $ME."&".$str;
		else
			return $ME.$str;
	}
	
	function PageNumber($sumrow,$maxrow,$maxpage,&$curpage,$ME,&$strCurPage,$strPrev,$strNext)
	{
		if ($maxrow>0)
		{	
			$sumpage=(int)($sumrow/$maxrow)+(($sumrow%$maxrow==0)?0:1);
			if ($sumpage>1)
			{
				$str="";
				if ($curpage=="" || $curpage<1) $curpage=1;
				if ($curpage>$sumpage) $curpage=$sumpage;

				$sumseg=(int)($sumpage/$maxpage)+(($sumpage%$maxpage==0)?0:1);
				$seg=(int)(($curpage-1)/$maxpage);

				$min=($sumpage<($seg*$maxpage+$maxpage)?$sumpage:($seg*$maxpage+$maxpage));

				for ($i=$seg*$maxpage+1; $i<=$min; $i++)
				{
					if ($i==$curpage)
						$str.="<b><i>$i</i></b>";
					else
						$str.="<a href=".AddToME($ME,"curpage=$i").">$i</a>";

					if ($i!=($sumpage<($seg*$maxpage+$maxpage)?$sumpage:($seg*$maxpage+$maxpage)))
						$str.="&nbsp;";
				}
				
				if ($seg>0)
				{
					$st=AddToME($ME,"curpage=".($seg*$maxpage));
					$str="<a href=".$st." style=text-decoration:none><< $strPrev</a>&nbsp;&nbsp;".$str;
				}
				if ($seg<$sumseg-1)
				{
					$st=AddToME($ME,"curpage=".($seg*$maxpage+$maxpage+1));
					$str.="&nbsp;&nbsp;<a href=".$st." style=text-decoration:none>$strNext >></a>";
				}
				$strCurPage="Trang ".$curpage."/".$sumpage;
				return $str;
			}
		}
	}
 
 	function initTyping($root_path)
	{
		$str = '<script language="javascript" src="'.$root_path.'lib/vietuni.js"></script>'.
			   '<script language="javascript">'.
				 	'initTypingForm();'.
					'setTypingMode(2);'.
				'</script>';
		echo $str;
	}
	
function checkFile($userfile, $maxsize)
{
		$mimetypes = array(
			"image/x-png" 			=> 1, 
		    "video/vivo"			=> 1, 
		    "video/x-msvideo" 		=> 1, 
		    "audio/x-pn-realaudio"	=> 1, 
		    "image/gif" 	    	=> 1, 
		    "video/mpeg"		    => 1, 
		    "image/ico"				=> 1, 
		    "image/x-MS-bmp"		=> 1, 
		    "image/tiff" 			=> 1, 
		    "video/quicktime" 		=> 1, 
		    "image/jpeg" 			=> 1, 
		    "image/pjpeg" 			=> 1);

		if ((empty($userfile['name'])) || ($userfile['name'] == "none"))
			return false;

		if (($userfile['size'] == 0) || ($userfile['size'] > $maxsize))
			return false;
			
		//if (!isset($mimetypes[$userfile['type']]))		
			//return false;		
			
		return true;
}
	
function uploadFile($userfile, $maxsize, $upload_dir)
{		
	if (!checkFile($userfile, $maxsize))
		return false;

	$tmp_dir = $userfile['tmp_name'];
	$name = $upload_dir."/".$userfile['name'];
	
	if (!@move_uploaded_file($tmp_dir, $name))
		return false;

	return true;	
}
	
	function get_today()
	{
		$local_time = get_time();
//		$year = isset($_REQUEST['year']) ? $_REQUEST['year'] : date("Y", $local_time);
//		$month = isset($_REQUEST['month']) ? $_REQUEST['month'] : date("m", $local_time);
//		$day = isset($_REQUEST['day']) ? $_REQUEST['day'] : date("j", $local_time);
//		$hour = isset($_REQUEST['hour']) ? $_REQUEST['hour'] : date("H", $local_time);
//		$min = isset($_REQUEST['min']) ? $_REQUEST['min'] : date("i", $local_time);

		$year = date("Y", $local_time);
		$month = date("m", $local_time);
		$day = date("j", $local_time);
		$hour = date("H", $local_time);
		$min = date("i", $local_time);
		
		$requested_time=mktime($hour,0,0,$month, $day, $year);
		$requested_date=date($_SESSION['GO_SESSION']['date_format'], $requested_time);
		
		return $requested_date;
	}


	function send_mail($mail_to, $mail_body, $mail_subject='No title',$mail_name='No name', $mail_from='',$mail_priority=3, $mail_wordwrap=50, $mail_altbody='')
	{
		global $GO_CONFIG;
	
//	$mail_to='ductk@hptvietnam.com.vn';		
	
//	$mail_from = 'bigduck1004@yahoo.com';
//	$mail_name = "333";
//	$mail_subject = 'subject';
	
//	$mail_body = '123456789';
//	$mail_altbody = 'qqqqqqqqqqqqqqqq';
	
		require($GO_CONFIG->class_path."phpmailer/class.phpmailer.php");
		require($GO_CONFIG->class_path."phpmailer/class.smtp.php");

	    $mail = new PHPMailer();
    	$mail->PluginDir = $GO_CONFIG->class_path.'phpmailer/';
	    $mail->SetLanguage($php_mailer_lang, $GO_CONFIG->class_path.'phpmailer/language/');

	    switch($GO_CONFIG->mailer)
    	{
      		case 'smtp':
				$mail->Host = $GO_CONFIG->smtp_server;
				$mail->Port = $GO_CONFIG->smtp_port;
				$mail->IsSMTP();			  
			break;			
    	    case 'qmail':
				$mail->IsQmail();
			break;			
    	  	case 'sendmail':
				$mail->IsSendmail();
			break;
	        case 'mail':
				$mail->IsMail();
			break;
    	}
	    $mail->Priority = $mail_priority;
	    $mail->Sender     = $mail_from;    
	    $mail->From     = $mail_from;
	
	    $mail->FromName = $mail_name;
	    $mail->AddReplyTo($mail_from,$mail_name);
	    $mail->WordWrap = $mail_wordwrap;
//    $mail->Encoding = "quoted-printable";

	    $mail->IsHTML(true);
	    $mail->Subject = $mail_subject;

		$mail->AddAddress($mail_to);

		$mail->Body = $mail_body;
		$mail->AltBody = $mail_altbody;

	    if(!$mail->Send())
			return '<p class="Error">'.$ml_send_error.' '.$mail->ErrorInfo.'</p>';
	}

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
	
	function alert($msg)
	{
		echo '<script language="javascript"> alert("'.$msg.'") </script>';
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

    	  		echo '<tr><td align="right" nowrap>'.$acc.'</td><td nowrap>&nbsp;&nbsp;'.htmlspecialchars($name).'</td></tr>';
		  	}
		}
	}
	
//----------------------------- CHOICE LIST ----------------------------------------------------	
	
class choice_list
{
	var $text1;
	var $value1;
	var $text2;
	var $value2;
	var $num;
	var $name;
	var $form_name;

	function choice_list()
	{	
		$this->num=0;
		$str = '<script language="javascript">
	function move(select1,select2)
	{
		if (select1.selectedIndex>-1)
		{
			select2.options[select2.options.length] = new Option(select1.options[select1.selectedIndex].text,select1.options[select1.selectedIndex].value);
			select1.options[select1.selectedIndex] = null;
		}
	}

	function move_all(select1,select2)
	{
		len = select1.length;
		for (i=0; i<len; i++)
		{
			select2.options[select2.options.length] = new Option(select1.options[0].text,select1.options[0].value);
			select1.options[0] = null;
		}
	}
	
/*	function move_value(value)
	{
		len ='.$select2.'.length; 
		for (i=0; i<len; i++)
		{
			alert(i+"-"+'.$select2.'.length);
			'.$select2.'.options[0] = null;
		}
			
		for (i=0; i<value.length; i++)
		{
			for (j=0; j<$select1.length; j++)
				if ($select1.options[j].value == value[i]) 
				{
					select2.options[select2.options.length] = new Option(select1.options[0].text,select1.options[0].value);
				}
		}
	}*/
	</script>';

		echo $str;
	}
	
	function clear($list_num = 0)
	{
		if ($list_num == 1 || $list_num == 0)
		{
			unset($this->text1);
			unset($this->value1);
		}
		if ($list_num == 2 || $list_num == 0)
		{
			unset($this->text2);
			unset($this->value2);
		}
	}

	function add_option($text,$value,$to_list=false)
	{
		if ($to_list)
		{
			$this->text2[] = $text;
			$this->value2[] = $value;
		}
		else
		{
			$this->text1[] = $text;
			$this->value1[] = $value;
		}
		
	}
	
	function print_choice_list($form_name, $name, $width='100pt', $size = '6')
	{
		$this->form_name = $form_name;
		$this->name[] = $name;
		$this->num = count($this->name);
		$str = '
  <table width="1%"  border="0" cellspacing="0" cellpadding="3">
    <tr>
      <td width="1%">
	  	<select id="sl1'.$this->num.'" name="sl1'.$this->num.'[]" size="'.$size.'" class="textbox" style="width:'.$width.'">';
		
		for ($i=0; $i<count($this->text1); $i++)
        	$str .= '<option value="'.$this->value1[$i].'">'.$this->text1[$i].'</option>';
		
		$select1 = 'document.'.$form_name.'.sl1'.$this->num;
		$select2 = 'document.'.$form_name.'.'.$name;

      	$str .= '
		</select>
	  </td>
      <td width="1%">
	  	<table width="1%"  border="0" cellspacing="0" cellpadding="1">
		<tr><td align="center">
	  		<input type="button" class="button" name="Button0'.$this->num.'" value=">>" onClick="move_all('.$select1.','.$select2.')">
		</td></tr>
		<tr><td align="center">
	  		<input type="button" class="button" name="Button1'.$this->num.'" value=" > " onClick="move('.$select1.','.$select2.')">
		</td></tr>
		<tr><td align="center">
      		<input type="button" class="button" name="Button2'.$this->num.'" value=" < " onClick="move('.$select2.','.$select1.')">
		</td></tr>
		<tr><td align="center">
	  		<input type="button" class="button" name="Button3'.$this->num.'" value="<<" onClick="move_all('.$select2.','.$select1.')">
		</td></tr>
		</table>
	  </td>
      <td width="1%">
	  	<select id="'.$name.'" name="'.$name.'[]" size="'.$size.'" class="textbox" style="width:'.$width.'" multiple>';
		
		for ($i=0; $i<count($this->text2); $i++)
        	$str .= '<option value="'.$this->value2[$i].'">'.$this->text2[$i].'</option>';

		$str .='
      	</select>
	  </td>
    </tr>
  </table>';

		echo $str;
	}
	
	function select_all()
	{
		$str = '<script language="javascript">
			function selectall()
			{';
			
		for ($i=0; $i<count($this->name); $i++)
		{
			$select2 = 'document.'.$this->form_name.'.'.$this->name[$i];
				$str .= 'for (i=0; i<'.$select2.'.options.length;i++)
				'.$select2.'.options[i].selected=true;';
		}
		
		$str .= '
			}
		</script>';
		
		echo $str;
	}
	
	function onSubmit_event()
	{
		return "selectall();";
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
