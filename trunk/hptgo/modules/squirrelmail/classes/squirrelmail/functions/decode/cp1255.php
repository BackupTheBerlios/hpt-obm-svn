<?php
/**
 * decode/cp1255.php
 *
 * Copyright (c) 2003-2004 The SquirrelMail Project Team
 * Licensed under the GNU GPL. For full terms see the file COPYING.
 *
 * This file contains cp1255 decoding function that is needed to read
 * cp1255 encoded mails in non-cp1255 locale.
 * 
 * Original data taken from:
 *  ftp://ftp.unicode.org/Public/MAPPINGS/VENDORS/MICSFT/WINDOWS/CP1255.TXT
 *
 *   Name:     cp1255 to Unicode table
 *   Unicode version: 2.0
 *   Table version: 2.01
 *   Table format:  Format A
 *   Date:          1/7/2000
 *   Contact:       cpxlate@microsoft.com
 *
 * @version $Id: cp1255.php,v 1.5 2004/10/15 09:15:48 tokul Exp $
 * @package squirrelmail
 * @subpackage decode
 */

/**
 * Decode cp1255-encoded string
 * @param string $string Encoded string
 * @return string $string decoded string
 */
function charset_decode_cp1255 ($string) {
    global $default_charset;

    if (strtolower($default_charset) == 'windows-1255')
        return $string;

    // don't do decoding when there are no 8bit symbols
    if (! sq_is8bit($string,'windows-1255'))
        return $string;

    $cp1255 = array(
	"\x80" => '&#8364;',
	"\x81" => '&#65533;',
	"\x82" => '&#8218;',
	"\x83" => '&#402;',
	"\x84" => '&#8222;',
	"\x85" => '&#8230;',
	"\x86" => '&#8224;',
	"\x87" => '&#8225;',
	"\x88" => '&#710;',
	"\x89" => '&#8240;',
	"\x8A" => '&#65533;',
	"\x8B" => '&#8249;',
	"\x8C" => '&#65533;',
	"\x8D" => '&#65533;',
	"\x8E" => '&#65533;',
	"\x8F" => '&#65533;',
	"\x90" => '&#65533;',
	"\x91" => '&#8216;',
	"\x92" => '&#8217;',
	"\x93" => '&#8220;',
	"\x94" => '&#8221;',
	"\x95" => '&#8226;',
	"\x96" => '&#8211;',
	"\x97" => '&#8212;',
	"\x98" => '&#732;',
	"\x99" => '&#8482;',
	"\x9A" => '&#65533;',
	"\x9B" => '&#8250;',
	"\x9C" => '&#65533;',
	"\x9D" => '&#65533;',
	"\x9E" => '&#65533;',
	"\x9F" => '&#65533;',
	"\xA0" => '&#160;',
	"\xA1" => '&#161;',
	"\xA2" => '&#162;',
	"\xA3" => '&#163;',
	"\xA4" => '&#8362;',
	"\xA5" => '&#165;',
	"\xA6" => '&#166;',
	"\xA7" => '&#167;',
	"\xA8" => '&#168;',
	"\xA9" => '&#169;',
	"\xAA" => '&#215;',
	"\xAB" => '&#171;',
	"\xAC" => '&#172;',
	"\xAD" => '&#173;',
	"\xAE" => '&#174;',
	"\xAF" => '&#175;',
	"\xB0" => '&#176;',
	"\xB1" => '&#177;',
	"\xB2" => '&#178;',
	"\xB3" => '&#179;',
	"\xB4" => '&#180;',
	"\xB5" => '&#181;',
	"\xB6" => '&#182;',
	"\xB7" => '&#183;',
	"\xB8" => '&#184;',
	"\xB9" => '&#185;',
	"\xBA" => '&#247;',
	"\xBB" => '&#187;',
	"\xBC" => '&#188;',
	"\xBD" => '&#189;',
	"\xBE" => '&#190;',
	"\xBF" => '&#191;',
	"\xC0" => '&#1456;',
	"\xC1" => '&#1457;',
	"\xC2" => '&#1458;',
	"\xC3" => '&#1459;',
	"\xC4" => '&#1460;',
	"\xC5" => '&#1461;',
	"\xC6" => '&#1462;',
	"\xC7" => '&#1463;',
	"\xC8" => '&#1464;',
	"\xC9" => '&#1465;',
	"\xCA" => '&#65533;',
	"\xCB" => '&#1467;',
	"\xCC" => '&#1468;',
	"\xCD" => '&#1469;',
	"\xCE" => '&#1470;',
	"\xCF" => '&#1471;',
	"\xD0" => '&#1472;',
	"\xD1" => '&#1473;',
	"\xD2" => '&#1474;',
	"\xD3" => '&#1475;',
	"\xD4" => '&#1520;',
	"\xD5" => '&#1521;',
	"\xD6" => '&#1522;',
	"\xD7" => '&#1523;',
	"\xD8" => '&#1524;',
	"\xD9" => '&#65533;',
	"\xDA" => '&#65533;',
	"\xDB" => '&#65533;',
	"\xDC" => '&#65533;',
	"\xDD" => '&#65533;',
	"\xDE" => '&#65533;',
	"\xDF" => '&#65533;',
	"\xE0" => '&#1488;',
	"\xE1" => '&#1489;',
	"\xE2" => '&#1490;',
	"\xE3" => '&#1491;',
	"\xE4" => '&#1492;',
	"\xE5" => '&#1493;',
	"\xE6" => '&#1494;',
	"\xE7" => '&#1495;',
	"\xE8" => '&#1496;',
	"\xE9" => '&#1497;',
	"\xEA" => '&#1498;',
	"\xEB" => '&#1499;',
	"\xEC" => '&#1500;',
	"\xED" => '&#1501;',
	"\xEE" => '&#1502;',
	"\xEF" => '&#1503;',
	"\xF0" => '&#1504;',
	"\xF1" => '&#1505;',
	"\xF2" => '&#1506;',
	"\xF3" => '&#1507;',
	"\xF4" => '&#1508;',
	"\xF5" => '&#1509;',
	"\xF6" => '&#1510;',
	"\xF7" => '&#1511;',
	"\xF8" => '&#1512;',
	"\xF9" => '&#1513;',
	"\xFA" => '&#1514;',
	"\xFB" => '&#65533;',
	"\xFC" => '&#65533;',
	"\xFD" => '&#8206;',
	"\xFE" => '&#8207;',
	"\xFF" => '&#65533;'
    );

    $string = str_replace(array_keys($cp1255), array_values($cp1255), $string);

    return $string;
}

?>
