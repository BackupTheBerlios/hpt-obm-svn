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

require("../../Group-Office.php");

if(file_exists('phpsysinfo-dev/includes/lang/'.$GO_LANGUAGE->language['language_file'].'.php'))
{
	require('phpsysinfo-dev/includes/lang/'.$GO_LANGUAGE->language['language_file'].'.php');
}else
{
	require('phpsysinfo-dev/includes/lang/en.php');
}

require_once($GO_CONFIG->class_path."filesystem.class.inc");
require_once($GO_CONFIG->class_path."xml.class.inc");
$fso = new filesystem();

$return_to = $GO_CONFIG->host.'administrator/';

$GO_SECURITY->authenticate(true);


// Figure out which OS where running on, and detect support
if (file_exists('phpsysinfo-dev/includes/os/class.' . PHP_OS . '.inc.php')) {
  require('phpsysinfo-dev/includes/os/class.' . PHP_OS . '.inc.php');
  $sysinfo = new sysinfo;
} else {
  echo '<center><b>Error: ' . PHP_OS . ' is not currently supported</b></center>';
  exit;
}

if (!empty($GO_CONFIG->sensor_program)) {
  if (file_exists('phpsysinfo-dev/includes/mb/class.' . $GO_CONFIG->sensor_program. '.inc.php')) {
    require('phpsysinfo-dev/includes/mb/class.' . $GO_CONFIG->sensor_program . '.inc.php');
    $mbinfo = new mbinfo;
  } else {
    echo '<center><b>Error: ' . $GO_CONFIG->sensor_program . ' is not currently supported</b></center>';
    exit;
  }
}


require('phpsysinfo-dev/includes/common_functions.php'); // Set of common functions used through out the app
require('phpsysinfo-dev/includes/xml/vitals.php');
require('phpsysinfo-dev/includes/xml/network.php');
require('phpsysinfo-dev/includes/xml/hardware.php');
require('phpsysinfo-dev/includes/xml/memory.php');
require('phpsysinfo-dev/includes/xml/filesystems.php');
require('phpsysinfo-dev/includes/xml/mbinfo.php');

$xml = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n";
$xml .= "<!DOCTYPE phpsysinfo SYSTEM \"phpsysinfo-dev/phpsysinfo.dtd\">\n\n";
$xml .= created_by();
$xml .= "<phpsysinfo>\n";
$xml .= "  <Generation version=\"$VERSION\" timestamp=\"" . time() . "\"/>\n";
$xml .= xml_vitals();
$xml .= xml_network();
$xml .= xml_hardware();
$xml .= xml_memory();
$xml .= xml_filesystems();
if (!empty($GO_CONFIG->sensor_program)) {
  $xml .= xml_mbtemp();
  $xml .= xml_mbfans();
  $xml .= xml_mbvoltage();
} ;
$xml .= "</phpsysinfo>";

echo $xml;
$phpsysinfo = text_to_xml ($xml);

//var_dump($si_xml);
//$phpsysinfo = $si_xml->get_child ("phpsysinfo");

$page_title = $menu_sysinfo;
require($GO_THEME->theme_path."header.inc");



?>
<table border="0" cellpadding="0" cellspacing="10">
<tr>
<td valign="top">
<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $text['vitals']; ?></td>
</tr>
<tr>
 <Vitals>
    <Hostname>localhost.localdomain</Hostname>
    <IPAddr>127.0.0.1</IPAddr>
    <Kernel>2.6.8-1.521</Kernel>

    <Distro>Fedora Core release 2 (Tettnang)</Distro>
    <Distroicon>Fedora.gif</Distroicon>
    <Uptime>4  7 </Uptime>
    <Users>2</Users>
    <LoadAvg>0.49 0.55 0.40</LoadAvg>
  </Vitals>


<td class="TableInside">
<table border="0">
<tr>
<td><?php echo $si_hostname; ?>:</td>
<td><?php echo $phpsysinfo->get_data('Vitals/Hostname'); ?></td>
</tr>
<tr>
<td><?php echo $si_ip; ?>:</td>
<td><?php echo $phpsysinfo->get_data('Vitals/IPAddr'); ?></td>
</tr>
<tr>
<td><?php echo $si_kernel; ?>:</td>
<td><<?php echo $phpsysinfo->get_data('Vitals/Kernel'); ?></td>
</tr>
<tr>
	<td>
<tr>
<td><?php echo $si_web_server; ?>:</td>
<td><?php echo $_SERVER['SERVER_SOFTWARE']; ?></td>
</tr>
<tr>
<td><?php echo $si_php_version; ?>:</td>
<td><?php echo PHP_VERSION; ?> (<a class="normal" href="javascript:popup('phpinfo.php','','')"><?php echo $si_show_phpinfo; ?></a>)</td>
</tr>

<tr>
<td><?php echo $si_uptime; ?>:</td>
<td>
<?php

$min   = $sysinfo->uptime() / 60;
$hours = $min / 60;
$days  = floor($hours / 24);
$hours = floor($hours - ($days * 24));
$min   = floor($min - ($days * 60 * 24) - ($hours * 60));

echo $days.' '.$si_days.' ';
echo $hours.' '.$si_hours.' ';
echo $min.' '.$si_mins;
?>
</td>
</tr>
<tr>
<td><?php echo $si_users; ?>:</td>
<td><?php echo $sysinfo->users(); ?></td>
</tr>
<tr>
<td><?php echo $si_average_load; ?>:</td>
<td><?php echo $load_avg; ?></td>
</tr>
</table>
</td>
</tr>
</table>
</td>
</tr>
</table>
</td>
<td valign="top" rowspan="2">

<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $si_hardware_summary; ?></td>
</tr>
<tr>
<td class="TableInside">
<table border="0">
<tr>
<td><?php echo $si_processors; ?>:</td>
<td><?php echo $sys['cpus']; ?></td>
</tr>
<tr>
<td><?php echo $si_model; ?>:</td>
<td><?php echo $sys['model']; ?></td>
</tr>
<tr>
<td><?php echo $si_clock; ?>:</td>
<td><?php echo $sys['mhz']; ?></td>
</tr>
<tr>
<td><?php echo $si_buffersize; ?>:</td>
<td><?php echo $sys['cache']; ?></td>
</tr>
<tr>
<td><?php echo $si_bogomips; ?>:</td>
<td><?php echo $sys['bogomips']; ?></td>
</tr>
<tr>
<td valign="top"><?php echo $si_pci; ?>:</td>
<td>
<?php
$ar_buf = $sysinfo->pci();

if (count($ar_buf)) {
  for ($i=0;$i<sizeof($ar_buf);$i++) {
    if ($ar_buf[$i]) {
      echo chop($ar_buf[$i]).'<br />';
    }
  }
}

?>
</td>
</tr>
<tr>
<td valign="top"><?php echo $si_ide; ?>:</td>
<td>
<?php
$ar_buf = $sysinfo->ide();

ksort($ar_buf);

if (count($ar_buf)) {
  while (list($key, $value) = each($ar_buf)) {
    echo $key . ': ' . $ar_buf[$key]['model'];
    if (isset($ar_buf[$key]['capacity'])) {
      echo ' (' . $si_capacity . ': ' . format_size(($ar_buf[$key]['capacity']/2)*1024).')<br />';
    }else{
	  echo '<br />';
	}
  }
}

?>
</td>
</tr>
<tr>
<td valign="top"><?php echo $si_scsi; ?>:</td>
<td>
<?php
$ar_buf = $sysinfo->scsi();

if (count($ar_buf)) {
  for ($i=0;$i<sizeof($ar_buf);$i++) {
    echo $ar_buf[$i].'<br />';
  }
}
?>
</td>
</tr>
</table>
</td>
</tr>
</table>
</tr>

</table>
</td>
</tr>
<tr>
<td>
<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $si_network_use; ?></td>
</tr>
<tr>
<td class="TableInside">
<table border="0">
<tr>
<td><h3><?php echo $si_device; ?></h3></td>
<td align="right"><h3><?php echo $si_recieved; ?></h3></td>
<td align="right"><h3><?php echo $si_sent; ?></h3></td>
<td align="right"><h3><?php echo $si_error; ?></h3></td>
</tr>
<?php
$net = $sysinfo->network();

while (list($dev, $stats) = each($net)) {
  echo '<tr><td>'.trim($dev).'</td>';
  echo '<td align="right">'.format_size($stats['rx_bytes'], 2).'</td>';
  echo '<td align="right">'.format_size($stats['tx_bytes'], 2).'</td>';
  echo '<td align="right">'.$stats['errs'].' / '.$stats['drop'].'</td></tr>';
}
?>
</table>
</td>
</tr>
</table>
</td>
</tr>
</table>

</td>
</tr>

<tr>
<td colspan="2">

<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $si_memory_use; ?></td>
</tr>
<tr>
<td class="TableInside">
<table border="0" width="100%">
<tr>
<td><h3><?php echo $si_type; ?></h3></td>
<td><h3><?php echo $si_percentage_used; ?></h3></td>
<td align="right"><h3><?php echo $si_free; ?></h3></td>
<td align="right"><h3><?php echo $si_used; ?></h3></td>
<td align="right"><h3><?php echo $si_size; ?></h3></td>
</tr>
<?php
$mem = $sysinfo->memory();
echo '<tr><td valign="top">'.$si_physical.'</td>';
echo '<td valign="top">';
$statusbar = new statusbar;
$statusbar->info_text = $si_used;
$statusbar->turn_red_point = 90;
$statusbar->print_bar($mem['ram']['t_used'], $mem['ram']['total']);

echo '</td>';
echo '<td align="right" valign="top">'.format_size($mem['ram']['t_free']*1024, 2).'</td>';
echo '<td align="right" valign="top">'.format_size($mem['ram']['t_used']*1024, 2).'</td>';
echo '<td align="right" valign="top">'.format_size($mem['ram']['total']*1024, 2).'</td>';
echo '<tr><td valign="top">'.$si_swap.'</td>';
echo '<td valign="top">';

$statusbar = new statusbar;
$statusbar->info_text = $si_used;
$statusbar->turn_red_point = 90;
$statusbar->print_bar($mem['swap']['used'], $mem['swap']['total']);

echo '</td>';
echo '<td align="right" valign="top">'.format_size($mem['swap']['free']*1024, 2).'</td>';
echo '<td align="right" valign="top">'.format_size($mem['swap']['used']*1024, 2).'</td>';
echo '<td align="right" valign="top">'.format_size($mem['swap']['total']*1024, 2).'</td></tr>';
?>
</table>
</td>
</tr>
</table>
</td>
</tr>
</table>

</td>
</tr>

<tr>
<td colspan="2">

<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $si_filesystems; ?></td>
</tr>
<tr>
<td class="TableInside">
<table border="0" width="100%">
<tr>
<td><h3><?php echo $si_mount_point; ?></h3></td>
<td><h3><?php echo $si_type; ?></h3></td>
<td><h3><?php echo $si_partition; ?></h3></td>
<td><h3><?php echo $si_percentage_used; ?></h3></td>
<td align="right"><h3><?php echo $si_free; ?></h3></td>
<td align="right"><h3><?php echo $si_used; ?></h3></td>
<td align="right"><h3><?php echo $si_size; ?></h3></td>
</tr>
<?php
$fs = $sysinfo->filesystems();
$sum["size"] = 0;
$sum["used"] = 0;
$sum["free"] = 0;
for ($i=0; $i<sizeof($fs); $i++)
{
  $sum['size'] += $fs[$i]['size'];
  $sum['used'] += $fs[$i]['used'];
  $sum['free'] += $fs[$i]['free'];

  echo '<tr><td valign="top">'.$fs[$i]['mount'].'</td>';
  echo '<td valign="top">'.$fs[$i]['fstype'].'</td>';
  echo '<td valign="top">'.$fs[$i]['disk'].'</td>';
  echo '<td valign="top">';

  $statusbar = new statusbar;
  $statusbar->info_text = $si_used;
  $statusbar->turn_red_point = 90;
  $statusbar->print_bar($fs[$i]['used'], $fs[$i]['size']);

  echo '</td>';
  echo '<td align="right" valign="top">'.format_size($fs[$i]['free']*1024, 2).'</td>';
  echo '<td align="right" valign="top">'.format_size($fs[$i]['used']*1024, 2).'</td>';
  echo '<td align="right" valign="top">'.format_size($fs[$i]['size']*1024, 2).'</td></tr>';
}
echo '<tr><td valign="top" align="right" colspan="3"><i>'.$si_total.':</i></td>';
echo '<td valign="top">';

$statusbar = new statusbar;
$statusbar->info_text = $si_used;
$statusbar->turn_red_point = 90;
$statusbar->print_bar($sum['used'], $sum['size']);

echo '</td>';
echo '<td align="right" valign="top">'.format_size($sum['free']*1024, 2).'</td>';
echo '<td align="right" valign="top">'.format_size($sum['used']*1024, 2).'</td>';
echo '<td align="right" valign="top">'.format_size($sum['size']*1024, 2).'</td></tr>';
?>
</table>
</td>
</tr>
</table>
</td>
</tr>
</table>

</td>
</tr>
</table>
<br />
<?php
$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
require($GO_THEME->theme_path."footer.inc");
?>

