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

class cms_site extends cms
{
  var $site;
  var $search_words;
  var $publish_url;
  var $publish_path;
  var $publish_static = false;
  var $template_file_replacements = array();

  function cms_site($site_id, $domain='')
  {
    global $GO_CONFIG;
    $this->db();

    if ($site_id == 0 || !$this->site = $this->get_site($site_id))
    {
      //if no site_id was passed then we are not in Group-Office but somebody is
			// requesting the site through a domain. Try to get the site by domain.  
      if(!$this->site = $this->get_site_by_domain($domain))
      {
	return false;
      }
    }

    $publish_path = $this->site['publish_path'];
    $root_publish_path = $GO_CONFIG->get_setting('cms_publish_path');
    $this->publish_path = $root_publish_path.$publish_path;
    $this->publish_url = $GO_CONFIG->get_setting('cms_publish_url').$publish_path;
  }

  function publish()
  {
    global $GO_CONFIG;

    require_once($GO_CONFIG->class_path.'filesystem.class.inc');
    $fs = new filesystem(true);

    if (!file_exists($this->publish_path))
    {
      return false;
    }

    $fs->delete($this->publish_path);
    mkdir($this->publish_path);

    if ($this->site['publish_style'] == '2')
    {
      $this->publish_static = true;

      mkdir($this->publish_path.'_site_data');
      mkdir($this->publish_path.'_search');

      $file = '<?php $site_id='.$this->site["id"].';
      require("'.$GO_CONFIG->root_path.'Group-Office.php");
      if ($cms_module = $GO_MODULES->get_module("cms"))
      {
	$published = true;
	require($GO_CONFIG->root_path.$cms_module["path"]."view.inc");
      }else
      {
	die("Failed to get Content Management Module");
      }
      ?>';

      if (!$fp = fopen($this->publish_path.'_search/index.php', 'w+'))
      {
	return false;
      }elseif(!fwrite($fp, $file))
      {
	fclose($fp);
	return false;
      }else
      {
	fclose($fp);
      }

      $this->create_stylesheet($this->publish_path.'_site_data'.$GO_CONFIG->slash.'style.css');
      $this->create_template_files();

      return $this->create_files($this->site['root_folder_id'], $this->publish_path);

    }else
    {
      $file = '<?php $site_id='.$this->site["id"].';
      require("'.$GO_CONFIG->root_path.'Group-Office.php");
      if ($cms_module = $GO_MODULES->get_module("cms"))
      {
	$published = true;
	require($GO_CONFIG->root_path.$cms_module["path"]."view.inc");
      }else
      {
	die("Failed to get Content Management Module");
      }
      ?>';

      if (!$fp = fopen($this->publish_path.'index.php', 'w+'))
      {
	return false;
      }elseif(!fwrite($fp, $file))
      {
	fclose($fp);
	return false;
      }else
      {
	return fclose($fp);
      }
    }
  }

  function create_stylesheet($path)
  {
    global $GO_CONFIG;

    $file = '';
    if ($template = $this->get_template($this->site['template_id']))
    {
      $file = $template['style'].$template['additional_style'];
    }

    if (!$fp = fopen($path, 'w+'))
    {
      return false;
    }elseif(!fwrite($fp, $file))
    {
      fclose($fp);
      return false;
    }else
    {
      fclose($fp);
    }
    return true;
  }

  function create_template_files()
  {
    global $GO_CONFIG, $GO_MODULES;

    $cms_module = $GO_MODULES->get_module('cms');

    $this->get_template_files($this->site['template_id']);

    while ($this->next_record())
    {
      if (!$fp = fopen($this->publish_path.'_site_data'.$GO_CONFIG->slash.$this->f('name'), 'w+'))
      {
	return false;
      }elseif(!fwrite($fp, $this->f('content')))
      {
	fclose($fp);
	return false;
      }else
      {
	fclose($fp);
      }

      $tpl_file['dynamic'] = $cms_module['url'].'template_file.php?template_file_id='.$this->f('id');
      $tpl_file['static'] = $this->publish_url.'_site_data/'.$this->f('name');
      $this->template_file_replacements[] = $tpl_file;
    }
  }

  function create_files($folder_id, $path)
  {
    global $GO_CONFIG, $GO_MODULES;

    $cms_module = $GO_MODULES->get_module("cms");

    $cms_site = new cms_site($this->site['id']);

    $cms_site->get_folders($folder_id);

    while($cms_site->next_record())
    {
      $new_path = $path.urlencode($cms_site->f('name')).$GO_CONFIG->slash;;
      if (!file_exists($new_path))
      {
	mkdir($new_path);
      }

      if (!is_writable($new_path) || !$this->create_files($cms_site->f('id'), $new_path))
      {
	return false;
      }
    }

    $index_created = false;
    $cms_site->get_files($folder_id, 'priority', 'ASC');
    while ($cms_site->next_record())
    {
      if ($folder_id == $this->site['root_folder_id'] && !$index_created)
      {
	$name = 'index.html';
	$index_created = true;
      }else
      {
	$name = $cms_site->f('name');
      }

      $full_path = $path.urlencode($name);
      $file = $this->generate_page($cms_site->f('id'));
      if (!$fp = fopen($full_path, 'w+'))
      {
	return false;
      }elseif(!fwrite($fp, $file))
      {
	fclose($fp);
	return false;
      }else
      {
	fclose($fp);
      }

      $tpl_file['dynamic'] = $cms_module['full_url'].'view.php?site_id='.$this->site['id'].'&file_id='.$cms_site->f('id');
      $tpl_file['static'] = str_replace($this->publish_path, $this->publish_url, $full_path);
      $this->template_file_replacements[] = $tpl_file;
    }
    return true;
  }

  function generate_header($title='', $keywords='', $description='')
  {
    global $GO_CONFIG, $GO_MODULES;

    $cms_module = isset($cms_module) ? $cms_module : $GO_MODULES->get_module('cms');

    $header = '<html><head>'.
      '<meta name="keywords" content="'.$keywords.'" />'.
      '<meta name="description" content="'.$description.'" />'.
      '<meta name="Generator" content="Group-Office '.$GO_CONFIG->version.'" />'.
      '<title>'.$title.'</title>';

    if ($this->publish_static)
    {
      $header .= '<link href="'.$this->publish_url.'_site_data/style.css" rel="stylesheet" type="text/css" />';
    }else
    {
      $header .= '<link href="'.$cms_module['url'].'stylesheet.php?template_id='.$this->site['template_id'].'" rel="stylesheet" type="text/css" />';
    }
    $header .= '<script type="text/javascript" src="'.$GO_CONFIG->host.'javascript/common.js"></script></head><body>';
    return $header;
  }

  function generate_footer()
  {
    return '</body></html>';
  }

  function generate_page($_file_id=0, $task='', $search_word_id='')
  {
    global $GO_CONFIG, $GO_MODULES, $GO_LANGUAGE;

 $cms_module = isset($cms_module) ? $cms_module : $GO_MODULES->get_module('cms');

    //set the folder id we are in
    $file_id = $_file_id;
    $folder_id = isset($_REQUEST['folder_id']) ? $_REQUEST['folder_id'] : $this->site['root_folder_id'];

    require($GO_LANGUAGE->get_language_file('cms'));
    
    if ($file_id == 0)
		{
			if($this->site['display_type'] == MULTIPAGE_DISPLAY)
			{
				$task = 'multipage_display';
			}else
			{
			  $this->get_files($folder_id, 'priority', 'ASC');
			  while($this->next_record())
			  {
			    if (eregi('htm',$this->f('extension')))
			    {
			      $file_id = $this->f('id');
			      break;
			    }
			  }
			 }
		}

    if ($file_id > 0)
    {
      $file = $this->get_file($file_id);
      if (!eregi('htm', $file['extension']))
      {
				return $file['content'];
      }

      if ($file['description'] == '')
      {
				$file['description'] = $this->site['description'];
      }

      if ($file['keywords'] == '')
      {
				$file['keywords'] = $this->site['keywords'];
      }

      if ($file['title'] == '')
      {
				$file['title'] = $this->site['name'];
      }
      $folder_id = $file['folder_id'];

      $this->get_attached_search_words($file_id);
      while($this->next_record())
      {
				$file['keywords'] = $this->f('search_word').', '.$file['keywords'];
      }
    }else
    {
    	
      $file['title'] = $this->site['name'];
      $file['keywords'] = $this->site['keywords'];
      $file['description'] = $this->site['description'];
    }

    $main_template_item = $this->get_main_template_item($this->site['template_id']);

    //get the search words so we can highlight them
    $this->search_words=array();
    $this->get_search_words($this->site['id']);
    while ($this->next_record())
    {
      $search_word1 = $this->f('search_word');
      $search_word2 = htmlentities($this->f('search_word'));
      $this->search_words[] = $search_word1;
      if ($search_word1 != $search_word2)
      {
				$this->search_words[] = $search_word2;
      }
    }

    $browser = detect_browser();


    //start output
    $page = $this->generate_header($file['title'], $file['keywords'], $file['description']);

    switch($task)
    {
      case 'search':
				if (isset($main_template_item))
				{
				  $file_id=0;
				  $folder_id=0;
				  $main_template_content =  $this->parse_template($main_template_item['content']);
				  $page .= str_replace($this->get_tag('content', $main_template_content), $this->print_search_words(), $main_template_content);
				}else
				{
				  $page .= print_search_words();
				}
				break;

      case 'search_results':
				$file_id=0;
				$folder_id=0;
				$count = $this->get_attached_files($search_word_id);
				$result = '<b>';
				if ($count == 1)
				{
				  $result .= $cms_found_one;
				}else
				{
				  $result .= $count.' '.$cms_found;
				}
				$result .= '</b><br /><br />';
			
				while($this->next_record())
				{
				  $title = ($this->f('title') != '') ? $this->f('title') : strip_extension($this->f('name'));
				  $result .= '<table border="0"><tr><td>';
			
				  if (!$this->publish_static)
				  {
				    $this->publish_static = ($this->site['publish_style'] == '2') ? true : false;
				    $result .= '<a href="'.$this->create_url($this->f('folder_id'),$this->f('id')).'">'.$title.'</a>';
				    $this->publish_static = false;
				  }else
				  {
				    $result .= '<a href="'.$this->create_url($this->f('folder_id'),$this->f('id')).'">'.$title.'</a>';
				  }
				  $result .= '</td></tr></table>';
				}
				if (isset($main_template_item))
				{
				  $main_template_content =  $this->parse_template($main_template_item['content']);
				  $page .= str_replace($this->get_tag('content', $main_template_content), $result, $main_template_content);
				}else
				{
				  $page .= $this->print_search_words();
				}
			break;
	
			case 'multipage_display':
			$content = $this->build_multipage($folder_id, 300);
				 require_once($GO_CONFIG->class_path.'highlighting_skipphtml.class.inc');
				 $hl = new highlighting_skipphtml();

			  $content = $hl->dohighlight($this->search_words,$content,'<span class="search">','</span>',1);
		
			  if (!$this->publish_static)
			  {
			    $content = str_replace($cms_module['full_url'].'view.php', $_SERVER['PHP_SELF'], $content);
			  }
			  if (isset($main_template_item))
			  {
			    if ($main_template_item)
			    {
			      $main_template_content =  $this->parse_template($main_template_item['content']);
			      $page .= str_replace($this->get_tag('content', $main_template_content), $content, $main_template_content);
			    }else
			    {
			      $page .= $content;
			    }
			  }else
			  {
			    $page .= $content;
			  }			  
			break;

      default:

	if ($file_id > 0)
	{
	  $content = $this->parse_template($file['content']);
	  require_once($GO_CONFIG->class_path.'highlighting_skipphtml.class.inc');

	  $hl = new highlighting_skipphtml();

	  $content = $hl->dohighlight($this->search_words,$content,'<span class="search">','</span>',1);

	  if (!$this->publish_static)
	  {
	    $content = str_replace($cms_module['full_url'].'view.php', $_SERVER['PHP_SELF'], $content);
	  }
	  if (isset($main_template_item))
	  {
	    if ($main_template_item)
	    {
	      $main_template_content =  $this->parse_template($main_template_item['content']);
	      $page .= str_replace($this->get_tag('content', $main_template_content), $content, $main_template_content);
	    }else
	    {
	      $page .= $content;
	    }
	  }else
	  {
	    $page .= $content;
	  }
	}else
	{
	  //get the language file
	  require($GO_LANGUAGE->get_language_file('cms'));
	  if (isset($main_template_item))
	  {
	    $main_template_content =  $this->parse_template($main_template_item['content']);
	    $page .= str_replace($this->get_tag('content', $main_template_content), $cms_empty, $main_template_content);
	  }else
	  {
	    $page .= $cms_empty;
	  }
	}
	break;
    }

    $page .= $this->generate_footer();

    if ($this->publish_static)
    {
      for ($i=0; $i < count($this->template_file_replacements);$i++)
      {
	$page = str_replace($this->template_file_replacements[$i]['dynamic'], $this->template_file_replacements[$i]['static'], $page);
      }
    }

    return $page;
  }
  
  function build_multipage($folder_id, $max_length)
  {
  	global $cms_read_more;
  	$multipage='';
  	if($folder = $this->get_folder($folder_id))
  	{  	
  		if($folder['display_type'] == MULTIPAGE_DISPLAY)
			{
				$this->get_files($folder_id);
				if($this->next_record())
				{
					$content = cut_string($this->f('content'), $max_length);
					$multipage .= '<p>'.cut_string($content, $max_length);
					
					if(strlen($this->f('content')) > strlen($content))
					{
						$multipage .= '<a href="'.$this->create_url($folder['id'], $this->f('id')).
												'">'.$cms_read_more.'</a>';
					}
					$multipage .= '</p>';
				}  				 				
			}
			return $this->build_multipage_recursive($folder_id, $max_length, $multipage);
  	}else
		{
			return false; 
		}
  }
  
  function build_multipage_recursive($folder_id, $max_length, $multipage='')
  {
  	global $cms_read_more;
  	$cms_site = new cms_site($this->site['id']);
  	
  	if($this->get_folders($folder_id))
  	{
  	
  		while($this->next_record())
  		{
  			if($this->f('display_type') == MULTIPAGE_DISPLAY)
  			{
  				$cms_site->get_files($this->f('id'));
  				if($cms_site->next_record())
  				{
  					$content = cut_string($cms_site->f('content'), $max_length);
  					$multipage .= '<p>'.cut_string($content, $max_length);
  					
  					if(strlen($cms_site->f('content')) > strlen($content))
  					{
  						$multipage .= '<a href="'.$this->create_url($this->f('id'), $cms_site->f('id')).
  												'">'.$cms_read_more.'</a>';
  					}
  					$multipage .= '</p>';
  				}  				 				
  			}
  			$multipage .= $cms_site->build_multipage_recursive($this->f('id'), $max_length); 
  		}
  	}
 		return $multipage;
  }
  
  
  function print_hot_items($folder_id, $template_name, $class)
  {
  	$multipage='';
  	
  	$template = $this->get_template_item_by_name($this->site['template_id'], $template_name);
  	
  	if($folder = $this->get_folder($folder_id))
  	{  	
  		if($folder['display_type'] == HOT_ITEM_DISPLAY)
			{
				$this->get_files($folder_id);
				if($this->next_record())
				{				
					$title = $this->f('title') != '' ? $this->f('title') : strip_extension($this->f('name'));
				
					
					$link = '<a class="'.$class.'" href="'.
												$this->create_url($folder['id'], $this->f('id')).
												'">'.$title.'</a>';
					if($template)
					{
						$hot_item = $this->parse_template($template['content']);
						$multipage = str_replace($this->get_tag('content', $hot_item), $link, $hot_item);
					}else
					{
						$multipage = $link;
					}					
				}  				 				
			}
			return $this->print_hot_items_recursive($folder_id, $template, $class, $multipage);
  	}else
		{
			return false; 
		}
  }
  
  function print_hot_items_recursive($folder_id, $template, $class, $multipage='')
  {
  	$cms_site = new cms_site($this->site['id']);
  	
  	if($this->get_folders($folder_id))
  	{  	
  		while($this->next_record())
  		{
  			if($this->f('display_type') == HOT_ITEM_DISPLAY)
  			{
  				$cms_site->get_files($this->f('id'));
  				if($cms_site->next_record())
  				{
  					$title = $cms_site->f('title') != '' ? $cms_site->f('title') : strip_extension($cms_site->f('name'));					
						
						$link = '<a class="'.$class.'" href="'.
													$this->create_url($this->f('id'), $cms_site->f('id')).
													'">'.$title.'</a>';
						if($template)
						{
							$hot_item = $cms_site->parse_template($template['content']);
							$multipage .= str_replace($this->get_tag('content', $hot_item), $link, $hot_item);
						}else
						{
							$multipage .= $link;							
						}						
  				}  				 				
  			}
  			$multipage .= $cms_site->print_hot_items_recursive($this->f('id'), $template, $class); 
  		}
  	}
 		return $multipage;
  }

  function create_url($folder_id,$file_id=0)
  {
    $cms_site = new cms_site($this->site['id']);

    if ($this->publish_static)
    {
      $url = '';
      if ($file_id == 0 && $folder_id > 0)
      {
	$cms_site->get_files($folder_id, 'priority', 'ASC');
	while($cms_site->next_record())
	{
	  if (eregi('htm',$cms_site->f('extension')))
	  {
	    $file_id = $cms_site->f('id');
	    break;
	  }
	}
      }

      if ($file_id > 0)
      {
	$file = $cms_site->get_file($file_id);
	$folder_id = $file['folder_id'];
	if ($folder_id != $this->site['root_folder_id'])
	{
	  $url = urlencode(urlencode($file['name']));
	}else
	{
	  $url = 'index.html';
	}
      }

      if ($folder_id > 0 && $folder_id != $this->site['root_folder_id'])
      {
	while($folder_id != $this->site['root_folder_id'])
	{
	  $folder = $cms_site->get_folder($folder_id);
	  $url = urlencode(urlencode($folder['name'])).'/'.$url;
	  $folder_id = $folder['parent_id'];
	}
      }
      return $this->publish_url.$url;
    }else
    {
      return $_SERVER['PHP_SELF'].'?folder_id='.$folder_id.'&file_id='.$file_id;
    }
  }

  function print_icon()
  {
    global $extension, $GO_CONFIG;
    return  '<img border="0" align="absmiddle" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$extension.'" />';
  }

  function print_home($text, $template_name, $active_template_name, $class, $active_class)
  {
    global $folder_id;
    $home = '';

    if ($folder_id == $this->site['root_folder_id'])
    {
      $template = $this->get_template_item_by_name($this->site['template_id'], $active_template_name);
      $class=$active_class;
    }else
    {
      $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);
    }
    if ($template)
    {
      $home = $this->parse_template($template['content']);
    }

    return str_replace($this->get_tag('content', $home), '<a class="'.$class.'" href="'.$this->create_url($this->site['root_folder_id']).'">'.$text.'</a>', $home);
  }

  function print_path($template_name, $item_template_name, $active_item_template_name, $class, $active_class)
  {
    global $folder_id;
    $path = '';
    $id = $folder_id;

    $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);
    $item_template_passive = $this->get_template_item_by_name($this->site['template_id'], $item_template_name);
    $item_template_active = $this->get_template_item_by_name($this->site['template_id'], $active_item_template_name);

    $folder = $this->get_folder($id);

    while($folder['parent_id'] != 0)
    {
      $item_template = ($folder_id == $id) ? $item_template_active : $item_template_passive;
      $_class = ($folder_id == $id) ? $active_class : $class;
      if ($item_template)
      {
	$item_template_content = $this->parse_template($item_template['content']);
	$path = str_replace($this->get_tag('content', $item_template_content), '<a class="'.$class.'" href="'.$this->create_url($folder['id']).'">'.$folder['name'].'</a>', $item_template_content).$path;
      }else
      {
	$path = '<a class="'.$_class.'" href="'.$this->create_url($folder['id']).'">'.$folder['name'].'</a>'.$path;
      }

      $id = $folder['parent_id'];
      $folder = $this->get_folder($id);
    }

    if ($template)
    {
      $template_content = $this->parse_template($template['content']);
      $path = str_replace($this->get_tag('content', $template_content), $path, $template_content);
    }

    return $path;
  }

  function print_active_doc($template_name, $class)
  {
    global $file_id, $extension;

    $current_file = '';

    if(isset($file_id) && $file = $this->get_file($file_id))
    {
      $extension = get_extension($file['name']);
      $title = ($file['title'] == '') ? strip_extension($file['name']) : $file['title'];

      if ($template = $this->get_template_item_by_name($this->site['template_id'], $template_name))
      {
	$current_file = $this->parse_template($template['content']);
      }

      $current_file = str_replace($this->get_tag('content', $current_file), '<span class="'.$class.'">'.$title.'</span>', $current_file);
    }
    return $current_file;
  }

  function print_folders($template_name, $item_template_name, $class, $print_rootfolders='true')
  {
    global $folder_id;
    $folders = '';
    $item_template = $this->get_template_item_by_name($this->site['template_id'], $item_template_name);
    $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);

    if ($print_rootfolders == 'true' || $folder_id != $this->site['root_folder_id'])
    {
      $count = $this->get_visible_folders($folder_id);
      if ($count > 0)
      {
	while($this->next_record())
	{
	  if ($item_template)
	  {
	    $item_template_content =  $this->parse_template($item_template['content']);
	    $folders .= str_replace($this->get_tag('content', $item_template_content),'<a class="'.$class.'" href="'.$this->create_url($this->f('id')).'">'.$this->f('name').'</a>', $item_template_content);
	  }else
	  {
	    $folders .= '<a class="'.$class.'" href="'.$this->create_url($this->f('id')).'">'.$this->f('name').'</a>';
	  }
	}
	if ($template)
	{
	  $template_content = $this->parse_template($template['content']);
	  $folders = str_replace($this->get_tag('content', $template_content), $folders, $template_content);
	}
      }
    }
    return $folders;
  }

  function print_back($text, $template_name, $class, $print_rootfolders='true')
  {
    global $folder_id;
    $back = '';
    $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);

    $folder = $this->get_folder($folder_id);

    if ($folder['parent_id'] != 0 && ($print_rootfolders == 'true' || $folder['parent_id'] != $this->site['root_folder_id']))
    {
      $back = '<a class="'.$class.'" href="'.$this->create_url($folder['parent_id']).'">'.$text.'</a>';
      if ($template)
      {
	$template_content = $this->parse_template($template['content']);
	$back = str_replace($this->get_tag('content', $template_content), $back, $template_content);
      }
    }
    return $back;
  }

  function print_rootfolders($template_name, $item_template_name, $active_item_template_name, $class, $active_class)
  {
    global $folder_id;
    $folders = '';
    $active_item_template = $this->get_template_item_by_name($this->site['template_id'], $active_item_template_name);
    $passive_item_template = $this->get_template_item_by_name($this->site['template_id'], $item_template_name);
    $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);

    $count = $this->get_visible_folders($this->site['root_folder_id']);
    if ($count > 0)
    {

      while($this->next_record())
      {
	$item_template = ($this->f('id') == $folder_id) ? $active_item_template : $passive_item_template;
	$_class = ($this->f('id') == $folder_id) ? $active_class : $class;

	if ($item_template)
	{
	  $item_template_content =  $this->parse_template($item_template['content']);
	  $folders .= str_replace($this->get_tag('content', $item_template_content),'<a class="'.$_class.'" href="'.$this->create_url($this->f('id')).'">'.$this->f('name').'</a>', $item_template_content);
	}else
	{
	  $folders .= '<a class="'.$_class.'" href="'.$this->create_url($this->f('id')).'">'.$this->f('name').'</a>';
	}
      }
      if ($template)
      {
	$template_content = $this->parse_template($template['content']);
	$folders = str_replace($this->get_tag('content', $template_content), $folders, $template_content);
      }
    }
    return $folders;
  }

  function print_folderlist($id, $path)
  {
    global $folder_id, $file_id, $GO_CONFIG, $GO_MODULES;

    $folders = '';
    if ($resolved_path = $this->resolve_url($path, $this->site['id']))
    {
      $count = $this->get_visible_folders($resolved_path['folder_id']);
      if ($count > 0)
      {
	$folders .= '<ul id="'.$id.'">';
	while($this->next_record())
	{
	  if ($this->f('id') == $folder_id)
	  {
	    $folders .= '<li><strong><a href="'.$this->create_url($this->f('id')).'">'.$this->f('name').'</a></strong>';
	    $cms = new cms();

	    $folder_count = $cms->get_visible_folders($this->f('id'));
	    if ($folder_count > 0)
	    {
	      $folders .= '<ul>';
	      while($cms->next_record())
	      {
		$folders .= '<li><a href="'.$this->create_url($cms->f('id')).'">'.$cms->f('name').'</a></li>';
	      }
	    }

	    $file_count = $cms->get_files($folder_id, 'priority', 'ASC');

	    if ($file_count > 1)
	    {
	      $browser = detect_browser();

	      $cms_module = $GO_MODULES->get_module('cms');
	      if ($folder_count == 0)
	      {
		$folders .= '<ul>';
	      }
	      if ($browser['name'] == 'MSIE')
	      {
		$target = '_blank';
	      }else
	      {
		$target = '_self';
	      }

	      while($cms->next_record())
	      {
		if ($file_id == $cms->f('id'))
		{
		  $folders .= '<li><strong>';
		}else
		{
		  $folders .= '<li>';
		}
		$extension = $cms->f('extension');

		if ($this->publish_static || eregi('htm',$cms->f('extension')))
		{
		  $folders .= '<a href="'.$this->create_url($folder_id, $cms->f('id')).'">'.strip_extension($cms->f('name')).'</a>';
		}else
		{
		  $folders .= '<a target="'.$target.'" href="'.$cms_module['url'].'download.php?site_id='.$this->site['id'].'&file_id='.$cms->f('id').'">'.strip_extension($cms->f('name')).'</a>';
		}

		if ($file_id == $cms->f('id'))
		{
		  $folders .= '</strong></li>';
		}else
		{
		  $folders .= '</li>';
		}
	      }
	      if ($folder_count > 0 || $file_count > 0)
	      {
		$folders .= '</ul>';
	      }
	      $folders .='</li>';
	    }
	  }else
	  {
	    $folders .= '<li><a href="'.$this->create_url($this->f('id')).'">'.$this->f('name').'</a></li>';
	  }
	}

	$folders .= '</ul>';
      }
      return $folders;
    }
  }

  function print_files($template_name, $item_template_name, $active_item_template_name, $class, $active_class, $print_single=false)
  {
    global $folder_id, $file_id, $GO_CONFIG, $extension, $GO_MODULES;

    $cms_module = $GO_MODULES->get_module('cms');

    $files = '';
    $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);
    $item_template_passive = $this->get_template_item_by_name($this->site['template_id'], $item_template_name);
    $item_template_active = $this->get_template_item_by_name($this->site['template_id'], $active_item_template_name);

    $count = $this->get_files($folder_id, 'priority', 'ASC');
    $browser = detect_browser();
    if ($browser['name'] == 'MSIE')
    {
      $target = '_blank';
    }else
    {
      $target = '_self';
    }

    if ($count > 1 || ($count == 1 && $print_single == 'true'))
    {
      while($this->next_record())
      {
	$extension = $this->f('extension');

	$item_template = ($file_id == $this->f('id')) ? $item_template_active : $item_template_passive;
	$_class = ($file_id == $this->f('id')) ? $active_class : $class;

	if ($this->publish_static || eregi('htm',$this->f('extension')))
	{
	  $content = '<a class="'.$_class.'" href="'.$this->create_url($folder_id, $this->f('id')).'">'.strip_extension($this->f('name')).'</a>';
	}else
	{
	  $content = '<a target="'.$target.'" class="'.$_class.'" href="'.$cms_module['url'].'download.php?site_id='.$this->site['id'].'&file_id='.$this->f('id').'">'.strip_extension($this->f('name')).'</a>';
	}
	if ($item_template)
	{
	  $item_template_content = $this->parse_template($item_template['content']);
	  $files .= str_replace($this->get_tag('content', $item_template_content),$content, $item_template_content);
	}else
	{
	  $files .= $content;
	}
      }

      if ($template)
      {
	$template_content = $this->parse_template($template['content']);
	$files = str_replace($this->get_tag('content', $template_content), $files, $template_content);
      }
    }
    return $files;
  }

  function print_login($text, $template_name, $class)
  {
    global $GO_CONFIG;

    $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);
    $login = '<a class="'.$class.'" href="javascript:popup(\''.$GO_CONFIG->host.'\');">'.$text.'</a>';
    if($template)
    {
      $template_content = $this->parse_template($template['content']);
      $login = str_replace($this->get_tag('content', $template_content), $login, $template_content);
    }
    return $login;
  }

  function get_attributes($tag)
  {
    $attributes = array();
    $in_value = false;
    $in_name = false;
    $name = '';
    $value = '';
    $length = strlen($tag);
    for($i=0;$i<$length;$i++)
    {
      $char = $tag[$i];
      switch ($char)
      {
	case '"':
	  if ($in_value)
	  {
	    $in_value = false;

	    $attributes[trim($name)] = trim($value);
	    $name = '';
	    $value= '';
	  }else
	  {
	    $i++;
	    $char = $tag[$i];
	    $in_value = true;
	  }

	  break;

	case ' ':
	  $i++;
	  $char = $tag[$i];
	  $in_name = true;
	  break;

	case '=':
	  $in_name = false;
	  break;
      }

      if ($in_name)
      {
	$name .= $char;
      }

      if ($in_value)
      {
	$value .= $char;
      }
    }
    return $attributes;
  }

  function print_search_words()
  {
    $this->search_words = '';
    $columns=3;
    $column_count = 0;
    $last_char = '';
    $this->get_search_words($this->site['id']);

    $this->search_words .= '<table border="0" cellspacing="8"><tr>';
    while ($this->next_record())
    {
      $name = $this->f('search_word');
      $char = strtoupper($name[0]);
      if ($char != $last_char)
      {
	if ($last_char != '')
	{
	  $this->search_words .= '</td>';
	  $column_count++;
	}

	if ($column_count == $columns)
	{
	  $this->search_words .= '</tr><tr>';
	  $column_count=0;
	}

	$this->search_words .= '<td valign="top" width="150"><h3>'.$char.'</h3>';
      }

      $this->search_words .= '<table border="0" cellpadding="0" cellspacing="2"><tr><td><a href="'.$_SERVER['PHP_SELF'].'?task=search_results&search_word_id='.$this->f('id').'">'.$name.'</a></td>';
      $this->search_words .= '</tr></table>';
      $last_char = $char;
    }

    for ($i=$column_count+1;$i<$columns;$i++)
    {
      $this->search_words .= '</td><td width="150">&nbsp;';
    }

    $this->search_words .= '</td></tr></table><br />';
    return $this->search_words;
  }

  function print_search($text, $template_name, $active_template_name, $class, $active_class)
  {
    global $GO_MODULES, $GO_CONFIG, $task;

    $cms_module = $GO_MODULES->get_module('cms');

    if (isset($task) && $task == 'search')
    {
      $template = $this->get_template_item_by_name($this->site['template_id'], $active_template_name);
      $class = $active_class;
    }else
    {
      $template = $this->get_template_item_by_name($this->site['template_id'], $template_name);
    }
    if($this->publish_static)
    {
      $search = '<a class="'.$class.'" href="'.$this->publish_url.'_search/index.php?task=search&site_id='.$this->site['id'].'">'.$text.'</a>';
    }else
    {
      $search = '<a class="'.$class.'" href="'.$cms_module['full_url'].'view.php?task=search&site_id='.$this->site['id'].'">'.$text.'</a>';
    }
    if($template)
    {
      $template_content = $this->parse_template($template['content']);
      $search = str_replace($this->get_tag('content', $template_content), $search, $template_content);
    }

    return $search;
  }

  function print_document($path)
  {
    global $GO_CONFIG, $GO_MODULES;

    if ($resolved_path = $this->resolve_url($path, $this->site['id']))
    {
      if ($resolved_path['file_id'] > 0)
      {
	$file = $this->get_file($resolved_path['file_id']);
	$content = $this->parse_template($file['content']);
	$content = preg_replace($this->search_words, '<span class="search">\\0</span>', $content);
	$content = str_replace($cms_module['full_url'].'view.php', $_SERVER['PHP_SELF'], $content);
	return $content;
      }else
      {
	return "<p><b>Error: Could not resolve file from '$path'</b></p>";
      }

    }else
    {
      return "<p><b>Error: Could not resolve url to '$path'</b></p>";
    }
  }

  function get_tag($tag, $content)
  {
    $start_pos = strpos($content, '<'.$tag);
    if($start_pos !== false)
    {
      $content_length= strlen($content);
      $tag_length=0;
      $char=$content[$start_pos+$tag_length];
      $tag = $char;
      while($char != '>' && ($start_pos+$tag_length) < $content_length)
      {
	$tag_length++;
	$char = $content[$start_pos+$tag_length];
	$tag .= $char;
      }
      return $tag;
    }
    return false;
  }

  function parse_template($content)
  {
  	global $folder_id;
  	
    while($tag = $this->get_tag('home', $content))
    {
      $attributes = $this->get_attributes($tag);
      $text = isset($attributes['text']) ? $attributes['text'] : '';
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $active_template = isset($attributes['active_template']) ? $attributes['active_template'] : '';
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $active_class = isset($attributes['active_class']) ? $attributes['active_class'] : '';

      $content = str_replace($tag, $this->print_home($text, $template, $active_template, $class, $active_class), $content);
    }

    while($tag = $this->get_tag('folders', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $item_template = isset($attributes['item_template']) ? $attributes['item_template'] : '';
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $print_rootfolders = isset($attributes['print_rootfolders']) ? $attributes['print_rootfolders'] : 'true';
      $content = str_replace($tag, $this->print_folders($template, $item_template, $class, $print_rootfolders), $content);
    }

    while($tag = $this->get_tag('rootfolders', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $item_template = isset($attributes['item_template']) ? $attributes['item_template'] : '';
      $active_item_template = isset($attributes['active_item_template']) ? $attributes['active_item_template'] : $item_template;
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $active_class = isset($attributes['active_class']) ? $attributes['active_class'] : $class;
      $content = str_replace($tag, $this->print_rootfolders($template, $item_template, $active_item_template, $class, $active_class), $content);
    }

    while($tag = $this->get_tag('files', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $item_template = isset($attributes['item_template']) ? $attributes['item_template'] : '';
      $active_item_template = isset($attributes['active_item_template']) ? $attributes['active_item_template'] : $item_template;
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $active_class = isset($attributes['active_class']) ? $attributes['active_class'] : $class;
      $print_single = isset($attributes['print_single']) ? $attributes['print_single'] : 'true';
      $content = str_replace($tag, $this->print_files($template, $item_template, $active_item_template, $class, $active_class, $print_single), $content);
    }

    while($tag = $this->get_tag('path', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $item_template = isset($attributes['item_template']) ? $attributes['item_template'] : '';
      $active_item_template = isset($attributes['active_item_template']) ? $attributes['active_item_template'] : $item_template;
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $active_class = isset($attributes['active_class']) ? $attributes['active_class'] : $class;
      $content = str_replace($tag, $this->print_path($template, $item_template, $active_item_template, $class, $active_class), $content);
    }

    while($tag = $this->get_tag('icon', $content))
    {
      $content = str_replace($tag, $this->print_icon(), $content);
    }

    while($tag = $this->get_tag('login', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $text = isset($attributes['text']) ? $attributes['text'] : '';
      $content = str_replace($tag, $this->print_login($text,$template, $class), $content);
    }

    while($tag = $this->get_tag('search', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $active_template = isset($attributes['active_template']) ? $attributes['active_template'] : $template;
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $active_class = isset($attributes['active_class']) ? $attributes['active_class'] : $class;
      $text = isset($attributes['text']) ? $attributes['text'] : '';
      $content = str_replace($tag, $this->print_search($text,$template, $active_template, $class, $active_class), $content);
    }

    while($tag = $this->get_tag('back', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $text = isset($attributes['text']) ? $attributes['text'] : '';
      $print_rootfolders = isset($attributes['print_rootfolders']) ? $attributes['print_rootfolders'] : 'true';
      $content = str_replace($tag, $this->print_back($text,$template, $class, $print_rootfolders), $content);
    }

    while($tag = $this->get_tag('active_doc', $content))
    {
      $attributes = $this->get_attributes($tag);
      $template = isset($attributes['template']) ? $attributes['template'] : '';
      $class = isset($attributes['class']) ? $attributes['class'] : '';
      $content = str_replace($tag, $this->print_active_doc($template, $class), $content);
    }

    while($tag = $this->get_tag('document', $content))
    {
      $attributes = $this->get_attributes($tag);
      $path = isset($attributes['path']) ? $attributes['path'] : '';
      $content = str_replace($tag, $this->print_document($path), $content);
    }

    while($tag = $this->get_tag('folderlist', $content))
    {
      $attributes = $this->get_attributes($tag);
      $path = isset($attributes['path']) ? $attributes['path'] : '';
      $id = isset($attributes['id']) ? $attributes['id'] : '';
      $content = str_replace($tag, $this->print_folderlist($id, $path), $content);
    }
    
    while($tag = $this->get_tag('hot_items', $content))
    {
    	$attributes = $this->get_attributes($tag);
    	$template = isset($attributes['template']) ? $attributes['template'] : '';
      $class = isset($attributes['class']) ? $attributes['class'] : '';
    	$content = str_replace($tag, $this->print_hot_items($folder_id, $template, $class), $content);
    }
    
    return $content;
  }
}
?>
