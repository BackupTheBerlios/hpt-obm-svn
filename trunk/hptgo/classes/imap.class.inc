<?php
/*
   Copyright Intermesh 2004
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003
   Version: 1.1 Release date: 09 April 2004

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

/**
 * Convert $matches quoted format to NCR format.
 * Return the converted result. 
 * Used by imap.view_part()
 *
 * @param array $matches
 * @return string
 */
function quoted_to_ncr($matches)
{
  if (!isset($matches[2]) || $matches[2] == '') {
    $n = hexdec($matches[1]);
    if ($n > 127 && $n < 256)
      return '&#'.$n.';';
  }
  return $matches[0];
}

/**
 * Class for accessing IMAP mail folders.
 *  @author	Merijn Schering <mschering@intermesh.nl>
 *  @version	1.0 Release date: 08 July 2003
 *  @version	1.1 Release date: 09 April 2004
 *  @package	imap
 */
class imap
{
  /**
  * Imap stream
  * @var resource 
  */
  var $conn;
  /**
  * Count number mail
  * @var int
  */
  var $count;
  /**
  * ???
  */
  var $unseen;
  /**
  * Messages
  * @var array
  */
  var $messages = array();
  /**
  * Next message id
  * @var int
  */
  var $next_message_id;
  /**
  * A message
  * @var string
  */
  var $message;
  /**
  * Server type
  * @var string
  */
  var $servertype;
  /**
  * Connect string
  * @var string
  */
  var $connectstring;
  /**
  * Mail boxes name array
  * @var array
  */
  var $mailboxes;
  /**
  * Current mail box name
  * @var string
  */
  var $mailbox;
  /**
  * User name
  * @var string
  */
  var $username;
  /**
  * Mail box Information
  * @var string
  */
  var $mailbox_info;
  /**
  * Sort type
  * @var string
  */
  var $sort;
  /**
  * ???
  */
  var $next_sort_index;

  /**
   * Open connection to server and count total and new messages
   * $host, $port, $username, $password specify what host/port to connect,
   * what username/password is used to authenticate.
   *
   * @param string $host host name
   * @param string $type must be 'pop3' or 'imap'.
   * @param string $username
   * @param string $password
   * @param string $mailbox is used as root folder (imap protocol)
   * @param integer $flags this parameter is passed directly to imap_open()
   * @param boolean $ssl is true if it's a ssl(ed) protocol.
   * @param boolean $novalidate_cert: do not validate certification.
   * @return boolean
   */
  function open($host, $type, $port, $username, $password, $mailbox = "INBOX", $flags = 0, $ssl=false, $novalidate_cert=false)
  {
    global $GO_CONFIG;

    $this->username = $username;
    $this->servertype = strtolower($type);
    $this->mailbox = imap_utf7_encode($mailbox);
    $this->connectstring = $host.":".$port."/".$this->servertype.$GO_CONFIG->email_connectstring_options;

    if($ssl)
    {
    	$this->connectstring .= '/ssl';
    }
    
    if($novalidate_cert)
    {
    	$this->connectstring .= '/novalidate-cert';
    }

    if ($flags != 0)
    {
      $this->conn = @imap_open("{".$this->connectstring."}".$this->mailbox, $username, $password, $flags);
    }else
    {
      $this->conn = @imap_open("{".$this->connectstring."}".$this->mailbox, $username, $password);
    }
    return $this->conn;
  }

  /**
   *  Close connection with server
   */
  function close()
  {
    unset($this->messages);
    unset($this->count);
    unset($this->unseen);
    unset($this->next_message_id);
    @imap_close($this->conn);
    unset($this->conn);
  }

  /**
   * count total and new messages on server
   * @return	array	result of imap_status
   */
  function count()
  {
    $status = @imap_status($this->conn, "{".$this->connectstring."}".$this->mailbox, SA_ALL);
    if ($this->is_imap())
    {
      $this->unseen = $status->unseen;
    }

    $this->count = $status->messages;
    return $status;
  }


  /**
   * Get the status of mailbox $mailbox (the result of imap_status())
   *
   * @param		string	$mailbox
   * @return 	array 	result of imap_status
   */
  function status($mailbox)
  {
    $status = imap_status($this->conn, "{".$this->connectstring."}".$mailbox, SA_UNSEEN);
    return $status;
  }

  /**
   * @return	boolean	true when server is an IMAP server.
   */
  function is_imap()
  {
    if ($this->servertype == "imap")
    {
      return true;
    }else
    {
      return false;
    }
  }

  /**
   * Return sorted messages into $this->sort
   * if $uid is true, FT_UID is passed to imap_sort()
   * @param	const	$type	sort type
   * 		SORTDATE - message Date 
   *		SORTARRIVAL - arrival date 
   *		SORTFROM - mailbox in first From address 
   *		SORTSUBJECT - message subject 
   *		SORTTO - mailbox in first To address 
   *		SORTCC - mailbox in first cc address 
   *		SORTSIZE - size of message in octets 
   * @param	int		$reverse	is 1 for reverse-sorting. 
   * @param	bool	$uid		UIDs instead of sequence numbers 
   */
  function sort($type = SORTDATE, $reverse = "1", $uid=false)
  {
    if ($uid)
    {
      $this->sort = imap_sort($this->conn,$type,$reverse, FT_UID);
    }else
    {
      $this->sort = imap_sort($this->conn,$type,$reverse);
    }
  }

  /**
   * FIXME
   * This should be sort(), not search() :(
   * Call imap_sort() and store the result in $this->sort, then return count($this->sort).
   * If something goes wrong, make $this->sort[] empty and return zero.
   * @param		const	$type	sort type
   * 		SORTDATE - message Date 
   *		SORTARRIVAL - arrival date 
   *		SORTFROM - mailbox in first From address 
   *		SORTSUBJECT - message subject 
   *		SORTTO - mailbox in first To address 
   *		SORTCC - mailbox in first cc address 
   *		SORTSIZE - size of message in octets 
   * @param		int		$reverse	is 1 for reverse-sorting. 
   * @param		string	$query		Search criteria
   * @return	int		Finded item number
   */
  function search($sort_type = SORTDATE, $reverse = "1", $query)
  {
    if($this->sort = imap_sort($this->conn, $sort_type, $reverse, SE_NOPREFETCH, trim($query)))
    {
      $this->count = count($this->sort);
    }else
    {
      $this->sort = array();
      $this->count = 0;
    }
    return $this->count;
  }

  /**
   * get $offset messages starting from $first.
   * If $this->sort is true, then messages will be sorted.
   * The result is in $this->messages, $this->sort_index[], $this->sort_map[]
   * @param	int	$first	Position to start to get
   * @param	int	$offset	Number gotten mail
   */
  function get_messages($first, $offset)
  {
    if (!isset($this->sort))
    {
      $this->sort();
    }

    $last = $first+$offset;
    if ($offset==0 || ($last > $this->count))
    {
      $last = $this->count;
    }
    $get = '';
    $index=0;
    $this->sort_index = array();
    for($i=$first;$i<$last;$i++)
    {
      if ($i != $first)
      {
	$get .= ',';
      }
      $get .= $this->sort[$i];
      $this->sort_index[] = $this->sort[$i];
      $index++;
    }

    $tmp = $this->sort_index;
    sort($tmp);
    $count = count($tmp);
    for($i=0;$i<$count;$i++)
    {
      $this->sort_map[$tmp[$i]] = $i;		
    }

    $this->messages = imap_fetch_overview($this->conn, $get);
  }

  /**
   * Loop through messages and return array with message info needed for inbox listing.
   * @param		bool	$get_to_addresses
   * @return	array	with message info needed for inbox listing.
   */
  function next_message($get_to_addresses=false)
  {
    if(!isset($this->messages))
    {
      die('Call get_messages() first');
    }
    $sort_index = array_shift($this->sort_index);

    if(isset($sort_index))
    {			
      $overview = $this->messages[$this->sort_map[$sort_index]];
      unset($this->message);

      $this->message["uid"] = $overview->uid;
      $this->message["msgno"] = $overview->msgno;

      $this->message["udate"] = strtotime(str_replace('CEST', '', $overview->date));
      $this->message["date"] = $overview->date;
      $this->message["subject"] = isset($overview->subject) ? imap_utf8($overview->subject) : '';
      $from = imap_rfc822_parse_adrlist($overview->from, 'localhost');
      if (isset($from[0]->mailbox))
      {
	$host = isset($from[0]->host) ? '@'.$from[0]->host : '';
	$email = $from[0]->mailbox.$host;
      }
      $personal = isset($from[0]->personal) ? imap_utf8($from[0]->personal) : '';
      $this->message["from"] = $personal == '' ? $email : $personal;	
      $this->message["sender"] = $email;	
      $this->message["flagged"] = $overview->flagged;
      $this->message["size"] = $overview->size;
      $this->message["answered"] = $overview->answered;
      //$this->message['new'] = (($overview->recent==1 || $overview->seen==0) && $this->is_imap()) ? true : false;
      $this->message['new'] = ($overview->seen==0 && $this->is_imap()) ? true : false;

      if ($get_to_addresses)
      {
	$headerinfo = imap_header($this->conn, $overview->msgno);
	if (isset($headerinfo->to))
	{
	  $tmp = $headerinfo->to;
	  for ($x=0;$x<sizeof($tmp);$x++)
	  {
	    $email = '';
	    if (isset($tmp[$x]->mailbox))
	    {
	      $host = isset($tmp[$x]->host) ? '@'.$tmp[$x]->host : '';
	      $email = $tmp[$x]->mailbox.$host;
	    }

	    $personal = isset($tmp[$x]->personal) ? imap_utf8($tmp[$x]->personal) : '';

	    if ($personal != '' || $email != '')
	    {
	      $this->message["to"][$x] = $personal;

	      if ($email != '')
	      {
		$this->message["to"][$x] .= "<$email>";
	      }
	    }
	  }
	}
      }

      return $this->message;
    }else
    {
      return false;
    }
  }

/**
 * get part identified by $path from an array of parts $parts
 * @param	array	$parts
 * @param	array	$part
 * @return	mixed	part indentified by $part, false if things go wrong.
 */
function mail_fetchpart($parts, $part) {
 
  $partNos = explode(".", $part);
 
  $currentPart = $parts;
  while(list ($key, $val) = each($partNos)) {
    $currentPart = $currentPart[$val];
  }
 
  if ($currentPart != "")
    return $currentPart;
  else
    return false;
}

/**
 * splits a message given in the body if it is
 * a mulitpart mime message. 
 * @param	string	$header
 * @param	array	$body
 * @return	mixed	returns the parts, if no parts are found, returns false
 */
function mail_mimesplit($header, $body) {
  $parts = array();
 
  $PN_EREG_BOUNDARY = "Content-Type:(.*)boundary=\"([^\"]+)\"";

  if (eregi ($PN_EREG_BOUNDARY, $header, $regs)) {
    $boundary = $regs[2];

    $delimiterReg = "([^\r\n]*)$boundary([^\r\n]*)";
    if (eregi ($delimiterReg, $body, $results)) {
      $delimiter = $results[0];
      $parts = explode($delimiter, $body);
      $parts = array_slice ($parts, 1, -1);
    }
    return $parts;
  } else {
    return false;
  }   
}

/**
 * Split $part into header and body
 * @param	string	$part
 * @return	array	an array ('header','body') of $part
 */
function mail_mimesubme($part) {
  $headDelimiter = "\r\n\r\n";
  $delLength = strlen($headDelimiter);

  // get head & body of the current part
  $endOfHead = strpos( $part, $headDelimiter);
  $head = substr($part, 0, $endOfHead);
  $body = substr($part, $endOfHead + $delLength, strlen($part));
 
  $returnParts['header'] = $head;
  $returnParts['body'] = $body;
  return $returnParts;
}

/**
 * @returns	array	an array with all parts that are
 * subparts of the given part
 * if no subparts are found, return the body of
 * the current part
 */
function mail_mimesub($part) {
  $i = 1;
  $headDelimiter = "\r\n\r\n";
  $delLength = strlen($headDelimiter);

  // get head & body of the current part
  $endOfHead = strpos( $part, $headDelimiter);
  $head = substr($part, 0, $endOfHead);
  $body = substr($part, $endOfHead + $delLength, strlen($part));
 
  $returnParts['header'] = $head;
  // check whether it is a message according to rfc822
  if (stristr($head, "Content-Type: message/rfc822")) {
    $part = substr($part, $endOfHead + $delLength, strlen($part));
    $returnParts[1] = $this->mail_mimesub($part);
    return $returnParts;
  // if no message, get subparts and call function recursively
  } elseif ($subParts = $this->mail_mimesplit($head, $body)) {
    // got more subparts
    while (list ($key, $val) = each($subParts)) {
      $returnParts[$i] = $this->mail_mimesub($val);
      $i++;
    }           
    return $returnParts;
  } else {
    return $head;
  }
}

/**
 * get an array with the bodies all parts of an email
 * the structure of the array corresponds to the
 * structure that is available with imap_fetchstructure
 * @param	int		$msgNo
 * @return	array	parts of mail
 */
function mail_fetchparts($msgNo) {
  $parts = array();
  $header = imap_fetchheader($this->conn,$msgNo,FT_UID);
  $body = imap_body($this->conn,$msgNo,FT_UID);
  $i = 1;
  
  if ($newParts = $this->mail_mimesplit($header, $body)) {
    while (list ($key, $val) = each($newParts)) {
      $parts[$i] = $this->mail_mimesub($val);
      $i++;               
    }
  } else {
    $parts[$i] = $body;
  }
  return $parts;
}

  /**
   * @param		int		$iud
   * @param		string	$preferred_type
   * @return	array	an array with all message info.
   */
  function get_message($uid, $preferred_type = "html", $part="")
  {
    if ($this->conn)
    {
      unset($this->message);

      //determine next and previous message
      if(!isset($this->sort))
      {
	$this->sort();
      }

      $this->message['number'] = imap_msgno($this->conn, $uid);

      for ($i=0;$i<sizeof($this->sort);$i++)
      {
	if ($this->message['number'] == $this->sort[$i])
	{
	  $this->message["next"] = ($i - 1 >= 0) ? imap_uid($this->conn, $this->sort[$i - 1]) : 0;
	  $this->message["previous"] = ($i + 1 < sizeof($this->sort)) ? imap_uid($this->conn, $this->sort[$i + 1]) : 0;
	  break;
	}
      }

      $headerinfo = imap_header($this->conn, $this->message['number']);

      $this->message["udate"] = $headerinfo->udate;
      /*
      if ((($headerinfo->Unseen == 'U') || ($headerinfo->Recent == 'N')) && $this->is_imap())
      {
	$this->message["new"] = "1";
      }else
      {
	$this->message["new"] = "0";
      }
      */

      $this->message['new'] = ($headerinfo->Unseen == 'U' && $this->is_imap()) ? true : false;
      
      $tmp = $headerinfo->from;
      $this->message["sender"] = $tmp[0]->mailbox.'@'.$tmp[0]->host;
      $this->message["from"] = $this->message["sender"];
			$this->message["reply_to"] = $this->message["sender"];
			
      if (isset($tmp[0]->personal))
	$this->message["from"] = imap_utf8($tmp[0]->personal);
      
      if(isset($headerinfo->reply_to))
      {
				$tmp = $headerinfo->reply_to;
	      $this->message["reply_to"] = $tmp[0]->mailbox.'@'.$tmp[0]->host;
	    }
      
      if (isset($headerinfo->Subject))
	$this->message['subject'] .= imap_utf8($headerinfo->Subject);

      if (isset($headerinfo->to))
      {
	$tmp = $headerinfo->to;
	for ($x=0;$x<sizeof($tmp);$x++)
	{
	  $email = '';
	  if (isset($tmp[$x]->mailbox))
	  {
	    $host = isset($tmp[$x]->host) ? '@'.$tmp[$x]->host : '';
	    $email = $tmp[$x]->mailbox.$host;
	  }

	  $personal = '';
	  if (isset($tmp[$x]->personal))
	    $personal = imap_utf8($tmp[$x]->personal);

	  if ($personal != '' || $email != '')
	  {
	    $this->message["to"][$x] = $personal;

	    if ($email != '')
	    {
	      $this->message["to"][$x] .= "<$email>";
	    }
	  }
	}
      }
      if (isset($headerinfo->cc))
      {
	$tmp = $headerinfo->cc;
	for ($x=0;$x<sizeof($tmp);$x++)
	{
	  $email = '';
	  if (isset($tmp[$x]->mailbox))
	  {
	    $host = isset($tmp[$x]->host) ? '@'.$tmp[$x]->host : '';
	    $email = $tmp[$x]->mailbox.$host;
	  }

	  $personal = '';
	  if (isset($tmp[$x]->personal))
	    $personal = imap_utf8($tmp[$x]->personal);

	  if ($personal != '' || $email != '')
	  {
	    $this->message["cc"][$x] = $personal;

	    if ($email != '')
	    {
	      $this->message["cc"][$x] .= "<$email>";
	    }
	  }
	}
      }

      if (isset($headerinfo->bcc))
      {
	$tmp = $headerinfo->bcc;
	for ($x=0;$x<sizeof($tmp);$x++)
	{
	  $email = '';
	  if (isset($tmp[$x]->mailbox))
	  {
	    $host = isset($tmp[$x]->host) ? '@'.$tmp[$x]->host : '';
	    $email = $tmp[$x]->mailbox.$host;
	  }

	  $personal = '';
	  if (isset($tmp[$x]->personal))
	    $personal = imap_utf8($tmp[$x]->personal);

	  if ($personal != '' || $email != '')
	  {
	    $this->message["bcc"][$x] = $personal;

	    if ($email != '')
	    {
	      $this->message["bcc"][$x] .= "<$email>";
	    }
	  }
	}
      }

      $this->message["parts"] = array();

      if ($part == '')
      {
	$structure = imap_fetchstructure($this->conn, $uid, FT_UID);
      }else
      {
	$structure = imap_bodystruct($this->conn, $this->message['number'], $part);
      }
      $this->mail["parts"] = array();
      $headers = $this->mail_fetchparts($uid);
      $body = imap_body($this->conn,$uid,FT_UID);
      $header = imap_fetchheader($this->conn,$uid,FT_UID);
      $this->get_parts($structure, $preferred_type,'',$header,$body);

      //$this->print_structure($structure);

      $header = imap_fetchheader($this->conn, $uid, FT_UID);
      $this->message["priority"] = $this->get_header_value("X-Priority:", $header);
      $this->message["notification"] = $this->get_header_value("Disposition-Notification-To:", $header);
      $this->message["header"] = $header;

      $overview = imap_fetch_overview($this->conn, $uid, FT_UID);
      $this->message["flagged"] = $overview[0]->flagged;
      $this->message["size"] = $overview[0]->size;
      $this->message["answered"] = $overview[0]->answered;

      return $this->message;

    }else
    {
      return false;
    }
  }

  /**
   * private function to get all the parts from a message.
   * @param		$mimeobj
   * @param	string	$preferred_type
   * @param	string	$section
   * @param	string	$header
   * @param	string	$body
   * @param	bool	$skip
   */
  function get_parts($mimeobj,$preferred_type = "html", $section = '',$header,$body,$skip = false)
  {
    if (isset($mimeobj->type))
    {
      $type = $this->get_mime_type($mimeobj->type);
    }else
    {
      $type = 'text';
    }

    $full_mime_type = $type ."/".$mimeobj->subtype;
    $encoding = $this->get_encoding($mimeobj->encoding);

    if (isset($mimeobj->parameters))
    {
      $params = $mimeobj->parameters;
      for ($x=0;$x<count($params);$x++)
      {
	$param = $params[$x];
	if ((strtolower($param->attribute) == 'name') && ($param->value != ''))
	{
	  $name = $param->value;
	  break;
	}
      }
    }
    $name = isset($name) ? $name : '';

    if ((!isset($name) || $name == "") && isset($mimeobj->dparameters))
    {
      $params = $mimeobj->dparameters;
      for ($x=0;$x<count($params);$x++)
      {
	$param = $params[$x];
	if ((strtolower($param->attribute) == 'filename') && ($param->value != ''))
	{
	  $name = $param->value;
	  break;
	}
      }
    }
    $x=0;
    if (isset($mimeobj->parts))
    {
      // split $body into sections
      if (stristr($header,"Content-Type: message/rfc822"))
      {
	$headDelimiter = "\r\n\r\n";
	$delLength = strlen($headDelimiter);

	// get head & body of the current part
	$endOfHead = strpos( $body, $headDelimiter);
	$header = substr($body, 0, $endOfHead);
	$body = substr($body, $endOfHead + $delLength, strlen($body));
	
	//echo "\n<br>822HEADER $nsection<br>\n.".$header."\n<br>EHEADER $nsection<br>\n";
	//echo "\n<br>822BODY $nsection<br>\n.".$body."\n<br>EBODY $nsection<br>\n";
      }
      else
        $newParts = $this->mail_mimesplit($header, $body);
/*
	echo "\n<br>HEADER<br>\n";
	echo $header;
	echo "\n<br>END HEADER<br>\n";
	echo "\n<br>BODY<br>\n";
	echo $body;
	echo "\n<br>END BODY<br>\n";
*/
      for($x=0;$x<count($mimeobj->parts);$x++)
      {
	if ($mimeobj->subtype == "ALTERNATIVE" && $preferred_type == "html")
	{
	  if (isset($mimeobj->parts[$x+1]) && eregi('html', $mimeobj->parts[$x+1]->subtype))
	  {
	    $x++;
	  }else
	  {
	    $preferred_type = 'text';
	  }
	}

	  // Funny really, if a mime section is a inline message that has a multipart body you reference the message
	  // mime section with "2" the inline message header with "2.0" and the subsections with	"2.x"
	  // However if the mime section is a inline message with only 1 part then you reference the
	  // mime section in the message with 2.0 and the	inline message body with 2.1

	if (!$skip)
	  {
	    $nsection = ($section == '' ? $x+1 : $section.".".($x+1));
	  }
	else { $nsection = $section; /*echo "SKIP $nsection";*/ }
	//echo "[$x]";
	if(isset($mimeobj->parts[$x]->parts) && 
	   count($mimeobj->parts[$x]->parts) &&
	   $mimeobj->parts[$x]->type == TYPEMESSAGE &&
	   $mimeobj->parts[$x]->parts[0]->type == TYPEMULTIPART && section != '')
	  {
	    $skip = true;
	  }
	else
	    $skip = false;
	if (isset($newParts))
	{
		//echo "{"."$section-$nsection-".$type."/".$mimeobj->subtype."}";
		// separate sub{header,body}
		//echo "\n<br>PART ".count($newParts)." $x<br>\n.".substr($newParts[$x],0,100)."\n<br>EPART $x<br>\n";
		$parts = $this->mail_mimesubme($newParts[$x]);
		while (substr($parts['header'],0,2) == "\r\n") $parts['header'] = substr($parts['header'],2);
		//echo "\n<br>HEADER $nsection<br>\n.".$parts['header']."\n<br>EHEADER $nsection<br>\n";
		//echo "\n<br>BODY $nsection<br>\n.".substr($parts['body'],0,100)."\n<br>EBODY $nsection<br>\n";
		$this->get_parts($mimeobj->parts[$x],$preferred_type, $nsection ,$parts['header'], $parts['body'] ,$skip);
		//echo "{ return }";
	}
	else
	{
		$this->get_parts($mimeobj->parts[$x],$preferred_type, $nsection ,$header, $body ,$skip);
	}

	if ($mimeobj->subtype == "ALTERNATIVE" && $preferred_type == "plain") $x++;

      }
    }

    // If after processing the entire MIME object the $x variable is still zero then we didn't
    // process a multipart mime message.

    if($x == 0 && $section == 0)
    {
      $section = "1";
    }
    if ($type != "multipart" && $full_mime_type)
    {
      // try to get content-disposition header
      if ($header != '')
      {
	$header = strtolower($header);
	$lines = explode("\r\n",$header);
	$newlines = array();
	$i = 0;
	foreach ($lines as $line)
	{
		if (!eregi("^[^ :]*:",$line) && $i > 0)
			$newlines[count($newlines)-1] .= " ".$line;
		else
		{
			if (eregi("[a-z]",$line))
				$newlines[] = $line;
		}
		$i ++;
	}
	unset($header);
	unset($lines);
	$header = array();
	foreach ($newlines as $line)
	{
		$line_parts = explode(":",$line);
		$header[$line_parts[0]] = $line_parts[1];
	}
      }

      $charset = '';
      if($mimeobj->ifparameters)
      {
      	for ($x=0;$x<count($mimeobj->parameters);$x++)
			  {
					$param = $mimeobj->parameters[$x];
					if ((strtolower($param->attribute) == 'charset') && ($param->value != ''))
					{
					  $charset = $param->value;
					  break;
					}
				}
      }
      
      $bytes = isset($mimeobj->bytes) ? $mimeobj->bytes : 0;
      $tmp = Array(
	  'number' => $section,
	  'id' => $mimeobj->ifid ? $mimeobj->id : 0,
	  'name' => $name,
	  'mime' => $full_mime_type,
	  'transfer' => $encoding,
	  'disposition' => $mimeobj->ifdisposition ? $mimeobj->disposition : '',
	  'size' => $bytes
	  );

      if ($tmp['disposition'] == '' && isset($header['content-disposition']))
	$tmp['disposition'] = $header['content-disposition'];

      array_unshift($this->message["parts"], $tmp);
    }

  }

  /**
   * FIXME
   * @param		???		$mimeobj
   * @param		int		$depth
   * @param		int		$section
   */
  function print_structure($mimeobj, $depth = 0, $section = 0)
  {
    for($y = 0; $y < $depth; $y++)
    {
      echo("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
    }
    echo($this->get_mime_type($mimeobj->type) . "/{$mimeobj->subtype},");
    echo($this->get_encoding($mimeobj->encoding) . "(<B>$section</B>)<br>");

    $x=0;
    if (isset($mimeobj->parts))
    {

      for($x = 0; $x < count($mimeobj->parts); $x++)
      {
	// If we are in the root of the object increment by whole	integers

	if($section == 0)
	{
	  $nsection = $x + 1;
	  $subsection = 0;
	  // If we are in the object and the first sub-object of our	object isn't multipart
	  // then increment the postfix by ".1" otherwise	we are multipart or a message
	  // and leave the section id alone to be handled by the next	code block

	  //else if(($pos = strrpos($section, ".")) && sizeof($mimeobj->parts) > 1)
	}else if(($pos = strrpos($section, ".")) && ($mimeobj->parts[0]->type != TYPEMULTIPART || $mimeobj->parts[0]->subtype != 'RELATED'))
	  //}elseif($pos = strrpos($section, "."))
	{
	  $subsection = (int) substr($section, $pos+1)+$x;
	  if ($subsection == '')
	  {
	    $subsection = '0';
	  }
	  $nsection = substr($section, 0, $pos) .	"." . ($subsection + 1);
	}else
	{
	  $nsection = $section;

	}

	// If there are more parts to the part about to be processed reference it as a header with ".0"
	// but only if the child of this child isn't MULTIPART

	if(isset($mimeobj->parts[$x]->parts) && count($mimeobj->parts[$x]->parts))
	{
	  // Funny really, if a mime section is a inline message that has a multipart body you reference the message
	  // mime section with "2" the inline	message header with "2.0" and the subsections with	"2.x"
	  // However if the mime section is a inline message with only 1 part then you reference the
	  // mime section in the message with 2.0 and the	inline message body with 2.1

	  if(!($mimeobj->parts[$x]->type == TYPEMESSAGE && $mimeobj->parts[$x]->parts[0]->type ==	TYPEMULTIPART))
	  {
	    $nsection .= ".0";
	  }else
	  {
	    $nsection .= "";
	  }
	}

	$this->print_structure($mimeobj->parts[$x], $depth + 1, $nsection);
    }
  }

  // If after processing the entire MIME object the $x variable is still zero then we didn't
  // process a multipart mime message, it's just normal email so say so here.

  if($x == 0 && $section == 0)
  {
    echo($this->get_mime_type($mimeobj->type) . "/{$mimeobj->subtype}, ");
    echo($this->get_encoding($mimeobj->encoding) . "(<B>1</B>) (<B>NOT MIME MULTIPART</B>)<br>");
  }
}


/**
 * private function to get mimetype in text
 * @param	string	$encoding	Encoding
 */

function get_encoding($encoding)
{
  switch ($encoding)
  {
    case 0:
      $encoding = '7BIT';
      break;
    case 1:
      $encoding = '8BIT';
      break;
    case 2:
      $encoding = 'BINARY';
      break;
    case 3:
      $encoding = 'BASE64';
      break;
    case 4:
      $encoding = 'QUOTED-PRINTABLE';
      break;
    case 5:
      $encoding = 'OTHER';
      break;
    default:
      $encoding = 'none';
      break;
  }

  return $encoding;
}

/**
 * private function to get encoding in text
 * @param	string	$type	Mime type
 * @return	string	Encoding in text
 */
function get_mime_type($type)
{
  switch ($type)
  {
    case 0:
      $mime_type = 'text';
      break;
    case 1:
      $mime_type = 'multipart';
      break;
    case 2:
      $mime_type = 'message';
      break;
    case 3:
      $mime_type = 'application';
      break;
    case 4:
      $mime_type = 'audio';
      break;
    case 5:
      $mime_type = 'image';
      break;
    case 6:
      $mime_type = 'video';
      break;
    case 7:
      $mime_type = 'other';
      break;
    default:
      $mime_type = 'unknown';
  }
  return $mime_type;
}


/**
 * get a value of field $fieldname from header $header
 * @param	string	$fieldname	Field name
 * @param	string	$header		Header
 * @param	string	value of field $fieldname
 */
function get_header_value($fieldname, $header)
{
  $resu = '';
  $header = eregi_replace("\t", " ", $header);
  $results = array();
  if (eregi("$fieldname (.*)", $header, $results)) {
    $fieldval = $results[1];
    for ($b=0;$b<=strlen($fieldval);$b++) {
      $curr = substr($fieldval, $b, 1);
      $next = substr($fieldval, $b + 1, 1);
      if ($curr == "\n" && $next != " ") {
	break;
      }
      if ($curr == "\t") { $curr = " "; }
      if ($curr == "\n") { $curr = ""; }
      $resu .= $curr;
    }
  }
  $resu = eregi_replace("\([^\)]*\)", "", $resu);
  return trim($resu);
}

/**
 * Delete $messages (imap_delete() and then imap_expunge())
 * @param	array	a string array
 */
function delete($messages)
{
  for ($i=0;$i<count($messages);$i++)
  {
    @imap_delete($this->conn,$messages[$i], FT_UID);
  }
  @imap_expunge($this->conn);
}

/**
 * Return a MIME part identified by $part_no in message $uid
 * @param	int		$uid
 * @param	int		$part_no
 * @param	string	$transfer
 * @param	string	$part_charset
 * @return	string	Return a MIME part identified by $part_no in message $uid
 */
function view_part($uid, $part_no, $transfer, $part_charset='')
{
  $text = imap_fetchbody($this->conn, $uid, $part_no, FT_UID);
  if ($transfer == 'BASE64')
    $str = imap_base64($text);
  elseif($transfer == 'QUOTED-PRINTABLE') {
    $patterns = array("/=20\r\n/", "/=3D/", "/=\r\n/");
    $replaces = array(' ', '=', '');
    $text = preg_replace ($patterns, $replaces, $text);
    $patterns = array("/=([0-9A-F]{2})(=[0-9A-F]{2})*/");
    $text = preg_replace_callback ($patterns, "quoted_to_ncr", $text);
    $str = quoted_printable_decode($text);
  }
  else
    $str = $text;

  return ($str);
}


/**
 * DB interface's method.
 * @param	string	$name
 * @return	mixed
 */
function f($name)
{
  $value = isset($this->message[$name]) ? $this->message[$name] : false;
  return $value;
}

/**
 * Get mailbox delimiter()
 * @return	mixed	
 */
function get_mailbox_delimiter()
{
  $list = imap_getmailboxes($this->conn, "{".$this->connectstring."}", '%');
  if(is_array($list))
  {
    $folder = array_shift($list);
    if (strlen($folder->delimiter) > 0)
    {
      return $folder->delimiter;
    }
  }
  return false;
}

/**
 * FIXME
 * @param	string	$mbroot
 * @return	mixed	if success return a string : $mbroot + $demiliter, otherwise false
 */
function check_mbroot($mbroot)
{
  $mbroot = trim($mbroot);

  $list = imap_getmailboxes($this->conn, "{".$this->connectstring."}",'%');
  if(is_array($list))
  {
    while($folder = array_shift($list))
    {
      if (!isset($delimiter) && strlen($folder->delimiter) > 0)
      {
	$delimiter = $folder->delimiter;
	if (substr($mbroot, -1) == $delimiter)
	{
	  $mbroot = substr($mbroot, 0, -1);
	}
      }

      if (str_replace("{".$this->connectstring."}","",imap_utf7_decode($folder->name)) == $mbroot)
      {
	return $mbroot.$delimiter;
      }
    }
  }
  return false;
}


/**
 * FIXME
 * @param	string	$mailbox_root
 * @param	bool	$name_only
 * @return	array
 */
function get_mailboxes($mailbox_root='', $name_only=false)
{
  $this->mailboxes = array();
  $list = imap_getmailboxes($this->conn, "{".$this->connectstring."}", $mailbox_root.'%');
  if(is_array($list))
  {
    foreach($list as $value)
    {
      if(substr($value->name, -1) != $value->delimiter && strlen($value->delimiter) > 0)
      {
	if ($name_only)
	{
	  $this->mailboxes[]= str_replace("{".$this->connectstring."}","",imap_utf7_decode($value->name));
	}else
	{
	  $mailbox['name'] = str_replace("{".$this->connectstring."}","",imap_utf7_decode($value->name));
	  $mailbox['delimiter'] = $value->delimiter;
	  $mailbox['attributes'] = $value->attributes;
	  $this->mailboxes[] = $mailbox;
	  if(!($mailbox['attributes']&LATT_NOINFERIORS))
	  {
	    $this->mailboxes = array_merge($this->mailboxes, $this->get_mailboxes($mailbox['name'].$mailbox['delimiter']));
	  }
	}
      }
    }
  }

  return $this->mailboxes;
}

/**
 * FIXME
 * @param	string	$mailbox_root
 * @param	bool	$name_only
 * @return	array
 */
function get_subscribed($mailbox_root='', $name_only=false)
{
  $this->mailboxes = array();
  $list = imap_getsubscribed($this->conn, "{".$this->connectstring."}", imap_utf7_encode($mailbox_root).'%');
  if(is_array($list))
  {
    foreach($list as $value)
    {
      if(substr($value->name, -1) != $value->delimiter && strlen($value->delimiter) > 0)
      {
	if ($name_only)
	{
	  $this->mailboxes[]= str_replace("{".$this->connectstring."}","",imap_utf7_decode($value->name));
	}else
	{
	  $mailbox['name'] = str_replace("{".$this->connectstring."}","",imap_utf7_decode($value->name));
	  $mailbox['delimiter'] = $value->delimiter;
	  $mailbox['attributes'] = $value->attributes;
	  $this->mailboxes[] = $mailbox;
	  if(!($mailbox['attributes']&LATT_NOINFERIORS))
	  {
	    $this->mailboxes = array_merge($this->mailboxes, $this->get_subscribed($mailbox['name'].$mailbox['delimiter']));
	  }
	}
      }
    }
  }

  return $this->mailboxes;
}



/**
 * FIXME
 * @param	string	$name	folder name
 * @param	$mailbox_root
 * @return	bool	TRUE if success, else FALSE
 */
function is_subscribed($name, $mailbox_root)
{
  $this->get_subscribed($mailbox_root);
  for ($i=0;$i<count($this->mailboxes);$i++)
  {
    if ($this->mailboxes[$i]['name']==imap_utf7_encode($name))
    {
      return true;
    }
  }
  return false;
}

/**
 * Subcribe an imap folder named $name
 * @param	string	$name	Folder name
 * @return	bool	TRUE on success or FALSE on failure. 
 */
function subscribe($name)
{
  return imap_subscribe($this->conn, "{".$this->connectstring."}".imap_utf7_encode($name));
}

/**
 * Unsubcribe an imap folder named $name
 * @param	string	$name	Folder name
 * @return	bool	TRUE on success or FALSE on failure. 
 */
function unsubscribe($name)
{
  return imap_unsubscribe($this->conn, "{".$this->connectstring."}".imap_utf7_encode($name));
}

/**
 * Delete folder $name from mailbox $mailbox_root
 * @param	string	$name	Folder name
 * @param	string	$mailbox_root
 * @returns	bool	TRUE on success or FALSE on failure
 */
function delete_folder($name, $mailbox_root)
{
  if ($this->is_subscribed($name, $mailbox_root))
  {
    if ($this->unsubscribe($name))
    {
      return imap_deletemailbox($this->conn,"{".$this->connectstring."}".imap_utf7_encode($name));
    }
    return false;
  }else
  {
    return imap_deletemailbox($this->conn,"{".$this->connectstring."}".imap_utf7_encode($name));
  }
}

/**
 * Create new folder named $name
 * @param	string	$name
 */
function create_folder($name)
{
  if (imap_createmailbox($this->conn,"{".$this->connectstring."}".imap_utf7_encode($name)))
  {
    return $this->subscribe($name);
  }
}

/**
 * Rename folder $old_name to $new_name
 */
function rename_folder($old_name, $new_name)
{
  if ($this->unsubscribe($old_name))
  {
    if (imap_renamemailbox($this->conn,"{".$this->connectstring."}".imap_utf7_encode($old_name),"{".$this->connectstring."}".imap_utf7_encode($new_name)))
    {
      return $this->subscribe($new_name);
    }
  }
  return false;
}

/**
 * Move messages $messages to folder $folder.
 */
function move($folder, $messages)
{
  $messageset = implode (",",$messages);
  if (imap_mail_move($this->conn,$messageset,imap_utf7_encode($folder), CP_UID))
  {
    imap_expunge($this->conn);
    return true;
  }
  return false;
}

/**
 * Get mail quota
 * @returns	array	an array with integer values limit and usage for the given mailbox. 
 * The value of limit represents the total amount of space allowed for this mailbox. 
 * The usage value represents the mailboxes current level of capacity. 
 * Will return FALSE in the case of failure
 */
function get_quota()
{
  return imap_get_quota($this->conn,"user.".$this->username);
}

/**
 * FIXME
 * @param	string	$mailbox
 * @param	string	$body
 * @param	string	$flag
 * @return	bool	TRUE if success, else FALSE 
 */
function append_message($mailbox, $body, $flags = "")
{
  if(@imap_append($this->conn, "{".$this->connectstring."}".imap_utf7_encode($mailbox), $body, $flags))
  {
    return true;
  }else
  {
    return false;
  }
}

/**
 * FIXME
 * @param	string	$mailbox
 * @param	array	$uid_array
 * @param			$flags
 * @param	string	$action
 */
function set_message_flag($mailbox = "INBOX", $uid_array, $flags, $action = "")
{
  if($mailbox == $this->mailbox || $this->reopen($mailbox))
  {
    $msgno_set = implode(",",$uid_array);

    if ($action == "reset")
    {
      if(imap_clearflag_full($this->conn, $msgno_set, $flags, ST_UID))
      {
	return true;
      }else
      {
	return false;
      }
    }else
    {
      if(imap_setflag_full($this->conn, $msgno_set, $flags, ST_UID))
      {
	return true;
      }else
      {
	return false;
      }
    }
  }else
  {
    return false;
  }
}

/**
 * Reopen the connection.
 */
function reopen($mailbox = "INBOX", $flags = "")
{
  if(imap_reopen($this->conn, "{".$this->connectstring."}".imap_utf7_encode($mailbox), $flags))
  {
    $this->mailbox = $mailbox;
    return true;
  }else
  {
    return false;
  }
}

/**
 * Get information from imap_mailboxmsginfo().
 * The result is the array $this->mailbox_info ('date','driver','mailbox','nmsgs','recent','size')
 * Return true if successful, false otherwise
 */
function mailbox_info()
{
  $info = imap_mailboxmsginfo($this->conn);
  if ($info)
  {
    $this->mailbox_info = array();
    $this->mailbox_info["date"]		= $info->Date;
    $this->mailbox_info["driver"]		= $info->Driver;
    $this->mailbox_info["mailbox"]		= $info->Mailbox;
    $this->mailbox_info["nmsgs"]		= $info->Nmsgs;
    $this->mailbox_info["recent"]		= $info->Recent;
    $this->mailbox_info["size"]		= $info->Size;
    return true;
  }else
  {
    return false;
  }
}
}
?>
