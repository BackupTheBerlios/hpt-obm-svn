<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';
//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Inloggen";
$cmdSave = "Opslaan";
$cmdBack = "Terug";
$cmdCancel = "Annuleren";
$cmdClose = "Sluiten";
$cmdOk = "Ok";
$cmdDelete = "Verwijderen";
$cmdNext = "Volgende";
$cmdPrevious = "Vorige";
$cmdContinue = "Doorgaan";
$cmdUpdate = "Synchroniseren";
$cmdApply = "Toepassen";
$cmdYes = "Ja";
$cmdNo = "Nee";
$cmdAdd = 'Toevoegen';
$cmdEdit = 'Bewerken';
$cmdInvertSelection = 'Selectie inverteren';
$cmdGenerate = 'Genereren';
$cmdSearch = 'Zoeken';

$strEdit = "Bewerk";
$strThis = 'deze';
$strItems = 'items';
$strName = "Naam";
$strUser = "Gebruiker";
$strUsername = "Gebruikersnaam";
$strPassword = "Wachtwoord";
$strAuthcode = "Authorisatie code";
$strCountry = "Land";
$strState = "Provincie";
$strCity = "Woonplaats";
$strZip = "Postcode";
$strAddress = "Adres";
$strEmail = "E-mail";
$strPhone = "Telefoon";
$strWorkphone = "Telefoon werk";
$strCellular = "Mobiel";
$strCompany = "Bedrijfsnaam";
$strDepartment = "Afdeling";
$strFunction = "Functie";
$strQuestion = "Geheime vraag";
$strAnswer = "Antwoord";
$strFax = "Fax";
$strWorkFax = "Fax werk";
$strHomepage = "Homepage";
$strWorkAddress = "Adres Werk";
$strWorkZip = "Postcode werk";
$strWorkCountry = "Land werk";
$strWorkState = "Provincie werk";
$strWorkCity = "Woonplaats werk";
$strPhoto = "Foto";
$strComments = "Opmerkingen";
$strProperties = 'Eigenschappen';


$strDate = "Datum";
$strDeleteItem = "Verwijder";
$strSaveSuccess = "Gegevens succesvol opgeslagen";
$strSaveError = "Er is een fout opgetreden tijdens het opslaan van de gegevens.";
$strDataError = "Er is een fout opgetreden tijdens het lezen van de gegevens.";
$strDeleteError = "Kon de gegevens niet verwijderen.";
$strAclError = "Fout in het toegangsbeheer, Probeer het nog eens.";
$strDateError = "U heeft een ongeldige datum ingevoerd.";
$strAccessDenied = "Geen Toegang";
$AccessDenied_text = "U heeft geen rechten om dit gedeelte van de site te bekijken of deze actie uit te voeren.<br />Een andere gebruiker heeft de toegang beperkt.";
$invalid_chars = "De volgende karakters zijn niet toegestaan";
$strGroups = "Groepen";
$strOwner = "Eigenaar";
$strDeletePrefix = "Weet u zeker dat u ";
$strDeleteSuffix = " wilt verwijderen?";

//menu
$menu_logout = "Uitloggen";
$menu_applications = "Toepassingen";
$menu_help = "Help";
$menu_about = "Informatie";
$menu_documentation = "Documentatie";
$menu_configuration = "Configuratie";
$menu_groups = "Gebruikersgroepen beheren";
$menu_accounts = "Uw account instellingen wijzigen";
$menu_preferences = "Persoonlijke voorkeursinstellingen aanpassen";
$menu_search_files = "Bestanden&nbsp;en&nbsp;mappen";
$menu_search_web = "Het&nbsp;web";
$menu_file_browser = "Bestanden";
$menu_filetypes = "Bestandstypes aanpassen";
$menu_discussions = "Discussies";
$menu_scheduler = "Agenda";
$menu_users = "Gebruikers beheren";
$menu_new = "Nieuw";
$menu_message = "Bericht";
$menu_contact = "Contactpersoon";
$menu_event = "Afspraak";
$menu_bookmark = "Bladwijzer";
$menu_topics = "Onderwerpen";
$menu_support = "Ondersteuning";
$menu_projects = "Projecten";
$menu_sysinfo = "Systeem informatie bekijken";
$menu_admin = 'Beheerder menu';
$menu_modules = 'Modules toevoegen/verwijderen';
$menu_manual = 'Handleiding';

//class validate
$error_required = "U heeft het onderstaande vereiste veld niet ingevuld.";
$error_match_auth = "De authorisatie codes kwamen niet overeen.";
$error_match_pass = "De wachtwoorden kwamen niet overeen.";
$error_min_length = "Minimaal aantal karakters van het onderstaande veld:";
$error_max_length = "Maximaal aantal karakters van het onderstaande veld:";
$error_email = "Het opgegeven E-mail adres is ongeldig";
$error_email_exists = "Het E-mail adres dat u heeft opgegeven bestaat al.";
$error_username_exists = "Helaas bestaat de opgegeven gebruikersnaam al. Kies een andere.";
$errors_in_form = "Er zitten fouten in de opgegeven gegevens. Herstel deze en probeer opnieuw.";
$error_missing_field = "U heeft niet alle vereiste velden ingevuld.";
//acl control
$acl_control_auth_groups = "Geauthoriseerde gebruikersgroepen";
$acl_control_auth_users = "Geauthoriseerde gebruikers";
$acl_control_no_contacts = "U heeft geen gebruikers in uw contactpersonenlijst om te selecteren.<br />Gebruik het Adresboek om gebruikers toe te voegen aan je contactpersonen.";

$strShowProfile = "Profiel Weergeven";
$strShowGroup = "Gebruikers in groep weergeven";
$strProcessingTime = "Het genereren van deze pagina duurde ";
$strLoggedInAs = "Ingelogd als ";
$strAt = " op ";
$strEmailTo = "Stuur een bericht naar: ";
$strProtectedUser = "Afgeschermde Gebruiker";

$http_not_found = "Document niet gevonden";
$http_not_found_text = "Het opgevraagde document is niet gevonden. Misschien heeft u de URL verkeerd ingevoerd.";
$http_error = "Interne Server Fout";
$http_bad_request = "Ongeldig Verzoek";

$strAddContact = "Voeg toe aan uw contactpersonen";
$strReadRights = "Leesrechten";
$strWriteRights = "Schrijfrechten";
$strPermissions = 'Toegangsrechten';

$days[0]="Zo";
$days[1]="Ma";
$days[2]="Di";
$days[3]="Wo";
$days[4]="Do";
$days[5]="Vr";
$days[6]="Za";

$months[0]="Januari";
$months[1]="Februari";
$months[2]="Maart";
$months[3]="April";
$months[4]="Mei";
$months[5]="Juni";
$months[6]="Juli";
$months[7]="Augustus";
$months[8]="September";
$months[9]="Oktober";
$months[10]="November";
$months[11]="December";

$full_days[0] = "Zondag";
$full_days[1] = "Maandag";
$full_days[2] = "Dinsdag";
$full_days[3] = "Woensdag";
$full_days[4] = "Donderdag";
$full_days[5] = "Vrijdag";
$full_days[6] = "Zaterdag";

$strLoadTime = 'Gegenereerd in';
$strDescription = 'Beschrijving';
$strTitle = 'Titel';
$cmdShowAll = 'Toon alles';

$cmdYesToAll = 'Ja op alles';
$cmdNoToAll = 'Nee op alles';


$short_months[0]="Jan";
$short_months[1]="Feb";
$short_months[2]="Mrt";
$short_months[3]="Apr";
$short_months[4]="Mei";
$short_months[5]="Jun";
$short_months[6]="Jul";
$short_months[7]="Aug";
$short_months[8]="Sep";
$short_months[9]="Okt";
$short_months[10]="Nov";
$short_months[11]="Dec";

$strToday = 'Vandaag';

$strShortWeek = 'wk';

$cal_prev_year = "Vorig jaar (houd in voor menu)";
$cal_prev_month  = "Vorige maand (houd in voor menu)";
$cal_go_today = "Naar vandaag";
$cal_next_month = "Volg. maand (houd in voor menu)";
$cal_next_year  = "Volg. jaar (houd in voor menu)";
$cal_drag_to_move = "Versleep";
$cal_part_today = " (vandaag)";
$cal_monday_first = "Maandag eerst";
$cal_sunday_first  = "Zondag eerst";
$cal_select_date = "Selecteer datum";

$cmdPrint = 'Printen';

$strFirstName = 'Voornaam';
$strLastName = 'Achternaam';
$strInitials = 'Initialen';
$strSex = 'Geslacht';
$strBirthday = 'Geboortedatum';
$strSexes['M'] = 'Man';
$strSexes['F'] = 'Vrouw';

$strTitle = 'Titel';

$strBeginning = 'Aanhef';

$strMiddleName = 'Tussenvoegsel';

$sir_madam['M'] = 'heer';
$sir_madam['F'] = 'mevrouw';

$strModifiedAt = 'Gewijzigd op';
$strCreatedAt = 'Gemaakt op';

$htmlarea_language_name = 'nl';

$strLocalization = 'Localization';
//look up the language in controls/jscalendar/lang
$jscalendar_language_name = 'calendar-nl.js';
//look up language in 'classes/phpmailer/language'
$php_mailer_lang = 'nl';

$strEmailFail = 'Fout bij het verzenden van de e-mail.';

$lang_modules['bookmarks'] = "Favorieten";
$lang_modules['notes'] = "Notities";
$lang_modules['scheduler'] = "Agenda";
$lang_modules['calendar'] = "Agenda";
$lang_modules['filesystem'] = 'Bestanden';
$lang_modules['email'] = 'E-mail';
$lang_modules['projects'] = 'Projecten';
$lang_modules['search'] = 'Zoeken';
$lang_modules['contacts'] = "Adresboek";
$lang_modules['addressbook'] = "Adresboek";
$lang_modules['cms'] = "Websites";
$lang_modules['presentations'] = "Presentaties";
$lang_modules['questionnaires'] = "Vragenlijsten";
$lang_modules['summary'] = "Samenvatting";
$lang_modules['todos'] = "Taken";

//for people that want to use ktmlarea
$ktml_language = 'dutch';
$ml_host_unreachable = ' is onbereikbaar. Automatisch controleren van e-mail is uitgeschakeld.';
//2.05
$strMyName = 'Mijn naam';
$cmdReset = 'Wissen';
?>
