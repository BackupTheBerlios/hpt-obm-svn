<?php
require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "Titre";
$sc_description = "Description";
$sc_participants = "Invités";

$sc_location = "Emplacement";

$month_times[0] = "Permier";
$month_times[1] = "Deuxième";
$month_times[2] = "Troisième";
$month_times[3] = "Quatrième";

$sc_new_app = "Nouvel évènement";
$sc_calendars = "Calendriers";
$sc_invited = 'Vous êtes invité à joindre l\'évènement cité ci dessous.';

$sc_types1[0] = 'Ne répeter qu\'une fois';
$sc_types1[1] = 'jours';
$sc_types1[2] = 'semaines';
$sc_types1[3] = 'mois par date';
$sc_types1[4] = 'mois par jour';
$sc_types1[5] = 'années';


$sc_types[0] = 'Une fois';
$sc_types[1] = 'Tous les jours';
$sc_types[2] = 'Toutes les semaines';
$sc_types[3] = 'Mensuellement par date';
$sc_types[4] = 'Mensuellement par jour';
$sc_types[5] = 'Annuellemnt';


$sc_start_at = 'Débute à';
$sc_end_at = 'Se termine à';
$sc_start_time = 'Heure de début';
$sc_end_time = 'Heure de fin';

$sc_at_days = 'Les jours';
$sc_of_month = 'du mois.';

$sc_cycle_start = 'Le cycle démarre à';
$sc_cycle_end = 'Récurrent jusqu\'à';
$sc_accept_question = 'Souhaitez vous valider et planifier cet évènement ?';

$sc_accept = 'accepter';
$sc_decline = 'décliner';

$sc_notime = 'L\'heure n\'est pas applicable';
$sc_noend = 'Pas de date de fin.';

$sc_never_happens = 'Mauvaise entrée ! Cet évènement ne se produit jamais.';
$sc_start_later = 'Mauvais entrée !! L\'heure de début est après l\'heure de fin.';

$sc_add_self = 'Entrée manuelle';

$sc_select_calendar = 'Choisissez le calendrier dans lequel programmer cet évènement';
$sc_accept_confirm = 'L\'évènement est accepté et programmé.';
$sc_no_calendars = 'Pas de calendrier valide trouvé pour programmer cet évènement.';
$sc_bad_event = 'Aucun évènement valide trouvé. L\'évènement est probablement supprimé.';
$sc_accept_title = 'Accpeter l\'évènement';

$sc_decline_confirm = 'L\'évènement a été décliné.';
$sc_decline_title = 'Décliner l\'évènement';

$sc_status = 'Statut';
$sc_not_responded = 'Sans réponse';
$sc_accepted = 'Accepté';
$sc_declined = 'Refusé';
$sc_no_participants = 'Pas de participants';

$sc_person = 'Personne';
$sc_owner = 'Propriétaire';
$sc_type = 'Type';
$sc_subscribed = 'Souscrit';

$sc_subscribe = 'souscrire';
$sc_new_calendar = 'Nouveau calendrier';
$sc_no_calendar = 'Pas d\'agenda trouvé. Veuillez en créer en un maintenant ou laissez Group-Office en créer un automatiquement pour vous.';
$sc_day_view = 'Vue par jours';
$sc_week_view = 'Vue par semaine';
$sc_list_view = 'Liste';
$sc_put_in = 'Placer l\'évènement dans';
$sc_inherit_permissions ='Hériter des permissions de';
$sc_no_inherit = 'Ne pas hériter';

$sc_select_calendar_please = 'Veuillez au moins choisir un calendrier.';
$sc_events = 'Evènements';

$sc_from_calendar = 'depuis le calendrier';
$sc_enitrely = 'Entièrement';
$sc_delete_event = 'Supprimer l\'évènement';
$sc_delete_pre = 'Souhaitez vous supprimer l\'évènement';
$sc_delete_suf = 'entièrement ou juste depuis ce calendrier ?';

$sc_refresh = 'Rafraîchir';
$sc_reminder = 'Rappel';
$sc_mins = 'minutes';
$sc_hour = 'heure';
$sc_hours = 'heures';
$sc_day = 'jour';
$sc_days = 'jours';
$sc_week = 'semaine';
$sc_weeks = 'semaines';
$sc_reminder_set_to_early = "Le rappel ne peut pas se déclencher avant la précédente occurence. Veuillez raccourcir l\'heure de rappel ou la période.";
$sc_cycle_start_later = 'La date de début du cyle est supérieure ou égale à la date de fin.';

$cmdGenerate = 'Générer';
$sc_preferences = 'Préférences';
$sc_holidays = 'Vacances';

$strNoHolidays = 'Pas de vacances dans la base';
$strUnknown = 'Inconnu';
$strYear = 'Année';
$strDeleteHolidaysPrefix = 'Etes vous certain(e) de vouloir supprimer toutes les vacances de l\'année ';
$strDeleteHolidaysSuffix = '?';
$strReplaceHolidays = 'Etes vous certain(e) de vouloir écraser les enregsitrements concernants les vancances ?';
$sc_holidays_text1 = 'Veuillez cliquer sur \''.$cmdGenerate.'\' pour calculer les vacances pour l\'année choisie.';
$sc_holidays_text2 = 'Veuillez cliquer sur \''.$cmdApply.'\' pour enregistrer toutes les dates de vacances calculées dans la base de données.';

//translation required below this
$sc_client = 'Client';
$sc_no_contact = 'Aucun';

$sc_background = 'Fond';

$sc_colors['red'] = 'Rouge';
$sc_colors['green'] = 'Vert';
$sc_colors['blue'] = 'Bleu';
$sc_colors['pink'] = 'Rose';
$sc_colors['orange'] = 'Orange';
$sc_colors['brown'] = 'Brun';
$sc_colors['yellow'] = 'Jaune';
$sc_colors['grey'] = 'Gris';
$sc_colors['purple'] = 'Pourpre';
$sc_colors['white']= 'Blanc';
$sc_status = 'statut';

$sc_show_hours = 'Voir les heures depuis';
$sc_to = 'jusqu\'à';

$sc_today = 'Ajourd\'hui';

$sc_recur_every = 'Répéter tous les';
$sc_months = 'mois';

$sc_private_event = 'Evènement privé';
$sc_calendar_exists = 'Un calendrier avec ce nom existe déjà';

//translate
$sc_month = 'Mois';
$sc_view = 'Vue';
$sc_calendar = 'Calendrier';
$sc_default = 'Défaut';

$sc_month_view = 'Vue par mois';

//text changed, needs new translation
$sc_holidays_no_file_prefix = 'Les vacances ne peuvent pas être générées car le fichier qui contient les informations de vacance pour votre région [\'';
$sc_holidays_no_file_suffix = '\'] est soit vide ou n\'existe pas.';

$sc_no_events = 'Aucun Evènement trouvé';

//2.03
$cal_export = 'Exporter';
$cal_import = 'Importer';
$cal_please_wait = 'Patientez...';
$cal_import_text = 'Sélectionner un fichier iCal (*.ics) et appuyer sur \'Ok\'';
$cal_filename = 'Fichier';
$cal_import_success = 'Données importées';
?>
