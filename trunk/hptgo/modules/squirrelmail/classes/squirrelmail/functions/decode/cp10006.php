<?php
/*
 * decode/cp10006.php
 * $Id: cp10006.php,v 1.1.2.2 2004/02/24 15:57:27 kink Exp $
 *
 * Copyright (c) 2003-2004 The SquirrelMail Project Team
 * Licensed under the GNU GPL. For full terms see the file COPYING.
 *
 * This file contains cp10006 (MacGreek) decoding function that 
 * is needed to read cp10006 encoded mails in non-cp10006 locale.
 * 
 * Original data taken from:
 *  ftp://ftp.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/MAC/GREEK.TXT
 *
 *  Name:     cp10006_MacGreek to Unicode table
 *  Unicode version: 2.0
 *  Table version: 2.00
 *  Table format:  Format A
 *  Date:          04/24/96
 *  Authors:       Lori Brownell <loribr@microsoft.com>
 *                 K.D. Chang    <a-kchang@microsoft.com>
 */

function charset_decode_cp10006 ($string) {
    global $default_charset;

    if (strtolower($default_charset) == 'x-mac-greek')
        return $string;

    /* Only do the slow convert if there are 8-bit characters */
    /* avoid using 0xA0 (\240) in ereg ranges. RH73 does not like that */
    if (! ereg("[\200-\237]", $string) and ! ereg("[\241-\377]", $string) )
        return $string;

    $cp10006 = array(
	"\0x80" => '&#196;',
	"\0x81" => '&#185;',
	"\0x82" => '&#178;',
	"\0x83" => '&#201;',
	"\0x84" => '&#179;',
	"\0x85" => '&#214;',
	"\0x86" => '&#220;',
	"\0x87" => '&#901;',
	"\0x88" => '&#224;',
	"\0x89" => '&#226;',
	"\0x8A" => '&#228;',
	"\0x8B" => '&#900;',
	"\0x8C" => '&#168;',
	"\0x8D" => '&#231;',
	"\0x8E" => '&#233;',
	"\0x8F" => '&#232;',
	"\0x90" => '&#234;',
	"\0x91" => '&#235;',
	"\0x92" => '&#163;',
	"\0x93" => '&#8482;',
	"\0x94" => '&#238;',
	"\0x95" => '&#239;',
	"\0x96" => '&#8226;',
	"\0x97" => '&#189;',
	"\0x98" => '&#8240;',
	"\0x99" => '&#244;',
	"\0x9A" => '&#246;',
	"\0x9B" => '&#166;',
	"\0x9C" => '&#173;',
	"\0x9D" => '&#249;',
	"\0x9E" => '&#251;',
	"\0x9F" => '&#252;',
	"\0xA0" => '&#8224;',
	"\0xA1" => '&#915;',
	"\0xA2" => '&#916;',
	"\0xA3" => '&#920;',
	"\0xA4" => '&#923;',
	"\0xA5" => '&#926;',
	"\0xA6" => '&#928;',
	"\0xA7" => '&#223;',
	"\0xA8" => '&#174;',
	"\0xA9" => '&#169;',
	"\0xAA" => '&#931;',
	"\0xAB" => '&#938;',
	"\0xAC" => '&#167;',
	"\0xAD" => '&#8800;',
	"\0xAE" => '&#176;',
	"\0xAF" => '&#903;',
	"\0xB0" => '&#913;',
	"\0xB1" => '&#177;',
	"\0xB2" => '&#8804;',
	"\0xB3" => '&#8805;',
	"\0xB4" => '&#165;',
	"\0xB5" => '&#914;',
	"\0xB6" => '&#917;',
	"\0xB7" => '&#918;',
	"\0xB8" => '&#919;',
	"\0xB9" => '&#921;',
	"\0xBA" => '&#922;',
	"\0xBB" => '&#924;',
	"\0xBC" => '&#934;',
	"\0xBD" => '&#939;',
	"\0xBE" => '&#936;',
	"\0xBF" => '&#937;',
	"\0xC0" => '&#940;',
	"\0xC1" => '&#925;',
	"\0xC2" => '&#172;',
	"\0xC3" => '&#927;',
	"\0xC4" => '&#929;',
	"\0xC5" => '&#8776;',
	"\0xC6" => '&#932;',
	"\0xC7" => '&#171;',
	"\0xC8" => '&#187;',
	"\0xC9" => '&#8230;',
	"\0xCA" => '&#160;',
	"\0xCB" => '&#933;',
	"\0xCC" => '&#935;',
	"\0xCD" => '&#902;',
	"\0xCE" => '&#904;',
	"\0xCF" => '&#339;',
	"\0xD0" => '&#8211;',
	"\0xD1" => '&#8213;',
	"\0xD2" => '&#8220;',
	"\0xD3" => '&#8221;',
	"\0xD4" => '&#8216;',
	"\0xD5" => '&#8217;',
	"\0xD6" => '&#247;',
	"\0xD7" => '&#905;',
	"\0xD8" => '&#906;',
	"\0xD9" => '&#908;',
	"\0xDA" => '&#910;',
	"\0xDB" => '&#941;',
	"\0xDC" => '&#942;',
	"\0xDD" => '&#943;',
	"\0xDE" => '&#972;',
	"\0xDF" => '&#911;',
	"\0xE0" => '&#973;',
	"\0xE1" => '&#945;',
	"\0xE2" => '&#946;',
	"\0xE3" => '&#968;',
	"\0xE4" => '&#948;',
	"\0xE5" => '&#949;',
	"\0xE6" => '&#966;',
	"\0xE7" => '&#947;',
	"\0xE8" => '&#951;',
	"\0xE9" => '&#953;',
	"\0xEA" => '&#958;',
	"\0xEB" => '&#954;',
	"\0xEC" => '&#955;',
	"\0xED" => '&#956;',
	"\0xEE" => '&#957;',
	"\0xEF" => '&#959;',
	"\0xF0" => '&#960;',
	"\0xF1" => '&#974;',
	"\0xF2" => '&#961;',
	"\0xF3" => '&#963;',
	"\0xF4" => '&#964;',
	"\0xF5" => '&#952;',
	"\0xF6" => '&#969;',
	"\0xF7" => '&#962;',
	"\0xF8" => '&#967;',
	"\0xF9" => '&#965;',
	"\0xFA" => '&#950;',
	"\0xFB" => '&#970;',
	"\0xFC" => '&#971;',
	"\0xFD" => '&#912;',
	"\0xFE" => '&#944;',
	"\0xFF" => '&#65535;'
   );

    $string = str_replace(array_keys($cp10006), array_values($cp10006), $string);

    return $string;
}
?>