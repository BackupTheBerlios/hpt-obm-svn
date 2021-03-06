<?php
require($GO_LANGUAGE->get_fallback_base_language_file('common'));

$charset = 'ISO-8859-1';

//main language menu file register login and other non logged in pages
//common
$cmdLogin = "Login";
$cmdSave = "Salvar";
$cmdBack = "Voltar";
$cmdCancel = "Cancelar";
$cmdClose = "Fechar";
$cmdOk = "OK";
$cmdDelete = "Excluir";
$cmdNext = "Pr�ximo";
$cmdPrevious = "Anterior";
$cmdContinue = "Continuar";
$cmdUpdate = "Atualizar";
$cmdApply = "Aplicar";
$cmdYes = "Sim";
$cmdNo = "N�o";
$cmdAdd = 'Adicionar';
$cmdEdit = 'Editar';
$cmdInvertSelection = 'Inverter a sele��o';

$strEdit = "Editar";
$strThis = 'Isto';
$strItems = 'Items';
$strDescription = 'Descri��o';

$strName = "Nome";
$strUser = "Usu�rio";
$strUsername = "Nome do Usu�rio";
$strPassword = "Senha";
$strAuthcode = "C�digo de autoriza��o";
$strCountry = "Pa�s";
$strState = "Estado";
$strCity = "Cidade";
$strZip = "CEP";
$strAddress = "Endere�o";
$strEmail = "E-mail";
$strPhone = "Telefone";
$strWorkphone = "Telefone Comercial";
$strCellular = "Telefone Celular";
$strCompany = "Empresa";
$strDepartment = "Departamento";
$strFunction = "Cargo";
$strQuestion = "Pergunta Secreta";
$strAnswer = "Resposta";
$strFax = "Fax";
$strWorkFax = "Fax Comercial";
$strHomepage = "Homepage";
$strWorkAddress = "Endere�o Comercial";
$strWorkZip = "CEP Comercial";
$strWorkCountry = "Pa�s Comercial";
$strWorkState = "Estado Comercial";
$strWorkCity = "Cidade Comercial";
$strPhoto = "Foto";
$strComments = "Coment�rios";

$strProperties = 'Propriedades';

$strDeleteItem = "Excluir";

$strDate = "Data";

$strSaveSuccess = "Os dados foram salvos com sucesso";
$strSaveError = "Um erro ocorreu ao tentar salvar os dados. Por favor tente outra vez.";
$strDataError = "Um erro ocorreu ao ler dados. Por favor tente atualizar esta p�gina.";
$strDeleteError = "N�o foi poss�vel excluir estes dados.";
$strAclError = "Erro de controle de acesso";
$strDateError = "Voc� inseriu uma data inv�lida";



$strAccessDenied = "Acesso Negado";
$AccessDenied_text = "N�o � permitido voc� ler esta sess�o do local ou executar esta a��o.<br/>Um outro usu�rio restringiu o acesso � informa��o que voc� pediu.";
$invalid_chars = "Os seguintes caracteres n�o s�o permitidos";

$strGroups = "Grupos";
$strOwner = "Propriet�rio";

$strDeletePrefix = "Tem certeza que deseja excluir ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "Sair";
$menu_applications = "Aplica��es";
$menu_help = "Ajuda";
$menu_about = "Sobre";
$menu_documentation = "Documenta��o";
$menu_configuration = "Configura��o";
$menu_groups = "Grupos de Usu�rios";
$menu_accounts = "Sua Conta";
$menu_preferences = "Prefer�ncias";
$menu_bookmarks = "Favoritos";
$menu_bookmarks_manage = "Organizar meus Favoritos";
$menu_add_bookmark = "Adicionar aos Favoritos";
$menu_add_current = "Adicionar est� p�gina aos Favoritos";

$menu_invite = "Convidar outros usu�rios";

$menu_search_discussions = "Discuss�es";
$menu_search_files = "Arquivos e Pastas";
$menu_search_web = "Na Web";
$menu_scheduler = "Calend�rio";
$menu_filetypes = "Tipos de Arquivos";
$menu_discussions = "Discuss�o";

$menu_users = "Usu�rios";
$menu_start = "P�gina Inicial";
$menu_reboot = "Reiniciar o Servidor";
$menu_halt = "Parar o Servidor";
$menu_reboot_text = "AVISO! Todos os servi�os do Group-Office estar�o temporariamente indispon�veis. Tem certeza que quer continuar?";
$menu_halt_text = "AVISO! Todos os servi�os do Group-Office ser�o terminados. Tem certeza que quer continuar?";

$menu_new = "Nova";
$menu_message = "Mensagem";
$menu_contact = "Contato";
$menu_event = "Evento";
$menu_bookmark = "Favoritos";
$menu_topics = "T�picos";
$menu_support = "Suporte";
$menu_projects = "Projetos";
$menu_sysinfo = "Informa��o sobre o sistema";
$menu_admin = 'Administrador';
$menu_modules = 'M�dulos';
$menu_manual = 'Manual';

//class validate
$error_required = "Voc� n�o especificou o campo requerido abaixo.";
$error_match_auth = "Os c�digos de autoriza��o n�o combinaram.";
$error_match_pass = "As senhas n�o combinaram.";
$error_min_length = "Caracteres m�nimos do campo abaixo:";
$error_max_length = "Caracteres m�ximos do campo abaixo:";
$error_email = "O endere�o de E-mail que voc� especificou � innv�lido."; 
$error_email_exists = "O endere�o de E-mail que voc� especificou j� existe.";
$error_username_exists = "Desculpe, mas o nome do usu�rio que voc� introduziu j� existe. Por favor escolha outro.";
$errors_in_form = "Existem erros no formul�rio. Corrija-os e tente outra vez.";
$error_missing_field = "Voc� n�o preencheu todos os campos requeridos.";

//acl control
$acl_control_auth_groups = "Grupos autorizados";
$acl_control_auth_users = "Usu�rios autorizados";
$acl_control_no_contacts = "Voc� n�o tem contatos cadastrados.<br /> Use o Livro de Endere�os para adicionar membros aos seus contatos."; 

$strShowProfile = "Veja o perfil";
$strShowGroup = "Veja os membros dos grupos";
$strProcessingTime = "Tempo para abrir esta p�gina ";
$strLoggedInAs = "Logado como ";
$strAt = " em ";
$strEmailTo = "Envie uma mensagem para: ";
$strProtectedUser = "Usu�rios Protegidos";

$http_not_found = "O Arquivo n�o foi encontrado";
$http_not_found_text = "O Arquivo que voc� est� pesquisando n�o foi encontrado. Tente talvez corrigir o URL.";
$http_error = "Erro Interno do Servidor";
$http_bad_request = "M� requisi��o";
$strAddContact = "Adicionar aos contatos";

$strReadRights = "Permiss�o para ler";
$strWriteRights = "Permiss�o para escrever";
$strPermissions = 'Permiss�es';

$editor_bold = "Negrito";
$editor_italic = "It�lico";
$editor_underline = "Sublinhado";
$editor_left = "Esquerda";
$editor_right = "Direita";
$editor_center = "Centro";
$editor_ord_list = "Lista Ordenada";
$editor_bul_list = "Lista Em Balas";
$editor_txt_color = "Cor Do Texto";
$editor_back_color = "Cor Do Fundo";
$editor_hyperlink = "Hyperlink";
$editor_image = "Imagem";
$editor_rule = "R�gua horizontal";
$editor_very_small = "Muito Pequeno";
$editor_small = "Pequeno";
$editor_medium = "M�dio";
$editor_large = "Grande";
$editor_larger = "Maior";
$editor_very_large = "Muito Grande";
$editor_strikethrough = "Tachado";
$editor_outdent = "Outdent";
$editor_indent = "Indent";
$editor_superscript = "Sobreescrever";
$editor_subscript = "Subescrever";
$editor_undo = "Desfazer";
$editor_redo = "Refazer";
$editor_copy = "Copiar";
$editor_paste = "Colar";
$editor_cut = "Cortar";
$editor_font = "Font";
$editor_size = "Tamanho";
$editor_heading = "Cabe�alho";
$editor_select = "Selecione";
$editor_mode = "&quot;Lan&ccedil;amento do editor";

$days[0]="Do";
$days[1]="Sg";
$days[2]="Te";
$days[3]="Qa";
$days[4]="Qi";
$days[5]="Se";
$days[6]="Sa";

$months[0]="Janeiro";
$months[1]="Fevereiro";
$months[2]="Mar�o";
$months[3]="Abril";
$months[4]="Maio";
$months[5]="Junho";
$months[6]="Julho";
$months[7]="Agosto";
$months[8]="Setembro";
$months[9]="Outubro";
$months[10]="Novembro";
$months[11]="Dezembro";

$full_days[0] = "Domingo";
$full_days[1] = "Segunda-feira";
$full_days[2] = "Ter�a-feira";
$full_days[3] = "Quarta-feira";
$full_days[4] = "Quinta-feira";
$full_days[5] = "Sexta-feira";
$full_days[6] = "S�bado";
$strLoadTime = 'Gerado dentro de';
$strDescription = 'Descri��o';
$strTitle = 'T�tulo';

$editorCmd['undo'] = "Desfazer";
$editorCmd['redo'] = "Refazer";
$editorCmd['bold'] = "Negrito";
$editorCmd['italic'] = "It�lico";
$editorCmd['underline'] = "Sublinhado";
$editorCmd['left'] = "Alinhar � Esquerda";
$editorCmd['right'] = "Alinhar � Direita";
$editorCmd['full'] = "Justificado";
$editorCmd['center'] = "Centralizado";
$editorCmd['ord_list'] = "Numera��o";
$editorCmd['bul_list'] = "Marcadores";
$editorCmd['txt_color'] = "Cor do Texto";
$editorCmd['back_color'] = "Cor do Fundo";
$editorCmd['hyperlink'] = "Hyperlink";
$editorCmd['image'] = "Inserir imagem";
$editorCmd['rule'] = "R�gua Horizontal";
$editorCmd['table'] = "Inserir tabela";
$editorCmd['strikethrough'] = "Tachado";
$editorCmd['outdent'] = "Outdent";
$editorCmd['indent'] = "Indent";
$editorCmd['superscript'] = "Sobrescrever";
$editorCmd['subscript'] = "Subescrever";
$editorCmd['copy'] = "Copiar";
$editorCmd['paste'] = "Colar";
$editorCmd['cut'] = "Recortar";
$editorCmd['font'] = "Fonte";
$editorCmd['size'] = "Tamanho";
$editorCmd['heading'] = "Cabe�alho";
$editorCmd['select'] = "Selecionar";
$editorCmd['mode'] = "Alterar modo";
$editorCmd['textindicator'] = 'Estilo atual';

$cmdShowAll = 'Mostrar todos';
$cmdSearch = 'Procurar';

//translation required
$cmdYesToAll = 'Sim para todos';
$cmdNoToAll = 'N�o para todos';

$short_months[0]="Jan";
$short_months[1]="Fev";
$short_months[2]="Mar";
$short_months[3]="Abr";
$short_months[4]="Mai";
$short_months[5]="Jun";
$short_months[6]="Jul";
$short_months[7]="Ago";
$short_months[8]="Set";
$short_months[9]="Out";
$short_months[10]="Nov";
$short_months[11]="Dez";

$strToday = 'Hoje';

$strShortWeek = 'sm';

$cal_prev_year = "Ano anterior (pressione para ver o menu)";
$cal_prev_month  = "M�s anterior (pressione para ver o menu)";
$cal_go_today = "Ver Hoje";
$cal_next_month = "Pr�ximo m�s (pressione para ver o menu)";
$cal_next_year  = "Pr�ximo ano (pressione para ver o menu)";
$cal_drag_to_move = "Arraste para mover";
$cal_part_today = " (hoje)";
$cal_monday_first = "Exibir Segunda primeiro";
$cal_sunday_first  = "Exibir Domingo primeiro";
$cal_select_date = "Selecionar data";

$cmdPrint = 'Imprimir';
$strFirstName = 'Primeiro nome';
$strLastName = '�ltimo nome';
$strInitials = 'Iniciais';
$strSex = 'Sexo';
$strBirthday = 'Nascimento';
$strSexes['M'] = 'Masculino';
$strSexes['F'] = 'Feminino';

$strTitle = 'T�tulo';

$strBeginning = 'In�cio';

$strMiddleName = 'Nome do meio';

$sir_madam['M'] = 'Senhor';
$sir_madam['F'] = 'Senhora';

$strModifiedAt = 'Alterar para';
$strCreatedAt = 'Criado em';

$htmlarea_language_name = 'br';

$strLocalization = 'Localiza��o';

$lang_modules['notes'] = "Notas";
//look up the language in controls/jscalendar/lang
$jscalendar_language_name = 'calendar-br.js';

$lang_modules['bookmarks'] = "Favoritos";

//2.01 translations

//look up language in 'classes/phpmailer/language'
$php_mailer_lang = 'br';
$strEmailFail = 'Falha ao enviar e-mail.';

//for people that want to use ktmlarea
$ktml_language = 'english';

$ml_host_unreachable = ' n�o p�de ser localizado. A verifica��o autom�tica de e-mails foi desativada.';
$strRegion = 'Regi�o';

//2.05
$cmdReset = 'Limpar';
$strMyName = 'Meu nome';

//modules
$lang_modules['scheduler'] = 'Agenda';
$lang_modules['calendar'] = "Calend�rio";
$lang_modules['filesystem'] = 'Arquivos';
$lang_modules['email'] = 'E-mail';
$lang_modules['projects'] = 'Projetos';
$lang_modules['search'] = 'Pesquisar';
$lang_modules['contacts'] = "Contatos";
$lang_modules['addressbook'] = "Contatos";
$lang_modules['cms'] = "Websites";
$lang_modules['notes'] = "Anota��es";
$lang_modules['bookmarks'] = "Favoritos";
$lang_modules['summary'] = "Sum�rio";
$lang_modules['todos'] = "Pend�ncias";
$lang_modules['documents'] = "Documentos";
?>