<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "��������";
$strRegistrationDate = "���� �� �����������";
$strUsers = "����������(�)";
$user_profile = "������ �� �����������";
$ac_login_info = "���������� �� ���������";
$ac_registration_time = "����� �� �����������";
$ac_lastlogin = "�������� �������";
$ac_logins = "���� ��������";
$strMaxOf = "����������� �� ��������";
$strUsername = "������������� ���";
$delete_fail = "�� ������ �� �������� ��� � �����������������.";
$admin_new_user = "��� ����������";

$admin_modules = '������';
$admin_module_access = '���� ���������� ��� ������ �� �������� ������:';

$admin_password = '������';
$admin_confirm_password = '������� ������';
$admin_change_password = '��������� ������';
$admin_pass_match = '�������� �� �������. ���� �������� ������.';

//registration
$registration_personal_info = "���������� ����������";
$registration_account_info = "���������� �� �������";
$registartion_username_text = "�������� ������������� ��� � ������ �� ���� ���������� �� �� ������� � �����.";

$registration_question_text=<<<registration_question_text
�������� ���� ������ �� ����� ���� ���� ���������� ���� ��������. �� ������� ���� ������ � ������ �� ����������� ������� ��������/������� ������.
������ � ����� �������� �������, �� ���� �������� e-mail �� ������������� e-mail ����� � ��������������� ��� � ���� ������ �� �������.
registration_question_text;

$registration_text=<<<registration_text
��������� ���� ����� � ��������� 'Ok' �� �� �� ������������. �������� ��������� � * �� ������������.<br />
������������ ����� ������������ � �� ���������� �� Group-Office ���� �� ��� ���� �� � ������������ �� ����� ����.
registration_text;

$registartion_authcode_text = "�������� ��� �� ����������. ����� �� ���������� � ��� �� ����� ������� ����������� �� ���� ����� �� �� ������� ���� ���������� ��� ������ ������ � �������� � �� �� ����� ������ �������.";
$registartion_confirm_authcode = "���������� ��� �� ����������";
$registration_visibility = "������� ���� ���������� ����� �� ������ �����������.";
$registration_success = "<h1>������������!</h1>��� ��� ����������� ������� � ����������� �� ����� ����� ���� ��������� ��:";
$registration_failure = "���������� � ������ ������ �� ������������ ������ ����������. ���� �������� ������.";
$registration_mail_subject = "����� ����� � Group-Office.";
$registration_folder_error = "����������� �� ����� ����� �����������. �������� �� � ��� �� �� ������������ ����!";
$registration_title = "������������� �� � Group-Office";

$registration_email = "�������� E-mail ����� �� ���� ���������� � Group-Office.";
$registration_samba = "���� & ����� ������.";
$registration_email_error = "���������� � ������ ��� ����������� �� ����� E-mail ������. �������� �� � ��� �� ������ ����������.";
$strPasswordConfirm = "������� ������";
$strPassword = "������";

$reg_company_info = "���������� �� ����������";

$max_user_limit = "����������� ���� ����������� � ���������.";
$max_users_text = "����������, ��� �� ������ �� ������������ ������ �����������. �������� �� � ��� ��� ������ �� ��������� ������ ���� �����������.";
$ml_sent_items = "��������� ��������";
$ml_spam = "����";
$ac_samba_user = "����������� ��� ������ �� ����� � ����� �������.";
$ml_email_only = "���� �� ������� ������ ������ �������� E-mail �����";

//translation required
$register_new_user = '��� Group-Office ���������';
$register_thanks = '<h2>��������� ��</h2>. ������ ����������� �� ������� �������� ���-�����. �� �������� ����������� ����������.';
$register_accept = '������ ��� �����������';
$register_addressbook = '������ ��� adressbook';
$register_title = '���������';

//modified:
$registration_mail_body=<<<registration_mail_body
����� �-�/�-�� %name%,

��� ��� ������������ � Group-Office.
�������� �� �� �� �������� ������� �� ������ ������.

��� ���� �� ������� ��� �������� �����:

���: %username%
������: %password%

��������� �� �������� ��-���� �� �� �������:

registration_mail_body;
?>
