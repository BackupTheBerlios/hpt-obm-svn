<?php
require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "Titel";
$sc_description = "Beschreibung";
$sc_participants = "Teilnehmer";

$sc_location = "Veranstaltungsort";

$month_times[0] = "Ersten";
$month_times[1] = "Zweiten";
$month_times[2] = "Dritten";
$month_times[3] = "Vierten";

$sc_new_app = "Neuer Termin";
$sc_calendars = "Kalender";
$sc_invited = 'Sie sind eingeladen, der unten beschriebenen Veranstaltung beizutreten.';

$sc_types1[0] = 'Keine Wiederholung';
$sc_types1[1] = 'Tage';
$sc_types1[2] = 'Wochen';
$sc_types1[3] = 'Monate nach Datum';
$sc_types1[4] = 'Monate nach Wochentag';
$sc_types1[5] = 'Jahre';


$sc_types[0] = 'Einmalig';
$sc_types[1] = 'Taeglich';
$sc_types[2] = 'Woechentlich';
$sc_types[3] = 'Monatlich nach Datum';
$sc_types[4] = 'Monatlich nach Wochentag';
$sc_types[5] = 'Jaehrlich';


$sc_start_at = 'Startet am';
$sc_end_at = 'Endet am';
$sc_start_time = 'Startzeit';
$sc_end_time = 'Endezeit';

$sc_at_days = 'Am';
$sc_of_month = 'des Monats.';

$sc_cycle_start = 'Zyklusstart am';
$sc_cycle_end = 'Wiederholung bis';
$sc_accept_question = 'Moechten Sie diesen Termin eintragen?';

$sc_accept = 'Akzeptieren';
$sc_decline = 'Ablehnen';

$sc_notime = 'die Veranstaltung ist ganzt&auml;gig';
$sc_noend = 'kein Endedatum.';

$sc_never_happens = 'Eingabefehler! Dieser Termin wurde nicht gespeichert.';
$sc_start_later = 'Eingabefehler!! Die Startzeit liegt hinter der Endezeit.';

$sc_add_self = 'Manuelle Eingabe';

$sc_select_calendar = 'Einen Kalender auswaehlen, welchem der Termin zugeordnet werden soll.';
$sc_accept_confirm = 'Der Termin wurde erfolgreich eingetragen.';
$sc_no_calendars = 'Kein gueltiger Kalender vorhanden.';
$sc_bad_event = 'Kein gueltiger Termineintrag vorhanden. Ist moeglicherweise bereits geloescht.';
$sc_accept_title = 'Termin akzeptieren';

$sc_decline_confirm = 'Der Termin wurde abgelehnt.';
$sc_decline_title = 'Termin ablehnen';

$sc_status = 'Status';
$sc_not_responded = 'Keine Antwort';
$sc_accepted = 'Akzeptiert';
$sc_declined = 'Abgelehnt';
$sc_no_participants = 'Keine Teilnehmer';

$sc_person = 'Person';
$sc_owner = 'Eigentuemer';
$sc_type = 'Typ';
$sc_subscribed = 'Abonniert';

$sc_subscribe = 'Abonnieren';
$sc_new_calendar = 'Neuer Kalender';
$sc_no_calendar = 'Kein Kalender vorhanden. Bitte legen Sie jetzt einen an, oder lassen Sie Group-Office einen fuer Sie anlegen.';
$sc_day_view = 'Tagesansicht';
$sc_week_view = 'Wochenansicht';
$sc_month_view = 'Monatsansicht';
$sc_list_view = 'Listenansicht';
$sc_put_in = 'Termin eintragen';
$sc_inherit_permissions ='Uebernehme Berechtigungen von';
$sc_no_inherit = 'Nicht uebernehmen';

$sc_select_calendar_please = 'Bitte mindestens einen Kalender auswaehlen.';
$sc_events = 'Termine';

$sc_from_calendar = 'Vom Kalender';
$sc_enitrely = 'Komplett';
$sc_delete_event = 'Loesche Termin';
$sc_delete_pre = 'Moechten Sie den Termin';
$sc_delete_suf = 'komplett oder lediglich aus diesem Kalender entfernen?';

$sc_refresh = 'Aktualisieren';
$sc_reminder = 'Erinnerung';
$sc_mins = 'Minuten';
$sc_hour = 'Stunde';
$sc_hours = 'Stunden';
$sc_day = 'Tag';
$sc_days = 'Tage';
$sc_week = 'Woche';
$sc_weeks = 'Wochen';
$sc_reminder_set_to_early = "Die Erinnerungsfunktion kann nicht aktiviert werden. Bitte entweder den Zeitraum fuer die Erinnerungsfunktion verringern oder die Periode des Wiederholungstermins vergroessern.";
$sc_cycle_start_later = 'Das Startdatum des Zyklus liegt hinter oder ist gleich dem Endedatum.';

$cmdGenerate = 'Generieren';
$sc_preferences = 'Einstellungen';
$sc_holidays = 'Feiertage';
$sc_holidays_no_file_prefix = 'Feiertagsermittlung nicht moeglich. Die Definitionsdatei fuer die Berechnung der Feiertage ist fuer die Sprache ';
$sc_holidays_no_file_suffix = ' nicht gepflegt.';
$sc_holidays_text1 = 'Bitte auf \''.$cmdGenerate.'\' klicken, um die Feiertage fuer das gewaehlte Jahr zu ermitteln.';
$sc_holidays_text2 = 'Bitte auf \''.$cmdApply.'\' klicken, um die ermittelten Feiertage in die Datenbank zu uebernehmen.';

$strNoHolidays = 'Keine Feiertage in der Datenbank';
$strUnknown = 'Unbekannt';
$strYear = 'Jahr';
$strDeleteHolidaysPrefix = 'Sollen alle Feiertage fuer das Jahr ';
$strDeleteHolidaysSuffix = ' aus der Datenbank entfernt werden?';
$strReplaceHolidays = 'Alle gespeicherten Feiertage werden ueberschrieben. Sind Sie sicher?';

$sc_client = 'Mandant';
$sc_no_contact = 'kein';

$sc_background = 'Hintergrund';

$sc_colors['red'] = 'Rot';
$sc_colors['green'] = 'Gruen';
$sc_colors['blue'] = 'Blau';
$sc_colors['pink'] = 'Pink';
$sc_colors['orange'] = 'Orange';
$sc_colors['brown'] = 'Braun';
$sc_colors['yellow'] = 'Gelb';
$sc_colors['grey'] = 'Grau';
$sc_colors['purple'] = 'Lila';
$sc_colors['white']= 'Weiss';
$sc_status = 'status';

$sc_show_hours = 'Stundenanzeige von';
$sc_to = 'bis';

$sc_today = 'Heute';

$sc_recur_every = 'Wiederholung alle';
$sc_months = 'Monate';

$sc_private_event = 'Privater Termin';
$sc_calendar_exists = 'Ein Kalender mit diesem Namen ist bereits vorhanden';
$sc_month = 'Monat';
$sc_view = 'Ansicht';
$sc_calendar = 'Kalender';
$sc_default = 'Standard';

$sc_no_events = 'Keine Termine';

//2.03
$cal_export = 'Export';
$cal_import = 'Import';
$cal_please_wait = 'Bitte warten...';
$cal_import_text = 'iCal (*.ics) Datei waehlen und auf \'Ok\' klicken';
$cal_filename = 'Datei';
$cal_import_success = 'Daten importiert';

//2.04
$cal_add_todo = 'Neue Aufgabe';
$cal_todos = 'Aufgaben';

$cal_hide_completed = 'Erledigte verbergen';
$cal_show_all = 'Alle Aufgaben';
$cal_show_own = 'Eigene Aufgaben';
$cal_show_responsible = 'Eigene Zust&auml;ndigkeit';

$cal_priority = 'Priorit&auml;t';
$cal_priority_values[0] = 'Niedrig';
$cal_priority_values[1] = 'Normal';
$cal_priority_values[2] = 'Hoch';

$cal_status = 'Status';
$cal_canceled = 'Abgebrochen';
$cal_completed = 'Erledigt am';
$cal_percent_completed = '% erledigt';

$cal_before_task_start = 'vor Aufgabenbeginn';
$cal_before_task_due = 'vor Aufgabenende';
$cal_responsible = 'Zust&auml;ndiger';

$cal_due_at = 'Endet am';

$cal_no_todos= 'Keine Aufgaben';
$cal_resend_invitation = 'Einladung erneut versenden';
//2.05
$cal_everybody_write = 'Alle d&uuml;rfen diesen Eintrag bearbeiten';
$cal_everybody_read = 'Nur ich darf diesen Eintrag bearbeiten';

$cal_view = 'Ansicht';
$cal_views = 'Ansichten';
$sc_view_exists = 'Eine Ansicht mit diesem Namen ist bereits vorhanden';
$cal_new_view = 'Neue Ansicht';
$cal_event_calendars = 'Kalender';

//2.06
$cal_view_merged = 'zusammen';
$cal_view_emerged = 'einzeln';
?>
