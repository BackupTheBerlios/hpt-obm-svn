<?php
//Uncomment this line in new translations!
//require($GO_LANGUAGE->get_fallback_language_file('projects'));

$pm_invalid_period = 'You entered an invalid time period';
$pm_add_hours_success = 'Data as entered below was saved successfully';
$pm_already_booked = 'You have already booked in this period';
$pm_new_entry = 'New entry at';
$pm_fees = 'Fees';
$pm_new_fee = 'New fee';
$pm_users = 'Users';
$pm_materials = 'Materials';
$pm_new_material = 'New material';
$pm_project_data = 'Projectdata from';
$pm_load= 'Load';
$pm_enter_data = 'Enter data';
$pm_value = 'Value';
$pm_too_old = 'You are not allow to enter data older then one week';

$pm_task = 'Tasks';

$pm_employee = 'Project Manager';
$pm_project = 'Project';
$pm_starttime = 'Starting time';
$pm_endtime = 'Ending time';
$pm_breaktime = 'Break time';
$pm_this_week = 'Entered this week';
$pm_no_projects = 'You don\'t have any projects';
$pm_hours = 'Hours';
$pm_total_hours = 'Total number of hours';
$pm_client = 'Client';
$pm_created_at = 'Created at';

$pm_show_all = 'Show all';
$pm_show_period = 'Show from';
$pm_to = 'to';
$pm_show_month = 'Show month';

$pm_total_hours_days = 'Total number of hours/days';
$pm_projects_count = 'project(s)';
$pm_show_data = 'Show data';
$pm_new_project = 'New project';
$pm_other_contacts = 'Other contacts';
$pm_no_contacts = 'You don\'t have a contact that can function as a client for this project. Please create one first';

$pm_unit_value = 'Unit value';
$pm_units = 'Units';
$pm_mins = 'mins';
$pm_no_fees = 'There are no fees created yet. Only a user with administrator privileges can create fees.';
$pm_fee = 'Fee';
$pm_no_contact = 'None';
$pm_load_period= 'Period';
$pm_load_type= 'Only load data for';
$pm_total_fee = 'Total fee';
$pm_total_days = 'Total days';
$pm_total_units = 'Total units';
$pm_summary = 'Summary';
$pm_this_entry = 'this booking';
$pm_entry = 'booking';
//translation required
$pm_timer_started_at = 'Timer started at';
$pm_start_timer = 'Clock in';
$pm_stop_timer = 'Clock out';

$pm_start_date = 'Start date';
$pm_end_date = 'End date';
$pm_responsible = 'Responsible';
$pm_status = 'Status';
$pm_category = 'Category';
$pm_process_template = 'Process template';
$pm_new_category = 'New Category';
$pm_new_template = 'New Template';

$pm_status_values[0] = 'Discontinued';
$pm_status_values[1] = 'Offer';
$pm_status_values[2] = 'Signed';
$pm_status_values[3] = 'Implementing';
$pm_status_values[4] = 'Done';
$pm_status_drop = 'Discontinued';
$pm_status_done = 'Done';
$pm_status_begin = 'Begin';
$pm_status_running = 'Running';

$pm_filter = 'Filter';
$pm_project_exists = 'A project with this name already exists';

$pm_clocked_in = 'You are clocked in. Click here if you want to clock out';

$pm_alert = 'This project is over due';
$pm_budget = 'Budget';
$pm_probability = 'Probability';
$pm_progress = 'Progress';
$pm_yours_only = 'Yourself';
$pm_description = 'Description';

$pm_no_projects = 'No projects found.';

$pm_days = 'days';
$pm_delay = 'Delay';
$pm_remain = 'Remain';
$pm_advance = 'Advance';
$pm_predict_delay = 'Predicted Delay';

$pm_task_status_values[0] = 'Bypass';
$pm_task_status_values[1] = 'Done';
$pm_task_status_values[2] = 'Ongoing';
$pm_task_status_values[3] = 'Waiting';

$pm_task_order = '#';
$pm_task_name = 'Task name';
$pm_task_person = 'Responsibility';
$pm_task_predecessors = 'Predecessors';
$pm_task_time = 'Days';
$pm_task_status = 'Status';
$pm_task_approved = 'Check';
$pm_task_document = 'Doc';
$pm_task_level = 'Level';

$pm_task_duration = 'Duration';
$pm_task_planning = 'Planning';
$pm_task_reality  = 'Reality';
$pm_task_advance  = 'Advance';
$pm_task_latency  = 'Latency';
$pm_task_start    = 'Start';
$pm_task_stop     = 'Stop';
$pm_task_days     = 'days';
$pm_task_day      = 'day';

$pm_task_comments = 'Comments';
$pm_task_new_comments = 'Type new comments here';
$pm_insert_task = 'Insert new task';
$pm_task_list_predecessors = 'Predecessors';
$pm_move_task_up = 'Move up';
$pm_move_task_dn = 'Move down';

$pm_legend_planning = 'Planning';
$pm_legend_ongoing = 'Ongoing';
$pm_legend_done = 'Done';

$pm_legend_addtask = 'Insert new task';
$pm_legend_movetask = 'Move task up/down';
$pm_legend_deltask = 'Delete task/template';

$cmdGantt = 'Gantt chart';
$cmdDrop = 'Discontinue';
$cmdRecover = 'Recover';

$pm_ConfirmDrop = 'Are you sure you want to discontinue this project?';
$pm_ConfirmRecover = 'Are you sure you want to recover this project?';
$pm_ConfirmDeleteStatus = 'Are you sure you want to delete status \"%s\"?';
$pm_ConfirmDeleteCategory = 'Are you sure you want to delete project category ';
$pm_ConfirmDeleteTemplate = 'Are you sure you want to delete process template ';
$pm_DeleteCategory = 'Delete';
$pm_CannotDelete = 'Cannot delete this project category, there are some projects using it';
$pm_CategoryExists = "Category with name '%s' is already exists";
$pm_EditCategory = 'Rename category:';
$pm_AddCategory = 'Add new category:';
$pm_EditTemplate = 'Rename process template: ';
$pm_AddTemplate = 'Add process template:';

$strReadRights = 'Monitors';
$strWriteRights = 'Members';
$strStatusChangeConfirm = 'You have changed task status to done. Everytime task status is changed to done, the project manager will be notified by email. Are you sure you want to change the task status?';
$mailTaskStatusChangedToDone  = '<html><body>You are requested to check and approve a done task:<br/><br/>Project name: %s<br/>Project description: %s<br/>Task name: %s<br/><br/>Please click <a href="%s">here</a> or go to the project section for more information.</body></html>';
$mailTaskStatusChangedToOngoing  = '<html><body>A task which you are assigned is changed to status %s:<br/><br/>Project name: %s<br/>Project description: %s<br/>Task name: %s<br/>Days: %s<br/><br/>Please click <a href="%s">here</a> or go to the project section for more information.</body></html>';
$subjectTaskStatusChanged = '[Projects] Status of task %s (project %s) has changed to %s';
$subjectTaskAssigneeChanged = '[Projects] Task responsibility has changed (task %s - project %s)';
$mailTaskAssigneeLeft = '<html><body>Công việc bạn đang thực hiện đã được chuyển sang người khác: <br/><br/>Tên dự án: %s<br/>Mô tả dự án: %s<br/>Tên công việc: %s</body></html>';
$mailTaskAssigneeJoined  = '<html><body>Bạn được chọn thực hiện công việc:<br/><br/>Project name: %s<br/>Project description: %s<br/>Task name: %s<br/>Days: %s<br/><br/>Please click <a href="%s">here</a> or go to the project section for more information.</body></html>';
?>
