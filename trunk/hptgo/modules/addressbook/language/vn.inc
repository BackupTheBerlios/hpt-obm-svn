<?php
//Uncomment this line in new translations!
require($GO_LANGUAGE->get_fallback_language_file('addressbook'));

//contacts
$contacts_title = "Sổ địa chỉ";
$contacts_no_contacts = "Không tìm thấy.";
$contacts_members = "Thành viên";
$contacts_contacts = "Liên hệ";
$contacts_add = "Thêm liên hệ";
$contacts_groups = "Các nhóm";
$contacts_other = "Các liên hệ khác";
$contacts_move_to_group = "Chuyển thông tin liên hệ tới nhóm...";
$groups_no_groups = "Bạn không có bất cứ 1 nhóm nào";
$contacts_group = "Nhóm";
$contacts_empty_group = "Use the dropdown box to move contacts to this group.";
$contacts_this_group = 'nhóm này';

$add_contacts_text = "Điền địa chỉ E-mail và mã xác thực của các thành viên rồi nhấn 'Ok'";
$add_contacts_success = "Thêm thành công";
$add_contacts_wrong = "Nhầm E-mail hoặc mã xác thực.";
$add_personal_text = "Nhấn 'Save' đê thêm thông tin liên hệ. Các vùng đánh dấu * cần ghi đủ.";
$contacts_select = "Chọn các liên hệ để thêm rồi nhấn 'Ok'.";
$contacts_select_title = "Các liên hệ";
$contact_profile = "Thông tin";
$user_profile = "Thông tin người dùng";
$contacts_authorise = "Xác thực";
$contacts_add_member = "Thêm thành viên này vào sổ liên hệ của bạn";
$strConfirmSynchronise = "This will update this contact with data the user has entered himself. Manual changes by you will be lost. Are you sure you want to continue?";
$contacts_import = "Đưa vào";
$contacts_export = "Xuất ra";
$contacts_export_text = "Chọn kiểu CSV mà bạn muốn tạo.";
$contacts_delete = "Xoá";

$contacts_import_text_group = "Chọn 1 nhóm và hồ sơ CSV rồi nhấn 'Ok'.";
$fbNoFile = "No file was not recieved";
$fbPleaseWait = "Đang đưa hồ sơ vào, xin chờ trong giây lát...";

$strNotIncluded = "Không được nhập vào";
$contacts_import_feedback = "Các cột sau có trong hồ sơ CSV của bạn.<br />";
$contacts_import_feedback .= "Chọn nhóm mà bạn muốn nhập vào hoặc tự động tạo các nhóm dựa trên các cột trong hồ sơ.<br />";
$contacts_import_feedback .= " So trùng các cột của Group-Office với các cột trong hồ sơ và chắc chắn là ban không chọn 1 cột 2 lần.";
$contacts_separator = "Các giá trị được phân cách bởi";
$contacts_no_separator = "Bạn chưa chọn ký tự phân cách.";
$fbFilename = "Tên hồ sơ";
$contacts_import_title = "Đưa thêm sổ địa chỉ vào";
$contacts_import_success = "Đưa vào thành công";
$contacts_import_double = "Bạn đã chọn 1 hoặc nhiều cột 2 lần. Sửa chữa và thử lại.";
//changed:
$contacts_import_noname = 'Bạn phải chọn vùng \'Họ\' và \'Tên\' ';
$contacts_import_incompatible = "Định dạng hồ sơ không tương thích.<br /><br />Hồ sơ CSV là dạng lưu trữ dữ liệu phân cách bởi dấu phẩy.";
$strMember = "Thành viên";
$contact_exist_warning = "<b>Chú ý:</b> Người này đã có trong danh sách liên hệ của bạn.";
$contacts_add_to_group = "Thêm những người được chọn vào danh sách liên hệ theo nhóm...";
$contacts_add_to = "Thêm những người được chọn vào sổ liên hệ";
$contacts_add_user = "thêm";
$contacts_edit = "Sửa";
$contacts_no_select = "Bạn chưa chọn gì";
$contacts_this_contact = "liên hệ này";

$contacts_seperator_text = 'Các giá trị được phân cách bởi';
$contacts_quote = 'Các giá trị được bao bởi';
$contacts_clrf = 'Các dòng kết thúc bởi';
$contacts_auto_group = 'Tự động tạo các nhóm từ hồ sơ';
$contacts_import_to_group = 'Nhập vào nhóm';

$contacts_search = 'Tìm';
$cmdSearch = 'Tìm';
$contacts_search_text = "Nhấn lên 1 chữ hoặc nhận 1 cụm từ rồi nhấn lên 'Tìm'. Bạn có thể dùng * làm ký tự thay thế.";
$contacts_results = 'kết quả';
$contacts_result = 'kết quả';


$contacts_color = 'Màu';

$contacts_colors['black'] = 'Đen';
$contacts_colors['red'] = 'Đỏ';
$contacts_colors['green'] = 'Xanh lá cây';
$contacts_colors['blue'] = 'Xanh dương';
$contacts_colors['pink'] = 'Hồng';
$contacts_colors['orange'] = 'Cam';
$contacts_colors['brown'] = 'Nâu';

//---------------- new TKD
$ab_category_name = 'Tên danh mục';
$ab_new_category = 'Thêm danh mục';
$ab_category = 'Danh mục';
$ab_parent_category = 'Danh mục thêm vào chứa trong';
$ab_TOP = '[NHÓM DANH MỤC]';

$ab_ConfirmDeleteStatus = 'Bạn có chắc chắn là muốn xoá danh mục \"%s\" ?';
$ab_ConfirmDeleteStatus2 = 'Bạn có chắc chắn là muốn xoá danh mục \"%s\" và tất cả các danh mục con ?';
//-----------------------------

$ab_addressbooks = 'Sổ địa chỉ';
$ab_owner = 'Người lập';
$ab_subscribed = 'Chọn';
$ab_new_ab = 'Thêm sổ địa chỉ';
$ab_addressbook_exists = 'Đã có sổ địa chỉ trùng tên này';
$ab_addressbook = 'Sổ địa chỉ';
$contacts_select_single = 'Nhấn lên thông tin liên hệ để chọn người này';

$ab_group_exists = 'Đã có sổ địa chỉ trùng tên này';
$ab_group_on_file = 'Nhóm các thông tin liên hệ dựa vào hồ sơ';
$ab_default = 'Mặc định';

//translation required
$ab_edit_contact = 'Nhấn \'Ok\' để lưu. Các vùng đánh dấu * cần điền đầy đủ';
$ab_group_on_file = 'Nhóm các liên hệ dựa vào hồ sơ';
$ab_default = 'Mặc định';
$ab_browse = 'Duyệt';
$ab_comment = 'Chú thích';

$ab_contact = 'Liên hệ';
$ab_send_message = 'E-mail';
$ab_new_event = 'Sự kiện';

$ab_templates = 'Các biểu mẫu';
$ab_your_templates = 'Của bạn';
$ab_all_templates = 'Tất cả';

$template_type[0] = 'E-mail';
$template_type[1] = 'OpenOffice.Org';
$ab_type = 'Kiểu mẫu';
$ab_template_exists = 'Biểu mẫu với tên này đã có.';

$sir_madam['M'] = 'Ô.';
$sir_madam['F'] = 'B.';

$ab_upload_oo = 'Upload OpenOffice.org template';
$ab_upload_nofile = 'Lỗi khi gửi hồ sơ.';

$ab_oo_doc = 'Tài liệu';
$ab_select_template = 'Select the template for this document:';
$ab_add_field_data = 'You can use the following values in your document:';
$ab_datafield = 'Vùng dữ liệu';

$ab_no_template = 'Không có biểu mẫu';
$ab_new_note = 'Ghi chú';

$ab_events = 'Sự kiện';

$ab_new_contact = 'Thêm liên hệ';
$ab_new_company = 'Thêm công ty';

$ab_companies = 'Công ty';
$ab_company_properties = 'Thông tin công ty';
$ab_bank_no = 'Số tài khoản ngân hàng';
$ab_companies_count_single = 'công ty';
$ab_companies_count = 'công ty';
$ab_no_companies = 'Không tìm thấy công ty nào';
$ab_no_company = 'Không tìm thấy công ty nào';

$ab_import_type = 'Đưa vào';
$ab_no_company_found = 'Không tìm thấy công ty nào';
$ab_search_company = 'Tìm công ty';
$ab_edit_company = 'Thay đổi thông tin công ty';
$ab_new_group = 'Thêm nhóm';
$ab_export_type = 'Xuất ra';

$ab_custom_fields = 'Vùng dữ liệu phụ';
$ab_field_size = 'Kích thước';

$ab_new_field = 'Tạo vùng dữ liệu';
$ab_invalid_field_size = 'Sai kích thước. Chọn giá trị trong khoảng từ 10 đến 255.';
$ab_field_exists = 'Đã có vùng với tên này.';

$ab_employees = 'Nhân viên';
$ab_no_employees = 'Không có ai';

$ab_employee_count_single = 'nhân viên';
$ab_employee_count_multiple = 'nhân viên';
$ab_vat_no = 'VAT number';

$ab_add_new = 'Thêm mới';
$ab_add_existing = 'Thêm người đã có';

$ab_access = 'Truy cập';
//1.98
$ab_mailings = 'Mailings';
$ab_mailing_groups = 'Mailing groups';
$ab_no_mailing_groups = 'NO mailing groups found';
$ab_no_templates = 'No templates found';
$ab_mailing_group_exists = 'A mailing group with this name already exists.';
$ab_contacts_count_single = 'contact';
$ab_contacts_count_multiple = 'contacts';
$ab_new_mailing = 'New mailing';
$ab_all_your_addressbooks = 'tất cả các sổ địa chỉ';

$ab_search_for = 'Tìm';
$ab_search_in = '&nbsp;trong&nbsp;';
$ab_search_on = '&nbsp;theo&nbsp;';
$ab_search_keyword = 'Chứa cụm từ';
$ab_search_category = '&nbsp;thuộc danh mục&nbsp;';
$ab_wildcard = '(Dùng \'%\' làm ký tự thay thế)';
$ab_search_all = '[TẤT CẢ]';

//2.03
$ab_copy_read_acl = 'Copy quyền đọc qua sổ \'Liên hệ\' và \'Công ty\'';
$ab_copy_write_acl = 'Copy quyền ghi qua sổ \'Liên hệ\' và \'Công ty\'';

$ab_catagory_exists = 'Phân loại bị trùng tên.';
$ab_add_catagory = 'Thêm phân loại';
$ab_add_field = 'Thêm thông tin';
$ab_catagory = 'Phân loại';

//2.04
$ab_todos = 'Cần làm';


//2.05
$ab_download = 'Lấy ra';
$ab_download_template = 'Nhấn \''.$ab_download.'\' để lấy biểu mẫu về chỉnh sửa trên máy tính của bạn.';
$ab_upload = 'Đưa vào';
//modified string:
$ab_upload_oo_text = 'Sau khi sửa xong các biểu mẫu của OpenOffice.org bạn nên chuyển nó sang định dạng của Group-Office. Chọn hồ sơ và nhấn \''.$ab_upload.'\' để chuyển biểu mẫu.';
$ab_file_type = 'Kiểu hồ sơ';
$ab_import_vcf_file_ok = 'The uploaded file seems to be fine. Please click on \'Ok\' to adopt it to the addressbook.';
$ab_import_failed = 'Đưa vào thất bại.';
//modified string:
$contacts_import_text = "Chọn hồ sơ để đưa vào rồi nhấn 'Ok'";

$strTitle = 'Chức danh';
$strParentCompany = 'Công ty cha';
$strNone = "Không";
$strListView = "Xem dạng phẳng";
$strTreeView = "Xem dạng phân cấp";
$strParentCompanyError = "Công ty cha không hợp lệ";
$strShortCompanyName = "Tên viết tắt";
$strEnglishCompanyName = "Tên tiếng Anh";
$strRelationDate = "Ngày thiết lập quan hệ";
$strAccountManager = "Nhân viên Phụ trách";
$strNew = "Mới";

$strConfig = "Cấu hình";
$strModifyCategories = "Hiệu chỉnh Danh mục";

$strCom['address'] = $strAddress;
$strCom['zip'] = $strZip;
$strCom['city'] = $strCity;
$strCom['state'] = $strState;
$strCom['country'] = $strCountry;
$strCom['phone'] = $strPhone;
$strCom['fax'] = $strFax;
$strCom['email'] = $strEmail;
$strCom['homepage'] = $strHomepage;
$strCom['bank_no'] = $ab_bank_no;
$strCom['vat_no'] = $ab_vat_no;
$strCom['relation_date'] = $strRelationDate;
$strCom['shortname'] = $strShortCompanyName;
$strCom['engname'] = $strEnglishCompanyName;
$strCom['parent'] = $strParentCompany;

$strCom['initials'] = $strInitials;
$strCom['sex'] = $strSex;
$strCom['birthday'] = $strBirthday;
$strCom['comment'] = $ab_comment;
$strCom['cellular'] = $strCellular;
$strCom['home_phone'] = $strPhone;
$strCom['department'] = $strDepartment;
$strCom['function'] = $strFunction;
$strCom['work_phone'] = $strWorkphone;
$strCom['work_fax'] = $strWorkFax;
$strCom['company_id'] = $strCompany;
$strCom['company'] = $strCompany;
?>
