<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

$lang_modules['scheduler'] = "tidsschema";
$lang_modules['filesystem'] = 'filer';
$lang_modules['email'] = 'e-post';
$lang_modules['projects'] = 'projekt';
$lang_modules['search'] = "sök";
$lang_modules['contacts']  = "adressbok";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "login";
$cmdSave = "spara";
$cmdBack = "tillbaka";
$cmdCancel = "avbryt";
$cmdClose = "stäng";
$cmdOk = "ok";
$cmdDelete = "Radera";
$cmdNext = "nästa";
$cmdPrevious = "föregående";
$cmdContinue = "fortsätt";
$cmdUpdate = "synkronisera";
$cmdApply = "bekräfta";
$cmdYes = "ja";
$cmdNo = "nej";
$cmdAdd = 'lägg till';
$cmdEdit = 'editera';
$cmdInvertSelection = 'inverta markering';

$strEdit = "Editera";
$strThis = 'denna';
$strItems = 'sak';

$strName = "Namn";
$strUser = "Användare";
$strUsername = "Användarnamn";
$strPassword = "Lösenord";
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
$strCompany = "Företag";
$strDepartment = "Avdelning";
$strFunction = "Funktion";
$strQuestion = "Hemlig fråga";
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
$strSaveError = "Ett fel uppstod vid sparningen av data. Var god försök igen";
$strDataError = "Ett fel uppstod vid läsningen av data. Försök ladda om sidan igen";
$strDeleteError = "Kunde inte radera data.";
$strAclError = "Åtkomstfel";
$strDateError = "Du angav ett felaktigt datum";



$strAccessDenied = "Behörighet nekas";
$AccessDenied_text = "Du är inte behörig att kolla på denna sektion av siten eller utföra denna återgärd. <br /> En annan användare har restrikterat åtkomligheten till den information du försökte nå.";

$invalid_chars = "Följande tecken är inte tillåtna";

$strGroups = "Grupper";
$strOwner = "Ägare";

$strDeletePrefix = "Är du säker på att du vill radera ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "logga ur";
$menu_applications = "programvaror";
$menu_help = "hjälp";
$menu_about = "om";
$menu_documentation = "dokumentation";
$menu_configuration = "konfiguration";
$menu_groups = "användare&nbsp;grupper";
$menu_accounts = "ditt&nbsp;konto";
$menu_preferences = "preferences";
$menu_bookmarks = "bokmärken";
$menu_bookmarks_manage = "modifiera&nbsp;bokmärken";
$menu_add_bookmark = "lägg&nbsp;till&nbsp;bokmärken";
$menu_add_current = "lägg&nbsp;till&nbsp;befintlig&nbsp;sida";

$menu_invite = "bjud&nbsp;in&nbsp;användare";

$menu_search_discussions = "diskussioner";
$menu_search_files = "filer&nbsp;och&nbsp;kataloger";
$menu_search_web = "webben";
$menu_scheduler = "tidsschema";
$menu_filetypes = "filtyper";
$menu_discussions = "diskussions&nbsp;forum";

$menu_users = "användare";
$menu_start = "startsida";
$menu_reboot = "starta&nbsp;om&nbsp;server";
$menu_halt = "stäng&nbsp;ned&nbsp;server";
$menu_reboot_text = "VARNING! Alla Group-Office-tjänster kommer att vara temporärt bortkopplade. Är du säker på att du vill fortsätta?";
$menu_halt_text = "VARNING! All Group-Office-tjänster kommera att avslutas. Är du säker på att du vill fortsätta?";
$menu_new = "ny";
$menu_message = "meddelande";
$menu_contact = "kontakt";
$menu_event = "händelse";
$menu_bookmark = "bokmärke";
$menu_topics = "rubriker";
$menu_support = "support";
$menu_projects = "projekt";
$menu_sysinfo = "systeminformation";
$menu_admin = 'administrator';
$menu_modules = 'moduler';
$menu_manual = 'manual';

//class validate
$error_required = "Du specifierade inte det krävda fältet nedanför.";
$error_match_auth = "Authoriseringskoderna matchar inte.";
$error_match_pass = "Lösenorden överensstämmer itne.";
$error_min_length = "Minst antal tecken av fälten nedan:";
$error_max_length = "Max antal tecken av fälten nedan:";
$error_email = "E-postadressen du angav är ogilitig.";
$error_email_exists = "E-postadressen du angav är redan registrerad.";
$error_username_exists = "Tyvärr, användarnamnet du angav existerar redan. Var god välj ett annat.";
$errors_in_form = "Det är fel i formuläret. Korrigera dessa och pröva igen.";
$error_missing_field = "Du fyllde inte i alla krävda fält.";

//acl control
$acl_control_auth_groups = "Aktoriserade grupper";
$acl_control_auth_users = "Akotriserad användare";
$acl_control_no_contacts = "Du har inga medlemskontakter valda.<br />Använd adressboken för att lägga in medlemmar till dina kontakter.";

$strShowProfile = "Kolla profil";
$strShowGroup = "Kolla gruppanvändare";
$strProcessingTime = "Laddningen av denna sida tog ";
$strLoggedInAs = "Inloggad som ";
$strAt = " at ";
$strEmailTo = "Skicka E-postmeddelande till: ";
$strProtectedUser = "Skyddad användare";

$http_not_found = "Dokumentet hittades inte";
$http_not_found_text = "Det begärda dokumentet hittades inte. Du kanske skrev fel URL.";
$http_error = "Internt serverfel";
$http_bad_request = "Felaktig begäran";
$strAddContact = "Läggtill kontakter";

$strReadRights = "Läsrättigheter";
$strWriteRights = "Skrivrättigheter";
$strPermissions = 'rättigheter';

$editor_bold = "Fet";
$editor_italic = "Kursiverad";
$editor_underline = "Understruken";
$editor_left = "Vänster";
$editor_right = "Höger";
$editor_center = "Centrerad";
$editor_ord_list = "Ordnadlista";
$editor_bul_list = "Bullet list";
$editor_txt_color = "Textfärg";
$editor_back_color = "Bakgrundsfärg";
$editor_hyperlink = "Hyperlänk";
$editor_image = "Bild";
$editor_rule = "Horisontal linje";

$editor_very_small = "Mycket litet";
$editor_small = "Litet";
$editor_medium = "Medium";
$editor_large = "Stor";
$editor_larger = "Större";
$editor_very_large = "Mycket stor";

$editor_strikethrough = "Strike Through";
$editor_outdent = "Outdent";
$editor_indent = "Indent";

$editor_superscript = "Superskript";
$editor_subscript = "Subscript";
$editor_undo = "Ångra";
$editor_redo = "Ändra tillbaka";
$editor_copy = "Kopiera";
$editor_paste = "Klistra in";
$editor_cut = "Klipp ut";

$editor_font = "Font";
$editor_size = "Storlek";
$editor_heading = "Rubrik";
$editor_select = "Väljd";

$editor_mode = "Byt editeringsläge";

$days[0]="Sö";
$days[1]="Må";
$days[2]="Ti";
$days[3]="On";
$days[4]="To";
$days[5]="Fr";
$days[6]="Lö";

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

$full_days[0] = "Söndag";
$full_days[1] = "Måndag";
$full_days[2] = "Tisdag";
$full_days[3] = "Onsdag";
$full_days[4] = "Torsdag";
$full_days[5] = "Fredag";
$full_days[6] = "Lördag";

$strLoadTime = 'Skapad';
$strDescription = 'Beskrivning';

$strTitle = 'Titel';
$lang_modules['cms'] = "websidor";
$editorCmd['undo'] = "Ångra";
$editorCmd['redo'] = "Gör om";
$editorCmd['bold'] = "Fet";
$editorCmd['italic'] = "Kursiv";
$editorCmd['underline'] = "Understruken";
$editorCmd['left'] = "Justera vänster";
$editorCmd['right'] = "Justera höger";
$editorCmd['full'] = "Fyll";
$editorCmd['center'] = "Justera mitten";
$editorCmd['ord_list'] = "Numrerad lista";
$editorCmd['bul_list'] = "Punktlista";
$editorCmd['txt_color'] = "Textfärg";
$editorCmd['back_color'] = "Bakgrundsfärg";
$editorCmd['hyperlink'] = "Hyperlänk";
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
$editorCmd['select'] = "Välj";
$editorCmd['mode'] = "Byt läge";
$editorCmd['textindicator'] = 'Aktuell stil';
?>
