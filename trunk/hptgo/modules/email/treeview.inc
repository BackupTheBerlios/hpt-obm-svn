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

$tv_image['mlastnode'] = '<img src="'.$GO_THEME->images['mlastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['emptylastnode'] = '<img src="'.$GO_THEME->images['emptylastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['plastnode'] = '<img src="'.$GO_THEME->images['plastnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['mnode'] = '<img src="'.$GO_THEME->images['mnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['emptynode'] = '<img src="'.$GO_THEME->images['emptynode'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['pnode'] = '<img src="'.$GO_THEME->images['pnode'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['vertline'] = '<img src="'.$GO_THEME->images['vertline'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['blank'] = '<img src="'.$GO_THEME->images['blank'].'" border="0" height="22" width="16" align="absmiddle" />';
$tv_image['group'] = '<img src="'.$GO_THEME->images['workgroup'].'" border="0" height="22" width="24" align="absmiddle" />';
$tv_image['user'] = '<img src="'.$GO_THEME->images['workstation'].'" border="0" height="22" width="20" align="absmiddle" />';

$tv_image['opened_folder'] = '<img src="'.$GO_THEME->images['folderopen'].'" border="0" height="22" width="24" align="absmiddle" />';
$tv_image['closed_folder'] = '<img src="'.$GO_THEME->images['folderclosed'].'" border="0" height="22" width="24" align="absmiddle" />';

/*
   prints the folders in a tree
   $folders is an array of associve arrays containing an 'id' and 'name'
 */
function print_tree($folders, $prefix='')
{
  global $tv_image, $folder_id, $email2, $mailbox, $mail, $total_unseen, $account_id;

  $count = count($folders);
  for ($i=0;$i<$count;$i++)
  {
    //for each folder check if there are subfolders and
    //add them to an array for the next instance of this recursive function
    $subfolders = array();
    $subfolders_count = $email2->get_folders($folders[$i]['account_id'], $folders[$i]['id']);
    while($email2->next_record())
    {
      $subfolders[] = $email2->Record;
    }
    //is this folder opened in the tree?

    $open = in_array($folders[$i]['id'], $_SESSION['expanded']);

    if (isset($_POST['form_action']) && $_POST['form_action'] == 'expand_all')
    {
      if (!$open)
      {
	$_SESSION['expanded'][] = $folders[$i]['id'];
	$open = true;
      }
    }

    //determine the image and node to display
    if ($subfolders_count > 0)
    {
      if ($i < ($count-1))
      {
	$new_image = $tv_image['vertline'];
	$node = $open ? $tv_image['mnode'] : $tv_image['pnode'];

      }else
      {
	$new_image = $tv_image['blank'];
	$node = $open ? $tv_image['mlastnode'] : $tv_image['plastnode'];
      }
    }else
    {
      if ($i < ($count-1))
      {
	$new_image = $tv_image['vertline'];
	$node = $tv_image['emptynode'];
      }else
      {
	$new_image = $tv_image['blank'];
	$node = $tv_image['emptylastnode'];
      }
    }

    //if the current folder is the current opened folder then show an opened folder
    if ($mailbox == $folders[$i]['name'])
    {
      $folder_image = $tv_image['opened_folder'];
    }else
    {
      $folder_image = $tv_image['closed_folder'];
    }

    //actually print the current folder
    //if the delimiter is found in the path then this is a subfolder cut the
    //location to print it user friendly
    if ($pos = strrpos($folders[$i]['name'], $folders[$i]['delimiter']))
    {
      $folder_name = substr($folders[$i]['name'],$pos+1);
    }else
    {
      $folder_name = $folders[$i]['name'];
    }
    $short_name = cut_string($folder_name, 30);

    //check for unread mail
    $status = $mail->status($folders[$i]['name']);
    if ($status->unseen > 0)
    {
      $status = '<span class="count">&nbsp;('.$status->unseen.')</span>';
    }else
    {
      $status = '';
    }

    echo '<a href="'.$_SERVER['PHP_SELF'].'?account_id='.$account_id.'&expand_id='.$folders[$i]['id'].'&mailbox='.urlencode($mailbox).'">'.$prefix.$node.'</a>';
    echo '<a href="'.$_SERVER['PHP_SELF'].'?account_id='.$account_id.'&folder_id='.$folders[$i]['id'].'&mailbox='.urlencode($folders[$i]['name']).'" title="'.$folders[$i]['name'].'">'.$folder_image.$short_name.'</a>'.$status.'<br />';

    if ($open)
    {
      print_tree($subfolders, $prefix.$new_image);
    }
  }
}

function set_expanded($expanded, $expand_id, $folder_id)
{
  //fill the first value of the expanded with -1 because of key 0
  //results into false at the array_search() function and behaves differently at different PHP versions
  if (empty($expanded))
  {
    $expanded[]=-1;
  }

  //expand or collaps the expand id which is passed when a user clicks a node
  if ($expand_id > 0)
  {
    $expand_path = smartstrip($expand_id);
    $key = array_search($expand_path, $expanded);
    if (!$key)
    {
      $expanded[] = $expand_path;
    }else
    {
      unset($expanded[$key]);
    }
  }elseif ($folder_id > 0)
  {
    //always expand folder clicks
    $key = array_search($folder_id, $expanded);
    if (!$key)
    {
      $expanded[] = $folder_id;
    }
  }
  return $expanded;
}

$folder_id = isset($_REQUEST['folder_id']) ? $_REQUEST['folder_id'] : 0;
$expand_id = isset($_REQUEST['expand_id']) ? $_REQUEST['expand_id'] : 0;

if (isset($_POST['form_action']) && $_POST['form_action'] == 'collapse_all')
{
  unset($_SESSION['expanded']);
}

//$account_expanded = isset($account_expanded) ? $account_expanded : array();
$_SESSION['expanded'] = isset($_SESSION['expanded']) ? $_SESSION['expanded'] : array($account['id']);
$_SESSION['expanded'] = set_expanded($_SESSION['expanded'], $expand_id, $folder_id);


$email2 = new email();
$count = $email->get_accounts($GO_SECURITY->user_id);
if ($count > 0)
{
  //print an image that represents the email accounts

  echo '<img src="'.$GO_THEME->images['newmail'].'" border="0" height="16" width="16" align="absmiddle" /><br />';

  //loop thorugh accounts
  $i=0;
  while($email->next_record())
  {
    //if it is the last account then print mlastnode
    $i++;

    $folder_count =0;
    if ($email->f('type') == 'imap')
    {
      $folder_count = $email2->get_folders($email->f('id'), 0);
    }
    if ($i == $count)
    {

      if ($email->f('id') == $account['id'])
      {
	if ($folder_count > 0)
	{
	  $node = $tv_image['mlastnode'];
	}else
	{
	  $node = $tv_image['emptylastnode'];
	}
      }else
      {
	if ($folder_count > 0)
	{
	  $node = $tv_image['plastnode'];
	}else
	{
	  $node = $tv_image['emptylastnode'];
	}
      }

      $prefix = $tv_image['blank'];
    }else
    {
      if ($email->f('id') == $account['id'])
      {
	if ($folder_count > 0)
	{
	  $node = $tv_image['mnode'];
	}else
	{
	  $node = $tv_image['emptynode'];
	}
      }else
      {
	if ($folder_count > 0)
	{
	  $node = $tv_image['pnode'];
	}else
	{
	  $node = $tv_image['emptynode'];
	}
      }

      $prefix = $tv_image['vertline'];
    }

    $short_name = cut_string($email->f('email'), 30);

    echo '<a href="'.$_SERVER['PHP_SELF'].'?account_id='.$email->f('id').'">'.$node.'</a>';
    echo '<a href="'.$_SERVER['PHP_SELF'].'?account_id='.$email->f('id').'" title="'.$email->f('email').'"><img src="'.$GO_THEME->images['inbox'].'" border="0" align="absmiddle" />&nbsp;'.$short_name.'</a>';

    if ($email->f('type') == 'imap' && $email->f('id') == $account['id'])
    {
      echo '&nbsp;<a align="absmiddle" href="javascript:expand_all()" title="'.$ml_expand_all.'"><img src="'.$GO_THEME->images['expand_all'].'" border="0" /></a>';
      echo '&nbsp;<a align="absmiddle" href="javascript:collapse_all()" title="'.$ml_collapse_all.'"><img src="'.$GO_THEME->images['collapse_all'].'" border="0" /></a>';
    }

    echo '<br />';

    if ($email->f('type') == 'imap')
    {
      //if this is the active account then print the tree

      $folder_count = $email2->get_folders($account['id'], 0);

      if ($email->f('id') == $account['id'])
      {
	if (eregi('INBOX', $account['mbroot']))
	{
	  if ($folder_count > 0)
	  {
	    $node = $tv_image['emptynode'];
	  }else
	  {
	    $node = $tv_image['emptylastnode'];
	  }
	  if ($mailbox == 'INBOX')
	  {
	    $folder_image = $tv_image['opened_folder'];
	  }else
	  {
	    $folder_image = $tv_image['closed_folder'];
	  }

	  //check for unread mail
	  $status = $mail->status('INBOX');
	  if ($status->unseen > 0)
	  {
	    $status = '<span class="count">&nbsp;('.$status->unseen.')</span>';
	  }else
	  {
	    $status = '';
	  }
	  echo $prefix.$node.'<a href="'.$_SERVER['PHP_SELF'].'?account_id='.$account_id.'&mailbox=INBOX" title="'.$ml_inbox.'">'.$folder_image.$ml_inbox.'</a>'.$status.'<br />';
	}

	$folders = array();
	while($email2->next_record())
	{
	  $folders[] = $email2->Record;
	}

	print_tree($folders, $prefix);
      }
    }
  }
}
?>
