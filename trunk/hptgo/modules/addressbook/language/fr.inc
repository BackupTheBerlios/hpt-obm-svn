<?php
require($GO_LANGUAGE->get_fallback_language_file('addressbook'));

//contacts
$contacts_title = "Carnet d'adresses";
$contacts_no_contacts = "Pas de contacts.";
$contacts_members = "membres";
$contacts_contacts = "contacts";
$contacts_add = "Nouveau contact";
$contacts_groups = "groupes";
$contacts_other = "Autres Contacts";
$contacts_move_to_group = "D�placer les contacts s�lectionn�s vers le groupe...";
$groups_no_groups = "Vous n'avez pas de groupe";
$contacts_group = "groupe";
$contacts_empty_group = "Utilisez la liste d�roulante pour d�placer des contacts dans ce groupe.";
$contacts_this_group = 'ce groupe';

$add_contacts_text = "Entrez les adresses mail des membres et codes d'autorisation et cliquez sur 'Ok'";
$add_contacts_success = "L'utilisateur a �t� ajout�";
$add_contacts_wrong = "Mauvais email ou code d'autorisation.";
$add_personal_text = "Cliquez sur 'Enregistrer' pour ajouter le contact. Les champs marqu�s d'une * sont obligatoires.";
$contacts_select = "Choisissez les contacts � ajouter et cliquez sur 'Ok'.";
$contacts_select_title = "Contacts";
$contact_profile = "Profils de contact";
$user_profile = "Profil utilisateur";
$contacts_authorise = "autorisation";
$contacts_add_member = "Ajouter ce membre � vos contacts";
$strConfirmSynchronise = "ceci mettra � jour ce contact avec les donn�es que l'utilisateur a lui m�me saisi. Des changements manuels de votre part seront perdus. Etes vous certain(e) de vouloir continuer ?";
$contacts_import = "importer";
$contacts_export = "exporter";
$contacts_export_text = "Choisissez le type de fichier CSV que vous souhaitez cr�er.";
$contacts_delete = "supprimer";

$contacts_import_text = "Choisissez un fichier CSV et cliquez sur 'Ok'";
$contacts_import_text_group = "Choisissez un groupe et le fichier CSV. Cliquez sur 'Ok'.";
$fbNoFile = "Pas de fichier re�u";
$fbPleaseWait = "Patientez pendant le chargement du fichier...";

$strNotIncluded = "Ne pas importer";
$contacts_import_feedback = "Les colonnes suivantes ont �t� trouv�s dans le fichier CSV.";
$contacts_import_feedback = "Choisissez le groupe dans lequel vous souhaitez faire l'importation ou laissez le programme cr�er automatiquement des groupes en se basant sur une colonne du fichier.";
$contacts_import_feedback = " Faites correspondre les colonnes de Group-Office avec les colonnes du fichier et assurez vous de ne pas avoir choisi deux fois la m�me colonne.";
$contacts_separator = "Les valeurs sont s�par�es par";
$contacts_no_separator = "Vous n'avez pas sp�cifi� un s�parateur de caract�res.";
$fbFilename = "Nom de fichier";
$contacts_import_title = "Importer un carnet d'adresses";
$contacts_import_success = "Le fichier a �t� import� avec succ�s";
$contacts_import_double = "Vous avez s�lectionn� une ou plusieurs colonnes en double. Corrigez le et recommencez.";
//changed:
$contacts_import_noname = 'Vous devez choisir le champs  \'Nom\' and \'Pr�nom\' ';
$contacts_import_incompatible = "Format de fichier incompatible.Un fichier CSV est un fichier dont les valeurs sont s�par�es par des virgules.";
$strMember = "Membre";
$contact_exist_warning = "Note: Cet utilisateur est d�j� dans votre carnet d'adresses.";
$contacts_add_to_group = "Ajouter les utilisateurs s�lectionn�s dans le groupe de contacts...";
$contacts_add_to = "Ajouter les utilisateurs s�lectionn�s dans les contacts";
$contacts_add_user = "Ajouter";
$contacts_edit = "Modifier";
$contacts_no_select = "Vous n'avez rien s�lectionn�";
$contacts_this_contact = "ce contact";

$contacts_seperator_text = 'Valeurs s�par�es par';
$contacts_quote = 'Valeurs entour�es par';
$contacts_clrf = 'Lignes termin�es par';
$contacts_auto_group = 'Cr�er automatiquement des groupes depuis le fichier';
$contacts_import_to_group = 'Importer dans le groupe';

$contacts_search = 'Recherche';
$cmdSearch = 'Recherche';
$contacts_search_text = "Entrez votre recherche et cliquez sur 'recherche'. Vous pouvez utiliser le caract�re jocker *";
$contacts_results = 'r�sultat(s)';
$contacts_result = 'r�sultat(s)';


$contacts_color = 'Couleur';

$contacts_colors['black'] = 'Noir';
$contacts_colors['red'] = 'Rouge';
$contacts_colors['green'] = 'Vert';
$contacts_colors['blue'] = 'Bleu';
$contacts_colors['pink'] = 'Rose';
$contacts_colors['orange'] = 'Orange';
$contacts_colors['brown'] = 'Brun';

$ab_addressbooks = 'Carnet d\'adresses';
$ab_owner = 'Propri�taire';
$ab_subscribed = 'Souscrit';
$ab_new_ab = 'Nouveau carnet d\'adresses';
$ab_addressbook_exists = 'Un carnet d\'adresses avec un nom identique existe d�j�';
$ab_addressbook = 'Carnet d\'adresses';
$contacts_select_single = 'Cliquez sur un contact pour s�lectionner cette personne';

$ab_group_exists = 'Il y a d�j� un carnet d\'adresses avec ce nom';

//translation required
$ab_edit_contact = 'Cliquez sur \'Ok\' pour enregistrer les changements. Les champs marqu�s d\'une �toile sont obligatoires';
$ab_group_on_file = 'Grouper les contact en se basant sur le fichier';
$ab_default = 'D�faut';
$ab_browse = 'Parcourir';
$ab_comment = 'Commentaire';

$ab_contact = 'Contact';
$ab_send_message = 'E-mail';
$ab_new_event = 'Ev�nement';

$ab_templates = 'Templates';
$ab_your_templates = 'le votre';
$ab_all_templates = 'tous';

$template_type[0] = 'E-mail';
$template_type[1] = 'OpenOffice.Org';
$ab_type = 'Type';
$ab_template_exists = 'Un template avec ce nom existe d�j�.';

$sir_madam['M'] = 'monsieur';
$sir_madam['F'] = 'madame';

$ab_upload_oo = 'Charger le template de OpenOffice.org';
$ab_upload_oo_text = 'Choisissez le templace que vous avez �dit� avec OpenOffice.org et cliquez sur \'Ok\'';
$ab_upload_nofile = 'Erreur pendant l\'envoi du fichier.';

$ab_oo_doc = 'Document texte';
$ab_select_template = 'Choisissez un template pour ce document:';
$ab_add_field_data = 'Vous pouvez utiliser les valeurs suivantes dans votre document :';
$ab_datafield = 'Champs';

$ab_no_template = 'Pas de template';
$ab_new_note = 'Note';

$ab_events = 'Rendez-vous';

$ab_new_contact = 'Nouveau contact';
$ab_new_company = 'Nouvelle soci�t�';
$ab_companies = 'Soci�t�s';
$ab_company_properties = 'Profil soci�t�';
$ab_bank_no = 'Num�ro de Banque';
$ab_companies_count_single = 'soci�t�';
$ab_companies_count = 'soci�t�s';
$ab_no_companies = 'Aucunes soci�t�s trouv�es';
$ab_no_company = 'Aucune soci�t�';

$ab_import_type = 'Importer';
$ab_no_company_found = 'Aucunes soci�t�s trouv�es';
$ab_search_company = 'Consultation soci�t�';
$ab_edit_company = 'Changer le profil de la soci�t�';
$ab_new_group = 'Nouveau groupe';
$ab_export_type = 'Exporter';

$ab_custom_fields = 'Champs suppl�m�ntaires';
$ab_field_size = 'Taille';

$ab_new_field = 'Nouveau champ';
$ab_invalid_field_size = 'Taille invalide. Choisissez une valeur comprise entre 10 et 255.';
$ab_field_exists = 'Un champ avec le m�me nom existe d�j�.';

$ab_employees = 'Employ�s';
$ab_no_employees = 'Aucun Employ�';

$ab_employee_count_single = 'employ�';
$ab_employee_count_multiple = 'employ�s';
$ab_vat_no = 'Num�ro de VAT';

$ab_add_new = 'Ajouter un nouveau';
$ab_add_existing = 'Add existing';

$ab_access = 'Acc�s';
//1.98
$ab_mailings = 'Mailings';
$ab_mailing_groups = 'Mailing groups';
$ab_no_mailing_groups = 'NO mailing groups found';
$ab_no_templates = 'NO templates found';
$ab_mailing_group_exists = 'A mailing group with this name already exists.';
$ab_contacts_count_single = 'contact';
$ab_contacts_count_multiple = 'contacts';
$ab_new_mailing = 'New mailing';
$ab_all_your_addressbooks = 'tous les carnets d\'adresses';

$ab_search_for = 'Recherche de';
$ab_search_in = 'dans';
$ab_search_on = 'sur';
$ab_search_keyword = 'Rechercher la phrase';
$ab_wildcard = '(Utilisez \'%\' comme wildcard)';
//2.03
$ab_copy_read_acl = 'Copy read permissions to contact and companies';
$ab_copy_write_acl = 'Copy write permissions to contact and companies';

$ab_catagory_exists = 'Une cat�gorie avec le m�me nom existe d�j�.';
$ab_add_catagory = 'Ajouter une cat�gorie';
$ab_add_field = 'Ajouter un champ';
$ab_catagory = 'Cat�gorie';
?>
