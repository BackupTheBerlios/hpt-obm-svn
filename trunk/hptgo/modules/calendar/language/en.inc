<?php
//Uncomment this line in new translations!
//require($GO_LANGUAGE->get_fallback_language_file('calendar'));

$sc_title = "Title";
$sc_description = "Description";
$sc_participants = "Invitees";

$sc_location = "Location";

$month_times[0] = "First";
$month_times[1] = "Second";
$month_times[2] = "Third";
$month_times[3] = "Fourth";

$sc_new_app = "New event";
$sc_calendars = "Calendars";
$sc_invited = 'You are invited to join the event described below.';

$sc_types1[0] = 'No recurrence';
$sc_types1[1] = 'days';
$sc_types1[2] = 'weeks';
$sc_types1[3] = 'months by date';
$sc_types1[4] = 'months by day';
$sc_types1[5] = 'years';


$sc_types[0] = 'Once';
$sc_types[1] = 'Daily';
$sc_types[2] = 'Weekly';
$sc_types[3] = 'Monthly by date';
$sc_types[4] = 'Monthly by day';
$sc_types[5] = 'Yearly';


$sc_start_at = 'Starting at';
$sc_end_at = 'Ending at';
$sc_start_time = 'Start time';
$sc_end_time = 'End time';

$sc_at_days = 'At days';
$sc_of_month = 'of the month.';

$sc_cycle_start = 'Cycle starts at';
$sc_cycle_end = 'Recur until';
$sc_accept_question = 'Do you wish to accept and schedule this event?';

$sc_accept = 'accept';
$sc_decline = 'decline';

$sc_notime = 'Time is not applicable';
$sc_noend = 'No ending date.';

$sc_never_happens = 'Bad input! This event never happens.';
$sc_start_later = 'Bad input!! The starting time is later then the ending time.';

$sc_add_self = 'Manual input';

$sc_select_calendar = 'Select the calendar to schedule this event in';
$sc_accept_confirm = 'The event is accepted and scheduled.';
$sc_no_calendars = 'No valid calendar found to schedule this event in.';
$sc_bad_event = 'No valid event found. The event is probably deleted.';
$sc_accept_title = 'Accept Event';

$sc_decline_confirm = 'The event is declined.';
$sc_decline_title = 'Decline event';

$sc_status = 'Status';
$sc_not_responded = 'Not responded';
$sc_accepted = 'Accepted';
$sc_declined = 'Declined';
$sc_no_participants = 'No participants';

$sc_person = 'Person';
$sc_owner = 'Owner';
$sc_type = 'Type';
$sc_subscribed = 'Subscribed';

$sc_subscribe = 'Subscribe';
$sc_new_calendar = 'New calendar';
$sc_no_calendar = 'No calendar was found. Please create one now or let Group-Office automatically create one for you.';
$sc_day_view = 'Day&nbsp;view';
$sc_week_view = 'Week&nbsp;view';
$sc_list_view = 'List&nbsp;view';
$sc_put_in = 'Put event in';
$sc_inherit_permissions ='Inherit permissions from';
$sc_no_inherit = 'Don\'t inherit';

$sc_select_calendar_please = 'Select at least one calendar please.';
$sc_events = 'Events';

$sc_from_calendar = 'from calendar';
$sc_enitrely = 'Entirely';
$sc_delete_event = 'Delete event';
$sc_delete_pre = 'Do you want to delete the event';
$sc_delete_suf = 'entirely or just from this calendar?';

$sc_refresh = 'Refresh';
$sc_reminder = 'Reminder';
$sc_mins = 'mins';
$sc_hour = 'hour';
$sc_hours = 'hours';
$sc_day = 'day';
$sc_days = 'days';
$sc_week = 'week';
$sc_weeks = 'weeks';
$sc_reminder_set_to_early = "The reminder cannnot be set to appear before the previous occurence. Please shorten the reminder time or lengthen the recurring period.";
$sc_cycle_start_later = 'The cycle starting date is later or equal then the ending date.';


$cmdGenerate = 'Generate';
$sc_preferences = 'Preferences';
$sc_holidays = 'Holidays';

$strNoHolidays = 'No holidays in database';
$strUnknown = 'Unknown';
$strYear = 'Year';
$strDeleteHolidaysPrefix = 'Are you sure you want to delete all holiday records of the year ';
$strDeleteHolidaysSuffix = '?';
$strReplaceHolidays = 'Are you sure you want to overwrite all existent holiday records?';
$sc_holidays_text1 = 'Please click on \''.$cmdGenerate.'\' to compute the holiday records for the selected year.';
$sc_holidays_text2 = 'Please click on \''.$cmdApply.'\' to store all computed holiday records in the database.';

//translation required below this
$sc_client = 'Client';
$sc_no_contact = 'None';

$sc_background = 'Background';

$sc_colors['red'] = 'Red';
$sc_colors['green'] = 'Green';
$sc_colors['blue'] = 'Blue';
$sc_colors['pink'] = 'Pink';
$sc_colors['orange'] = 'Orange';
$sc_colors['brown'] = 'Brown';
$sc_colors['yellow'] = 'Yellow';
$sc_colors['grey'] = 'Grey';
$sc_colors['purple'] = 'Purple';
$sc_colors['white']= 'White';
$sc_status = 'status';

$sc_show_hours = 'Show hours from';
$sc_to = 'to';

$sc_today = 'Today';

$sc_recur_every = 'Recur every';
$sc_months = 'months';

$sc_private_event = 'Private event';
$sc_calendar_exists = 'A calendar with this name already exists';
//translate
$sc_month = 'Month';
$sc_view = 'View';
$sc_calendar = 'Calendar';
$sc_default = 'Default';
$sc_month_view = 'Month&nbsp;view';

//text changed, needs new translation
$sc_holidays_no_file_prefix = 'The holidays couldn\'t be generated, because the file containing holidays information for the ';
$sc_holidays_no_file_suffix = ' language is either empty or doesn\'t exist.';

$sc_no_events = 'No events found';

//2.03
$cal_export = 'Export';
$cal_import = 'Import';
$cal_please_wait = 'Please wait...';
$cal_import_text = 'Select an iCal (*.ics) file and click at \'Ok\'';
$cal_filename = 'File';
$cal_import_success = 'Data imported';

//2.04
$cal_add_todo = 'New task';
$cal_todos = 'Tasks';

$cal_hide_completed = 'Hide completed';
$cal_show_all = 'Show all tasks';
$cal_show_own = 'Show own tasks';
$cal_show_responsible = 'Show reponsible tasks';

$cal_priority = 'Priotity';
$cal_priority_values[0] = 'Low';
$cal_priority_values[1] = 'Normal';
$cal_priority_values[2] = 'High';

$cal_status = 'Status';
$cal_canceled = 'Cancelled';
$cal_completed = 'Completed at';
$cal_percent_completed = '% completed';

$cal_before_task_start = 'Before the task starts';
$cal_before_task_due = 'Before the task should be finnished';
$cal_responsible = 'Responsible';
$cal_due_at = 'Due at';
$cal_no_todos= 'No tasks found';
$cal_resend_invitation = 'Resend invitation';

//2.05
$cal_everybody_write = 'Everybody can modify this event';
$cal_everybody_read = 'Only I can modify this event';

$cal_view = 'View';
$cal_views = 'Views';
$sc_view_exists = 'A view with this name already exists';
$cal_new_view = 'New view';
$cal_event_calendars = 'Calendar(s)';

//2.06
$cal_view_merged = 'merged';
$cal_view_emerged = 'emerged';
?>
