<?php
/**
*  Copyright Intermesh 2004
*  Author: Merijn Schering <mschering@intermesh.nl>
*  Version: 1.0 Release date: 19 July 2004
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at your
*  option) any later version.
**/

class GO_LOG
{
	var $fp;
	
  function GO_LOG()
  {
  	global $GO_CONFIG;
		$this->fp = fopen($GO_CONFIG->root_path.'groupoffice.log', 'a+');  	
  }

  function log($text)
  {
		global $GO_MODULES;
		
		$module = isset($GO_MODULES->id) ? $GO_MODULES->id : 'general';
		
		$text = date('Y-m-d G:i:s', time()).' ['.$module.']: '.$text."\n";
		return fwrite($this->fp, $text);
  }
  
  function close()
  {
  	return fclose($this->fp);
  }
}
?>
