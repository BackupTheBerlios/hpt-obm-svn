<?php
/*
   Copyright Lorenz Softwareentwicklung & Systemintegration 2004
   Author: Georg Lorenz <georg@lonux.de>
   Version: 0.99 Release date: 17 May 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

define("ADR_POBOX", 0);
define("ADR_EXTENDED", 1);
define("ADR_STREET", 2);
define("ADR_LOCALITY", 3);
define("ADR_REGION", 4);
define("ADR_POSTALCODE", 5);
define("ADR_COUNTRY", 6);

define("N_FAMILY", 0);
define("N_GIVEN", 1);
define("N_ADDITIONAL", 2);
define("N_PREFIX", 3);
define("N_SUFFIX", 4);

define("ORG_NAME", 0);
define("ORG_UNIT", 1);
define("ORG_OPTIONAL_UNIT", 2);

define("PARM_TYPE", "TYPE");
define("PARM_ENCODING", "ENCODING");
define("PARM_VALUE", "VALUE");
define("PARM_CHARSET", "CHARSET");
define("PARM_LANGUAGE", "LANGUAGE");
define("DELIM_DOT", ".");
define("DELIM_COLON", ":");
define("DELIM_SEMICOLON", ";");
define("DELIM_COMMA", ",");
define("DELIM_EQUAL", "=");
define("LINE_LENGTH", 75);
define("FOLDING_CHAR", chr(13) . chr(10) . chr(32));
define("WORD_WRAP_DOS", chr(13) . chr(10));
define("WORD_WRAP_MAC", chr(13));
define("WORD_WRAP_UNIX", chr(10));
define("CHAR_WSP", chr(32));
define("CHAR_HTAB", chr(9));

class vcard extends addressbook {
    var $instance;
    var $nested;
    var $version;
    var $revision;

    function vcard()
    {
        $this->instance = array();
        $this->nested = array();
        $this->version = 'VERSION:2.1';
        $this->_set_revision();
        $this->addressbook();
    } 

    /*get the data from vcf-file and save to database (import)*/
    function import($file, $user_id, $addressbook_id, $vnconv)
    {
        if ($content = $this->_get_file_content($file)) {
            if ($this->_set_vcard($content)) {
                foreach($this->instance as $vcard) {
                    $contact = array('first_name' => '', 'middle_name' => '', 'last_name' => '', 
									'title' => '', 'function' => '', 'birthday' => '', 
									'country' => '', 'state' => '', 'city' => '', 'zip' => '', 'address' => '', 
									'home_fax' => '', 'home_phone' => '', 'work_fax' => '', 'work_phone' => '', 
									'cellular' => '', 'email' => '', 'company_name' => '', 'department' => '', 
									'homepage' => '', 'comment' => '');
                    $company = array('name' => '', 'homepage' => '', 'country' => '', 'state' => '', 
									'city' => '', 'zip' => '', 'address' => '');

                    foreach($vcard as $property) {
                        switch ($property->name) {
                            case "N":
                                $contact['last_name'] = $property->values[N_FAMILY];
                                $contact['first_name'] = $property->values[N_GIVEN];
                                $contact['middle_name'] = $property->values[N_ADDITIONAL];
                                break;
                            case "TITLE":
                                $contact['title'] = empty($property->values[0]) ? '' : $property->values[0];
                                break;
                            case "ROLE":
                                $contact['function'] = empty($property->values[0]) ? '' : $property->values[0];
                                break;
                            case "BDAY":
                                if (!empty($property->values[0])) {
                                    if (!is_numeric($property->values[0])) {
                                        $date = split("-", $property->values[0]);
                                        $property->values[0] = date("Ymd", mktime(0, 0, 0, $date[1], $date[2], $date[0]));
                                    } 
                                    $contact['birthday'] = $property->values[0];
                                } 
                                break;
                            case "ADR":
                                if (in_array('WORK', $property->parm_types)) {
                                    $company['country'] = $property->values[ADR_COUNTRY];
                                    $company['state'] = $property->values[ADR_REGION];
                                    $company['city'] = $property->values[ADR_LOCALITY];
                                    $company['zip'] = $property->values[ADR_POSTALCODE];
                                    $company['address'] = $property->values[ADR_STREET];
                                } else {
                                    $contact['country'] = $property->values[ADR_COUNTRY];
                                    $contact['state'] = $property->values[ADR_REGION];
                                    $contact['city'] = $property->values[ADR_LOCALITY];
                                    $contact['zip'] = $property->values[ADR_POSTALCODE];
                                    $contact['address'] = $property->values[ADR_STREET];
                                } 
                                break;
                            case "TEL":
                                if (in_array('HOME', $property->parm_types)) {
                                    if (in_array('FAX', $property->parm_types)) {
                                        $contact['home_fax'] = $property->values[0];
                                    } 
                                    if (in_array('VOICE', $property->parm_types)) {
                                        $contact['home_phone'] = $property->values[0];
                                    } 
                                    if (!in_array('FAX', $property->parm_types) && !in_array('VOICE', $property->parm_types)) {
                                        $contact['home_phone'] = $property->values[0];
                                    } 
                                } 

                                if (in_array('WORK', $property->parm_types)) {
                                    if (in_array('FAX', $property->parm_types)) {
                                        $contact['work_fax'] = $property->values[0];
                                    } 
                                    if (in_array('VOICE', $property->parm_types)) {
                                        $contact['work_phone'] = $property->values[0];
                                    } 
                                    if (!in_array('FAX', $property->parm_types) && !in_array('VOICE', $property->parm_types)) {
                                        $contact['work_phone'] = $property->values[0];
                                    } 
                                } 

                                if (in_array('CELL', $property->parm_types)) {
                                    $contact['cellular'] = $property->values[0];
                                } 
                                break;
                            case "EMAIL":
                                $contact['email'] = $property->values[0];
                                break;
                            case "ORG":
                                $contact['company_name'] = $property->values[ORG_NAME];
                                $contact['department'] = $property->values[ORG_UNIT];
                                $company['name'] = $property->values[ORG_NAME];
                                break;
                            case "URL":
                                if (in_array('HOME', $property->parm_types) || empty($property->parm_types)) {
                                    $contact['homepage'] = $property->values[0];
                                } 
                                if (in_array('WORK', $property->parm_types)) {
                                    $company['homepage'] = $property->values[0];
                                } 
                                break;
                            case "NOTE":
                                $contact['comment'] = $property->values[0];
                                break;
                            case "TZ":
                                break;
                        } 
                    } 

                    global $GO_SECURITY;
                    if ($ab = $this->get_addressbook($addressbook_id)) {
                        $acl_read = $GO_SECURITY->get_new_acl('contact read');
                        $acl_write = $GO_SECURITY->get_new_acl('contact write');

                        if ($acl_read > 0 && $acl_write > 0) {
                            if ($contact_id = $this->add_contact("", $user_id, $addressbook_id, $contact['first_name'], $contact['middle_name'], $contact['last_name'], "", $contact['title'], "", $contact['birthday'], $contact['email'], $contact['work_phone'], $contact['home_phone'], $contact['home_fax'], $contact['cellular'], $contact['country'], $contact['state'], $contact['city'], $contact['zip'], $contact['address'], 0, $contact['work_fax'], $contact['department'], $contact['function'], $contact['comment'], 0, "", $acl_read, $acl_write)) {
                                $GO_SECURITY->copy_acl($ab['acl_read'], $acl_read);
                                $GO_SECURITY->copy_acl($ab['acl_write'], $acl_write);

                                if (!empty($company['name'])) {
                                    if (!$company_id = $this->get_company_id_by_name($company['name'], $addressbook_id)) {
                                        $acl_read = $GO_SECURITY->get_new_acl('company read');
                                        $acl_write = $GO_SECURITY->get_new_acl('company write');

                                        if ($acl_read > 0 && $acl_write > 0) {
                                            if ($company_id = $this->add_company($addressbook_id, $user_id, $company['name'], $company['address'], $company['zip'], $company['city'], $company['state'], $company['country'], '', '', '', $company['homepage'], '', '', $acl_read, $acl_write)) {
                                                $GO_SECURITY->copy_acl($ab['acl_read'], $acl_read);
                                                $GO_SECURITY->copy_acl($ab['acl_write'], $acl_write);
                                            } 
                                        } else {
                                            $GO_SECURITY->delete_acl($acl_read);
                                            $GO_SECURITY->delete_acl($acl_write);
                                        } 
                                    } 

                                    $this->add_contact_to_company($contact_id, $company_id);
                                } 
                            } else {
                                $GO_SECURITY->delete_acl($acl_read);
                                $GO_SECURITY->delete_acl($acl_write);
                                return false;
                            } 
                        } else {
                            return false;
                        } 
                    } else {
                        return false;
                    } 
                } 
            } else {
                return false;
            } 
        } else {
            return false;
        } 
        return true;
    } 

    /*get the whole addressbook data from database and create a vcf-file (export)*/
    function export($user_id, $addressbook_id, $contact_id)
    {
        
		/*will be implemented as soon as possible*/
    } 

    /*get the users data from database and create a vcf-file (vcard)*/
    function create($user_id)
    {
        /*will be implemented as soon as possible*/
    } 

    function _set_vcard($file_content)
    {
        if (!is_array($file_content)) {
            return false;
        } 

        $inside_vcard = false;

        foreach($file_content as $line) {
            $property = new vcard_property();
            $property->set_by_file($line);

            if (count($property->values)) {
				if (strtoupper($property->values[0]) == 'VCARD') {
	                if ($property->name == 'BEGIN') {
    	                $index = isset($index) ? $index + 1 : 0;
        	            $inside_vcard = true;
            	    } elseif ($property->name == 'END') {
                	    $inside_vcard = false;
                	} 
            	}
			}

            if ($inside_vcard) {
                $this->instance[$index][] = $property;
                unset($property);
            } 
        } 
        return true;
    } 

    function _get_file_content($file)
    {
        $content = "";
		
		if (!$handle = fopen ($file, "r")) {
            return false;
        } while (!feof($handle)) {
            $line = fgets($handle, 4096);
            if (strlen($line) > 0) {
                /*word wrap - replace <CRLF> by <LF> (dos)*/
                $line = str_replace(WORD_WRAP_DOS, WORD_WRAP_UNIX, $line);
                /*word wrap - replace <CR> by <LF> (mac)*/
                $line = str_replace(WORD_WRAP_MAC, WORD_WRAP_UNIX, $line);
                /*unfolding lines ending up in '=<LF>', originally '=<CRLF>'*/
                $regex = '/(' . DELIM_EQUAL . WORD_WRAP_UNIX . ')/i';
                $content .= preg_replace($regex, "", $line);
            } 
        } 
        fclose($handle);

        /*unfolding lines as specified in RFC2425*/
        $regex = '/(' . WORD_WRAP_UNIX . ')([' . CHAR_WSP . '|' . CHAR_HTAB . '])/i';
        $content = preg_replace($regex, "", $content);

        $content = preg_split('/' . WORD_WRAP_UNIX . '/', $content);
        return $content;
    } 

    function _get_db_content($user_id, $addressbook_id = 0, $contact_id = 0)
    {
        $records = array();
		
		if($addressbook_id)
		{
			if($contact_id)
			{
		    	if($records[] = $this->get_contact($contact_id))
				{
					return $records;
				}
			}else
			{
		    	if($this->get_contacts_for_export($addressbook_id, $user_id))
				{
					while($this->next_record())
					{
						$records[] = $this->Record;
					}
					return $records;
				}
			}
		}else
		{
			global $GO_USERS;
			
			if($records[] = $GO_USERS->get_user($user_id))
			{
				return $records;
			}
		}
		return false;
    } 

    function _set_revision()
    {
        $dt = getdate();
        $date = date("Y-m-d", mktime(0, 0, 0, $dt['mon'], $dt['mday'], $dt['year']));
        $time = date("H:i:s", mktime($dt['hours'], $dt['minutes'], $dt['seconds'], 0, 0, 0));
        $this->revision = $date . 'T' . $time;
    } 
} 

class vcard_property {
    var $group;
    var $name;
    var $parm_encoding;
    var $parm_language;
    var $parm_value;
    var $parm_charset;
    var $parm_types;
    var $values;

    function vcard_property()
    {
        $this->group = null;
        $this->name = null;
        $this->parm_encoding = null;
        $this->parm_language = null;
        $this->parm_value = null;
        $this->parm_charset = null;
        $this->parm_types = array();
        $this->values = array();
    } 

    function set_by_file($line)
    {
        if ($pos = strpos($line, DELIM_COLON))
		{
            $left_part = trim(substr($line, 0, $pos));
            $right_part = trim(substr($line, $pos + 1));
            $this->_set_name($left_part);
            $this->_set_parms($left_part);
            $this->_set_values($right_part);
        } 
    } 

    function set_by_db()
    {
    } 

    function _set_name($text)
    {
        /*
		//	we need the first element only
		//	it may contain a value e.g. ADR or A.ADR
		*/
        $name_part = array_shift($this->_split($text, DELIM_SEMICOLON));

        if ($pos = strpos($name_part, DELIM_DOT)) {
            $array = $this->_split($name_part, DELIM_DOT);
            $this->group = strtoupper(trim(array_shift($array)));
            $this->name = strtoupper(trim(array_shift($array)));
        } else {
            $this->name = strtoupper(trim($name_part));
        } 
    } 

    function _set_parms($text)
    {
        static $encodings = array('7BIT', '8BIT', 'BASE64', 'QUOTED-PRINTABLE', 'B');
        static $values = array('INLINE', 'CONTENT-ID', 'CID', 'URL', 'BINARY',
            'PHONE-NUMBER', 'TEXT', 'URI', 'UTC-OFFSET', 'VCARD');

        $parm_array = $this->_split($text, DELIM_SEMICOLON);
        /*remove the first array value - it is the property name, we don't need it*/
        array_shift($parm_array);

        foreach($parm_array as $array_value) {
            $parm = $this->_split($array_value, DELIM_COMMA);

            foreach($parm as $value) {
                $parameter = $this->_split($value, DELIM_EQUAL);
                if (count($parameter) > 1) {
                    switch (strtoupper(trim($parameter[0]))) {
                        case PARM_ENCODING:
                            $this->parm_encoding = strtoupper(trim($parameter[1]));
                            break;
                        case PARM_VALUE:
                            $this->parm_value = strtoupper(trim($parameter[1]));
                            break;
                        case PARM_CHARSET:
                            $this->parm_charset = strtoupper(trim($parameter[1]));
                            break;
                        case PARM_LANGUAGE:
                            $this->parm_language = strtoupper(trim($parameter[1]));
                            break;
                        default:
                            $this->parm_types[] = strtoupper(trim($parameter[1]));
                            break;
                    } 
                } else {
                    $parameter[0] = strtoupper(trim($parameter[0]));

                    if (in_array($parameter[0], $encodings)) {
                        $this->parm_encoding = $parameter[0];
                    } elseif (in_array($parameter[0], $values)) {
                        $this->parm_value = $parameter[0];
                    } else {
                        $this->parm_types[] = $parameter[0];
                    } 
                } 
            } 
        } 
    } 

    function _set_values($text)
    {
        switch ($this->parm_encoding) {
            case 'QUOTED-PRINTABLE':
                $text = quoted_printable_decode($text);
                break;
            case 'BASE64':
                $text = base64_decode($text);
                break;
            case 'B':
                break;
            case '8BIT':
                break;
            case '7BIT':
                break;
        } // switch
        $this->values = $this->_split($text, DELIM_SEMICOLON);
    } 

    function _split($text, $delimiter)
    {
        switch ($delimiter) {
            case DELIM_SEMICOLON:
                $delimiter = '\\' . $delimiter;
                break;
            case DELIM_COMMA:
                $delimiter = '\\' . $delimiter;
                break;
            default:
                break;
        } // switch
        $regex = '/(?<!\\\\)(' . $delimiter . ')/i';
        $array = preg_split($regex, $text);

        return $array;
    } 
} 

?>
