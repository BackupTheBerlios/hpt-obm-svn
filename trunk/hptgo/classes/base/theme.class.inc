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

class GO_THEME
{
	var $theme; // theme name
	var $default_theme; // default theme
	var $image_url; // theme image url
	var $theme_path; // theme image path
	var $theme_url; // them url
	var $stylesheet; // stylesheet url
	var $path_to_themes; // theme path
	var $images = array();
	var $sounds = array();

	function GO_THEME()
	{
		global $GO_CONFIG;
		$this->path_to_themes = $GO_CONFIG->root_path.$GO_CONFIG->theme_path.$GO_CONFIG->slash;
		if ($GO_CONFIG->slash == '\\')
		{
			$this->url_to_themes = str_replace('\\', '', $GO_CONFIG->host.$GO_CONFIG->theme_path.$GO_CONFIG->slash);
		}else
		{
			$this->url_to_themes = $GO_CONFIG->host.$GO_CONFIG->theme_path.$GO_CONFIG->slash;
		}

		$this->default_theme = $GO_CONFIG->theme;
		$_SESSION['GO_SESSION']['theme'] = isset($_SESSION['GO_SESSION']['theme']) ? $_SESSION['GO_SESSION']['theme'] : $this->default_theme;

		if ($_SESSION['GO_SESSION']['theme'] != '' && file_exists($this->path_to_themes.$_SESSION['GO_SESSION']['theme']))
		{
			$this->theme = $_SESSION['GO_SESSION']['theme'];
		}else
		{
			$_SESSION['GO_SESSION']['theme'] = $this->default_theme;
			$this->theme = $this->default_theme;
		}

		$this->theme_path = $this->path_to_themes.$this->theme.$GO_CONFIG->slash;
		$this->theme_url = $this->url_to_themes.$this->theme.'/';
		$this->image_url = $this->theme_url.'images/';
		$this->stylesheet = $this->theme_url."style.css";
		require($this->theme_path.'images.inc');
		foreach($images as $key => $value)
		{
			$this->images[$key] = $this->image_url.$value;
		}
		require($this->theme_path.'sounds.inc');
		foreach($sounds as $key => $value)
		{
			$this->sounds[$key] = $this->theme_url.'sounds/'.$value;
		}
	}

	//gets the themes
	function get_themes()
	{
		//$location = $SCRIPT_FILENAME."languages";
		$theme_dir=opendir($this->path_to_themes);
		while ($file=readdir($theme_dir))
		{
			//Couldn't get is_file to work right so i worked around
			if (is_dir($this->path_to_themes.$file) && $file != "." && $file != ".." && $file != 'CVS')
			{
				  $themes[] = $file;
			}
		}
		closedir($theme_dir);
		return $themes;
	}
}
?>
