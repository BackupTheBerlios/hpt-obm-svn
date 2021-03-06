<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Inloggningar";
$strRegistrationDate = "Registreringsdatum";
$strUsers = "anv�ndare";
$user_profile = "Anv�ndarprofile";
$ac_login_info = "Logininformation";
$ac_registration_time = "Registrationstid";
$ac_lastlogin = "Senast inloggad";
$ac_logins = "Total logins";
$strMaxOf = "registered from the maximum of";
$strUsername = "Username";
$delete_fail = "Du kan ej radera dig sj�lv och superusern.";
$admin_new_user = "Ny anv�ndare";

$admin_modules = 'Moduler';
$admin_module_access = 'Denna anv�ndare har tillg�ng till f�ljande moduler:';

$admin_password = 'L�senord';
$admin_confirm_password = 'Bekr�fta l�senord';
$admin_change_password = 'Byt l�senord';
$admin_pass_match = 'L�senorden matchade inte. Var god f�rs�k igen.';

//registration
$registration_personal_info = "Personlig information";
$registration_account_info = "Kontoinformation";
$registartion_username_text = "Ange ett anv�ndarnamn och l�senord f�r denna anv�ndare f�r att logga in till
systemet.";

$registration_question_text=<<<registration_question_text
Ange en hemlig fr�ga som endast denna anv�ndare vet svaret till. Vi kommer att st�lla denna fr�ga ifall
anv�ndaren gl�mmer bort sitt l�senord.
N�r r�tt svar angivits, s� kommer ett e-post att skickas till dennes epostadress med anv�ndarnamnet och ett nytt
l�senord.
registration_question_text;

$registration_text=<<<registration_text
Fyll i detta formul�r och tryck p� 'ok' f�r att registrera. F�lt markerade med * �r n�dv�ndiga.<br />
Informationen du anger �r endast f�r anv�ndning av Group-Office, och den kommer inte att spridas vidare
till obeh�riga.
registration_text;

$registartion_authcode_text = "Ange authensieringskod. Authensieringskoden �r en kod andra anv�ndare beh�ver f�r
att l�gga till denna anv�ndare till deras kontaktlistor.";
$registartion_confirm_authcode = "Konfirmera authensieringskod";
$registration_visibility = "G�r denna anv�ndare synlig f�r alla.";
$registration_success = "<h1>Grattis!</h1>Du har blivit registrerad och ett v�lkomstmeddelande har skickats
till:";
$registration_failure = "Ett fel uppstod n�r informationen skulle sparas. Vad god f�rs�k igen.";
$registration_mail_subject = "V�lkommen till Group-Office.";
$registration_folder_error = "Misslyckades med att skapa personlig mapp. Kontakta oss f�r att �terg�rda detta!";
$registration_title = "Registra dig vid Group-Office";
$registration_mail_body=<<<registration_mail_body
Hejsan %name%,

Du har registrerats ett konto p� Group-Office.
Vi hoppas att du kommer att ha nytta av v�ra tj�nster!

Tryck p� l�nken nedan f�r att logga in:

registration_mail_body;
$registration_email = "Skapa en epostadress f�r denna anv�ndare p� Group-Office.";
$registration_samba = "Fil & Skrivarserver.";
$registration_email_error = "Ett fel uppstod n�r epostkontot skulle skapas. Kontakta oss f�r support.";
$strPasswordConfirm = "Bekr�fta l�senord";
$strPassword = "L�senord";

$reg_company_info = "F�retagsinformation";

$max_user_limit = "Maximalt antal registrerad anv�ndare har n�tts.";
$max_users_text = "Tyv�rr, du kan ej registrera fler anv�ndare. Kontakta oss ifall du vill �ka antalet av dina
anv�ndare.";
$ml_sent_items = "Skickade saker";
$ml_spam = "Spam";
$ac_samba_user = "Anv�ndaren har tillg�ng till fil och skrivarsserver.";
$ml_email_only = "Endast n�dv�ndig n�r du inte skapar epostadresser";
?>
