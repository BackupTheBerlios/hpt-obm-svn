<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

$lang_modules['scheduler'] = "Kalender";
$lang_modules['filesystem'] = 'Filer';
$lang_modules['email'] = 'Post';
$lang_modules['projects'] = 'Projekter';
$lang_modules['search'] = "Søg";
$lang_modules['contacts'] = "Adressebog";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Log ind";
$cmdSave = "Gem";
$cmdBack = "Tilbage";
$cmdCancel = "Afbryd";
$cmdClose = "Luk";
$cmdOk = "OK";
$cmdDelete = "Slet";
$cmdNext = "Næste";
$cmdPrevious = "Forrige";
$cmdContinue = "Fortsæt";
$cmdUpdate = "Synkroniser";
$cmdApply = "Anvend";
$cmdYes = "Ja";
$cmdNo = "Nej";
$cmdAdd = 'Tilføj';
$cmdEdit = 'Rediger';
$cmdInvertSelection = 'Skift valg';

$strEdit = "Rediger";
$strThis = 'disse';
$strItems = 'beskeder';

$strName = "Navn";
$strUser = "Bruger";
$strUsername = "Brugernavn";
$strPassword = "Adgangskode";
$strAuthcode = "Autorisationskode";
$strCountry = "Land";
$strState = "Stat";
$strCity = "By";
$strZip = "Postnummer";
$strAddress = "Vej";
$strEmail = "Mailadresse";
$strPhone = "Telefon";
$strWorkphone = "Telefon arbejde";
$strCellular = "Mobil";
$strCompany = "Firma";
$strDepartment = "Afdeling";
$strFunction = "Funktion";
$strQuestion = "Hemmeligt spørgsmål";
$strAnswer = "Svar";
$strFax = "Fax";
$strWorkFax = "Fax arbejde";
$strHomepage = "Hjemmeside";
$strWorkAddress = "Adresse arbejde";
$strWorkZip = "Postnummer arbejde";
$strWorkCountry = "Land arbejde";
$strWorkState = "Stat arbejde";
$strWorkCity = "By arbejde";
$strPhoto = "Foto";
$strComments = "Kommentar";

$strProperties = 'egenskaber';

$strDeleteItem = "Slet";

$strDate = "Dato";

$strSaveSuccess = "Data er gemt";
$strSaveError = "Der opstod en fejl under forsøg på at gemme data. Prøv igen.";
$strDataError = "Der opstod en fejl under læsning af data. Prøv at opdatere siden.";
$strDeleteError = "Data kunne ikke slette.";
$strAclError = "Adgangskontrol fejl";
$strDateError = "Du har indtastet en forkert dato";



$strAccessDenied = "Adgang nægtet";
$AccessDenied_text = "Du har ikke tilladelse til at se denne del af sitet eller til at udføre denne handling.<br />En anden bruger har begrænset adgangen til den information du har søgt.";

$invalid_chars = "Følgende karakterer er ikke tilladt";

$strGroups = "Grupper";
$strOwner = "Ejer";

$strDeletePrefix = "Er du sikker på du vil slette ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "Log ud";
$menu_applications = "Programmer";
$menu_help = "Hjælp";
$menu_about = "Om";
$menu_documentation = "Dokumentation";
$menu_configuration = "Opsætning";
$menu_groups = "Brugergrupper";
$menu_accounts = "Din&nbsp;konto";
$menu_preferences = "Indstillinger";
$menu_bookmarks = "Bogmærker";
$menu_bookmarks_manage = "Ret&nbsp;bogmærker";
$menu_add_bookmark = "Tilføj&nbsp;bogmærke";
$menu_add_current = "Tilføj&nbsp;aktuel&nbsp;side";

$menu_invite = "Inviter&nbsp;brugere";

$menu_search_discussions = "Diskussion";
$menu_search_files = "Filer&nbsp;og&nbsp;mapper";
$menu_search_web = "Internet";
$menu_scheduler = "Aftaler";
$menu_filetypes = "Filtyper";
$menu_discussions = "Diskussionsforum";

$menu_users = "Brugere";
$menu_start = "Startside";
$menu_reboot = "Genstart&nbsp;server";
$menu_halt = "Luk&nbsp;server";
$menu_reboot_text = "ADVARSEL! Alle Group-Office funktioner vil være midlertidig utilgængelige. Er du sikker på du vil fortsætte?";
$menu_halt_text = "ADVARSEL! Alle Group-Office funktioner vil blive afbrudt. Er du sikker på du vil fortsætte?";
$menu_new = "y";
$menu_message = "Besked";
$menu_contact = "Kontakt";
$menu_event = "Hændelse";
$menu_bookmark = "Bogmærke";
$menu_topics = "Emner";
$menu_support = "Support";
$menu_projects = "Projekter";
$menu_sysinfo = "Systeminformation";
$menu_admin = 'Administrator';
$menu_modules = 'Moduler';
$menu_manual = 'Brugermanual';

//class validate
$error_required = "Du har ikke udfyldt det krævede felt herunder.";
$error_match_auth = "Autorisationskoden er forkert.";
$error_match_pass = "Adgangskoden passer ikke.";
$error_min_length = "Mindste antal karakterer for feltet herunder:";
$error_max_length = "Største antal karakterer for feltet herunder:";
$error_email = "Mailadressen er ugyldig.";
$error_email_exists = "Mailadressen eksisterer allerede.";
$error_username_exists = "Brugernavnet findes allerede. Du skal vælge et andet.";
$errors_in_form = "Der er fejl i indtastningen. Ret fejlen og prøv igen.";
$error_missing_field = "Du skal udfylde alle de nødvendige felter.";

//acl control
$acl_control_auth_groups = "Autoriserede grupper";
$acl_control_auth_users = "Autoriserede brugere";
$acl_control_no_contacts = "Du kan ikke vælge nogen kontakter.<br /> Brug adressebogen til at tilføje kontakter.";

$strShowProfile = "Se profil";
$strShowGroup = "Se gruppemedlemmer";
$strProcessingTime = "Behandlingen af denne side tog ";
$strLoggedInAs = "Logget ind som ";
$strAt = " at ";
$strEmailTo = "Send mail til: ";
$strProtectedUser = "Beskyttet bruger";

$http_not_found = "Dokumentet findes ikke";
$http_not_found_text = "Det søgte dokument blev ikke fundet. Har du tastet rigtigt?";
$http_error = "Intern Serverfejl";
$http_bad_request = "Bad request";
$strAddContact = "Tilføj til kontakter";

$strReadRights = "Læserettigheder";
$strWriteRights = "Skriverettigheder";
$strPermissions = 'Rettigheder';

$editor_bold = "Fed";
$editor_italic = "Kursiv";
$editor_underline = "Understreget";
$editor_left = "Venstre";
$editor_right = "Højre";
$editor_center = "Centreret";
$editor_ord_list = "Ordnet liste";
$editor_bul_list = "Bullet liste";
$editor_txt_color = "Tekstfarve";
$editor_back_color = "Backgrundsfarve";
$editor_hyperlink = "Hyperlink";
$editor_image = "Billede";
$editor_rule = "Horizontal Rule";

$editor_very_small = "Meget lille";
$editor_small = "Lille";
$editor_medium = "Medium";
$editor_large = "Stor";
$editor_larger = "Større";
$editor_very_large = "Meget stor";

$editor_strikethrough = "Gennemstreget";
$editor_outdent = "Ryk ud";
$editor_indent = "Ryk ind";

$editor_superscript = "Hævet skrift";
$editor_subscript = "Sænket skrift";
$editor_undo = "Fortryd";
$editor_redo = "Fortryd fortryd";
$editor_copy = "Kopier";
$editor_paste = "Sæt ind";
$editor_cut = "Klip";

$editor_font = "Font";
$editor_size = "Størrelse";
$editor_heading = "Overskrift";
$editor_select = "Vælg";

$editor_mode = "Skift redigeringsmåde";

$days[0]="Sø";
$days[1]="Ma";
$days[2]="Ti";
$days[3]="On";
$days[4]="To";
$days[5]="Fr";
$days[6]="Lø";

$months[0]="Januar";
$months[1]="Februar";
$months[2]="Marts";
$months[3]="April";
$months[4]="Maj";
$months[5]="Juni";
$months[6]="Juli";
$months[7]="August";
$months[8]="September";
$months[9]="Oktober";
$months[10]="November";
$months[11]="December";

$full_days[0] = "Søndag";
$full_days[1] = "Mandag";
$full_days[2] = "Tirsdag";
$full_days[3] = "Onsdag";
$full_days[4] = "Torsdag";
$full_days[5]= "Fredag";
$full_days[6] = "Lørdag";

$strLoadTime = 'Genereret på';
$strDescription = 'Beskrivelse';

//translation required below here
$editorCmd['textindicator'] = 'Current style';
$strTitle = 'Title';
$lang_modules['cms'] = "web sites";
?>
