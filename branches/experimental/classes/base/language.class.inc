<?php
/**
*  Copyright Intermesh 2004
*  Author: Merijn Schering <mschering@intermesh.nl>
*  Version: 1.0 Release date: 27 March 2003
*  Version: 2.0 Release date: 23 May 2004
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at your
*  option) any later version.
*/

/**
* This handles the multilingual system of Group-Office
*
* @package  E-mail authentication class
* @author   Merijn Schering <mschering@intermesh.nl>
* @since    Group-Office 1.0
*/

class GO_LANGUAGE
{
	/**
	* The current language setting
	*
	* @var     string
	* @access  private
	*/
  var $language;
  
	/**
	* The path to the common language files
	*
	* @var     string
	* @access  private
	*/
  var $language_path;
  
  /**
	* The default language
	*
	* @var     string
	* @access  private
	*/
  var $default_language;

	/**
	* Constructor. Initialises language setting Checks in following order:
	* User preference (Session), Browser language setting, default language
	*
	* @access public
	* @return string	language code (See developer guidelines for codes)
	*/
  function GO_LANGUAGE()
  {	
    global $GO_CONFIG;

    $this->language_path = $GO_CONFIG->root_path.$GO_CONFIG->language_path.'/';
    $this->default_language = $GO_CONFIG->language;		
    $this->language = $this->default_language;		

    if (!isset($_SESSION['GO_SESSION']['language']) || $_SESSION['GO_SESSION']['language'] == '')
    {
      require($this->language_path.'languages.inc');

      if (isset($_SERVER['HTTP_ACCEPT_LANGUAGE']) && 
	  isset($languages[$_SERVER['HTTP_ACCEPT_LANGUAGE']]) && 
	  file_exists($this->language_path.$languages[$_SERVER['HTTP_ACCEPT_LANGUAGE']]['language_file']))
      {
	return $this->set_language($_SERVER['HTTP_ACCEPT_LANGUAGE']);
      }else
      {
	return $this->set_language($this->default_language);
      }
    }else
    {
      return $this->language = $_SESSION['GO_SESSION']['language'];
    }
  }
  
	/**
	*	Set the language for this browser session
	*
	*	@param  $language	The language code (See developer guidelines for codes)
	* @access public
	* @return string	language code
	*/
  function set_language($language)
  {
    if ($this->language = $this->get_language($language))
    {	
      $_SESSION['GO_SESSION']['language'] = $this->language;		
    }
  }

	/**
	*	Get's a language file from the framework (Not a module)
	*
	*	@param  $section	The section to fetch language for. (See dirs in 'language')
	* @access public
	* @return string	Full path to the language file
	*/
  function get_base_language_file($section)
  {
    global $GO_CONFIG;
     $file = $this->language_path.
      $section.$GO_CONFIG->slash.
      $this->language['language_file'].'.inc';
    if ( file_exists( $file ) ) {
      return $file;
    } else {
      return $this->get_fallback_base_language_file($section);
    }
  }

	/**
	*	Get's the default language file from the framework (Not a module).
	* This is always included before the prefered language file. 
	* If the prefered language file misses some strings they will be
	* defined by the default language.
	*
	*	@param  $section	The section to fetch language for. (See dirs in 'language')
	* @access public
	* @return string	Full path to the fallback language file
	*/
  function get_fallback_base_language_file($section)
  {
    global $GO_CONFIG;

    $file = $this->language_path.
      $section.$GO_CONFIG->slash."en.inc";
    if ( file_exists( $file ) ) {
      return $file;
    } else {
      die('<b>Fatal Error:</b> Your language is not supported and '.
	  'Group-Office could not fall back on English in '.$_SERVER['PHP_SELF'].' ('.$section.')');
    }
  }

	/**
	*	Get's a language file from a module
	*
	*	@param  $module_id	The module to fetch language for.
	* @access public
	* @return string	Full path to the language file
	*/
  function get_language_file($module_id)
  {
    global $GO_CONFIG;

    /* 
		 The new language file location is inside the language folder in the
     modules folder. So we create the absolute path to the file and check
     if this file exists.
    */

    $file = $GO_CONFIG->module_path.
      $module_id.$GO_CONFIG->slash.
      "language".$GO_CONFIG->slash.
      $this->language['language_file'].'.inc';

    if ( file_exists( $file ) ) {
      return $file;
    } else {
      return $this->get_fallback_language_file($module_id);
    }
  }

	/**
	*	Get's the prefered language file.
	*
	*	@param  $section	The section to fetch language for. (See dirs in 'language')
	* @access public
	* @return string	Full path to the language file
	*/
  function get_fallback_language_file($module_id)
  {
    global $GO_CONFIG;

    $file = $GO_CONFIG->module_path.
      $module_id.$GO_CONFIG->slash.
      "language".$GO_CONFIG->slash."en.inc";

    if ( file_exists( $file ) ) {
      return $file;
    } else {
      die('<b>Fatal Error:</b> Your language is not supported and '.
	  'Group-Office could not fall back on English in '.$_SERVER['PHP_SELF'].' ('.$module_id.')');
    }
  }

	/**
	*	Get's all languges in an array sorted by description.
	*
	* @access public
	* @return array		The array contains the array's with:
	*									language_file
	*									description
	*									currency
	*									decimal_seperator
	*									thousands_seperator
	*									date_format
	*									time_format
	*									first_weekday
	*									timezone
	*									DST (Daylight savings time)
	*/
  function get_languages()
  {
    require($this->language_path.'languages.inc');

    $countries = array();
    foreach($languages as $key => $value)
    {
      if (isset($languages[$key]['description']))
      {
	$countries[$key] = $languages[$key]['description'];
      }
    }
    asort($countries);

    $_languages = array();
    foreach($countries as $key => $value)
    {
      $languages[$key]['code'] = $key;
      $_languages[] = $languages[$key];
    }
    return $_languages;	
  }
  
	/**
	*	Get's the properties of a language code in an array
	*
	*	@param  $code		The language code (See developer guidelines for codes)
	* @access public
	* @return array		Properties of the language:
	*									code
	*									language_file
	*									description
	*									currency
	*									decimal_seperator
	*									thousands_seperator
	*									date_format
	*									time_format
	*									first_weekday
	*									timezone
	*									DST (Daylight savings time)
	*/
  function get_language($code)
  {
    require($this->language_path.'languages.inc');
    if(isset($languages[$code]))
    {
      $languages[$code]['code'] = $code;
      return $languages[$code];
    }
    return false;
  }
}
?>
