<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Connexions";
$strRegistrationDate = "Date d'enregistrement";
$strUsers = "utilisateur(s)";
$user_profile = "Profil utilisateur";
$ac_login_info = "Information de connexion";
$ac_registration_time = "Heure d'enregistrement";
$ac_lastlogin = "Derni�re connexion";
$ac_logins = "Nombre de connexions";
$strMaxOf = "enregistr� depuis le maximum de";
$strUsername = "Nom d'utilisateur";
$delete_fail = "Vous ne pouvez pas vous supprimer vous m�me ainsi que le super utilisateur.";
$admin_new_user = "Nouvel utlisateur";

$admin_modules = 'Modules';
$admin_module_access = 'Cet utilisateur a acc�d� aux modules suivants :';

$admin_password = 'Mot de passe';
$admin_confirm_password = 'Confirmez le mot de passe';
$admin_change_password = 'Changer le mot de passe';
$admin_pass_match = 'Les mots de passe ne correspondent pas. R�essayez.';

//registration
$registration_personal_info = "Information personnelle";
$registration_account_info = "Information du compte";
$registartion_username_text = "Entrez un nom d'utilsateur et un mot de passe pour que cet utilisateur puisse se connecter � ce site.";

$registration_question_text=<<<registration_question_text
Entrez une question secr�te pour laquelle seul cet utilisateur pourra r�pondre. Nous poserons cette question dans le cas o� l'utilisateur a oubli� son mot de passe.
Lorsque la r�ponse correcte est donn�e, un email sera envoy� � l'adresse mail enregistr�e avec le nom d'utilisateur et un nouveau mot de passe pour se connecter.
registration_question_text;

$registration_text=<<<registration_text
Remplissez ce formulaire et cliquez sur 'Ok' pour vous enregistrer. Les champs marqu�s d'une * sont n�cessaires.<br />
Les informations fournies ne sont utilis�e que pour Group-Office, nous ne les fournirons pas � d'autre personnes.
registration_text;

$registartion_authcode_text = "Entrez un code d'autorisation. Le code d'autorisation est un code que les autres utilisateurs devront conna�tre pour ajouter cet utilisateur � leur liste de contacts ainsi que pour voir vos informations d�taill�es.";
$registartion_confirm_authcode = "Confirmez le code d'autorisation";
$registration_visibility = "Rendre cet utilisateur visible � tous les utilisateurs.";
$registration_success = "<h1>F�licitations !</h1>Votre enregistrement s'est d�roul� avec succ�s et un message de bienvenue vous a �t� envoy� � :";
$registration_failure = "Une erreur est apparue pendant l'enregistrement de vos informations. Merci de r�essayer.";
$registration_mail_subject = "Bienvenue � Group-Office.";
$registration_folder_error = "Impossible de cr�er le dossier personnel. Contactez nous pour le restorer !";
$registration_title = "Enregistr� � Group-Office";

$registration_email = "Cr�er une adresse mail pour cet utilisateur au Group-Office.";
$registration_samba = "Fichier & Serveur d'impression.";
$registration_email_error = "Une erreur est apparue pendant la cr�ation de votre compte. Contactez nous.";
$strPasswordConfirm = "Confirmez votre mot de passe";
$strPassword = "Mot de passe";

$reg_company_info = "Informations sur la soci�t�";

$max_user_limit = "Le nombre maximum d'utilisateurs a �t� atteint.";
$max_users_text = "D�sol�, vous ne pouvez pas cr�er plus d'utilisateur. Contactez nous si vous souhaitez augmenter votre nombre d'utilisateurs.";
$ml_sent_items = "El�ments envoy�s";
$ml_spam = "Spam";
$ac_samba_user = "L'utlisateur a acc�d� � ce fichier et au serveur d'impression.";
$ml_email_only = "Seulement n�cessaire lorsque vous ne voulez pas cr�er une adresse de mail";
$register_new_user = 'Nouvelle inscription � Group-Office';
$register_thanks = '<h2>Merci</h2>. Votre inscription sera trait�e d�s que possible. Vous recevrez plus d\'informations.';
$register_accept = 'Ajouter aux utilisateurs';
$register_addressbook = 'Ajouter au carnet d\'adresses';
$register_title = 'Souscrire';
$registration_mail_body=<<<registration_mail_body
Cher(e) %last_name%,

Vous �tes enregistr� sur Group-Office.
Nous esp�rons que vous appr�cierez nos services.

Vous pouvez vous connecter avec les donn�es suivantes :

Utilisateur  : %username%
Mot de passe : %password%

Cliquez sur le lien ci-dessous pour vous connecter :

registration_mail_body;

//1.98
$reg_language = 'Langage';
$reg_theme = 'Th�me';

//2.05
$admin_use = 'Utiliser';
$admin_module = 'Module';
$admin_manage = 'G�rer';

$admin_groups = 'Goupes d'utilisateur ';
$admin_groups_user = 'L'utilisateur est membre des groupes d'utilisateur suivants';
?>
