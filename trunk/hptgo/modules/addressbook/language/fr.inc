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
$contacts_move_to_group = "Déplacer les contacts sélectionnés vers le groupe...";
$groups_no_groups = "Vous n'avez pas de groupe";
$contacts_group = "groupe";
$contacts_empty_group = "Utilisez la liste déroulante pour déplacer des contacts dans ce groupe.";
$contacts_this_group = 'ce groupe';

$add_contacts_text = "Entrez les adresses mail des membres et codes d'autorisation et cliquez sur 'Ok'";
$add_contacts_success = "L'utilisateur a été ajouté";
$add_contacts_wrong = "Mauvais email ou code d'autorisation.";
$add_personal_text = "Cliquez sur 'Enregistrer' pour ajouter le contact. Les champs marqués d'une * sont obligatoires.";
$contacts_select = "Choisissez les contacts à ajouter et cliquez sur 'Ok'.";
$contacts_select_title = "Contacts";
$contact_profile = "Profils de contact";
$user_profile = "Profil utilisateur";
$contacts_authorise = "autorisation";
$contacts_add_member = "Ajouter ce membre à vos contacts";
$strConfirmSynchronise = "ceci mettra à jour ce contact avec les données que l'utilisateur a lui même saisi. Des changements manuels de votre part seront perdus. Etes vous certain(e) de vouloir continuer ?";
$contacts_import = "importer";
$contacts_export = "exporter";
$contacts_export_text = "Choisissez le type de fichier CSV que vous souhaitez créer.";
$contacts_delete = "supprimer";

$contacts_import_text = "Choisissez un fichier CSV et cliquez sur 'Ok'";
$contacts_import_text_group = "Choisissez un groupe et le fichier CSV. Cliquez sur 'Ok'.";
$fbNoFile = "Pas de fichier reçu";
$fbPleaseWait = "Patientez pendant le chargement du fichier...";

$strNotIncluded = "Ne pas importer";
$contacts_import_feedback = "Les colonnes suivantes ont été trouvés dans le fichier CSV.";
$contacts_import_feedback = "Choisissez le groupe dans lequel vous souhaitez faire l'importation ou laissez le programme créer automatiquement des groupes en se basant sur une colonne du fichier.";
$contacts_import_feedback = " Faites correspondre les colonnes de Group-Office avec les colonnes du fichier et assurez vous de ne pas avoir choisi deux fois la même colonne.";
$contacts_separator = "Les valeurs sont séparées par";
$contacts_no_separator = "Vous n'avez pas spécifié un séparateur de caractères.";
$fbFilename = "Nom de fichier";
$contacts_import_title = "Importer un carnet d'adresses";
$contacts_import_success = "Le fichier a été importé avec succès";
$contacts_import_double = "Vous avez sélectionné une ou plusieurs colonnes en double. Corrigez le et recommencez.";
//changed:
$contacts_import_noname = 'Vous devez choisir le champs  \'Nom\' and \'Prénom\' ';
$contacts_import_incompatible = "Format de fichier incompatible.Un fichier CSV est un fichier dont les valeurs sont séparées par des virgules.";
$strMember = "Membre";
$contact_exist_warning = "Note: Cet utilisateur est déjà dans votre carnet d'adresses.";
$contacts_add_to_group = "Ajouter les utilisateurs sélectionnés dans le groupe de contacts...";
$contacts_add_to = "Ajouter les utilisateurs sélectionnés dans les contacts";
$contacts_add_user = "Ajouter";
$contacts_edit = "Modifier";
$contacts_no_select = "Vous n'avez rien sélectionné";
$contacts_this_contact = "ce contact";

$contacts_seperator_text = 'Valeurs séparées par';
$contacts_quote = 'Valeurs entourées par';
$contacts_clrf = 'Lignes terminées par';
$contacts_auto_group = 'Créer automatiquement des groupes depuis le fichier';
$contacts_import_to_group = 'Importer dans le groupe';

$contacts_search = 'Recherche';
$cmdSearch = 'Recherche';
$contacts_search_text = "Entrez votre recherche et cliquez sur 'recherche'. Vous pouvez utiliser le caractère jocker *";
$contacts_results = 'résultat(s)';
$contacts_result = 'résultat(s)';


$contacts_color = 'Couleur';

$contacts_colors['black'] = 'Noir';
$contacts_colors['red'] = 'Rouge';
$contacts_colors['green'] = 'Vert';
$contacts_colors['blue'] = 'Bleu';
$contacts_colors['pink'] = 'Rose';
$contacts_colors['orange'] = 'Orange';
$contacts_colors['brown'] = 'Brun';

$ab_addressbooks = 'Carnet d\'adresses';
$ab_owner = 'Propriétaire';
$ab_subscribed = 'Souscrit';
$ab_new_ab = 'Nouveau carnet d\'adresses';
$ab_addressbook_exists = 'Un carnet d\'adresses avec un nom identique existe déjà';
$ab_addressbook = 'Carnet d\'adresses';
$contacts_select_single = 'Cliquez sur un contact pour sélectionner cette personne';

$ab_group_exists = 'Il y a déjà un carnet d\'adresses avec ce nom';

//translation required
$ab_edit_contact = 'Cliquez sur \'Ok\' pour enregistrer les changements. Les champs marqués d\'une étoile sont obligatoires';
$ab_group_on_file = 'Grouper les contact en se basant sur le fichier';
$ab_default = 'Défaut';
$ab_browse = 'Parcourir';
$ab_comment = 'Commentaire';

$ab_contact = 'Contact';
$ab_send_message = 'E-mail';
$ab_new_event = 'Evènement';

$ab_templates = 'Templates';
$ab_your_templates = 'le votre';
$ab_all_templates = 'tous';

$template_type[0] = 'E-mail';
$template_type[1] = 'OpenOffice.Org';
$ab_type = 'Type';
$ab_template_exists = 'Un template avec ce nom existe déjà.';

$sir_madam['M'] = 'monsieur';
$sir_madam['F'] = 'madame';

$ab_upload_oo = 'Charger le template de OpenOffice.org';
$ab_upload_oo_text = 'Choisissez le templace que vous avez édité avec OpenOffice.org et cliquez sur \'Ok\'';
$ab_upload_nofile = 'Erreur pendant l\'envoi du fichier.';

$ab_oo_doc = 'Document texte';
$ab_select_template = 'Choisissez un template pour ce document:';
$ab_add_field_data = 'Vous pouvez utiliser les valeurs suivantes dans votre document :';
$ab_datafield = 'Champs';

$ab_no_template = 'Pas de template';
$ab_new_note = 'Note';

$ab_events = 'Rendez-vous';

$ab_new_contact = 'Nouveau contact';
$ab_new_company = 'Nouvelle société';
$ab_companies = 'Sociétés';
$ab_company_properties = 'Profil société';
$ab_bank_no = 'Numéro de Banque';
$ab_companies_count_single = 'société';
$ab_companies_count = 'sociétés';
$ab_no_companies = 'Aucunes sociétés trouvées';
$ab_no_company = 'Aucune société';

$ab_import_type = 'Importer';
$ab_no_company_found = 'Aucunes sociétés trouvées';
$ab_search_company = 'Consultation société';
$ab_edit_company = 'Changer le profil de la société';
$ab_new_group = 'Nouveau groupe';
$ab_export_type = 'Exporter';

$ab_custom_fields = 'Champs suppléméntaires';
$ab_field_size = 'Taille';

$ab_new_field = 'Nouveau champ';
$ab_invalid_field_size = 'Taille invalide. Choisissez une valeur comprise entre 10 et 255.';
$ab_field_exists = 'Un champ avec le même nom existe déjà.';

$ab_employees = 'Employés';
$ab_no_employees = 'Aucun Employé';

$ab_employee_count_single = 'employé';
$ab_employee_count_multiple = 'employés';
$ab_vat_no = 'Numéro de VAT';

$ab_add_new = 'Ajouter un nouveau';
$ab_add_existing = 'Add existing';

$ab_access = 'Accès';
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

$ab_catagory_exists = 'Une catégorie avec le même nom existe déjà.';
$ab_add_catagory = 'Ajouter une catégorie';
$ab_add_field = 'Ajouter un champ';
$ab_catagory = 'Catégorie';
?>
