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

define('NORMAL_DISPLAY', 0);
define('MULTIPAGE_DISPLAY', 1);
define('HOT_ITEM_DISPLAY', 2);


class cms extends db
{
  function cms()
  {
    $this->db();
  }
  
  function get_settings($user_id)
  {
    $this->query("SELECT * FROM cms_settings WHERE user_id='$user_id'");
    if ($this->next_record())
    {
      return $this->Record;
    }else
    {
      $this->query("INSERT INTO cms_settings ".
	  "(user_id, sort_field, sort_order) VALUES ".
	  "('$user_id', 'cms_files.priority', 'ASC')");
      return $this->get_settings($user_id);
    }		
  }

  function set_sorting($user_id, $sort_field, $sort_order)
  {
    $sql = "UPDATE cms_settings SET sort_field='$sort_field' , ".
      "sort_order='$sort_order' WHERE user_id='$user_id'";
    return $this->query($sql);
  }

  function resolve_url($path, $site_id)
  {
    $site = $this->get_site($site_id);

    $path = explode('/', $path);

    $last_folder_id = $site['root_folder_id'];
    $count = count($path);
    for($i=0;$i<$count;$i++)
    {
      if ($path[$i] != '')
      {
	if(!$new_folder_id = $this->folder_exists($last_folder_id, $path[$i]))
	{
	  $file_id = $this->file_exists($last_folder_id, $path[$i]);
	  if(!$file_id = $this->file_exists($last_folder_id, $path[$i]))
	  {
	    echo $i.': '.$path[$i].' last folder id: '.$last_folder_id.'<br />';
	    return false;
	  }
	}else
	{
	  $last_folder_id = $new_folder_id;
	}
      }
    }
    $path['folder_id'] = $last_folder_id;
    if (isset($file_id))
    {
      $path['file_id'] = $file_id;
    }
    return $path;

  }

  function move_file($file_id, $new_folder_id)
  {
    //if the name exists add (1) behind it.
    if($file = $this->get_file($file_id))
    {
      $name = $file['name'];
      $x=0;
      while ($this->file_exists($new_folder_id, $name))
      {
	$x++;
	$name = strip_extension($file['name']).' ('.$x.').'.get_extension($file['name']);
      }
      return $this->query("UPDATE cms_files SET folder_id='$new_folder_id', name='$name' WHERE id='$file_id'");
    }
    return false;
  }

  function move_folder($folder_id, $new_parent_id)
  {
    if ($folder = $this->get_folder($folder_id))
    {
      if ($folder['parent_id'] != $new_parent_id)
      {
	//don't move folders into thier own path
	if (!$this->is_in_path($folder_id, $new_parent_id))
	{
	  //if the name exists add (1) behind it.
	  $name = $folder['name'];
	  $x=0;
	  while ($this->folder_exists($new_parent_id, $name))
	  {
	    $x++;
	    $name = $folder['name'].' ('.$x.')';
		}
		return $this->query("UPDATE cms_folders SET parent_id='$new_parent_id', name='$name' WHERE id='$folder_id'");
		}
		}
		}
		return false;
		}

		function copy_file($file_id, $new_folder_id)
		{
		//if the name exists add (1) behind it.
		if($file = $this->get_file($file_id))
		{
		$name = $file['name'];
		$x=0;
		while ($this->file_exists($new_folder_id, $name))
		{
		$x++;
		$name = $file['name'].' ('.$x.')';
		  }
		  return $this->add_file($new_folder_id, $name, $file['content'], $file['title'], $file['description'], $file['keywords']);
		  }
		  return false;
		  }

		  function copy_folder($folder_id, $new_parent_id)
		  {
		  if ($folder = $this->get_folder($folder_id))
		  {
		  //don't move folders into thier own path
		  if (!$this->is_in_path($folder_id, $new_parent_id))
		  {
		  //if the name exists add (1) behind it.
		  $name = $folder['name'];
		  $x=0;
		  while ($this->folder_exists($new_parent_id, $name))
		  {
		  $x++;
		  $name = $folder['name'].' ('.$x.')';
		    }
		    if($new_folder_id = $this->add_folder($new_parent_id, $name))
		    {
		    $cms = new cms();
		    $this->get_files($folder_id);
		    while($this->next_record())
		    {
		    if(!$cms->copy_file($this->f('id'), $new_folder_id))
		    {
		    return false;
		    }
		    }

		    $this->get_folders($folder_id);
		    while($this->next_record())
		    {
		    if(!$this->copy_folder($this->f('id'),$new_folder_id))
		    {
		    return false;
		    }
		    }
		    return true;
		    }
		  }
		  }
		  return false;
		  }

		  /*
		     Check if a folder is in the path of another folder.
		     This is used to check if we can move a folder into another.
		   */
		  function is_in_path($check_folder_id, $target_folder_id)
		  {
		    if($target_folder_id == 0)
		    {
		      return false;
		    }elseif ($target_folder_id == $check_folder_id)
		    {
		      return true;
		    }else
		    {
		      $folder = $this->get_folder($target_folder_id);
		      return $this->is_in_path($check_folder_id, $folder['parent_id']);
		    }
		  }

		  function file_exists($folder_id, $filename)
		  {
		    $this->query("SELECT id FROM cms_files WHERE folder_id='$folder_id' AND name='$filename'");
		    if ($this->next_record())
		    {
		      return $this->f('id');
		    }
		    return false;
		  }

		  //builds the full path to this directory with links
		  function get_path($folder_id)
		  {
		    $path = '';

		    while($folder = $this->get_folder($folder_id))
		    {
		      $path = '/'.$folder['name'].$path;
		      $folder_id = $folder['parent_id'];
		    }
		    return $path;
		  }

		  //functions for personal sites
		  function add_site($user_id, $name, $domain, $acl_read, $acl_write, $description, $keywords, $template_id, $display_type=0)
		  {
			$domain = $this->prepare_domain($domain);  
		    //create a root folder for the new site
		    if ($root_folder_id = $this->add_folder(0, $domain))
		    {
		      //create the site
		      $site_id = $this->nextid('cms_sites');
		      if ($site_id > 0)
		      {
			if($this->query("INSERT INTO cms_sites (id, user_id, root_folder_id, name, domain, acl_read, acl_write, description, keywords, template_id, display_type) VALUES ('$site_id', '$user_id', '$root_folder_id','$name', '$domain', '$acl_read', '$acl_write', '$description', '$keywords', '$template_id', '$display_type')"))
			{
			  return $site_id;
			}
		      }else
		      {
			//on error clean up
			$this->delete_folder($folder_id);
		      }
		    }
		    return false;
		  }

		  function delete_site($site_id)
		  {
		    if($site = $this->get_site($site_id))
		    {
		      if ($this->delete_folder($site['root_folder_id']))
		      {
			if($this->query("DELETE FROM cms_sites WHERE id='$site_id'"))
			{
			  global $GO_SECURITY;
			  $GO_SECURITY->delete_acl($site['acl_read']);
			  $GO_SECURITY->delete_acl($site['acl_write']);
			  return true;
			}
		      }
		    }
		    return false;
		  }

		  function update_site($site_id, $name, $domain, $description, $keywords, $template_id, $display_type, $acl_read)
		  {
			$domain = $this->prepare_domain($domain);  
		    return $this->query("UPDATE cms_sites SET name='$name', domain='$domain', description='$description', keywords='$keywords', template_id='$template_id', display_type='$display_type', acl_read='$acl_read' WHERE id='$site_id'");
		  }

		  function set_publishing($site_id, $publish_style, $publish_path)
		  {
		    return $this->query("UPDATE cms_sites SET publish_style='$publish_style', publish_path='$publish_path' WHERE id='$site_id'");
		  }

		  function get_site_by_publish_path($path)
		  {
		    $this->query("SELECT * FROM cms_sites WHERE publish_path='$path'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }else
		    {
		      return false;
		    }
		  }

		  function get_site($site_id)
		  {
		    $this->query("SELECT * FROM cms_sites WHERE id='$site_id'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }
		
		  function prepare_domain($domain)
		  {
			$domain = preg_replace("/http(s?):\/\//i", '', $domain);
			if (substr($domain, -1)=='/')
			{
			  $domain = substr($domain,0, -1);
			}
			return $domain;
		  }

		  function get_site_by_domain($domain)
		  {
			$domain = $this->prepare_domain($domain);  
		    $this->query("SELECT * FROM cms_sites WHERE domain='$domain'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function get_sites()
		  {
		    $this->query("SELECT * FROM cms_sites");
		    return $this->num_rows();
		  }

		  function get_authorized_sites($user_id)
		  {
		    $sql = "SELECT DISTINCT cms_sites.* FROM cms_sites, acl, users_groups WHERE ".
		      "cms_sites.acl_write = acl.acl_id".
		      " AND (( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
		      "acl.group_id = 0 AND acl.user_id = ".$user_id." ) ) ORDER BY cms_sites.domain ASC";
		    $this->query($sql);
		    return $this->num_rows();
		  }

		  function get_subscribed_sites($user_id)
		  {
		    $this->query("SELECT cms_sites.* FROM cms_sites LEFT JOIN cms_subscribed ON (cms_subscribed.site_id=cms_sites.id) WHERE cms_subscribed.user_id='$user_id' ORDER BY cms_sites.domain ASC");
		    return $this->num_rows();
		  }


		  function subscribe_site($user_id, $site_id)
		  {
		    return $this->query("INSERT INTO cms_subscribed (user_id, site_id) VALUES ('$user_id', '$site_id')");
		  }

		  function unsubscribe_site($user_id, $site_id)
		  {
		    return $this->query("DELETE FROM cms_subscribed WHERE user_id='$user_id' AND site_id='$site_id'");
		  }

		  function is_subscribed($user_id, $site_id)
		  {
		    $this->query("SELECT * FROM cms_subscribed WHERE user_id='$user_id' AND site_id='$site_id'");
		    return $this->next_record();
		  }


		  //functions for files and folders of sites
		  function get_files($folder_id, $sort = "name", $direction = "ASC")
		  {
		    $sql = "SELECT cms_files.*, filetypes.friendly FROM cms_files INNER JOIN filetypes ON filetypes.extension=cms_files.extension WHERE folder_id='$folder_id' ORDER BY $sort $direction";
		    $this->query($sql);
		    return $this->num_rows();
		  }

		  function get_visible_folders($folder_id)
		  {
		    $this->query("SELECT * FROM cms_folders WHERE parent_id='$folder_id' AND disabled='0' ORDER BY priority ASC");
		    return $this->num_rows();
		  }

		  function get_folders($folder_id, $sort = "name", $direction = "ASC")
		  {
		    $this->query("SELECT * FROM cms_folders WHERE parent_id='$folder_id' ORDER BY '$sort' $direction");
		    return $this->num_rows();
		  }

		  function add_folder($parent_id, $name, $priority=0, $disabled=false)
		  {
			$disabled = $disabled ? '1' : '0';
		    $folder_id = $this->nextid('cms_folders');
		    if ($folder_id > 0)
		    {
		      $mtime = get_gmt_time();
		      if($this->query("INSERT INTO cms_folders (id, parent_id, name, mtime, priority, disabled) VALUES ('$folder_id', '$parent_id', '$name', '$mtime', '$priority', '$disabled')"))
		      {
			return $folder_id;
		      }
		    }
		    return false;
		  }

		  function get_folder($folder_id)
		  {
		    $this->query("SELECT * FROM cms_folders WHERE id='$folder_id';");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;

		  }

		  function update_folder($folder_id, $name, $disabled, $priority, $display_type=0)
		  {
		  	$sql = "UPDATE cms_folders SET name='$name', mtime='".get_gmt_time().
							"', disabled='$disabled', priority='$priority', ".
							"display_type='$display_type' WHERE id='$folder_id'";
		    return $this->query($sql);
		  }

		  function folder_exists($parent_id, $name)
		  {
		    $this->query("SELECT id FROM cms_folders WHERE parent_id='$parent_id' AND name='$name'");
		    if($this->next_record())
		    {
		      return $this->f('id');
		    }
		    return false;
		  }

		  function delete_folder($folder_id)
		  {
		    if ($folder_id > 0)
		    {
		      //add a second cms object for simultanious select and delete from the db
		      $cms2 = new cms();

		      //get all folders
		      $this->get_folders($folder_id);
		      while($this->next_record())
		      {
			if (!$cms2->delete_folder($this->f('id')))
			{
			  return false;
			}
		      }

		      $this->get_files($folder_id);
		      while ($this->next_record())
		      {
			if(!$cms2->delete_file($this->f('id')))
			{
			  return false;
			}
		      }

		      return $this->query("DELETE FROM cms_folders WHERE id='$folder_id'");
		    }else
		    {
		      return false;
		    }
		  }

		  function delete_file($file_id)
		  {
		    return $this->query("DELETE FROM cms_files WHERE id='$file_id'");
		  }

		  function get_file($file_id)
		  {
		    $this->query("SELECT cms_files.*, filetypes.mime AS content_type, filetypes.friendly AS content_type_friendly FROM cms_files LEFT JOIN filetypes ON (filetypes.extension=cms_files.extension) WHERE id='$file_id'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function add_file($folder_id, $name, $content, $title='', $description='', $keywords='', $priority='0')
		  {
		    $file_id = $this->nextid('cms_files');
		    if ($file_id > 0)
		    {
		      $size = strlen($content);
		      $extension = get_extension($name);
		      $mtime = get_gmt_time();

		      $sql  = "INSERT INTO cms_files (id, folder_id, name, extension, mtime, size, content, title, description, keywords, priority) ";
		      $sql .= "VALUES ('$file_id', '$folder_id', '$name', '$extension', '$mtime', '$size', '$content', '$title', '$description', '$keywords', '$priority')";
		      if ($this->query($sql))
		      {
			return $file_id;
		      }
		    }
		    return false;
		  }

		  function update_file($file_id, $name, $content, $title='', $description='', $keywords='', $priority='0')
		  {
		    $size = strlen($content);
		    $extension = get_extension($name);
		    $mtime = get_gmt_time();

		    $sql = "UPDATE cms_files SET name='$name', extension='$extension', ".
		      "mtime='$mtime', size='$size', content='$content', ".
		      "title='$title', description='$description', ".
		      "keywords='$keywords', priority='$priority' ".
		      "WHERE id='$file_id'";
		    return $this->query($sql);
		  }

		  //template functions

		  function add_template($user_id, $name, $style, $additional_style, $restrict_editor, $acl_read, $acl_write)
		  {
		    //create the template
		    $template_id = $this->nextid('cms_templates');
		    if ($template_id > 0)
		    {
		      $restrict_editor = ($restrict_editor == "true") ? '1' : '0';

		      if($this->query("INSERT INTO cms_templates (id, user_id, name, style, additional_style, restrict_editor, acl_read, acl_write) VALUES ('$template_id', '$user_id', '$name', '$style', '$additional_style', '$restrict_editor', '$acl_read', '$acl_write')"))
		      {
			return $template_id;
		      }
		    }

		    return false;
		  }

		  function update_template($template_id, $name, $style, $additional_style, $restrict_editor)
		  {
		    $restrict_editor = ($restrict_editor == "true") ? '1' : '0';
		    return $this->query("UPDATE cms_templates SET restrict_editor='$restrict_editor', name='$name', style='$style', additional_style='$additional_style' WHERE id='$template_id'");
		  }

		  function get_templates()
		  {
		    $this->query("SELECT * FROM cms_templates");
		    return $this->num_rows();
		  }

		  function get_authorized_templates($user_id)
		  {

		    $sql = "SELECT DISTINCT cms_templates.* FROM cms_templates, acl, users_groups WHERE ".
		      "(cms_templates.acl_write = acl.acl_id OR cms_templates.acl_read = acl.acl_id)".
		      " AND (( acl.group_id = users_groups.group_id AND users_groups.user_id = ".$user_id." AND acl.user_id = 0 ) OR (".
		      "acl.group_id = 0 AND acl.user_id = ".$user_id." ) )";
		    $this->query($sql);
		    return $this->num_rows();
		  }

		  function get_template($template_id)
		  {
		    $this->query("SELECT * FROM cms_templates WHERE id='$template_id'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function get_template_by_name($user_id, $name)
		  {
		    $this->query("SELECT * FROM cms_templates WHERE user_id='$user_id' AND name='$name'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function delete_template($template_id)
		  {
		    if($this->query("DELETE FROM cms_template_items WHERE template_id='$template_id'"))
		    {
		      return $this->query("DELETE FROM cms_templates WHERE id='$template_id'");
		    }
		    return false;

		  }

		  function add_template_item($template_id, $name, $content)
		  {
		    //create the template
		    $template_item_id = $this->nextid('cms_template_items');
		    if ($template_item_id > 0)
		    {
		      if($this->query("INSERT INTO cms_template_items (id, template_id, name, content) VALUES ('$template_item_id', '$template_id', '$name', '$content')"))
		      {
			return $template_item_id;
		      }
		    }

		    return false;
		  }

		  function update_template_item($template_item_id, $name, $content)
		  {
		    return $this->query("UPDATE cms_template_items SET name='$name',content='$content' WHERE id='$template_item_id'");
		  }

		  function get_template_items($template_id)
		  {
		    $this->query("SELECT * FROM cms_template_items WHERE template_id='$template_id'");
		    return $this->num_rows();
		  }

		  function get_template_item($template_item_id)
		  {
		    $this->query("SELECT * FROM cms_template_items WHERE id='$template_item_id'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function delete_template_item($template_item_id)
		  {
		    return $this->query("DELETE FROM cms_template_items WHERE id='$template_item_id'");
		  }
		  function get_template_item_by_name($template_id, $name)
		  {
		    $this->query("SELECT * FROM cms_template_items WHERE template_id='$template_id' AND name='$name'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }
		
		  function replace_template_items($template_id)
		  {
			$cms = new cms();  
			$this->get_template_items($template_id);
			while($this->next_record())
			{
			  $content = $this->replace_template_files($template_id, $this->f('content'));		 
			  $cms->update_template_item($this->f('id'), $this->f('name'), $content);			  
			}  
		  }
		
		  function replace_template_files($template_id, $content)
		  {
			global $GO_MODULES;		
			
			$cms = new cms();
			$cms_module = $GO_MODULES->get_module('cms');  
			
			$attributes[] = 'src';
			$attributes[] = 'href';
			$attributes[] = 'url(';
			
			$length = strlen($content);
			$replacements = array();
			
			while($attribute = array_shift($attributes))
			{
			  $offset = 0;
			  $url = '';			  
			  while($pos = strpos($content, $attribute, $offset))
			  {
				$in_value = false;
				for($offset=$pos;$offset<$length;$offset++)
				{
				  $char = $content[$offset];
				  switch ($char)
				  {
					case '"':
					  if($in_value && $escape_char == '"')
					  {
						if($template_file_id = $cms->template_file_exists($template_id, basename($url)))
						{
						  $replacement['old'] = $url;
						  $replacement['new'] = $cms_module['url'].'template_file.php?template_file_id='.$template_file_id;
						  $replacements[] = $replacement;
						}
						$url = '';
						break;
					  }elseif(!$in_value)
					  {
						$escape_char = '"';  
						$in_value = true;  
					  }						
					break;
					
					case "'":
						if($in_value && $escape_char == "'")
						{
						  if($template_file_id = $cms->template_file_exists($template_id, addslases(basename($url))))
						  {
							$replacement['old'] = $url;
							$replacement['new'] = $cms_module['url'].'template_file.php?template_id='.$template_file_id;
							$replacements[] = $replacement;
						  }
						  $url = '';
						  break;
						}elseif(!$in_value)
						{
						  $escape_char = "'";
						  $in_value = true;  
						}
					break;
					
					default:
					  if($in_value)
					  {
						$url .= $char;
					  }
					break;						
				  }
				}				  
			  }				
			}
			while($replacement = array_shift($replacements))
			{
			  //echo $replacement['old'].'- >'.$replacement['new'].'<br>';
			  $content = str_replace($replacement['old'], $replacement['new'], $content);					
			}
			return $content;
		  }
		  
		  function set_main_template_item($template_id, $template_item_id)
		  {
		    $this->query("UPDATE cms_template_items SET main='0' WHERE template_id='$template_id' AND main='1'");
		    $this->query("UPDATE cms_template_items SET main='1' WHERE template_id='$template_id' AND id='$template_item_id'");
		  }

		  function get_main_template_item($template_id)
		  {
		    $this->query("SELECT * FROM cms_template_items WHERE template_id='$template_id' AND main='1'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function get_template_files($template_id)
		  {
		    $this->query("SELECT cms_template_files.*, filetypes.mime AS content_type, filetypes.friendly AS content_type_friendly FROM cms_template_files LEFT JOIN filetypes ON (filetypes.extension=cms_template_files.extension) WHERE template_id='$template_id'");
		    return $this->num_rows();
		  }

		  function add_template_file($template_id, $name, $content)
		  {
		    $template_file_id = $this->nextid('cms_template_files');
		    if ($template_file_id > 0)
		    {
		      $size = strlen($content);
		      $extension = get_extension($name);
		      $mtime = get_gmt_time();
		      return $this->query("INSERT INTO cms_template_files (id, template_id, name, extension, size, mtime, content) VALUES ('$template_file_id','$template_id','$name','$extension','$size','$mtime','$content')");
		    }
		    return false;
		  }

		  function get_template_file($template_file_id)
		  {
		    $this->query("SELECT cms_template_files.*, filetypes.mime AS content_type, filetypes.friendly AS content_type_friendly FROM cms_template_files LEFT JOIN filetypes ON (filetypes.extension=cms_template_files.extension) WHERE id='$template_file_id'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function template_file_exists($template_id, $name)
		  {
		    $this->query("SELECT id FROM cms_template_files WHERE template_id='$template_id' AND name='$name'");
		    if ($this->next_record())
		    {
		      return $this->f('id');
		    }
		    return false;
		  }

		  function delete_template_file($template_file_id)
		  {
		    return $this->query("DELETE FROM cms_template_files WHERE id='$template_file_id'");
		  }

		  function add_search_word($site_id, $search_word)
		  {
		    //create the template
		    $search_word_id = $this->nextid('cms_search_words');
		    if ($search_word_id > 0)
		    {
		      if($this->query("INSERT INTO cms_search_words VALUES ('$search_word_id', '$site_id', '$search_word')"))
		      {
			return $search_word_id;
		      }
		    }
		    return false;
		  }

		  function delete_search_word($search_word_id)
		  {
		    if($this->query("DELETE FROM cms_search_files WHERE search_word_id='$search_word_id'"))
		    {
		      return $this->query("DELETE FROM cms_search_words WHERE id='$search_word_id'");
		    }
		  }

		  function update_search_word($search_word_id, $search_word)
		  {
		    return $this->query("UPDATE cms_search_words SET search_word='$search_word' WHERE id='$search_word_id'");
		  }

		  function get_search_word($search_word_id)
		  {
		    $this->query("SELECT * FROM cms_search_words WHERE id='$search_word_id'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }


		  function get_search_words($site_id)
		  {
		    $this->query("SELECT * FROM cms_search_words WHERE site_id='$site_id' ORDER BY search_word ASC");
		    return $this->num_rows();
		  }

		  function get_search_word_by_name($site_id, $search_word)
		  {
		    $this->query("SELECT * FROM cms_search_words WHERE site_id='$site_id' AND search_word='$search_word'");
		    if ($this->next_record())
		    {
		      return $this->Record;
		    }
		    return false;
		  }

		  function file_is_attached($file_id, $search_word_id)
		  {
		    $this->query("SELECT * FROM cms_search_files WHERE search_word_id='$search_word_id' AND file_id='$file_id'");
		    return $this->next_record();
		  }

		  function attach_file($file_id, $search_word_id)
		  {
		    return $this->query("INSERT INTO cms_search_files (search_word_id, file_id) VALUES ('$search_word_id', '$file_id')");
		  }

		  function detach_file($file_id, $search_word_id)
		  {
		    return $this->query("DELETE FROM cms_search_files WHERE file_id='$file_id' AND search_word_id='$search_word_id'");
		  }

		  function search_files($folder_id, $search_word_id, $attach_results=true)
		  {
		    $cms = new cms();
		    $search_word = $this->get_search_word($search_word_id);
		    $files = array();
		    $sql = "SELECT id FROM cms_files WHERE extension='html' AND folder_id='$folder_id' AND (UPPER(content) REGEXP '[[:<:]]".strtoupper($search_word['search_word'])."[[:>:]]' OR UPPER(content) REGEXP '[[:<:]]".htmlentities(strtoupper($search_word['search_word']))."[[:>:]]')";
		    $this->query($sql);
		    while ($this->next_record())
		    {
		      if ($attach_results && !$cms->file_is_attached($this->f('id'), $search_word_id))
		      {
			$cms->attach_file($this->f('id'), $search_word_id);
		      }
		      $files[] = $this->f('id');

		    }

		    $this->get_folders($folder_id);
		    while($this->next_record())
		    {
		      $files = array_merge($files, $cms->search_files($this->f('id'), $search_word_id, $attach_results));
		    }
		    return $files;
		  }

		  function get_attached_files($search_word_id)
		  {
		    $this->query("SELECT cms_files.* FROM cms_files LEFT JOIN cms_search_files ON (cms_search_files.file_id=cms_files.id) WHERE cms_search_files.search_word_id='$search_word_id'");
		    return $this->num_rows();
		  }

		  function get_attached_search_words($file_id)
		  {
		    $this->query("SELECT cms_search_words.* FROM cms_search_words LEFT JOIN cms_search_files ON (cms_search_files.search_word_id=cms_search_words.id) WHERE cms_search_files.file_id='$file_id'");
		    return $this->num_rows();
		  }

		  function get_body($html)
		  {
		    $to_removed_array = array (
			"'<html[^>]*>'si",
			"'</html>'si",
			"'<body[^>]*>'si",
			"'</body>'si",
			"'<head[^>]*>.*?</head>'si",
			"'<style[^>]*>.*?</style>'si",
			"'<object[^>]*>.*?</object>'si",
			);
		    $html = preg_replace($to_removed_array, '', $html);
		    //$html = preg_replace("/class\s*=\s*[\"']?.*?[\"']?\s/is", '', $html);
		    //$html = preg_replace("/style\s*=\s*[\"'].*?[\"']\s/is", '', $html);
		    //werkt min of meer$html = preg_replace("/class.*?\s/is", '', $html);
		    //$html = preg_replace("/style.*?\s/is", '', $html);


		    return $html;

		  }

		  function remove_styles($html)
		  {
		    $to_removed_array = array (
			"'class=[\w]*\b'si",
			"'class=\"[^\"]*\"'si",
			"'class=\'[^\']*\''si",
			"'style=\"[^\"]*\"'si",
			"'style=\'[^\']*\''si",
			);
		    //$html = preg_replace("/class\s*=\s*.*?\s/is", '', $html);
		    //$html = preg_replace("/class\s*=\s*.*?\s/is", '', $html);
		    $html = preg_replace($to_removed_array, '', $html);
		    return $html;
		  }


		  function extract_styles($style)
		  {
		    $styles = array();
		    $style_name = '';
		    $in_style = true;

		    $style = str_replace("\r", '', $style);
		    $style = str_replace("\n", '', $style);
		    $style = str_replace(' ', '', $style);

		    for ($i=0; $i<strlen($style); $i++)
		    {
		      $char = $style[$i];

		      if ($char == '{' || $char == ',')
		      {
			$in_style = false;
			if (trim($style_name) != '')
			{
			  $styles[] = $style_name;
			}
			$style_name = '';
		      }elseif($char == '.')
		      {
			if ($style_name != '')
			{
			  $style_name = '';
			  $in_style = false;
			}else
			{
			  $in_style = true;
			}
		      }elseif($char == ':')
		      {
			$style_name = '';
			$in_style = false;
		      }elseif($char == '}')
		      {
			$in_style = true;
			$style_name = '';
		      }else
		      {
			if ($in_style)
			{
			  $style_name .= $char;
			}
		      }
		    }

		    return $styles;
		  }

		  function clean_up_html($html)
		  {
		    // remove escape slashes
		    $html = stripslashes($html);

		    // strip tags, still leaving attributes, second variable is allowable tags
		    $html = strip_tags($html, '<p><b><br><img><i><u><a><h1><h2><h3><h4><h4><h5><h6><table><tr><td>');

		    /*
		    // removes the attributes for allowed tags, use separate replace for heading tags since a
		    // heading tag is two characters
		    $html = ereg_replace("<([p|b|i|u])[^>]*>", "<\\1>", $html);
		    $html = ereg_replace("<([h1|h2|h3|h4|h5|h6][1-6])[^>]*>", "<\\1>", $html);
		     */
		    // remove all class and style attributes from the remaining tags
		    $to_removed_array = array (
			"'class=[\w]*\b'si",
			"'class=\"[^\"]*\"'si",
			"'class=\'[^\']*\''si",
			"'style=\"[^\"]*\"'si",
			"'style=\'[^\']*\''si",
			);
		    //$html = preg_replace("/class\s*=\s*.*?\s/is", '', $html);
		    //$html = preg_replace("/class\s*=\s*.*?\s/is", '', $html);
		    $html = preg_replace($to_removed_array, '', $html);
		    return $html;
		  }

		  function delete_user($user_id)
		  {
		    $cms = new cms();
		    $sql = "SELECT id FROM cms_sites WHERE user_id='$user_id'";
		    $this->query($sql);
		    while($this->next_record())
		    {
		      $cms->delete_site($this->f('id'));
		    }

		    $sql = "SELECT id FROM cms_templates WHERE user_id='$user_id'";
		    $this->query($sql);
		    while($this->next_record())
		    {
		      $cms->delete_template($this->f('id'));
		    }

		  }
}
?>
