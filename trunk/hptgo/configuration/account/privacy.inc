<?php
/*
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */
?>
<br />
<?php
if ($task == 'authorize')
{
	if ($profile)
	{
		if ($user = $GO_USERS->get_profile_by_email($_POST['auth_email_address']))
		{
			$mail_body  = '<html><body>';

			$middle_name = $user['middle_name'] == '' ? '' : $user['middle_name'].' ';
			$user_name = $middle_name.$user['last_name'];

			$middle_name = $profile['middle_name'] == '' ? '' : $profile['middle_name'].' ';
			$profile_name = $profile['last_name'].' '.$middle_name.$profile['first_name'];

			$mail_body .= $ac_salutation.' '.$sir_madam[$user['sex']].' '.$user_name.',<br /><br />';
			$mail_body .= $profile_name.' '.$ac_request_auth.'<br /><br />';

			$mail_body .= '<a href="'.$GO_CONFIG->full_url.'configuration/account/index.php?requesting_user_id='.$GO_SECURITY->user_id.'&authcode='.$profile['authcode'].'&requested_user_id='.$user['id'].'&task=accept'.'">'.$ac_accept.'</a>&nbsp;&nbsp;&nbsp;';
			$mail_body .= '<a href="'.$GO_CONFIG->full_url.'configuration/account/index.php?requesting_user_id='.$GO_SECURITY->user_id.'&authcode='.$profile['authcode'].'&requested_user_id='.$user['id'].'&task=decline'.'">'.$ac_decline.'</a>&nbsp;&nbsp;&nbsp;';
			$mail_body .= '</body></html>';
			if(!sendmail($user['email'], $profile['email'], $GO_CONFIG->title, $ac_request_auth_mail_title, $mail_body,'3 (Normal)', 'text/html'))
			{
				$feedback = '<p class="Error">'.$strEmailFail.'</p>';
				$task = 'add_user';
			}

		}else
		{
			$feedback = '<p class="Error">'.$ac_invalid_email.'</p>';
			$task = 'add_user';
		}
	}else
	{
		$feedback = '<p class="Error">'.$strDataError.'</p>';
		$task = 'add_user';
	}
}

if (isset($feedback))
{
	echo $feedback;
}

switch($task)
{
	case 'add_user':
		echo '<table border="0"><tr><td colspan="2">'.$ac_auth_text.'<br /><br /></td></tr><tr><td>'.$strEmail.':</td>';
		echo '<td><input type="text" class="textbox" name="auth_email_address"  maxlength="100" size="50" /></td></tr>';
		echo '<tr><td colspan="2"><br />';
		$button = new button($cmdOk, "javascript:send_request()");
		echo '&nbsp;&nbsp;';
		$button = new button($cmdCancel, "javascript:document.forms[0].task.value='privacy';document.forms[0].submit()");
		echo '</td></tr></table>';

		echo '
		<script type="text/javascript">

		function send_request()
		{
			document.forms[0].task.value="authorize";
			document.forms[0].submit();
		}

		var nav4 = window.Event ? true : false;
		function processkeypress(e)
		{
			if(nav4)
			{
				var whichCode = e.which;
			}else
			{
				var whichCode = event.keyCode;
			}

			if (whichCode == 13)
			{
				send_request();
				return true;
			}
		}
		if (window.Event) //if Navigator 4.X
		{
			document.captureEvents(Event.KEYPRESS)
		}
		document.onkeypress = processkeypress;
		document.forms[0].auth_email_address.focus();
		</script>';
	break;

	case 'authorize':
		echo $ac_request_mail_body_prefix.' '.$user['first_name'].' '.$user_name.' '.$ac_request_mail_body_suffix.'<br /><br />';
		$button = new button($cmdContinue, "javascript:document.forms[0].task.value='privacy';document.forms[0].submit()");
	break;


	default:
		echo '<a href="javascript:document.forms[0].task.value=\'add_user\';document.forms[0].submit();" class="normal">'.$ac_add_user.'</a><br /><br />';
		echo $privacy_text."<br /><br />";

		print_acl($profile['acl_id']);
		echo '<br />';
		$button = new button($cmdClose, "javascript:document.location='".$return_to ."'");
	break;
}
?>
