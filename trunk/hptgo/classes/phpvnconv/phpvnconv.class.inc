<?php
/* ---------------------------------------------------------------------------   
   phpVnconv: Vietnamese Converting Engine for PHP v1.0 (20/09/2002)
   http://linux.nguyendai.org/phpvnconv/
   (C) 2001-2002, NGUYEN-DAI Quy <Quy@NguyenDai.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
   --------------------------------------------------------------------------- */
  
/* Wed Jan  8 22:22:27 CET 2003 */
/*
  Usage: i
   $a = new phpVnconv();
   $a->set_from('utf8');
   $a->set_to('viscii');
   echo $a->vnconv('hahaha');
  
*/

class phpVnconv {
	var $FROM = 'utf8';
	var $TO   = 'ascii';
	var $PATH = '.';

	function phpVnconv ($path = ''){
		global $GO_CONFIG;
		if ($path == '')
    		$path = $GO_CONFIG->class_path.'phpvnconv/';
		if (!is_dir($path))
			$this->error('Path not found');
			
		if (substr($path, -1) != '/')
			$path .= '/';
		$this->PATH = $path;
	}
	
	function set_from ($from) {
		if ($from)
			$this->FROM = strtolower($from);
		else
			$this->FROM = 'viqr';
	}
	
	function set_to ($to) {
		if ($to)
			$this->TO = strtolower($to);
		else
			$this->TO = 'utf8';
	}

	function error ($msg){
		echo 'ERROR: '.$msg;
		exit;
	}
	///////////////////////////////////////////////////////////
	// Input:
	//    $str : string
	//    $from: convert from this charset (VIQR by default)
	//    $to  : convert to this charset   (UTF-8 by default)
	// Output: string in $to charset
	///////////////////////////////////////////////////////////
	function VnConv ($str='') {
		$vn_from = $vn_to = array();
		
		if ($str === '' OR $this->FROM === '' OR $this->TO === '')
			return '';
	
/*		// Some aliases
		switch ($this->TO){
			case 'iso8859-1' : $this->TO = 'viqr'; break;
			case 'iso10646-1': $this->TO = 'utf8'; break;
		}
	*/
		// Not need to go more
		if ($this->FROM == $this->TO)
			return $str;

		// sviqr: Special VIQR => Escape for URL, E-mail,...
		if ($this->FROM == 'sviqr'){
			$is_sviqr = 1;
			$this->FROM = 'viqr';
		} else {
			$is_sviqr = 0;
		}
	
		// Including path	
		//$path = '/opt/apache/htdocs/vn';
//		$path = 'phpvnconv';
		// Extension of including files
		$ext  = '.inc';
		$fto   = $this->PATH.'/'.$this->TO.$ext;
		$ffrom = $this->PATH.'/'.$this->FROM.$ext;
		// Check including files
		if ( !file_exists($ffrom) OR !file_exists($fto))
			$this->error('Vietnamese charset not found from '.$ffrom);

		include ($ffrom);	
		include ($fto);

		$cfrom   = $this->FROM;
		$cto     = $this->TO;
		$vn_from = $$cfrom;
		$vn_to   = $$cto;

		// Build converting array
		$size = sizeof ($vn_from);
		for ($i = 0; $i < $size; $i++){
			$trans[$vn_from[$i]] = $vn_to[$i];
		}
		// Some escape for VIQR
		if ($this->FROM == 'viqr'){
			$trans["\d"] = "d";
			$trans["\D"] = "D";
			$trans["\^"] = "^";
			$trans["\+"] = "+";
			$trans["\."] = ".";
			$trans["\'"] = "'";
			$trans["\`"] = "`";
			$trans["\?"] = "?";
			$trans["\~"] = "~";
		}
		// If string contains URL, E-mail=> it should be escaped
		if ($is_sviqr == 1) {
			$tmp = $str;
			$str = '';
			while ( preg_match("/http|ftp|mailto|@/i", $tmp) ){
				$ss   = Escape_Viqr($tmp);
				$str .= $ss[0].' ';
				$tmp  = $ss[1];
			}
			if ($ss[1] != ''){ $str .= $ss[1]; }
		}

		return Strtr ($str, $trans);
	}

	function Escape_Viqr($str){
		if (preg_match("/(http|ftp|mailto)/", $str, $mat)){
			$url = $mat[1];
			$aa = preg_split("/$url/", $str, 2);
			$s1 = $aa[0];
			$bb = $url.$aa[1];
			$aa = preg_split("/ /", $bb, 2);
			// for return
			$tmp[0] = $s1. ' ' . preg_replace("/([aeyuio])\./i","\\1\\.",$aa[0]);
			$tmp[1] = $aa[1];
		} else if (preg_match("/[\w\.]+@[\w\-\.]+\.\w{2,3}/", $str)){
			$aa = preg_split("/@/", $str, 2);
			$pos1 = strrpos($aa[0],' ');
			if ($pos1 === false){ $pos1 = 0;}
			$pos2 = strpos($aa[1], ' ');
			if ($pos2 === false){ $pos2 = strlen($aa[1]);}
			//echo 'pos1=',$pos1,', pos2=',$pos2,'<br>';
			$tmp[0] = substr($aa[0], 0, $pos1);
			$email = substr($aa[0],$pos1).'@'.substr($aa[1],0,$pos2);
			// for return
			$tmp[0] .= ' ' . preg_replace("/([aeyuio])\./i","\\1\\.",$email);
			$tmp[1] = substr($aa[1], $pos2);
			
		} else {
			$tmp[0] = $str; $tmp[1] = '';
		}
		return $tmp;
	}

} // end of class	
/* vim: set si ai ts=4 tw=0 sw=4 : */
?>
