<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Anmelden";
$cmdSave = "Speichern";
$cmdBack = "Zur&uuml;ck";
$cmdCancel = "Abbrechen";
$cmdClose = "Schliessen";
$cmdOk = "Ok";
$cmdDelete = "L&ouml;schen";
$cmdNext = "Vor";
$cmdPrevious = "Zur&uuml;ck";
$cmdContinue = "Weiter";
$cmdUpdate = "Synchronisieren";
$cmdApply = "&Uuml;bernehmen";
$cmdYes = "Ja";
$cmdNo = "Nein";
$cmdAdd = 'Hinzuf&uuml;gen';
$cmdEdit = 'Bearbeiten';
$cmdInvertSelection = 'Auswahl umkehren';

$strEdit = "Bearbeiten";
$strThis = 'diese';
$strItems = 'Elemente';

$strName = "Name";
$strUser = "Benutzer";
$strUsername = "Benutzername";
$strPassword = "Passwort";
$strAuthcode = "Authorisierungscode";
$strCountry = "Land";
$strState = "Bundesland";
$strCity = "Stadt";
$strZip = "Postleitzahl";
$strAddress = "Strasse";
$strEmail = "Email";
$strPhone = "Telefon";
$strWorkphone = "Telefon berufl.";
$strCellular = "Mobil";
$strCompany = "Firma";
$strDepartment = "Abteilung";
$strFunction = "Funktion";
$strQuestion = "Geheime Frage";
$strAnswer = "Antwort";
$strFax = "Fax";
$strWorkFax = "Fax berufl.";
$strHomepage = "Homepage";
$strWorkAddress = "Strasse berufl.";
$strWorkZip = "PLZ berufl.";
$strWorkCountry = "Land berufl.";
$strWorkState = "Bundesland berufl.";
$strWorkCity = "Stadt berufl.";
$strPhoto = "Foto";
$strComments = "Kommentar";

$strProperties = 'Eigenschaften';

$strDeleteItem = "L&ouml;schen";

$strDate = "Datum";

$strSaveSuccess = "Daten erfolgreich gespeichert";
$strSaveError = "Beim Speichern der Daten ist ein Fehler aufgetreten. Bitte erneut versuchen.";
$strDataError = "Beim Lesen der Daten ist ein Fehler aufgetreten. Bitte versuchen Sie, die Seite neu zu laden.";
$strDeleteError = "Die Daten konnten nicht gel&ouml;scht werden.";
$strAclError = "Fehler in der Zugriffskontrolle";
$strDateError = "Sie haben ein ung&uuml;ltiges Datum eingegeben";



$strAccessDenied = "Zugriff verweigert";
$AccessDenied_text = "Keine Berechtigung.<br />Ein anderer Benutzer hat Ihnen eingeschr&auml;nkte Zugriffsrechte erteilt.";

$invalid_chars = "Die nachfolgenden Zeichen sind nicht erlaubt";

$strGroups = "Gruppen";
$strOwner = "Eigent&uuml;mer";

$strDeletePrefix = "Sind Sie sicher, dass Sie ";
$strDeleteSuffix = " löschen möchten?";

//menu
$menu_logout = "Abmelden";
$menu_applications = "Applikationen";
$menu_help = "Hilfe";
$menu_about = "&Uuml;ber";
$menu_documentation = "Dokumentation";
$menu_configuration = "Konfiguration";
$menu_groups = "Benutzer&nbsp;und&nbsp;Gruppen";
$menu_accounts = "Mein&nbsp;Account";
$menu_preferences = "Einstellungen";
$menu_bookmarks = "Lesezeichen";
$menu_bookmarks_manage = "Bearbeiten";
$menu_add_bookmark = "Hinzuf&uuml;gen";
$menu_add_current = "Aktuelle&nbsp;Seite&nbsp;hinzuf&uuml;gen";
$menu_contacts = "Adressbuch";
$menu_invite = "Benutzer&nbsp;einladen";

$menu_search_discussions = "Diskussionen";
$menu_search_files = "Dateien&nbsp;und&nbsp;Ordner";
$menu_search_web = "Das&nbsp;Web";
$menu_scheduler = "Termine";
$menu_filetypes = "Dateitypen";
$menu_discussions = "Diskussionen";

$menu_users = "Benutzer";
$menu_start = "Startseite";
$menu_reboot = "Server&nbsp;neu&nbsp;starten";
$menu_halt = "Server&nbsp;herunterfahren";
$menu_reboot_text = "WARNUNG! Alle Group-Office Dienste werden vorübergehend nicht zur Verfügung stehen. Möchten Sie trotz allem fortfahren?";
$menu_halt_text = "WARNUNG! Alle Group-Office Dienste werden beendet. Möchten Sie trotz allem fortfahren?";
$menu_new = "Neu";
$menu_message = "Nachricht";
$menu_contact = "Kontakt";
$menu_event = "Vorgang";
$menu_bookmark = "Lesezeichen";
$menu_topics = "Themen";
$menu_support = "Support";
$menu_projects = "Projekte";
$menu_sysinfo = "Systeminformation";
$menu_admin = 'Administrator';
$menu_modules = 'Module';
$menu_manual = 'Manual';

//class validate
$error_required = "Sie haben die Pflichtfelder nicht vollständig ausgefüllt.";
$error_match_auth = "Die Authorisierungscodes stimmen nicht überein.";
$error_match_pass = "Die Passwörter stimmen nicht überein.";
$error_min_length = "Die Mindestanzahl von Zeichen f&uuml;r das untere Feld ist:";
$error_max_length = "Die H&ouml;chstanzahl von Zeichen f&uuml;r das untere Feld ist:";
$error_email = "Die von Ihnen eingegebene  Email-Adresse ist ungültig.";
$error_email_exists = "Die von Ihnen eingegebene  Email-Adresse ist bereits vorhanden.";
$error_username_exists = "Leider ist der von Ihnen eingegebene Benutzername bereits vorhanden. Bitte wählen Sie einen anderen Benutzernamen.";
$errors_in_form = "Das Formular wurde nicht fehlerfrei ausgefüllt. Bitte korrigieren und erneut versuchen.";
$error_missing_field = "Sie haben nicht alle Pflichtfelder ausgefüllt.";

//acl control
$acl_control_auth_groups = "Authorisierte Gruppen";
$acl_control_auth_users = "Authorisierte Benutzer";
$acl_control_no_contacts = "Ihnen stehen keine Kontakte zur Auswahl.<br /> Bitte benutzen Sie das Adressbuch, um Mitglieder in Ihre Kontaktliste aufzunehmen.";

$strShowProfile = "Profil anzeigen";
$strShowGroup = "Gruppenmitglieder anzeigen";
$strProcessingTime = "Die Verarbeitung dieser Seite ben&ouml;tigte ";
$strLoggedInAs = "Angemeldet als ";
$strAt = " um ";
$strEmailTo = "Sende eine Email-Nachricht an: ";
$strProtectedUser = "Gesch&uuml;tzter Benutzer";

$http_not_found = "Dokument nicht gefunden";
$http_not_found_text = "Das angeforderte Dokument wurde nicht gefunden. M&ouml;glicherweise haben Sie die URL nicht korrekt eingegeben.";
$http_error = "Interner Serverfehler";
$http_bad_request = "Fehlerhafte Anforderung";
$strAddContact = "Zu den Kontakten hinzuf&uuml;gen";

$strReadRights = "Leseberechtigungen";
$strWriteRights = "Schreibberechtigungen";
$strPermissions = 'Berechtigungen';

$editor_bold = "Fett";
$editor_italic = "Kursiv";
$editor_underline = "Unterstrichen";
$editor_left = "Linksb&uuml;ndig";
$editor_right = "Rechtsb&uuml;ndig";
$editor_center = "Zentriert";
$editor_ord_list = "Nummerierung";
$editor_bul_list = "Aufz&auml;hlungszeichen";
$editor_txt_color = "Textfarbe";
$editor_back_color = "Hintergrundfarbe";
$editor_hyperlink = "Hyperlink";
$editor_image = "Bild";
$editor_rule = "Horizontale Linie";

$editor_very_small = "Sehr klein";
$editor_small = "Klein";
$editor_medium = "Normal";
$editor_large = "Gross";
$editor_larger = "Gr&ouml;sser";
$editor_very_large = "Sehr gross";

$editor_strikethrough = "Durchgestrichen";
$editor_outdent = "Einzug verkleinern";
$editor_indent = "Einzug vergr&ouml;ssern";

$editor_superscript = "Hochgestellt";
$editor_subscript = "Tiefgestellt";
$editor_undo = "R&uuml;ckg&auml;ngig";
$editor_redo = "Wiederherstellen";
$editor_copy = "Kopieren";
$editor_paste = "Einf&uuml;gen";
$editor_cut = "Ausschneiden";

$editor_font = "Schriftart";
$editor_size = "Gr&ouml;sse";
$editor_heading = "Titel";
$editor_select = "W&auml;hlen";

$editor_mode = "Editormodus umschalten";

$days[0]="So";
$days[1]="Mo";
$days[2]="Di";
$days[3]="Mi";
$days[4]="Do";
$days[5]="Fr";
$days[6]="Sa";

$months[0]="Januar";
$months[1]="Februar";
$months[2]="M&auml;rz";
$months[3]="April";
$months[4]="Mai";
$months[5]="Juni";
$months[6]="Juli";
$months[7]="August";
$months[8]="September";
$months[9]="Oktober";
$months[10]="November";
$months[11]="Dezember";

$full_days[0] = "Sonntag";
$full_days[1] = "Montag";
$full_days[2] = "Dienstag";
$full_days[3] = "Mittwoch";
$full_days[4] = "Donnerstag";
$full_days[5]= "Freitag";
$full_days[6] = "Samstag";

$strLoadTime = 'geladen in';
$strDescription = 'Beschreibung';
$cmdGenerate = 'Generieren';
$strNoHolidays = 'Keine Feiertage in der Datenbank';
$strUnknown = 'unbekannt';
$strYear = 'Jahr';
$strDeleteHolidaysPrefix = 'Sind Sie sicher, dass Sie alle Feiertage des Jahres ';
$strDeleteHolidaysSuffix = ' l&ouml;schen m&ouml;chten?';
$strReplaceHolidays = 'Bereits existierende Feiertage werden &uuml;berschrieben. Sind Sie sicher?';
$strTitle = 'Titel';

$editorCmd['bold'] = "Fett";
$editorCmd['italic'] = "Kursiv";
$editorCmd['underline'] = "Unterstrichen";
$editorCmd['left'] = "Linksb&uuml;ndig";
$editorCmd['right'] = "Rechtsb&uuml;ndig";
$editorCmd['full'] = "Block";
$editorCmd['center'] = "Zentriert";
$editorCmd['ord_list'] = "Nummerierung";
$editorCmd['bul_list'] = "Aufz&auml;hlung";
$editorCmd['txt_color'] = "Textfarbe";
$editorCmd['back_color'] = "Hintergrundfarbe";
$editorCmd['hyperlink'] = "Hyperlink";
$editorCmd['image'] = "Bild";
$editorCmd['rule'] = "Horizontale Linie";
$editorCmd['table'] = "Tabelle";

$editorCmd['strikethrough'] = "Durchgestrichen";
$editorCmd['outdent'] = "Einzug verkleinern";
$editorCmd['indent'] = "Einzug vergr&ouml;ssern";

$editorCmd['superscript'] = "Hochgestellt";
$editorCmd['subscript'] = "Tiefgestellt";
$editorCmd['undo'] = "R&uuml;ckg&auml;ngig";
$editorCmd['redo'] = "Wiederherstellen";
$editorCmd['copy'] = "Kopieren";
$editorCmd['paste'] = "Einf&uuml;gen";
$editorCmd['cut'] = "Ausschneiden";

$editorCmd['font'] = "Schriftart";
$editorCmd['size'] = "Gr&ouml;sse";
$editorCmd['heading'] = "Titel";
$editorCmd['select'] = "W&auml;hlen";
$editorCmd['mode'] = "Editormodus umschalten";
$editorCmd['textindicator'] = 'Aktueller Stil';
$cmdShowAll = 'Alle anzeigen';

$cmdSearch = "Suchen";

//translation required
$cmdYesToAll = 'Ja zu allem';
$cmdNoToAll = 'Nein zu allem';

$short_months[0]="Jan";
$short_months[1]="Feb";
$short_months[2]="Mär";
$short_months[3]="Apr";
$short_months[4]="Mai";
$short_months[5]="Jun";
$short_months[6]="Jul";
$short_months[7]="Aug";
$short_months[8]="Sep";
$short_months[9]="Okt";
$short_months[10]="Nov";
$short_months[11]="Dez";

$strToday = 'Heute';

$strShortWeek = 'Wo';

$cal_prev_year = "Jahr zur&uuml;ck (halten f&uuml;rs Menu)";
$cal_prev_month  = "Monat zur&uuml;ck (halten f&uuml;rs Menu)";
$cal_go_today = "Gehe zu heute";
$cal_next_month = "Monat vor (halten f&uuml;rs Menu)";
$cal_next_year  = "Jahr vor (halten f&uuml;rs Menu)";
$cal_drag_to_move = "zum Verschieben ziehen";
$cal_part_today = " (Heute)";
$cal_monday_first = "mit Montag beginnen";
$cal_sunday_first  = "mit Sonntag beginnen";
$cal_select_date = "Datum w&auml;hlen";

$cmdPrint = 'Drucken';
$strFirstName = 'Vorname';
$strLastName = 'Nachname';
$strInitials = 'Initialen';
$strSex = 'Geschlecht';
$strBirthday = 'Geburtstag';
$strSexes['M'] = 'm&auml;nnlich';
$strSexes['F'] = 'weiblich';

$strTitle = 'Titel';

$strBeginning = 'Beginn';

$strMiddleName = '2. Vorname';

$sir_madam['M'] = 'Herr';
$sir_madam['F'] = 'Frau';

$strModifiedAt = 'Ge&auml;ndert am';
$strCreatedAt = 'Erzeugt am';

$htmlarea_language_name = 'de';

$strLocalization = 'Lokalit&auml;t';
//look up the language in controls/jscalendar/lang
$jscalendar_language_name = 'calendar-de.js';
//look up language in 'classes/phpmailer/language'
$php_mailer_lang = 'de';
$strEmailFail = 'Die E-Mail konnte nicht versandt werden.';

//for people that want to use ktmlarea
$ktml_language = 'deutsch';
$ml_host_unreachable = ' ist nicht erreichbar. Die automatische E-Mail-Abfrage wird deaktiviert.';

$strRegion = 'Region';

$lang_modules['addressbook'] = "Adressbuch";
$lang_modules['filesystem'] = "Dateien";
$lang_modules['email'] = "Email";
$lang_modules['calendar'] = "Kalender";
$lang_modules['contacts'] = "Adressbuch";
$lang_modules['bookmarks'] = "Lesezeichen";
$lang_modules['notes'] = "Notizen";
$lang_modules['projects'] = "Projekte";
$lang_modules['search'] = "Suchen";
$lang_modules['scheduler'] = "Termine";
$lang_modules['cms'] = "Cms";
$lang_modules['summary'] = "&Uuml;bersicht";
$lang_modules['todos'] = "Aufgaben";
$lang_modules['example'] = "Beispiel";
$lang_modules['comics'] = "Comics";
$lang_modules['daogen'] = "Daogen";
$lang_modules['news'] = "News";
$lang_modules['wiki'] = "Wiki";

//2.05
$strMyName = 'Mein Name';
$cmdReset = 'Zur&uuml;cksetzen';
?>
