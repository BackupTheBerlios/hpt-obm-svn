<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Inloggningar";
$strRegistrationDate = "Registreringsdatum";
$strUsers = "användare";
$user_profile = "Användarprofile";
$ac_login_info = "Logininformation";
$ac_registration_time = "Registrationstid";
$ac_lastlogin = "Senast inloggad";
$ac_logins = "Total logins";
$strMaxOf = "registered from the maximum of";
$strUsername = "Username";
$delete_fail = "Du kan ej radera dig själv och superusern.";
$admin_new_user = "Ny användare";

$admin_modules = 'Moduler';
$admin_module_access = 'Denna användare har tillgång till följande moduler:';

$admin_password = 'Lösenord';
$admin_confirm_password = 'Bekräfta lösenord';
$admin_change_password = 'Byt lösenord';
$admin_pass_match = 'Lösenorden matchade inte. Var god försök igen.';

//registration
$registration_personal_info = "Personlig information";
$registration_account_info = "Kontoinformation";
$registartion_username_text = "Ange ett användarnamn och lösenord för denna användare för att logga in till
systemet.";

$registration_question_text=<<<registration_question_text
Ange en hemlig fråga som endast denna användare vet svaret till. Vi kommer att ställa denna fråga ifall
användaren glömmer bort sitt lösenord.
När rätt svar angivits, så kommer ett e-post att skickas till dennes epostadress med användarnamnet och ett nytt
lösenord.
registration_question_text;

$registration_text=<<<registration_text
Fyll i detta formulär och tryck på 'ok' för att registrera. Fält markerade med * är nödvändiga.<br />
Informationen du anger är endast för användning av Group-Office, och den kommer inte att spridas vidare
till obehöriga.
registration_text;

$registartion_authcode_text = "Ange authensieringskod. Authensieringskoden är en kod andra användare behöver för
att lägga till denna användare till deras kontaktlistor.";
$registartion_confirm_authcode = "Konfirmera authensieringskod";
$registration_visibility = "Gör denna användare synlig för alla.";
$registration_success = "<h1>Grattis!</h1>Du har blivit registrerad och ett välkomstmeddelande har skickats
till:";
$registration_failure = "Ett fel uppstod när informationen skulle sparas. Vad god försök igen.";
$registration_mail_subject = "Välkommen till Group-Office.";
$registration_folder_error = "Misslyckades med att skapa personlig mapp. Kontakta oss för att återgärda detta!";
$registration_title = "Registra dig vid Group-Office";
$registration_mail_body=<<<registration_mail_body
Hejsan %name%,

Du har registrerats ett konto på Group-Office.
Vi hoppas att du kommer att ha nytta av våra tjänster!

Tryck på länken nedan för att logga in:

registration_mail_body;
$registration_email = "Skapa en epostadress för denna användare på Group-Office.";
$registration_samba = "Fil & Skrivarserver.";
$registration_email_error = "Ett fel uppstod när epostkontot skulle skapas. Kontakta oss för support.";
$strPasswordConfirm = "Bekräfta lösenord";
$strPassword = "Lösenord";

$reg_company_info = "Företagsinformation";

$max_user_limit = "Maximalt antal registrerad användare har nåtts.";
$max_users_text = "Tyvärr, du kan ej registrera fler användare. Kontakta oss ifall du vill öka antalet av dina
användare.";
$ml_sent_items = "Skickade saker";
$ml_spam = "Spam";
$ac_samba_user = "Användaren har tillgång till fil och skrivarsserver.";
$ml_email_only = "Endast nödvändig när du inte skapar epostadresser";
?>
