<?php
/*
   This scripts replaces strings in all Group-Office files.
   Enter old keywords in $string[n]['old'] and the new in $string[n]['new'].
   BE CAREFULL WITH THIS SCRIPT!
 */

require("../../Group-Office.php");
//$GO_SECURITY->authenticate();

	$string[0]['old'] = 'get_fallback_language_file(\'filetypes\')';
    $string[0]['new'] = 'get_fallback_base_language_file(\'filetypes\')';

      $count = count($string);
      require($GO_CONFIG->class_path."filesystem.class.inc");
      $filesystem = new filesystem;

      function replace_files($path)
      {
      global $count, $string;

      $replace_count=0;
      $dir=opendir($path);
      while ($file=readdir($dir))
      {
      if (is_dir($path.$file))
      {
      if ($file != "." && $file != ".." && $file != 'CVS')
      {
      replace_files($path.$file.'/');
      }
      }else
      {
	if ($file != 'replace.php')
	{
	  $fp = fopen($path.$file, 'r');

	  $data = fread($fp, filesize($path.$file));
	  $old_data = $data;
	  fclose($fp);

	  for ($i=0;$i<$count;$i++)
	  {
	    if($data = str_replace($string[$i]['old'],$string[$i]['new'], $data))
	    {
	      $replace_count++;
	    }
	  }
	  if($data != $old_data)
	  {
	    $fp = fopen($path.$file, 'w+');
	    fwrite($fp, $data);
	    fclose($fp);
	    //echo $path.$file."<br />";
	  }  
	}

      }
      }
      closedir($dir);
      return $replace_count;
      }
echo replace_files($GO_CONFIG->root_path.'language/filetypes/').' occurences replaced';
?>
