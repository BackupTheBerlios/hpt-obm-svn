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
session_start();

$contacts = isset($_REQUEST['contacts']) ? $_REQUEST['contacts'] : array();
$companies = isset($_REQUEST['companies']) ? $_REQUEST['companies'] : array();
$users = isset($_REQUEST['users']) ? $_REQUEST['users'] : array();

if (isset($_REQUEST['clicked_value']) && $_REQUEST['clicked_value'] != '')
{
	switch($_REQUEST['clicked_type'])
	{
		case 'contact':
			$contacts[] = $_REQUEST['clicked_value'];
		break;

		case 'company':
			$companies[] = $_REQUEST['clicked_value'];
		break;

		case 'user':
			$users[] = $_REQUEST['clicked_value'];
		break;
	}
}

$string = '';
if (isset($_REQUEST['addresses']))
{
	for ($i=0;$i<sizeof($_REQUEST['addresses']);$i++)
	{
		if ($i != 0)
		{
			$string .= ", ";
		}
		$string .= $_REQUEST['addresses'][$i];
	}
}

if ($string != "")
{
   	$string .= ", ";
}

if (isset($_REQUEST['email']))
{
	$string .= $_REQUEST['email'];
}else
{
	if (isset($contacts))
	{
		for($i=0;$i<sizeof($contacts);$i++)
		{
			if (isset($addresses))
			{
				if (!in_array($contacts[$i],$addresses))
				{
					if (isset($first))
						$string .= ", ";
					else
						$first = true;

					$string .= $contacts[$i];
				}
			}else
			{
				if (isset($first))
					$string .= ", ";
				else
					$first = true;

				$string .= $contacts[$i];

			}
		}
	}
	if (isset($companies))
	{
		for($i=0;$i<sizeof($companies);$i++)
		{
			if (isset($_REQUEST['addresses']))
			{
				if (!in_array($companies[$i],$_REQUEST['addresses']))
				{
					if (isset($first))
						$string .= ", ";
					else
						$first = true;

					$string .= $companies[$i];
				}
			}else
			{
				if (isset($first))
					$string .= ", ";
				else
					$first = true;

				$string .= $companies[$i];

			}
		}
	}
	if (isset($users))
	{
		for($i=0;$i<sizeof($users);$i++)
		{
			if (isset($_REQUEST['addresses']))
			{
				if (!in_array($users[$i],$_REQUEST['addresses']))
				{
					if (isset($first))
						$string .= ", ";
					else
						$first = true;

					$string .= $users[$i];
				}
			}else
			{
				if (isset($first))
					$string .= ", ";
				else
					$first = true;

				$string .= $users[$i];

			}
		}
	}
}

unset($_SESSION['GO_HANDLER']);
?>
<html>
<body>
<script language="javascript" type="text/javascript">
        opener.<?php echo $_REQUEST['GO_FIELD']; ?>.value = '<?php echo $string; ?>';
        window.close();
</script>
</body>
</html>
