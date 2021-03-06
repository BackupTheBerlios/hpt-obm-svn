<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

$lang_modules['scheduler'] = 'Agenda';
$lang_modules['calendar'] = 'Calendario';
$lang_modules['filesystem'] = 'Documenti';
$lang_modules['email'] = 'Posta-Elettronica';
$lang_modules['projects'] = 'Progetti';
$lang_modules['search'] = 'Cerca';
$lang_modules['contacts'] = "Rubrica";
$lang_modules['cms'] = "Siti-Web";
$lang_modules['addressbook'] = "Rubrica";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Connetti";
$cmdSave = "Salva";
$cmdBack = "Indietro";
$cmdCancel = "Annulla";
$cmdClose = "Chiudi";
$cmdOk = "Ok";
$cmdDelete = "Elimina";
$cmdNext = "Successivo";
$cmdPrevious = "Precedente";
$cmdContinue = "Continua";
$cmdUpdate = "Sincronizza";
$cmdApply = "Applica";
$cmdYes = "Si";
$cmdNo = "No";
$cmdAdd = 'Aggiungi';
$cmdEdit = 'Modifica';
$cmdInvertSelection = 'Inverti selezione';

$strEdit = "Modifica";
$strThis = 'questo';
$strItems = 'elementi';

$strName = "Nome";
$strUser = "Utente";
$strUsername = "Nome utente";
$strPassword = "Password";
$strAuthcode = "Codice autorizzazione";
$strCountry = "Nazione";
$strState = "Stato";
$strCity = "Citt�";
$strZip = "CAP";
$strAddress = "Indirizzo";
$strEmail = "E-mail";
$strPhone = "Telefono";
$strWorkphone = "Telefono lavoro";
$strCellular = "Cellulare";
$strCompany = "Societ�";
$strDepartment = "Dipartimento";
$strFunction = "Mansione";
$strQuestion = "Domanda segreta";
$strAnswer = "Risposta";
$strFax = "Fax";
$strWorkFax = "Fax ufficio";
$strHomepage = "Homepage";
$strWorkAddress = "Indirizzo ufficio";
$strWorkZip = "CAP ufficio";
$strWorkCountry = "Nazione ufficio";
$strWorkState = "Stato ufficio";
$strWorkCity = "Citt� ufficio";
$strPhoto = "Fotografia";
$strComments = "Commenti";

$strProperties = 'Propriet�';

$strDeleteItem = "Elimina";

$strDate = "Data";

$strSaveSuccess = "Dati salvati con successo";
$strSaveError = "Si � verificato un errore nel salvataggio dei dati.";
$strDataError = "Si � verificato un errore durante la lettura dei dati.";
$strDeleteError = "Non � possibile eliminare i dati.";
$strAclError = "Errore di Accesso";
$strDateError = "Hai inserito una data non valida";



$strAccessDenied = "Accesso negato";
$AccessDenied_text = "Non hai il permesso di vedere questa sezione del sito o di eseguire questa operazione.<br />Un altro utente ha ristretto l'accesso alle informazioni che hai richiesto.";

$invalid_chars = "I seguenti caratteri non sono consentiti";

$strGroups = "Gruppi";
$strOwner = "Proprietario";

$strDeletePrefix = "Sei sicuro di volere eliminare ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "Disconnetti";
$menu_applications = "Applicazioni";
$menu_help = "Aiuto";
$menu_about = "Informazioni";
$menu_documentation = "Documentazione";
$menu_configuration = "Configurazione";
$menu_groups = "Gruppi&nbsp;di&nbsp;utenti";
$menu_accounts = "Account&nbsp;personale";
$menu_preferences = "Preferenze";
$menu_bookmarks = "Segnalibri";
$menu_bookmarks_manage = "Modifica&nbsp;segnalibri";
$menu_add_bookmark = "Aggiungi&nbsp;segnalibro";
$menu_add_current = "Aggiungi&nbsp;pagina&nbsp;corrente";
$menu_contacts = "Rubrica";
$menu_invite = "Invita&nbsp;utenti";

$menu_search_discussions = "Discussioni";
$menu_search_files = "File&nbsp;e&nbsp;cartelle";
$menu_search_web = "Il&nbsp;web";
$menu_scheduler = "Agenda";
$menu_filetypes = "Tipi&nbsp;di&nbsp;file";
$menu_discussions = "Bacheca&nbsp;elettronica";

$menu_users = "Utenti";
$menu_start = "Pagina&nbsp;principale";
$menu_reboot = "Reboot&nbsp;server";
$menu_halt = "Shutdown&nbsp;server";
$menu_reboot_text = "ATTENZIONE! Tutti i servizi di Group-Office saranno temporaneamente inaccessibili. Sei sicuro di volere continuare?";
$menu_halt_text = "ATTENZIONE! Tutti i servizi di Group-Office saranno terminati. Sei sicuro di volere continuare?";
$menu_new = "Nuovo";
$menu_message = "Messaggio";
$menu_contact = "Contatto";
$menu_event = "Evento";
$menu_bookmark = "Segnalibro";
$menu_topics = "Soggetti";
$menu_support = "Supporto";
$menu_projects = "Progetti";
$menu_sysinfo = "Informazioni di sistema";
$menu_admin = 'Amministrazione';
$menu_modules = 'Moduli';
$menu_manual = 'Manuale';

//class validate
$error_required = "Non hai specificato il seguente campo obbligatorio.";
$error_match_auth = "I codici di autorizzazione non corrispondono.";
$error_match_pass = "Le password non corrispondono.";
$error_min_length = "Minimo numero di caratteri per il campo seguente:";
$error_max_length = "Massimo numero di caratteri per il campo seguente:";
$error_email = "L'indirizzo E-mail specificato non � valido.";
$error_email_exists = "L'indirizzo E-mail specificato esiste gi�.";
$error_username_exists = "Il nome utente specificato esiste gi�. Specificate un nome diverso.";
$errors_in_form = "C'erano errori nella form. Correggeteli e provare ancora.";
$error_missing_field = "Non hai compilato tutti i campi obbligatori.";

//acl control
$acl_control_auth_groups = "Gruppi autorizzati";
$acl_control_auth_users = "Utenti autorizzati";
$acl_control_no_contacts = "Non hai nessun membro nei contatti da selezionare.<br /> Usa la Rubrica per aggiugere membri nei constatti.";

$strShowProfile = "Mostra profilo";
$strShowGroup = "Mostra i menbri del gruppo";
$strProcessingTime = "L'elaborazione di questa pagina ha impiegato ";
$strLoggedInAs = "Connesso come ";
$strAt = " alle ";
$strEmailTo = "Invia un messaggio E-mail a: ";
$strProtectedUser = "Utente protetto";

$http_not_found = "Documento non trovato";
$http_not_found_text = "Il documento richiesto non � stato trovato. Forse l'indirizzo � errato.";
$http_error = "Internal Server Error";
$http_bad_request = "Richiesta non valida";
$strAddContact = "Aggiungi alla rubrica";

$strReadRights = "Permessi di lettura";
$strWriteRights = "Permessi di scrittura";
$strPermissions = 'Permessi';

$editor_bold = "Grassetto";
$editor_italic = "Corsivo";
$editor_underline = "Sottolineato";
$editor_left = "Sinistra";
$editor_right = "Destra";
$editor_center = "Centro";
$editor_ord_list = "Lista ordinata";
$editor_bul_list = "Lista comunicati";
$editor_txt_color = "Colore Testo";
$editor_back_color = "Colore Sfondo";
$editor_hyperlink = "Collegamento";
$editor_image = "Immagine";
$editor_rule = "Righello orizzontale";

$editor_very_small = "Molto piccolo";
$editor_small = "Piccolo";
$editor_medium = "Medio";
$editor_large = "Medio grande";
$editor_larger = "Grande";
$editor_very_large = "Molto grande";

$editor_strikethrough = "Barrato";
$editor_outdent = "Outdent";
$editor_indent = "Indent";

$editor_superscript = "Superscript";
$editor_subscript = "Subscript";
$editor_undo = "Annulla";
$editor_redo = "Ripristina";
$editor_copy = "Copia";
$editor_paste = "Incolla";
$editor_cut = "Taglia";

$editor_font = "Carattere";
$editor_size = "Dimensione";
$editor_heading = "Titolo";
$editor_select = "Selezione";

$editor_mode = "Imposta modo editor";

$days[0]="Do";
$days[1]="Lu";
$days[2]="Ma";
$days[3]="Me";
$days[4]="Gi";
$days[5]="Ve";
$days[6]="Sa";

$months[0]="Gennaio";
$months[1]="Febbraio";
$months[2]="Marzo";
$months[3]="Aprile";
$months[4]="Maggio";
$months[5]="Giugno";
$months[6]="Luglio";
$months[7]="Agosto";
$months[8]="Settembre";
$months[9]="Ottobre";
$months[10]="Novembre";
$months[11]="Dicembre";

$full_days[0] = "Domenica";
$full_days[1] = "Luned�";
$full_days[2] = "Marted�";
$full_days[3] = "Mercoled�";
$full_days[4] = "Gioved�";
$full_days[5]= "Venerd�";
$full_days[6] = "Sabato";

$strLoadTime = 'Generato in';
$strDescription = 'Descrizione';

$strTitle = 'Titolo';

$editorCmd['undo'] = "Annulla";
$editorCmd['redo'] = "Ripeti";
$editorCmd['bold'] = "Grassetto";
$editorCmd['italic'] = "Corsivo";
$editorCmd['underline'] = "Sottolineato";
$editorCmd['left'] = "Allineato a sinistra";
$editorCmd['right'] = "Allineato a destra";
$editorCmd['full'] = "Giustificato";
$editorCmd['center'] = "Centrato";
$editorCmd['ord_list'] = "Elenchi numerati";
$editorCmd['bul_list'] = "Elenchi puntati";
$editorCmd['txt_color'] = "Colore testo";
$editorCmd['back_color'] = "Colore sfondo";
$editorCmd['hyperlink'] = "Collegamento";
$editorCmd['image'] = "Inserisci immagine";
$editorCmd['rule'] = "Righello orizzontale";
$editorCmd['table'] = "Inserisci tabella";



$editorCmd['strikethrough'] = "Barrato";
$editorCmd['outdent'] = "Ritorna";
$editorCmd['indent'] = "Rientra";

$editorCmd['superscript'] = "Apice";
$editorCmd['subscript'] = "Pedice";
$editorCmd['undo'] = "Annulla";
$editorCmd['redo'] = "Ripeti";
$editorCmd['copy'] = "Copia";
$editorCmd['paste'] = "Incolla";
$editorCmd['cut'] = "Taglia";

$editorCmd['font'] = "Carattere";
$editorCmd['size'] = "Dimensione";
$editorCmd['heading'] = "Intestazione";
$editorCmd['select'] = "Seleziona";
$editorCmd['mode'] = "Cambia modalit�";
$editorCmd['textindicator'] = 'Stile corrente';

$cmdShowAll = 'Mostra tutto';

$cmdSearch = 'Cerca';

$cmdYesToAll = 'Si a tutti';
$cmdNoToAll = 'No a tutti';

$short_months[0]="Gen";
$short_months[1]="Feb";
$short_months[2]="Mar";
$short_months[3]="Apr";
$short_months[4]="Mag";
$short_months[5]="Giu";
$short_months[6]="Lug";
$short_months[7]="Ago";
$short_months[8]="Set";
$short_months[9]="Ott";
$short_months[10]="Nov";
$short_months[11]="Dic";

$strToday = 'Oggi';

$strShortWeek = 'st';


$cal_prev_year = "Anno prec.(premi per men�)";
$cal_prev_month  = "Mese prec.(premi per men�)";
$cal_go_today = "Vai a oggi";
$cal_next_month = "Mese succ.(premi per men�)";
$cal_next_year  = "Anno succ.(premi per men�)";
$cal_drag_to_move = "Trscina per muovere";
$cal_part_today = " (oggi)";
$cal_monday_first = "Mostra prima il luned�";
$cal_sunday_first  = "Mostra prima la domenica";
$cal_select_date = "Seleziona data";

$cmdPrint = 'Stampa';
$strFirstName = 'Nome';
$strLastName = 'Cognome';
$strInitials = 'Iniziali';
$strSex = 'Sesso';
$strBirthday = 'Compleanno';
$strSexes['M'] = 'Uomo';
$strSexes['F'] = 'Donna';

$strTitle = 'Titolo';

$strBeginning = 'Inizio';

$strMiddleName = 'Secondo&nbsp;nome';

$sir_madam['M'] = 'signore';
$sir_madam['F'] = 'signora';

$strModifiedAt = 'Modificato il';
$strCreatedAt = 'Creato il';

$htmlarea_language_name = 'it';
$strLocalization = 'Localizzazione';
$lang_modules['notes'] = "Note";

//look up the language in controls/jscalendar/lang
$jscalendar_language_name = 'calendar-it.js';

$lang_modules['bookmarks'] = "Segnalibri";

//2.01 translations

//look up language in 'classes/phpmailer/language'
$php_mailer_lang = 'it';
$strEmailFail = 'Invio dell\'e-mail non riuscito.';

$lang_modules['todos'] = "Attivit�";
$lang_modules['summary'] = "Sommario";

//for people that want to use ktmlarea
$ktml_language = 'italian';

$ml_host_unreachable = ' is unreachable. Automatic checking of e-mail is disabled.';
$strRegion = 'Regione';

//2.05
$cmdReset = 'Reset';
$strMyName = 'My name';

?>
