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

require_once($GO_CONFIG->class_path.'base/crypto.class.inc');
$GO_CRYPTO = new GO_CRYPTO();

class email extends db
{
  var $last_error;

  function get_accounts($user_id)
  {
    $sql = "SELECT * FROM emAccounts WHERE user_id='$user_id' ORDER BY standard DESC";
    $result = $this->query($sql);
    return $this->num_rows($result);
  }

  function re_encrypt_email($user_id, $old_password, $new_password)
  {
    global $GO_CRYPTO;
    if($this->get_accounts($user_id))
    {
      $update=new db();
      while($this->next_record())
      {
	$old_email_password=$GO_CRYPTO->decrypt($this->f('password'), $old_password);
	$email_password=$GO_CRYPTO->encrypt($old_email_password, $new_password);
	$id=$this->f('id');
	$sql="UPDATE emAccounts SET password='$email_password' WHERE user_id='$user_id' AND id='$id'";
	$update->query($sql);
      }
    }
  }

  function set_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE em_settings SET sort_field='$sort_field' , sort_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function save_send_format($user_id, $send_format)
  {
    $sql = "UPDATE em_settings SET send_format='$send_format'  WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function get_settings($user_id)
  {
    $this->query("SELECT * FROM em_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO em_settings (user_id, send_format) VALUES ('$user_id', 'text/HTML')");
      return $this->get_settings($user_id);
    }		
  }

  function add_account($user_id, $type, $host, $port, $use_ssl, $novalidate_cert, $mbroot, $username, $password, $name, $email, $signature, $sent, $spam, $trash, $draft,$auto_check=0)
  {
    global $GO_CONFIG;

    $GO_CRYPTO = new GO_CRYPTO();
    
    require_once($GO_CONFIG->class_path."imap.class.inc");

    $mail= new imap();
    if (!$mail->open($host, $type, $port, $username, $password, "INBOX", 0, $use_ssl, $novalidate_cert))
    {
	    $this->last_error = "<p class=\"Error\">".imap_last_error()."</p>";
      return false;
    }else
    {
      if (!$mbroot = $mail->check_mbroot($mbroot))
      {
	$mbroot = '';
      }

      $account_id  = $this->nextid("emAccounts");
      if (!$account_id)
      {
      	$this->last_error = "<p class=\"Error\">".$strSaveError."</p>";
      	return false;
      }else
      {
	if ($this->get_accounts($user_id) == 0)
	{
	  $default="1";
	}else
	{
	  $default="0";
	}

				$sql  = "INSERT INTO emAccounts (id, user_id, type, host, port, use_ssl, novalidate_cert, mbroot, username, password, name, email, signature, standard, auto_check) ";
				$sql .= "VALUES ('$account_id', '$user_id', '$type', '$host', '$port', '$use_ssl', '$novalidate_cert', '$mbroot', '$username', '".$GO_CRYPTO->encrypt($password)."', '$name', '$email', '$signature','$default' , '$auto_check')";
				if(!$this->query($sql))
				{
					$this->last_error = "<p class=\"Error\">".$strSaveError."</p>";
      		return false;
				}else
      {
	if ($type=='imap')
	{
	  $mailboxes =  $mail->get_mailboxes($mbroot);
	  $subscribed =  $mail->get_subscribed($mbroot);

	  $this->synchronise($account_id, $subscribed, $mailboxes);

	  $mailbox_names = array();
	  while($mailbox = array_shift($mailboxes))
	  {
	    $mailbox_names[]=$mailbox['name'];
	  }

	  $sent = $sent == '' ? '' : $mbroot.$sent;
	  $draft = $draft == '' ? '' : $mbroot.$draft;
	  $spam = $spam == '' ? '' : $mbroot.$spam;
	  $trash = $trash == '' ? '' : $mbroot.$trash;

	  $subscribed_names = array();
	  while($mailbox = array_shift($subscribed))
	  {
	    $subscribed_names[]=$mailbox['name'];
	  }

	  $sent_folder = '';
	  if ($sent != '')
	  {
	    //create sent items folder
	    if (!in_array($sent, $mailbox_names))
	    {
	      if ($mail->create_folder($sent))
	      {
		$sent_folder = $sent;
	      }
	    }else
	    {
	      if (!in_array($sent, $subscribed_names))
	      {
		$mail->subscribe($sent);
	      }
	      $sent_folder = $sent;
	    }
	  }

	  $draft_folder = '';
	  if ($draft != '')
	  {
	    //create draft items folder
	    if (!in_array($draft, $mailbox_names))
	    {
	      if ($mail->create_folder($draft))
	      {
		$draft_folder = $draft;
	      }
	    }else
	    {
	      if (!in_array($draft, $subscribed_names))
	      {
		$mail->subscribe($draft);
	      }
	      $draft_folder = $draft;
	    }
	  }

	  $spam_folder = '';
	  if ($spam != '')
	  {
	    //create spam folder
	    if (!in_array($spam, $mailbox_names))
	    {
	      if($mail->create_folder($spam))
	      {
		$spam_folder = $spam;
	      }
	    }else
	    {
	      if (!in_array($spam, $subscribed_names))
	      {
		$mail->subscribe($spam);
	      }
	      $spam_folder = $spam;
	    }
	  }

	  $trash_folder = '';
	  if ($trash != '')
	  {
	    //create trash folder
	    if (!in_array($trash, $mailbox_names))
	    {
	      if ($mail->create_folder($trash))
	      {
		$trash_folder = $trash;
	      }
	    }else
	    {
	      if (!in_array($trash, $subscribed_names))
	      {
		$mail->subscribe($trash);
	      }
	      $trash_folder = $trash;
	    }
	  }

	  //update the special folders
	  $this->update_folders($account_id, $sent_folder, $spam_folder, $trash_folder, $draft_folder);

	  $mailboxes =  $mail->get_mailboxes($mbroot);
	  $subscribed =  $mail->get_subscribed($mbroot);
	  $this->synchronise($account_id, $mailboxes, $subscribed);
	}
			  	$mail->close();
	return $account_id;
      }
    }
  }
  }

  function disable_auto_check($account_id)
  {
    return $this->query("UPDATE emAccounts SET auto_check='0' WHERE id='$account_id'");
  }

  function update_account($account_id, $type, $host, $port, $use_ssl, $novalidate_cert, $mbroot, $username, $password, $name, $email, $signature, $sent, $spam, $trash, $draft, $auto_check=0)
  {
    global $GO_CONFIG, $GO_CRYPTO;

    require_once($GO_CONFIG->class_path."imap.class.inc");
    $mail= new imap();

    if ($mail->open($host, $type, $port, $username, $password, "INBOX", 0, $use_ssl, $novalidate_cert))
    {
      if (!$mbroot = $mail->check_mbroot($mbroot))
      {
	$mbroot = '';
      }

      $sql  = "UPDATE emAccounts SET type='$type', host='$host', port='$port'".
      ", use_ssl='$use_ssl', novalidate_cert='$novalidate_cert'".
	", mbroot='$mbroot', username='$username', password='".
	",".$GO_CRYPTO->encrypt($password)."', name='$name', email='$email'".
	", signature='$signature', auto_check='$auto_check' WHERE ".
	"id='$account_id'";

      if ($this->query($sql))
      {
	$mailboxes =  $mail->get_mailboxes($mbroot);
	$subscribed =  $mail->get_subscribed($mbroot);
	$this->synchronise($account_id, $subscribed, $mailboxes);

	$mailbox_names = array();
	while($mailbox = array_shift($mailboxes))
	{
	  $mailbox_names[]=$mailbox['name'];
	}

	$subscribed_names = array();
	while($mailbox = array_shift($subscribed))
	{
	  $subscribed_names[]=$mailbox['name'];
	}
	$sent_folder = '';
	if ($sent != '')
	{
	  //create sent items folder
	  if (!in_array($sent, $mailbox_names))
	  {
	    if ($mail->create_folder($sent))
	    {
	      $sent_folder = $sent;
	    }
	  }else
	  {
	    if (!in_array($sent, $subscribed_names))
	    {
	      $mail->subscribe($sent);
	    }
	    $sent_folder = $sent;
	  }
	}

	$draft_folder = '';
	if ($draft != '')
	{
	  //create draft items folder
	  if (!in_array($draft, $mailbox_names))
	  {
	    if ($mail->create_folder($draft))
	    {
	      $draft_folder = $draft;
	    }
	  }else
	  {
	    if (!in_array($draft, $subscribed_names))
	    {
	      $mail->subscribe($draft);
	    }
	    $draft_folder = $draft;
	  }
	}

	$spam_folder = '';
	if ($spam != '')
	{
	  //create spam folder
	  if (!in_array($spam, $mailbox_names))
	  {
	    if($mail->create_folder($spam))
	    {
	      $spam_folder = $spam;
	    }
	  }else
	  {
	    if (!in_array($spam, $subscribed_names))
	    {
	      $mail->subscribe($spam);
	    }
	    $spam_folder = $spam;
	  }
	}

	$trash_folder = '';
	if ($trash != '')
	{
	  //create trash folder
	  if (!in_array($trash, $mailbox_names))
	  {
	    if ($mail->create_folder($trash))
	    {
	      $trash_folder = $trash;
	    }
	  }else
	  {
	    if (!in_array($trash, $subscribed_names))
	    {
	      $mail->subscribe($trash);
	    }
	    $trash_folder = $trash;
	  }
	}

	//update the special folders
	$this->update_folders($account_id, $sent_folder, $spam_folder, $trash_folder, $draft_folder);

	$mailboxes =  $mail->get_mailboxes($mbroot);
	$subscribed =  $mail->get_subscribed($mbroot);
	$this->synchronise($account_id, $mailboxes, $subscribed);

	$mail->close();
      }
      return true;
    }
    return false;
  }

  function update_password($host, $username, $password)
  {
    global  $GO_CRYPTO;
    $sql = "UPDATE emAccounts SET password='".$GO_CRYPTO->encrypt($password).
      "' WHERE username='$username' AND host='$host'";
    return $this->query($sql);
  }

  function update_folders($account_id, $sent, $spam, $trash, $draft)
  {
    $sql = "UPDATE emAccounts SET sent='$sent', spam=".
      "'$spam',trash='$trash',draft='$draft' ".
      "WHERE id='$account_id'";
    return $this->query($sql);
  }

  function get_account($id = 0)
  {
    if ($id == 0)
    {
      $sql = "SELECT * FROM emAccounts WHERE standard='1' AND user_id='".
	$_SESSION['GO_SESSION']['user_id']."'";
    }else
    {
      $sql = "SELECT * FROM emAccounts WHERE id='$id'";
    }

    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      return false;
    }
  }

  function is_default_account($id)
  {
    $sql = "SELECT * FROM emAccounts WHERE id='$id' AND standard='1'";
    $this->query($sql);
    if ($this->num_rows() == 1)
    {
      return true;
    }else
    {
      return false;
    }
  }

  function delete_account($user_id, $id)
  {
    $default = $this->is_default_account($id);

    $sql = "DELETE FROM emAccounts WHERE id='$id' AND user_id='$user_id'";
    if ($this->query($sql))
    {
      $sql = "DELETE FROM emFolders WHERE account_id='$id'";
      $this->query($sql);
      $sql = "DELETE FROM emFilters WHERE account_id='$id'";
      $this->query($sql);
    }
    if ($default)
    {
      $this->get_accounts($user_id);
      $this->next_record();
      $this->set_as_default($this->f("id"), $user_id);
    }
    return true;
  }

  function set_as_default($account_id, $user_id)
  {
    $sql = "UPDATE emAccounts SET standard='0' WHERE user_id='$user_id'".
      "AND standard='1'";
    $this->query($sql);
    $sql = "UPDATE emAccounts SET standard='1' WHERE id='$account_id'";
    $this->query($sql);
  }
  /*
     gets the subfolder of a folder id. Account id is only usefull for the root
     level where all folders have parent 0
   */

  function get_folders($account_id, $folder_id=-1)
  {
    $sql = "SELECT * FROM emFolders WHERE account_id='$account_id' AND ".
      "subscribed='1' ";

    if ($folder_id > -1)
    {
      $sql .= "AND parent_id='$folder_id' ";
    }
    $sql .= "ORDER BY NAME ASC";

    $this->query($sql);
    return $this->num_rows();
  }

  function get_all_folders($account_id, $subscribed_only=false)
  {
    if ($subscribed_only)
    {
      $sql = "SELECT * FROM emFolders WHERE account_id='$account_id' AND ".
	"subscribed='1' ORDER BY NAME ASC";
    }else
    {
      $sql = "SELECT * FROM emFolders WHERE account_id='$account_id' ORDER ".
	"BY NAME ASC";
    }
    $this->query($sql);
    return $this->num_rows();
  }


  function add_folder($account_id, $name, $parent_id=0, $subscribed=1,
      $delimiter='/', $attributes=0)
  {
    $next_id = $this->nextid("emFolders");
    if ($next_id > 0)
    {
      $sql = "INSERT INTO emFolders (id, parent_id, account_id, subscribed, ".
	"name, attributes, delimiter) VALUES ('$next_id', '$parent_id', ".
	"'$account_id', '$subscribed', '$name', ".
	"'$attributes', '$delimiter')";
      if ($this->query($sql))
      {
	return $next_id;
      }
    }
    return false;
  }
  function rename_folder($account_id, $old_name, $new_name)
  {
    $sql = "UPDATE emFolders SET name='$new_name' WHERE".
      " name='".smart_addslashes($old_name)."' AND ".
      "account_id='$account_id'";

    $this->query($sql);
    $sql = "UPDATE emFilters SET folder='$new_name' ".
      "WHERE folder='$old_name' AND ".
      "account_id='$account_id'";
    $this->query($sql);

  }

  function update_folder($folder_id, $parent_id, $subscribed, $attributes)
  {
    $sql = "UPDATE emFolders SET subscribed='$subscribed', ".
      "parent_id='$parent_id', attributes='$attributes' ".
      "WHERE id='$folder_id'";

    $this->query($sql);
  }

  function delete_folder($account_id, $name)
  {
    $sql = "DELETE FROM emFolders WHERE account_id='$account_id' ".
      "AND name='$name'";
    $this->query($sql);

    $sql = "DELETE FROM emFilters WHERE account_id='$account_id' ".
      "AND folder='$name'";
    $this->query($sql);
  }
  function folder_exists($account_id, $name)
  {
    $sql = "SELECT id FROM emFolders WHERE name='$name' AND ">
      "account_id='$account_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->f("id");
    }else
    {
      return false;
    }
  }

  function get_folder($account_id, $name)
  {
    $sql = "SELECT * FROM emFolders WHERE name='$name' AND ".
      "account_id='$account_id'";
    $this->query($sql);
    if ($this->next_record())
    {
      return $this->Record;;
    }else
    {
      return false;
    }
  }

  function subscribe($account_id, $name)
  {
    return $this->query("UPDATE emFolders SET subscribed='1' ".
	"WHERE account_id='$account_id' AND name='$name'");
  }

  function unsubscribe($account_id, $name)
  {
    return $this->query("UPDATE emFolders SET subscribed='0' ".
	"WHERE account_id='$account_id' AND name='$name'");
  }

  /*
     Gets the parent_id from a folder path
   */
  function get_parent_id($account_id, $path, $delimiter)
  {
    if ($pos = strrpos($path, $delimiter))
    {
      $parent_name = substr($path, 0, $pos);
      if ($parent_folder = $this->get_folder($account_id, $parent_name))
      {
	return $parent_folder['id'];
      }
    }else
    {
      return 0;
    }
    return false;

  }

  /*
     IMAP function to synchronise Group-Office folders with the IMAP server
   */
  function synchronise($account_id,$mailboxes, $subscribed)
  {
    //add the subscribed folders into a searchable array
    $subscribed_names = array();
    $mailbox_names = array();
    while($mailbox = array_shift($subscribed))
    {
      $subscribed_names[]=$mailbox['name'];
    }
    //Add all missing folders to Group-Office
    while($mailbox = array_shift($mailboxes))
    {
      $mailbox_names[] = $mailbox['name'];
      $parent_id = $this->get_parent_id($account_id, addslashes($mailbox['name']),
	  $mailbox['delimiter']);

      $used = in_array($mailbox['name'], $subscribed_names);
      $attributes = $mailbox['attributes'];
      if ($this->get_folder($account_id,addslashes($mailbox['name'])))
      {
	if ($this->f('subscribed') != $used ||
	    $this->f('parent_id') != $parent_id ||
	    $this->f('attributes') != $attributes)
	{
	  $this->update_folder($this->f('id'),$parent_id, $used, $attributes);
	}
      }else
      {
	$this->add_folder($account_id, addslashes($mailbox['name']),$parent_id,
	    $used, $mailbox['delimiter'], $mailbox['attributes']);
      }
    }

    /*
       get all the Group-Office folders and delete the folders that no longer
       exist on the IMAP server
     */

    $this->get_all_folders($account_id);
    $GO_mailboxes = array();
    $emailobj = new email();
    while ($this->next_record())
    {
      if (!in_array($this->f('name'), $mailbox_names))
      {
	$emailobj->delete_folder($account_id, addslashes($this->f('name')));
      }
    }
  }

  function get_filters($account_id)
  {
    $sql = "SELECT * FROM emFilters WHERE account_id='$account_id' ".
      "ORDER BY priority DESC";
    $this->query($sql);
    return $this->num_rows();
  }

  function add_filter($account_id, $field, $keyword, $folder)
  {
    $next_id = $this->nextid("emFilters");
    if ($next_id > 0)
    {
      $sql = "INSERT INTO emFilters (id, account_id, field, keyword, ".
	"folder, priority) VALUES ('$next_id','$account_id','$field',".
	"'$keyword','$folder','$next_id')";

      return $this->query($sql);
    }else
    {
      return false;
    }
  }

  function update_filter($id, $field, $keyword, $folder)
  {
    $sql = "UPDATE emFilters SET field='$field', keyword='$keyword', ".
      "folder='$folder'";
    $this->query($sql);
  }

  function delete_filter($id)
  {
    $sql = "DELETE FROM emFilters WHERE id='$id'";
    $this->query($sql);
  }

  function move_up($move_up_id, $move_dn_id, $move_up_pr, $move_dn_pr)
  {
    if ($move_up_pr == $move_dn_pr)
      $move_up_pr++;

    $sql = "UPDATE emFilters SET priority='$move_up_pr' WHERE id='$move_up_id'";
    $this->query($sql);

    $sql = "UPDATE emFilters SET priority='$move_dn_pr' WHERE id='$move_dn_id'";
    $this->query($sql);
  }

  function delete_user($user_id)
  {
    $del = new email;
    $this->get_accounts($user_id);
    while ($this->next_record())
    {
      $del->delete_account($user_id,$this->f("id"));
    }
  }

  function register_attachment($tmp_file, $filename, $filesize, $filemime='',
      $disposition='attachment', $content_id='')
  {
    global $GO_CONFIG,$GO_CRYPTO;

    $filename = smart_addslashes($filename);

    $tmp_file = smart_addslashes($tmp_file);

    if (!isset($_SESSION['attach_array']))
    {
      $_SESSION['attach_array'] = array();
      $_SESSION['num_attach'] = 1;
    }else
    {
      $_SESSION['num_attach']++;
    }

    require_once($GO_CONFIG->class_path.'filetypes.class.inc');
    $filetypes = new filetypes();

    if ($filename != '')
    {
      $extension = get_extension($filename);

      if(!$type = $filetypes->get_type($extension))
      {
	$type = $filetypes->add_type($extension, $filemime);
      }
      $filemime = $type['mime'];
    }
    $_SESSION['attach_array'][$_SESSION['num_attach']]->file_name = $filename;
    $_SESSION['attach_array'][$_SESSION['num_attach']]->tmp_file =  $tmp_file;
    $_SESSION['attach_array'][$_SESSION['num_attach']]->file_size = $filesize;
    $_SESSION['attach_array'][$_SESSION['num_attach']]->file_mime = $filemime;
    $_SESSION['attach_array'][$_SESSION['num_attach']]->disposition = $disposition;
    $_SESSION['attach_array'][$_SESSION['num_attach']]->content_id = $content_id;
  }
}
?>
