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

require($GO_LANGUAGE->get_language_file('squirrelmail'));

$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('squirrelmail');

require($GO_MODULES->class_path."email.class.inc");
$email = new email();

$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$account_id = isset($_REQUEST['account_id']) ? $_REQUEST['account_id'] : 0;
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];
$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];

require($GO_THEME->theme_path."header.inc");

echo '<form method="POST" action="'.$_SERVER['PHP_SELF'].'" name="email_client">';
echo '<input type="hidden" name="task" value="" />';
echo '<input type="hidden" name="account_id" value="'.$account_id.'" />';
echo '<input type="hidden" name="close" value="false" />';
echo '<input type="hidden" name="return_to" value="'.$return_to.'" />';
echo '<input type="hidden" name="link_back" value="'.$link_back.'" />';

$tabtable = new tabtable('filters_list', $ml_filters, '600', '300', '100', '', true);
$tabtable->print_head();


echo '<table border="0" cellpadding="0" cellspacing="8" class="normal"><tr><td>';

if (isset($_REQUEST['delete_filter']))
{
	$email->delete_filter($_REQUEST['delete_filter']);
}

if (isset($_REQUEST['move_up_id']) && $_REQUEST['move_dn_id'] != 0)
{
	$email->move_up($_REQUEST['move_up_id'], $_REQUEST['move_dn_id'], $_REQUEST['move_up_pr'], $_REQUEST['move_dn_pr']);
}

if ($email->get_all_folders($account_id, true) > 0)
{
	echo '<a class="normal" href="filter.php?id='.$account_id.'">'.$cmdAdd.'</a><br /><br />';
	$count = $email->get_filters($account_id);

	if ($count>0)
	{
		echo '<table border="0" cellpadding="2" cellspacing="0"><tr>';
		echo '<td><h3>'.$ml_field.'</h3></td>';
		echo '<td><h3>'.$ml_contains.'</h3></td>';
		echo '<td><h3>'.$ml_folder.'</h3></td>';
		echo '<td>&nbsp;</td>';
		echo '<td>&nbsp;</td>';
		echo '<td>&nbsp;</td></tr>';

		$last_id  = 0;
		$last_pr = 0;

		while($email->next_record())
		{
			switch($email->f("field"))
			{
				case "sender":
					$field = "E-mail";
				break;

				case "subject":
					$field = $ml_subject;
				break;

				case "to";
					$field = $ml_to;
				break;

				default:
					$field = $email->f("field");
				break;
			}

			echo '<tr height="18"><td>'.$field.'&nbsp;&nbsp;</td>';
			echo '<td>'.$email->f("keyword").'&nbsp;&nbsp;</td>';
			echo '<td>'.$email->f("folder").'</td>';
			echo '<td>&nbsp;<a href="'.$_SERVER['PHP_SELF'].'?account_id='.
						$account_id.'&return_to='.urlencode($return_to).'&move_up_id='.
						$email->f("id").'&move_dn_id='.$last_id.'&move_dn_pr='.
						$email->f("priority").'&move_up_pr='.$last_pr.'" title="'.
						$ml_move_up.'"><img src="'.$GO_THEME->images['up'].
						'" border="0"></a></td>';

			echo "<td>&nbsp;<a href='javascript:div_confirm_action(\"".
						$_SERVER['PHP_SELF']."?account_id=".$account_id."&return_to=".
						urlencode($return_to)."&delete_filter=".$email->f("id")."\",\"".
						div_confirm_id($ml_delete_filter)."\")' title=\"".$ml_delete_filter1.
						"\"><img src=\"".$GO_THEME->images['delete'].
						"\" border=\"0\"></a></td>";

			echo '</tr>';
			$last_id = $email->f("id");
			$last_pr = $email->f("priority");
		}
		echo "</table>";
	}else
	{
		echo $ml_no_filters;
	}
}else
{
	echo $ml_no_folders;
}

?>
		<br />
		<?php
		$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
		?>
        </td>
</tr>
</table>
<script type="text/javascript" language="javascript">

function _save(task, close)
{
	document.forms[0].task.value = task;
	document.forms[0].close.value = close;
	document.forms[0].submit();
}
</script>

<?php
$tabtable->print_foot();
echo '</form>';
require($GO_THEME->theme_path."footer.inc");
?>
