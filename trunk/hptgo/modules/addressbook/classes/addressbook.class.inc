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

class addressbook extends db
{
  var $selector_is_enabled = false;

  function addressbook()
  {
    $this->db();
  }
  function set_contacts_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE ab_settings SET sort_contacts_field='$sort_field' , ".
      "sort_contacts_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }
  function set_companies_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE ab_settings SET sort_companies_field='$sort_field' , ".
      "sort_companies_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }
  function set_users_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE ab_settings SET sort_users_field='$sort_field' , ".
      "sort_users_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }
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

  function get_default_addressbook($user_id)
  {
    $sql = "SELECT addressbook_id FROM ab_subscribed WHERE user_id='$user_id' AND standard='1'";
    $this->query($sql);
    if($this->next_record())
    {
      return $this->f('addressbook_id');
    }
    return false;
  }

  function set_default_addressbook($user_id, $addressbook_id)
  {
    $sql = "UPDATE ab_subscribed SET standard='0' WHERE user_id='$user_id' AND standard='1'";
    if ($this->query($sql))
    {
      return $this->query("UPDATE ab_subscribed SET standard='1' WHERE user_id='$user_id' AND addressbook_id='$addressbook_id'");
    }
    return false;
  }

  function get_addressbooks()
  {
    $sql = "SELECT * FROM ab_addressbooks ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function get_user_addressbooks($user_id)
  {
    $sql = "SELECT DISTINCT ab_addressbooks.* FROM ab_addressbooks, acl, users_groups WHERE (".
      "ab_addressbooks.acl_read = acl.acl_id OR ab_addressbooks.acl_write = acl.acl_id".
      ") AND ( ( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
      "acl.group_id = 0 AND acl.user_id = ".$user_id." ) ) ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }


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

	if (!$this->get_default_addressbook($user_id))
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

  function update_addressbook($addressbook_id, $name)
  {
    $sql = "UPDATE ab_addressbooks SET name='$name' WHERE id='$addressbook_id'";
    return $this->query($sql);
  }

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

  function get_subscribed_addressbooks($user_id)
  {
    $sql = "SELECT ab_addressbooks.* FROM ab_addressbooks LEFT JOIN ab_subscribed ON (ab_subscribed.addressbook_id=ab_addressbooks.id) WHERE ab_subscribed.user_id='$user_id' ORDER BY name ASC";
    $this->query($sql);
    return $this->num_rows();
  }

  function subscribe($user_id, $addressbook_id)
  {
    $sql = "INSERT INTO ab_subscribed (user_id, addressbook_id) VALUES ('$user_id', '$addressbook_id')";
    return $this->query($sql);
  }

  function is_subscribed($user_id, $addressbook_id)
  {
    $sql = "SELECT addressbook_id FROM ab_subscribed WHERE user_id='$user_id' AND addressbook_id='$addressbook_id'";
    $this->query($sql);
    return $this->next_record();
  }

  function unsubscribe_all($user_id)
  {
    $sql = "DELETE FROM ab_subscribed WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function unsubscribe($user_id, $addressbook_id)
  {
    $sql = "DELETE FROM ab_subscribed WHERE user_id='$user_id' AND addressbook_id='$addressbook_id'";
    return $this->query($sql);
  }

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

  function get_contacts_by_birth_day($interval_start, $interval_end)
  {
	
  }

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

  function get_user_contacts($user_id, $addressbook_id)
  {
    $this->query("SELECT * FROM ab_contacts WHERE addressbook_id='$addressbook_id' AND user_id='$user_id'");
    return $this->num_rows();
  }

  function get_user_companies($user_id, $addressbook_id)
  {
    $this->query("SELECT * FROM ab_companies WHERE addressbook_id='$addressbook_id' AND user_id='$user_id'");
    return $this->num_rows();
  }

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

  function delete_contact($contact_id)
  {

    $sql1 = "DELETE FROM tp_mailing_contacts WHERE contact_id='$contact_id'";
    $sql2 = "DELETE FROM ab_custom_contact_fields WHERE id='$contact_id'";

    if ($this->query($sql1) && $this->query($sql2))
    {
      return $this->query("DELETE FROM ab_contacts WHERE id='$contact_id'");
    }
  }

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

  function user_is_in_addressbook($user_id, $addressbook_id)
  {
    $sql = "SELECT id FROM ab_contacts WHERE source_id='$user_id' AND addressbook_id='$addressbook_id'";
    $this->query($sql);
    return $this->next_record();
  }

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

  function get_groups($addressbook_id)
  {
    $sql= "SELECT * FROM ab_groups WHERE addressbook_id='$addressbook_id'";
    $this->query($sql);
    return $this->num_rows();
  }

  function add_group($addressbook_id, $name)
  {
    $name = htmlentities($name);
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

  function move_contact_to_group($contact_id, $group_id)
  {
    $sql = "UPDATE ab_contacts SET group_id='$group_id' WHERE id='$contact_id'";
    $this->query($sql);
  }

  function clear_group($group_id)
  {
    $sql = "UPDATE ab_contacts SET group_id='0' WHERE group_id='$group_id'";
    $this->query($sql);
  }

  function change_group_name($group_id, $name)
  {
    $sql = "UPDATE ab_groups SET name='$name' WHERE id='$group_id'";
    $this->query($sql);
  }

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

  function add_contact_to_company($contact_id, $company_id)
  {
    return $this->query("UPDATE ab_contacts SET company_id='$company_id' WHERE id='$contact_id'");
  }

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

  function search_company($name, $addressbook_id)
  {
    $sql = "SELECT * FROM ab_companies WHERE addressbook_id='$addressbook_id' AND name LIKE '%$name%'";
    $this->query($sql);
    return $this->num_rows();
  }

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

  function delete_company($company_id)
  {
    $sql = "DELETE FROM ab_companies WHERE id='$company_id'";
    if ($this->query($sql))
    {
	  return $this->query("DELETE FROM ab_cate_companies WHERE company_id='$company_id'");
      return $this->query("DELETE FROM ab_custom_company_fields WHERE id='$company_id'");
    }
  }


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
