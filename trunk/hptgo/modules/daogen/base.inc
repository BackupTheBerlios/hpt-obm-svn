<?php
echo "<h1>Value Object Base Class</h1>";
print "<textarea name=\"valueObj\" cols=\"80\" rows=\"15\">";
print htmlspecialchars("<?php\n");
?>
class valueObject {
        function toString() {
                $arr = get_object_vars($this);
                $str = get_class($this)." {\n";
                foreach($arr as $k => $v) {
                        $str .= "\t$k: $v;\n";
                }
                $str .= "}\n";
                return $str;
        }
                                                                                                                                            
        function serialize() {
                $arr = get_object_vars($this);
                return $arr;
        }
                                                                                                                                            
        function unserialize($serializedValueObject) {
                if(is_array($serializedValueObject)) {
                        foreach($serializedValueObject as $k => $v)
                                $this->$k = $v;
                                                                                                                                            
                        return $this;
                }
                else {
                        return FALSE;
                }
        }
}
<?php
print htmlspecialchars("\n?>");
echo '</textarea>';
print "</pre>";

echo "<h1>Data Access Object Base Class</h1>";
print "<pre>";
print "<textarea name=\"valueObj\" cols=\"80\" rows=\"15\">";
print htmlspecialchars("<?php\n");
?>
/**
  * Data Access Object (DAO).
  * This class contains all database handling that is needed to
  * permanently store and retrieve object instances.
  */

/**
 * Replace this with an inclusion of the database handler class
 */

class Dao
{

	/**
	 * populateVo-method.  This will take a database row and populate
	 * a Value Object with it's contents.
	 *
	 * @param row This is a row object from the db_mysql->query call
	 */
	function populateVo(&$row) {
		die("Must be implemented fully inside child class");
	}
	
	/**
	 * doQuery-method.  This method serves as a wrapper to the database
	 * class used behind this DAO.  The purpose is twofold: one, don't
	 * duplicate code; two, make it easy to change out the database backend
	 * three, keep all database error checking in one place
	 *
	 * @param sql SQL string to use as query
	 * @return result Database result object
	 */
	function doQuery($sql) {
		$db = db_mysql::instance();

		$dbresult = $db->query($sql);

		if($db->geterrorstatus() != 0) {
			//return and raise an error
			trigger_error("Database Error".$sql);
			$return = FALSE;
		}
		else {
			$return =& $dbresult; 
		}

		return $return;
	}

	/**
	 * sanitize-method.  This method sanitizes database variables.
	 *
	 * @param var Unclean variable
	 * @return var Clean variable
	 */
	function sanitize($var) {
		$db = db_mysql::instance();
		return $db->sanitize($var);
	}

	/**
	 * checkPrimaryKey-method.  This method makes sure that the primary keys
	 * of the value object being passed are defined because they must be used
	 * in the SQL query.
	 *
	 * @param vo Value Object to check
	 * @return boolean TRUE/FALSE depending on whether it passed the test
	 */
	function checkPrimaryKeys(&$vo) {
		die("Must be implemented fully inside child class");
	}

	/**
	 * exists-method.  this will execute a query and return
	 * true or false if the passed valueobject exists in the
	 * data store.
	 *
	 * @param vo  this parameter contains the class instance to be loaded.
	 */
	function exists(&$vo) {
		if($this->checkPrimaryKeys($vo)) {
			$sql = $this->selectAll_SQLHOOK().$this->where_SQLHOOK(&$vo);
			
			$dbresult = $this->doQuery($sql);

			if($dbresult->total_rows() == 0 || !$dbresult) {
				$return FALSE;
			}
			else {
				$return TRUE;
			}
		}
		else {
			$return FALSE;
		}
		return $return;
	}

	/**
	 * load-method. this will load vo contents from database using
	 * primary-key as identifier. upper layer should use this so that vo
	 * instance is created and only primary-key should be specified. then call
	 * this method to complete other persistent information. this method will
	 * overwrite all other fields except primary-key and possible runtime variables.
	 *
	 * @param vo  this parameter contains the class instance to be loaded.
	 *                     primary-key field must be set for this to work properly.
	 */
	function load(&$vo) 
	{
		if($this->checkPrimaryKeys($vo)) {
			$sql = $this->selectAll_SQLHOOK().$this->where_SQLHOOK(&$vo);
			unset($vo);
			$dbresult = $this->doQuery($sql);
			$row = $dbresult->next_obj();

			$result = $this->populateVo($row);
		}
		else {
			$result = FALSE;
		}

		return $result;
	}


	/**
	 * loadall-method. this will read all contents from database table and
	 * build an vector containing vos. please note, that this method
	 * will consume huge amounts of resources if table has lot's of rows.
	 * this should only be used when target tables have only small amounts
	 * of data.
	 *
	 */
	function loadAll() 
	{
		  $sql = $this->selectAll_SQLHOOK().$this->orderBy_SQLHOOK;
		  $dbresult = $this->doQuery($sql);

		  $searchresults = array();
		  while($row = $dbresult->next_obj()) 
			$searchresults[] = $this->populateVo($row);

		  return $searchresults;
	}

	/**
	 * loadlimit-method. this will read all contents from database table and
	 * build an vector containing vos. please note, that this method
	 * will consume huge amounts of resources if table has lot's of rows.
	 * this should only be used when target tables have only small amounts
	 * of data.
	 *
	 */
	function loadLimit($limit=10,$offset=0) 
	{
		  if($offset == 0)
			  $limit = "LIMIT $limit";
		  else
			  $limit = "LIMIT $offset,$limit";

		  $sql = $this->selectAll_SQLHOOK().$this->orderBy_SQLHOOK." $limit";

		  $dbresult = $this->doQuery($sql);

		  $searchresults = array();
		  while($row = $dbresult->next_obj()) 
			$searchresults[] = $this->populateVo($row);

		  return $searchresults;
	}

	/**
	 * create-method. this will create new row in database according to supplied
	 * vo contents. make sure that values for all not null columns are
	 * correctly specified. also, if this table does not use automatic surrogate-keys
	 * the primary-key must be specified. after insert command this method will
	 * read the generated primary-key back to vo if automatic surrogate-keys
	 * were used.
	 *
	 * @param vo  this parameter contains the class instance to be created.
	 *                  if automatic surrogate-keys are not used the primary-key
	 *                  field must be set for this to work properly.
	 */
	function create(&$vo) 
	{
		$db = db_mysql::instance();
		$sql = $this->insert_SQLHOOK(&$vo);
		$this->doQuery($sql);
		$result = $db->getlatestinsertid();
		return $result;
	}

	/**
	 * save-method. this method will discover the current state of the vo in the
	 * database.  it will decide whether it already exists and update the record
	 * or it will insert it as a new record.
	 *
	 * @param vo  this parameter contains the class instance to be updated.
	 *                     primary-key field must be set for this to work properly.
	 */

	function save(&$vo) {
		if($this->exists($vo)) {
			return $this->update($vo);
		}
		else {
			return $this->create($vo);
		}
	}

	/**
	 * update-method. this method will update the current state of vo to database.
	 * save can not be used to create new instances in database, so upper layer must
	 * make sure that the primary-key is correctly specified. primary-key will indicate
	 * which instance is going to be updated in database.
	 *
	 * @param vo  this parameter contains the class instance to be updated.
	 *                     primary-key field must be set for this to work properly.
	 */
	function update(&$vo) 
	{
		$sql = $this->update_SQLHOOK(&$vo);

		$this->doQuery($sql);

		return true;
	}


	/**
	 * delete-method. this method will remove the information from database as identified by
	 * by primary-key in supplied vo. once vo has been deleted it can not
	 * be restored by calling save. restoring can only be done using create method but if
	 * database is using automatic surrogate-keys, the resulting object will have different
	 * primary-key than what it was in the deleted object.
	 *
	 * @param vo  this parameter contains the class instance to be deleted.
	 *                     primary-key field must be set for this to work properly.
	 */
	function delete(&$vo) 
	{
		if($this->checkPrimaryKeys($vo)) {
			$sql = $this->delete_SQLHOOK().$this->where_SQLHOOK(&$vo);
			$this->doQuery($sql);
			$result = true;
		}
		else {
			$result = false;
		}

		return $result;
	}

	/**
	* deleteall-method. this method will remove all information from the table that matches
	* this dao and valueobject couple. this should be the most efficient way to clear table.
	* once deleteall has been called, no vo that has been created before can be
	* restored by calling save. Restoring can only be done using create method but if database
	* is using automatic surrogate-keys, the resulting object will have different primary-key
	* than what it was in the deleted object. (Note, the implementation of this method should
	* be different with different DB backends.)
	*/
	function deleteAll() 
	{
		$sql = $this->delete_SQLHOOK();

		$this->doQuery($sql);

		return TRUE;
	}

	/**
	* coutAll-method. This method will return the number of all rows from table that matches
	* this Dao. The implementation will simply execute "select count(primarykey) from table".
	* If table is empty, the return value is 0. This method should be used before calling
	* loadAll, to make sure table has not too many rows.
	*
	*/
	function countAll() 
	{
		$result = $this->doQuery($this->countAll_SQLHOOK());

		$row = $result->next_row();

		return $row[0];
	}

	/**
	 * searchMatching-Method. This method provides searching capability to
	 * get matching vos from database. It works by searching all
	 * objects that match permanent instance variables of this objects.
	 * Upper layer should use this by setting some parameters in vo
	 * and then  call searchMatching. The result will be 0-N objects in vector,
	 * all matching those criteria you specified. Those instance-variables that
	 * have NULL values are excluded in search-criteria.
	 *
	 * @param vo  This parameter contains the class instance where search will be based.
	 *            Primary-key field should not be set.
	 */
	function searchMatching(&$vo) 
	{
		$dbresult = $this->doQuery($this->searchMatching_SQLHOOK(&$vo));

		$searchresults = array();
		while($row = $dbresult->next_obj()) 
			$searchresults[] = $this->populateVo($row);

		return $searchresults;
	}
}
<?php
print htmlspecialchars("\n?>");
echo '</textarea>';
print "</pre>";
?>
