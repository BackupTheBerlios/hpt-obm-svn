<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Logins ialt";
$strRegistrationDate = "Oprettet";
$strUsers = "bruger(e)";
$user_profile = "Bruger profil";
$ac_login_info = "Login information";
$ac_registration_time = "Oprettet";
$ac_lastlogin = "Sidste login";
$ac_logins = "Totalt antal logins";
$strMaxOf = "registered from the maximum of";
$strUsername = "Brugernavn";
$delete_fail = "Du kan ikke slette dit eget brugernavn eller superuser.";
$admin_new_user = "Ny bruger";

$admin_modules = 'Moduler';
$admin_module_access = 'Denne bruger har adgang til følgende moduler';

$admin_password = 'Adgangskode';
$admin_confirm_password = 'Bekræft adgangskode';
$admin_change_password = 'Skift adgangskode';
$admin_pass_match = 'Adgangskoderne er ikke ens.';

//registration
$registration_personal_info = "Personlig information";
$registration_account_info = "Konto information";
$registartion_username_text = "Indtast brugernavn og adgangskode for denne bruger.";

$registration_question_text=<<<registration_question_text
Indtast et hemmeligt spørgsmål, som kun brugeren kender svaret på. Spørgsmålet bruges i tilfælde af at brugeren har glemt sit password.
Hvis det rigtige svar gives, sendes der en besked til den registrerede mailadresse, med brugernavn og en ny adgangskode der kan bruges til login.
registration_question_text;

$registration_text=<<<registration_text
Indtast information og klik 'OK' for at registrere en ny bruger. Felter markeret med * skal udfyldes.<br />
Information der indtastes bruges kun i Group-Office og vil ikke blive videregivet til andre.
registration_text;

$registartion_authcode_text = "Indtast en autorisationskode. Koden skal bruges af andre brugere, hvis de ønsker at tilføje denne bruger til kontaktlisten og se de indtastede persondata.";
$registartion_confirm_authcode = "Bekræft autorisationskode";
$registration_visibility = "Gør brugeren synlig for alle andre brugere.";
$registration_success = "<h1>Tillykke!</h1>Du er nu registreret og der er sendt en velkomstbesked til:";
$registration_failure = "Der opstod en fejl ved registrering af dine personinformationer. Prøv igen.";
$registration_mail_subject = "Velkommen til Group-Office.";
$registration_folder_error = "Fejl under oprette af din personlige mappe. Kontakt systemadministratoren!";
$registration_title = "Registrering i Group-Office";
$registration_mail_body=<<<registration_mail_body
Kære %name%,

Holdet bag Group-Office vil gerne byde dig velkommen.
Vi håber du finder systemet brugbart. Send gerne tips om forbedringer til os via email, så vi kan gøre systemet endnu bedre.


God fornøjelse med systemet!

Klik på genvejen herunder for at logge ind:

registration_mail_body;
$registration_email = "Opret en mailadresse for denne Group-Office bruger.";
$registration_samba = "Fil & Print server.";
$registration_email_error = "Der opstod en fejl under oprettelsen af din brugerkonto. Kontakt systemadministratoren.";
$strPasswordConfirm = "Bekræft adgangskode";
$strPassword = "Adgangskode";

$reg_company_info = "Firmainformation";

$max_user_limit = "Det maksimale antal brugere er nået.";
$max_users_text = "Beklager, der kan ikke registreres flere brugere. Kontakt systemadministratoren hvis du ønsker at øge antallet af brugere.";
$ml_sent_items = "Sendte beskeder";
$ml_spam = "Spam";
$ac_samba_user = "Brugerem har adgang til fil og print serveren.";
$ml_email_only = "Dette kræves kun hvis du ikke opretter en mailadresse";
?>
