<?php
//Uncomment this line in new translations!
//require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Số lần đăng nhập";
$strRegistrationDate = "Ngày đăng ký";
$strUsers = "người";
$user_profile = "Thông tin thành viên";
$ac_login_info = "Thông tin đăng nhập";
$ac_registration_time = "Thời điểm đăng ký";
$ac_lastlogin = "Lần đăng nhập cuối";
$ac_logins = "Tổng số lần đăng nhập";
$strMaxOf = "thành viên trên tối đa";
$strUsername = "Username";
$delete_fail = "Bạn không thể tự xoá và xoá người quản lý.";
$admin_new_user = "Thêm mới";

$admin_modules = 'Các chức năng';
$admin_module_access = 'Thành viên này được phép dùng các chức năng sau:';

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
Các thông tin bạn cung cấp chỉ sử dụng trong HPT-OBM, và sẽ không tiết lộ ra bên ngoài với bất cứ lý do nào.
registration_text;

$registartion_authcode_text = "Nhập mã xác thực. Mã xác thực là mã mà thành viên khác cần khi thêm người dùng này vào danh sách liên hệ và xem thông tin chi tiết cá nhânh.";
$registartion_confirm_authcode = "Xác nhận mã xác thực";
$registration_visibility = "Cho phép mọi người xem thông tin cá nhân.";
$registration_success = "<h1>Chúc mừng!</h1>Bạn đã hoàn tất việc tạo người dùng mới và thông báo đã được gửi đến:";
$registration_failure = "Lỗi xảy ra khi đăng ký với các thông tin của bạn. Xin hãy thử lại.";
$registration_mail_subject = "Chào mừng bạn tham gia vào HPT-OPM.";
$registration_folder_error = "Lỗi khi tạo thư mục cá nhân của người dùng. Xin liên hệ với chúng tôi để khắc phục sự cố!";
$registration_title = "Đăng ký tại HPT-OBM";

$registration_email = "Tạo địa chỉ E-mail cho người dùng này trong HPT-OBM.";
$registration_samba = "File & Print server.";
$registration_email_error = "Lỗi khi tạo tài khoản e-mail. Liên hệ với chúng tôi để khắc phục.";
$strPasswordConfirm = "Xác nhận lại mật khẩu";
$strPassword = "Mật khẩu";

$reg_company_info = "Thông tin công ty";

$max_user_limit = "Đã đạt tới số lượng thành viên tối đa của hệ thống.";
$max_users_text = "Xin lỗi!  Bạn không thể tạo thêm người dùng. Xin liên hệ với chúng tôi nếu bạn muốn tăng số lượng người dùng.";
$ml_sent_items = "Sent items";
$ml_spam = "Spam";
$ac_samba_user = "Thành viên được truy cập vào máy chủ quản lý hồ sơ và máy in.";
$ml_email_only = "Chỉ cần khi bạn không tạo địa chỉ E-mail";
$register_new_user = 'Thành viên mới của HPT-OBM';
$register_thanks = '<h2>Cảm ơn bạn</h2>Bản đăng ký của bạn sẽ được xem xét và xử lý sớm nhất. Bạn sẽ nhận được các thông báo sau.';
$register_accept = 'Thêm vào';
$register_addressbook = 'Thêm vào sổ địa chỉ';
$register_title = 'Tham gia';
$registration_mail_body=<<<registration_mail_body
Chào bạn %last_name% %middle_name% %first_name%,

Bạn vừa được đăng ký tại HPT-OBM.
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
$admin_use = 'Dùng';
$admin_module = 'Chức năng';
$admin_manage = 'Quản lý';

$admin_groups = 'Nhóm người';
$admin_groups_user = 'Người dùng là thành viên của các nhóm sau';
?>
