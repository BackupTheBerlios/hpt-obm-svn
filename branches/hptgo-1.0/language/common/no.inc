<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

$lang_modules['scheduler'] = "Kalender";
$lang_modules['filesystem'] = 'Filer';
$lang_modules['email'] = 'Post';
$lang_modules['projects'] = 'Prosjekter';
$lang_modules['search'] = "S�k";
$lang_modules['contacts'] = "Adressebok";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Logg inn";
$cmdSave = "Lagre";
$cmdBack = "Tilbake";
$cmdCancel = "Avbryt";
$cmdClose = "Lukk";
$cmdOk = "OK";
$cmdDelete = "Slett";
$cmdNext = "Neste";
$cmdPrevious = "Forrige";
$cmdContinue = "Fortsett";
$cmdUpdate = "Synkroniser";
$cmdApply = "Bruk";
$cmdYes = "Ja";
$cmdNo = "Nei";
$cmdAdd = 'Tilf�y';
$cmdEdit = 'Rediger';
$cmdInvertSelection = 'Skift valg';

$strEdit = "Rediger";
$strThis = 'disse';
$strItems = 'beskjeder';

$strName = "Navn";
$strUser = "Bruker";
$strUsername = "Brukernavn";
$strPassword = "Adgangskode";
$strAuthcode = "Autorisasjonskode";
$strCountry = "Land";
$strState = "Stat";
$strCity = "By";
$strZip = "Postnummer";
$strAddress = "Vei";
$strEmail = "E-postadresse";
$strPhone = "Telefon";
$strWorkphone = "Telefon arbeid";
$strCellular = "Mobil";
$strCompany = "Firma";
$strDepartment = "Avdeling";
$strFunction = "Funksjon";
$strQuestion = "Hemmelig sp�rsm�l";
$strAnswer = "Svar";
$strFax = "Fax";
$strWorkFax = "Fax arbeid";
$strHomepage = "Hjemmeside";
$strWorkAddress = "Adresse arbeid";
$strWorkZip = "Postnummer arbeid";
$strWorkCountry = "Land arbeid";
$strWorkState = "Stat arbeid";
$strWorkCity = "By arbeid";
$strPhoto = "Foto";
$strComments = "Kommentar";

$strProperties = 'egenskaper';

$strDeleteItem = "Slett";

$strDate = "Dato";

$strSaveSuccess = "Data er gjemt";
$strSaveError = "Det oppstod en feil under fors�k p� � gjemme data. Pr�v igen.";
$strDataError = "Det oppstod en feil under lesning av data. Pr�v � oppdatere siden.";
$strDeleteError = "Data kunne ikke slettes.";
$strAclError = "Adgangskontroll feil";
$strDateError = "Du har tastet inn en feil dato";



$strAccessDenied = "Adgang nektet";
$AccessDenied_text = "Du har ikke tillatelse � se denne delen av websiden eller til � utf�re denne handling.<br />En anden bruger har begr�nset adgangen til den information du har s�gt.";

$invalid_chars = "F�lgende karakterer er ikke tillatt";

$strGroups = "Grupper";
$strOwner = "Eier";

$strDeletePrefix = "Er du sikker p� du vil slette ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "Logg ut";
$menu_applications = "Programmer";
$menu_help = "Hjelp";
$menu_about = "Om";
$menu_documentation = "Dokumentasjon";
$menu_configuration = "Oppsett";
$menu_groups = "Brukergrupper";
$menu_accounts = "Din&nbsp;konto";
$menu_preferences = "Innstillinger";
$menu_bookmarks = "Bokmerker";
$menu_bookmarks_manage = "Endre&nbsp;bokmerker";
$menu_add_bookmark = "Tilf�y&nbsp;bokmerke";
$menu_add_current = "Tilf�y&nbsp;denne&nbsp;siden";

$menu_invite = "Inviter&nbsp;brukere";

$menu_search_discussions = "Diskusjon";
$menu_search_files = "Filer&nbsp;og&nbsp;foldere";
$menu_search_web = "Internett";
$menu_scheduler = "Avtaler";
$menu_filetypes = "Filtyper";
$menu_discussions = "Diskusjonssforum";

$menu_users = "Brukere";
$menu_start = "Startside";
$menu_reboot = "Restart&nbsp;server";
$menu_halt = "Steng&nbsp;server";
$menu_reboot_text = "ADVARSEL! Alle Group-Office funktioner vil v�re midlertidig utilgjengelig. Er du sikker p� du vil fortsette?";
$menu_halt_text = "ADVARSEL! Alle Group-Office funktioner vil bli avbrutt. Er du sikker p� du vil fortsette?";
$menu_new = "y";
$menu_message = "Beskjed";
$menu_contact = "Kontakt";
$menu_event = "Hendelse";
$menu_bookmark = "Bokmerke";
$menu_topics = "Emner";
$menu_support = "Support";
$menu_projects = "Prosjekter";
$menu_sysinfo = "Systeminformasjon";
$menu_admin = 'Administrator';
$menu_modules = 'Moduler';
$menu_manual = 'Brukermanual';

//class validate
$error_required = "Du har ikke fyllt ut et felt under som du m� fylle ut.";
$error_match_auth = "Autorisasjonskoden er feil.";
$error_match_pass = "Adgangskoden passer ikke.";
$error_min_length = "Minste antall karakterer for feltet under:";
$error_max_length = "St�rste antalk karakterer for feltet under:";
$error_email = "E-postadressen er ugyldig.";
$error_email_exists = "E-postadressen eksisterer allerede.";
$error_username_exists = "Brukernavnet finnes allerede. Du m� velge et annet.";
$errors_in_form = "Der er feil i inntastningen. Rett feilen og pr�v igen.";
$error_missing_field = "Du m� vennligst fylle ut alle n�dvendige felt.";

//acl control
$acl_control_auth_groups = "Autoriserte grupper";
$acl_control_auth_users = "Autoriserte brukere";
$acl_control_no_contacts = "Du kan ikke velge noeb kontakter.<br /> Bruk adresseboken til � legge til kontakter.";

$strShowProfile = "Se profil";
$strShowGroup = "Se gruppemedlemmer";
$strProcessingTime = "Behandlingen av denne side tok ";
$strLoggedInAs = "Logget inn som ";
$strAt = " at ";
$strEmailTo = "Send e-post til: ";
$strProtectedUser = "Beskyttet bruker";

$http_not_found = "Dokumentet finnes ikke";
$http_not_found_text = "Det s�kte dokument ble ikke funnet. Har du skrevet rigtig?";
$http_error = "Intern Serverfeil";
$http_bad_request = "Bad request";
$strAddContact = "Tilf�y til kontakter";

$strReadRights = "Leserettigheter";
$strWriteRights = "Skriverettigheter";
$strPermissions = 'Rettigheter';

$editor_bold = "Fet";
$editor_italic = "Kursiv";
$editor_underline = "Understreket";
$editor_left = "Venstre";
$editor_right = "H�yre";
$editor_center = "Sentrert";
$editor_ord_list = "Ordnet liste";
$editor_bul_list = "Bullet liste";
$editor_txt_color = "Tekstfarve";
$editor_back_color = "Bakgrunnsfarge";
$editor_hyperlink = "Hyperlink";
$editor_image = "Bilde";
$editor_rule = "Horisontal Linje";

$editor_very_small = "Veldig liten";
$editor_small = "Liten";
$editor_medium = "Medium";
$editor_large = "Stor";
$editor_larger = "St�rre";
$editor_very_large = "Meget stor";

$editor_strikethrough = "Gjennemstreket";
$editor_outdent = "Rykk ut";
$editor_indent = "Innrykk";

$editor_superscript = "Hevet skrift";
$editor_subscript = "Senket skrift";
$editor_undo = "Endre";
$editor_redo = "Endre tilbake";
$editor_copy = "Kopier";
$editor_paste = "Sett inn";
$editor_cut = "Klipp";

$editor_font = "Font";
$editor_size = "St�rrelse";
$editor_heading = "Overskrift";
$editor_select = "Velg";

$editor_mode = "Skift redigeringsm�te";

$days[0]="S�";
$days[1]="Ma";
$days[2]="Ti";
$days[3]="On";
$days[4]="To";
$days[5]="Fr";
$days[6]="L�";

$months[0]="Januar";
$months[1]="Februar";
$months[2]="Mars";
$months[3]="April";
$months[4]="Mai";
$months[5]="Juni";
$months[6]="Juli";
$months[7]="August";
$months[8]="September";
$months[9]="Oktober";
$months[10]="November";
$months[11]="Desember";

$full_days[0] = "S�ndag";
$full_days[1] = "Mandag";
$full_days[2] = "Tirsdag";
$full_days[3] = "Onsdag";
$full_days[4] = "Torsdag";
$full_days[5]= "Fredag";
$full_days[6] = "L�rdag";

$strLoadTime = 'Generert p�';
$strDescription = 'Beskrivelse';


$editorCmd['textindicator'] = 'N�v�rende stil';
$strTitle = 'Tittel';
$lang_modules['cms'] = "websider";


$editorCmd['undo'] = "G� tilbake";
$editorCmd['redo'] = "Gj�r om";
$editorCmd['bold'] = "Fet";
$editorCmd['italic'] = "Kursiv";
$editorCmd['underline'] = "Understreket";
$editorCmd['left'] = "Juster til venstre";
$editorCmd['right'] = "Juster til h�yre";
$editorCmd['full'] = "Fyll";
$editorCmd['center'] = "Juster til senter";
$editorCmd['ord_list'] = "Numerert liste";
$editorCmd['bul_list'] = "Bulleted liste";
$editorCmd['txt_color'] = "Tekstfarger";
$editorCmd['back_color'] = "Bakgrunnsfarger";
$editorCmd['hyperlink'] = "Hyperlink";
$editorCmd['image'] = "Leggtil bilde";
$editorCmd['rule'] = "Horisontal linje";
$editorCmd['table'] = "Sett in tabell";

$editorCmd['strikethrough'] = "Gjennomstreket";
$editorCmd['outdent'] = "Still inn";
$editorCmd['indent'] = "Still ut";

$editorCmd['superscript'] = "Superscript";
$editorCmd['subscript'] = "Subscript";
$editorCmd['undo'] = "G� tilbake";
$editorCmd['redo'] = "Gj�r om";
$editorCmd['copy'] = "Kopier";
$editorCmd['paste'] = "Klipp inn";
$editorCmd['cut'] = "Kutt";

$editorCmd['font'] = "Font";
$editorCmd['size'] = "St�rrelse";
$editorCmd['heading'] = "Overskrift";
$editorCmd['select'] = "Velg";
$editorCmd['mode'] = "Endre modus";
$editorCmd['textindicator'] = 'N�v�rende stil';


$lang_modules['addressbook'] = "Addressebok";
$cmdShowAll = 'Vis alle';

$cmdSearch = 'S�k';

?>
