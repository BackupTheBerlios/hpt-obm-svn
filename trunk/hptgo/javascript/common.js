function popup(url,width,height)
{
	var centered;
	x = (screen.availWidth - width) / 2;
	y = (screen.availHeight - height) / 2;
	centered =',width=' + width + ',height=' + height + ',left=' + x + ',top=' + y + ',scrollbars=yes,resizable=yes,status=yes';
	var popup = window.open(url, '_blank', centered);
    	if (!popup.opener) popup.opener = self;
	popup.focus();
}

function confirm_action(url, message)
{
	if (confirm(unescape(message)))
	{
		window.location=url
	}
}

function div_text_replace(string,text,by) {
	// Replaces text with by in string
	var strLength = string.length, txtLength = text.length;
	if ((strLength == 0) || (txtLength == 0)) return string;

	var i = string.indexOf(text);
	if ((!i) && (text != string.substring(0,txtLength))) return string;
	if (i == -1) return string;

	var newstr = string.substring(0,i) + by;

	if (i+txtLength < strLength)
		newstr += div_text_replace(string.substring(i+txtLength,strLength),text,by);

	return newstr;
}

function div_confirm_text(message_id)
{
	str = document.getElementById('div_confirm_'+message_id).innerHTML
	str = div_text_replace(str,'&lt;','<')
	str = div_text_replace(str,'&gt;','>')
	str = div_text_replace(str,'&amp;','&')
	return str;
}

function div_confirm(message_id)
{
	return confirm(div_confirm_text(message_id))
}

function div_confirm_action(url, message_id)
{
	if (div_confirm(message_id))
	{
		window.location=url
	}
}

function get_object(name)
{
	if (document.getElementById)
	{
		return document.getElementById(name);
 	}
 	else if (document.all)
	{
  		return document.all[name];
 	}
 	else if (document.layers)
	{
  		return document.layers[name];
	}
	return false;
}

function check_checkbox(id)
{
	if(check_box = get_object(id))
	{
		if (!check_box.disabled)
		{
			check_box.checked = !check_box.checked;
			if (check_box.onclick)
			{
				check_box.onclick();
			}
		}
	}
}

function select_radio(id)
{
	if(radio_but = get_object(id))
	{
		radio_but.checked = true;
		if (radio_but.onclick)
		{
			radio_but.onclick();
		}
	}
}


function getSelected(opt) 
{
	var selected = new Array();
	var index = 0;
	for (var i=0; i<opt.length;i++) 
	{
		if ((opt[i].selected) || (opt[i].checked)) 
		{
			index = selected.length;
			selected[index] = new Object;
			selected[index].value = opt[i].value;
			selected[index].index = i;
		}
	}
	return selected;
}
