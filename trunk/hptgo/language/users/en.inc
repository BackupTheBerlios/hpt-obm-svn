<?php
//Uncomment this line in new translations!
//require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Logins";
$strRegistrationDate = "Registration Date";
$strUsers = "user(s)";
$user_profile = "User Profile";
$ac_login_info = "Login information";
$ac_registration_time = "Registration time";
$ac_lastlogin = "Last login";
$ac_logins = "Total logins";
$strMaxOf = "registered from the maximum of";
$strUsername = "Username";
$delete_fail = "You can't delete yourself and the superuser.";
$admin_new_user = "New user";

$admin_modules = 'Modules';
$admin_module_access = 'This user has access to the following modules:';

$admin_password = 'Password';
$admin_confirm_password = 'Confirm password';
$admin_change_password = 'Change password';
$admin_pass_match = 'The passwords didn\'t match. Please try again.';

//registration
$registration_personal_info = "Personal information";
$registration_account_info = "Account information";
$registartion_username_text = "Enter a username and password for this user to login to the site.";

$registration_question_text=<<<registration_question_text
Enter a secret question to which only this user knows the answer to. We will ask this question in case the user forgets his or her password.
When the correct answer is given, an e-mail will be send to the registered e-mail address with the username and a new password to log in.
registration_question_text;

$registration_text=<<<registration_text
Fill out this form and click on 'Ok' to register. The fields marked with a * are required.<br />
The information you provide is only for use at Group-Office so we will not provide them to third parties.
registration_text;

$registartion_authcode_text = "Enter an authorisation code. The authorisationcode is a code that other users will need to add this user to thier contactlist and view your details.";
$registartion_confirm_authcode = "Confirm authorisation code";
$registration_visibility = "Make this user visible to all users.";
$registration_success = "<h1>Congratulations!</h1>You are successfully registered and a welcome message has been sent to:";
$registration_failure = "An error occured while trying to register your information. Please try again.";
$registration_mail_subject = "Welcome to HPT OPM Group-Office.";
$registration_folder_error = "Failed creating the personal folder. Contact us to restore this!";
$registration_title = "Register at Group-Office";

$registration_email = "Create an E-mail address for this user at Group-Office.";
$registration_samba = "File & Print server.";
$registration_email_error = "An error occured while creating your e-mail account. Contact us for support.";
$strPasswordConfirm = "Confirm password";
$strPassword = "Password";

$reg_company_info = "Company information";

$max_user_limit = "Maximum ammount of users reached.";
$max_users_text = "Sorry, you can't register more users. Contact us if you want to increase your number of users.";
$ml_sent_items = "Sent items";
$ml_spam = "Spam";
$ac_samba_user = "User has acces to the file and print server.";
$ml_email_only = "Only required when you don't create an E-mail address";
$register_new_user = 'New Group-Office subscriber';
$register_thanks = '<h2>Thank you</h2>Your subscription will be processed as soon as possible. You will recieve further notice.';
$register_accept = 'Add to users';
$register_addressbook = 'Add to addressbook';
$register_title = 'Subscribe';
$registration_mail_body=<<<registration_mail_body
Dear %sex% %last_name%,

You have been registered at Group-Office.
We hope you will enjoy our service.

You can login with the following data:

Username: %username%
Password: %password%

Click on the link below to login:

registration_mail_body;

//1.98
$reg_language = 'Language';
$reg_theme = 'Theme';

//2.05
$admin_use = 'Use';
$admin_module = 'Module';
$admin_manage = 'Manage';

$admin_groups = 'User groups';
$admin_groups_user = 'User is member of the following user groups';
?>
