	function click_but(frm, task, close_win)//, info=false, con_info='')
	{
		frm.close_win.value = close_win;
		frm.task.value = task;		
		frm.submit();
	}
	
	function click_txt(frm, task, info, con_info, close_win)
	{
		if (info) info.value = con_info;
		if (frm.close_win) frm.close_win.value = close_win;
		frm.task.value = task;	
		frm.submit();
	}
	
	function click_del(frm, id, name, mess)
	{
		if (confirm(mess))
		{
			frm.task.value = 'delete';
			frm.id.value = id;
			frm.txt_name.value = name;
			frm.submit();
		}
	}
	
	function click_delete(frm, id, name, mess, txt_att)
	{
		if (confirm(mess))
		{
			frm.task.value = 'delete' + txt_att;
			frm.txt_id.value = id;
			frm.txt_name.value = name;
			frm.submit();
		}
	}
	
	function check_not_bland(frm, ctrl, msg)
	{
		for (i=0; i<ctrl.length; i++)
		{
			if (ctrl[i].value == "")
			{
				alert(msg[i]);
				ctrl[i].focus();
				return false;
			}
		}
		return true;
	}
	
	function inputNumber(number, other_chars)
	{
		var pattern = "0123456789";
		if (other_chars != '') pattern += other_chars;
	
		if (len != 0)
		{
			var index = 0;
			var len = number.value.length;
		
			while ((index < len) && (len != 0))
				if (pattern.indexOf(number.value.charAt(index)) == -1)
				{
					if (index == len-1)
						number.value = number.value.substring(0, len-1);
					else if (index == 0)
						 	number.value = number.value.substring(1, len);
						 else number.value = number.value.substring(0, index)+number.value.substring(index+1, len);
					index = 0;
					len = number.value.length;
				}
				else index++;
		}
	}

	