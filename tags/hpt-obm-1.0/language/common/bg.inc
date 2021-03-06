<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'Windows-1251';

$lang_modules['scheduler'] = '��������';
$lang_modules['filesystem'] = '�������';
$lang_modules['email'] = '����';
$lang_modules['projects'] = '�������';
$lang_modules['search'] = '�����';
$lang_modules['contacts'] = "������� �����";
$lang_modules['phone'] = "��������� ��������";

$lang_modules['cms'] = "�������";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "�����";
$cmdSave = "�����";
$cmdBack = "�����";
$cmdCancel = "�����";
$cmdClose = "�������";
$cmdOk = "��";
$cmdDelete = "�������";
$cmdNext = "�������";
$cmdPrevious = "��������";
$cmdContinue = "�����������";
$cmdUpdate = "������������";
$cmdApply = "�������";
$cmdYes = "��";
$cmdNo = "��";
$cmdAdd = '������';
$cmdEdit = '�����������';
$cmdInvertSelection = '������� ��������';

$strEdit = "�����������";
$strThis = '����';
$strItems = '��������';
$strDescription = '��������';

$strName = "���";
$strUser = "����������";
$strUsername = "������������� ���";
$strPassword = "������";
$strAuthcode = "���������� ���";
$strCountry = "������";
$strState = "���������";
$strCity = "����";
$strZip = "ZIP/�������� ���";
$strAddress = "�����";
$strEmail = "E-mail";
$strPhone = "�������";
$strWorkphone = "�������� �������";
$strCellular = "�������� �������";
$strCompany = "��������";
$strDepartment = "�����";
$strFunction = "�������";
$strQuestion = "�������� ������";
$strAnswer = "�������";
$strFax = "����";
$strWorkFax = "���� �� ������";
$strHomepage = "������� ��������";
$strWorkAddress = "����� �� ������";
$strWorkZip = "Zipcode �� ������";
$strWorkCountry = "������ � ����� ������";
$strWorkState = "������� ���������";
$strWorkCity = "���� � ����� ������";
$strPhoto = "�����������";
$strComments = "��������";

$strProperties = '��������';

$strDeleteItem = "�������";

$strDate = "����";

$strSaveSuccess = "������� �������� �������";
$strSaveError = "���������� � ������ ��� ��������� �� �������. ���� �������� ������.";
$strDataError = "���������� � ������ ��� ������ �� �������. ���� �������� �� ����������� ����������.";
$strDeleteError = "�� ���� �� �� �������� �������.";
$strAclError = "������ ��� �������� �� ������";
$strDateError = "������ ��� ������ ����";



$strAccessDenied = "������� ��������";
$AccessDenied_text = "�� �� � ��������� �� ������ ���� ���� �� ����� ��� �� ��������� ���� ��������.<br />���� ���������� � ��������� ������� �� ������������ ����� ������.";

$invalid_chars = "�������� ������� �� �� ���������";

$strGroups = "�����";
$strOwner = "����������";

$strDeletePrefix = "������� �� ��� �� ������ �� �������� ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "�������";
$menu_applications = "����������";
$menu_help = "�����";
$menu_about = "�������";
$menu_documentation = "���������";
$menu_configuration = "���������";
$menu_groups = "����������&nbsp;�����";
$menu_accounts = "�����&nbsp;account";
$menu_preferences = "�������������";
$menu_bookmarks = "�������";
$menu_bookmarks_manage = "�������&nbsp;�������";
$menu_add_bookmark = "������&nbsp;�������";
$menu_add_current = "������ �&nbsp;��������&nbsp;��������";

$menu_invite = "������&nbsp;�����������";

$menu_search_discussions = "��������";
$menu_search_files = "�������&nbsp;�&nbsp;�����";
$menu_search_web = "The&nbsp;web";
$menu_scheduler = "�������";
$menu_filetypes = "������ �������";
$menu_discussions = "�����������&nbsp;�����";

$menu_users = "�����������";
$menu_start = "�������&nbsp;��������";
$menu_reboot = "�����������&nbsp;�������";
$menu_halt = "�������&nbsp;�������";
$menu_reboot_text = "��������! ������ �����-���� ������� �� ����� �������� ����������. ������� �� ��� �� ������ �� ����������?";
$menu_halt_text = "��������! ������ �����-���� ������� �� ����� ����������. ������� �� ��� �� ������ �� ����������?";
$menu_new = "���";
$menu_message = "���������";
$menu_contact = "�������";
$menu_event = "�������";
$menu_bookmark = "�������";
$menu_topics = "������";
$menu_support = "��������";
$menu_projects = "�������";
$menu_sysinfo = "�������� ����������";
$menu_admin = '�������������';
$menu_modules = '������';
$menu_manual = '��������';

//class validate
$error_required = "�� ��� ���������� ���������� ���� ������.";
$error_match_auth = "�������� �� ���������� �� ��������.";
$error_match_pass = "�������� �� ��������.";
$error_min_length = "������� ������� �� ������ ������:";
$error_max_length = "�������� ������� �� ������ ������:";
$error_email = "E-mail ������� ����� ��� ���������� � ���������.";
$error_email_exists = "E-mail ������� ����� ��� ���������� ���� ����������.";
$error_username_exists = "����������, ��������������� ��� ����� ��� ������ ���� ����������. ���� �������� �����.";
$errors_in_form = "��� ������ � �����������. ��������� �� � �������� ������.";
$error_missing_field = "�� ��� ��������� ������ �������� ������.";

//acl control
$acl_control_auth_groups = "����������� �����";
$acl_control_auth_users = "����������� �����������";
$acl_control_no_contacts = "������ ������� �������� �������� �� �� ��������.<br /> ����������� ��������� �� �� �������� ��������.";

$strShowProfile = "����� ������";
$strShowGroup = "����� ������� �� �������";
$strProcessingTime = "���� �� ���� �������� ���� ";
$strLoggedInAs = "������ ���� ";
$strAt = " �� ";
$strEmailTo = "������� E-mail ��: ";
$strProtectedUser = "������� ����������";

$http_not_found = "���������� �� � �������";
$http_not_found_text = "������� �������� �� � �������. ���� �� ��� �������� URL.";
$http_error = "������ �� ��������� ������";
$http_bad_request = "���� ������";
$strAddContact = "������ ��� ��������";

$strReadRights = "����� �� ������";
$strWriteRights = "����� �� ������";
$strPermissions = '�����';

$editor_bold = "�������";
$editor_italic = "������";
$editor_underline = "���������";
$editor_left = "����";
$editor_right = "�����";
$editor_center = "������";
$editor_ord_list = "������� ������";
$editor_bul_list = "��������� ������";
$editor_txt_color = "���� �� ������";
$editor_back_color = "���� �� ����";
$editor_hyperlink = "������";
$editor_image = "�����������";
$editor_rule = "������������ �������";

$editor_very_small = "����� �����";
$editor_small = "�����";
$editor_medium = "������";
$editor_large = "�����";
$editor_larger = "��-�����";
$editor_very_large = "����� �����";

$editor_strikethrough = "����������� ����";
$editor_outdent = "Outdent";
$editor_indent = "Indent";

$editor_superscript = "������";
$editor_subscript = "������";
$editor_undo = "������";
$editor_redo = "��������������";
$editor_copy = "��������";
$editor_paste = "��������";
$editor_cut = "���������";

$editor_font = "�����";
$editor_size = "������";
$editor_heading = "��������";
$editor_select = "��������";

$editor_mode = "Toggle editor mode";

$days[0]="��";
$days[1]="��";
$days[2]="��";
$days[3]="��";
$days[4]="��";
$days[5]="��";
$days[6]="��";

$months[0]="������";
$months[1]="��������";
$months[2]="����";
$months[3]="�����";
$months[4]="���";
$months[5]="���";
$months[6]="���";
$months[7]="������";
$months[8]="���������";
$months[9]="��������";
$months[10]="�������";
$months[11]="��������";

$full_days[0] = "������";
$full_days[1] = "����������";
$full_days[2] = "�������";
$full_days[3] = "�����";
$full_days[4] = "���������";
$full_days[5]= "�����";
$full_days[6] = "������";
$strLoadTime = '���������� �';
$strDescription = '��������';
$strTitle = '��������';


//translation required
$editorCmd['undo'] = "������";
$editorCmd['redo'] = "��������������";
$editorCmd['bold'] = "�������";
$editorCmd['italic'] = "������";
$editorCmd['underline'] = "���������";
$editorCmd['left'] = "���������� �� ����";
$editorCmd['right'] = "���������� �� �����";
$editorCmd['full'] = "�����";
$editorCmd['center'] = "��������";
$editorCmd['ord_list'] = "��������� ������";
$editorCmd['bul_list'] = "��������� ������";
$editorCmd['txt_color'] = "���������";
$editorCmd['back_color'] = "�������";
$editorCmd['hyperlink'] = "������";
$editorCmd['image'] = "������ �����������";
$editorCmd['rule'] = "������������ �������";
$editorCmd['table'] = "������ �������";

$editorCmd['strikethrough'] = "����������� ����";
$editorCmd['outdent'] = "Outdent";
$editorCmd['indent'] = "Indent";

$editorCmd['superscript'] = "������";
$editorCmd['subscript'] = "������";
$editorCmd['undo'] = "������";
$editorCmd['redo'] = "��������������";
$editorCmd['copy'] = "��������";
$editorCmd['paste'] = "��������";
$editorCmd['cut'] = "���������";

$editorCmd['font'] = "����";
$editorCmd['size'] = "������";
$editorCmd['heading'] = "��������";
$editorCmd['select'] = "��������";
$editorCmd['mode'] = "����� �� �������";
$editorCmd['textindicator'] = '����� ����';

//translation required
$lang_modules['addressbook'] = "������� �����";
$cmdShowAll = '������ ������';

$cmdSearch = '�����';
?>
