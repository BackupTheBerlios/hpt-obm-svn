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
require($GO_LANGUAGE->get_base_language_file('sysinfo'));
require($GO_CONFIG->class_path."sysinfo.class.inc");
require_once($GO_CONFIG->class_path."filesystem.class.inc");
$fso = new filesystem();
$sysinfo = new sysinfo();

$return_to = $GO_CONFIG->host.'configuration/';

$GO_SECURITY->authenticate(true);
$page_title = $menu_sysinfo;
require($GO_THEME->theme_path."header.inc");


//average load
$ar_buf = $sysinfo->loadavg();
$load_avg = '';
for ($i=0;$i<3;$i++) {
  if ($ar_buf[$i] > 2) {
    $load_avg .= ' ';
  } else {
    $load_avg .= $ar_buf[$i] . ' ';
  }
}
$load_avg = trim($load_avg);

//cpu information
$sys = $sysinfo->cpu_info();

?>
<table border="0" cellpadding="0" cellspacing="10">
<tr>
<td valign="top">
<table border="0" cellpadding="0" cellspacing="0" class="TableBorder" width="100%">
<tr>
<td valign="top">
<table border="0" cellpadding="1" cellspacing="1" width="100%">
<tr>
<td colspan="99" class="TableHead"><?php echo $si_system_summary; ?></td>
</tr>
<tr>
<td class="TableInside">
<table border="0">
<tr>
<td><?php echo $si_hostname; ?>:</td>
<td><?php echo $sysinfo->chostname(); ?></td>
</tr>
<tr>
<td><?php echo $si_ip; ?>:</td>
<td><?php echo $sysinfo->ip_addr(); ?></td>
</tr>
<tr>
<td><?php echo $si_kernel; ?>:</td>
<td><?php echo $sysinfo->kernel(); ?></td>
</tr>
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

