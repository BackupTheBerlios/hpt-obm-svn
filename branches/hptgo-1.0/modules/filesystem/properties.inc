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

$return_to_path = urlencode(smartstrip($return_to_path));
$filename = basename($path);
$extension = get_extension($filename);
$type = $filetypes->get_type($extension);
$location = dirname($path);
$share = $fs->find_share($path);
//do not let user share it's home folder or modify the name!
$write_perms = ($fs->has_write_permission($GO_SECURITY->user_id, $path) && !$fs->is_home_path($GO_SECURITY->user_id, $path));
//do not let user rename common folder too!
$write_perms &= $path != $GO_CONFIG->file_storage_path.'common';

$return_to = $_SERVER['PHP_SELF'].'?path='.$return_to_path;
$link_back = $_SERVER['REQUEST_URI'];

echo '<input type="hidden" name="task" value="properties" />';
echo '<input type="hidden" name="close" value="false" />';

$tabtable = new tabtable('properties_tab', $strProperties.' - '.htmlspecialchars(basename($path)), '400', '400', '120', '', true);
$tabtable->add_tab('properties', $fbGeneral);

if ($share)
{
  $tabtable->add_tab('read_permissions', $strReadRights);
  $tabtable->add_tab('write_permissions', $strWriteRights);
}

$notes_module = $GO_MODULES->get_module('notes');
if ($notes_module)
{
  if ($GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_read']) || 
  	$GO_SECURITY->has_permission($GO_SECURITY->user_id, $notes_module['acl_write']))
  {
    $tabtable->add_tab('notes', $lang_modules['notes']);
  }
}
$tabtable->print_head();

switch($tabtable->get_active_tab_id())
{
  case 'read_permissions':
    print_acl($share['acl_read']);
    echo '<br /><br />&nbsp;&nbsp;&nbsp;&nbsp;';
    $button = new button($cmdClose, 'javascript:document.location=\''.$_SERVER['PHP_SELF'].'?path='.$return_to_path.'\';');
    break;

  case 'write_permissions':
    print_acl($share['acl_write']);
    echo '<br /><br />&nbsp;&nbsp;&nbsp;&nbsp;';
    $button = new button($cmdClose, 'javascript:document.location=\''.$_SERVER['PHP_SELF'].'?path='.$return_to_path.'\';');
    break;

  case 'notes':
    echo '<input type="hidden" name="sort_cookie_prefix" value="no_" />';
    require($GO_LANGUAGE->get_language_file('notes'));
    require_once($notes_module['class_path'].'notes.class.inc');
    $notes = new notes();
    $notes_module_url = $notes_module['url'];
    $link_back .= '?task=properties&active_tab='.$tabtable->get_active_tab_id();
    $file_path=$path;
    require($notes_module['path'].'notes.inc');
    echo '<br />';
    $button = new button($cmdClose, "javascript:document.location='".$return_to."'");
    break;

  default:

    ?>
      <table border="0" cellpadding="10">
      <tr>
      <td class="TableInside" colspan="99" width="100%" height="300" valign="top">
      <table border="0" cellpadding="4" cellspacing="0">
      <tr>
      <td colspan="2"><?php if(isset($feedback)) echo $feedback; ?>&nbsp;</td>
      </tr>
      <tr>
      <td>
      <?php echo $strName; ?>:
      </td>
      <td>
      <?php
      if ($write_perms)
      {
	echo '<input type="text" class="textbox" name="name" value="'.strip_extension(basename($path)).'" maxlength="100" size="30" />';
	echo '<input type="hidden" name="extension" value ="'.get_extension(basename($path)).'" />';
      }else
      {
	echo basename($path);
      }
      ?>
	</td>
	</tr>
	<tr>
	<td>
	<?php echo $fbLocation; ?>:
	</td>
	<td>
	<?php
	if (dirname($GO_CONFIG->file_storage_path).$GO_CONFIG->slash.basename($GO_CONFIG->file_storage_path) == $location)
	{
	  echo $GO_CONFIG->slash;
	}else
	{
	  echo str_replace($GO_CONFIG->file_storage_path,$GO_CONFIG->slash,$location);
	}
	?>
	  </td>
	  </tr>
	  <tr>
	  <td colspan="2"><hr /></td>
	  </tr>
	  <tr>
	  <td valign="top">
	  <?php echo $strType; ?>:
	  </td>
	  <td>
	  <table border="0" cellpadding="0" cellspacing="0">
	  <tr>
	  <td valign="top">
	  <?php
	  if (is_dir($path))
	  {
	    echo '<img border="0" width="16" height="16" src="'.$GO_THEME->images['folder'].'" />';
	    echo '&nbsp;</td><td valign="top">';
	    echo $fbFolder;
	  }else
	  {
	    echo '<img border="0" width="16" height="16" src="'.$GO_CONFIG->control_url.'icon.php?extension='.$extension.'" />';
	    echo '&nbsp;</td><td valign="top">';
	    echo $type['friendly'];
	    if (($type['mime'] != $type['friendly']) && $type['mime'] != '')
	    {
	      echo '<br />('.$type['mime'].')';
		  }
		  }
		  ?>
		  </td>
		  </tr>
		  </table>
		  </td>
		  </tr>
		  <tr>
		  <td><?php echo $strSize; ?>:</td>
		  <td><?php
		  if (is_dir($path) && !isset($_REQUEST['calc_size']))
		  {
		  echo '<a href="'.$_SERVER['PHP_SELF'].'?task=properties&path='.$urlencoded_path.'&calc_size=true" class="normal">'.$fs_calculate_folder_size.'</a>';
		  }else
		  {
		  echo format_size($fs->size($path));
		  }
		  ?></td>
		  </tr>
		  <tr>
		<td colspan="2"><hr /></td>
		</tr>
		<tr>
		<td>
		<?php echo $strCreated; ?>:
		</td>
		<td>
		<?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time(filectime($path))); ?>
		</td>
		</tr>
		<tr>
		<td>
		<?php echo $strAccessed; ?>:
		</td>
		<td>
		<?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time(fileatime($path))); ?>
		</td>
		</tr>
		<tr>
		<td>
		<?php echo $strModified; ?>:
		</td>
		<td>
		<?php echo date($_SESSION['GO_SESSION']['date_format'].' '.$_SESSION['GO_SESSION']['time_format'], get_time(filemtime($path))); ?>
		</td>
		</tr>
		<?php
		if (is_dir($path))
		{
		  echo '<tr><td colspan="2"><hr /></td></tr>';
		  $disabled = ($fs->is_owner($GO_SECURITY->user_id, $path) && $write_perms) ? false : true;

		  if ($fs->is_common_folder($path) && $GO_SECURITY->has_admin_permission($GO_SECURITY->user_id))
		  {
		    if ($path != $GO_CONFIG->file_storage_path.'common')
		    {
		      echo '<tr><td colspan="2">';
		      $checkbox = new checkbox('share_folder', $path, $fs_set_permissions, $share, $disabled);
		      if ($disabled)
		      {
			echo '<input type="hidden" name="share_folder" value="'.$path.'" />';
		      }
		      echo '</td></tr>';
		      if (!$share)
		      {
			$fs3 = new filesystem();
			$share = $fs3->find_share($path);
			$display_path = htmlspecialchars(str_replace($GO_CONFIG->file_storage_path,$GO_CONFIG->slash,$share['path']));
			echo '<tr><td colspan="2">';
			echo "$strUsePermissionsOf:&nbsp; ".$display_path;
			echo '</td></tr>';
		      }
		    }
		    else
		    {
		      echo '<input type="hidden" name="share_folder" value="'.$path.'" />';
		    }

		    echo '<tr><td colspan="2">';
		    $checkbox = new checkbox('system_folder', $path, $fs_system_folder, $fs->is_system_folder($path));
		    echo '</td></tr>';
		  }
		  else
		  {
		    echo '<tr><td colspan="2">';
		    $checkbox = new checkbox('share_folder', $path, $fs_activate_sharing, $share, $disabled);
		    if ($disabled)
		    {
		      echo '<input type="hidden" name="share_folder" value="'.$path.'" />';
		    }
		    echo '</td></tr>';
		  }
		}
	      ?>
		<tr>
		<td colspan="2" align="center">
		<br />
		<?php
		if ($write_perms)
		{
		  $button = new button($cmdOk, 'javascript:save_close()');
		  echo '&nbsp;&nbsp;';
		  $button = new button($cmdApply, 'javascript:document.forms[0].submit()');
		  echo '&nbsp;&nbsp;';
		}
	      $button = new button($cmdClose, 'javascript:document.location=\''.$_SERVER['PHP_SELF'].'?path='.$return_to_path.'\';');
	      ?>
		</td>
		</tr>
		</table>
		</td>
		</tr>
		</table>
		<?php
		break;
}
$tabtable->print_foot();
?>
  <script type="text/javascript">
function save_close()
{
  document.forms[0].close.value='true';
javascript:document.forms[0].submit();
}
</script>

