<?php
/**
 * Session Management for PHP3
 *
 * Copyright (c) 1998-2000 NetUSE AG
 *                    Boris Erdmann, Kristian Koehntopp
 *
 * $Id: mysql.class.inc,v 1.1.1.1 2004/08/03 11:50:16 vutp Exp $
 */

class db {
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
   * Result identifier
   *
   * The positive number is returned by <i> mysql_query() </i> function
   * @access private
   */
  var $Query_ID = 0;

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
   * Return current Link_ID
   *
   * @access public
   * @return int true on success, false on failure
   */
  function link_id() {
    return $this->Link_ID;
  }

  /**
   * Return current Query_ID
   *
   * @access public
   * @return int positive on success, false on failure
   */
  function query_id() {
    return $this->Query_ID;
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

    /* establish connection, select database */
    if ( 0 == $this->Link_ID ) {

      $this->Link_ID=mysql_pconnect($Host, $User, $Password);
      if (!$this->Link_ID) {
        $this->halt("pconnect($Host, $User, \$Password) failed.");
        return 0;
      }

      if (!@mysql_select_db($Database,$this->Link_ID)) {
        $this->halt("cannot use database ".$this->Database);
        return 0;
      }
    }

    return $this->Link_ID;
  }

  /**
   * Free all associated data in memory
   *
   * @access public
   */
  /* public: discard the query result */
  function free() {
      @mysql_free_result($this->Query_ID);
      $this->Query_ID = 0;
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
    if ($this->Query_ID) {
      $this->free();
    }

    if ($this->Debug)
      printf("Debug: query = %s<br>\n", $Query_String);

    $this->Query_ID = @mysql_query($Query_String,$this->Link_ID);
    $this->Row   = 0;
    $this->Errno = mysql_errno();
    $this->Error = mysql_error();
    if (!$this->Query_ID) {
      $this->halt("Invalid SQL: ".$Query_String);
    }

    # Will return nada if it fails. That's fine.
    return $this->Query_ID;
  }

  /**
   * Fetch next record in the current recordset
   * @access public
   * @return int 1 on success, 0 on failure
   */
  /* public: walk result set */
  function next_record() {
    if (!$this->Query_ID) {
      $this->halt("next_record called with no query pending.");
      return 0;
    }

    $this->Record = @mysql_fetch_array($this->Query_ID);
    $this->Row   += 1;
    $this->Errno  = mysql_errno();
    $this->Error  = mysql_error();

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
    $status = @mysql_data_seek($this->Query_ID, $pos);
    if ($status)
      $this->Row = $pos;
    else {
      $this->halt("seek($pos) failed: result has ".$this->num_rows()." rows");

      /* half assed attempt to save the day,
       * but do not consider this documented or even
       * desireable behaviour.
       */
      @mysql_data_seek($this->Query_ID, $this->num_rows());
      $this->Row = $this->num_rows;
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
    $this->connect();

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
    $res = @mysql_query($query, $this->Link_ID);
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
    $this->connect();

    $res = @mysql_query("unlock tables", $this->Link_ID);
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
    return @mysql_affected_rows($this->Link_ID);
  }

  /**
   * Return the number of rows returned by the previous SELECT SQL query
   *
   * @access public
   * @return int
   */
  function num_rows() {
    return @mysql_num_rows($this->Query_ID);
  }

  /**
   * Return the number of fields in a result set
   * @access public
   * @return int
   */
  function num_fields() {
    return @mysql_num_fields($this->Query_ID);
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
    $this->connect();

    if ($this->lock($this->Seq_Table)) {
      /* get sequence number (locked) and increment */
      $q  = sprintf("select nextid from %s where seq_name = '%s'",
                $this->Seq_Table,
                $seq_name);
      $id  = @mysql_query($q, $this->Link_ID);
      $res = @mysql_fetch_array($id);

      /* No current value, make one */
      if (!is_array($res)) {
        $currentid = 0;
        $q = sprintf("insert into %s values('%s', %s)",
                 $this->Seq_Table,
                 $seq_name,
                 $currentid);
        $id = @mysql_query($q, $this->Link_ID);
      } else {
        $currentid = $res["nextid"];
      }
      $nextid = $currentid + 1;
      $q = sprintf("update %s set nextid = '%s' where seq_name = '%s'",
               $this->Seq_Table,
               $nextid,
               $seq_name);
      $id = @mysql_query($q, $this->Link_ID);
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

    /*
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
      $id = @mysql_list_fields($this->Database, $table);
      if (!$id)
        $this->halt("Metadata query failed.");
    } else {
      $id = $this->Query_ID;
      if (!$id)
        $this->halt("No query specified.");
    }

    $count = @mysql_num_fields($id);

    // made this IF due to performance (one if is faster than $count if's)
    if (!$full) {
      for ($i=0; $i<$count; $i++) {
        $res[$i]["table"] = @mysql_field_table ($id, $i);
        $res[$i]["name"]  = @mysql_field_name  ($id, $i);
        $res[$i]["type"]  = @mysql_field_type  ($id, $i);
        $res[$i]["len"]   = @mysql_field_len   ($id, $i);
        $res[$i]["flags"] = @mysql_field_flags ($id, $i);
      }
    } else { // full
      $res["num_fields"]= $count;

      for ($i=0; $i<$count; $i++) {
        $res[$i]["table"] = @mysql_field_table ($id, $i);
        $res[$i]["name"]  = @mysql_field_name  ($id, $i);
        $res[$i]["type"]  = @mysql_field_type  ($id, $i);
        $res[$i]["len"]   = @mysql_field_len   ($id, $i);
        $res[$i]["flags"] = @mysql_field_flags ($id, $i);
        $res["meta"][$res[$i]["name"]] = $i;
      }
    }

    // free the result only if we were called on a table
    if ($table) @mysql_free_result($id);
    return $res;
  }

  /**
   * Error handling
   * @access private
   * @param string $msg
   */
  /* private: error handling */
  function halt($msg) {
    $this->Error = @mysql_error($this->Link_ID);
    $this->Errno = @mysql_errno($this->Link_ID);
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
  function table_names() {
    $this->query("SHOW TABLES");
    $i=0;
    while ($info=mysql_fetch_row($this->Query_ID))
     {
      $return[$i]["table_name"]= $info[0];
      $return[$i]["tablespace_name"]=$this->Database;
      $return[$i]["database"]=$this->Database;
      $i++;
     }
   return $return;
  }
}
?>
