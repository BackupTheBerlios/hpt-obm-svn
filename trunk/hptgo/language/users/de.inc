<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Anmeldungen";
$strRegistrationDate = "Registrierungs-Datum";
$strUsers = "Benutzer";
$user_profile = "Benutzerprofil";
$ac_login_info = "Anmeldeinformation";
$ac_registration_time = "Registrierungs-Zeit";
$ac_lastlogin = "Letzte Anmeldung";
$ac_logins = "Anzahl Anmeldungen";
$strMaxOf = "registriert von moeglichen";
$strUsername = "Benutzername";
$delete_fail = "Sich selbst bzw. den Superuser duerfen Sie nicht loeschen.";
$admin_new_user = "Neuer Benutzer";

$admin_modules = 'Module';
$admin_module_access = 'Dieser Benutzer soll eine Zugriffsberechtigung auf nachfolgende Module erhalten:';

$admin_password = 'Passwort';
$admin_confirm_password = 'Passwort wiederholen';
$admin_change_password = 'Passwort aendern';
$admin_pass_match = 'Keine Uebereinstimmung der Passworte. Bitte erneut versuchen.';

//registration
$registration_personal_info = "Persoenliche Informationen";
$registration_account_info = "Zugangsinformationen";
$registartion_username_text = "Bitte Benutzername und Passwort des Benutzers fuer die Anmeldung festlegen.";

$registration_question_text=<<<registration_question_text
Bitte hier eine geheime Frage hinterlegen, welche nur der Benutzer beantworten kann. Sollte der Benutzer sein Passwort vergessen,
wird diese Frage an den Benutzer gerichtet. Bei korrekter Beantwortung der Frage erhaelt der Benutzer eine Email mit neuen Anmeldeinformationen.
registration_question_text;

$registration_text=<<<registration_text
Bitte dieses Formular ausfuellen und auf die Schaltflaeche 'Ok' klicken, um die Registrierung abzuschliessen. Mit einem * markierte Felder sind Pflichtfelder.<br />
Die eingegebenen Informationen werden lediglich innerhalb von Group-Office verwendet und werden nicht an dritte weitergereicht.
registration_text;

$registartion_authcode_text = "Bitte Authorisierungscode eingeben. Der Authorisierungscode ermoeglicht anderen Benutzern die Einsicht in diese Benutzerinformationen und Eintragung des Benutzers in die eigene Kontaktliste.";
$registartion_confirm_authcode = "Authorisierungscode wiederholen";
$registration_visibility = "Dieser Benutzer soll fuer alle Benutzer sichtbar sein.";
$registration_success = "<h1>Herzlichen Glueckwunsch!</h1>Die Registrierung war erfolgreich. Eine Willkommens-Email wurde versendet an:";
$registration_failure = "Waehrend der Registrierung ist ein Fehler aufgetreten. Versuchen Sie es bitte erneut.";
$registration_mail_subject = "Willkommen in Group-Office.";
$registration_folder_error = "Beim Anlegen des persoenlichen Ordners ist ein Fehler aufgetreten. Bitte an uns wenden, um eine Wiederherstellung vornehmen zu lassen!";
$registration_title = "Registrierung in Group-Office";

$registration_email = "Eine Email-Adresse fuer diesen Benutzer in Group-Office einrichten.";
$registration_samba = "datei- und druckdienste.";
$registration_email_error = "Beim Anlegen Ihres Zugangs ist ein Fehler aufgetreten. Bitte setzen Sie sich mit uns in Verbindung, um Support zu erhalten.";
$strPasswordConfirm = "Passwort wiederholen";
$strPassword = "Passwort";

$reg_company_info = "Firmen-Information";

$max_user_limit = "Die hoechstmoegliche Benutzeranzahl ist erreicht.";
$max_users_text = "Verzeihung, aber es koennen keine neuen Benutzer registriert werden. Kontaktieren Sie uns, wenn Sie die Benutzeranzahl erhoeht haben moechten.";
$ml_sent_items = "Gesendet";
$ml_spam = "Spam";
$ac_samba_user = "Benutzer ist berechtigt auf Datei- und Druck-Server zuzugreifen.";
$ml_email_only = "Nur notwendig, wenn Email-Adresse bereits vorhanden";

$register_new_user = 'Neuer Group-Office Abonnent';
$register_thanks = '<h2>Vielen dank</h2>. Ihr Abonnement wird so schnell wie moeglich bearbeitet. Sie werden ueber weitere Schritte informiert werden.';
$register_accept = 'Zu den Benutzern hinzufuegen';
$register_addressbook = 'Zum Adressbuch hinzufuegen';
$register_title = 'Abonnieren';

$registration_mail_body=<<<registration_mail_body
Sehr geehrte(r) %sex% %last_name%,

ein Benutzerkonto fuer Sie ist unter Group-Office eingerichtet.
Wir hoffen darauf, dass Sie Freude mit Group-Office haben werden.

Nachfolgend finden Sie Ihre Zugangsdaten fuer die Anmeldung:

Benutzername: %username%
Passwort: %password%

Bitte den unteren Link anklicken, um sich anzumelden:

registration_mail_body;

$reg_language = 'Sprache';
$reg_theme = 'Thema';

$admin_use = 'Benutze';
$admin_module = 'Modul';
$admin_manage = 'Verwalten';

$admin_groups = 'Benutzergruppen';
$admin_groups_user = 'Der Benutzer ist Mitglied in den nachfolgenden Benutzergruppen';
?>
