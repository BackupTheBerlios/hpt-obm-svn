// ** I18N

// Calendar EN language
// Author: Mihai Bazon, <mishoo@infoiasi.ro>
// Encoding: any
// Distributed under the same terms as the calendar itself.

// For translators: please use UTF-8 if possible.  We strongly believe that
// Unicode is the answer to a real internationalized world.  Also please
// include your contact information in the header, as can be seen above.

// full day names
Calendar._DN = new Array
("Chủ nhật",
 "Thứ hai",
 "Thứ ba",
 "Thứ tư",
 "Thứ năm",
 "Thứ sáu",
 "Thứ bảy",
 "Chủ nhật");

// Please note that the following array of short day names (and the same goes
// for short month names, _SMN) isn't absolutely necessary.  We give it here
// for exemplification on how one can customize the short day names, but if
// they are simply the first N letters of the full name you can simply say:
//
//   Calendar._SDN_len = N; // short day name length
//   Calendar._SMN_len = N; // short month name length
//
// If N = 3 then this is not needed either since we assume a value of 3 if not
// present, to be compatible with translation files that were written before
// this feature.

// short day names
Calendar._SDN = new Array
("CN",
 "T2",
 "T3",
 "T4",
 "T5",
 "T6",
 "T7",
 "CN");

// full month names
Calendar._MN = new Array
("Tháng 1",
 "Tháng 2",
 "Tháng 3",
 "Tháng 4",
 "Tháng 5",
 "Tháng 6",
 "Tháng 7",
 "Tháng 8",
 "Tháng 9",
 "Tháng 10",
 "Tháng 11",
 "Tháng 12");

// short month names
Calendar._SMN = new Array
("Th1",
 "Th2",
 "Th3",
 "Th4",
 "Th5",
 "Th6",
 "Th7",
 "Th8",
 "Th9",
 "Th10",
 "Th11",
 "Th12");

// tooltips
Calendar._TT = {};
Calendar._TT["INFO"] = "Thông tin về chương trình Lịch";

Calendar._TT["ABOUT"] =
"DHTML Date/Time Selector\n" +
"(c) dynarch.com 2002-2003\n" + // don't translate this this ;-)
"Để biết về các bản mới ghé thăm: http://dynarch.com/mishoo/calendar.epl\n" +
"Được cung cấp theo giấy phép GNU LGPL.  Xem http://gnu.org/licenses/lgpl.html để biết thêm chi tiết." +
"\n\n" +
"Chọn ngày:\n" +
"- Dùng \xab, \xbb để chọn năm\n" +
"- Dùng " + String.fromCharCode(0x2039) + ", " + String.fromCharCode(0x203a) + " để chọn tháng\n" +
"- Nhấn và giữ phím chuột trên bất cứ phím nào để chọn nhanh.";
Calendar._TT["ABOUT_TIME"] = "\n\n" +
"Chọn giờ:\n" +
"- Nhấn lên giờ, phút, giây để tăng\n" +
"- hoặc vừa nhấn vừa giữ Shift để giảm\n" +
"- hoặc nhấn và kéo đồng thời để chọn nhanh.";

Calendar._TT["PREV_YEAR"] = "Năm trước (hold for menu)";
Calendar._TT["PREV_MONTH"] = "Tháng trước (hold for menu)";
Calendar._TT["GO_TODAY"] = "Hôm nay";
Calendar._TT["NEXT_MONTH"] = "Tháng kế (hold for menu)";
Calendar._TT["NEXT_YEAR"] = "Năm sau (hold for menu)";
Calendar._TT["SEL_DATE"] = "Chọn ngày";
Calendar._TT["DRAG_TO_MOVE"] = "Kéo để dịch chuyển";
Calendar._TT["PART_TODAY"] = " (hôm nay)";

// the following is to inform that "%s" is to be the first day of week
// %s will be replaced with the day name.
Calendar._TT["DAY_FIRST"] = "Display %s first";

// This may be locale-dependent.  It specifies the week-end days, as an array
// of comma-separated numbers.  The numbers are from 0 to 6: 0 means Sunday, 1
// means Monday, etc.
Calendar._TT["WEEKEND"] = "0,6";

Calendar._TT["CLOSE"] = "Đóng";
Calendar._TT["TODAY"] = "Hôm nay";
Calendar._TT["TIME_PART"] = "(Shift-)Click or drag to change value";

// date formats
Calendar._TT["DEF_DATE_FORMAT"] = "%Y-%m-%d";
Calendar._TT["TT_DATE_FORMAT"] = "%a, %b %e";

Calendar._TT["WK"] = "[]";
Calendar._TT["TIME"] = "Giờ:";
