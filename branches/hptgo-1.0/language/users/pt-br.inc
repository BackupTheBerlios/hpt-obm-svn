<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Logins";
$strRegistrationDate = "Última data de registro";
$strUsers = "Usuário(s)";
$user_profile = "Perfil de Usuário";
$ac_login_info = "Informações de Login";
$ac_registration_time = "Tempo registrado";
$ac_lastlogin = "Último login";
$ac_logins = "Total de logins";
$strMaxOf = "registrado no máximo por";
$strUsername = "Nome de usuário";
$delete_fail = "Você não pode deletar seu usuário ou o Administrador.";
$admin_new_user = "Novo usuário";
$admin_modules = 'Módulos';
$admin_module_access = 'Este usuário tem o acesso aos seguintes módulos:';
$admin_password = 'Senha';
$admin_confirm_password = 'Confirmar senha';
$admin_change_password = 'Mude a senha';
$admin_pass_match = 'As senhas não combinaram. Por favor tente outra vez.';



//registration

$registration_personal_info = "Informação pessoal";
$registration_account_info = "Informações sobre a sua conta";
$registartion_username_text = "Introduza o nome e senha de usuário para acesso a este site.";
$registration_question_text=<<<registration_question_text

Adicione uma pergunta secreta a que somente este usuário sabe a resposta. Nós faremos esta pergunta ao usuário caso ele se esqueça de sua senha.

Quando a resposta correta é dada, um E-mail será emitido ao endereço registado com o nome de usuário e uma senha nova para poder iniciar uma sessão.

registration_question_text;
$registration_text=<<<registration_text

Complete este formulário e clique no 'Ok' para registar. Os campos marcados com o * são obrigatórios.<br />

As informações que você fornece são somente para o uso no Group-Office, e assim nós não as forneceremos a outros.

registration_text;
$registartion_authcode_text = "Adicione um código de autorização. O código de autorização é um código que outros usuários necessitam para adicionar este usuário em suas listas de contato e ver seus detalhes.";
$registartion_confirm_authcode = "Confirme o código de autorização ";
$registration_visibility = "Torne este usuário visível a todos os outros usuários.";
$registration_success = "<h1>Sucesso!</h1>Você foi registado com sucesso e uma mensagem de boas vindas foi encaminhada a:";
$registration_failure = "Um erro ocorreu ao tentar registar as suas informações. Por favor tente outra vez.";
$registration_mail_subject = "Bem-vindo ao Group-Office.";
$registration_folder_error = "Criação da pasta pessoal falhou. Contate-nos para restaurar isto!";
$registration_title = "Registro do Group-Office";
$registration_email = "Crie um endereço de E-mail para este usuário no Group-Office.";
$registration_samba = "Servidor de arquivos e impressão.";
$registration_email_error = "Um erro ocorreu ao criar a sua conta de E-mail. Contate-nos para suporte.";
$strPasswordConfirm = "Confirme a senha";
$strPassword = "Senha";
$reg_company_info = "Informação da Empresa";
$max_user_limit = "Uma quantidade máxima de usuários foi alcançada.";
$max_users_text = "Desculpe, você não pode registar mais usuários. Contate-nos se você necessitar aumentar o número de usuários.";
$ml_sent_items = "Itens enviados";
$ml_spam = "Spam";
$ac_samba_user = "Usuário tem acesso ao servidor de arquivos e impressão.";
$ml_email_only = "Requerido somente quando você não criar um endereço de Correio Eletrônico";
$register_new_user = 'Novo usuário do Group-Office';
$register_thanks = 'Obrigado. Sua inscrição será processada o mais breve possível. Você  receberá noticias em breve.';
$register_accept = 'Adicionar aos usuários';
$register_addressbook = 'Adicinoar ao Contatos';
$register_title = 'Assinar';

//modified:

$registration_mail_body=<<<registration_mail_body

Prezado(a),

Você foi cadastrado no Group-Office. Esperamos que você aprecie este serviço!

Dados para acesso:

Usuário: %username%
Senha: %password%

registration_mail_body;

//1.98
$reg_language = 'Idioma';
$reg_theme = "Tema";

//2.05
$admin_use = 'Usar';
$admin_module = 'Módulo';
$admin_manage = 'Gerenciar';

$admin_groups = 'Grupos de usuários';
$admin_groups_user = 'O usuário é membro dos seguintes grupos';
?>
