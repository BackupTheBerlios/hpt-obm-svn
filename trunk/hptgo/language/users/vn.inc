<?php
//Uncomment this line in new translations!
//require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Số lần đăng nhập";
$strRegistrationDate = "Ngày đăng ký";
$strUsers = "người";
$user_profile = "Thông tin người dùng";
$ac_login_info = "Thông tin đăng nhập";
$ac_registration_time = "Thời điểm đăng ký";
$ac_lastlogin = "Lần đăng nhập cuối";
$ac_logins = "Tổng số lần đăng nhập";
$strMaxOf = "registered from the maximum of";
$strUsername = "Username";
$delete_fail = "You can't delete yourself and the superuser.";
$admin_new_user = "Thêm mới";

$admin_modules = 'Modules';
$admin_module_access = 'This user has access to the following modules:';

$admin_password = 'Mật khẩu';
$admin_confirm_password = 'Xác nhận mật khẩu';
$admin_change_password = 'Thay đổi mật khẩu';
$admin_pass_match = 'Mật khẩu không trùng. Xin thử lại.';

//registration
$registration_personal_info = "Thông tin cá nhân";
$registration_account_info = "Thông tin tài khoản";
$registartion_username_text = "Nhập tên và mật khẩu cho người dùng khi đăng nhập vào hệ thống.";

$registration_question_text=<<<registration_question_text
Enter a secret question to which only this user knows the answer to. We will ask this question in case the user forgets his or her password.
When the correct answer is given, an e-mail will be send to the registered e-mail address with the username and a new password to log in.
registration_question_text;

$registration_text=<<<registration_text
Điền thông tin và nhấn vào 'Đồng ý' để đăng ký. Cần điền đầy đủ các vùng được đánh dấu *.<br />
Các thông tin bạn cung cấp chỉ sử dụng trong Group-Office, và sẽ không tiết lộ ra bên ngoài với bất cứ lý do nào.
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
$strPasswordConfirm = "Xác nhận lại mật khẩu";
$strPassword = "Mật khẩu";

$reg_company_info = "Thông tin công ty";

$max_user_limit = "Maximum ammount of users reached.";
$max_users_text = "Sorry, you can't register more users. Contact us if you want to increase your number of users.";
$ml_sent_items = "Sent items";
$ml_spam = "Spam";
$ac_samba_user = "User has acces to the file and print server.";
$ml_email_only = "Only required when you don't create an E-mail address";
$register_new_user = 'New Group-Office subscriber';
$register_thanks = '<h2>Thank you</h2>Your subscription will be processed as soon as possible. You will recieve further notice.';
$register_accept = 'Add to users';
$register_addressbook = 'Thêm vào sổ địa chỉ';
$register_title = 'Tham gia';
$registration_mail_body=<<<registration_mail_body
Chào %sex% %last_name% %middle_name% %first_name%,

Bạn vừa được đăng ký tại Group-Office.
Chúc bạn làm việc thật thoải mái.

Bạn có thể đăng nhập với các thông tin sau:

Tài khoản: %username%
Mật khẩu: %password%

Nhấn lên liên kết phía dưới để đăng nhập:

registration_mail_body;

//1.98
$reg_language = 'Ngôn ngữ';
$reg_theme = 'Kiểu dáng';

//2.05
$admin_use = 'Use';
$admin_module = 'Module';
$admin_manage = 'Quản lý';

$admin_groups = 'User groups';
$admin_groups_user = 'Người dùng là thành viên của các nhóm sau';
?>
