<?php
/**
 * Session Management for PHP3
 *
 * Copyright (c) 1998-2000 NetUSE AG
 *                    Boris Erdmann, Kristian Koehntopp
 *
 * $Id: mysql.class.inc,v 1.1.1.1 2004/08/03 11:50:16 vutp Exp $
 */

include("adodb/adodb.inc.php");

class db {
  /**
  *	The ADODB thing
  *
  * @access private
  */
  var $adodb     = null;

  /**
  *	Host name or database server name 
  *
  * @access public
  */
  var $Host     = "";
  
  /**
  * Database name 
  * 
  * @access public
  */
  var $Database = "";

  /**
  * User name - used to connect database server 
  *
  * @access public
  */
  var $User     = "";
  
  /**
  * Password connecting
  *
  * @access public
  */
  var $Password = "";

  /** 
  * Configuration parameter
  *
  * Set to 1 for automatic mysql_free_result()
  *
  * @access public
  */
  var $Auto_Free     = 0;     ## Set to 1 for automatic mysql_free_result()

  /** 
  * Configuration parameter 
  *
  * Set to 1 for debugging messages
  * 
  * @access public
  *
  */
  var $Debug         = 0;     ## Set to 1 for debugging messages.

  /** 
   * Configuration parameter
   * 
   * <b>$Halt_On_Error</b> = 
   *
   * <pre>	= <i>"yes"</i>: 	(halt with message)</pre>
   *
   * <pre>	= <i>"no"</i>:		(ignore errors quietly)</pre>
   *
   * <pre>	= <i>"report"</i>:	(ignore errror, but spit a warning)</pre>
   *
   * @access public
   */
  var $Halt_On_Error = "yes"; ## "yes" (halt with message), "no" (ignore errors quietly), "report" (ignore errror, but spit a warning)
  
  /** 
   * Configuration parameter
   *
   * Table name stores next ID values of some tables in the database
   * @access public 
   */
  var $Seq_Table     = "db_sequence";

  /**
   * Result array and current row number 
   *
   * @access public
   */
  var $Record   = array();
  
  /**
   * Retain a record in a recordset
   *
   * @access public
   */
  var $Row;

  /** 
   * Current error number
   *
   * @access public
   */
  var $Errno    = 0;
  
  /**
   * Current error text
   * 
   * @access public
   */
  var $Error    = "";

  /** 
   * Database type 
   *
   * @access public
   */
  var $type     = "mysql";
  
  /**
   * Revision
   *
   * @access public
   */
  var $revision = "1.2";

  /**
   * Link identifier 
   *
   * The positive number is returned by <i> mysql_pconnect() </i> or <i> mysql_connect() </i> functions
   *
   * @access private 
   */
  var $Link_ID  = 0;
  
  /**
   * RecordSet
   *
   * @access private
   */
  var $RS = null;

  /**
   * The class constructor
   *
   * @access public
   * @param string $query
   */
  /* public: constructor */
  function db($query = "") {
	  global $GO_CONFIG;
	  $this->Host = $GO_CONFIG->db_host;
	  $this->Database = $GO_CONFIG->db_name;
	  $this->User = $GO_CONFIG->db_user;
  	  $this->Password = $GO_CONFIG->db_pass;
      $this->query($query);
  }

  /**
   * Connecting database
   *
   * @access public
   * @param string $Database
   * @param string $Host
   * @param string $User
   * @param string $Password
   * @return mixed true on success, PEAR_Error on failure
   */
  /* public: connection management */
  function connect($Database = "", $Host = "", $User = "", $Password = "") {
    /* Handle defaults */
    if ("" == $Database)
      $Database = $this->Database;
    if ("" == $Host)
      $Host     = $this->Host;
    if ("" == $User)
      $User     = $this->User;
    if ("" == $Password)
      $Password = $this->Password;

    if ($this->adodb == null)
      $this->adodb = NewADOConnection('mysql');
    if (!$this->adodb->IsConnected()) {
      if (!$this->adodb->Connect($Host, $User, $Password, $Database)) {
        $this->halt("Connect ion ($Host,$User,\$Password) failed.");
	return 0;
      }
    }
    return $this->adodb;
  }

  /**
   * Free all associated data in memory
   *
   * @access public
   */
  /* public: discard the query result */
  function free() {
      if ($this->RS)
        unset($this->RS);
      $this->RS = null;
  }

  /**
   * Execute a query
   *
   * @access public
   * @param string $Query_String 
   * @return int positive value (result identifier) on success, 0 on failure
   */
  /* public: perform a query */
  function query($Query_String) {
    /* No empty queries, please, since PHP4 chokes on them. */
    if ($Query_String == "")
      /* The empty query string is passed on from the constructor,
       * when calling the class without a query, e.g. in situations
       * like these: '$db = new DB_Sql_Subclass;'
       */
      return 0;

    if (!$this->connect()) {
      return 0; /* we already complained in connect() about that. */
    };

    # New query, discard previous result.
    $this->free();

    if ($this->Debug)
      printf("Debug: query = %s<br>\n", $Query_String);

    $this->RS = $this->adodb->Execute($Query_String);
    $this->Row   = 0;
    $this->Errno = $this->adodb->ErrorNo();
    $this->Error = $this->adodb->ErrorMsg();
    if (!$this->RS) {
      $this->halt("Invalid SQL: ".$Query_String);
    }

    # Will return nada if it fails. That's fine.
    return $this->RS;
  }

  /**
   * Fetch next record in the current recordset
   * @access public
   * @return int 1 on success, 0 on failure
   */
  /* public: walk result set */
  function next_record() {
    if (!$this->RS) {
      $this->halt("next_record called with no query pending.");
      return 0;
    }

    $this->Record = $this->RS->FetchRow();
    $this->Row   += 1;
    $this->Errno  = $this->adodb->ErrorNo();
    $this->Error  = $this->adodb->ErrorMsg();

    $stat = is_array($this->Record);
    if (!$stat && $this->Auto_Free) {
      $this->free();
    }
    return $stat;
  }

  /**
   * Set the internal row pointer to $pos 
   * 
   * If $pos is valid, go to $pos record
   *
   * Else go to the end record
   * @access public
   * @param mixed $pos
   * @return int 1 on success, 0 on failure
   */
  /* public: position in result set */
  function seek($pos=0) {
    $status = $this->RS->Move($pos);
    if ($status)
      $this->Row = $pos;
    else {
      $this->halt("seek($pos) failed: result has ".$this->num_rows()." rows");
      return 0;
    }

    return 1;
  }
  
  /**
   * Locking a table or some tables in the current database
   *
   * @access public
   * @param mixed $table
   * @param string $mode
   * @return int positive value (result identifier) on success, 0 on failure
   */
  /* public: table locking */
  function lock($table, $mode="write") {
    if (!$this->connect()) return 0;

    $query="lock tables ";
    if (is_array($table)) {
      while (list($key,$value)=each($table)) {
        if ($key=="read" && $key!=0) {
          $query.="$value read, ";
        } else {
          $query.="$value $mode, ";
        }
      }
      $query=substr($query,0,-2);
    } else {
      $query.="$table $mode";
    }
    $res = $this->adodb->Execute($query);
    if (!$res) {
      $this->halt("lock($table, $mode) failed.");
      return 0;
    }
    return $res;
  }
  /**
   * Unlock all tables in the current database
   * @access public
   * @return int positive value (result identifier) on success, 0 on failure
   */
  function unlock() {
    if (!$this->connect()) return 0;

    $res = $this->adodb->Execute("unlock tables");
    if (!$res) {
      $this->halt("unlock() failed.");
      return 0;
    }
    return $res;
  }

  /**
   * Return the number of rows affected by the previous SQL query (for example: INSERT, DELETE, REPLACE, UPDATE)
   *
   * @access public
   * @return int
   */
  /* public: evaluate the result (size, width) */
  function affected_rows() {
    return $this->adodb->Affected_Rows();
  }

  /**
   * Return the number of rows returned by the previous SELECT SQL query
   *
   * @access public
   * @return int
   */
  function num_rows() {
    return $this->RS->RecordCount();
  }

  /**
   * Return the number of fields in a result set
   * @access public
   * @return int
   */
  function num_fields() {
    return $this->RS->FieldCount();
  }

  /**
   * Return the number of rows
   *
   * Shorthand notation - invoke num_rows() function
   * @access public
   * @return int
   */
  /* public: shorthand notation */
  function nf() {
    return $this->num_rows();
  }

  /**
   * Print the number of rows
   *
   * Print the value returned by <i> num_rows() </i> function 
   * @access public
   */
  function np() {
    print $this->num_rows();
  }

  /**
   * Return the value of $Name field in the current record
   * @access public
   * @param string $Name
   * @return mixed
   */
  function f($Name) {
    return isset($this->Record[$Name]) ? $this->Record[$Name] : null;
  }

  /**
   * Print the value of $Name field in the current record
   * @access public
   * @param string $Name
   */
  function p($Name) {
    print isset($this->Record[$Name]) ? $this->Record[$Name] : '';
  }

  /**
   * Return the next ID value of $seq_name table
   * @access public
   * @param string $seq_name
   * @return int
   */
  /* public: sequence numbers */
  function nextid($seq_name) {
    if (!$this->connect()) return 0;

    if ($this->lock($this->Seq_Table)) {
      /* get sequence number (locked) and increment */
      $q  = sprintf("select nextid from %s where seq_name = '%s'",
                $this->Seq_Table,
                $seq_name);
      $rs  = $this->adodb->Execute($q);
      $res = $rs->FetchRow();

      /* No current value, make one */
      if (!is_array($res)) {
        $currentid = 0;
        $q = sprintf("insert into %s values('%s', %s)",
                 $this->Seq_Table,
                 $seq_name,
                 $currentid);
        $id = $this->adodb->Execute($q);
      } else {
        $currentid = $res["nextid"];
      }
      $nextid = $currentid + 1;
      $q = sprintf("update %s set nextid = '%s' where seq_name = '%s'",
               $this->Seq_Table,
               $nextid,
               $seq_name);
      $id = $this->adodb->Execute($q);
      $this->unlock();
    } else {
      $this->halt("cannot lock ".$this->Seq_Table." - has it been created?");
      return 0;
    }
    return $nextid;
  }

  /**
   * Return metadata of a table (determined by $table parameter) or of a query result
   *
   * Depending on $full parameter, this function returns the short/full values:  
   * @access public
   * @param string $table
   * @param int $full
   * @return mixed array of metadata on success
   */
  /* public: return table metadata */
  function metadata($table='',$full=false) {
    $count = 0;
    $id    = 0;
    $res   = array();

    /* NOTIMPL
     * Due to compatibility problems with Table we changed the behavior
     * of metadata();
     * depending on $full, metadata returns the following values:
     *
     * - full is false (default):
     * $result[]:
     *   [0]["table"]  table name
     *   [0]["name"]   field name
     *   [0]["type"]   field type
     *   [0]["len"]    field length
     *   [0]["flags"]  field flags
     *
     * - full is true
     * $result[]:
     *   ["num_fields"] number of metadata records
     *   [0]["table"]  table name
     *   [0]["name"]   field name
     *   [0]["type"]   field type
     *   [0]["len"]    field length
     *   [0]["flags"]  field flags
     *   ["meta"][field name]  index of field named "field name"
     *   The last one is used, if you have a field name, but no index.
     *   Test:  if (isset($result['meta']['myfield'])) { ...
     */

    // if no $table specified, assume that we are working with a query
    // result
    if ($table) {
      $this->connect();
      $ret = $this->adodb->MetaColumns($table);
      $count = count($ret);
      if ($full)
        $res["num_fields"]= $count;

      $i = 0;
      foreach ($ret as $r) {
        $res[$i]["table"] = $table;
	$res[$i]["name"] = $r->name;
	$res[$i]["type"] = $r->type;
	$res[$i]["len"] = $r->max_length;
	$res[$i]["flags"] = '';
	if ($full)
          $res["meta"][$res[$i]["name"]] = $i;
	$i ++;
      }
    } else {
      $count = $this->RS->FieldCount();
      if ($full)
        $res["num_fields"]= $count;

      for ($i = 0;$i < $count;$i ++) {
        $field = $this->RS->FetchField($i);
        $res[$i]["table"] = '';
	$res[$i]["name"] = $field->name;
	$res[$i]["type"] = $field->type;
	$res[$i]["len"] = $field->max_length;
	$res[$i]["flags"] = '';
	if ($full)
          $res["meta"][$res[$i]["name"]] = $i;
      }
    }

    return $res;
  }

  /**
   * Error handling
   * @access private
   * @param string $msg
   */
  /* private: error handling */
  function halt($msg) {
    $this->Error = $this->adodb->ErrorMsg();
    $this->Errno = $this->adodb->ErrorNo();
    if ($this->Halt_On_Error == "no")
      return;

    $this->haltmsg($msg);

    if ($this->Halt_On_Error != "report")
      die("Session halted.");
  }

  /*
   * Display the error information on the web page
   * @access private
   * @param string $msg 
   */
  function haltmsg($msg) {
    printf("</td></tr></table><b>Database error:</b> %s<br>\n", $msg);
    printf("<b>MySQL Error</b>: %s (%s)<br>\n",
      $this->Errno,
      $this->Error);
  }
  /**
   * Return an array listing names of the permanent table (Non-Temporary tables) in the current database
   * @access private
   * @return mixed 
   */
  function table_names() { // NOTIMPL
    $tables = $this->adodb->MetaTables('TABLES');
    $i = 0;
    foreach ($tables as $info)
     {
      $return[$i]["table_name"]= $info;
      $return[$i]["tablespace_name"]=$this->Database;
      $return[$i]["database"]=$this->Database;
      $i++;
     }
   return $return;
  }
}
?>
