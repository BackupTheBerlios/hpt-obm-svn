<?php

/**
 * Oracle/OCI8 accessor based on Session Management for PHP3
 *
 * (C) Copyright 1999-2000 Stefan Sels phplib@sels.com
 *
 * based on db_oracle.inc by Luis Francisco Gonzalez Hernandez
 * contains metadata() from db_oracle.inc 1.10
 *
 * $Id: oci8.class.inc,v 1.1.1.1 2004/08/03 11:50:16 vutp Exp $
 */

class db {
  /** 
   * Configuration parameter 
   *
   * Set to 1 for debugging messages
   * 
   * @access public
   *
   */
  var $Debug    =  0;
  
  
  var $sqoe     =  1; // sqoe= show query on error

  /**
   * Host name or database server name 
   *
   * @access public
   */
  var $Host = "";
  
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
   * Link identifier 
   *
   * The positive number is returned by <i> mysql_pconnect() </i> or <i> mysql_connect() </i> functions
   *
   * @access private 
   */
  var $Link_ID    = 0;
  
  /**
   * Result array and current row number 
   *
   * @access public
   */
  var $Record    = array();
  
  /**
   * Storing a record in a recordset
   *
   * @access public
   */
  var $Row;
  
  /**
   * Retain an statement identifier returned by ociparse()/oci_parse() functions
   * 
   * The identifier can be used with <i> oci_execute() </i> function 
   * @access public
   */
  var $Parse;
  
  /**
   * Current error text
   * 
   * @access public
   */
  var $Error     = "";

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
   * If the web page is in debug mode, the web browser will display all statements executed 
   * @access public
   */
  function connect() {
      if ( 0 == $this->Link_ID ) {
          if($this->Debug) {
              printf("<br>Connecting to $this->Database...<br>\n");
          }
          $this->Link_ID=OCIplogon
                ("$this->User","$this->Password","$this->Database");

          if (!$this->Link_ID) {
              $this->halt("Link-ID == false " .
                          "($this->Link_ID), OCILogon failed");
          }

          if($this->Debug) {
              printf("<br>Obtained the Link_ID: $this->Link_ID<br>\n");
          }
      }
  }

  /**
   * Execute a query
   *
   * @access public
   * @param string $Query_String 
   * @return int positive value (statement identifier) on success, 0 on failure
   */
  function query($Query_String) {

    /* No empty queries, please, since PHP4 chokes on them. */
    if ($Query_String == "")
      /* The empty query string is passed on from the constructor,
       * when calling the class without a query, e.g. in situations
       * like these: '$db = new DB_Sql_Subclass;'
       */
      return 0;


      $this->connect();

       $this->Parse=OCIParse($this->Link_ID,$Query_String);
      if(!$this->Parse) {
           $this->Error=OCIError($this->Parse);
      } else { OCIExecute($this->Parse);
          $this->Error=OCIError($this->Parse);
      }

      $this->Row=0;

      if($this->Debug) {
          printf("Debug: query = %s<br>\n", $Query_String);
      }

      if ($this->Error["code"]!=1403 && $this->Error["code"]!=0 && $this->sqoe)
      echo "<BR><FONT color=red><B>".$this->Error["message"]."<BR>Query :\"$Query_String\"</B></FONT>";
      return $this->Parse;
  }

  /**
   * Fetch next record in the current recordset
   * @access public
   * @return int 1 on success, 0 on failure
   */
  function next_record() {
      if(0 == OCIFetchInto($this->Parse,$result,OCI_ASSOC+OCI_RETURN_NULLS)) {
          if ($this->Debug) {
            printf("<br>ID: %d,Rows: %d<br>\n",
              $this->Link_ID,$this->num_rows());
          }
          $this->Row        +=1;

          $errno=OCIError($this->Parse);
          if(1403 == $errno) { # 1043 means no more records found
              $this->Error="";
              $this->disconnect();
              $stat=0;
          } else {
              $this->Error=OCIError($this->Parse);
              if($this->Debug) {
                  printf("<br>Error: %s",
                  $this->Error["message"]);
              }
              $stat=0;
          }
      } else {
          for($ix=1;$ix<=OCINumcols($this->Parse);$ix++) {
              $col=strtoupper(OCIColumnname($this->Parse,$ix));
              $colreturn=strtolower($col);
              $this->Record[ "$colreturn" ] = $result["$col"];
              if($this->Debug) echo"<b>[$col]</b>:".$result["$col"]."<br>\n";
          }
          $stat=1;
      }

  return $stat;
  }

  /**
   * Set the internal row pointer to $pos 
   *
   * @access public
   * @param int $pos
   */
  function seek($pos) {
      $this->Row=$pos;
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
  function metadata($table,$full=false) {
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
     *   [0]["flags"]  field flags ("NOT NULL", "INDEX")
     *   [0]["format"] precision and scale of number (eg. "10,2") or empty
     *   [0]["index"]  name of index (if has one)
     *   [0]["chars"]  number of chars (if any char-type)
     *
     * - full is true
     * $result[]:
     *   ["num_fields"] number of metadata records
     *   [0]["table"]  table name
     *   [0]["name"]   field name
     *   [0]["type"]   field type
     *   [0]["len"]    field length
     *   [0]["flags"]  field flags ("NOT NULL", "INDEX")
     *   [0]["format"] precision and scale of number (eg. "10,2") or empty
     *   [0]["index"]  name of index (if has one)
     *   [0]["chars"]  number of chars (if any char-type)
     *   ["meta"][field name]  index of field named "field name"
     *   The last one is used, if you have a field name, but no index.
     *   Test:  if (isset($result['meta']['myfield'])) {} ...
     */

      $this->connect();

      ## This is a RIGHT OUTER JOIN: "(+)", if you want to see, what
      ## this query results try the following:
      ## $table = new Table; $db = new my_DB_Sql; # you have to make
      ##                                          # your own class
      ## $table->show_results($db->query(see query vvvvvv))
      ##
      $this->query("SELECT T.table_name,T.column_name,T.data_type,".
           "T.data_length,T.data_precision,T.data_scale,T.nullable,".
           "T.char_col_decl_length,I.index_name".
           " FROM ALL_TAB_COLUMNS T,ALL_IND_COLUMNS I".
           " WHERE T.column_name=I.column_name (+)".
           " AND T.table_name=I.table_name (+)".
           " AND T.table_name=UPPER('$table') ORDER BY T.column_id");

      $i=0;
      while ($this->next_record()) {
        $res[$i]["table"] =  $this->Record[table_name];
        $res[$i]["name"]  =  strtolower($this->Record[column_name]);
        $res[$i]["type"]  =  $this->Record[data_type];
        $res[$i]["len"]   =  $this->Record[data_length];
        if ($this->Record[index_name]) $res[$i]["flags"] = "INDEX ";
        $res[$i]["flags"] .= ( $this->Record[nullable] == 'N') ? '' : 'NOT NULL';
        $res[$i]["format"]=  (int)$this->Record[data_precision].",".
                             (int)$this->Record[data_scale];
        if ("0,0"==$res[$i]["format"]) $res[$i]["format"]='';
        $res[$i]["index"] =  $this->Record[index_name];
        $res[$i]["chars"] =  $this->Record[char_col_decl_length];
        if ($full) {
                $j=$res[$i]["name"];
                $res["meta"][$j] = $i;
                $res["meta"][strtoupper($j)] = $i;
        }
        if ($full) $res["meta"][$res[$i]["name"]] = $i;
        $i++;
      }
      if ($full) $res["num_fields"]=$i;
#      $this->disconnect();
      return $res;
  }

  /**
   * Return number of rows affected during statement execution
   *
   * The function invoked <i> num_rows() </i> function
   * @access public
   * @return int
   */
  function affected_rows() {
    return $this->num_rows();
  }

  /**
   * Return number of rows affected during statement execution
   *
   * Note: This function does not return number of rows selected
   * @access public
   * @return int
   */
  function num_rows() {
    return OCIrowcount($this->Parse);
  }

  /**
   * Return the number of fields in a result set
   * @access public
   * @return int
   */
  function num_fields() {
      return OCINumcols($this->Parse);
  }

  /**
   * Return the number of rows affected during statement execution
   *
   * Shorthand notation - invoke <i> num_rows() </i> function
   * @access public
   * @return int
   */
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
  function f($Name){
    if (is_object($this->Record[$Name]))
    {
      return $this->Record[$Name]->load();
    } else
    {
      return $this->Record[$Name];
    }
  }

  /**
   * Print the value of $Name field in the current record
   * @access public
   * @param string $Name
   */
  function p($Name) {
    print $this->f($Name);
  }

  /**
   * Return the next ID value of $seq_name table
   * @access public
   * @param string $seq_name
   * @return int
   */
  function nextid($seqname)
  {
    $this->connect();

    $Query_ID=@ociparse($this->Link_ID,"SELECT $seqname.NEXTVAL FROM DUAL");

    if(!@ociexecute($Query_ID))
    {
	$this->Error=@OCIError($Query_ID);
	if($this->Error["code"]==2289)
	{
	    $Query_ID=ociparse($this->Link_ID,"CREATE SEQUENCE $seqname");
	    if(!ociexecute($Query_ID))
	    {
		$this->Error=OCIError($Query_ID);
		$this->halt("<BR> nextid() function - unable to create sequence<br>".$this->Error["message"]);
	    } else
	     {
		$Query_ID=ociparse($this->Link_ID,"SELECT $seqname.NEXTVAL FROM DUAL");
		ociexecute($Query_ID);
	    }
	}
    }

    if (ocifetch($Query_ID))
    {
       $next_id = ociresult($Query_ID,"NEXTVAL");
    } else
    {
       $next_id = 0;
    }
    ocifreestatement($Query_ID);
    return $next_id;
  }

  /**
   * Free all associated data in memory
   *
   * @access public
   */
  function disconnect() {
      if($this->Debug) {
          printf("Disconnecting...<br>\n");
      }
      OCILogoff($this->Link_ID);
  }

  /**
   * Error handling
   * @access private
   * @param string $msg
   */
  function halt($msg) {
    printf("</td></tr></table><b>Database error:</b> %s<br>\n", $msg);
    printf("<b>ORACLE Error</b>: %s<br>\n",
      $this->Error["message"]);
    die("Session halted.");
  }

  /**
   * Locking a table in the current database
   *
   * @access public
   * @param string $table
   * @param string $mode
   * @return int 1 (true) on success, 0 (false) on failure
   */
  function lock($table, $mode = "write") {
    $this->connect();
    if ($mode == "write") {
      $Parse=OCIParse($this->Link_ID,"lock table $table in row exclusive mode");
      $result = OCIExecute($Parse);
    } else {
      $result = 1;
    }
    return $result;
  }

  /**
   * Unlock all tables in the current database
   * @access public
   * @return int true on success, false on failure
   */
  function unlock() {
    return $this->query("commit");
  }

  /**
   * Return an array listing names of the permanent table (Non-Temporary tables) in the current database
   * @access private
   * @return mixed 
   */
  function table_names() {
   $this->connect();
   $this->query("
   SELECT table_name,tablespace_name
     FROM user_tables");
   $i=0;
   while ($this->next_record())
   {
   $info[$i]["table_name"]     =$this->Record["table_name"];
   $info[$i]["tablespace_name"]=$this->Record["tablespace_name"];
   $i++;
   }
  return $info;
  }

  /**
   * Add special characters to query string
   * @access private
   * @param string $query
   * @return string
   */
  function add_specialcharacters($query)
  {
  	return str_replace("'","''",$query);
  }

  /**
   * Split special characters in query string
   * @access private
   * @param string $query
   * @return string
   */
  function split_specialcharacters($query)
  {
  	return str_replace("''","'",$query);
  }
}
?>
