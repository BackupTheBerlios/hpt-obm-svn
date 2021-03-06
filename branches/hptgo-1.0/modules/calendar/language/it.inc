<?php
require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "Titolo";
$sc_description = "Descrizione";
$sc_participants = "Partecipanti";

$sc_location = "Luogo";

$month_times[0] = "Primo";
$month_times[1] = "Secondo";
$month_times[2] = "Terzo";
$month_times[3] = "Quarto";

$sc_new_app = "Nuovo appuntamento";
$sc_calendars = "Calendari";
$sc_invited = "Sei stato invitato a partecipare all'evento descritto di seguito.";

$sc_types1[0] = 'Nessuna ricorrenza';
$sc_types1[1] = 'giorni';
$sc_types1[2] = 'settimane';
$sc_types1[3] = 'mesi per data';
$sc_types1[4] = 'mesi per giorno';
$sc_types1[5] = 'anni';


$sc_types[0] = 'Unico';
$sc_types[1] = 'Giornaliero';
$sc_types[2] = 'Settimanale';
$sc_types[3] = 'Mensile per data';
$sc_types[4] = 'Mensile per giorno';
$sc_types[5] = 'Annuale';


$sc_start_at = 'Inizio';
$sc_end_at = 'Fine';
$sc_start_time = 'Ora inizio';
$sc_end_time = 'Ora fine';

$sc_at_days = 'Ai giorni';
$sc_of_month = 'del mese.';

$sc_cycle_start = 'Inizio ciclo il';
$sc_cycle_end = 'Ricorrenza finch�';
$sc_accept_question = 'Desideri accettare e inserire questo evento?';

$sc_accept = 'accetto';
$sc_decline = 'rinuncio';

$sc_notime = 'L\'ora non � applicabile';
$sc_noend = 'Nessuna data finale';

$sc_never_happens = 'Inserimento errato! Questo evento non accade mai.';
$sc_start_later = 'Inserimento errato!! L\'ora iniziale � posteriore all\'ora finale.';

$sc_add_self = 'Inserimento manuale';

$sc_select_calendar = 'Seleziona il calendario in cui inserire l\'evento';
$sc_accept_confirm = 'L\'evento � stato inserito ed elaborato.';
$sc_no_calendars = 'Nessun calendario valido per inserire l\'evento.';
$sc_bad_event = 'Nessun evento valido trovato. L\'evento probabilmente � stato cancellato.';
$sc_accept_title = 'Accetta evento';

$sc_decline_confirm = 'L\'evento � .';
$sc_decline_title = 'Rifiuta evento';

$sc_status = 'Stato';
$sc_not_responded = 'Nessuna risposta';
$sc_accepted = 'Accettato';
$sc_declined = 'Rifiutato';
$sc_no_participants = 'Nessun partecipante';

$sc_person = 'Persona';
$sc_owner = 'Proprietario';
$sc_type = 'Tipo';
$sc_subscribed = 'Sottoscritto';

$sc_subscribe = 'Sottoscrivi';
$sc_new_calendar = 'Nuovo calendario';
$sc_no_calendar = 'Nessun calendario trovato. Creane uno ora oppure lascia che Group-Office te ne crei uno automaticamente.';
$sc_day_view = 'Visualizza&nbsp;giorno';
$sc_week_view = 'Visualizza&nbsp;settimana';
$sc_list_view = 'Visualizza&nbsp;lista';
$sc_put_in = 'Inserisci in';
$sc_inherit_permissions ='Eredita permessi da';
$sc_no_inherit = 'Non ereditare';

$sc_select_calendar_please = 'Seleziona almeno un calendario.';
$sc_events = 'Eventi';

$sc_from_calendar = 'dal calendario';
$sc_enitrely = 'Interamente';
$sc_delete_event = 'Elimina evento';
$sc_delete_pre = 'Vuoi eliminare questo evento';
$sc_delete_suf = 'interamente o solo da questo calendario?';

$sc_refresh = 'Aggiorna';
$sc_reminder = 'Ricorda';
$sc_mins = 'min';
$sc_hour = 'ora';
$sc_hours = 'ore';
$sc_day = 'giorno';
$sc_days = 'giorni';
$sc_week = 'settimana';
$sc_weeks = 'settimane';
$sc_reminder_set_to_early = "The reminder cannnot be set to appear before the previous occurence. Please shorten the reminder time or lengthen the recurring period.";
$sc_cycle_start_later = 'The cycle starting date is later or equal then the ending date.';


$cmdGenerate = 'Genera';
$sc_preferences = 'Preferenze';
$sc_holidays = 'Festivit�';
$sc_holidays_no_file_prefix = 'Le festivit� non possono essere generate, poich� il file contenente le informazioni della vostra regione [\'';
$sc_holidays_no_file_suffix = '\'] � vuoto oppure non esiste.';
$sc_holidays_text1 = 'Seleziona \''.$cmdGenerate.'\' per elaborare le festivit� dell\'anno selezionator.';
$sc_holidays_text2 = 'Seleziona \''.$cmdApply.'\' per memorizzare tutte le festivit� generate nel database.';

$strNoHolidays = 'Nessuna festivit� nel database.';
$strUnknown = 'Sconosciuto';
$strYear = 'Anno';
$strDeleteHolidaysPrefix = 'Sei sicuro di voler modificare tutte le festivit� dell\'anno ';
$strDeleteHolidaysSuffix = '?';
$strReplaceHolidays = 'Sei sicuro di voler sovrascrivere tutte le festivit� esistenti?';

//translation required below this
$sc_client = 'Cliente';
$sc_no_contact = 'Nessuno';

$sc_background = 'Sfondo';

$sc_colors['red'] = 'Rosso';
$sc_colors['green'] = 'Verde';
$sc_colors['blue'] = 'Blu';
$sc_colors['pink'] = 'Rosa';
$sc_colors['orange'] = 'Arancione';
$sc_colors['brown'] = 'Marrone';
$sc_colors['yellow'] = 'Giallo';
$sc_colors['grey'] = 'Grigio';
$sc_colors['purple'] = 'Viola';
$sc_colors['white']= 'Bianco';
$sc_status = 'stato';

$sc_show_hours = 'Mostra ora dalle';
$sc_to = 'alle';

$sc_today = 'Oggi';

$sc_recur_every = 'Ricorre ogni';
$sc_months = 'mesi';

$sc_private_event = 'Evento privato';
$sc_calendar_exists = 'Un calendario con questo nome esiste gi�';

$sc_month = 'mese';
$sc_view = 'Visualizza';
$sc_calendar = 'Calendario';
$sc_default = 'Predefinito';


$sc_month_view = 'Visualizza&nbsp;mese';

$sc_no_events = 'Nessun evento trovato.';

//2.03
$cal_export = 'Esporta';
$cal_import = 'Importa';
$cal_please_wait = 'Attendere prego...';
$cal_import_text = 'Seleziona un file iCal (*.ics) e premi \'Ok\'';
$cal_filename = 'File';
$cal_import_success = 'Importazione conclusa';

//2.04
$cal_add_todo = 'Nuova attivit�';
$cal_todos = 'Attivit�';

$cal_hide_completed = 'Nascondi le attivit� completate';
$cal_show_all = 'Tutte le attivit�';
$cal_show_own = 'Le mie attivit�';
$cal_show_responsible = 'Attivit� di mia responsabilit�';

$cal_priority = 'Priotit�';
$cal_priority_values[0] = 'Bassa';
$cal_priority_values[1] = 'Normale';
$cal_priority_values[2] = 'Alta';

$cal_status = 'Stato';
$cal_canceled = 'Cancellata';
$cal_completed = 'Completata';
$cal_percent_completed = '% completato';

$cal_before_task_start = 'Prima dell\'inizio';
$cal_before_task_due = 'Prima della prevista scadenza';
$cal_responsible = 'Responsabile';

$cal_due_at = 'Scadenza prevista';
$cal_no_todos= 'Nessuna attivit� presente';
$cal_resend_invitation = 'Rispedisci gli inviti';

//2.05
$cal_everybody_write = 'Questo evento pu� essere modificato da tutti';
$cal_everybody_read = 'Questo evento pu� essere modificato solo da me';

$cal_view = 'Vista';
$cal_views = 'Viste';
$sc_view_exists = 'Una vista con questo nome esiste gi�';
$cal_new_view = 'Nuova vista';
$cal_event_calendars = 'Calendari';

?>
