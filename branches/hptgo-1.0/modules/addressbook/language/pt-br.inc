<?php
require($GO_LANGUAGE->get_fallback_language_file('addressbook'));

//contacts
$contacts_title = "Contatos";
$contacts_no_contacts = "Nenhum contato encontrado.";
$contacts_members = "Membros";
$contacts_contacts = "Contatos";
$contacts_add = "Novo Contato";
$contacts_groups = "Grupos";
$contacts_other = "Outros Contatos";
$contacts_move_to_group = "Mude os contatos selecionados para o grupo...";
$groups_no_groups = "Voc� n�o tem nenhum grupo";
$contacts_group = "Grupo";
$contacts_empty_group = "Use o menu de lista para mudar contatos para este grupo.";
$contacts_this_group = 'Este grupo';



$add_contacts_text = "Preencha o endere�o de E-mail do contato e o codigo de autoriza��o e clique 'Ok'";
$add_contacts_success = "Usu�rio adicionado com sucesso";
$add_contacts_wrong = "C�digo errado do E-mail ou de autoriza��o";
$add_personal_text = "Clique sobre 'Salvar' para adicionar o contato. Os campos marcados com a * s�o obrigat�rios.";
$contacts_select = "Selecione os contatos para adicionar e clique 'Ok'.";
$contacts_select_title = "Contatos";
$contact_profile = "Perfil do contato";
$user_profile = "Perfil do usu�rio";
$contacts_authorise = "Autoriza��o";
$contacts_add_member = "Adicione este membro a seus contatos";
$strConfirmSynchronise = "Isto atualizar� este contato com dados que o usu�rio alterou. As suas mudan�as manuais ser�o perdidas. Tem certeza que voc� quer continuar?";
$contacts_import = "Importar";
$contacts_export = "Exportar";
$contacts_export_text = "Selecione o tipo de CSV que voc� gostaria de criar..";
$contacts_delete = "Excluir";



$contacts_import_text = "Selecione um arquivo de CSV e clique sobre 'Ok'";
$contacts_import_text_group = "Selecione um grupo e arquivo de CSV. Clique sobre 'Ok'.";
$fbNoFile = "Nenhum arquivo foi recebido";
$fbPleaseWait = "Por favor aguarde enquanto o arquivo � enviado...";



$strNotIncluded = "N�o importe";
$contacts_import_feedback = "As seguintes colunas foram encontradas no seu arquivo de CSV.<br/>";
$contacts_import_feedback .= "Selecione o grupo que voc� deseja importar ou criar automaticamente os grupos baseados numa coluna do arquivo.<br/>";
$contacts_import_feedback .= "Combine colunas correspondentes do Group-Office com as colunas do arquivo e verifique que voc� n�o seleciona a mesma coluna duas vezes.";
$contacts_separator = "Os valores s�o separados com";
$contacts_no_separator = "Voc� n�o inseriu um caracter de separa��o.";
$fbFilename = "Nome do arquivo";
$contacts_import_title = "Importar um Contato";
$contacts_import_success = "Arquivo importado com sucesso";
$contacts_import_double = "Voc� selecionou uma ou mais colunas duas vezes. Corrija isto e tente-o outra vez.";
$contacts_import_noname = 'Voc� deve selecionar o campo relacionado nome';
$contacts_import_incompatible = "Formato incompat�vel de arquivo.<br /><br />Um arquivo tipo CSV � um arquivo com valores separados por v�rgula.";
$strMember = "Membro";
$contact_exist_warning = "<b>Nota:</b> Este usu�rio j� est� na sua lista de contatos.";
$contacts_add_to_group = "Adicione os usu�rios selecionados aos contatos do grupo...";
$contacts_add_to = "Adicione os usu�rios selecionados aos contatos";
$contacts_add_user = "Adicionar";
$contacts_edit = "Modificar";
$contacts_no_select = "Voc� n�o selecionou nada";
$contacts_this_contact = "Este contato";



$contacts_seperator_text = 'Valores separados por';
$contacts_quote = 'Valores inclu�dos por';
$contacts_clrf = 'Linhas terminadas por';
$contacts_auto_group = 'Automaticamente crie grupos por arquivo';
$contacts_import_to_group = 'Importar ao grupo';



$contacts_search = 'Pesquisar';
$cmdSearch = 'Pesquisar';
$contacts_search_text = "Entre com a frase para busca e clique sobre 'pesquisar'. Voc� pode usar * como um caracter geral.";
$contacts_results = 'Resultado(s)';



$contacts_color = 'Cor';



$contacts_colors['black'] = 'Preto';
$contacts_colors['red'] = 'Vermelho';
$contacts_colors['green'] = 'Verde';
$contacts_colors['blue'] = 'Azul';
$contacts_colors['pink'] = 'Rosa';
$contacts_colors['orange'] = 'Laranja';
$contacts_colors['brown'] = 'Marron';



$ab_addressbooks = 'Outros Contatos';
$ab_owner = 'Propriet�rio';
$ab_subscribed = 'Ativado';
$ab_new_ab = 'Nova lista de contatos';
$ab_addressbook_exists = 'Um contatos com este nome j� existe';
$ab_addressbook = 'Contatos';
$contacts_select_single = 'Clique em um contato para selecionar uma pessoa';

$ab_group_exists = 'Um contatos com este nome j� existe';

//translation required
$ab_edit_contact = 'Clique em \'Ok\' para salvar as altera��es. Os campos marcados com * s�o obrigat�rios';
$ab_group_on_file = 'Grupo de contatos baseado em arquivo';
$ab_default = 'Padr�o';
$ab_browse = 'Procurar';
$ab_comment = 'Coment�rio';

$ab_contact = 'Contato';
$ab_send_message = 'E-mail';
$ab_new_event = 'Evento';

$ab_templates = 'Modelos';
$ab_your_templates = 'Seus';
$ab_all_templates = 'Todos';

$template_type[0] = 'E-mail';
$template_type[1] = 'OpenOffice.Org';
$ab_type = 'Tipo';
$ab_template_exists = 'A modelo com esse nome j� existe.';

$sir_madam['M'] = 'senhor';
$sir_madam['F'] = 'senhora';

$ab_upload_oo = 'Carregar modelo OpenOffice.org ';
$ab_upload_oo_text = 'Selecione o modelo que voc� editou com o OpenOffice.org e clique em \'Ok\'';
$ab_upload_nofile = 'Erro ao enviar arquivo.';

$ab_oo_doc = 'Documento de texto';
$ab_select_template = 'Selecione o modelo para este documento:';
$ab_add_field_data = 'Voc� pode usar os seguintes valores em seu documento:';
$ab_datafield = 'Campo de data';

$ab_no_template = 'Sem modelo';
$ab_new_note = 'Nota';

$ab_events = 'Apontamentos';

$ab_new_contact = 'Novo contato';
$ab_new_company = 'Nova empresa';
$ab_companies = 'Empresas';
$ab_company_properties = 'Perfil da empresa';
$ab_bank_no = 'N�mero do Banco';
$ab_companies_count_single = 'empresa';
$ab_companies_count = 'empresas';
$ab_no_companies = 'Nenhuma empresa encontrada';
$ab_no_company = 'Nenhuma empresa';

$ab_import_type = 'Importar';
$ab_no_company_found = 'Nenhuma empresa encontrada';
$ab_search_company = 'Procurar empresa';
$ab_edit_company = 'Alterar perfil';
$ab_new_group = 'Novo grupo';
$ab_export_type = 'Exportar';

$ab_custom_fields = 'Campos de data extras';
$ab_field_size = 'Tamanho';

$ab_new_field = 'Novo campo de data';
$ab_invalid_field_size = 'Tamanho inv�lido. Escolha um valor entre 10 e 255.';
$ab_field_exists = 'Um campo om este nome j� existe.';

$ab_employees = 'Funcion�rios';
$ab_no_employees = 'Nenhum funcion�rio encontrado';

$ab_employee_count_single = 'funcion�rio';
$ab_employee_count_multiple = 'funcion�rios';
$ab_vat_no = 'CNPJ';

$ab_add_new = 'Adicionar novo';
$ab_add_existing = 'Adicionar existente';

$ab_access = 'Acesso';

//1.98
$ab_mailings = 'E-mails';
$ab_mailing_groups = 'Listas de e-mails';
$ab_no_mailing_groups = 'Nenhuma lista de e-mails encontrada';
$ab_no_templates = 'Nenhum modelo encontrado';
$ab_mailing_group_exists = 'Uma lista de e-mails com este nome j� existe.';
$ab_contacts_count_single = 'contato';
$ab_contacts_count_multiple = 'contatos';
$ab_new_mailing = 'Nova lista de e-mails';
$ab_all_your_addressbooks = 'todos os seus contatos do contatos';

$ab_search_for = 'Procurar por';
$ab_search_in = ' em ';
$ab_search_on = ' em ';
$ab_search_keyword = 'Procurar frase';
$ab_wildcard = '(Use \'%\' como caractere curinga)';

//2.03
$ab_copy_read_acl = 'Copiar permiss�es de leitura pata contatos e empresas';
$ab_copy_write_acl = 'Copiar permiss�es de altera��o para contatos e empresas';

$ab_catagory_exists = 'Uma categoria com este nome j� existe.';
$ab_add_catagory = 'Adicionar categoria';
$ab_add_field = 'Adicionar campo';
$ab_catagory = 'Categoria';

//2.04
$ab_todos = 'Pend�ncias';

//2.05
$ab_download = 'Baixar';
$ab_download_template = 'Clique no bot�o \'Baixar\' para baixar o modelo para edi��o em seu computador.';
$ab_upload = 'Enviar';
//modified string:
$ab_upload_oo_text = 'Quando finalizar a edi��o do seu modelo no  OpenOffice.org voc� dever� transfer�-lo de volta para o Group-Office. Selecione o arquivo e clique no bot�o \'Enviar\' para transferir seu modelo.';
$ab_file_type = 'Tipo de arquivo';
$ab_import_vcf_file_ok = 'O arquivo enviado est� ok. Clique em \'Ok\' para adot�lo em sua lista de contatos.';
$ab_import_failed = 'A importa��o falhou.';
//modified string:
$contacts_import_text = "Selecione o arquivo contendo os dados a serem importados e clique em 'Ok'";
?>
