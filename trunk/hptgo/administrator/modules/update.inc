<?php
	$module_info = $GO_MODULES->get_module_info($module_id);
	if($module_info)
	{
		$installed_version = intval(str_replace('.', '', $GO_MODULES->modules[$module_id]['version']));
		$new_version = intval(str_replace('.', '',$module_info['version']));
		if($installed_version < $new_version)
		{
			if ($task != 'update')
			{
				echo "There are updates from {$GO_MODULES->modules[$module_id]['version']} to {$module_info['version']}. Proceed?";
?>
<script language="javascript">
function update()
{
  document.forms[0].task.value = 'update';
	document.forms[0].submit();
}
</script>
<?php
				echo '<br/><br/>';
				new button("Update","javascript:update();");
			}
			else
			{
				$db = new db();
				$update_file = $GO_CONFIG->root_path.'modules/'.$module_id.'/sql/'.$module_id.'.updates.inc';
			 	if(file_exists($update_file))
			 	{
			 		require($update_file);
			 		for ($cur_ver=$installed_version;$cur_ver<$new_version;$cur_ver++)
					{
						if (isset($updates[$cur_ver]))
						{
							echo "Update version $cur_ver<br/>";
							while($query = array_shift($updates[$cur_ver]))
							{
								echo "Query ".htmlspecialchars($query)."<br/>";
								@$db->query($query);
							}
						}
					}
				}
				else
				{
					echo 'updates file not exist. No update<br/>';
				}
				$db->query("UPDATE modules SET version='{$module_info['version']}' WHERE id='{$module_id}'");
				echo "Done!";
				echo '<br/><br/>';
				new button($cmdClose,"javascript:document.location='$return_to'");
			}
		}
		else
			echo "You have been already up to date";
			echo '<br/><br/>';
			new button($cmdClose,"javascript:document.location='$return_to'");
	}

?>
