<?php
/* Highlighting skipps html -highlighting_skipphtml.php-

   This class can be used to highlight words in a html text.
   Matches in the html tags will be skipped.

   Example:
   ereg_replace('image','<b>image</b>','<img src="image.gif">image.gif');
   will give you:
   <img src="<b>image</b>.gif"><b>image</b>.gif
   using this class you will get:
   <img src="image.gif"><b>image</b>.gif

   see also example.php

   Version 0.1
   Last change: 2003/05/23
   copyrigth 2002 Email Communications, http://www.emailcommunications.nl/
   written by Bas Jobsen (bas@startpunt.cc)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/


class highlighting_skipphtml
{
    var $counter;

    function highlighting_skipphtml()
    {
    	$this->counter=0;
    }

	function highlight($words,&$string,$open='<b>',$close='</b>',$doublecheck=0)
	{
		if(is_array($words))
		{
			foreach($words as $word)
				$this->highlight($word,$string,$open,$close,$doublecheck);
		}else
		{
            $wordreg=preg_replace('/([\.\*\+\(\)\[\]])/','\\\\\1',$words);
			$string=preg_replace('/(<)([^>]*)('.("$wordreg").')([^<]*)(>)/sei',"'\\1'.preg_replace('/'.(\"$wordreg\").'/i','###','\\2\\3\\4').'\\5'",stripslashes($string));
			$string=preg_replace('/('.$wordreg.')/si',$open.'\\1'.$close,stripslashes($string));
			$string=preg_replace('/###/si',$words,$string);
			if($this->counter>0 && $doublecheck)
			{
				$tc=str_replace('/','\/',$close);
				$string=preg_replace('/('.$open.')([^<]*)('.$open.')([^<]+)('.$tc.')([^<]*)('.$tc.')/si','\\1\\2\\4\\6\\7',$string);
			}
			$this->counter++;
		}
	}

	function dohighlight($words,$string,$open='<b>',$close='</b>',$doublecheck=0)
	{
		$this->highlight($words,$string,$open,$close,$doublecheck);
		return $string;
	}
}
?>