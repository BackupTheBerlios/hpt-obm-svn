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
$GO_SECURITY->authenticate();
$GO_MODULES->authenticate('addressbook');
require($GO_LANGUAGE->get_language_file('addressbook'));

$link_back = (isset($_REQUEST['link_back']) && $_REQUEST['link_back'] != '') ? $_REQUEST['link_back'] : $_SERVER['REQUEST_URI'];
$task = isset($_REQUEST['task']) ? $_REQUEST['task'] : '';
$return_to = (isset($_REQUEST['return_to']) && $_REQUEST['return_to'] != '') ? $_REQUEST['return_to'] : $_SERVER['HTTP_REFERER'];

//load contact management class
require($GO_MODULES->path."classes/addressbook.class.inc");
$ab = new addressbook();

switch($task)
{
	case 'delete_addressbook':
		$delete_ab = $ab->get_addressbook($_POST['delete_addressbook_id']);

		if($GO_SECURITY->user_id == $delete_ab['user_id'])
		{
			$default_id = $ab->get_default_addressbook($GO_SECURITY->user_id);
			if ($ab->delete_addressbook($_POST['delete_addressbook_id']))
			{
				$GO_SECURITY->delete_acl($delete_ab['acl_write']);
				$GO_SECURITY->delete_acl($delete_ab['acl_read']);
			}

			$ab->get_subscribed_addressbooks($GO_SECURITY->user_id);
			if ($ab->next_record())
			{
				$next_id = $ab->f('id');
				if ($_POST['delete_addressbook_id'] == $default_id)
				{
					$ab->set_default_addressbook($GO_SECURITY->user_id, $next_id);
				}
				if ($_POST['addressbook_id'] = $_POST['delete_addressbook_id'])
				{
					$_POST['addressbook_id'] = $next_id;
				}
			}else
			{
				unset($addressbook_id);
			}

		}
		$post_action = 'addressbooks';
	break;

	case 'subscribe':
		$ab->unsubscribe_all($GO_SECURITY->user_id);
		if(isset($_POST['subscribed']))
		{
			for ($i=0;$i<sizeof($_POST['subscribed']);$i++)
			{
				$ab->subscribe($GO_SECURITY->user_id, $_POST['subscribed'][$i]);
			}
		}

		if (!$ab->is_subscribed($GO_SECURITY->user_id, $_POST['default_addressbook_id']))
		{
			$ab->subscribe($GO_SECURITY->user_id, $_POST['default_addressbook_id']);
		}
		$ab->set_default_addressbook($GO_SECURITY->user_id, $_POST['default_addressbook_id']);
		if ($_POST['close'] == 'true')
		{
			header('Location: '.$return_to);
			exit();
		}
	break;
}

require($GO_THEME->theme_path."header.inc");
?>
<form name="contacts" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
<input type="hidden" name="delete_addressbook_id" />
<input type="hidden" name="task" />
<input type="hidden" name="close" value="false" />
<input type="hidden" name="return_to" value="<?php echo $return_to; ?>" />
<input type="hidden" name="link_back" value="<?php echo $link_back; ?>" />

<?php
$tabtable = new tabtable('addressbooks', $ab_addressbooks, '100%', '400', '100');
$tabtable->print_head();
?>

<table border="0" cellpadding="10">
<tr>
	<td>
	<table border="0" cellpadding="3" cellspacing="0">
	<?php
	if (isset($feedback))
	{
		echo '<tr><td colspan="6" height="25">'.$feedback.'</td></tr>';
	}

	$default_ab = $ab->get_default_addressbook($GO_SECURITY->user_id);
	echo '<tr height="30"><td colspan="6"><a href="addressbook.php?return_to='.urlencode($link_back).'" class="normal">'.$ab_new_ab.'</a></td></tr>';
	echo '<tr><td><h3>'.$ab_default.'</h3></td><td><h3>'.$strName.'</h3></td><td><h3>'.$ab_owner.'</h3></td><td><h3>'.$ab_subscribed.'</h3></td><td>&nbsp;</td><td>&nbsp;</td></tr>';
	$ab_count = $ab->get_user_addressbooks($GO_SECURITY->user_id);
	$ab1 = new addressbook();

	if ($ab_count > 0)
	{
		while ($ab->next_record())
		{
			if($ab1->is_subscribed($GO_SECURITY->user_id, $ab->f("id")))
			{
				$checked = 'checked';
			}else
			{
				$checked = '';
			}

			$check = ($ab->f('id') == $default_ab) ? 'checked' : '';

			echo '<tr><td><input type="radio" name="default_addressbook_id" value="'.$ab->f("id").'" '.$check.' /></td>';
			echo '<td nowrap><a href="index.php?post_action=browse&addressbook_id='.$ab->f("id").'" class="normal">'.htmlspecialchars($ab->f("name")).'</a>&nbsp;</td>';
			echo '<td nowrap>'.show_profile($ab->f("user_id")).'&nbsp;</td>';
			echo '<td align="center">&nbsp;<input type="checkbox" name="subscribed[]" value="'.$ab->f("id").'" '.$checked.' /></td>';
			echo '<td>&nbsp;<a href="addressbook.php?addressbook_id='.$ab->f("id").
						'&return_to='.urlencode($link_back).'" title="'.$strEdit.' \''.
						htmlspecialchars($ab->f("name")).'\'"><img src="'.
						$GO_THEME->images['edit'].'" border="0" /></a></td>';
			if ($ab->f('user_id') == $GO_SECURITY->user_id)
				echo "<td>&nbsp;<a href='javascript:delete_addressbook(\"".$ab->f("id").
						"\",\"".rawurlencode($strDeletePrefix."'".
						addslashes($ab->f("name"))."'".$strDeleteSuffix).
						"\")' title=\"".$strDeleteItem." '".
						htmlspecialchars($ab->f("name"))."'\"><img src=\"".
						$GO_THEME->images['delete']."\" border=\"0\"></a></td>";
			echo "</tr>\n";
		}
	}
	echo '</table><br />';
	$button = new button($cmdOk, "javascript:_save('subscribe', 'true')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdApply, "javascript:_save('subscribe', 'false')");
	echo '&nbsp;&nbsp;';
	$button = new button($cmdClose, "javascript:document.location='".$return_to."'");
	?>
	</td>
</tr>
</table>
<?php
$tabtable->print_foot();
?>
</form>
<script type="text/javascript" language="javascript">
function delete_addressbook(addressbook_id, message)
{
	if (confirm(unescape(message)))
	{
		document.forms[0].delete_addressbook_id.value = addressbook_id;
		document.forms[0].task.value='delete_addressbook';
		document.forms[0].submit();
	}
}

function _save(task, close)
{
	document.forms[0].task.value = task;
	document.forms[0].close.value = close;
	document.forms[0].submit();
}

</script>

<?php
require($GO_THEME->theme_path."footer.inc");
?>
