<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" type="text/css" media="all" href="spell-check-style.css" />
</head>
<body onload="window.parent.finishedSpellChecking();">
<?php

    if ((!isset($_POST['dictionary'])) || (strlen(trim($_POST['dictionary'])) < 1))
    {
        $lang = 'en_US';
    } //if
    else
    {
        $lang = $_POST['dictionary'];
    } //else
    $pspell_link = pspell_new ($lang, '', '', 'utf-8', (PSPELL_NORMAL));
//    $pspell_link = pspell_new ($lang, '', '', 'ISO8859-15', (PSPELL_NORMAL));
//    $text = file_get_contents('content.html');
    $text = stripslashes($_POST['content']);
    
    require_once('XML/XML_HTMLSax.php');
    
    class MyHandler
    {
        var $start_pos;
        var $attrs =array();
        function MyHandler()
        {
//            $this->i = 0;
        }
        function openHandler(& $parser, $name, $attrs)
        {
            $this->start_pos = $parser->get_current_position();
            $this->attrs = $attrs;
        }
        function closeHandler(& $parser, $name)
        {
//            $GLOBALS['word'][($this->i) - 1]['end_pos'] = $parser->get_current_position();
        }
        function dataHandler(& $parser, $data)
        {
//            $GLOBALS['word'][$this->i]['data'] = $data;
//            $this->i++;
            if (!in_array('HA-spellcheck-fixed', $this->attrs))
            {
                $beforeword = substr($text, 0, $this->start_pos);
                $afterword = substr($text, $this->start_pos + strlen($data) + 1);
                $GLOBALS['text'] = str_replace($data, text_handler(unhtmlentities($data)), $GLOBALS['text']);
            } //if

        }
    }

    function unhtmlentities($string)
    {
        $trans_tbl = get_html_translation_table(HTML_ENTITIES);
        $trans_tbl = array_flip($trans_tbl);
        return strtr($string, $trans_tbl);
    }

    function text_handler($data)
    {
//quick and dirty hack
        $data = str_replace('$', ' $ ', $data);
        $data = str_replace('?', ' ? ', $data);
        $data = str_replace('!', ' ! ', $data);
        $data = str_replace('.', ' . ', $data);
        $data = str_replace(',', ' , ', $data);
        $data = str_replace(';', ' ; ', $data);
        $data = str_replace(':', ' : ', $data);
        $data = str_replace('(', ' ( ', $data);
        $data = str_replace(')', ' ) ', $data);
        $data = str_replace('{', ' { ', $data);
        $data = str_replace('}', ' } ', $data);
        $data = str_replace('[', ' [ ', $data);
        $data = str_replace('[', ' ] ', $data);
        $data = str_replace('"', ' " ', $data);
        $words = explode(' ', $data);
        foreach ($words as $key=>$word)
        {
            if (strlen($word) > 1)
            {
                $words[$key] = spellcheck($word);
            } //if
        } //foreach
        $data = implode(' ', $words);
        $data = str_replace(' $ ', '$', $data);
        $data = str_replace(' ? ', '?', $data);
        $data = str_replace(' ! ', '!', $data);
        $data = str_replace(' . ', '.', $data);
        $data = str_replace(' , ', ',', $data);
        $data = str_replace(' ; ', ';', $data);
        $data = str_replace(' : ', ':', $data);
        $data = str_replace(' ( ', '(', $data);
        $data = str_replace(' ) ', ')', $data);
        $data = str_replace(' { ', '{', $data);
        $data = str_replace(' } ', '}', $data);
        $data = str_replace(' [ ', '[', $data);
        $data = str_replace(' [ ', ']', $data);
        $data = str_replace(' " ', '"', $data);
        return $data;
    } //function

    function spellcheck($word)
    {
//echo '<hr>'.$word.'<br>';
//convert word from UTF8 -> ISO-8859-15
//        $word = utf8_decode($word_utf8);
        if (!pspell_check ($GLOBALS['pspell_link'], $word))
        {
            $suggestions = pspell_suggest ($GLOBALS['pspell_link'], $word);
            $suggestion_lst = '<span class="HA-spellcheck-suggestions">';
            foreach ($suggestions as $suggestion)
            {
                //convert word from ISO-8859-15 -> UTF8
//                $suggestion_lst .= utf8_encode($suggestion).',';
//                $suggestion_lst .= htmlentities($suggestion, ENT_COMPAT, 'utf-8').',';
                $suggestion_lst .= htmlentities($suggestion).',';
            }
            $suggestion_lst = substr($suggestion_lst, 0, strlen($suggestion_lst) - 1).'</span>';
            if (strlen($suggestion_lst) < 48)
            {
//                return '<span class="HA-spellcheck-error">'.htmlentities($word, ENT_COMPAT, 'ISO-8859-15').'</span><span class="HA-spellcheck-suggestions">'.htmlentities($word, ENT_COMPAT, 'ISO-8859-15').'</span>';
                return '<span class="HA-spellcheck-error">'.htmlentities($word).'</span><span class="HA-spellcheck-suggestions">'.htmlentities($word).'</span>';
            } //if
            else
            {
//                return '<span class="HA-spellcheck-error">'.htmlentities($word, ENT_COMPAT, 'ISO-8859-15').'</span>'.$suggestion_lst;
                return '<span class="HA-spellcheck-error">'.htmlentities($word).'</span>'.$suggestion_lst;
            } //else
        }
        else
        {
//            return $word_utf8;
            return htmlentities($word);
        } //else
    } //function

    // Instantiate the handler
    $handler=new MyHandler();
    
    // Instantiate the parser
    $parser=& new XML_HTMLSax();
    
    // Register the handler with the parser
    $parser->set_object($handler);
    
    // Set a parser option
    $parser->set_option('XML_OPTION_TRIM_DATA_NODES');
    
    // Set the handlers
    $parser->set_element_handler('openHandler', 'closeHandler');
    $parser->set_data_handler('dataHandler');
    
    // Parse the document
    $parser->parse($text);
    
    echo $text;

?>
<div id="HA-spellcheck-dictionaries">en_US,fr_FR,es</div></body></html>