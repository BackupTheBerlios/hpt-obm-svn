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

if (isset($_REQUEST['delete_type']))
{
	$filetypes->delete_type($_REQUEST['delete_type']);
}

$max_rows = isset($_REQUEST['max_rows']) ? $_REQUEST['max_rows'] : $_SESSION['GO_SESSION']['max_rows_list'];
$first = isset($_REQUEST['first']) ? $_REQUEST['first'] : 0;
?>

<table border="0" cellpadding="10" cellspacing="0" width="100%">
<tr>
        <td>
        <?php
        $count = $filetypes->get_filetypes($first, $max_rows);

        if ($count>0)
        {
			?>
			<h1><?php echo $ft_title; ?></h1>
			<table border="0" cellspacing="0" cellpadding="1" width="600">

			<tr height="16">
				<td class="TableHead2" width="16">
				</td>
				<td class="TableHead2">
				<?php echo $ft_extension; ?>
				</td>
				<td class="TableHead2">
				<?php echo $ft_content; ?>
				</td>
				<td class="TableHead2">
				<?php echo $ft_friendly; ?>
				</td>
				<td class="TableHead2">&nbsp;</td>
				<td class="TableHead2">&nbsp;</td>
			</tr>
			<?php
			while ($filetypes->next_record())
			{
				echo '<tr><td><img src="'.$GO_CONFIG->control_url.'icon.php?extension='.$filetypes->f("extension").'" border="0" height="16" width="16" /></td>';
				echo '<td>'.$filetypes->f("extension").'</td>';
				echo '<td>'.$filetypes->f("mime").'</td>';
				echo '<td>'.$filetypes->f("friendly").'</td>';
				echo '<td><a href="'.$_SERVER['PHP_SELF'].'?task=filetype&extension='.$filetypes->f("extension").'&first='.$first.'" title="'.$strEdit.' '.$ft_friendly.'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
				echo "<td><a href='javascript:confirm_action(\"".$_SERVER['PHP_SELF']."?delete_type=".$filetypes->f("extension")."&first=".$first."\",\"".rawurlencode($strDeletePrefix."'".$filetypes->f("extension")."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".$filetypes->f("extension")."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
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
				}
				echo '<tr><td colspan="99" height="20">'.$links.'</td></tr>';
				echo '<tr><td colspan="99" height="1"><img src="'.$GO_THEME->images['cccccc'].'" border="0" height="1" width="100%" /></td></tr>';
			}
			echo '</table>';
		}else
		{
			echo '<p align="center">'.$ft_no_filetypes.'</p>';
		}
        ?>
        </td>
</tr>
</table>
<br />
<?php
$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
?>

