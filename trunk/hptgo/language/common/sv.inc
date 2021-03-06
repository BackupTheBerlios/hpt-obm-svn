<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

$lang_modules['scheduler'] = "tidsschema";
$lang_modules['filesystem'] = 'filer';
$lang_modules['email'] = 'e-post';
$lang_modules['projects'] = 'projekt';
$lang_modules['search'] = "s�k";
$lang_modules['contacts']  = "adressbok";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "login";
$cmdSave = "spara";
$cmdBack = "tillbaka";
$cmdCancel = "avbryt";
$cmdClose = "st�ng";
$cmdOk = "ok";
$cmdDelete = "Radera";
$cmdNext = "n�sta";
$cmdPrevious = "f�reg�ende";
$cmdContinue = "forts�tt";
$cmdUpdate = "synkronisera";
$cmdApply = "bekr�fta";
$cmdYes = "ja";
$cmdNo = "nej";
$cmdAdd = 'l�gg till';
$cmdEdit = 'editera';
$cmdInvertSelection = 'inverta markering';

$strEdit = "Editera";
$strThis = 'denna';
$strItems = 'sak';

$strName = "Namn";
$strUser = "Anv�ndare";
$strUsername = "Anv�ndarnamn";
$strPassword = "L�senord";
$strAuthcode = "Authoriseringskod";
$strCountry = "Land";
$strState = "Stat";
$strCity = "Stad";
$strZip = "Postnummer";
$strAddress = "Gatuadress";
$strEmail = "E-post";
$strPhone = "Telefon";
$strWorkphone = "Telefon arbete";
$strCellular = "Mobil";
$strCompany = "F�retag";
$strDepartment = "Avdelning";
$strFunction = "Funktion";
$strQuestion = "Hemlig fr�ga";
$strAnswer = "Svar";
$strFax = "Fax";
$strWorkFax = "Fax arbete";
$strHomepage = "Hemsida";
$strWorkAddress = "Adress arbete";
$strWorkZip = "Postnummer arbete";
$strWorkCountry = "Land arbete";
$strWorkState = "Stat arbete";
$strWorkCity = "Stad arbete";
$strPhoto = "Foto";
$strComments = "Kommentarer";

$strProperties = 'egenskaper';

$strDeleteItem = "Radera";

$strDate = "Datum";

$strSaveSuccess = "Data sparat";
$strSaveError = "Ett fel uppstod vid sparningen av data. Var god f�rs�k igen";
$strDataError = "Ett fel uppstod vid l�sningen av data. F�rs�k ladda om sidan igen";
$strDeleteError = "Kunde inte radera data.";
$strAclError = "�tkomstfel";
$strDateError = "Du angav ett felaktigt datum";



$strAccessDenied = "Beh�righet nekas";
$AccessDenied_text = "Du �r inte beh�rig att kolla p� denna sektion av siten eller utf�ra denna �terg�rd. <br /> En annan anv�ndare har restrikterat �tkomligheten till den information du f�rs�kte n�.";

$invalid_chars = "F�ljande tecken �r inte till�tna";

$strGroups = "Grupper";
$strOwner = "�gare";

$strDeletePrefix = "�r du s�ker p� att du vill radera ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "logga ur";
$menu_applications = "programvaror";
$menu_help = "hj�lp";
$menu_about = "om";
$menu_documentation = "dokumentation";
$menu_configuration = "konfiguration";
$menu_groups = "anv�ndare&nbsp;grupper";
$menu_accounts = "ditt&nbsp;konto";
$menu_preferences = "preferences";
$menu_bookmarks = "bokm�rken";
$menu_bookmarks_manage = "modifiera&nbsp;bokm�rken";
$menu_add_bookmark = "l�gg&nbsp;till&nbsp;bokm�rken";
$menu_add_current = "l�gg&nbsp;till&nbsp;befintlig&nbsp;sida";

$menu_invite = "bjud&nbsp;in&nbsp;anv�ndare";

$menu_search_discussions = "diskussioner";
$menu_search_files = "filer&nbsp;och&nbsp;kataloger";
$menu_search_web = "webben";
$menu_scheduler = "tidsschema";
$menu_filetypes = "filtyper";
$menu_discussions = "diskussions&nbsp;forum";

$menu_users = "anv�ndare";
$menu_start = "startsida";
$menu_reboot = "starta&nbsp;om&nbsp;server";
$menu_halt = "st�ng&nbsp;ned&nbsp;server";
$menu_reboot_text = "VARNING! Alla Group-Office-tj�nster kommer att vara tempor�rt bortkopplade. �r du s�ker p� att du vill forts�tta?";
$menu_halt_text = "VARNING! All Group-Office-tj�nster kommera att avslutas. �r du s�ker p� att du vill forts�tta?";
$menu_new = "ny";
$menu_message = "meddelande";
$menu_contact = "kontakt";
$menu_event = "h�ndelse";
$menu_bookmark = "bokm�rke";
$menu_topics = "rubriker";
$menu_support = "support";
$menu_projects = "projekt";
$menu_sysinfo = "systeminformation";
$menu_admin = 'administrator';
$menu_modules = 'moduler';
$menu_manual = 'manual';

//class validate
$error_required = "Du specifierade inte det kr�vda f�ltet nedanf�r.";
$error_match_auth = "Authoriseringskoderna matchar inte.";
$error_match_pass = "L�senorden �verensst�mmer itne.";
$error_min_length = "Minst antal tecken av f�lten nedan:";
$error_max_length = "Max antal tecken av f�lten nedan:";
$error_email = "E-postadressen du angav �r ogilitig.";
$error_email_exists = "E-postadressen du angav �r redan registrerad.";
$error_username_exists = "Tyv�rr, anv�ndarnamnet du angav existerar redan. Var god v�lj ett annat.";
$errors_in_form = "Det �r fel i formul�ret. Korrigera dessa och pr�va igen.";
$error_missing_field = "Du fyllde inte i alla kr�vda f�lt.";

//acl control
$acl_control_auth_groups = "Aktoriserade grupper";
$acl_control_auth_users = "Akotriserad anv�ndare";
$acl_control_no_contacts = "Du har inga medlemskontakter valda.<br />Anv�nd adressboken f�r att l�gga in medlemmar till dina kontakter.";

$strShowProfile = "Kolla profil";
$strShowGroup = "Kolla gruppanv�ndare";
$strProcessingTime = "Laddningen av denna sida tog ";
$strLoggedInAs = "Inloggad som ";
$strAt = " at ";
$strEmailTo = "Skicka E-postmeddelande till: ";
$strProtectedUser = "Skyddad anv�ndare";

$http_not_found = "Dokumentet hittades inte";
$http_not_found_text = "Det beg�rda dokumentet hittades inte. Du kanske skrev fel URL.";
$http_error = "Internt serverfel";
$http_bad_request = "Felaktig beg�ran";
$strAddContact = "L�ggtill kontakter";

$strReadRights = "L�sr�ttigheter";
$strWriteRights = "Skrivr�ttigheter";
$strPermissions = 'r�ttigheter';

$editor_bold = "Fet";
$editor_italic = "Kursiverad";
$editor_underline = "Understruken";
$editor_left = "V�nster";
$editor_right = "H�ger";
$editor_center = "Centrerad";
$editor_ord_list = "Ordnadlista";
$editor_bul_list = "Bullet list";
$editor_txt_color = "Textf�rg";
$editor_back_color = "Bakgrundsf�rg";
$editor_hyperlink = "Hyperl�nk";
$editor_image = "Bild";
$editor_rule = "Horisontal linje";

$editor_very_small = "Mycket litet";
$editor_small = "Litet";
$editor_medium = "Medium";
$editor_large = "Stor";
$editor_larger = "St�rre";
$editor_very_large = "Mycket stor";

$editor_strikethrough = "Strike Through";
$editor_outdent = "Outdent";
$editor_indent = "Indent";

$editor_superscript = "Superskript";
$editor_subscript = "Subscript";
$editor_undo = "�ngra";
$editor_redo = "�ndra tillbaka";
$editor_copy = "Kopiera";
$editor_paste = "Klistra in";
$editor_cut = "Klipp ut";

$editor_font = "Font";
$editor_size = "Storlek";
$editor_heading = "Rubrik";
$editor_select = "V�ljd";

$editor_mode = "Byt editeringsl�ge";

$days[0]="S�";
$days[1]="M�";
$days[2]="Ti";
$days[3]="On";
$days[4]="To";
$days[5]="Fr";
$days[6]="L�";

$months[0]="Januaryi";
$months[1]="Februari";
$months[2]="Mars";
$months[3]="April";
$months[4]="Maj";
$months[5]="Juni";
$months[6]="Juli";
$months[7]="Augusti";
$months[8]="September";
$months[9]="Oktober";
$months[10]="November";
$months[11]="December";

$full_days[0] = "S�ndag";
$full_days[1] = "M�ndag";
$full_days[2] = "Tisdag";
$full_days[3] = "Onsdag";
$full_days[4] = "Torsdag";
$full_days[5] = "Fredag";
$full_days[6] = "L�rdag";

$strLoadTime = 'Skapad';
$strDescription = 'Beskrivning';

$strTitle = 'Titel';
$lang_modules['cms'] = "websidor";
$editorCmd['undo'] = "�ngra";
$editorCmd['redo'] = "G�r om";
$editorCmd['bold'] = "Fet";
$editorCmd['italic'] = "Kursiv";
$editorCmd['underline'] = "Understruken";
$editorCmd['left'] = "Justera v�nster";
$editorCmd['right'] = "Justera h�ger";
$editorCmd['full'] = "Fyll";
$editorCmd['center'] = "Justera mitten";
$editorCmd['ord_list'] = "Numrerad lista";
$editorCmd['bul_list'] = "Punktlista";
$editorCmd['txt_color'] = "Textf�rg";
$editorCmd['back_color'] = "Bakgrundsf�rg";
$editorCmd['hyperlink'] = "Hyperl�nk";
$editorCmd['image'] = "Infoga bild";
$editorCmd['rule'] = "Horizontel linje";
$editorCmd['table'] = "Infoga tabell";
$editorCmd['strikethrough'] = "Strikethrough";
$editorCmd['outdent'] = "Outdent";
$editorCmd['indent'] = "Indent";
$editorCmd['superscript'] = "Superscript";
$editorCmd['subscript'] = "Subscript";
$editorCmd['copy'] = "Kopiera";
$editorCmd['paste'] = "Klistra in";
$editorCmd['cut'] = "Klipp";
$editorCmd['font'] = "Font";
$editorCmd['size'] = "Storlek";
$editorCmd['heading'] = "Rubrik";
$editorCmd['select'] = "V�lj";
$editorCmd['mode'] = "Byt l�ge";
$editorCmd['textindicator'] = 'Aktuell stil';
?>
