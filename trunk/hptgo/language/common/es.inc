<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';
$lang_modules['scheduler'] = 'agenda';
$lang_modules['filesystem'] = 'archivos';
$lang_modules['email'] = 'e-mail';
$lang_modules['projects'] = 'proyectos';
$lang_modules['search'] = 'buscar';
$lang_modules['contacts'] = "directorio";

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "inicio sesi&oacute;n";
$cmdSave = "grabar";
$cmdBack = "anterior";
$cmdCancel = "cancelar";
$cmdClose = "cerrar";
$cmdOk = "ok";
$cmdDelete = "borrar";
$cmdNext = "siguiente";
$cmdPrevious = "anterior";
$cmdContinue = "continuar";
$cmdUpdate = "sincronizar";
$cmdApply = "aplicar";
$cmdYes = "si";
$cmdNo = "no";
$cmdAdd = 'adicionar';
$cmdEdit = 'editar';
$cmdInvertSelection = 'invertir selecci&oacute;n';

$strEdit = "Editar";
$strThis = 'Este';
$strItems = 'items';
$strDescription = 'Descripci&oacute;n';

$strName = "Nombre";
$strUser = "Usuario";
$strUsername = "Usuario";
$strPassword = "Clave";
$strAuthcode = "C&oacute;digo Autorizaci&oacute;n";
$strCountry = "Pa&iacute;s";
$strState = "Estado";
$strCity = "Ciudad";
$strZip = "C&oacute;digo Postal";
$strAddress = "Direcci&oacute;n";
$strEmail = "E-mail";
$strPhone = "Telefono";
$strWorkphone = "Telefono Trabajo";
$strCellular = "Celular";
$strCompany = "Compa&ntilde;ia";
$strDepartment = "Oficina";
$strFunction = "Cargo";
$strQuestion = "Pregunta Secreta";
$strAnswer = "Respuesta";
$strFax = "Fax";
$strWorkFax = "Fax Trabajo";
$strHomepage = "Extensi&oacute;n Personal";
$strWorkAddress = "Direcci&oacute;n Trabajo";
$strWorkZip = "C&oacute;digo Postal Trabajo";
$strWorkCountry = "Pais Trabajo";
$strWorkState = "Estado Trabajo";
$strWorkCity = "Ciudad Trabajo";
$strPhoto = "Foto";
$strComments = "Comentarios";

$strProperties = 'propiedades';

$strDeleteItem = "Borrar";

$strDate = "Fecha";

$strSaveSuccess = "Datos grabados exitosamente";
$strSaveError = "Ha ocurrido un error mientras se grababan sus datos. Por favor trate nuevamente.";
$strDataError = "Ha ocurrido un error mientras se leian sus datos. Por favor recargue esta p&aacute;gina.";
$strDeleteError = "No se pueden borrar los datos.";
$strAclError = "Error de control de Acceso";
$strDateError = "Ud. ha entrado una fecha errada";



$strAccessDenied = "Acceso denegado";
$AccessDenied_text = "Ud. no tiene permitido ver esta secci&oacute;n o desarrollar esta acci&oacute;n.<br />Otro usuario tiene restringida la informaci&oacute;n que requiere.";

$invalid_chars = "Los siguientes caracteres no son permitidos.";

$strGroups = "Grupos";
$strOwner = "Propietarios";

$strDeletePrefix = "Est&aacute; seguro que desea borrar ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "salir";
$menu_applications = "aplicaciones";
$menu_help = "ayuda";
$menu_about = "acerca de";
$menu_documentation = "documentaci&oacute;n";
$menu_configuration = "configuraci&oacute;n";
$menu_groups = "usuario&nbsp;grupos";
$menu_accounts = "su&nbsp;cuenta";
$menu_preferences = "preferencias";
$menu_bookmarks = "favoritos";
$menu_bookmarks_manage = "modificar&nbsp;favoritos";
$menu_add_bookmark = "adicionar&nbsp;favoritos";
$menu_add_current = "adicionar&nbsp;p&aacute;gina&nbsp;actual";

$menu_invite = "usuarios&nbsp;invitados";

$menu_search_discussions = "discusiones";
$menu_search_files = "archivos&nbsp;y&nbsp;carpetas";
$menu_search_web = "la&nbsp;web";
$menu_scheduler = "agenda";
$menu_filetypes = "tipos archivo";
$menu_discussions = "tablero&nbsp;discusi&oacute;n";

$menu_users = "usuarios";
$menu_start = "p&aacute;gina&nbsp;inicio";
$menu_reboot = "reiniciar&nbsp;servidor";
$menu_halt = "apagar&nbsp;servidor";
$menu_reboot_text = "CUIDADO! Todos los servicios de Group-Office no estar&aacute;n disponibles temporalmente. Est&aacute; seguro que desa continuar?";
$menu_halt_text = "CUIDADO! Todos los servicios de Group-Office han terminado. Est&aacute; seguro que desea continuar?";
$menu_new = "nuevo";
$menu_message = "mensaje";
$menu_contact = "contacto";
$menu_event = "evento";
$menu_bookmark = "favorito";
$menu_topics = "t&oacute;picos";
$menu_support = "soporte";
$menu_projects = "proyectos";
$menu_sysinfo = "informaci&oacute;n del sistema";
$menu_admin = 'administrador';
$menu_modules = 'm&oacute;dulos';
$menu_manual = 'manual';

//class validate
$error_required = "No ha especificado alg&uacute;n campo requerido.";
$error_match_auth = "El c&oacute;digo de autorizaci&oacute;n no coincide.";
$error_match_pass = "Las claves no coinciden.";
$error_min_length = "M&iacute;nimo numero de caracteres del campo:";
$error_max_length = "M&aacute;ximo numero de caracteres del campo:";
$error_email = "La direcci&oacute;n de e-mail especificada no es v&aacute;lida.";
$error_email_exists = "La direcci&oacute;n de e-mail digitada ya existe.";
$error_username_exists = "Disculpe, el nombre de usuario ya existe. Por favor escoja otro.";
$errors_in_form = "Existen errores en la forma. Corr&iacute;jalos y trate de nuevo.";
$error_missing_field = "No se han diligenciado todos los campos requeridos.";

//acl control
$acl_control_auth_groups = "Grupos autorizados";
$acl_control_auth_users = "Usuarios Autorizados";
$acl_control_no_contacts = "Ud. no tiene miembros en su lista de contactos.<br /> Use el Direcctorio para adicionar miembros a su lista de contactos.";

$strShowProfile = "Ver perfil";
$strShowGroup = "Ver miembros del grupo";
$strProcessingTime = "El procesamiento de esta p&aacute;gina tomo ";
$strLoggedInAs = "Sesi&oacute;n abierta como: ";
$strAt = " en ";
$strEmailTo = "Enviar un mensaje de e-mail a: ";
$strProtectedUser = "Usuario Protegido";

$http_not_found = "Documento no encontrado";
$http_not_found_text = "El documento requerido no fue encontrado. Puede ser un URL perdido.";
$http_error = "Error Interno del Servidor";
$http_bad_request = "Mal requerimiento";
$strAddContact = "A&ntilde;adir al directorio";

$strReadRights = "Permisos de Lectura";
$strWriteRights = "Permisos de Escritura";
$strPermissions = 'permisos';

$editor_bold = "Negrilla";
$editor_italic = "Italica";
$editor_underline = "Subrayado";
$editor_left = "Izquierda";
$editor_right = "Derecha";
$editor_center = "Centrado";
$editor_ord_list = "Lista Ordenada";
$editor_bul_list = "Lista";
$editor_txt_color = "Color Texto";
$editor_back_color = "Color Fondo";
$editor_hyperlink = "Hyperlink";
$editor_image = "Imagen";
$editor_rule = "Regla Horizontal";

$editor_very_small = "Diminuto";
$editor_small = "Peque&ntilde;o";
$editor_medium = "Medio";
$editor_large = "Grande";
$editor_larger = "Muy Grande";
$editor_very_large = "Enorme";

$editor_strikethrough = "Strike Through";
$editor_outdent = "Outdent";
$editor_indent = "Indent";

$editor_superscript = "Superindice";
$editor_subscript = "Subindice";
$editor_undo = "Deshacer";
$editor_redo = "Rehacer";
$editor_copy = "Copiar";
$editor_paste = "Pegar";
$editor_cut = "Cortar";

$editor_font = "Fuente";
$editor_size = "Tama&ntilde;o";
$editor_heading = "Encabezado";
$editor_select = "Seleccionar";

$editor_mode = "Cambiar modo de edicion";

$days[0]="Do";
$days[1]="Lu";
$days[2]="Ma";
$days[3]="Mi";
$days[4]="Ju";
$days[5]="Vi";
$days[6]="Sa";

$months[0]="Enero";
$months[1]="Febrero";
$months[2]="Marzo";
$months[3]="Abril";
$months[4]="Mayo";
$months[5]="Junio";
$months[6]="Julio";
$months[7]="Agosto";
$months[8]="Septiembre";
$months[9]="Octubre";
$months[10]="Noviembre";
$months[11]="Diciembre";

$full_days[0] = "Domingo";
$full_days[1] = "Lunes";
$full_days[2] = "Martes";
$full_days[3] = "Miercoles";
$full_days[4] = "Jueves";
$full_days[5] = "Viernes";
$full_days[6] = "S&aacute;bado";
$strLoadTime = 'Generado en';
$strDescription = 'Descripci&oacute;n';
$strTitle = 'T&iacute;tulo';
$lang_modules['cms'] = "sitios web";

$editorCmd['undo'] = "Deshacer";
$editorCmd['redo'] = "Repetir";
$editorCmd['bold'] = "Negrilla";
$editorCmd['italic'] = "Italica";
$editorCmd['underline'] = "Subrayado";
$editorCmd['left'] = "Izquierda";
$editorCmd['right'] = "Derecha";
$editorCmd['full'] = "Justificar";
$editorCmd['center'] = "Centrar";
$editorCmd['ord_list'] = "Lista Numerada";
$editorCmd['bul_list'] = "Viñetas";
$editorCmd['txt_color'] = "Color Texto";
$editorCmd['back_color'] = "Color Fondo";
$editorCmd['hyperlink'] = "Hyperlink";
$editorCmd['image'] = "Insertar imagen";
$editorCmd['rule'] = "Linea Horizontal";
$editorCmd['table'] = "Insertar tabla";



$editorCmd['strikethrough'] = "Strikethrough";
$editorCmd['outdent'] = "Outdent";
$editorCmd['indent'] = "Indentar";

$editorCmd['superscript'] = "Superscript";
$editorCmd['subscript'] = "Subscript";
$editorCmd['undo'] = "Deshacer";
$editorCmd['redo'] = "Repetir";
$editorCmd['copy'] = "Copiar";
$editorCmd['paste'] = "Pegar";
$editorCmd['cut'] = "Cortar";

$editorCmd['font'] = "Fuente";
$editorCmd['size'] = "Tamano";
$editorCmd['heading'] = "Encabezado";
$editorCmd['select'] = "Seleccionar";
$editorCmd['mode'] = "Cambiar modo";
$editorCmd['textindicator'] = "Estilo Actual";
$lang_modules['addressbook'] = "Libreta Direcciones";
$cmdShowAll = 'Mostrar Todo';
$cmdSearch = 'Buscar';
?>
