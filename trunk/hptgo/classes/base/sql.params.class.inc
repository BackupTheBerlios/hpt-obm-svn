<?php
/*
   Copyright HPTVietnam 2004
   Author: Nguyễn Thái Ngọc Duy <duyntn@hptvietnam.com.vn>

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

class GO_PARAMS
{
	var $params = array();

  function GO_PARAMS()
  {
		$this->reload();
  }

	function reload()
	{
		$db = new db();
		$db->query("SELECT * FROM settings");
		while ($db->next_record())
			$this->params[$db->f('name')] = $db->f('value');	
	}

	function update()
	{
		$db = new db();
		foreach ($this->params as $key => $val)
		{
			$db->query("REPLACE settings SET name='$key', value='$val'");
		}
	}
}
?>
