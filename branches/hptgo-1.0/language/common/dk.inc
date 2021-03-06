<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

$lang_modules['scheduler'] = "Kalender";
$lang_modules['filesystem'] = 'Filer';
$lang_modules['email'] = 'Post';
$lang_modules['projects'] = 'Projekter';
$lang_modules['search'] = "S�g";
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
$cmdNext = "N�ste";
$cmdPrevious = "Forrige";
$cmdContinue = "Forts�t";
$cmdUpdate = "Synkroniser";
$cmdApply = "Anvend";
$cmdYes = "Ja";
$cmdNo = "Nej";
$cmdAdd = 'Tilf�j';
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
$strQuestion = "Hemmeligt sp�rgsm�l";
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
$strSaveError = "Der opstod en fejl under fors�g p� at gemme data. Pr�v igen.";
$strDataError = "Der opstod en fejl under l�sning af data. Pr�v at opdatere siden.";
$strDeleteError = "Data kunne ikke slette.";
$strAclError = "Adgangskontrol fejl";
$strDateError = "Du har indtastet en forkert dato";



$strAccessDenied = "Adgang n�gtet";
$AccessDenied_text = "Du har ikke tilladelse til at se denne del af sitet eller til at udf�re denne handling.<br />En anden bruger har begr�nset adgangen til den information du har s�gt.";

$invalid_chars = "F�lgende karakterer er ikke tilladt";

$strGroups = "Grupper";
$strOwner = "Ejer";

$strDeletePrefix = "Er du sikker p� du vil slette ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "Log ud";
$menu_applications = "Programmer";
$menu_help = "Hj�lp";
$menu_about = "Om";
$menu_documentation = "Dokumentation";
$menu_configuration = "Ops�tning";
$menu_groups = "Brugergrupper";
$menu_accounts = "Din&nbsp;konto";
$menu_preferences = "Indstillinger";
$menu_bookmarks = "Bogm�rker";
$menu_bookmarks_manage = "Ret&nbsp;bogm�rker";
$menu_add_bookmark = "Tilf�j&nbsp;bogm�rke";
$menu_add_current = "Tilf�j&nbsp;aktuel&nbsp;side";

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
$menu_reboot_text = "ADVARSEL! Alle Group-Office funktioner vil v�re midlertidig utilg�ngelige. Er du sikker p� du vil forts�tte?";
$menu_halt_text = "ADVARSEL! Alle Group-Office funktioner vil blive afbrudt. Er du sikker p� du vil forts�tte?";
$menu_new = "y";
$menu_message = "Besked";
$menu_contact = "Kontakt";
$menu_event = "H�ndelse";
$menu_bookmark = "Bogm�rke";
$menu_topics = "Emner";
$menu_support = "Support";
$menu_projects = "Projekter";
$menu_sysinfo = "Systeminformation";
$menu_admin = 'Administrator';
$menu_modules = 'Moduler';
$menu_manual = 'Brugermanual';

//class validate
$error_required = "Du har ikke udfyldt det kr�vede felt herunder.";
$error_match_auth = "Autorisationskoden er forkert.";
$error_match_pass = "Adgangskoden passer ikke.";
$error_min_length = "Mindste antal karakterer for feltet herunder:";
$error_max_length = "St�rste antal karakterer for feltet herunder:";
$error_email = "Mailadressen er ugyldig.";
$error_email_exists = "Mailadressen eksisterer allerede.";
$error_username_exists = "Brugernavnet findes allerede. Du skal v�lge et andet.";
$errors_in_form = "Der er fejl i indtastningen. Ret fejlen og pr�v igen.";
$error_missing_field = "Du skal udfylde alle de n�dvendige felter.";

//acl control
$acl_control_auth_groups = "Autoriserede grupper";
$acl_control_auth_users = "Autoriserede brugere";
$acl_control_no_contacts = "Du kan ikke v�lge nogen kontakter.<br /> Brug adressebogen til at tilf�je kontakter.";

$strShowProfile = "Se profil";
$strShowGroup = "Se gruppemedlemmer";
$strProcessingTime = "Behandlingen af denne side tog ";
$strLoggedInAs = "Logget ind som ";
$strAt = " at ";
$strEmailTo = "Send mail til: ";
$strProtectedUser = "Beskyttet bruger";

$http_not_found = "Dokumentet findes ikke";
$http_not_found_text = "Det s�gte dokument blev ikke fundet. Har du tastet rigtigt?";
$http_error = "Intern Serverfejl";
$http_bad_request = "Bad request";
$strAddContact = "Tilf�j til kontakter";

$strReadRights = "L�serettigheder";
$strWriteRights = "Skriverettigheder";
$strPermissions = 'Rettigheder';

$editor_bold = "Fed";
$editor_italic = "Kursiv";
$editor_underline = "Understreget";
$editor_left = "Venstre";
$editor_right = "H�jre";
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
$editor_larger = "St�rre";
$editor_very_large = "Meget stor";

$editor_strikethrough = "Gennemstreget";
$editor_outdent = "Ryk ud";
$editor_indent = "Ryk ind";

$editor_superscript = "H�vet skrift";
$editor_subscript = "S�nket skrift";
$editor_undo = "Fortryd";
$editor_redo = "Fortryd fortryd";
$editor_copy = "Kopier";
$editor_paste = "S�t ind";
$editor_cut = "Klip";

$editor_font = "Font";
$editor_size = "St�rrelse";
$editor_heading = "Overskrift";
$editor_select = "V�lg";

$editor_mode = "Skift redigeringsm�de";

$days[0]="S�";
$days[1]="Ma";
$days[2]="Ti";
$days[3]="On";
$days[4]="To";
$days[5]="Fr";
$days[6]="L�";

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

$full_days[0] = "S�ndag";
$full_days[1] = "Mandag";
$full_days[2] = "Tirsdag";
$full_days[3] = "Onsdag";
$full_days[4] = "Torsdag";
$full_days[5]= "Fredag";
$full_days[6] = "L�rdag";

$strLoadTime = 'Genereret p�';
$strDescription = 'Beskrivelse';

//translation required below here
$editorCmd['textindicator'] = 'Current style';
$strTitle = 'Title';
$lang_modules['cms'] = "web sites";
?>
