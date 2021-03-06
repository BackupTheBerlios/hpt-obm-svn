<?php
/**
 * addressbook.php
 *
 * Copyright (c) 1999-2004 The SquirrelMail Project Team
 * Licensed under the GNU GPL. For full terms see the file COPYING.
 *
 * Functions and classes for the addressbook system.
 *
 * @version $Id: addressbook.php,v 1.63 2004/10/09 08:12:23 tokul Exp $
 * @package squirrelmail
 * @subpackage addressbook
 */

/**
   This is the path to the global site-wide addressbook.
   It looks and feels just like a user's .abook file
   If this is in the data directory, use "$data_dir/global.abook"
   If not, specify the path as though it was accessed from the
   src/ directory ("../global.abook" -> in main directory)

   If you don't want a global site-wide addressbook, comment these
   two lines out.  (They are disabled by default.)

   The global addressbook is unmodifiable by anyone.  You must actually
   use a shell script or whatnot to modify the contents.

  global $data_dir, $address_book_global_filename;
  $address_book_global_filename = "$data_dir/global.abook";

*/

global $addrbook_dsn, $addrbook_global_dsn;

/**
   Create and initialize an addressbook object.
   Returns the created object
*/
function addressbook_init($showerr = true, $onlylocal = false) {
    global $data_dir, $username, $ldap_server, $address_book_global_filename;
    global $addrbook_dsn, $addrbook_table;
    global $addrbook_global_dsn, $addrbook_global_table, $addrbook_global_writeable, $addrbook_global_listing;

    /* Create a new addressbook object */
    $abook = new AddressBook;

    /*
        Always add a local backend. We use *either* file-based *or* a
        database addressbook. If $addrbook_dsn is set, the database
        backend is used. If not, addressbooks are stores in files.
    */
    if (isset($addrbook_dsn) && !empty($addrbook_dsn)) {
        /* Database */
        if (!isset($addrbook_table) || empty($addrbook_table)) {
            $addrbook_table = 'address';
        }
        $r = $abook->add_backend('database', Array('dsn' => $addrbook_dsn,
                            'owner' => $username,
                            'table' => $addrbook_table));
        if (!$r && $showerr) {
            echo _("Error initializing addressbook database.");
            exit;
        }
    } else {
        /* File */
        $filename = getHashedFile($username, $data_dir, "$username.abook");
        $r = $abook->add_backend('local_file', Array('filename' => $filename,
                              'create'   => true));
        if(!$r && $showerr) {
            printf( _("Error opening file %s"), $filename );
            exit;
        }

    }

    /* This would be for the global addressbook */
    if (isset($address_book_global_filename)) {
        $r = $abook->add_backend('global_file');
        if (!$r && $showerr) {
            echo _("Error initializing global addressbook.");
            exit;
        }
    }

    /* Load global addressbook from SQL if configured */
    if (isset($addrbook_global_dsn) && !empty($addrbook_global_dsn)) {
      /* Database configured */
      if (!isset($addrbook_global_table) || empty($addrbook_global_table)) {
          $addrbook_global_table = 'global_abook';
      }
      $r = $abook->add_backend('database',
                               Array('dsn' => $addrbook_global_dsn,
                                     'owner' => 'global',
                                     'name' => _("Global address book"),
                                     'writeable' => $addrbook_global_writeable,
                                     'listing' => $addrbook_global_listing,
                                     'table' => $addrbook_global_table));
    }

    /*
     * hook allows to include different address book backends.
     * plugins should extract $abook and $r from arguments
     * and use same add_backend commands as above functions.
     */
    $hookReturn = do_hook('abook_init', $abook, $r);
    $abook = $hookReturn[1];
    $r = $hookReturn[2];

    if ($onlylocal) {
        return $abook;
    }

    /* Load configured LDAP servers (if PHP has LDAP support) */
    if (isset($ldap_server) && is_array($ldap_server) && function_exists('ldap_connect')) {
        reset($ldap_server);
        while (list($undef,$param) = each($ldap_server)) {
            if (is_array($param)) {
                $r = $abook->add_backend('ldap_server', $param);
                if (!$r && $showerr) {
                    printf( '&nbsp;' . _("Error initializing LDAP server %s:") .
                            "<br />\n", $param['host']);
                    echo '&nbsp;' . $abook->error;
                    exit;
                }
            }
        }
    }

    /* Return the initialized object */
    return $abook;
}

/**
 * Display the "new address" form
 *
 * Form is not closed and you must add closing form tag.
 * @since 1.5.1
 * @param string $form_url form action url
 * @param string $name form name
 * @param string $title form title
 * @param string $button form button name
 * @param array $defdata values of form fields
 */
function abook_create_form($form_url,$name,$title,$button,$defdata=array()) {
    global $color;
    echo addForm($form_url, 'post', 'f_add').
        html_tag( 'table',
                  html_tag( 'tr',
                            html_tag( 'td', "\n". '<strong>' . $title . '</strong>' . "\n",
                                      'center', $color[0]
                                      )
                            )
                  , 'center', '', 'width="100%"' ) ."\n";
    address_form($name, $button, $defdata);
}


/*
 *   Had to move this function outside of the Addressbook Class
 *   PHP 4.0.4 Seemed to be having problems with inline functions.
 *   Note: this can return now since we don't support 4.0.4 anymore.
 */
function addressbook_cmp($a,$b) {

    if($a['backend'] > $b['backend']) {
        return 1;
    } else if($a['backend'] < $b['backend']) {
        return -1;
    }

    return (strtolower($a['name']) > strtolower($b['name'])) ? 1 : -1;

}

/**
 * Make an input field
 * @param string $label
 * @param string $field
 * @param string $name
 * @param string $size
 * @param array $values
 * @param string $add
 */
function addressbook_inp_field($label, $field, $name, $size, $values, $add='') {
    global $color;
    $value = ( isset($values[$field]) ? $values[$field] : '');

    if (is_array($value)) {
        $td_str = addSelect($name.'['.$field.']', $value);
    } else {
        $td_str = addInput($name.'['.$field.']', $value, $size);
    }
    $td_str .= $add ;

    return html_tag( 'tr' ,
            html_tag( 'td', $label . ':', 'right', $color[4]) .
            html_tag( 'td', $td_str, 'left', $color[4])
            )
        . "\n";
}

/**
 * Output form to add and modify address data
 */
function address_form($name, $submittext, $values = array()) {
    global $color, $squirrelmail_language;

    if ($squirrelmail_language == 'ja_JP') {
        echo html_tag( 'table',
                addressbook_inp_field(_("Nickname"),     'nickname', $name, 15, $values,
                    ' <small>' . _("Must be unique") . '</small>') .
                addressbook_inp_field(_("E-mail address"),  'email', $name, 45, $values, '') .
                addressbook_inp_field(_("Last name"),    'lastname', $name, 45, $values, '') .
                addressbook_inp_field(_("First name"),  'firstname', $name, 45, $values, '') .
                addressbook_inp_field(_("Additional info"), 'label', $name, 45, $values, '') .
                list_writable_backends($name) .
                html_tag( 'tr',
                    html_tag( 'td',
                        addSubmit($submittext, $name.'[SUBMIT]'),
                        'center', $color[4], 'colspan="2"')
                    )
                , 'center', '', 'border="0" cellpadding="1" width="90%"') ."\n";
    } else {
        echo html_tag( 'table',
                addressbook_inp_field(_("Nickname"),     'nickname', $name, 15, $values,
                    ' <small>' . _("Must be unique") . '</small>') .
                addressbook_inp_field(_("E-mail address"),  'email', $name, 45, $values, '') .
                addressbook_inp_field(_("First name"),  'firstname', $name, 45, $values, '') .
                addressbook_inp_field(_("Last name"),    'lastname', $name, 45, $values, '') .
                addressbook_inp_field(_("Additional info"), 'label', $name, 45, $values, '') .
                list_writable_backends($name) .
                html_tag( 'tr',
                    html_tag( 'td',
                        addSubmit($submittext, $name.'[SUBMIT]') ,
                        'center', $color[4], 'colspan="2"')
                    )
                , 'center', '', 'border="0" cellpadding="1" width="90%"') ."\n";
    }
}

function list_writable_backends($name) {
    global $color, $abook;
    if ( $name != 'addaddr' ) { return; }
    if ( $abook->numbackends > 1 ) {
        $ret = '<select name="backend">';
        $backends = $abook->get_backend_list();
        while (list($undef,$v) = each($backends)) {
            if ($v->writeable) {
                $ret .= '<option value="' . $v->bnum;
                $ret .= '">' . $v->sname . "</option>\n";
            }
        }
        $ret .= "</select>";
        return html_tag( 'tr',
                html_tag( 'td', _("Add to:"),'right', $color[4] ) .
                html_tag( 'td', $ret, 'left', $color[4] )) . "\n";
    } else {
        return html_tag( 'tr',
                html_tag( 'td',
                    addHidden('backend', '1'),
                    'center', $color[4], 'colspan="2"')) . "\n";
    }
}

/**
 * Sort array by the key "name"
 */
function alistcmp($a,$b) {
    $abook_sort_order=get_abook_sort();

    switch ($abook_sort_order) {
    case 0:
    case 1:
      $abook_sort='nickname';
      break;
    case 4:
    case 5:
      $abook_sort='email';
      break;
    case 6:
    case 7:
      $abook_sort='label';
      break;
    case 2:
    case 3:
    case 8:
    default:
      $abook_sort='name';
    }

    if ($a['backend'] > $b['backend']) {
        return 1;
    } else {
        if ($a['backend'] < $b['backend']) {
            return -1;
        }
    }

    if( (($abook_sort_order+2) % 2) == 1) {
      return (strtolower($a[$abook_sort]) < strtolower($b[$abook_sort])) ? 1 : -1;
    } else {
      return (strtolower($a[$abook_sort]) > strtolower($b[$abook_sort])) ? 1 : -1;
    }
}

/**
 * Address book sorting options
 *
 * returns address book sorting order
 * @return integer book sorting options order
 */
function get_abook_sort() {
    global $data_dir, $username;

    /* get sorting order */
    if(sqgetGlobalVar('abook_sort_order', $temp, SQ_GET)) {
      $abook_sort_order = (int) $temp;

      if ($abook_sort_order < 0 or $abook_sort_order > 8)
        $abook_sort_order=8;

      setPref($data_dir, $username, 'abook_sort_order', $abook_sort_order);
    } else {
      /* get previous sorting options. default to unsorted */
      $abook_sort_order = getPref($data_dir, $username, 'abook_sort_order', 8);
    }

    return $abook_sort_order;
}

/**
 * This function shows the address book sort button.
 *
 * @param integer $abook_sort_order current sort value
 * @param string $alt_tag alt tag value (string visible to text only browsers)
 * @param integer $Down sort value when list is sorted ascending
 * @param integer $Up sort value when list is sorted descending
 * @return string html code with sorting images and urls
 */
function show_abook_sort_button($abook_sort_order, $alt_tag, $Down, $Up ) {
    global $form_url;

     /* Figure out which image we want to use. */
    if ($abook_sort_order != $Up && $abook_sort_order != $Down) {
        $img = 'sort_none.png';
        $which = $Up;
    } elseif ($abook_sort_order == $Up) {
        $img = 'up_pointer.png';
        $which = $Down;
    } else {
        $img = 'down_pointer.png';
        $which = 8;
    }

      /* Now that we have everything figured out, show the actual button. */
    return ' <a href="' . $form_url .'?abook_sort_order=' . $which
         . '"><img src="../images/' . $img
         . '" border="0" width="12" height="10" alt="' . $alt_tag . '" title="'
         . _("Click here to change the sorting of the address list") .'" /></a>';
}


/**
 * This is the main address book class that connect all the
 * backends and provide services to the functions above.
 * @package squirrelmail
 * @subpackage addressbook
 */
class AddressBook {

    var $backends    = array();
    var $numbackends = 0;
    var $error       = '';
    var $localbackend = 0;
    var $localbackendname = '';

      // Constructor function.
    function AddressBook() {
        $localbackendname = _("Personal address book");
    }

    /*
     * Return an array of backends of a given type,
     * or all backends if no type is specified.
     */
    function get_backend_list($type = '') {
        $ret = array();
        for ($i = 1 ; $i <= $this->numbackends ; $i++) {
            if (empty($type) || $type == $this->backends[$i]->btype) {
                $ret[] = &$this->backends[$i];
            }
        }
        return $ret;
    }


    /*
       ========================== Public ========================

        Add a new backend. $backend is the name of a backend
        (without the abook_ prefix), and $param is an optional
        mixed variable that is passed to the backend constructor.
        See each of the backend classes for valid parameters.
     */
    function add_backend($backend, $param = '') {
        $backend_name = 'abook_' . $backend;
        eval('$newback = new ' . $backend_name . '($param);');
        if(!empty($newback->error)) {
            $this->error = $newback->error;
            return false;
        }

        $this->numbackends++;

        $newback->bnum = $this->numbackends;
        $this->backends[$this->numbackends] = $newback;

        /* Store ID of first local backend added */
        if ($this->localbackend == 0 && $newback->btype == 'local') {
            $this->localbackend = $this->numbackends;
            $this->localbackendname = $newback->sname;
        }

        return $this->numbackends;
    }


    /*
     * This function takes a $row array as returned by the addressbook
     * search and returns an e-mail address with the full name or
     * nickname optionally prepended.
     */

    function full_address($row) {
        global $addrsrch_fullname, $data_dir, $username;
        $prefix = getPref($data_dir, $username, 'addrsrch_fullname');
        if (($prefix != "" || (isset($addrsrch_fullname) &&
            $prefix == $addrsrch_fullname)) && $prefix != 'noprefix') {
            $name = ($prefix == 'nickname' ? $row['nickname'] : $row['name']);
            return $name . ' <' . trim($row['email']) . '>';
        } else {
            return trim($row['email']);
        }
    }

    /*
        Return a list of addresses matching expression in
        all backends of a given type.
    */
    function search($expression, $bnum = -1) {
        $ret = array();
        $this->error = '';

        /* Search all backends */
        if ($bnum == -1) {
            $sel = $this->get_backend_list('');
            $failed = 0;
            for ($i = 0 ; $i < sizeof($sel) ; $i++) {
                $backend = &$sel[$i];
                $backend->error = '';
                $res = $backend->search($expression);
                if (is_array($res)) {
                    $ret = array_merge($ret, $res);
                } else {
                    $this->error .= "<br />\n" . $backend->error;
                    $failed++;
                }
            }

            /* Only fail if all backends failed */
            if( $failed >= sizeof( $sel ) ) {
                $ret = FALSE;
            }

        }  else {

            /* Search only one backend */

            $ret = $this->backends[$bnum]->search($expression);
            if (!is_array($ret)) {
                $this->error .= "<br />\n" . $this->backends[$bnum]->error;
                $ret = FALSE;
            }
        }

        return( $ret );
    }


    /* Return a sorted search */
    function s_search($expression, $bnum = -1) {

        $ret = $this->search($expression, $bnum);
        if ( is_array( $ret ) ) {
            usort($ret, 'addressbook_cmp');
        }
        return $ret;
    }


    /*
     *  Lookup an address by alias. Only possible in
     *  local backends.
     */
    function lookup($alias, $bnum = -1) {

        $ret = array();

        if ($bnum > -1) {
            $res = $this->backends[$bnum]->lookup($alias);
            if (is_array($res)) {
               return $res;
            } else {
               $this->error = $backend->error;
               return false;
            }
        }

        $sel = $this->get_backend_list('local');
        for ($i = 0 ; $i < sizeof($sel) ; $i++) {
            $backend = &$sel[$i];
            $backend->error = '';
            $res = $backend->lookup($alias);
            if (is_array($res)) {
               if(!empty($res))
              return $res;
            } else {
               $this->error = $backend->error;
               return false;
            }
        }

        return $ret;
    }


    /* Return all addresses */
    function list_addr($bnum = -1) {
        $ret = array();

        if ($bnum == -1) {
            $sel = $this->get_backend_list('local');
        } else {
            $sel = array(0 => &$this->backends[$bnum]);
        }

        for ($i = 0 ; $i < sizeof($sel) ; $i++) {
            $backend = &$sel[$i];
            $backend->error = '';
            $res = $backend->list_addr();
            if (is_array($res)) {
               $ret = array_merge($ret, $res);
            } else {
               $this->error = $backend->error;
               return false;
            }
        }

        return $ret;
    }

    /*
     * Create a new address from $userdata, in backend $bnum.
     * Return the backend number that the/ address was added
     * to, or false if it failed.
     */
    function add($userdata, $bnum) {

        /* Validate data */
        if (!is_array($userdata)) {
            $this->error = _("Invalid input data");
            return false;
        }
        if (empty($userdata['firstname']) && empty($userdata['lastname'])) {
            $this->error = _("Name is missing");
            return false;
        }
        if (empty($userdata['email'])) {
            $this->error = _("E-mail address is missing");
            return false;
        }
        if (empty($userdata['nickname'])) {
            $userdata['nickname'] = $userdata['email'];
        }

        if (eregi('[ \\:\\|\\#\\"\\!]', $userdata['nickname'])) {
            $this->error = _("Nickname contains illegal characters");
            return false;
        }

        /* Check that specified backend accept new entries */
        if (!$this->backends[$bnum]->writeable) {
            $this->error = _("Addressbook is read-only");
            return false;
        }

        /* Add address to backend */
        $res = $this->backends[$bnum]->add($userdata);
        if ($res) {
            return $bnum;
        } else {
            $this->error = $this->backends[$bnum]->error;
            return false;
        }

        return false;  // Not reached
    } /* end of add() */


    /*
     * Remove the user identified by $alias from backend $bnum
     * If $alias is an array, all users in the array are removed.
     */
    function remove($alias, $bnum) {

        /* Check input */
        if (empty($alias)) {
            return true;
        }

        /* Convert string to single element array */
        if (!is_array($alias)) {
            $alias = array(0 => $alias);
        }

        /* Check that specified backend is writable */
        if (!$this->backends[$bnum]->writeable) {
            $this->error = _("Addressbook is read-only");
            return false;
        }

        /* Remove user from backend */
        $res = $this->backends[$bnum]->remove($alias);
        if ($res) {
            return $bnum;
        } else {
            $this->error = $this->backends[$bnum]->error;
            return false;
        }

        return FALSE;  /* Not reached */
    } /* end of remove() */


    /*
     * Remove the user identified by $alias from backend $bnum
     * If $alias is an array, all users in the array are removed.
     */
    function modify($alias, $userdata, $bnum) {

        /* Check input */
        if (empty($alias) || !is_string($alias)) {
            return true;
        }

        /* Validate data */
        if(!is_array($userdata)) {
            $this->error = _("Invalid input data");
            return false;
        }
        if (empty($userdata['firstname']) && empty($userdata['lastname'])) {
            $this->error = _("Name is missing");
            return false;
        }
        if (empty($userdata['email'])) {
            $this->error = _("E-mail address is missing");
            return false;
        }

        if (eregi('[\\: \\|\\#"\\!]', $userdata['nickname'])) {
            $this->error = _("Nickname contains illegal characters");
            return false;
        }

        if (empty($userdata['nickname'])) {
            $userdata['nickname'] = $userdata['email'];
        }

        /* Check that specified backend is writable */
        if (!$this->backends[$bnum]->writeable) {
            $this->error = _("Addressbook is read-only");;
            return false;
        }

        /* Modify user in backend */
        $res = $this->backends[$bnum]->modify($alias, $userdata);
        if ($res) {
            return $bnum;
        } else {
            $this->error = $this->backends[$bnum]->error;
            return false;
        }

        return FALSE;  /* Not reached */
    } /* end of modify() */


} /* End of class Addressbook */

/**
 * Generic backend that all other backends extend
 * @package squirrelmail
 * @subpackage addressbook
 */
class addressbook_backend {

    /* Variables that all backends must provide. */
    var $btype      = 'dummy';
    var $bname      = 'dummy';
    var $sname      = 'Dummy backend';

    /*
     * Variables common for all backends, but that
     * should not be changed by the backends.
     */
    var $bnum       = -1;
    var $error      = '';
    var $writeable  = false;

    function set_error($string) {
        $this->error = '[' . $this->sname . '] ' . $string;
        return false;
    }


    /* ========================== Public ======================== */

    function search($expression) {
        $this->set_error('search not implemented');
        return false;
    }

    function lookup($alias) {
        $this->set_error('lookup not implemented');
        return false;
    }

    function list_addr() {
        $this->set_error('list_addr not implemented');
        return false;
    }

    function add($userdata) {
        $this->set_error('add not implemented');
        return false;
    }

    function remove($alias) {
        $this->set_error('delete not implemented');
        return false;
    }

    function modify($alias, $newuserdata) {
        $this->set_error('modify not implemented');
        return false;
    }

}

/*
  PHP 5 requires that the class be made first, which seems rather
  logical, and should have been the way it was generated the first time.
*/

require_once(SM_PATH . 'functions/abook_local_file.php');
require_once(SM_PATH . 'functions/abook_ldap_server.php');

/* Use this if you wanna have a global address book */
if (isset($address_book_global_filename)) {
    include_once(SM_PATH . 'functions/abook_global_file.php');
}

/* Only load database backend if database is configured */
if((isset($addrbook_dsn) && !empty($addrbook_dsn)) ||
 (isset($addrbook_global_dsn) && !empty($addrbook_global_dsn)) ) {
  include_once(SM_PATH . 'functions/abook_database.php');
}

/*
 * hook allows adding different address book classes.
 * class must follow address book class coding standards.
 *
 * see addressbook_backend class and functions/abook_*.php files.
 */
do_hook('abook_add_class');

?>