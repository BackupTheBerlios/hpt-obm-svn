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

if(!isset($template_item_id))
{
	$template_item_id = isset($_REQUEST['template_item_id']) ? $_REQUEST['template_item_id'] : 0;
}

if($task == 'template_item' || $task == 'replace_template_files')
{
  if ($template_item_id > 0 && $task != 'replace_template_files')
  {
    $template_item = $cms->get_template_item($template_item_id);
    $name = $template_item['name'];
    $content = $template_item['content'];
  }else
  {
    $name = isset($_POST['name']) ? smartstrip($_POST['name']) : '';
    $content = isset($_POST['content']) ? smartstrip($_POST['content']) : '';
  }
  if($task == 'replace_template_files')
  {
    $content = $cms->replace_template_files($template_id, smartstrip($content));
  }

  echo '<input type="hidden" name="template_item_id" value="'.$template_item_id.'" />';
  ?>
    <input type="hidden" name="close" value="false" />
    <input type="hidden" name="task" value="" />
    <table border="0" cellpadding="10" cellspacing="0">
    <tr>
    <td>
    <?php if(isset($feedback)) echo $feedback; ?>
    <?php echo $strName; ?>: <input type="text" value="<?php echo htmlspecialchars($name); ?>" name="name" class="textbox" size="30" /><br />
    <input type="file" class="textbox" name="content_file" />&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:popup('template_file_selector.php?template_id=<?php echo $template_id; ?>&SET_FIELD=content','250','300')" class="normal"><?php echo $cms_insert_file_url; ?></a>
    &nbsp;&nbsp;|&nbsp;&nbsp;<a href="javascript:replace_template_files()" class="normal"><?php echo $cms_replace_filenames; ?></a>

    <br />
    <textarea class="textbox" name="content" rows="25" style="width: 700px;"><?php echo htmlspecialchars($content); ?></textarea>
    <br />
    <?php
    $button = new button($cmdOk, "javascript:save('true')");
  echo '&nbsp;&nbsp;';
  $button = new button($cmdSave, "javascript:save('false');");
  echo '&nbsp;&nbsp;';
  $button = new button($cmdClose, "document.forms[0].submit()");
  ?>
    </td>
    </tr>
    </table>
    <script type="text/javascript">

    function save(close)
    {
      document.forms[0].task.value ='save_template_item';
      document.forms[0].close.value=close;
      document.forms[0].submit();
    }

  function replace_template_files()
  {
    document.forms[0].task.value = 'replace_template_files';
    document.forms[0].submit();
  }

  </script>
    <?php
}else
{
  if ($task == 'delete')
  {
    $cms->delete_template_item($template_item_id);
  }

  ?>
    <input type="hidden" name="template_item_id" value="0" />
    <input type="hidden" name="task" value="template_item" />
    <input type="hidden" name="close" value="false" />
    <table  border="0" cellspacing="5" cellpadding="0">
    <tr>
    <td>
    <a href="javascript:edit_template_item(0)" class="normal"><?php echo $cms_new_template_item; ?></a><br /><br />
    <?php
    if(isset($feedback)) echo $feedback;

  echo '<table border="0">';
  echo '<tr><td><h3>'.$cms_main_template.'</h3></td><td></td>';
  echo '<td><h3>'.$strName.'</h3></td>';
  echo '<td colspan="2">&nbsp;</td></tr>';

  $count = $cms->get_template_items($template_id);
  while($cms->next_record())
  {
    $check = ($cms->f('main') == 1) ? 'checked' : '';
    echo '<tr><td><input type="radio" name="main_template_item_id" onclick="javascript:save_main_template_item()" value="'.$cms->f('id').'" '.$check.' /></td>';
    echo '<td width="16" height="16"><img src="'.$GO_THEME->images['site'].'" border="0" widht="16" height="16" /></td>';
    echo '<td>'.$cms->f('name').'</td>';
    echo '<td><a href="javascript:edit_template_item('.$cms->f('id').')" title="'.$strEdit.' \''.htmlspecialchars($cms->f('name')).'\'"><img src="'.$GO_THEME->images['edit'].'" border="0" /></a></td>';
    echo "<td><a href='javascript:delete_template_item(".$cms->f('id').",\"".rawurlencode($strDeletePrefix."'".addslashes($cms->f('name'))."'".$strDeleteSuffix)."\")' title=\"".$strDeleteItem." '".htmlspecialchars($cms->f('name'))."'\"><img src=\"".$GO_THEME->images['delete']."\" border=\"0\"></a></td></tr>\n";
  }
  echo '<tr><td colspan="5"><i>'.$count.'&nbsp;'.$cms_template_items.'</i></td></tr>';
  echo '</table>';

  echo '<br />';
  $button = new button($cmdClose, "javascript:document.location='index.php?tabindex=2'");
  ?>
    </td>
    </tr>
    </table>
    <script type="text/javascript">
    function edit_template_item(template_item_id)
    {
      document.forms[0].template_item_id.value=template_item_id;
      document.forms[0].submit();
    }

  function save_main_template_item()
  {
    document.forms[0].task.value='save_main_template_item';
    document.forms[0].submit();
  }

  function delete_template_item(template_item_id, question)
  {
    if (confirm(question))
    {
      document.forms[0].task.value='delete';
      document.forms[0].template_item_id.value=template_item_id;
      document.forms[0].submit();
    }
  }
  </script>
    <?php
}
?>
