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

/**
 * Addressbook class.
 */
class addressbook extends db
{
  var $selector_is_enabled = false;

  /**
   * Constructor.
   */
  function addressbook()
  {
    $this->db();
  }

  function parse_address($address)
  {
  	$address = trim($address);

  	$address_arr['housenumber'] = '';
		$address_arr['street'] = $address;
				
  	if($address != '')
  	{
			$last_space = strrpos($address, ' ');
			
			if($last_space !== false)
			{
				$address_arr['housenumber'] = substr($address, $last_space+1);
				$address_arr['street'] = substr($address, 0, $last_space);
				
			}
		}
  	return $address_arr;  
  }
  
  function get_addressinfo($zip)
  {
  	$sql = "SELECT * FROM ab_zipcodes WHERE zip LIKE '$zip%'";
  	$this->query($sql);
  	if($this->next_record())
  	{
  		return $this->Record;
  	}
  	return false;
  }
  
  function address_format($contact_id)
  {
  	if($contact = $this->get_contact($contact_id))
  	{
  		$middle_name = $contact['middle_name'] == '' ? '' : $contact['middle_name'].' ';
		  $name = $contact['first_name'].' '.$middle_name.$contact['last_name'];
		  
		  $address = $name.'<br />';
		  
		  if($contact['address'] != '')
		  {
		  	$address .= $contact['address'].'<br />';
		  }
		  if($contact['zip'] != '')
		  {
		  	$address .=	$contact['zip'].' ';
			}
			if($contact['city'] != '')
		  {
		  	$address .= $contact['city'].'<br />';
		  }
		  if($contact['country'] != '')
		  {
		  	$address .= $contact['country'].'<br />';
		  }
		  return $address;
  	}  	
  	return false;
  }
  
  /**
   * Save contact sort preferences of user $user_id.
   *
   * @return true/false.
   */
  function set_contacts_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE ab_settings SET sort_contacts_field='$sort_field' , ".
      "sort_contacts_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
   * Save company sort preferences of user $user_id.
   *
   * @return true/false.
   */
  function set_companies_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE ab_settings SET sort_companies_field='$sort_field' , ".
      "sort_companies_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
   * Save user sort preferences of user $user_id.
   *
   * @return true/false.
   */
  function set_users_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE ab_settings SET sort_users_field='$sort_field' , ".
      "sort_users_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
   * Save user/contact/company search preferences of user $user_id.
   *
   * @return true/false.
   */
  function set_search($user_id, $search_type, $search_contacts_field, 
      $search_companies_field, $search_users_field,$search_addressbook_id)
  {
    $sql = "UPDATE ab_settings SET search_type='$search_type' , ".
      "search_contacts_field='$search_contacts_field' ,".
      "search_companies_field='$search_companies_field',".
      "search_users_field='$search_users_field',".
      "search_addressbook_id='$search_addressbook_id' ".
      "WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
   * Get user/contact/company search/sort preferences of user $user_id.
   * Add new one if there is no preferences for the user.
   *
   * @return the setting record.
   */
  function get_settings($user_id)
  {
    $this->query("SELECT * FROM ab_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO ab_settings ".
	  "(user_id, sort_contacts_field, sort_contacts_order, sort_companies_field, ".
	  "sort_companies_order, sort_users_field, sort_users_order, ".
	  "search_type, search_contacts_field, ".
	  "search_companies_field, search_users_field) VALUES ".
	  "('$user_id', 'name', 'ASC', 'name', 'ASC', 'name', 'ASC',".
	  "'contacts','ab_contacts.first_name', 'ab_companies.name', 'users.first_name')");
      return $this->get_settings($user_id);
    }		
  }

  /**
   * Return an URL (javascript url exactly) that calls open_addressbook()
   * or die() if contact_selector is not enabled.
   *
   * @return the link string or die().
   */
  function select_contacts($target_field, $handler_file, $multiselect='true',
      $require_email_address='true', $show_users='true',
      $show_contacts='true', $show_companies='true',
      $pass_value='email')
  {
    if (!$this->selector_is_enabled)
    {
      die('FATAL ERROR: called select contacts while contact_selector is not'.
	  'enabled. Call enable_contact_selector() first outside a form!');
    }else
    {
      $link =  "javascript:open_addressbook('$target_field',";
      if ($target_field != '')
      {
	$link .= $target_field.'.value';
      }else
      {
	$link .= "''";
      }
      $link .= ", '$handler_file', '$multiselect', '$require_email_address', ".
	"'$show_users', '$show_contacts', '$show_companies', ".
	"'$pass_value');";

      return $link;
    }
  }

  /**
   * Enable contact selector.
   * This is a form that submits to classes/base/controls/select/select.php.
   * open_addressbook() function is included beside the form.
   *
   * @return none
   */
  function enable_contact_selector()
  {
    global $GO_CONFIG;

    echo '
      <form name="ab_form" action="'.$GO_CONFIG->control_url.'select/select.php" method="post" target="ab_select">
      <input type="hidden" name="address_string" />
      <input type="hidden" name="GO_HANDLER" />
      <input type="hidden" name="GO_FIELD" />
      <input type="hidden" name="multiselect" />
      <input type="hidden" name="require_email_address" />
      <input type="hidden" name="show_users" />
      <input type="hidden" name="show_contacts" />
      <input type="hidden" name="show_companies" />
      <input type="hidden" name="pass_value" />
      </form>

      <script type="text/javascript">
      function open_addressbook(field, addresses, handler_file, multiselect, require_email_address, show_users, show_contacts, show_companies, pass_value)
      {
	var width = 600;
	var height = 400;
	var centered;
	x = (screen.availWidth - width) / 2;
	y = (screen.availHeight - height) / 2;
	centered = ",width=" + width + ",height=" + height + ",left=" + x + ",top=" + y + ",scrollbars=yes,resizable=yes,status=no";
	var popup = window.open("about:blank", "ab_select", centered);

	document.ab_form.address_string.value = addresses;
	document.ab_form.GO_HANDLER.value = handler_file;
	document.ab_form.GO_FIELD.value = field;
	document.ab_form.multiselect.value = multiselect;
	document.ab_form.require_email_address.value = require_email_address;
	document.ab_form.show_users.value = show_users;
	document.ab_form.show_contacts.value = show_contacts;
	document.ab_form.show_companies.value = show_companies;
	document.ab_form.pass_value.value = pass_value;
	document.ab_form.submit();

	if (!popup.opener) popup.opener = self;
	popup.focus();
      }
    </script>
      ';
    $this->selector_is_enabled = true;
  }

  /**
   * Get default addressbook of user $user_id.
   * That is the addressbook in ab_subscribed table with standard field=1
   *
   * @return addressbook id or false.
   */
  function get_default_addressbook($user_id,$auto_add=true)
  {
    global $GO_SECURITY;
    $sql = "SELECT addressbook_id FROM ab_subscribed WHERE user_id='$user_id' AND standard='1'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->f('addressbook_id');
    }elseif($auto_add)
    {
			$new_ab_name = $_SESSION['GO_SESSION']['name'];
			$x = 1;
			while($this->get_addressbook_by_name($new_ab_name))
			{
				$new_ab_name = $_SESSION['GO_SESSION']['name'].' ('.$x.')';
				$x++;
			}
			$addressbook_id = $this->add_addressbook($GO_SECURITY->user_id, addslashes($new_ab_name));
			return $addressbook_id;
    }
    return false;
  }

  /**
   * Set default addressbook $addressbook_id for  user $user_id
   *
   * @return true if successful, false otherwise.
   */
  function set_default_addressbook($user_id, $addressbook_id)
  {
    $sql = "UPDATE ab_subscribed SET standard='0' WHERE user_id='$user_id' AND standard='1'";
    if ($this->query($sql))
    {
      return $this->query("UPDATE ab_subscribed SET standard='1' WHERE user_id='$user_id' AND addressbook_id='$addressbook_id'");
    }
    return false;
  }

  /**
   * Get all addressbooks, sorted by name.
   *
   * @return number of addressbooks.
   */
  function get_addressbooks()
  {
    $sql = "SELECT * FROM ab_addressbooks ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get all addressbooks of user $user_id.
   *
   * @return number of addressbooks.
   */
  function get_user_addressbooks($user_id)
  {
    $sql = "SELECT DISTINCT ab_addressbooks.* FROM ab_addressbooks, acl, users_groups WHERE (".
      "ab_addressbooks.acl_read = acl.acl_id OR ab_addressbooks.acl_write = acl.acl_id".
      ") AND ( ( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id." ) ) ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }


  /**
   * Add new addressbook named $name for user $user_id
   * Addressbook id is from nextid "ab_addressbooks".
   *
   * @return the addressbook id or false.
   */
  function add_addressbook($user_id, $name)
  {
    global $GO_SECURITY;

    $addressbook_id = $this->nextid('ab_addressbooks');
    $acl_read = $GO_SECURITY->get_new_acl('acl_read addressbook_id: '.$addressbook_id);
    $acl_write = $GO_SECURITY->get_new_acl('acl_write addressbook_id: '.$addressbook_id);

    if($addressbook_id > 0 && $acl_read > 0 && $acl_write > 0)
    {
      $sql = "INSERT INTO ab_addressbooks (id, user_id, name, acl_read, acl_write) VALUES ('$addressbook_id', '$user_id', '$name', '$acl_read', '$acl_write')";
      if($this->query($sql))
      {
	$GO_SECURITY->add_user_to_acl($user_id, $acl_write);

	$this->subscribe($user_id, $addressbook_id);

	if (!$this->get_default_addressbook($user_id,false))
	{
	  $this->set_default_addressbook($user_id, $addressbook_id);
	}
	return $addressbook_id;
      }
    }else
    {
      $GO_SECURITY->delete_acl($acl_read);
      $GO_SECURITY->delete_acl($acl_write);
    }
    return false;
  }

  /**
   * Update name of addressbook $addressbook_id 
   * return true if successful, false otherwise.
   */
  function update_addressbook($addressbook_id, $name)
  {
    $sql = "UPDATE ab_addressbooks SET name='$name' WHERE id='$addressbook_id'";
    return $this->query($sql);
  }

  /**
   * Get addressbook by name $name.
   * Return the record of the addressbook, false otherwise.
   */
  function get_addressbook_by_name($name)
  {
    $sql = "SELECT * FROM ab_addressbooks WHERE name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  /**
   * Delete addressbook $addressbook_id
   */
  function delete_addressbook($addressbook_id)
  {
    global $GO_SECURITY;

    $ab = new addressbook();

    $sql = "SELECT * FROM ab_contacts WHERE addressbook_id='$addressbook_id'";
    $this->query($sql);
    while($this->next_record())
    {
      $GO_SECURITY->delete_acl($this->f('acl_read'));
      $GO_SECURITY->delete_acl($this->f('acl_write'));
      $ab->delete_contact($this->f('id'));
    }

    $sql = "SELECT * FROM ab_companies WHERE addressbook_id='$addressbook_id'";
    $this->query($sql);
    while($this->next_record())
    {
      $GO_SECURITY->delete_acl($this->f('acl_read'));
      $GO_SECURITY->delete_acl($this->f('acl_write'));
      $ab->delete_contact($this->f('id'));
    }

    if ($this->query("DELETE FROM ab_subscribed WHERE addressbook_id='$addressbook_id'"))
    {
      $sql = "DELETE FROM ab_addressbooks WHERE id='$addressbook_id'";
      return $this->query($sql);
    }
  }

  /**
   * Get addressbook $addressbook_id.
   * Return the record of the addressbook, false otherwise.
   */
  function get_addressbook($addressbook_id)
  {
    $sql = "SELECT * FROM ab_addressbooks WHERE id='$addressbook_id'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Get all subscribed addressbook of user $user_id.
   * Return number of addressbooks.
   */
  function get_subscribed_addressbooks($user_id)
  {
    $sql = "SELECT ab_addressbooks.* FROM ab_addressbooks LEFT JOIN ab_subscribed ON (ab_subscribed.addressbook_id=ab_addressbooks.id) WHERE ab_subscribed.user_id='$user_id' ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Subscribe addressbook $addressbook_id to user $user_id.
   * Return true if successful, false otherwise.
   */
  function subscribe($user_id, $addressbook_id)
  {
    $sql = "INSERT INTO ab_subscribed (user_id, addressbook_id) VALUES ('$user_id', '$addressbook_id')";
    return $this->query($sql);
  }

  /**
   * Check if user $user_id has subscribed addressbook $addressbook_id.
   * Return the addressbook record, false otherwise.
   */
  function is_subscribed($user_id, $addressbook_id)
  {
    $sql = "SELECT addressbook_id FROM ab_subscribed WHERE user_id='$user_id' AND addressbook_id='$addressbook_id'";
    $this->query($sql);
    return $this->next_record();
  }

  /**
   * Unsubscribe all addressbooks from user $user_id.
   * Return true if successful, false otherwise.
   */
  function unsubscribe_all($user_id)
  {
    $sql = "DELETE FROM ab_subscribed WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  /**
   * Unsubscribe addressbook $addressbook_id from user $user_id.
   * Return true if successful, false otherwise.
   */
  function unsubscribe($user_id, $addressbook_id)
  {
    $sql = "DELETE FROM ab_subscribed WHERE user_id='$user_id' AND addressbook_id='$addressbook_id'";
    return $this->query($sql);
  }

  /**
   * Get contacts whose field $field match query string $query in addressbook $addressbook_id. 
   * Only get $max_rows contacts starting from $first.
   *
   * @param
   * $user_id whose addressbooks will be searched for.
   * $query the pattern to be matched. '*' represents an abitrary string (will be replaced by % in the function).
   * $field Default is 'last_name'
   * $addressbook_id the addressbook to be searched for. Default is all subscribed addressbooks of user $user_id (zero value)
   * $first .
   * $max_rows the number of contacts. Default is all contacts (zero value).
   * @return the number of all selected contacts (assuming $first=0 and $max_rows=0)
   */
  function search_contacts($user_id, $query, $field='last_name', $addressbook_id=0,$first=0,$max_rows=0)
  {
    $query = str_replace('*', '%', $query);

    $sql = "SELECT DISTINCT ab_contacts.* FROM ab_contacts ";

    if (strpos($field, 'ab_custom_contact_fields') !== false)
    {
      $sql .= "INNER JOIN ab_custom_contact_fields ON ab_custom_contact_fields.id=ab_contacts.id ";
    }

    if ($addressbook_id > 0)
    {
      $sql .= "WHERE ab_contacts.addressbook_id='$addressbook_id' AND ";
    }else
    {
      $sql .=	"INNER JOIN ab_subscribed ON (ab_subscribed.addressbook_id=ab_contacts.addressbook_id) WHERE ab_subscribed.user_id='$user_id' AND ";
    }

    $sql .= "($field LIKE '$query' ";
    // Additional Vietnamese glyphs
    if ($query == 'Đ%')
      $sql .= " OR $field LIKE 'đ%' ";
    $sql .=  ") ORDER BY ab_contacts.first_name ASC, ab_contacts.last_name ASC";
    if ($max_rows != 0) {
      $this->query($sql);
      $count = $this->num_rows();
      $sql .= " LIMIT $first, $max_rows";
    }
    $this->query($sql);
    return isset($count) ? $count : $this->num_rows();
  }

  /**
   * Unimplemented function. 
   */
  function get_contacts_by_birth_day($interval_start, $interval_end)
  {
	
  }

  /**
   * Get companies whose field $field match query string $query in addressbook $addressbook_id. 
   * Only get $max_rows companies starting from $first.
   *
   * @param
   * $user_id whose addressbooks will be searched for.
   * $query the pattern to be matched. '*' represents an abitrary string (will be replaced by % in the function).
   * $field Default is 'last_name'
   * $addressbook_id the addressbook to be searched for. Default is all subscribed addressbooks of user $user_id (zero value)
   * $category limit the search in this category or search in all categories (value -1, also the default value)
   * $first .
   * $max_rows the number of companies. Default is all companies (zero value).
   * @return the number of all selected contacts (assuming $first=0 and $max_rows=0)
   */
  function search_companies($user_id, $query, $field='name', $addressbook_id=0, $category=-1,$first=0,$max_rows=0)
  {
    $query = str_replace('*', '%', $query);

    $sql = "SELECT DISTINCT ab_companies.* FROM ab_companies ";
	
	if ($category > -1)
		$sql.="INNER JOIN ab_cate_companies ON ab_cate_companies.company_id = ab_companies.id ";

    if (strpos($field, 'ab_custom_company_fields') !== false)
    {
      $sql .= "INNER JOIN ab_custom_company_fields ON ab_custom_company_fields.id=ab_companies.id ";
    }
    if ($addressbook_id > 0)
    {
      $sql .= "WHERE ab_companies.addressbook_id='$addressbook_id' AND ";
    }else
    {
      $sql .=	"INNER JOIN ab_subscribed ON (ab_subscribed.addressbook_id=ab_companies.addressbook_id) WHERE ab_subscribed.user_id='$user_id' AND ";
    }
	
	if ($category > -1)
		$sql.= " ab_cate_companies.category_id = '$category' AND ";

    $sql .= "($field LIKE '$query' ";
    // Additional Vietnamese glyphs
    if ($query == 'Đ%')
      $sql .= " OR $field LIKE 'đ%' ";
    $sql .=  ") ORDER BY ab_companies.name ASC";
    if ($max_rows != 0) {
      $this->query($sql);
      $count = $this->num_rows();
      $sql .= " LIMIT $first, $max_rows";
    }
    $this->query($sql);
    return isset($count) ? $count : $this->num_rows();
  }

  /**
   * Get all authorised contacts from addressbook $addressbook_id of user $user_id, sorted by $sort with order $direction.
   *
   * $addressbook_id addressbook id.
   * $sort sort field.
   * $direction sort direction (ASC or DESC).
   * $user default is 0 (current user).
   * @return the number of contacts.
   */
  function get_contacts($addressbook_id, $sort = "name", $direction = "ASC", $user_id=0)
  {
    global $GO_SECURITY;

    if ($user_id == 0)
    {
      $user_id = $GO_SECURITY->user_id;
    }
    if($sort == 'name')
    {
      $sort = 'first_name '.$direction.', last_name';
    }
    $sql = "SELECT DISTINCT * FROM ab_contacts ".
      "INNER JOIN acl ON (ab_contacts.acl_read = acl.acl_id OR ab_contacts.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
      " WHERE ((users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id.")) ";
    "AND ab_contacts.addressbook_id='$addressbook_id' ".
      "ORDER BY $sort $direction";

    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get all contacts of addressbook $addressbook_id, user $user_id.
   *
   * @return number of contacts.
   */
  function get_user_contacts($user_id, $addressbook_id)
  {
    $this->query("SELECT * FROM ab_contacts WHERE addressbook_id='$addressbook_id' AND user_id='$user_id'");
    return $this->num_rows();
  }

  /**
   * Get all companies of addressbook $addressbook_id, user $user_id.
   *
   * @return number of companies.
   */
  function get_user_companies($user_id, $addressbook_id)
  {
    $this->query("SELECT * FROM ab_companies WHERE addressbook_id='$addressbook_id' AND user_id='$user_id'");
    return $this->num_rows();
  }

  /**
   * Get a range of contacts associated with company $company_id, sorted by field $sort.
   *
   * $company_id the company id
   * $sort sorted field.
   * $direction sort direction.
   * $start the first contact of the range.
   * $offset number of contacts of the range.
   * @return number of all contacts (independent with $start/$offset).
   */
  function get_company_contacts($company_id, $sort="name", $direction="ASC", $start, $offset)
  {
    if($sort == 'name')
    {
      $sort = 'first_name '.$direction.', last_name';
    }
    $sql = "SELECT * FROM ab_contacts WHERE company_id='$company_id' ORDER BY $sort $direction";

    if ($offset != 0)
    {
      $sql .= " LIMIT $start, $offset";

      $sql2= "SELECT * FROM ab_contacts WHERE company_id='$company_id'";

      $this->query($sql2);
      $count = $this->num_rows();

      if ($count > 0)
      {
	$this->query($sql);
	return $count;
      }
      return 0;

    }else
    {
      $this->query($sql);
      return $this->num_rows();
    }
  }

  /**
   * Get all contact-related information from addressbook $addressbook_id of user $user_id.
   * 
   * @param $addressbook_id Addressbook id.
   *        $user_id        User id. Default is the current user.
   * @return number of contacts.
   */
  function get_contacts_for_export($addressbook_id, $user_id=0)
  {
    global $GO_SECURITY;

    if ($user_id == 0)
    {
      $user_id = $GO_SECURITY->user_id;
    }
    $sql = "SELECT DISTINCT ab_contacts.*, ab_groups.name AS group_name,".
      "ab_companies.name AS company FROM ab_contacts ".
      "INNER JOIN acl ON (ab_contacts.acl_read = acl.acl_id OR ab_contacts.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
      "LEFT JOIN ab_groups ON (ab_contacts.group_id=ab_groups.id) ".
      "LEFT JOIN ab_companies ON (ab_contacts.company_id=ab_companies.id) ".
      " WHERE ((users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id.")) ".
      "AND ab_contacts.addressbook_id='$addressbook_id' ".
      "ORDER BY ab_contacts.first_name, ab_contacts.last_name ASC";

    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get all authorised contacts of group $group_id, addressbook $addressbook_id, $user_id, sorted by $sort with direction $direction.
   * 
   * @param $addressbook_id
   * @param $group_id
   * @param $sort sort field (default is 'name', which consists of first name and last name)
   * @param $direction default is ASC.
   * @param $user_id Default is the current user.
   * @return the number of contacts.
   */
  function get_contacts_group($addressbook_id, $group_id, $sort = "name", $direction = "ASC", $user_id=0)
  {
    global $GO_SECURITY;
    if ($user_id == 0)
    {
      $user_id = $GO_SECURITY->user_id;
    }
    if($sort == 'name')
    {
      $sort = 'first_name '.$direction.', last_name';
    }
    $sql = "SELECT DISTINCT ab_contacts.* FROM ab_contacts ".
      "INNER JOIN acl ON (ab_contacts.acl_read = acl.acl_id OR ab_contacts.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
      "WHERE ((users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id.")) ".
      "AND ab_contacts.addressbook_id='$addressbook_id' ".
      "AND ab_contacts.group_id='$group_id' ".
      "ORDER BY $sort $direction";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get all contacts that contact name is $name from $addressbook_id.
   *
   * @return the record or false. 
   */
  function get_contact_by_name($addressbook_id, $name)
  {
    $sql = "SELECT * FROM ab_contacts WHERE addressbook_id='$addressbook_id' AND name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Get contact $contact_id that associated with a company.
   * 
   * @return the record contains both contact (overrided with some company information) or false.
   */
  function get_contact($contact_id)
  {
    $this->query("SELECT ab_contacts.*, ab_companies.address AS work_address, ab_companies.zip AS work_zip, ".
	"ab_companies.city AS work_city, ab_companies.state AS work_state, ".
	"ab_companies.country AS work_country, ab_companies.homepage, ".
	"ab_companies.bank_no, ab_companies.email AS company_email, ".
	"ab_companies.phone AS company_phone, ab_companies.fax AS company_fax, ".
	"ab_companies.name AS company_name FROM ab_contacts ".
	"LEFT JOIN ab_companies ON (ab_contacts.company_id=ab_companies.id) WHERE ab_contacts.id='$contact_id'");

    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Add a new contact.
   * Contact id is created from nextid "contacts"
   * 
   * @param $source_id if this contact is created from user ID then $source_id is ID
   * @param $user_id
   * @param $addressbook_id
   * @param $first_name
   * @param $middle_name
   * @param $last_name
   * @param $initials
   * @param $title
   * @param $sex
   * @param $birthday
   * @param $email
   * @param $work_phone default is empty
   * @param $home_phone default is empty
   * @param $fax default is empty
   * @param $cellular default is empty
   * @param $country default is empty
   * @param $state default is empty
   * @param $city default is empty
   * @param $zip default is empty
   * @param $address default is empty
   * @param $company_id Default is zero (no company)
   * @param $work_fax default is empty
   * @param $department default is empty
   * @param $function default is empty
   * @param $comment default is empty
   * @param $group_id Default is zero (Group "other")
   * @param $color default is empty
   * @param $acl_read
   * @param $acl_write
   * @return the new contact id or false.
   */
  function add_contact($source_id, $user_id, $addressbook_id, $first_name, $middle_name, $last_name, $initials, $title, $sex, $birthday, $email, $work_phone="", $home_phone="", $fax="", $cellular="", $country="", $state="", $city="", $zip="", $address="", $company_id=0, $work_fax = "", $department="", $function="", $comment="", $group_id=0, $color='', $acl_read, $acl_write)
  {
    $contact_id = $this->nextid("contacts");
    if ($contact_id > 0)
    {
      $sql = "INSERT INTO ab_contacts ";
      $sql .= "(id, user_id, source_id, addressbook_id, first_name, middle_name, last_name, initials, title, sex, birthday, email, work_phone, home_phone, fax, cellular, country, state, city, zip, address, company_id, work_fax, department, function, comment, group_id, color, acl_read, acl_write) VALUES ";
      $sql .= "('$contact_id', '$user_id', '$source_id', '$addressbook_id', '$first_name', '$middle_name', '$last_name', '$initials', '$title', '$sex', '$birthday', '$email', '$work_phone', '$home_phone', '$fax', '$cellular', '$country', '$state', '$city', '$zip', '$address', '$company_id', '$work_fax', '$department', '$function','$comment', '$group_id', '$color', '$acl_read', '$acl_write')";
      $query = $this->query($sql);
      if ($this->affected_rows() > 0)
      {
	$this->query("INSERT INTO ab_custom_contact_fields (id) VALUES ('$contact_id')");
	return $contact_id;
      }else
      {
	return false;
      }
    }else
    {
      return false;
    }
  }

  /**
   * Update contact with id $id. 
   *
   * @param $id contact id
   * @param $addressbook_id
   * @param $first_name
   * @param $middle_name
   * @param $last_name
   * @param $initials
   * @param $title
   * @param $sex
   * @param $birthday
   * @param $email
   * @param $work_phone
   * @param $home_phone
   * @param $fax
   * @param $cellular
   * @param $country
   * @param $state
   * @param $city
   * @param $zip
   * @param $address
   * @param $company_id
   * @param $work_fax
   * @param $department
   * @param $function
   * @param $comment default is empty
   * @param $group_id default is zero (group "other")
   * @param $color
   * @return true/false.
   */
  function update_contact($id, $addressbook_id, $first_name, $middle_name, $last_name, $initials, $title, $sex, $birthday, $email, $work_phone, $home_phone, $fax, $cellular, $country, $state, $city, $zip, $address, $company_id, $work_fax, $department, $function, $comment='', $group_id='0', $color)
  {
    $sql = "UPDATE ab_contacts SET ";
    $sql .= "addressbook_id='$addressbook_id', first_name='$first_name', middle_name='$middle_name', last_name='$last_name', initials='$initials', title='$title', sex='$sex', birthday='$birthday', email='$email', work_phone='$work_phone', home_phone='$home_phone', fax='$fax', cellular='$cellular', state='$state'";
    $sql .= ", country='$country', city='$city', zip='$zip', address='$address', company_id='$company_id', department='$department', function='$function', work_fax='$work_fax', comment='$comment', group_id='$group_id', color='$color'";
    $sql .= " WHERE id='$id'";
    $query = $this->query($sql);
    if (isset($query))
    {
      return true;
    }else
    {
      return false;
    }
  }

  /**
   * Delete contact $contact_id.
   * 
   * Return true/false.
   */
  function delete_contact($contact_id)
  {

    $sql1 = "DELETE FROM tp_mailing_contacts WHERE contact_id='$contact_id'";
    $sql2 = "DELETE FROM ab_custom_contact_fields WHERE id='$contact_id'";

    if ($this->query($sql1) && $this->query($sql2))
    {
      return $this->query("DELETE FROM ab_contacts WHERE id='$contact_id'");
    }
  }

  /**
   * Check if contact $id is created from user $user_id.
   * 
   * @return the contact record or false.
   */
  function user_is_contact($user_id, $id)
  {
    $sql = "SELECT ab_contacts.* FROM ab_contacts ".
      "LEFT JOIN ab_addressbooks ".
      "ON (ab_addressbooks.id=ab_contacts.addressbook_id) ".
      "WHERE ab_contacts.source_id='$id' ".
      "AND ab_addressbooks.user_id='$user_id'";

    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  /**
   * Check if user $user_id is in contacts of addressbook $addressbook_id.
   * 
   * @return true/false.
   */
  function user_is_in_addressbook($user_id, $addressbook_id)
  {
    $sql = "SELECT id FROM ab_contacts WHERE source_id='$user_id' AND addressbook_id='$addressbook_id'";
    $this->query($sql);
    return $this->next_record();
  }

  /**
   * Get contact id whose email is $email and the contact is belonged to an addressbook of user $user_id.
   * 
   * @return the contact id or false.
   * @see get_contact_profile_by_email
   */
  function get_contact_id_by_email($email, $user_id)
  {
    $sql = "SELECT ab_contacts.id FROM ab_contacts ".
      "LEFT JOIN ab_addressbooks ".
      "ON (ab_addressbooks.id=ab_contacts.addressbook_id) ".
      "WHERE ab_contacts.email='".$email."' ".
      "AND ab_addressbooks.user_id='$user_id'";

    $this->query($sql);
    if ($this->next_record())
      return $this->f("id");
    else
      return false;
  }

  /**
   * Get contact id whose email is $email and the contact is belonged to an addressbook of user $user_id.
   * 
   * @return the contact record or false.
   */
  function get_contact_profile_by_email($email, $user_id)
  {
    $sql = "SELECT ab_contacts.* FROM ab_contacts LEFT JOIN ab_addressbooks ON".
      " (ab_addressbooks.id=ab_contacts.addressbook_id) ".
      "WHERE ab_contacts.email='$email' ".
      "AND ab_addressbooks.user_id='$user_id'";
    $this->query($sql);
    if ($this->next_record())
      return $this->Record;
    else
      return false;
  }

  /**
   * Get all contact groups of addressbook $addressbook_id.
   *
   * @return number of groups. 
   */
  function get_groups($addressbook_id)
  {
    $sql= "SELECT * FROM ab_groups WHERE addressbook_id='$addressbook_id'";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Add a new group with name $name to addressbook $addressbook_id.
   * The name will be htmlentities(ed) inside the function.
   * Group id is in nextid "ab_groups"
   *
   * @return the group id or false.
   */
  function add_group($addressbook_id, $name)
  {
    $group_id = $this->nextid("ab_groups");
    if ($group_id > 0)
    {
      $sql = "INSERT INTO ab_groups (id, addressbook_id, name) VALUES ".
	"('$group_id', '$addressbook_id', '$name')";
      $this->query($sql);
      return $group_id;
    }else
    {
      return false;
    }
  }

  /**
   * Delete group $group_id.
   * 
   * @return true/false.
   */
  function delete_group($group_id)
  {
    $sql = "UPDATE ab_contacts SET group_id='0' WHERE group_id='$group_id'";
    if ($this->query($sql))
    {
      $sql = "DELETE FROM ab_groups WHERE id='$group_id'";
      $this->query($sql);
      return true;
    }
    return false;
  }

  /**
   * Move contact $contact_id to group $group_id.
   *
   * @return true/false. 
   */
  function move_contact_to_group($contact_id, $group_id)
  {
    $sql = "UPDATE ab_contacts SET group_id='$group_id' WHERE id='$contact_id'";
    $this->query($sql);
  }

  /**
   * Clear group $group_id (Move all contacts in group $group_id to group "other").
   *
   * @return true/false.
   */
  function clear_group($group_id)
  {
    $sql = "UPDATE ab_contacts SET group_id='0' WHERE group_id='$group_id'";
    $this->query($sql);
  }

  /**
   * Rename group $group_id with new name $name.
   * 
   * @return true/false.
   */
  function change_group_name($group_id, $name)
  {
    $sql = "UPDATE ab_groups SET name='$name' WHERE id='$group_id'";
    $this->query($sql);
  }

  /**
   * Get the record of group $group_id.
   * 
   * @return the group record or false.
   */
  function get_group($group_id)
  {
    $sql= "SELECT * FROM ab_groups WHERE id='$group_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Get group by name $name in addressbook $addressbook_id.
   * 
   * @return the group record or false.
   */
  function get_group_by_name($addressbook_id, $name)
  {
    $sql= "SELECT * FROM ab_groups WHERE addressbook_id='$addressbook_id' AND name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Add contact $contact_id to company $company_id.
   *
   * @return true/false.
   */
  function add_contact_to_company($contact_id, $company_id)
  {
    return $this->query("UPDATE ab_contacts SET company_id='$company_id' WHERE id='$contact_id'");
  }

  /**
   * Add new company to addressbook $addressbook_id, user $user_id.
   * Company id is from nextid "ab_companies".
   * If $user_id is not the current user, $company_id will be added to ab_new_companies.
   * 
   * @param $addressbook_id
   * @param $user_id
   * @param $name
   * @param $shortname
   * @param $engname
   * @param $relation_date
   * @param $address
   * @param $zip
   * @param $city
   * @param $state
   * @param $country
   * @param $email
   * @param $phone
   * @param $fax
   * @param $homepage
   * @param $bank_no,
   * @param $vat_no
   * @param $acl_read
   * @param $acl_write
   * @param $parent_id
   * @return the new company id or false.
   */
  function add_company($addressbook_id, $user_id, $name, $shortname, $engname, $relation_date,
  	$address, $zip, $city, $state, $country, $email, $phone, $fax, $homepage, $bank_no,
      $vat_no, $acl_read, $acl_write, $parent_id)
  {
    $company_id = $this->nextid("ab_companies");
    if ($company_id > 0)
    {
      $sql = "INSERT INTO ab_companies ".
	"(id, user_id, addressbook_id, name, address, zip, city, state, country, email, phone, fax, homepage, bank_no, vat_no, acl_read, acl_write, parent, shortname, engname, relation_date) VALUES ".
	"('$company_id', '$user_id', '$addressbook_id', '$name', '$address', '$zip', '$city', '$state', '$country', '$email', '$phone', '$fax', '$homepage', '$bank_no', '$vat_no', '$acl_read', '$acl_write', '$parent_id', ".
	" '$shortname', '$engname', '$relation_date')";
      if ($this->query($sql))
      {
	global $GO_SECURITY;
	$this->query("INSERT INTO ab_custom_company_fields (id) VALUES ('$company_id')");

	if ($this->query("SELECT * FROM ab_addressbooks WHERE id='$addressbook_id'") && 
	    $this->next_record() &&
	    $GO_SECURITY->user_id != $this->f('user_id'))
	{
	  @$this->query("REPLACE INTO ab_new_companies (addressbook_id,company_id) VALUES('$addressbook_id','$company_id')");
	}

	return $company_id;
      }
    }
    return false;
  }

  /**
   * Update information of company $company_id.
   * If $user_id is not the current user, $company_id will be added to ab_new_companies.
   *
   * @param $company_id
   * @param $addressbook_id
   * @param $name
   * @param $shortname
   * @param $engname
   * @param $relation_date
   * @param $address
   * @param $zip,$city
   * @param $state
   * @param $country
   * @param $email
   * @param $phone
   * @param $fax
   * @param $homepage
   * @param $bank_no
   * @param $vat_no
   * @param $parent_id
   * @return true/false.
   */
  function update_company($company_id, $addressbook_id, $name, $shortname, $engname, $relation_date, 
  	$address, $zip,$city, $state, $country, $email, $phone, $fax,
      $homepage, $bank_no, $vat_no,$parent_id)
  {
    $sql = "UPDATE ab_companies SET ".
      "addressbook_id='$addressbook_id', name='$name', ".
      "address='$address', zip='$zip', city='$city', state='$state', ".
      "country='$country', email='$email', phone='$phone', fax='$fax', ".
      "homepage='$homepage', bank_no='$bank_no', vat_no='$vat_no', parent='$parent_id', ".
      "shortname='$shortname', engname='$engname', relation_date = '$relation_date' WHERE id='$company_id'";

    if ($this->query($sql))
    {
      global $GO_SECURITY;
      if ($this->query("SELECT * FROM ab_addressbooks WHERE id='$addressbook_id'") && 
	  $this->next_record() && $GO_SECURITY->user_id != $this->f('user_id'))
      {	
	@$this->query("REPLACE INTO ab_new_companies (addressbook_id,company_id) VALUES('$addressbook_id','$company_id')");
      }
      return true;
    }
    else
      return false;

  }

  /**
   * Get company record of the company named $name in addressbook $addressbook_id.
   *
   * @return the company record or false.
   */
  function get_company_by_name($addressbook_id, $name)
  {
    $sql = "SELECT * FROM ab_companies WHERE addressbook_id='$addressbook_id' AND name='$name'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Get company record of company $company_id.
   * 
   * @return the company record or false.
   */
  function get_company($company_id)
  {
    $sql = "SELECT * FROM ab_companies WHERE id='$company_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }
    return false;
  }

  /**
   * Get a range of companies.
   * If $user_id is the addressbook owner, then ab_new_companies will be fetched too.
   * 
   * @param $addressbook_id the addressbook storing companies
   * @param $sort sort field. default is 'name' (both last_name and first_name)
   * @param $direction sort direction. default is 'ASC'
   * @param $start the begin of the range.
   * @param $offset the size of the range (default is zero, all companies)
   * @param $user_id the user that gets these information (default is the current user)
   * @return the number of all companies.
   */
  function get_companies($addressbook_id, $sort='name', $direction='ASC', $start=0, $offset=0, $user_id=0)
  {
    global $GO_SECURITY;
    if ($user_id == 0)
    {
      $user_id = $GO_SECURITY->user_id;
    }
    $myab = new addressbook();
    if ($myab->query("SELECT * FROM ab_addressbooks WHERE id='$addressbook_id'") &&
	$myab->next_record())
      $owner = $myab->f('user_id');

    if (isset($owner) && $owner == $GO_SECURITY->user_id)
      $sql = "SELECT DISTINCT ab_companies.*, ab_new_companies.company_id FROM ab_companies ".
	"LEFT JOIN ab_new_companies ON ab_companies.id=ab_new_companies.company_id AND ab_companies.addressbook_id=ab_new_companies.addressbook_id ".
	"INNER JOIN acl ON (ab_companies.acl_read = acl.acl_id OR ab_companies.acl_write = acl.acl_id) ".
	"LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
	"WHERE ((users_groups.user_id='$user_id' AND acl.user_id=0) OR (".
	"acl.group_id=0 AND acl.user_id='$user_id')) ".
	"AND ab_companies.addressbook_id='$addressbook_id' ORDER BY $sort $direction";
    else
      $sql = "SELECT DISTINCT ab_companies.* FROM ab_companies ".
	"INNER JOIN acl ON (ab_companies.acl_read = acl.acl_id OR ab_companies.acl_write = acl.acl_id) ".
	"LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
	"WHERE ((users_groups.user_id='$user_id' AND acl.user_id=0) OR (".
	"acl.group_id=0 AND acl.user_id='$user_id')) ".
	"AND addressbook_id='$addressbook_id'".($sort != 'company_id' ? " ORDER BY $sort $direction" : "");

    if ($offset != 0)
    {
      $sql .= " LIMIT $start, $offset";

    if (isset($owner) && $owner == $GO_SECURITY->user_id)
      $sql2 = "SELECT DISTINCT ab_companies.id FROM ab_companies ".
	"LEFT JOIN ab_new_companies ON ab_companies.id=ab_new_companies.company_id AND ab_companies.addressbook_id=ab_new_companies.addressbook_id ".
	"INNER JOIN acl ON (ab_companies.acl_read = acl.acl_id OR ab_companies.acl_write = acl.acl_id) ".
	"LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
	"WHERE ((users_groups.user_id='$user_id' AND acl.user_id=0) OR (".
	"acl.group_id=0 AND acl.user_id='$user_id')) ".
	"AND ab_companies.addressbook_id='$addressbook_id'";
    else
      $sql2= "SELECT DISTINCT ab_companies.id FROM ab_companies ".
	"INNER JOIN acl ON (ab_companies.acl_read = acl.acl_id OR ab_companies.acl_write = acl.acl_id) ".
	"LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
	"WHERE ((users_groups.user_id='$user_id' AND acl.user_id=0) OR (".
	"acl.group_id=0 AND acl.user_id='$user_id')) ".
	"AND addressbook_id='$addressbook_id'";
      $this->query($sql2);
      $count = $this->num_rows();

      if ($count > 0)
      {
	$this->query($sql);

	return $count;
      }
      return 0;

    }else
    {
      $this->query($sql);

      return $this->num_rows();
    }
  }

  /**
   * Get companiy parents in addressbook_ $addressbook_id of user $user_id.
   *
   * @param $addressbook_id
   * @param $user_id default is zero (the current user).
   * @return the array of parent companies whose keys is ids, values is array of ('id', 'parent', 'name')
   */
  function get_companies_parents($addressbook_id, $user_id=0)
  {
    global $GO_SECURITY;
    if ($user_id == 0)
      $user_id = $GO_SECURITY->user_id;

    $sql = "SELECT DISTINCT id, parent, name FROM ab_companies ".
      "WHERE addressbook_id='$addressbook_id'";

    $this->query($sql);
    
    $ret = array();
    while ($this->next_record()) {
      $ret[$this->f('id')] = array('id' => $this->f('parent'),
				   'parent' => $this->f('parent'),
				   'name' => $this->f('name'));
    }
    return $ret;
  }

  /**
   * Get all companies whose name match pattern '%$name%' in addressbook $addressbook_id.
   *
   * @param $name the company name.
   * @param $addressbook_id addressbook_id.
   * @return number of companies.
   */
  function search_company($name, $addressbook_id)
  {
    $sql = "SELECT * FROM ab_companies WHERE addressbook_id='$addressbook_id' AND name LIKE '%$name%'";
    $this->query($sql);
    return $this->num_rows();
  }

  /**
   * Get the company whose name is $name in addressbook $addressbook_id.
   *
   * @param $name the company name.
   * @param $addressbook_id addressbook_id.
   * @return the company record or false.
   */
  function get_company_id_by_name($name, $addressbook_id)
  {
    $sql = "SELECT id FROM ab_companies WHERE addressbook_id='$addressbook_id' AND name='$name'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->f('id');
    }
    return false;
  }

  /**
   * Delete company $company_id.
   * 
   * @param $company_id.
   * @return true/false.
   */
  function delete_company($company_id)
  {
    $sql = "DELETE FROM ab_companies WHERE id='$company_id'";
    if ($this->query($sql))
    {
	  return $this->query("DELETE FROM ab_cate_companies WHERE company_id='$company_id'");
      return $this->query("DELETE FROM ab_custom_company_fields WHERE id='$company_id'");
    }
  }


  /**
   * Delete user's contacts and user's addressbooks.
   * 
   * @param $user_id.
   * @return none
   */
  function delete_user($user_id)
  {
    $sql = "UPDATE ab_contacts SET source_id='0' WHERE source_id='$user_id'";
    $this->query($sql);

    $ab = new addressbook();

    $sql = "SELECT id FROM ab_addressbooks WHERE user_id='$user_id'";
    $this->query($sql);
    while ($this->next_record())
    {
      $ab->delete_addressbook($this->f('id'));
    }
  }

  /**
   * Get the full company name including parent names.
   * 
   * @param $parents if not null, parent company info will be retrieved from $parents.
   * @return the full name.
   * @see get_companies_parents
   */
  function company_name($parents = null)
  {
    if (is_array($parents)) {
      $seen = array();
      $seen[$this->f('id')] = 1;
      $parent = $this->f('parent');
      $name = $this->f('name');
      while ($parent != 0 && !isset($seen[$parent])) {
	$seen[$parent] = 1;
	if (isset($parents[$parent])) {
	  $name = $parents[$parent]['name'].' - '.$name;
	  $parent = $parents[$parent]['parent'];
	}
	else
	  break;
      }
      return $name;
    }

    $seen = array();
    $seen[$this->f('id')] = 1;
    $parent = $this->f('parent');
    $ab = new addressbook();
    $name = $this->f('name');
    while ($parent != 0 && !isset($seen[$parent]))
    {
      $seen[$parent] = 1;
      $sql = "SELECT id,parent,name FROM ab_companies WHERE id='$parent'";
      $ab->query($sql);
      if ($ab->num_rows() && $ab->next_record())
	$name = $ab->f('name').' - '.$name;
      $parent = $ab->f('parent');
    }
    return $name;
  }

  /**
   * Get all parent companies' links of company $company_id.
   * 
   * @param $company_id
   * @return the html string.
   */
  function parent_companies_urls($company_id)
  {
    global $link_back;
    $seen = array();
    $seen[$this->f('id')] = 1;
    $parent = $this->f('parent');
    $ab = new addressbook();
    $name = "";
    while ($parent != 0 && !isset($seen[$parent]))
    {
     $seen[$parent] = 1;
      $sql = "SELECT id,parent,name FROM ab_companies WHERE id='$parent'";
      $ab->query($sql);
      if ($ab->num_rows() && $ab->next_record())
        $name = '<a class="normal" href="'.$GO_MODULES->url.'company.php?company_id='.$ab->f('id').'&return_to='.rawurlencode($link_back).'">'. $ab->f('name').'</a>&nbsp;'.$name;
      $parent = $ab->f('parent');
    }
    return $name;
  }
  /**
   * Get companies by parent company.

   * @param $addressbook_id
   * @param $parent
   * @param $sort
   * @param $direction
   * @param $start
   * @param $offset
   * @param $user_id
   * @return number of all companies by parent company.
   */
  function get_companies_by_parent($addressbook_id,$parent,$sort='name', $direction='ASC', $start=0, $offset=0,$user_id=0)
  {
    global $GO_SECURITY;
    if ($user_id == 0)
    {
      $user_id = $GO_SECURITY->user_id;
    }
    $sql = "SELECT DISTINCT ab_companies.* FROM ab_companies ".
      "INNER JOIN acl ON (ab_companies.acl_read = acl.acl_id OR ab_companies.acl_write = acl.acl_id) ".
      "LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
      "WHERE ((users_groups.user_id='$user_id' AND acl.user_id=0) OR (".
      "acl.group_id=0 AND acl.user_id='$user_id')) ".
      "AND addressbook_id='$addressbook_id' AND parent='$parent' ORDER BY $sort $direction";
    if ($offset != 0)
    {
      // HACKING BY PCLOUDS
      $sql .= $parent == 0 ?  " LIMIT $start, $offset" : " LIMIT $offset";

      $sql2= "SELECT DISTINCT ab_companies.id FROM ab_companies ".
	"INNER JOIN acl ON (ab_companies.acl_read = acl.acl_id OR ab_companies.acl_write = acl.acl_id) ".
	"LEFT JOIN users_groups ON (acl.group_id = users_groups.group_id) ".
	"WHERE ((users_groups.user_id='$user_id' AND acl.user_id=0) OR (".
	"acl.group_id=0 AND acl.user_id='$user_id')) ".
	"AND addressbook_id='$addressbook_id' AND parent='$parent'";
      $this->query($sql2);
      $count = $this->num_rows();

      if ($count > 0)
      {
	$this->query($sql);
	return $count;
      }
      return 0;
    }
    else
    {
      $this->query($sql);
      return $this->num_rows();
    }
  }

  /**
   * Get count of companies whose parent is $parent.
   * 
   * @param $addressbook_id
   * @param $parent
   * @return the count.
   */
  function get_count_by_parent($addressbook_id,$parent)
  {
    $sql = "SELECT DISTINCT COUNT(ab_companies.id) AS COUNT FROM ab_companies WHERE parent='$parent'";
    if ($this->query($sql) && $this->num_rows()  && $this->next_record())
      return $this->f('COUNT');
    else
      return 0;
  }
}
?>
