function expand_all()
{
	document.forms[0].form_action.value = 'expand_all';
	document.forms[0].submit();
}

function collapse_all()
{
	document.forms[0].form_action.value = 'collapse_all';
	document.forms[0].submit();
}

function move_mail()
{
	document.forms[0].form_action.value = 'move';
	document.forms[0].submit();
}

function invert_selection()
{
	for (var i=0;i<document.forms[0].elements.length;i++)
	{
		if(document.forms[0].elements[i].type == 'checkbox' && document.forms[0].elements[i].name != 'dummy')
		{
			document.forms[0].elements[i].checked = !(document.forms[0].elements[i].checked);
			item_click(document.forms[0].elements[i]);
		}
	}
}

function confirm_empty_mailbox(message)
{
	if (confirm(message))
	{
		document.forms[0].empty_mailbox.value='true';
		document.forms[0].submit();
	}
}

function item_click(check_box)
{
	var item = get_object(check_box.value);
	if (check_box.checked)
	{
		if (item.className == 'Table1' || item.className == 'Table5')
		{
			item.className = 'Table2';
		}else
		{
			item.className = 'Table3';
		}
	}else
	{
		if (item.className == 'Table2')
		{
			item.className = 'Table1';
		}else
		{
			item.className = 'Table4';
		}
	}
}

function set_flag()
{
	document.forms[0].form_action.value = 'set_flag';
	document.forms[0].submit();
}

function change_page(first_row, max_rows)
{
	document.forms[0].max_rows.value=max_rows;
	document.forms[0].first_row.value=first_row;
	document.forms[0].submit();
}

function sort(new_sort_field, new_sort_order)
{
	document.forms[0].new_sort_field.value=new_sort_field;
	document.forms[0].new_sort_order.value=new_sort_order;
	document.forms[0].submit();
}