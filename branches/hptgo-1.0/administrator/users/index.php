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
require($GO_LANGUAGE->get_base_language_file('users'));

$GO_SECURITY->authenticate(true);

$return_to = $GO_CONFIG->host.'configuration/';

if (isset($_REQUEST['delete_user']))
{
  if (($_REQUEST['delete_user'] != $GO_SECURITY->user_id) && ($_REQUEST['delete_user'] != 1))
  {
    $GO_USERS->delete_user($_REQUEST['delete_user']);
  }else
  {
    $feedback = '<p class="Error">'.$delete_fail.'</p>';
  }
}

$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;

//remember sorting in cookie
if (isset($_REQUEST['newsort']))
{
  SetCookie("admin_sort",$_REQUEST['newsort'],time()+3600*24*365,"/","",0);
  $_COOKIE['admin_sort'] = $_REQUEST['newsort'];
}
if (isset($_REQUEST['newdirection']))
{
  SetCookie("admin_direction",$_REQUEST['newdirection'],time()+3600*24*365,"/","",0);
  $_COOKIE['admin_direction'] = $_REQUEST['newdirection'];
}

$admin_sort = isset($_COOKIE['admin_sort']) ? $_COOKIE['admin_sort'] : 'name';
$admin_direction = isset($_COOKIE['admin_direction']) ? $_COOKIE['admin_direction'] : 'ASC';

if ($admin_direction == "DESC")
{
  $image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_down'].'" border="0" />';
  $newdirection = "ASC";
}else
{
  $image_string = '&nbsp;<img src="'.$GO_THEME->images['arrow_up'].'" border="0" />';
  $newdirection = "DESC";
}

$page_title = $menu_users;
require($GO_THEME->theme_path."header.inc");


$count = $GO_USERS->get_users($admin_sort, $admin_direction, $first, $max_rows);

echo "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">\n";

echo '<tr><td colspan="99"><h1>'.$menu_users.'</h1>';
if (isset($feedback)) echo $feedback;
echo '</td></tr>';
if ($GO_CONFIG->max_users == 0 || ($count < $GO_CONFIG->max_users))
  echo '<tr><td colspan="99" align="right"><a href="register.php" class="normal">'.$admin_new_user.'</a><br /><br /></td></tr>';

  echo '<tr><td colspan="99" align="right" class="small">'.$count.' '.$strUsers;
if ($GO_CONFIG->max_users != 0)
  echo ' '.$strMaxOf.' '.$GO_CONFIG->max_users;

  echo '</td></tr>';
  echo "<tr>";
  echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?newsort=name&newdirection=".$newdirection."\">".$strName;
  if ($admin_sort == "name")
  echo $image_string;
  echo "</a></td>\n";
  echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?newsort=company&newdirection=".$newdirection."\">".$strCompany;
  if ($admin_sort == "company")
  echo $image_string;
  echo "</a></td>\n";
  echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?newsort=logins&newdirection=".$newdirection."\">".$strLogins;
  if ($admin_sort == "logins")
  echo $image_string;
  echo "</a></td>\n";
  echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?newsort=lastlogin&newdirection=".$newdirection."\">".$ac_lastlogin;
  if ($admin_sort == "lastlogin")
  echo $image_string;
  echo "</a></td>\n";
  echo "<td class=\"TableHead2\"><a class=\"TableHead2\" href=\"".$_SERVER['PHP_SELF']."?newsort=registration_time&newdirection=".$newdirection."\">".$strRegistrationDate;
  if ($admin_sort == "registration_time")
  echo $image_string;
  echo "</a></td>\n";
  echo "<td class=\"TableHead2\">&nbsp;</td>\n";
  echo "</tr>\n";

while ($GO_USERS->next_record())
{
  $middle_name = $GO_USERS->f('middle_name') == '' ? '' : $GO_USERS->f('middle_name').' ';
//  $name = $GO_USERS->f('first_name').' '.$middle_name.$GO_USERS->f('last_name');
  $name = $GO_USERS->f('last_name').' '.$middle_name.$GO_USERS->f('first_name');
  echo '<tr height="18"><td><a class="normal" href="edit_user.php?id='.$GO_USERS->f("id").'" title="'.$strEdit.' '.$name.'">'.$name.'</a>&nbsp;</td>';
  echo '<td>'.empty_to_stripe($GO_USERS->f("company")).'&nbsp;</td>';
  echo '<td>'.number_format($GO_USERS->f("logins"), 0, $_SESSION['GO_SESSION']['decimal_seperator'], $_SESSION['GO_SESSION']['thousands_seperator']).'&nbsp;</td>';
  echo '<td>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], $GO_USERS->f("lastlogin")+($_SESSION['GO_SESSION']['timezone']*3600)).'&nbsp;&nbsp;&nbsp;</td>';
  echo '<td>'.date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'],$GO_USERS->f("registration_time")+($_SESSION['GO_SESSION']['timezone']*3600)).'</td>';
  echo "<td>&nbsp;<a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?delete_user=".$GO_USERS->f("id")."\",\"".rawurlencode($strDeletePrefix."'".$name."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$name."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td>";
  echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
}


$links = '';
$max_links=10;
if ($max_rows != 0)
{
  if ($count > $max_rows)
  {
    $links = '<table border="0" cellpadding="0" cellspacing="0" width="100%"><tr><td>';
    $next_start = $first+$max_rows;
    $previous_start = $first-$max_rows;
    if ($first != 0)
    {
      $links .= '<a href="'.$_SERVER['PHP_SELF'].'?first=0">&lt&lt</a>&nbsp;';
      $links .= '<a href="'.$_SERVER['PHP_SELF'].'?first='.$previous_start.'">'.$cmdPrevious.'</a>&nbsp;';
    }else
    {
      $links .= '<font color="#cccccc">&lt&lt '.$cmdPrevious.'</font>&nbsp;';
    }

    $start = ($first-(($max_links/2)*$max_rows));

    $end = ($first+(($max_links/2)*$max_rows));

    if ($start < 0)
    {
      $end = $end - $start;
      $start=0;
    }
    if ($end > $count)
    {
      $end = $count;
    }
    if ($start > 0)
    {
      $links .= '...&nbsp;';
    }

    for ($i=$start;$i<$end;$i+=$max_rows)
    {
      $page = ($i/$max_rows)+1;
      if ($i==$first)
      {
	$links .= '<b><i>'.$page.'</i></b>&nbsp;';
      }else
      {
	$links .= '<a href="'.$_SERVER['PHP_SELF'].'?first='.$i.'">'.$page.'</a>&nbsp;';
      }
    }

    if ($end < $count)
    {
      $links .= '...&nbsp;';
    }

    $last_page = floor($count/$max_rows)*$max_rows;

    if ($count > $next_start)
    {
      $links .= '<a href="'.$_SERVER['PHP_SELF'].'?first='.$next_start.'">'.$cmdNext.'</a>&nbsp;';
      $links .= '<a href="'.$_SERVER['PHP_SELF'].'?first='.$last_page.'">&gt&gt</a>';
    }else
    {
      $links .= '<font color="#cccccc">'.$cmdNext.' &gt&gt</font>';
    }
    $links .= '</td><td align="right"><a class="normal" href="'.$_SERVER['PHP_SELF'].'?max_rows=0">'.$cmdShowAll.'</a></td></tr></table>';
    echo '<tr><td colspan="99" height="20">'.$links.'</td></tr>';
    echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
  }
}
echo '</table>';

echo '<br />';
$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
require($GO_THEME->theme_path."footer.inc");
?>
