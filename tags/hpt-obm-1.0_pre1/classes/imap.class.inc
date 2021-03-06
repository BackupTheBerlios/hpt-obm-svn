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

function quoted_to_ncr($matches)
{
  if (!isset($matches[2]) || $matches[2] == '') {
    $n = hexdec($matches[1]);
    if ($n > 127 && $n < 256)
      return '&#'.$n.';';
  }
  return $matches[0];
}


class imap
{
  var $conn;
  var $count;
  var $unseen;
  var $messages = array();
  var $next_message_id;
  var $message;
  var $servertype;
  var $connectstring;
  var $mailboxes;
  var $mailbox;
  var $username;
  var $mailbox_info;
  var $sort;
  var $next_sort_index;

  /*
     Open connection to server and count total and new messages
   */

  function open($host, $type, $port, $username, $password, $mailbox = "INBOX", $flags = 0)
  {
    global $GO_CONFIG;

    $this->username = $username;
    $this->servertype = strtolower($type);
    $this->mailbox = imap_utf7_encode($mailbox);
    $this->connectstring = $host.":".$port."/".$this->servertype.$GO_CONFIG->email_connectstring_options;

    if ($flags != 0)
    {
      $this->conn = @imap_open("{".$this->connectstring."}".$this->mailbox, $username, $password, $flags);
    }else
    {
      $this->conn = @imap_open("{".$this->connectstring."}".$this->mailbox, $username, $password);
    }
    return $this->conn;
  }

  /*
     Close connection with server
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

  /*
     count total and new messages on server
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


  function status($mailbox)
  {
    $status = imap_status($this->conn, "{".$this->connectstring."}".$mailbox, SA_UNSEEN);
    return $status;
  }

  /*
     return true when server is an IMAP server.
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

  /*
     Return sorted messages into $this->sort
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

  /*
     Loop through messages and return array with message info needed for inbox listing.
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
      $this->message["subject"] = '';

      if (isset($overview->subject))
      {
	$tmp = imap_mime_header_decode($overview->subject);
	foreach ($tmp as $t)
	  if (isset($t->text))
	    $this->message['subject'] .= $t->text;
      }							
      $from = imap_rfc822_parse_adrlist($overview->from, 'localhost');
      if (isset($from[0]->mailbox))
      {
	$host = isset($from[0]->host) ? '@'.$from[0]->host : '';
	$email = $from[0]->mailbox.$host;
      }
      $personal = '';
      if (isset($from[0]->personal))
      {
	$decoded = imap_mime_header_decode($from[0]->personal);
	$personal = $decoded[0]->text;
      }
      $this->message["from"] = $personal == '' ? $email : $personal;	
      $this->message["sender"] = $email;	
      $this->message["flagged"] = $overview->flagged;
      $this->message["size"] = $overview->size;
      $this->message["answered"] = $overview->answered;
      $this->message['new'] = (($overview->recent==1 || $overview->seen==0) && $this->is_imap()) ? true : false;

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

	    $personal = '';
	    if (isset($tmp[$x]->personal))
	    {
	      $decoded = imap_mime_header_decode($tmp[$x]->personal);
	      if(isset($decoded[0]->text))
	      {
	      	$personal = $decoded[0]->text;
	      }
	    }

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

// splits a message given in the body if it is
// a mulitpart mime message and returns the parts,
// if no parts are found, returns false
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

// returns an array with all parts that are
// subparts of the given part
// if no subparts are found, return the body of
// the current part
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

// get an array with the bodies all parts of an email
// the structure of the array corresponds to the
// structure that is available with imap_fetchstructure
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

  /*
     return an array with all message info.
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
      if ((($headerinfo->Unseen == 'U') || ($headerinfo->Recent == 'N')) && $this->is_imap())
      {
	$this->message["new"] = "1";
      }else
      {
	$this->message["new"] = "0";
      }

      $tmp = $headerinfo->from;
      $this->message["sender"] = $tmp[0]->mailbox.'@'.$tmp[0]->host;
      $this->message["from"] = $this->message["sender"];
			$this->message["reply_to"] = $this->message["sender"];
			
      if (isset($tmp[0]->personal))
      {
	$tmp = imap_mime_header_decode($tmp[0]->personal);
	if (isset($tmp[0]->text))
	{
	  $this->message["from"] = $tmp[0]->text;
	}
      }      
      
      if(isset($headerinfo->reply_to))
      {
				$tmp = $headerinfo->reply_to;
	      $this->message["reply_to"] = $tmp[0]->mailbox.'@'.$tmp[0]->host;
	    }
      
      if (isset($headerinfo->Subject))
      {
	$tmp = imap_mime_header_decode($headerinfo->Subject);
	$this->message["subject"] = '';
	foreach ($tmp as $t)
	  if (isset($t->text))
	    $this->message['subject'] .= $t->text;
      }

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
	  {
	    $decoded = imap_mime_header_decode($tmp[$x]->personal);
	    $personal = $decoded[0]->text;
	  }

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
	  {
	    $decoded = imap_mime_header_decode($tmp[$x]->personal);
	    $personal = $decoded[0]->text;
	  }

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
	  {
	    $decoded = imap_mime_header_decode($tmp[$x]->personal);
	    $personal = $decoded[0]->text;
	  }

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

  /*
     private function to get all the parts from a message.
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


/*
   private function to get mimetype in text
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

/*
   private function to get encoding in text
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

function delete($messages)
{
  for ($i=0;$i<count($messages);$i++)
  {
    @imap_delete($this->conn,$messages[$i], FT_UID);
  }
  @imap_expunge($this->conn);
}

function view_part($uid, $part_no, $transfer)
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


function f($name)
{
  $value = isset($this->message[$name]) ? $this->message[$name] : false;
  return $value;
}

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

function subscribe($name)
{
  return imap_subscribe($this->conn, "{".$this->connectstring."}".imap_utf7_encode($name));
}

function unsubscribe($name)
{
  return imap_unsubscribe($this->conn, "{".$this->connectstring."}".imap_utf7_encode($name));
}

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

function create_folder($name)
{
  if (imap_createmailbox($this->conn,"{".$this->connectstring."}".imap_utf7_encode($name)))
  {
    return $this->subscribe($name);
  }
}

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

function get_quota()
{
  return imap_get_quota($this->conn,"user.".$this->username);
}

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
