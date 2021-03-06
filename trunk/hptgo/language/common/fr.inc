<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-15';

$lang_modules['scheduler'] = 'Agenda';
$lang_modules['calendar'] = "Calendrier";
$lang_modules['filesystem'] = 'Fichiers';
$lang_modules['email'] = 'E-mail';
$lang_modules['projects'] = 'Projets';
$lang_modules['search'] = 'Chercher';
$lang_modules['contacts'] = "Carnet d'adresses";

$lang_modules['cms'] = "Site Web";

$lang_modules['notes'] = "Notes";
$lang_modules['bookmarks'] = "Bookmarks";
$lang_modules['summary'] = "Page d'acceuil";
$lang_modules['todos'] = "T�ches";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Connexion";
$cmdSave = "enregistrer";
$cmdBack = "retour";
$cmdCancel = "annuler";
$cmdClose = "fermer";
$cmdOk = "Ok";
$cmdDelete = "supprimer";
$cmdNext = "Suivant";
$cmdPrevious = "Pr�c�dent";
$cmdContinue = "Continuer";
$cmdUpdate = "Synchroniser";
$cmdApply = "Appliquer";
$cmdYes = "Oui";
$cmdNo = "Non";
$cmdAdd = 'Ajouter';
$cmdEdit = 'Editer';
$cmdInvertSelection = 'Inverser la s�lection';

$strEdit = "Editer";
$strThis = 'cet';
$strItems = '�l�ments';
$strDescription = 'Description';

$strName = "Nom";
$strUser = "Utilisateur";
$strUsername = "Nom d'utilisateur";
$strPassword = "Mot de passe";
$strAuthcode = "Code d'autorisation";
$strCountry = "Pays";
$strState = "Etat";
$strCity = "Ville";
$strZip = "Code Postal";
$strAddress = "Rue";
$strEmail = "E-mail";
$strPhone = "T�l�phone";
$strWorkphone = "T�l�phone profesionnel";
$strCellular = "Cellulaire";
$strCompany = "Soci�t�";
$strDepartment = "D�partement";
$strFunction = "Fonction";
$strQuestion = "Question secr�te";
$strAnswer = "R�ponse";
$strFax = "Fax";
$strWorkFax = "Fax profesionnel";
$strHomepage = "site web";
$strWorkAddress = "Adresse profesionnelle";
$strWorkZip = "Code postal professionnel";
$strWorkCountry = "Pays de travail";
$strWorkState = "Etat de travail";
$strWorkCity = "Ville de travail";
$strPhoto = "Photo";
$strComments = "Commentaires";

$strProperties = 'propri�t�s';

$strDeleteItem = "Supprimer";

$strDate = "Date";

$strSaveSuccess = "Les donn�es ont �t� enregistr�es avec succ�s";
$strSaveError = "Une erreur est apparue pendant la sauvegarde des donn�es. R�essayez.";
$strDataError = "Une erreur est apparue pendant la lecture des donn�es. Rechargez cette page.";
$strDeleteError = "Impossible d'effacer les donn�es.";
$strAclError = "Erreur de contr�le d'acc�s";
$strDateError = "Vous avez entrez une date invalide";
$strAccessDenied = "Acc�s refus�";
$AccessDenied_text = "Vous n'�tes pas autoris� � voir cette section du site ni � effectuer cette action.Un autre utlisateur a restreint l'acc�s � l'information que vous avez demand�.";
$invalid_chars = "Les caract�res suivants ne sont pas autoris�s";
$strGroups = "Groupes";
$strOwner = "Propri�taire";
$strDeletePrefix = "Etes vous certain(e) de vouloir supprimer";
$strDeleteSuffix = "?";
//menu
$menu_logout = "D�connecter";
$menu_applications = "Applications";
$menu_help = "Aide";
$menu_about = "A propos de ";
$menu_documentation = "Documentation";
$menu_configuration = "Configuration";
$menu_groups = "G�rer Utilisateurs groupes";
$menu_accounts = "Votre compte";
$menu_preferences = "Pr�f�rences";
$menu_search_discussions = "Discussions";
$menu_search_files = "Fichiers et dossiers";
$menu_search_web = "Le web";
$menu_filetypes = "Types de fichiers";
$menu_discussions = "Forum de discussion";

$menu_users = "Utilisateurs";
$menu_new = "Nouveau";
$menu_message = "Message";
$menu_contact = "Contact";
$menu_event = "Ev�nement";
$menu_bookmark = "Favoris";
$menu_topics = "Mati�res";
$menu_support = "Support";
$menu_projects = "Projets";
$menu_sysinfo = "Informations syst�me";
$menu_admin = 'Administrateur';
$menu_modules = 'Modules';
$menu_manual = 'Manuel';

//class validate
$error_required = "Vous n'avez pas sp�cifi� le champs recquis ci-dessous.";
$error_match_auth = "Le code d'autorisation ne correspond pas.";
$error_match_pass = "Le mot de passe de correspond pas.";
$error_min_length = "Nombre minimun de caract�res du champs ci-dessous :";
$error_max_length = "Nombre maximum de caract�res du champs ci-dessous :";
$error_email = "L'adresse mail que vous avez sp�cifi� n'est pas valide.";
$error_email_exists = "L'adresse email que vous avez entr� existe d�j�.";
$error_username_exists = "D�sol�, le nom d'utilisateur que vous avez entr� existe d�j�. Merci d'en choisir un autre.";
$errors_in_form = "Il y a des erreurs dans le formulaire. Corrigez les et r�essayez.";
$error_missing_field = "Vous n'avez pas rempli tous les champs obligatoires.";

//acl control
$acl_control_auth_groups = "Groupes autoris�s";
$acl_control_auth_users = "Utilisateurs autoris�s";
$acl_control_no_contacts = "Vous n'avez aucun contact � s�lectionner. Utilisez le carnet d'adresses pour ajouter des membres � vos contacts.";

$strShowProfile = "Voir le profil";
$strShowGroup = "Voir les membres du groupe";
$strProcessingTime = "Le traitement de cette page � pris ";
$strLoggedInAs = "Connect� en tant que ";
$strAt = " � ";
$strEmailTo = "Envoyer un E-mail � : ";
$strProtectedUser = "Utilisateur prot�g�";

$http_not_found = "Document introuvable";
$http_not_found_text = "Le document demand� est introuvable. Vous avez peut �tre mal tap� l'adresse.";
$http_error = "Erreur interne au serveur";
$http_bad_request = "Mauvaise requ�te";
$strAddContact = "Ajouter aux contacts";

$strReadRights = "Permissions de lecture";
$strWriteRights = "Permissions d'�criture";
$strPermissions = 'permissions';

$days[0]="Dim";
$days[1]="Lun";
$days[2]="Mar";
$days[3]="Mer";
$days[4]="Jeu";
$days[5]="Ven";
$days[6]="Sam";

$months[0]="Janvier";
$months[1]="F�vrier";
$months[2]="Mars";
$months[3]="Avril";
$months[4]="Mai";
$months[5]="Juin";
$months[6]="Juillet";
$months[7]="Ao�t";
$months[8]="Septembre";
$months[9]="Octobre";
$months[10]="Novembre";
$months[11]="D�cembre";

$full_days[0] = "Dimanche";
$full_days[1] = "Lundi";
$full_days[2] = "Mardi";
$full_days[3] = "Mercredi";
$full_days[4] = "Jeudi";
$full_days[5]= "Vendredi";
$full_days[6] = "Samedi";
$strLoadTime = 'g�n�r� en ';
$strDescription = 'Description';



$lang_modules['addressbook'] = "Carnet d'adresses";
$cmdShowAll = 'Tout voirr';
$cmdSearch = 'Chercher';

//translation required
$cmdYesToAll = 'Oui pour tout';
$cmdNoToAll = 'non � tout';

$short_months[0]="Jan";
$short_months[1]="F�v";
$short_months[2]="Mar";
$short_months[3]="Avr";
$short_months[4]="Mai";
$short_months[5]="Juin";
$short_months[6]="Jul";
$short_months[7]="Ao�t";
$short_months[8]="Sept";
$short_months[9]="Oct";
$short_months[10]="Nov";
$short_months[11]="D�c";

$strToday = 'Aujourd\'hui';

$cmdPrint = 'Imprimer';
$strFirstName = 'Nom';
$strLastName = 'Pr�nom';
$strInitials = 'Initiales';
$strSex = 'Sexe';
$strBirthday = 'Anniversaire';
$strSexes['M'] = 'Homme';
$strSexes['F'] = 'Femme';

$strTitle = 'Titre';
$strBeginning = 'D�but';
$strMiddleName = 'Compl�ment du nom';
$sir_madam['M'] = 'monsieur';
$sir_madam['F'] = 'madame';
$strModifiedAt = 'Modifi� le';
$strCreatedAt = 'Cr�e le';

$htmlarea_language_name = 'fr';
$strLocalization = 'Localisation';


//look up the language in controls/jscalendar/lang
$jscalendar_language_name = 'calendar-fr.js';


//2.01 translations

//look up language in 'classes/phpmailer/language'
$php_mailer_lang = 'fr';
$strEmailFail = 'L\'envois de l\'email a �chou�.';

//for people that want to use ktmlarea
$ktml_language = 'french';

$ml_host_unreachable = ' is unreachable. Automatic checking of e-mail is disabled.';
$strRegion = 'R�gion';

//2.05
$cmdReset = 'Remise � z�ro';
$strMyName = 'Mon pr�nom';


//translation required

$menu_bookmarks = "Favoris";
$menu_bookmarks_manage = "Modifier  les favoris";
$menu_add_bookmark = "Ajouter un favoris";
$menu_add_current = "Ajouter la page courante";
$menu_invite = "inviter des utilisateurs";
$menu_scheduler = "Agenda";
$menu_reboot = "Red�marrer le serveur";
$menu_start = "Page de d�marrage";
$menu_halt = "Arr�ter le serveur";
$menu_reboot_text = "ATTENTION! Tous les services de Group-Office seront temporairement indisponibles. Etes vous certain(e) de vouloir continuer ?";
$menu_halt_text = "ATTENTION ! Tous les services de Group-Office seront arr�t�s. Etes vous certain(e) de vouloir continuer ?";

$strShortWeek = 'semaine';

$editor_bold = "Gras";
$editor_italic = "Italique";
$editor_underline = "Soulign�";
$editor_left = "Gauche";
$editor_right = "Droite";
$editor_center = "Centrer";
$editor_ord_list = "Liste tri�e";
$editor_bul_list = "Liste � puces";
$editor_txt_color = "Couleur du texte";
$editor_back_color = "Couleur de fond";
$editor_hyperlink = "Lien hypertexte";
$editor_image = "Image";
$editor_rule = "R�gle horizontale";
$editor_very_small = "Tr�s petit";
$editor_small = "Petit";
$editor_medium = "Moyen";
$editor_large = "Large";
$editor_larger = "Plus large";
$editor_very_large = "tr�s large";
$editor_strikethrough = "Barr�";
$editor_outdent = "Retrait � gauche";
$editor_indent = "Retrait � droite";
$editor_superscript = "Exposant";
$editor_subscript = "Indice";
$editor_undo = "Annuler";
$editor_redo = "Refaire";
$editor_copy = "Copier";
$editor_paste = "Coller";
$editor_cut = "Couper";
$editor_font = "Police";
$editor_size = "Taille";
$editor_heading = "Ent�te";
$editor_select = "Choix";
$editor_mode = "Changer le mode �diteur";

$editorCmd['undo'] = "Annuler";
$editorCmd['redo'] = "Refaire";
$editorCmd['bold'] = "Gras";
$editorCmd['italic'] = "Italique";
$editorCmd['underline'] = "Soulign�";
$editorCmd['left'] = "Justifi� � gauche";
$editorCmd['right'] = "Justifi� � droite";
$editorCmd['full'] = "Remplir";
$editorCmd['center'] = "Centrer";
$editorCmd['ord_list'] = "Liste num�rot�e";
$editorCmd['bul_list'] = "Liste � puces";
$editorCmd['txt_color'] = "Couleur du texte";
$editorCmd['back_color'] = "Couleur de fond";
$editorCmd['hyperlink'] = "Hyper lien";
$editorCmd['image'] = "Ins�rer une image";
$editorCmd['rule'] = "R�gle horizontale";
$editorCmd['table'] = "Ins�rer une table";
$editorCmd['strikethrough'] = "Barr�";
$editorCmd['outdent'] = "Augmenter le retrait";
$editorCmd['indent'] = "Diminuer le retrait";
$editorCmd['superscript'] = "Exposant";
$editorCmd['subscript'] = "Indice";
$editorCmd['undo'] = "Annuler";
$editorCmd['redo'] = "Refaire";
$editorCmd['copy'] = "Copier";
$editorCmd['paste'] = "Coller";
$editorCmd['cut'] = "Couper";
$editorCmd['font'] = "Police";
$editorCmd['size'] = "Taille";
$editorCmd['heading'] = "En t�te";
$editorCmd['select'] = "S�lectionner";
$editorCmd['mode'] = "Changer de mode";
$editorCmd['textindicator'] = 'Style courant';

$cal_prev_year = "Ann�e pr�c�dente (maintenez pour le menu)";
$cal_prev_month  = "Mois pr�c�dent (maintenez pour le menu)";
$cal_go_today = "Aller � aujourd'hui";
$cal_next_month = "Mois suivant (maintenez pour le menu)";
$cal_next_year  = "Ann�e suivante (maintenez pour le menu)";
$cal_drag_to_move = "Glissez pour d�placer";
$cal_part_today = " (aujourd'hui)";
$cal_monday_first = "Voir d'abord les lundi";
$cal_sunday_first  = "Voir d'abord les dimanche";
$cal_select_date = "Choisir une date";


?>
