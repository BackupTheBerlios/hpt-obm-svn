<?php
require($GO_LANGUAGE->get_fallback_base_language_file('users'));

$strLogins = "Ingresos";
$strRegistrationDate = "Fecha de Registro";
$strUsers = "usuario(s)";
$user_profile = "Perfil de Usuario";
$ac_login_info = "Informaci&oacute;n de ingreso";
$ac_registration_time = "Tiempo de Sesi&oacute;n";
$ac_lastlogin = "Ultimo ingreso";
$ac_logins = "Total ingresos";
$strMaxOf = "registrado desde el m&aacute;ximo de";
$strUsername = "Usuario";
$delete_fail = "Ud. no puede eliminar al superusuario ni a Ud. mismo.";
$admin_new_user = "Nuevo usuario";

$admin_modules = 'm&oacute;dulos';
$admin_module_access = 'Este usuario tiene acceso a los siguientes m&oacute;dulos:';

$admin_password = 'Clave';
$admin_confirm_password = 'Confirme la clave';
$admin_change_password = 'Cambiar clave';
$admin_pass_match = 'Las claves no coinciden. Trate de nuevo.';

//registration
$registration_personal_info = "Informaci&oacute;n Personal";
$registration_account_info = "Informaci&oacute;n de la Cuenta";
$registartion_username_text = "Ingrese Usuario y clave para ingresar en este sitio.";

$registration_question_text=<<<registration_question_text
Ingrese una pregunta secreta que su respuesta s&oacute;lo la conozca Ud.  Nosotros se la preguntaremos en caso de que olvide su clave.
Cuando la pregunta sea correctamente contestada, se enviar&aacute; un e-mail a su direcci&oacute;n de correo registrada con el usuario y nueva clave para que pueda ingresar.
registration_question_text;

$registration_text=<<<registration_text
Complete los datos de esta forma y haga click en 'Ok' para registrarse.  Los campos marcados con * son requeridos.<br />
La informaci&oacute;n que Ud. digite ser&aacute; utilizada por Group-Office &uacute;nicamente y no se suministrar&aacute; a otras personas.
registration_text;

$registartion_authcode_text = "D&iacute;gite un c&oacute;digo de Autorizaci&oacute;n. El c&oacute;digo de autorizaci&oacute;n es para que otros usuarios lo a&ntilde;adan a la lista de contactos y vean sus detalles.";
$registartion_confirm_authcode = "Confirme c&oacute;digo de autorizaci&oacute;n";
$registration_visibility = "Hacer visible este usuario a todos los dem&aacute;s.";
$registration_success = "<h1>Felicitaciones!</h1>Ud. se ha registrado exitosamente y un mensaje de bienvenida ha sido enviado a:";
$registration_failure = "Un error ha ocurrido mientras se tratraba de registrar su informaci&oacute;n. Por favor trate nuevamente.";
$registration_mail_subject = "Bienvenido a Group-Office.";
$registration_folder_error = "Fall&oacute; creando la carpeta personal. Cont&aacute;ctenos para hacerlo!";
$registration_title = "Reg&iacute;strese en Group-Office";
$registration_mail_body=<<<registration_mail_body
Querido Se&ntilde;or(a) %name%,

Ud. se ha registrado en Group-Office.
Esperamos disfrute nuestros servicios!

Puede ingresar con los siguientes datos:

Usuario : %username%
Password: %password%

Click en siguiente link para ingresar:

registration_mail_body;
$registration_email = "Crea una direcci&oacute;n de correo para este usuario en Group-Office.";
$registration_samba = "Servidor de Archivo & Impresi&oacute;n.";
$registration_email_error = "Ha ocurrido un error mientras se creaba su cuenta de correo. Cont&aacute;ctenos para soporte.";
$strPasswordConfirm = "Confirme clave";
$strPassword = "Clave";

$reg_company_info = "Informaci&oacute;n Institucional";

$max_user_limit = "Cantidad M&aacute;xima de usuarios registrados.";
$max_users_text = "D&iacute;sculpenos, Ud. no puede registrar m&aacute;s usuarios. Cont&aacute;ctenos si desea incrementar su n&uacute;mero de usuarios.";
$ml_sent_items = "Items Enviados";
$ml_spam = "Spam";
$ac_samba_user = "Usuario tiene acceso al servidor de Archivo e Impresi&oacute;n.";
$ml_email_only = "Solamente es requerido cuando no va a crear direcci&oacute;n e-mail";
$register_new_user = 'Nuevo Group-Office suscriptor';
$register_thanks = '<h2>Gracias</h2>. Su solicitud de suscripcion sera procesada lo mas pronto posible. Se le notificara.';
$register_accept = 'Adicionar Usuario';
$register_addressbook = 'Adiciona a Libreta Direcciones';
$register_title = 'Suscribir';
?>
