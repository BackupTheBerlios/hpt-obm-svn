<?php
require($GO_LANGUAGE->get_fallback_language_file('email'));

//Email
$ml_from = "Afzender";
$ml_to = "Aan";
$ml_send = "Verzenden";
$ml_attach = "Bijvoegen";
$ml_attachments = "Bijlagen";
$ml_attachment = "Bijlage";
$ml_high = "Hoog";
$ml_normal = "Normaal";
$ml_low = "Laag";
$ml_priority = "Prioriteit";
$ml_subject = "Onderwerp";
$ml_filename = "Bestandsnaam";
$ml_total_size = "Totale grootte";
$ml_size = "Grootte";
$ml_to_empty = "U heeft geen geadresseerde opgegeven.";
$ml_attach_forget = "U heeft een bijlage geselecteerd maar nog niet bijgevoegd. Klik op Bijvoegen.";
$ml_print = "Printen";
$fbNoSelect = "U heeft geen bericht geselecteerd";
$ml_inbox = 'Postbus';

$ml_add_account = "Toevoegen";
$em_new_folder = "Nieuwe&nbsp;map";
$ml_your_accounts = "Uw E-mail accounts";
$ml_no_accounts = "U heeft nog geen E-mail accounts ingesteld";
$ml_original_follows = "----- Oorspronkelijk Bericht -----";
$ml_welcome = "Welkom bij Group-Office E-mail";
$ml_text = "Klik op 'Accounts' om een account toe te voegen.";
$ml_accounts = "Accounts";
$ml_compose = "Nieuw&nbsp;bericht";
$ml_delete = "Verwijder";
$ml_refresh = "Vernieuwen";
$ml_reply = "Antwoorden";
$ml_forward = "Doorsturen";
$ml_reply_all = "Allen beantwoorden";
$ml_delete_message = "Weet u zeker dat u dit bericht permanent wilt verwijderen?";
$ml_no_subject = "Geen Onderwerp";
$ml_no_reciepent = "Niet gespecificeerde ontvanger";
$ml_online_files = "Online Bestanden";
$ml_send_error = "Er is een fout opgetreden bij het verzenden. Bericht is niet verstuurd.";
$ml_messages = "berichte(n)";
$ml_message = 'dit bericht';
$ml_messages2 = 'berichten';
$ml_new = "Nieuw";
$ml_delete_mailbox = "Weet u zeker dat u alle geselecteerde berichten permanent wilt verwijderen?";
$ml_add_contact = "Voeg afzender toe aan adresboek";

$strHost = "Host";
$strDefault = "Standaard";

$ml_edit_account = "Account bewerken";
$ml_new_account = "Nieuwe account";
$ml_no_messages = "Er zijn geen berichten in deze map.";
$ml_file_too_big = "Het bestand dat u probeerde bij te voegen is te groot. De maximale grootte voor bijlagen is: ";
$fbNoRightToOverwrite = "Het bestand bestaat al en u bent niet gemachtigd om deze te overschrijven.";
$ml_save_attachment = "Bijlage opslaan in Group-Office.";
$ml_folders = "Mappen";
$ml_properties = 'Eigenschappen';

$ml_select_folder = "Selecteer de map waar Group-Office verzonden E-mails in op zal slaan of maak een nieuwe map";
$ml_connect_failed = "Kan geen verbinding maken met mail server:";
$ml_at_port = "op poort";
$ml_solve_error = 'Klik op \'Accounts\' om de account instellingen te corrigeren of klik op \'Vernieuwen\' om de standaardaccount te openen.';
$ml_sent_items = "Verzonden items";
$ml_spam = "Spam";
$ml_use = "Gebruik";
$ml_synchronise = "Synchroniseren";
$ml_move_mail = "Verplaats geselecteerde berichten naar map...";
$ml_filters = "Filters";
$ml_move_to = "Verplaats het bericht dan naar map:";
$ml_email_is = "Als het adres van de afzender het volgende bevat:";
$ml_subject_is = "Als het onderwerp het volgende bevat:";
$ml_to_is = "Als het geadresseerden veld het volgende bevat:";
$ml_cc_is = "Als het cc veld het volgende bevat:";

$ml_new_filter = "Nieuwe Filter:";
$ml_field = "Veld";
$ml_contains = "Bevat";
$ml_folder = "Map";
$ml_no_folders = "U heeft geen mappen dus kunt u geen filters maken.";
$ml_unwanted = "Klik hier als E-mail van deze afzender ongewenst is";
$fbFolderSize = "Grootte van deze map";
$ml_move_up = "Verhoog prioriteit";
$ml_delete_filter = "Weet u zeker dat u deze filter wilt verwijderen?";
$ml_high_priority = "Dit bericht heeft hoge prioriteit!";
$ml_low_priority = "Dit bericht heeft lage prioriteit.";
$ml_block = "Blokkeer Afzender";
$ml_sent_items_fail = "Het bericht is verzonden maar er is een fout opgetreden bij het plaatsen van het bericht in uw Verzonden Items map. Controleer uw IMAP account.";
$fbProperties = "Eigenschappen";
$ml_notification = "Verzoek ontvanger om een leesbevestiging te versturen";
$ml_ask_notify = "De afzender verzoekt u om een leesbevestiging. Wilt u een leesbevestiging versturen?";
$ml_displayed = "Het volgende bericht is weergegeven aan ";
$ml_notify = "Leesbevestiging";
$ml_delete_filter1 = "Verwijder filter";
$ml_html_message_attached = "Oorspronkelijke HTML berichten zijn bijgevoegd";
$ml_add_recievers = "Voeg onbekende ontvangers toe aan...";
$ml_no_folders = 'U heeft geen mappen om de mail naar toe te filteren.';
$ml_no_filters = 'U heeft geen filters';

$ml_host = 'Host';
$ml_port = 'Port';
$ml_type = 'Type';

$ml_loose_changes = 'Wijzigingen gaan hierbij verloren. Weet u het zeker?';
$ml_format = 'Formaat';
$ml_servertype = 'Servertype';
$ml_markread = "Als gelezen";
$ml_markunread = "Als ongelezen";
$ml_root_mailbox = "de root mailbox";
$ml_inside = "in";
$ml_no_mailbox = "Dit is geen postbus. Deze map kan geen berichten bevatten.";
$ml_signature = 'Handtekening';
$ml_root = 'Root postbus';
$ml_disable_accounts = 'Schakel e-mail account beheer uit';

$ml_trash = 'Prullenbak';
$ml_disable = 'Uitschakelen';

$ml_confirm_empty_mailbox = 'Weet u zeker dat u deze map wilt legen?';
$ml_empty_mailbox = 'Map&nbsp;legen';

$ml_you_have = 'U heeft';
$ml_new_mail_single = 'nieuw E-mail bericht.';
$ml_new_mail_multiple = 'nieuwe E-mail berichten.';

$ml_auto_check = 'Controleer deze account automatisch op nieuwe berichten.';

$ml_no_contacts_in_mailing_group = 'Er zitten geen contactpersonen in de mailing groep die u heeft geselecteerd. Voeg deze eerst toe.';
$ml_attention = 'Attentie';

//2.02
$ml_autocheck_disabled = ' kon niet worden bereikt. De automatische controle op nieuwe berichten is uitgeschakeld voor deze account.';
$ml_mark = 'Markeer berichten...';
$ml_flag = 'Met vlag';
$ml_clearflag = 'Zonder vlag';
$ml_expand_all = 'Alle mappen openklappen';
$ml_collapse_all = 'Alle mappen dichtklappen';

//2.05
$ml_search = 'Zoeken';
$ml_answered = 'Beantwoord';
$ml_doesnt_matter = 'Maakt niet uit';
$ml_body = 'Body';
$ml_before = 'Ouder dan';
$ml_since = 'Nieuwer dan';
$ml_select_one_criteria = 'U dient tenminste één zoek criterium in te vullen of te selecteren.';
?>
