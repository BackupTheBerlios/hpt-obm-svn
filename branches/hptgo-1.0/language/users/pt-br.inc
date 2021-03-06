<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Logins";
$strRegistrationDate = "�ltima data de registro";
$strUsers = "Usu�rio(s)";
$user_profile = "Perfil de Usu�rio";
$ac_login_info = "Informa��es de Login";
$ac_registration_time = "Tempo registrado";
$ac_lastlogin = "�ltimo login";
$ac_logins = "Total de logins";
$strMaxOf = "registrado no m�ximo por";
$strUsername = "Nome de usu�rio";
$delete_fail = "Voc� n�o pode deletar seu usu�rio ou o Administrador.";
$admin_new_user = "Novo usu�rio";
$admin_modules = 'M�dulos';
$admin_module_access = 'Este usu�rio tem o acesso aos seguintes m�dulos:';
$admin_password = 'Senha';
$admin_confirm_password = 'Confirmar senha';
$admin_change_password = 'Mude a senha';
$admin_pass_match = 'As senhas n�o combinaram. Por favor tente outra vez.';



//registration

$registration_personal_info = "Informa��o pessoal";
$registration_account_info = "Informa��es sobre a sua conta";
$registartion_username_text = "Introduza o nome e senha de usu�rio para acesso a este site.";
$registration_question_text=<<<registration_question_text

Adicione uma pergunta secreta a que somente este usu�rio sabe a resposta. N�s faremos esta pergunta ao usu�rio caso ele se esque�a de sua senha.

Quando a resposta correta � dada, um E-mail ser� emitido ao endere�o registado com o nome de usu�rio e uma senha nova para poder iniciar uma sess�o.

registration_question_text;
$registration_text=<<<registration_text

Complete este formul�rio e clique no 'Ok' para registar. Os campos marcados com o * s�o obrigat�rios.<br />

As informa��es que voc� fornece s�o somente para o uso no Group-Office, e assim n�s n�o as forneceremos a outros.

registration_text;
$registartion_authcode_text = "Adicione um c�digo de autoriza��o. O c�digo de autoriza��o � um c�digo que outros usu�rios necessitam para adicionar este usu�rio em suas listas de contato e ver seus detalhes.";
$registartion_confirm_authcode = "Confirme o c�digo de autoriza��o ";
$registration_visibility = "Torne este usu�rio vis�vel a todos os outros usu�rios.";
$registration_success = "<h1>Sucesso!</h1>Voc� foi registado com sucesso e uma mensagem de boas vindas foi encaminhada a:";
$registration_failure = "Um erro ocorreu ao tentar registar as suas informa��es. Por favor tente outra vez.";
$registration_mail_subject = "Bem-vindo ao Group-Office.";
$registration_folder_error = "Cria��o da pasta pessoal falhou. Contate-nos para restaurar isto!";
$registration_title = "Registro do Group-Office";
$registration_email = "Crie um endere�o de E-mail para este usu�rio no Group-Office.";
$registration_samba = "Servidor de arquivos e impress�o.";
$registration_email_error = "Um erro ocorreu ao criar a sua conta de E-mail. Contate-nos para suporte.";
$strPasswordConfirm = "Confirme a senha";
$strPassword = "Senha";
$reg_company_info = "Informa��o da Empresa";
$max_user_limit = "Uma quantidade m�xima de usu�rios foi alcan�ada.";
$max_users_text = "Desculpe, voc� n�o pode registar mais usu�rios. Contate-nos se voc� necessitar aumentar o n�mero de usu�rios.";
$ml_sent_items = "Itens enviados";
$ml_spam = "Spam";
$ac_samba_user = "Usu�rio tem acesso ao servidor de arquivos e impress�o.";
$ml_email_only = "Requerido somente quando voc� n�o criar um endere�o de Correio Eletr�nico";
$register_new_user = 'Novo usu�rio do Group-Office';
$register_thanks = 'Obrigado. Sua inscri��o ser� processada o mais breve poss�vel. Voc�  receber� noticias em breve.';
$register_accept = 'Adicionar aos usu�rios';
$register_addressbook = 'Adicinoar ao Contatos';
$register_title = 'Assinar';

//modified:

$registration_mail_body=<<<registration_mail_body

Prezado(a),

Voc� foi cadastrado no Group-Office. Esperamos que voc� aprecie este servi�o!

Dados para acesso:

Usu�rio: %username%
Senha: %password%

registration_mail_body;

//1.98
$reg_language = 'Idioma';
$reg_theme = "Tema";

//2.05
$admin_use = 'Usar';
$admin_module = 'M�dulo';
$admin_manage = 'Gerenciar';

$admin_groups = 'Grupos de usu�rios';
$admin_groups_user = 'O usu�rio � membro dos seguintes grupos';
?>
