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

/**
 * Used to maintain table filetypes (MIME types)
 */
class filetypes extends db
{
	function filetypes()
	{
		$this->db();
	}

	/**
	 * Add a new type with extension $extension and optionally mime type $mime, friendly name $friendly, icon $icon
	 * Missing information will be filled by default values
	 * Return array ('extension','mime','friendly') (and 'image' (icon) if $return_image is true)
	 */
	function add_type($extension, $mime='', $friendly='', $icon='',$return_image=false)
	{
		global $GO_CONFIG;

		$extension = substr($extension, 0, 10);

		if ($mime == '')
		{
			$mime = $this->get_mime_type($extension);
		}

		if ($friendly == '')
		{
			$friendly = $mime;
		}

		if ($icon == '')
		{
			$GO_CONFIG->default_filetype_icon;
		}

		$fd = fopen ($GO_CONFIG->default_filetype_icon, "r");
		$icon = fread ($fd, filesize($GO_CONFIG->default_filetype_icon));
		fclose ($fd);

		$sql = "INSERT INTO filetypes (extension,mime, friendly, image) ".
						"VALUES ('$extension', '$mime', '$friendly', '$icon')";
		if($this->query($sql))
		{
			$type['extension'] = $extension;
			$type['mime'] = $mime;
			$type['friendly'] = $friendly;
			if ($return_image)
			{
				$type['image'] = $icon;
			}
			return $type;
		}

		return false;
	}

	/**
	 * return file info corresponding to extension $extension.
	 * Information may be retrieved through db interface.
	 * return true if successful, false otherwise.
	 */
	function get_type($extension, $return_image=false)
	{
		$extension = substr($extension, 0, 10);
		$fields = isset($return_image) ? '*' : 'mime, friendly, extension';
		$sql = "SELECT $fields FROM filetypes WHERE extension='$extension'";
		if($this->query($sql))
		{
			if ($this->next_record())
			{
				return $this->Record;
			}
		}
		return false;
	}

	/**
	 * return $offset file types, starting from $first.
	 * return total number of file types.
	 */
	function get_filetypes($first=0, $offset=0)
	{
		$sql = "SELECT * FROM filetypes ORDER BY extension ASC";
		if ($offset != 0)
		{
			$sql .= " LIMIT $first, $offset";
			$this->query("SELECT COUNT(*) FROM filetypes");
			if ($this->next_record())
			{
				$count = $this->f(0);
				if ($count > 0)
				{
					$this->query($sql);
				}
				return $count;
			}

		}else
		{
			$this->query($sql);
			return $this->num_rows();
		}
	}

	/**
	 * Change friendly name/icon of filetype $extension.
	 * return true if successful, false otherwise.
	 */
	function update_filetype($extension, $friendly, $icon='')
	{
		if ($icon != '' && $data = addslashes(fread(fopen($icon, "r"), filesize($icon))))
		{
			$sql = "UPDATE filetypes SET friendly='$friendly', image='$data' WHERE extension='$extension'";
		}else
		{
			$sql = "UPDATE filetypes SET friendly='$friendly' WHERE extension='$extension'";
		}
		return $this->query($sql);
	}

	/**
	 * Delete filetype $extension.
	 * return true if successful, false otherwise.
	 */
	function delete_type($extension)
	{
		$sql = "DELETE FROM filetypes WHERE extension='$extension'";
		return $this->query($sql);
	}

	/**
	 * Get filetype $extension from mimetype file $GO_CONFIG->mime_types_file.
	 * return correspoding mime type, application/octet-stream if not found.
	 */
	function get_mime_type($extension)
	{
		global $GO_CONFIG;
		$file = file($GO_CONFIG->mime_types_file);
		foreach($file as $line)
		{
			rtrim($line);
			if (preg_match('/^\#/', $line))
				continue;
			$elms = preg_split('/\s+/', $line);
			$mime = array_shift($elms);
			foreach ($elms as $elm)
			{
				if ($elm == $extension)
				{
					return $mime;
				}
		 	}
		}
		return 'application/OCTET-STREAM';
	}

	/**
	 * Get extension of filename $filename.
	 */
	function get_extension($filename)
	{
		if ($filename{0} == '.')
		{
			$filename = substr($filename, 1);
		}
		ereg(".*\.([a-zA-z0-9]{0,5})$", $filename, $regs);
		return $regs[1];
	}

	/**
	 * Get mime extension of mime type $mime.
	 */
	function get_mime_extension($mime)
	{
		$sql = "SELECT extension FROM filetypes WHERE mime='$mime'";
		$this->query($sql);
		if ($this->next_record())
		{
			return $this->f('extension');
		}
		return false;
	}
}
?>
