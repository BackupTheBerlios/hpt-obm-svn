<?php
class wbxml
{
	var $wbxmlfile = '/tmp/tmp.wbxml';
	var $xmlfile = '/tmp/tmp.xml';	
	
	function to_xml($wbxml)
	{		
		//create temp file
		$fp = fopen($this->wbxmlfile, 'w+');
		fwrite($fp, $wbxml);
		fclose($fp);
		//convert temp file
		exec('wbxml2xml -o '.$this->xmlfile.' '.$this->wbxmlfile);
		//read xml
		$wbxml = implode("\r\n", file($this->xmlfile));
		
		//remove temp files
		//unlink($this->xmlfile);
		//unlink($this->wbxmlfile);	
		
		return $wbxml;		
	}
	
	function to_wbxml($xml)
	{
			//create temp file
		$fp = fopen($this->xmlfile, 'w+');
		fwrite($fp, $xml);
		fclose($fp);
		//convert temp file
		exec('xml2wbxml -o '.$this->wbxmlfile.' '.$this->xmlfile);
		//read xml
		$xml = implode("\r\n", file($this->wbxmlfile));
		
		//remove temp files
		//unlink($this->xmlfile);
		//unlink($this->wbxmlfile);	
		
		return $xml;	
	}
}
?>
