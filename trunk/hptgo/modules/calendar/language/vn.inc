<?php
//Uncomment this line in new translations!
require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "Nhan đề";
$sc_description = "Mô tả";
$sc_participants = "Người mời";

$sc_location = "Địa điểm";

$month_times[0] = "Đầu tiên";
$month_times[1] = "Thứ nhì";
$month_times[2] = "Thứ ba";
$month_times[3] = "Thứ tư";

$sc_new_app = "Thêm sự kiện";
$sc_calendars = "Lịch công tác";
$sc_invited = 'Bạn được mời tham gia các việc mô tả dưới đây : ';

$sc_declined_event = 'từ chối tham gia sự kiện mô tả dưới đây :';
$sc_declined_mail_title = '(Lịch) Từ chối tham gia sự kiện của bạn';
$sc_reason = 'Lý do';

$sc_types1[0] = 'Không tái diễn';
$sc_types1[1] = 'ngày';
$sc_types1[2] = 'tuần';
$sc_types1[3] = 'tháng theo kỳ hạn';
$sc_types1[4] = 'tháng theo ngày';
$sc_types1[5] = 'năm';


$sc_types[0] = 'Một lần';
$sc_types[1] = 'Lặp theo ngày';
$sc_types[2] = 'Lặp theo tuần';
$sc_types[3] = 'Monthly by date';
$sc_types[4] = 'Monthly by day';
$sc_types[5] = 'Lặp theo năm';


$sc_start_at = 'Bắt đầu';
$sc_end_at = 'Kết thúc';
$sc_start_time = 'Khởi điểm';
$sc_end_time = 'Kết điểm';

$sc_at_days = 'Vào ngày';
$sc_of_month = 'của tháng.';

$sc_cycle_start = 'Chu kỳ bắt đầu từ';
$sc_cycle_end = 'Lặp lại cho đến khi';
$sc_accept_question = 'Bạn có đồng ý và lên lịch cho việc này?';

$sc_accept = 'đồng ý';
$sc_decline = 'từ chối';

$sc_notime = 'Không sử dụng thời gian';
$sc_noend = 'Không có ngày kết thúc.';

$sc_never_happens = 'Dữ liệu không phù hợp! Sự kiện này không bao giờ xảy ra.';
$sc_start_later = 'Dữ liệu không phù hợp! Thời điểm bắt đầu trễ hơn thời điểm kết thúc.';

$sc_add_self = 'Nhập bằng tay';

$sc_select_calendar = 'Chọn lịch để sắp xếp sự kiện này vào';
$sc_accept_confirm = 'Sự kiện được chấp nhận và lên lịch.';
$sc_no_calendars = 'Không có lịch nào phù hợp với sự kiện này.';
$sc_bad_event = 'Không tìm thấy sự kiện phù hợp. Sự kiện có lẽ đã bị xoá.';
$sc_accept_title = 'Chấp nhận sự kiện';

$sc_decline_confirm = 'Sự kiện đã bị từ chối.';
$sc_decline_title = 'Từ chối sự kiện';

$sc_status = 'Trạng thái';
$sc_not_responded = 'Chưa đáp';
$sc_accepted = 'Đã chấp thuận';
$sc_declined = 'Đã từ chối';
$sc_no_participants = 'Không có người tham gia';

$sc_person = 'Người';
$sc_owner = 'Người lập';
$sc_type = 'Kiểu';
$sc_subscribed = 'Đã tham gia';

$sc_subscribe = 'Tham gia';
$sc_new_calendar = 'Thêm lịch mới';
$sc_no_calendar = 'Không tìm thấy lịch. Hãy tạo bây giờ hoặc để Group-Office tạo tự động giúp bạn.';
$sc_day_view = 'Xem ngày';
$sc_week_view = 'Xem tuần';
$sc_list_view = 'Liệt kê';
$sc_put_in = 'Cho sự kiện vào';
$sc_inherit_permissions ='Kế thừa quyền từ';
$sc_no_inherit = 'Không kế thừa';

$sc_select_calendar_please = 'Chọn ít nhất 1 lịch.';
$sc_events = 'Sự kiện';

$sc_from_calendar = 'từ lịch';
$sc_enitrely = 'Toàn bộ';
$sc_delete_event = 'Xoá sự kiện';
$sc_delete_pre = 'Bạn có chắc bạn muốn xoá sự kiện';
$sc_delete_suf = 'toàn bộ hay chỉ trong lịch này?';

$sc_refresh = 'Nạp lại';
$sc_reminder = 'Nhắc nhở';
$sc_mins = 'phút';
$sc_hour = 'giờ';
$sc_hours = 'giờ';
$sc_day = 'ngày';
$sc_days = 'ngày';
$sc_week = 'tuần';
$sc_weeks = 'tuần';
$sc_reminder_set_to_early = "'Nhắc nhở' không thể xuất hiện trước một cái đã có. Hãy thu ngắn thời gian nhắc hoặc kéo dài chu kỳ lặp.";
$sc_cycle_start_later = 'Ngày bắt đầu trễ hơn hoặc bằng ngày kết thúc.';

$cmdGenerate = 'Tính';
$sc_preferences = 'Chọn giao diện';
$sc_holidays = 'Ngày lễ';

$strNoHolidays = 'Không có ngày lễ nào trong cơ sở dữ liệu';
$strUnknown = 'Không rõ';
$strYear = 'Năm';
$strDeleteHolidaysPrefix = 'Bạn có chắc bạn muốn xoá toàn bộ những ngày lễ trong năm';
$strDeleteHolidaysSuffix = '?';
$strReplaceHolidays = 'Bạn có chắc bạn muốn ghi đè lên toàn bộ những ngày lễ hiện tại?';
$sc_holidays_text1 = 'Nhấn \''.$cmdGenerate.'\' để tính ngày lễ trong năm được chọn.';
$sc_holidays_text2 = 'Nhấn \''.$cmdApply.'\' để lưu tất cả những ngày lễ được tính vào cơ sở dữ liệu.';

//translation required below this
$sc_client = 'Khách hàng';
$sc_no_contact = 'None';

$sc_background = 'Nền';

$sc_colors['red'] = 'Đỏ';
$sc_colors['green'] = 'Xanh lá cây';
$sc_colors['blue'] = 'Xanh dương';
$sc_colors['pink'] = 'Hồng';
$sc_colors['orange'] = 'Cam';
$sc_colors['brown'] = 'Nâu';
$sc_colors['yellow'] = 'Vàng';
$sc_colors['grey'] = 'Xám';
$sc_colors['purple'] = 'Tím';
$sc_colors['white']= 'Trắng';
$sc_status = 'trạng thái';

$sc_show_hours = 'Xem giờ từ';
$sc_to = 'đến';

$sc_today = 'Hôm nay';

$sc_recur_every = 'Lặp lại mỗi';
$sc_months = 'tháng';

$sc_private_event = 'Sự kiện cá nhân';
$sc_calendar_exists = 'Lịch công tác bị trùng tên';
//translate
$sc_month = 'tháng';
$sc_view = 'Xem';
$sc_calendar = 'Lịch';
$sc_default = 'Mặc định';
$sc_month_view = 'Xem tháng';

//text changed, needs new translation
$sc_holidays_no_file_prefix = 'Không thể tạo thông tin ngày lễ, bởi vì hồ sơ chứa thông tin ';
$sc_holidays_no_file_suffix = ' cho ngôn ngữ không tồn tại hoặc trống rỗng.';

$sc_no_events = 'Không có sự kiện nào';

//2.03
$cal_export = 'Xuất ra';
$cal_import = 'Đưa vào';
$cal_please_wait = 'Xin chờ...';
$cal_import_text = 'Chọn 1 hồ sơ iCal (*.ics) và nhấn \'Ok\'';
$cal_filename = 'Hồ sơ';
$cal_import_success = 'Dữ liệu đã được đưa vào';

//2.04
$cal_add_todo = 'Thêm công việc mới';
$cal_todos = 'Các công việc';

$cal_hide_completed = 'Ẩn những việc đã hoàn tất';
$cal_show_all = 'Xem tất cả công việc';
$cal_show_own = 'Xem công việc của mình';
$cal_show_responsible = 'Xem phần chịu trách nhiệm';

$cal_priority = 'Độ ưu tiên';
$cal_priority_values[0] = 'Thấp';
$cal_priority_values[1] = 'Bình thường';
$cal_priority_values[2] = 'Cao';

$cal_status = 'Trạng thái';
$cal_canceled = 'Đã ngưng';
$cal_completed = 'Đã hoàn thành được';
$cal_percent_completed = '%';

$cal_before_task_start = 'Trước khi công việc bắt đầu';
$cal_before_task_due = 'Trước khi công việc cần kết thúc';
$cal_responsible = 'Chịu trách nhiệm';
$cal_due_at = 'Hết hạn lúc';
$cal_no_todos= 'Không có công việc nào';
$cal_resend_invitation = 'Gửi lại lời mời';

//2.05
$cal_everybody_write = 'Mọi người có thể sửa';
$cal_everybody_read = 'Chỉ mình tôi có thể sửa';

$cal_view = 'Nhóm lịch';
$cal_views = 'Nhóm lịch';
$sc_view_exists = 'Đã có 1 nhóm lịch cùng tên';
$cal_new_view = 'Tạo nhóm lịch mới';
$cal_event_calendars = 'Lịch';

//2.06
$cal_view_merged = 'kết hợp';
$cal_view_emerged = 'tách riêng';
$strNoPastEvent = 'Bạn không được thêm một sự kiện vào quá khứ';
$strNotAddEvent= 'Bạn không có quyền thêm sự kiện trong nhóm lịch này';
$subjectEventInvitation = '[Calendar] Bạn được mời tham dự sự kiện %s';

$sc_subscribed = 'Chọn';
$sc_choice_calendar_msg = 'Bạn phải chọn ít nhất 1 lịch công tác !';
$sc_choice_view_msg = 'Bạn phải chọn ít nhất 1 lịch theo dõi !';

$sc_no_ok_in_past = 'Sự kiện này đã qua';

$cmdShowSubscribed = 'Xem lịch đã chọn';
$cmdShowOwned = 'Xem lịch của mình';
$strSaveOk = "Đã thêm sự kiện %s.";
?>
