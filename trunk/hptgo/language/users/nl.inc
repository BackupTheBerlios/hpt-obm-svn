<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Logins";
$strRegistrationDate = "Registratie Datum";
$strUsedDiskspace = "Gebruikte Schijfruimte";
$strUsers = "gebruiker(s)";
$user_profile = "Gebruikersprofiel";
$ac_login_info = "Inlog gegevens";
$ac_registration_time = "Registratietijd";
$ac_lastlogin = "Laaste login";
$ac_logins = "Aantal logins";
$strMaxOf = "geregistreerd van het maximum van";
$strUsername = "Gebruikersnaam";
$delete_fail = "U kunt uzelf en de hoofdgebruiker niet verwijderen.";
$admin_new_user = "Nieuwe gebruiker";

$admin_modules = 'Modules';
$admin_module_access = 'Deze gebruiker heeft toegang tot de volgende modules:';

$admin_password = 'Wachtwoord';
$admin_confirm_password = 'Bevestig wachtwoord';
$admin_change_password = 'Wachtwoord veranderen';
$admin_pass_match = 'De wachtwoorden kwamen niet overeen. Probeer het opnieuw.';

//registration
$registration_personal_info = "Persoonlijke informatie";
$registration_account_info = "Account informatie";
$registartion_username_text = "Voer een gebruikersnaam en wachtwoord voor deze gebruiker om op deze site in te loggen";

$registration_question_text=<<<registration_question_text
Voer een geheime vraag in waar alleen deze gebruiker antwoord op weet. Deze vraag wordt aan deze gebruiker gesteld indien de gebruiker zijn of haar wachtwoord is vergeten.
Er wordt dan een E-mail vestuurd naar het geregistreerde E-mail adres met een nieuw wachtwoord.
registration_question_text;

$registration_text=<<<registration_text
Vul dit formulier in en klik op 'Ok' om te registreren. De velden gemarkeerd met een * zijn vereist.<br />
De informatie die u verstrekt is alleen voor het gebruik bij Group-Office en zullen niet aan derden worden verstrekt.
registration_text;

$registartion_authcode_text = "Voer een aurhorisatiecode in. De authorisatie code kan worden gebruikt om contacten toe te voegen. Wanneer de gebruiker deze verstrekt aan een ander lid. Worden zij zichtbaar voor elkaar.";
$registartion_confirm_authcode = "Bevestig auhorisatiecode";
$registration_visibility = "Maak deze gebruiker zichtbaar voor alle gebruikers.";
$registration_success = "<h1>Gefeliciteerd!</h1>De gebruiker is succesvol geregistreerd en een welkomst bericht is verstuurd naar:";
$registration_failure = "Er is een fout opgetreden bij het registreren. Probeer het opnieuw.";
$registration_mail_subject = "Welkom bij Group-Office.";
$registration_folder_error = "Fout bij het maken van de persoonlijke map. Neem contact met ons op om dit te herstellen!";
$registration_title = "Registreer bij Group-Office";
$registration_mail_body=<<<registration_mail_body
Geachte %sex% %middle_name%%last_name%,

U bent geregistreerd in het Group-Office systeem.
Wij wensen u veel success en plezier bij deze service.

U kunt inloggen met de volgende gegevens:

Gebruikersnaam: %username%
Wachtwoord: %password%

Klik op onderstaande link om in te loggen (of plak hem in de adresbalk van uw browser).

registration_mail_body;

$registration_email = "Maak een E-mail adres voor deze gebruiker aan op Group-Office.";
$registration_samba = "File & Print server.";
$registration_email_error = "Er is een fout opgetreden bij het maken van uw email account. Neem contact met ons op voor ondersteuning.";
$max_user_limit = "Maximaal aantal gebruikers bereikt";
$max_users_text = "Helaas kunt u geen gebruikers meer registreren. Neem contact met ons op wanneer u het aantal gebruikers wilt uitbreiden.";
$strPasswordConfirm = "Bevestig wachtwoord";
$strPassword = "Wachtwoord";
$ml_sent_items = "Verzonden items";
$ml_spam = "Spam";
$ac_samba_user = "Gebruiker heeft toegang tot de file & printserver.";

$reg_company_info = "Bedrijfsinformatie";
$ml_email_only = 'Alleen vereist wanneer geen E-mail wordt aangemaakt';

$register_new_user = 'Nieuwe aanmelding bij Group-Office';
$register_title = 'Aanmelden';
$register_thanks = '<h2>Dank u wel</h2>Uw aanmelding wordt zo snel mogelijk in behandeling genomen. U ontvangt hierover nader bericht.';
$register_accept = 'Aan gebruikers toevoegen';
$register_addressbook = 'Aan adresboek toevoegen';

$reg_language = 'Taal';
$reg_theme = 'Thema';

//2.05
$admin_use = 'Gebruiken';
$admin_module = 'Module';
$admin_manage = 'Beheren';

$admin_groups = 'Gebruikersgroepen';
$admin_groups_user = 'Gebruiker behoort tot de volgende gebruikersgroepen';

?>
