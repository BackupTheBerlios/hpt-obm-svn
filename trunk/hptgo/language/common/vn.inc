<?php
//Uncomment this line in new translations!
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'utf-8';
//main language menu file register login and other non logged in pages
//common

$cmdCapnhat = "Cập nhật";

$cmdLogin = "Đăng nhập";
$cmdSave = "Lưu";
$cmdBack = "Trở lại";
$cmdCancel = "Ngưng";
$cmdClose = "Đóng";
$cmdDefault = "Mặc định";
$cmdOk = "Đồng ý";
$cmdDelete = "Xoá";
$cmdNext = "Kế tiếp";
$cmdPrevious = "Trước đó";
$cmdContinue = "Tiếp tục";
$cmdUpdate = "Đồng bộ";
$cmdApply = "Áp dụng";
$cmdYes = "Có";
$cmdNo = "Không";
$cmdAdd = 'Thêm';
$cmdEdit = 'Sửa';
$cmdInvertSelection = 'Đảo các chọn lựa';

$strEdit = "Sửa";
$strThis = 'này';
$strItems = 'mục';
$strDescription = 'Mô tả';

$strName = "Tên";
$strEnglishName = "Tên Tiếng Anh";
$strNickName = "Tên viết tắt";
$strUser = "Người dùng";
$strUsername = "Tài khoản";
$strPassword = "Mật khẩu";
$strAuthcode = "Mã xác thực";
$strCountry = "Quốc gia";
$strState = "Tỉnh";
$strCity = "Thành phố";
$strZip = "Mã bưu điện";
$strAddress = "Địa chỉ";
$strEmail = "E-mail";
$strPhone = "Điện thoại";
$strWorkphone = "Điện thoại công ty";
$strCellular = "ĐTDĐ";
$strCompany = "Công ty";
$strDepartment = "Phòng/Ban";
$strFunction = "Chức năng";
$strQuestion = "Cẩu hỏi gợi nhớ";
$strAnswer = "Trả lời";
$strFax = "Fax";
$strWorkFax = "Fax công ty";
$strHomepage = "Trang chủ";
$strWorkAddress = "Địa chỉ công ty";
$strWorkZip = "Mã bưu điện công ty";
$strWorkCountry = "Quốc gia";
$strWorkState = "Tỉnh nơi làm";
$strWorkCity = "Thành phố nơi làm";
$strPhoto = "Ảnh";
$strComments = "Chú thích";
$strProperties = 'Đặc tính';
$strDeleteItem = "Xoá";
$strDate = "Ngày";
$strSaveSuccess = "Lưu dữ liệu hoàn tất";
$strSaveError = "Có lỗi khi lưu dữ liệu.";
$strDataError = "Có lỗi khi đọc dữ liệu.";
$strDeleteError = "Không thể xoá dữ liệu.";
$strAclError = "Lỗi quản lý truy cập";
$strDateError = "Bạn nhập ngày không chính xác";
$strAccessDenied = "Không thể truy cập";
$AccessDenied_text = "Bạn không được phép xem trang này hoặc thực hiện việc này.<br />Các thông tin bạn yêu cầu đã bị giới hạn truy cập.";
$invalid_chars = "Những ký tự sau không được phép dùng";
$strGroups = "Các nhóm";
$strOwner = "Người lập";
$strDeletePrefix = "Bạn có chắc là bạn muốn xoá ";
$strDeleteSuffix = "?";
//menu
$menu_logout = "Thoát";
$menu_applications = "Các ứng dụng";
$menu_help = "Trợ giúp";
$menu_about = "Thông tin";
$menu_documentation = "Tài liệu";
$menu_configuration = "Cấu hình";
$menu_groups = "Quản lý các nhóm";
$menu_accounts = "Thay đổi các thông số tài khoản";
$menu_preferences = "Thay đổi các thiết lập cá nhân";
$menu_search_discussions = "Thảo luận";
$menu_search_files = "Hồ sơ và thư mục";
$menu_search_web = "Trang chủ";
$menu_filetypes = "Loại hồ sơ";
$menu_discussions = "Thảo luận";

$menu_users = "Quản lý người dùng";
$menu_new = "Tạo mới";
$menu_message = "Thông báo";
$menu_contact = "Liên hệ";
$menu_event = "Sự kiện";
$menu_bookmark = "Bookmark";
$menu_topics = "Các chủ đề";
$menu_support = "Hỗ trợ";
$menu_projects = "Các dự án";
$menu_sysinfo = "Xem thông tin hệ thống";
$menu_admin = 'Trình đơn quản trị viên';
$menu_modules = 'Thêm/bớt chức năng';
$menu_manual = 'Sổ tay';

//class validate
$error_required = "Bạn chưa cung cấp đủ thông tin sau.";
$error_match_auth = "Mã xác thực không trùng.";
$error_match_pass = "Mật khẩu không trùng.";
$error_min_length = "Số ký tự tối thiểu cho vùng dưới đây:";
$error_max_length = "Số ký tự tối đa cho vùng dưới đây:";
$error_email = "Địa chỉ E-mail không hợp lệ.";
$error_email_exists = "Địa chỉ E-mail đã tồn tại.";
$error_username_exists = "Xin lỗi, tên người dùng đã có. Hãy chọn tên khác.";
$errors_in_form = "Có lỗi trong mẫu biểu. Hãy chỉnh sửa và thử lại.";
$error_missing_field = "Bạn chưa nhập đủ thông tin.";

//acl control
$acl_control_auth_groups = "Nhóm được phép";
$acl_control_auth_users = "Người được phép";
$acl_control_no_contacts = "Bạn chưa có thành viên nào trong sổ liên hệ.<br /> Dùng sổ địa chỉ để thêm các thành viên vào sổ liên hệ của bạn.";

$strShowProfile = "Xem thông tin";
$strShowGroup = "Xem nhóm thành viên";
$strProcessingTime = "Quá trình xử lý thông tin cho trang này mất ";
$strLoggedInAs = "Đăng nhập bằng ";
$strAt = " tại ";
$strEmailTo = "Gửi E-mail cho: ";
$strProtectedUser = "Người dùng được bảo vệ";

$http_not_found = "Không tìm thấy tài liệu";
$http_not_found_text = "Không tìm thấy tài liệu yêu cầu. Có thể bạn đã nhập nhầm địa chỉ.";
$http_error = "Lỗi bên trong máy chủ";
$http_bad_request = "Yêu cầu không hợp lệ";
$strAddContact = "Thêm vào mục liên hệ";

$strReadRights = "Quyền đọc";
$strWriteRights = "Quyền thay đổi";
$strPermissions = 'Phân quyền';

$days[0]="CN";
$days[1]="T2";
$days[2]="T3";
$days[3]="T4";
$days[4]="T5";
$days[5]="T6";
$days[6]="T7";

$months[0]="tháng 1";
$months[1]="tháng 2";
$months[2]="tháng 3";
$months[3]="tháng 4";
$months[4]="tháng 5";
$months[5]="tháng 6";
$months[6]="tháng 7";
$months[7]="tháng 8";
$months[8]="tháng 9";
$months[9]="tháng 10";
$months[10]="tháng 11";
$months[11]="tháng 12";

$full_days[0] = "Chủ nhật";
$full_days[1] = "Thứ hai";
$full_days[2] = "Thứ ba";
$full_days[3] = "Thứ tư";
$full_days[4] = "Thứ năm";
$full_days[5] = "Thứ sáu";
$full_days[6] = "Thứ bảy";
$strLoadTime = 'Tạo ra trong';
$strDescription = 'Mô tả';



$lang_modules['addressbook'] = "Sổ địa chỉ";
$cmdShowAll = 'Xem toàn bộ';
$cmdSearch = 'Tìm kiếm';

//translation required
$cmdYesToAll = 'Đồng ý cho tất cả';
$cmdNoToAll = 'Từ chối tất cả';

$short_months[0]="tháng 1";
$short_months[1]="tháng 2";
$short_months[2]="tháng 3";
$short_months[3]="tháng 4";
$short_months[4]="tháng 5";
$short_months[5]="tháng 6";
$short_months[6]="tháng 7";
$short_months[7]="tháng 8";
$short_months[8]="tháng 9";
$short_months[9]="tháng 10";
$short_months[10]="tháng 11";
$short_months[11]="tháng 12";

$strToday = 'Hôm nay';

$cmdPrint = 'In ấn';
$strFirstName = 'Tên';
$strLastName = 'Họ';
$strInitials = 'Chữ đầu';
$strSex = 'Giới tính';
$strBirthday = 'Ngày sinh';
$strSexes['M'] = 'Nam';
$strSexes['F'] = 'Nữ';

$strTitle = 'Tiêu đề';
$strBeginning = 'Bắt đầu';
$strMiddleName = 'Tên đệm';
$sir_madam['M'] = 'Ô.';
$sir_madam['F'] = 'B.';
$strModifiedAt = 'Sửa đổi';
$strCreatedAt = 'Tạo';

$htmlarea_language_name = 'vn';
$strLocalization = 'Bản địa hoá';


//look up the language in controls/jscalendar/lang
$jscalendar_language_name = 'calendar-vn.js';


//2.01 translations

//look up language in 'classes/phpmailer/language'
$php_mailer_lang = 'utf-8';
$strEmailFail = 'Gửi e-mail thất bại.';

//for people that want to use ktmlarea
$ktml_language = 'vietnamese';

$ml_host_unreachable = ' không thê kết nối. Tắt chế độ tự động kiểm tra e-mail.';
$strRegion = 'Miền';

//2.05
$cmdReset = 'Tái lập';
$strMyName = 'Tên tôi';

//modules
$lang_modules['scheduler'] = 'Thời gian biểu';
$lang_modules['calendar'] = "Lịch";
$lang_modules['filesystem'] = 'Hồ sơ';
$lang_modules['email'] = 'E-mail';
$lang_modules['projects'] = 'Dự án';
$lang_modules['search'] = 'Tìm kiếm';
$lang_modules['comics'] = "Truyện tranh";
$lang_modules['contacts'] = "Sổ địa chỉ";
$lang_modules['cms'] = "Trang web";
$lang_modules['notes'] = "Ghi chú";
$lang_modules['bookmarks'] = "Bảng đánh dấu";
$lang_modules['summary'] = "Tóm lược";
$lang_modules['todos'] = "Công việc";
$strInvalidEmail = "Email sai";
$add_group_showall = 'Hiện mọi nhóm';
$add_group_showauth = 'Hiện những nhóm có tham gia';
?>
