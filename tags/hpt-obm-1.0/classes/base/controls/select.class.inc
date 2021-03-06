<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

class select
{
  var $value;
  var $name;
  var $type;
  var $form_name;
  var $type;

  var $name_field_value;
  var $name_field;

  function select($type, $form_name, $name, $value)
  {
    $this->name_field = $name.'_name';
    $this->type = $type;
    $this->form_name = $form_name;
    $this->name = $name;
    $this->value = $value;
  }

  function print_link($text)
  {
    global $GO_THEME, $GO_CONFIG;

    if ($this->type == 'user')
    {
      $argument = 'show_users=true';
    }else
    {
      $argument = 'show_contacts=true';
    }

    $link = 'javascript:popup(\''.$GO_CONFIG->control_url.
	'select/select.php?'.$argument.'&multiselect=false&GO_HANDLER='.
	base64_encode($GO_CONFIG->control_url.'select/add.php?type='.$this->type.
	  '&id_field=document.'.$this->form_name.'.'.$this->name.
	  '&name_field=document.'.$this->form_name.'.'.$this->name_field).
	'&pass_value=id&handler_base64_encoded=true\',\'600\',\'400\');';

    echo 	'<a class="normal" href="'.$link.'"><img align="bsmiddle" src="'.
      $GO_THEME->images['addressbook_small'].'" width="16" height="16" '.
      'border="0" style="margin-right: 5px;" />'.$text.'</a>';
  }

  function print_field()
  {
    global $GO_THEME, $GO_CONFIG, $strDeleteItem, $GO_USERS;

    $name_field_value = '';
    if ($this->value > 0)
    {
      if ($this->type == 'user')
      {
	if ($user = $GO_USERS->get_user($this->value))
	{
	  $middle_name = $user['middle_name'] == '' ? '' : $user['middle_name'].' ';
	  $name_field_value = htmlspecialchars($user['last_name'].' '.$middle_name.$user['first_name']);
	}
      }else
      {
	$ab = new addressbook();
	if ($contact = $ab->get_contact($this->value))
	{
	  $middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
	  if (!empty($contact['company_name']))
		  $name_field_value = htmlspecialchars($contact['last_name'].' '.$middle_name.$contact['first_name'] . ' / ' . $contact['company_name']);
	  else	
	  	$name_field_value = htmlspecialchars($contact['last_name'].' '.$middle_name.$contact['first_name']);
	}
      }
    }
    echo 	'<input type="hidden" value="'.$this->value.'" name="'.$this->name.'" />'.
      '<input type="text" name="'.$this->name_field.'" class="textbox" style="width: 300px;" value="'.$name_field_value.'" disabled />';

    echo 	'<a href="javascript:clear_'.$this->name.'()" title="'.$strDeleteItem.
      '"><img src="'.$GO_THEME->images['delete'].'" align="absmiddle" border="0"></a>';

    echo 	'<script type="text/javascript">'.
      'function clear_'.$this->name.'(){'.
	'document.'.$this->form_name.'.'.$this->name.'.value="";'.
	  'document.'.$this->form_name.'.'.$this->name_field.'.value="";}'.
	  '</script>';
  }
}
?>
