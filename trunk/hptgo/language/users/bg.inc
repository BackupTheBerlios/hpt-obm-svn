<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Влизания";
$strRegistrationDate = "Дата на регистрация";
$strUsers = "потребител(и)";
$user_profile = "Профил на Потребителя";
$ac_login_info = "Информация за влизането";
$ac_registration_time = "Време на регистрация";
$ac_lastlogin = "Последно влизане";
$ac_logins = "Общо влизания";
$strMaxOf = "регистриран от максимум";
$strUsername = "Потребителско име";
$delete_fail = "Не можете да изтриете вас и суперпотребителят.";
$admin_new_user = "Нов потребител";

$admin_modules = 'Модули';
$admin_module_access = 'Този потребител има достъп до следните модули:';

$admin_password = 'Парола';
$admin_confirm_password = 'Повтори парола';
$admin_change_password = 'Променете парола';
$admin_pass_match = 'Паролата не съвпада. Моля опитайте отново.';

//registration
$registration_personal_info = "Персонална информация";
$registration_account_info = "Информация за акаунта";
$registartion_username_text = "Въведете потребителско име и парола за този потребител за да влезете в сайта.";

$registration_question_text=<<<registration_question_text
Въведете таен въпрос на който само този потребител знае отговора. Ще зададем този въпрос в случай че потребителя забрави неговата/нейната парола.
Когато е даден правилен отговор, ще бъде изпратен e-mail на регистрирания e-mail адрес с потребителското име и нова парола за влизане.
registration_question_text;

$registration_text=<<<registration_text
Попълнете тази форма и натиснете 'Ok' за да се регистрирате. Полетата маркирани с * за задължителни.<br />
Информацията която предоставяте е за използване от Group-Office така че ние няма да я предоставяме на трети лица.
registration_text;

$registartion_authcode_text = "Въведете код за оторизация. Кодът за оторизация е код от който другите потребители ще имат нужда за да добавят този потребител към техния списък с контакти и за да видят вашите детайли.";
$registartion_confirm_authcode = "Потвърдете код за оторизация";
$registration_visibility = "Направи този потребител видим за всички потребители.";
$registration_success = "<h1>Поздравления!</h1>Вие сте регистриран успешно и съобщението за добре дошъл беше изпратено до:";
$registration_failure = "Възникнала е грешка докато се регистрираше вашата информация. Моля опитайте отново.";
$registration_mail_subject = "Добре дошли в Group-Office.";
$registration_folder_error = "Създаването на лична папка недовършено. Свържете се с нас за да възстановите това!";
$registration_title = "Регистрирайте се в Group-Office";

$registration_email = "Създайте E-mail адрес за този потребител в Group-Office.";
$registration_samba = "Файл & Принт сървър.";
$registration_email_error = "Възникнала е грешка при създаването на вашия E-mail акаунт. Свържете се с нас за повече информация.";
$strPasswordConfirm = "Повтори парола";
$strPassword = "Парола";

$reg_company_info = "Информация за компанията";

$max_user_limit = "Максималния брой потребители е достигнат.";
$max_users_text = "Съжаляваме, вие не можете да регистрирате повече потребители. Свържете се с нас ако искате да увеличите вашият брой потребители.";
$ml_sent_items = "Изпратени предмети";
$ml_spam = "Спам";
$ac_samba_user = "Потребителя има достъп до файла и принт сървъра.";
$ml_email_only = "Само се изисква когато нямате създаден E-mail адрес";

//translation required
$register_new_user = 'Нов Group-Office подписвач';
$register_thanks = '<h2>Благодаря Ви</h2>. Вашата регистрация ще протече възможно най-скоро. Ще получите понататъшна информация.';
$register_accept = 'Добави към потребители';
$register_addressbook = 'Добави към adressbook';
$register_title = 'Подписвам';

//modified:
$registration_mail_body=<<<registration_mail_body
Скъпи г-н/г-жо %name%,

Вие сте регистрирани в Group-Office.
Надяваме се че ще останете доволни от нашите услуги.

Вие може да влезете със следните данни:

Име: %username%
Парола: %password%

Натиснете на връзката по-долу за да влезете:

registration_mail_body;
?>
