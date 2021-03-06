<?php
require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "T�tulo";
$sc_description = "Descri��o";
$sc_participants = "Convidados";

$sc_location = "Local";

$month_times[0] = "Primeiro";
$month_times[1] = "Segundo";
$month_times[2] = "Terceiro";
$month_times[3] = "Quarto";

$sc_new_app = "Novo Evento";
$sc_calendars = "Calend�rios";
$sc_invited = 'Voc� foi convidado para partucipar do evento descrito abaixo.';

$sc_types1[0] = 'Sem repeti��o';
$sc_types1[1] = 'dias';
$sc_types1[2] = 'semanas';
$sc_types1[3] = 'data do m�s';
$sc_types1[4] = 'dia do m�s';
$sc_types1[5] = 'anos';

$sc_types[0] = 'Uma vez';
$sc_types[1] = 'Diariamente';
$sc_types[2] = 'Semanalmente';
$sc_types[3] = 'Mensalmente por data';
$sc_types[4] = 'Mensalmente por dia';
$sc_types[5] = 'Anualmente';


$sc_start_at = 'In�cio';
$sc_end_at = 'T�rmino';
$sc_start_time = 'Hora inicial';
$sc_end_time = 'Hora de t�rmino';

$sc_at_days = 'Nos dias';
$sc_of_month = 'do m�s.';

$sc_cycle_start = 'Per�odo iniciado em';
$sc_cycle_end = 'Repetir at�';
$sc_accept_question = 'Voc� deseja aceitar e agendar este evento?';

$sc_accept = 'aceitar';
$sc_decline = 'recusar';

$sc_notime = 'Tempo n�o aplic�vel';
$sc_noend = 'Sem data para t�rmino.';

$sc_never_happens = 'Dados incorretos! Este evento nunca ocorreu.';
$sc_start_later = 'Dados incorretos! A hora inicial � maior que a hora final.';

$sc_add_self = 'Entrada manual';

$sc_select_calendar = 'Selecione o calend�rio pra agendar este evento';
$sc_accept_confirm = 'O evento foi aceito e agendado.';
$sc_no_calendars = 'Nenhum calend�rio v�lido foi encontradopara agendar este evento.';
$sc_bad_event = 'Nenhum evento v�lido encontrado. O evento provavelmente foi apagado.';
$sc_accept_title = 'Aceitar evento';

$sc_decline_confirm = 'O evento foi recusado.';
$sc_decline_title = 'Recusar evento';

$sc_status = 'Situa��o';
$sc_not_responded = 'Sem resposta';
$sc_accepted = 'Aceito';
$sc_declined = 'Recusado';
$sc_no_participants = 'Sem participantes';

$sc_person = 'Pessoa';
$sc_owner = 'Propriet�rio';
$sc_type = 'Tipo';
$sc_subscribed = 'Inscrito';

$sc_subscribe = 'Inscrever';
$sc_new_calendar = 'Novo calend�rio';
$sc_no_calendar = 'Nenhum calend�rio encontrado. Por favor, crie um agora ou permita uma cria��o autom�tica.';
$sc_day_view = 'Listar por dia';
$sc_week_view = 'Listar por semana';
$sc_list_view = 'Listar eventos';
$sc_put_in = 'Colocar evento em';
$sc_inherit_permissions ='Permiss�es herdadas de';
$sc_no_inherit = 'N�o herdar';

$sc_select_calendar_please = 'Selecione apenas um calend�rio.';
$sc_events = 'Eventos';

$sc_from_calendar = 'Deste calend�rio';
$sc_enitrely = 'Completamente';
$sc_delete_event = 'Apagar evento';
$sc_delete_pre = 'Voc� deseja apagar o evento';
$sc_delete_suf = 'completamente ou apenas deste calend�rio?';

$sc_refresh = 'Atualizar';
$sc_reminder = 'Lembrete';
$sc_mins = 'minutos';
$sc_hour = 'hora';
$sc_hours = 'horas';
$sc_day = 'dia';
$sc_days = 'dias';
$sc_week = 'semana';
$sc_weeks = 'semanas';
$sc_reminder_set_to_early = "O lembrete n�o poder� aparecer antes de uma ocorr�ncia pr�via. Por favor, reduza o tempo do lembrete ou aumente o per�odo de repeti��o.";
$sc_cycle_start_later = 'A data inicial do per�odo � maior que a data final.';


$cmdGenerate = 'Gerar';
$sc_preferences = 'Prefer�ncias';
$sc_holidays = 'Feriados';

$strNoHolidays = 'Sem feriados no banco de dados';
$strUnknown = 'Desconhecido';
$strYear = 'Ano';
$strDeleteHolidaysPrefix = 'Voc� tem certeza que deseja apagar todos os feriados do ano ';
$strDeleteHolidaysSuffix = '?';
$strReplaceHolidays = 'Voc� tem certeza que deseja sobrescrever todos os feriados existentes?';
$sc_holidays_text1 = 'Por favor clique em \''.$cmdGenerate.'\' para processar os feriados para o ano selecionado.';
$sc_holidays_text2 = 'Por favor clique em \''.$cmdApply.'\' para armazenar os feriados para o ano selecionado.';

//translation required below this
$sc_client = 'Cliente';
$sc_no_contact = 'Nenhum';

$sc_background = 'Fundo';

$sc_colors['red'] = 'Vermelho';
$sc_colors['green'] = 'Verde';
$sc_colors['blue'] = 'Azul';
$sc_colors['pink'] = 'Rosa';
$sc_colors['orange'] = 'Laranja';
$sc_colors['brown'] = 'Marrom';
$sc_colors['yellow'] = 'Amarelo';
$sc_colors['grey'] = 'Cinza';
$sc_colors['purple'] = 'Violeta';
$sc_colors['white']= 'Branco';
$sc_status = 'situa��o';

$sc_show_hours = 'Exibir horas de';
$sc_to = 'at�';

$sc_today = 'Hoje';

$sc_recur_every = 'Repetir a cada';
$sc_months = 'meses';

$sc_private_event = 'Evento particular';
$sc_calendar_exists = 'Um calend�rio com esse nome j� existe';
//translate
$sc_month = 'M�s';
$sc_view = 'Exibir';
$sc_calendar = 'Calend�rio';
$sc_default = 'Padr�o';
$sc_month_view = 'Listar por m�s';

//text changed, needs new translation
$sc_holidays_no_file_prefix = 'Os feriados n�o puderam ser gerados porqu� o arquivo contendo as informa��es dos feriadospara o ';
$sc_holidays_no_file_suffix = ' idioma est� vazio ou n�o existe.';

$sc_no_events = 'Nenhum evento encontrado.';

//2.03
$cal_export = 'Exportar';
$cal_import = 'Importar';
$cal_please_wait = 'Por favor, aguarde...';
$cal_import_text = 'Selecione um arquivo iCal (*.ics) e clique em \'Ok\'';
$cal_filename = 'Arquivo';
$cal_import_success = 'Data importada';

//2.04
$cal_add_todo = 'Nova pend�ncia';
$cal_todos = 'Pend�ncias';

$cal_hide_completed = 'Ocultar conclu�das';
$cal_show_all = 'Exibir todas';
$cal_show_own = 'Exibir as minhas';
$cal_show_responsible = 'Exibir por respons�vel';

$cal_priority = 'Prioridade';
$cal_priority_values[0] = 'Baixa';
$cal_priority_values[1] = 'M�dia';
$cal_priority_values[2] = 'Alta';

$cal_status = 'Situa��o';
$cal_canceled = 'Cancelada';
$cal_completed = 'Conclu�da em';
$cal_percent_completed = '% conclu�da';

$cal_before_task_start = 'Antes da pend�ncia iniciar';
$cal_before_task_due = 'Antes da pend�ncia ser finalizada';
$cal_responsible = 'Respons�vel';

$cal_due_at = 'At�';
$cal_no_todos= 'Nenhuma pend�ncia encontrada';
$cal_resend_invitation = 'Reenviar convite';

//2.05
$cal_everybody_write = 'Todos podem alterar este evento';
$cal_everybody_read = 'Apenas eu posso alterar este evento';

$cal_view = 'Interface';
$cal_views = 'Interface';
$sc_view_exists = 'Uma interface com este nome j� existe';
$cal_new_view = 'Nova interface';
$cal_event_calendars = 'Calend�rio(s)';
?>
