<?php
require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "Titel";
$sc_description = "Beschrijving";
$sc_participants = "Deelnemers";
$sc_location = "Lokatie";

$month_times[0] = "Eerste";
$month_times[1] = "Tweede";
$month_times[2] = "Derde";
$month_times[3] = "Vierde";

$sc_new_app = "Nieuwe afspraak";
$sc_calendars = "Agenda's";
$sc_invited = 'U bent uitgenodigd voor deelname aan onderstaande afspraak.';

$sc_types1[0] = 'Geen herhaling';
$sc_types1[1] = 'dagen';
$sc_types1[2] = 'weken';
$sc_types1[3] = 'maanden op datum';
$sc_types1[4] = 'maanden op weekdagen';
$sc_types1[5] = 'jaren';

$sc_types[0] = 'Eenmalig';
$sc_types[1] = 'Dagelijks';
$sc_types[2] = 'Weekelijks';
$sc_types[3] = 'Maandelijks per datum';
$sc_types[4] = 'Maandelijks per dag';
$sc_types[5] = 'Jaarlijks';

$sc_start_at = 'Start op';
$sc_end_at = 'Eindigt op';
$sc_start_time = 'Starttijd';
$sc_end_time = 'Eindtijd';

$sc_at_days = 'Op dagen';
$sc_of_month = 'van de maand.';

$sc_cycle_start = 'Cyclus start op';
$sc_cycle_end = 'Herhaal tot';
$sc_accept_question = 'Wilt u deze afspraak accepteren en inplannen?';

$sc_accept = 'Accepteren';
$sc_decline = 'Weigeren';

$sc_notime = 'Tijd niet van toepassing';
$sc_noend = 'Geen einddatum';

$sc_never_happens = 'Ongeldige invoer! Deze gebeurtenis vind nooit plaats.';
$sc_start_later = 'Ongeldige invoer! De starttijd is later dan de eindtijd.';

$sc_cycle_start_later = 'Ongeldige invoer! De cyclus startdatum is later of gelijk aan de einddatum';
$sc_add_self = 'zelf invoeren';

$sc_select_calendar = 'Selecteer de agenda waarin u deze afspraak wilt inplannen';
$sc_accept_confirm = 'De afspraak is geaccepteerd en ingepland.';
$sc_no_calendars = 'Er is geen geldige agenda gevonden om deze afspraak in te plannen.';
$sc_bad_event = 'Geen geldige afspraak gevonden. De afspraak is waarschijnlijk verwijderd.';
$sc_accept_title = 'Afspraak Accepteren';

$sc_decline_confirm = 'De afspraak is geweigerd.';
$sc_decline_title = 'Afspraak Weigeren';

$sc_status = 'Status';
$sc_not_responded = 'Nog niet gereageerd';
$sc_accepted = 'Geaccepteerd';
$sc_declined = 'Gewijgerd';
$sc_no_participants = 'Geen deelnemers gevonden';

$sc_person = 'Persoon';

$sc_owner = 'Eigenaar';
$sc_type = 'Type';
$sc_subscribed = 'Aangemeld';

$sc_subscribe = 'Aanmelden';
$sc_new_calendar = 'Nieuwe agenda';
$sc_no_calendar = 'Er is geen agenda van u gevonden. Maak er één aan of laat Group-Office er automatisch één voor u aanmaken.';
$sc_day_view = 'Dag&nbsp;weergave';
$sc_week_view = 'Week&nbsp;weergave';
$sc_list_view = 'Lijst&nbsp;weergave';
$sc_put_in = 'Afspraak plaatsen in';
$sc_inherit_permissions ='Neem rechten over van';
$sc_no_inherit = 'Niet overnemen';

$sc_select_calendar_please = 'Selecteer tenminste één agenda a.u.b.';

$sc_from_calendar = 'Uit agenda';
$sc_enitrely = 'Helemaal';
$sc_delete_event = 'Afspraak verwijderen';
$sc_delete_pre = 'Wilt u de afspraak';
$sc_delete_suf = 'helemaal verwijderen of alleen uit deze agenda?';
$sc_events = 'Afspraken';

$cmdGenerate = 'Genereren';
$sc_refresh = 'Vernieuwen';
$sc_reminder = 'Herinnering';
$sc_mins = 'min';
$sc_hour = 'uur';
$sc_hours = 'uren';
$sc_day = 'dag';
$sc_days = 'dagen';
$sc_week = 'week';
$sc_weeks = 'weken';
$sc_reminder_set_to_early = "De herinnering kan niet zo worden ingesteld dat deze verschijnt voor de vorige cyclus. Stel deze korter in of verleng de cyclus.";
$sc_preferences = 'Instellingen';
$sc_holidays = 'Vakantiedagen';
$sc_holidays_no_file_prefix = 'De vakantiedagen konden niet worden gegenereerd, omdat er geen informatie is gevonden voor [\'';
$sc_holidays_no_file_suffix = '\'].';
$sc_holidays_text1 = 'Klik op \''.$cmdGenerate.'\' om de vakantiedagen voor dit jaar te genereren.';
$sc_holidays_text2 = 'Klik op \''.$cmdApply.'\' om alle vakantiedagen op te slaan.';

$strNoHolidays = 'Geen vakantiedagen in de database aanwezig';
$strUnknown = 'onbekend';
$strYear = 'Jaar';
$strDeleteHolidaysPrefix = 'Weet u zeker dat u alle vakantiedagen van jaar ';
$strDeleteHolidaysSuffix = ' wilt verwijderen?';
$strReplaceHolidays = 'Weet u zeker dat u alle bestaande vakantiedagen wilt overschrijven?';

$sc_client = 'Klant';
$sc_no_contact = 'geen';
$sc_background = 'Achtergrond';

$sc_colors['red'] = 'Rood';
$sc_colors['green'] = 'Groen';
$sc_colors['blue'] = 'Blauw';
$sc_colors['pink'] = 'Roze';
$sc_colors['orange'] = 'Oranje';
$sc_colors['brown'] = 'Bruin';
$sc_colors['yellow'] = 'Geel';
$sc_colors['grey'] = 'Grijs';
$sc_colors['purple'] = 'Paars';
$sc_colors['white'] = 'Wit';
$sc_status = 'status';

$sc_show_hours = 'Toon uren van';
$sc_to = 't/m';

$sc_today = 'Vandaag';

$sc_recur_every = 'Herhaal elke';
$sc_months = 'maanden';

$sc_private_event = 'Privé afspraak';
$sc_calendar_exists = 'Er bestaat al een agenda met deze naam';

$sc_month = 'Maand';
$sc_view = 'Weergave';
$sc_calendar = 'Agenda';
$sc_default = 'Standaard';
$sc_month_view = 'Maand&nbsp;weergave';

$sc_no_events = 'Geen afspraken gevonden';
//2.03
$cal_export = 'Exporteren';
$cal_import = 'Importeren';
$cal_please_wait = 'Even wachten aub...';
$cal_import_text = 'Selecteer een iCal (*.ics) bestand en klik op \'Ok\'';
$cal_filename = 'Bestand';
$cal_import_success = 'Gegevens geïmporteerd';

//2.04
$cal_add_todo = 'Nieuwe taak';
$cal_todos = 'Taken';

$cal_show_all = 'Alle taken tonen';
$cal_show_own = 'Gemaakte taken tonen';
$cal_show_responsible = 'Verantwoordelijke taken tonen';

$cal_priority = 'Prioriteit';
$cal_priority_values[0] = 'Laag';
$cal_priority_values[1] = 'Normal';
$cal_priority_values[2] = 'Hoog';

$cal_status = 'Status';
$cal_canceled = 'Cancelled';
$cal_completed = 'Voltooid op';
$cal_percent_completed = '% voltooid';

$cal_before_task_start = 'Voor de taak start';
$cal_before_task_due = 'Voor de taak klaar moet zijn';
$cal_responsible = 'Verantwoordelijke';

$cal_due_at = 'Doen voor';

$cal_no_todos= 'Geen taken gevonden';
$cal_hide_completed = 'Verberg voltooide taken';

$cal_resend_invitation = 'Verzend uitnodiging opnieuw';
//2.05
$cal_everybody_write = 'Iedereen mag deze afspraak wijzigen';
$cal_everybody_read = 'Alleen ik mag deze afspraak wijzigen';

$cal_view = 'Overzicht';
$cal_views = 'Overzichten';
$sc_view_exists = 'Er bestaat al een overzicht met deze naam';
$cal_new_view = 'Nieuw overzicht';
$cal_event_calendars = 'Agenda(\'s)';

//2.06
$cal_view_merged = 'Samengevoegd';
$cal_view_emerged = 'Onder elkaar';
?>
