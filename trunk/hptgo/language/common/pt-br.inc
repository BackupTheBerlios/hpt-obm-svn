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
$cmdNext = "Próximo";
$cmdPrevious = "Anterior";
$cmdContinue = "Continuar";
$cmdUpdate = "Atualizar";
$cmdApply = "Aplicar";
$cmdYes = "Sim";
$cmdNo = "Não";
$cmdAdd = 'Adicionar';
$cmdEdit = 'Editar';
$cmdInvertSelection = 'Inverter a seleção';

$strEdit = "Editar";
$strThis = 'Isto';
$strItems = 'Items';
$strDescription = 'Descrição';

$strName = "Nome";
$strUser = "Usuário";
$strUsername = "Nome do Usuário";
$strPassword = "Senha";
$strAuthcode = "Código de autorização";
$strCountry = "País";
$strState = "Estado";
$strCity = "Cidade";
$strZip = "CEP";
$strAddress = "Endereço";
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
$strWorkAddress = "Endereço Comercial";
$strWorkZip = "CEP Comercial";
$strWorkCountry = "País Comercial";
$strWorkState = "Estado Comercial";
$strWorkCity = "Cidade Comercial";
$strPhoto = "Foto";
$strComments = "Comentários";

$strProperties = 'Propriedades';

$strDeleteItem = "Excluir";

$strDate = "Data";

$strSaveSuccess = "Os dados foram salvos com sucesso";
$strSaveError = "Um erro ocorreu ao tentar salvar os dados. Por favor tente outra vez.";
$strDataError = "Um erro ocorreu ao ler dados. Por favor tente atualizar esta página.";
$strDeleteError = "Não foi possível excluir estes dados.";
$strAclError = "Erro de controle de acesso";
$strDateError = "Você inseriu uma data inválida";



$strAccessDenied = "Acesso Negado";
$AccessDenied_text = "Não é permitido você ler esta sessão do local ou executar esta ação.<br/>Um outro usuário restringiu o acesso à informação que você pediu.";
$invalid_chars = "Os seguintes caracteres não são permitidos";

$strGroups = "Grupos";
$strOwner = "Proprietário";

$strDeletePrefix = "Tem certeza que deseja excluir ";
$strDeleteSuffix = "?";

//menu
$menu_logout = "Sair";
$menu_applications = "Aplicações";
$menu_help = "Ajuda";
$menu_about = "Sobre";
$menu_documentation = "Documentação";
$menu_configuration = "Configuração";
$menu_groups = "Grupos de Usuários";
$menu_accounts = "Sua Conta";
$menu_preferences = "Preferências";
$menu_bookmarks = "Favoritos";
$menu_bookmarks_manage = "Organizar meus Favoritos";
$menu_add_bookmark = "Adicionar aos Favoritos";
$menu_add_current = "Adicionar está página aos Favoritos";

$menu_invite = "Convidar outros usuários";

$menu_search_discussions = "Discussões";
$menu_search_files = "Arquivos e Pastas";
$menu_search_web = "Na Web";
$menu_scheduler = "Calendário";
$menu_filetypes = "Tipos de Arquivos";
$menu_discussions = "Discussão";

$menu_users = "Usuários";
$menu_start = "Página Inicial";
$menu_reboot = "Reiniciar o Servidor";
$menu_halt = "Parar o Servidor";
$menu_reboot_text = "AVISO! Todos os serviços do Group-Office estarão temporariamente indisponíveis. Tem certeza que quer continuar?";
$menu_halt_text = "AVISO! Todos os serviços do Group-Office serão terminados. Tem certeza que quer continuar?";

$menu_new = "Nova";
$menu_message = "Mensagem";
$menu_contact = "Contato";
$menu_event = "Evento";
$menu_bookmark = "Favoritos";
$menu_topics = "Tópicos";
$menu_support = "Suporte";
$menu_projects = "Projetos";
$menu_sysinfo = "Informação sobre o sistema";
$menu_admin = 'Administrador';
$menu_modules = 'Módulos';
$menu_manual = 'Manual';

//class validate
$error_required = "Você não especificou o campo requerido abaixo.";
$error_match_auth = "Os códigos de autorização não combinaram.";
$error_match_pass = "As senhas não combinaram.";
$error_min_length = "Caracteres mínimos do campo abaixo:";
$error_max_length = "Caracteres máximos do campo abaixo:";
$error_email = "O endereço de E-mail que você especificou é innválido."; 
$error_email_exists = "O endereço de E-mail que você especificou já existe.";
$error_username_exists = "Desculpe, mas o nome do usuário que você introduziu já existe. Por favor escolha outro.";
$errors_in_form = "Existem erros no formulário. Corrija-os e tente outra vez.";
$error_missing_field = "Você não preencheu todos os campos requeridos.";

//acl control
$acl_control_auth_groups = "Grupos autorizados";
$acl_control_auth_users = "Usuários autorizados";
$acl_control_no_contacts = "Você não tem contatos cadastrados.<br /> Use o Livro de Endereços para adicionar membros aos seus contatos."; 

$strShowProfile = "Veja o perfil";
$strShowGroup = "Veja os membros dos grupos";
$strProcessingTime = "Tempo para abrir esta página ";
$strLoggedInAs = "Logado como ";
$strAt = " em ";
$strEmailTo = "Envie uma mensagem para: ";
$strProtectedUser = "Usuários Protegidos";

$http_not_found = "O Arquivo não foi encontrado";
$http_not_found_text = "O Arquivo que você está pesquisando não foi encontrado. Tente talvez corrigir o URL.";
$http_error = "Erro Interno do Servidor";
$http_bad_request = "Má requisição";
$strAddContact = "Adicionar aos contatos";

$strReadRights = "Permissão para ler";
$strWriteRights = "Permissão para escrever";
$strPermissions = 'Permissões';

$editor_bold = "Negrito";
$editor_italic = "Itálico";
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
$editor_rule = "Régua horizontal";
$editor_very_small = "Muito Pequeno";
$editor_small = "Pequeno";
$editor_medium = "Médio";
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
$editor_heading = "Cabeçalho";
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
$months[2]="Março";
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
$full_days[2] = "Terça-feira";
$full_days[3] = "Quarta-feira";
$full_days[4] = "Quinta-feira";
$full_days[5] = "Sexta-feira";
$full_days[6] = "Sábado";
$strLoadTime = 'Gerado dentro de';
$strDescription = 'Descrição';
$strTitle = 'Título';

$editorCmd['undo'] = "Desfazer";
$editorCmd['redo'] = "Refazer";
$editorCmd['bold'] = "Negrito";
$editorCmd['italic'] = "Itálico";
$editorCmd['underline'] = "Sublinhado";
$editorCmd['left'] = "Alinhar à Esquerda";
$editorCmd['right'] = "Alinhar à Direita";
$editorCmd['full'] = "Justificado";
$editorCmd['center'] = "Centralizado";
$editorCmd['ord_list'] = "Numeração";
$editorCmd['bul_list'] = "Marcadores";
$editorCmd['txt_color'] = "Cor do Texto";
$editorCmd['back_color'] = "Cor do Fundo";
$editorCmd['hyperlink'] = "Hyperlink";
$editorCmd['image'] = "Inserir imagem";
$editorCmd['rule'] = "Régua Horizontal";
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
$editorCmd['heading'] = "Cabeçalho";
$editorCmd['select'] = "Selecionar";
$editorCmd['mode'] = "Alterar modo";
$editorCmd['textindicator'] = 'Estilo atual';

$cmdShowAll = 'Mostrar todos';
$cmdSearch = 'Procurar';

//translation required
$cmdYesToAll = 'Sim para todos';
$cmdNoToAll = 'Não para todos';

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
$cal_prev_month  = "Mês anterior (pressione para ver o menu)";
$cal_go_today = "Ver Hoje";
$cal_next_month = "Próximo mês (pressione para ver o menu)";
$cal_next_year  = "Próximo ano (pressione para ver o menu)";
$cal_drag_to_move = "Arraste para mover";
$cal_part_today = " (hoje)";
$cal_monday_first = "Exibir Segunda primeiro";
$cal_sunday_first  = "Exibir Domingo primeiro";
$cal_select_date = "Selecionar data";

$cmdPrint = 'Imprimir';
$strFirstName = 'Primeiro nome';
$strLastName = 'Último nome';
$strInitials = 'Iniciais';
$strSex = 'Sexo';
$strBirthday = 'Nascimento';
$strSexes['M'] = 'Masculino';
$strSexes['F'] = 'Feminino';

$strTitle = 'Título';

$strBeginning = 'Início';

$strMiddleName = 'Nome do meio';

$sir_madam['M'] = 'Senhor';
$sir_madam['F'] = 'Senhora';

$strModifiedAt = 'Alterar para';
$strCreatedAt = 'Criado em';

$htmlarea_language_name = 'br';

$strLocalization = 'Localização';

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

$ml_host_unreachable = ' não pôde ser localizado. A verificação automática de e-mails foi desativada.';
$strRegion = 'Região';

//2.05
$cmdReset = 'Limpar';
$strMyName = 'Meu nome';

//modules
$lang_modules['scheduler'] = 'Agenda';
$lang_modules['calendar'] = "Calendário";
$lang_modules['filesystem'] = 'Arquivos';
$lang_modules['email'] = 'E-mail';
$lang_modules['projects'] = 'Projetos';
$lang_modules['search'] = 'Pesquisar';
$lang_modules['contacts'] = "Contatos";
$lang_modules['addressbook'] = "Contatos";
$lang_modules['cms'] = "Websites";
$lang_modules['notes'] = "Anotações";
$lang_modules['bookmarks'] = "Favoritos";
$lang_modules['summary'] = "Sumário";
$lang_modules['todos'] = "Pendências";
$lang_modules['documents'] = "Documentos";
?>