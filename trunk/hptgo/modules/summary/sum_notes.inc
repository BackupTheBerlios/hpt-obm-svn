<?php
$notes_module = $GO_MODULES->get_module('notes');
if ($notes_module && ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_read']) ||
		$GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_write'])))
{
	require_once($notes_module['class_path'].'notes.class.inc');
	$notes = new notes();

	echo 	'<table border="0" style="margin-top: 10px"><tr><td valign="top">'.
			'<img style="margin-right: 10px;" width="32" height="32" src="'.$GO_THEME->images['sum_notes'].
			'" border="0" /></td><td>'.
			'<h2><a href="'.$notes_module['url'].'">'.
			$lang_modules['notes'].'</a></h2></td></tr><tr><td></td><td>';

	if($notes->get_new_notes() > 0)
	{
		echo '<table border="0">';
		while($notes->next_record())
		{
			echo 	'<tr><td><a href="'.$notes_module['url'].
						'note.php?note_id='.$notes->f('id').'">'.$notes->f('name').
						'</a></td></tr>';
		}
		echo '</table>';
	}else
	{
		require($GO_LANGUAGE->get_language_file('notes'));
		echo $no_no_notes;
	}
	echo '</td></tr></table>';

}
?>