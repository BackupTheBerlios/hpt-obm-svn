<?php
require($GO_LANGUAGE->get_fallback_language_file('addressbook'));

//contacts
$contacts_title = "Adressbuch";
$contacts_no_contacts = "Keine Kontakte vorhanden.";
$contacts_members = "Mitglieder";
$contacts_contacts = "Kontakte";
$contacts_add = "Neuer Kontakt";
$contacts_groups = "Gruppen";
$contacts_other = "Sonstige Kontakte";
$contacts_move_to_group = "Verschiebe ausgew&auml;hlte Kontakte in die Gruppe...";
$groups_no_groups = "Es existieren keine Gruppen";
$contacts_group = "Gruppe";
$contacts_empty_group = "Bitte die Dropdown-Box benutzen, um selektierte Kontakte in eine Gruppe zu verschieben.";
$contacts_this_group = 'Diese Gruppe';

$add_contacts_text = "Bitte die Email und den Authorisierungscode eines Mitglieds eingeben und auf 'ok' klicken";
$add_contacts_success = "Benutzer erfolgreich hinzugef&uuml;gt";
$add_contacts_wrong = "Entweder die Email-Adresse oder der Authorisierungscode ist falsch.";
$add_personal_text = "Bitte klicken Sie auf 'ok', um den Kontakt hinzuzuf&uuml;gen. Die mit * gekennzeichneten Felder m&uuml;ssen ausgef&uuml;llt werden.";
$contacts_select = "Kontakte ausw&auml;hlen und auf 'ok' klicken.";
$contacts_select_title = "Kontakte";
$contact_profile = "Kontaktprofil";
$user_profile = "Benutzerprofil";
$contacts_authorise = "Authorisierung";
$contacts_add_member = "Dieses Mitglied in das Kontaktverzeichnis aufnehmen";
$strConfirmSynchronise = "Hiermit werden die Kontaktdaten mit den Benutzerdaten abgeglichen. Dabei werden Ihre manuellen Eintragungen verloren gehen. M&ouml;chten Sie fortfahren?";
$contacts_import = "Import";
$contacts_export = "Export";
$contacts_export_text = "Bitte den gew&uuml;nschten CSV-Typ ausw&auml;hlen.";
$contacts_delete = "L&ouml;schen";

$contacts_import_text_group = "Bitte eine Gruppe und die CSV-Datei ausw&auml;hlen. Anschliessend auf 'ok' klicken.";
$fbNoFile = "Keine Datei empfangen";
$fbPleaseWait = "Bitte warten Sie solange die Datei hochgeladen wird...";

$strNotIncluded = "Nicht importieren";
$contacts_import_feedback = "Die nachfolgenden Spalten existieren in Ihrer CSV-Datei.<br />";
$contacts_import_feedback .= "Bitte eine Zielgruppe f&uuml;r den Import ausw&auml;hlen. Andernfalls werden auf Grundlage der Spalten der Datei die Gruppen automatisch erzeugt.<br />";
$contacts_import_feedback .= " Ordnen Sie den Group-Office Spalten die korrespondierenden Spalten aus der zu importierenden Datei zu. Achten Sie bitte darauf, dass Sie die Spalten eindeutig zuordnen.";
$contacts_separator = "Die Spalten sind getrennt mit";
$contacts_no_separator = "Sie haben kein Trennzeichen angegeben.";
$fbFilename = "Dateiname";
$contacts_import_title = "Adressbuch importieren";
$contacts_import_success = "Datei erfolgreich importiert";
$contacts_import_double = "Sie haben mindestens eine Spalte nicht eindeutig zugeordnet. Bitte korrigieren und erneut versuchen.";
$contacts_import_noname = 'Sie m&uuml;ssen den Feldnamen ausw&auml;hlen';
$contacts_import_incompatible = "Das Dateiformat ist inkompatibel.<br /><br />Die Spaltenwerte einer CSV-Datei werden in der Regel mit Semikolons getrennt.";
$strMember = "Mitglied";
$contact_exist_warning = "<b>Hinweis:</b> Dieser Benutzer existiert bereits in Ihrer Kontaktliste.";
$contacts_add_to_group = "Ausgew&auml;hlte Benutzer direkt einer Gruppe zuordnen...";
$contacts_add_to = "Ausgew&auml;hlte Benutzer der Kontaktliste hinzuf&uuml;gen";
$contacts_add_user = "Neu";
$contacts_edit = "Bearbeiten";
$contacts_no_select = "Sie haben keine Auswahl getroffen";
$contacts_this_contact = "Diesen Kontakt";

$contacts_seperator_text = 'Trennzeichen';
$contacts_quote = 'Spaltenwerte umschliessen mit';
$contacts_clrf = 'Zeilenabschlusszeichen';
$contacts_auto_group = 'Gruppen aus der Datei automatisch erzeugen';
$contacts_import_to_group = 'Importiere in die Gruppe';
$contacts_search = 'Suchen';
$cmdSearch = 'Suchen';
$contacts_search_text = "Suchbegriff eingeben und auf 'suchen' klicken. Das Zeichen * kann als wildcard benutzt werden.";
$contacts_result = 'Ergebnis';
$contacts_results = 'Ergebnisse';
$contacts_color = 'Farbe';
$contacts_colors['black'] = 'Schwarz';
$contacts_colors['red'] = 'Rot';
$contacts_colors['green'] = 'Gr&uuml;n';
$contacts_colors['blue'] = 'Blau';
$contacts_colors['pink'] = 'Pink';
$contacts_colors['orange'] = 'Orange';
$contacts_colors['brown'] = 'Braun';

$ab_addressbooks = 'Adressb&uuml;cher';
$ab_owner = 'Eigent&uuml;mer';
$ab_subscribed = 'Abonniert';
$ab_new_ab = 'Neues Adressbuch';
$ab_addressbook_exists = 'Ein Adressbuch mit diesem Namen ist bereits vorhanden';
$ab_addressbook = 'Adressbuch';
$contacts_select_single = 'Zum Ausw&auml;hlen bitte auf einen Kontakt klicken';

$ab_group_exists = 'Eine Gruppe mit diesem Namen ist bereits vorhanden';

//translation required
$ab_group_on_file = 'Gruppen-Kontakte basierend auf einer Datei';

$ab_edit_contact = 'Zum Speichern auf \'ok\' klicken. Mit * gekennzeichnete Felder m&uuml;ssen ausgef&uuml;llt werden.';
$ab_default = 'Standard';
$ab_browse = 'Browse';
$ab_comment = 'Kommentar';

$ab_contact = 'Kontakt';
$ab_send_message = 'Email';
$ab_new_event = 'Termin';

$ab_templates = 'Vorlage';
$ab_your_templates = 'Eigene';
$ab_all_templates = 'Alle';

$template_type[0] = 'Email';
$template_type[1] = 'OpenOffice.org';
$ab_type = 'Typ';
$ab_template_esists = 'Eine Vorlage mit diesem Namen ist bereits vorhanden.';

$sir_madam['M'] = 'Herr';
$sir_madam['F'] = 'Frau';

$ab_upload_oo = 'OpenOffice.org Vorlage hochladen';
$ab_upload_nofile = 'Fehler bei der Datei&uuml;bertragung aufgetreten.';

$ab_oo_doc = 'Textdokument';
$ab_select_template = 'Bitte eine Vorlage f&uuml;r dieses Dokument w&auml;hlen:';
$ab_add_field_data = 'Folgende Werte k&ouml;nnen im Dokument verwendet werden:';
$ab_datafield = 'Datenfeld';

$ab_no_template = 'Keine Vorlage';
$ab_new_note = 'Notiz';

$ab_events = 'Termine';

$ab_new_contact = 'Neuer&nbsp;Kontakt';
$ab_new_company = 'Neue&nbsp;Firma';
$ab_companies = 'Firmen';
$ab_company_properties = 'Firmenprofil';
$ab_bank_no = 'Bankverbindung';
$ab_companies_count_single = 'Firma';
$ab_companies_count = 'Firmen';
$ab_no_companies = 'Keine Firmen vorhanden';
$ab_no_company = 'Keine Firma';

$ab_import_type = 'Importieren';
$ab_no_company_found = 'Keine Firma gefunden';
$ab_search_company = 'Firma suchen';
$ab_edit_company = 'Firmenprofil bearbeiten';
$ab_new_group = 'Neue Gruppe';
$ab_export_type = 'Export';

$ab_custom_fields = 'Weitere&nbsp;Datenfelder';
$ab_field_size = 'Gr&ouml;sse';

$ab_new_field = 'Neues Datenfeld';
$ab_invalid_field_size = 'Ung&uuml;ltige Eingabe. Es d&uuml;rfen lediglich 10 bis 255 Zeichen eingegeben werden.';
$ab_field_exists = 'Ein Feld mit diesem Namen ist bereits vorhanden.';

$ab_employees = 'Mitarbeiter';
$ab_no_employees = 'Keine Mitarbeiter gefunden';

$ab_employee_count_single = 'Mitarbeiter';
$ab_employee_count_multiple = 'Mitarbeiter';
$ab_vat_no = 'Steuernummer';

$ab_add_new = 'neuen hinzuf&uuml;gen';
$ab_add_existing = 'vorhand. hinzuf&uuml;gen';

$ab_access = 'Zugriff';

$ab_mailings = 'Mailings';
$ab_mailing_groups = 'Mailing-Gruppen';
$ab_no_mailing_groups = 'Keine Mailing-Gruppen vorhanden';
$ab_no_templates = 'Keine Vorlagen vorhanden';
$ab_mailing_group_exists = 'Eine Mailing-Gruppe mit diesem Namen ist bereits vorhanden.';
$ab_contacts_count_single = 'Kontakt';
$ab_contacts_count_multiple = 'Kontakte';
$ab_new_mailing = 'Neues Mailing';
$ab_all_your_addressbooks = 'Alle Ihre Adressb&uuml;cher';

$ab_search_for = 'Suche in';
$ab_search_in = 'in';
$ab_search_on = 'unter';
$ab_search_keyword = 'Suchbegriff';
$ab_wildcard = '(Benutze \'%\' als Platzhalter)';

//2.03
$ab_copy_read_acl = 'Kopiere Leseberechtigungen zu Kontakten und Firmen';
$ab_copy_write_acl = 'Kopiere Schreibberechtigungen zu Kontakten und Firmen';

$ab_catagory_exists = 'Eine Kategorie mit diesem Namen existiert bereits';
$ab_add_catagory = 'Kategorie hinzufuegen';
$ab_add_field = 'Feld hinzufuegen';
$ab_catagory = 'Kategorie';

//2.04
$ab_todos = 'Aufgaben';

//2.05
$ab_download = 'Herunterladen';
$ab_download_template = 'Auf \'Herunterladen\' klicken, um eine Vorlage auf Ihrem PC bearbeiten zu k&ouml;nnen.';
$ab_upload = 'Hochladen';
//modified string:
$ab_upload_oo_text = 'Sobald Sie mit der Bearbeitung der OpenOffice.org-Vorlage fertig sind, sollten Sie diese wieder zu Group-Office hochladen. Zum Hochladen Datei ausw&auml;hlen und auf \'Hochladen\' klicken.';
$ab_file_type = 'Dateityp';
$ab_import_vcf_file_ok = 'Die hochgeladene Datei scheint in Ordnung zu sein. <br />Bitte best&auml;tigen Sie mit \'Ok\', um den Inhalt der Datei in das Adressbuch zu &uuml;bernehmen.';
$ab_import_failed = 'Der Import ist fehlgeschlagen.';
//modified string:
$contacts_import_text = "Bitte die zu importierende Datei ausw&auml;hlen, und mit 'Ok' best&auml;tigen.";
?>
