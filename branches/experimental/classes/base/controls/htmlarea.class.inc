<?php
/*
   Copyright Intermesh 2003
   Author: Merijn Schering <mschering@intermesh.nl>
   Version: 1.0 Release date: 08 July 2003

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2 of the License, or (at your
   option) any later version.
 */

class htmlarea
{
  var $buttons = array();
  var $fonts = array();
  var $pagestyle = '';
  var $header = '';
  var $name;
  var $rows;
  var $width;
  var $browser_is_supported = false;
  var $button_controls = array();

  function get_styles($style)
  {
    $styles = array();
    $style_name = '';
    $in_style = false;

    $style = str_replace("\r", '', $style);
    $style = str_replace("\n", '', $style);
    $style = str_replace(' ', '', $style);
    for ($i=0; $i<strlen($style); $i++)
    {
      $char = $style[$i];

      if ($char == '{' || $char == ',')
      {
	$in_style = false;
	if (trim($style_name) != '')
	{
	  $styles[] = $style_name;
	}
	$style_name = '';
      }elseif($char == '.')
      {
	if ($style_name != '')
	{
	  $style_name = '';
	  $in_style = false;
	}else
	{
	  $in_style = true;
	}
      }elseif($char == ':')
      {
	$style_name = '';
	$in_style = false;
      }elseif($char == '}')
      {
	$in_style = false;
	$style_name = '';
      }else
      {
	if ($in_style)
	{
	  $style_name .= $char;
	}
      }
    }

    return $styles;
  }


  function register_dropdown($id, $tooltip, $options, $context, $action, $refresh)
  {
    $button['code'] = '
      config.registerDropdown({
id: "'.$id.'",
tooltip: "'.$tooltip.'",
options: {';
    $option_count = count($options);
    for($i=0;$i<$option_count;$i++)
      {
	if ($i!=0)
	{
	  $button['code'] .= ',';
	}
	$button['code'] .= '"'.$options[$i]['text'].'":"'.$options[$i]['value'].'"';
      }
    
    $button['code'] .= '},
context : "'.$context.'",
action: '.$action.',
refresh: '.$refresh.'
});';
    $button['id'] = $id;
    $this->buttons[] = $button;
  }

  function add_button($id, $tooltip, $image, $textmode, $action)
  {
    $button['code'] =  '
    config.registerButton("'.$id.'",  "'.$tooltip.'", "'.$image.'", '.$textmode.', '.$action.');
  ';
    $button['id'] = $id;
    $this->buttons[] = $button;
  }

  function add_font($name, $family)
  {
    $font['name']  = $name;
    $font['family']  = $family;
    $this->fonts[] = $font;
  }

  function get_textarea_header($width, $height)
  {
    $header = '
    <script type="text/javascript">
    function initEditor()
    {
      window.onresize = resize_editor;
      resize_editor();
    }

  function resize_editor()
  {
    if(textarea_obj = get_object("'.$this->name.'"))
    {
      var newHeight;
      var newWidth;

      if (parseInt(navigator.appVersion)>3)
      {
	if (navigator.appName=="Netscape")
	{
	  newHeight = window.innerHeight'.$height.';
	  newWidth = window.innerWidth'.$width.'-10;
	}
	if (navigator.appName.indexOf("Microsoft")!=-1)
	{
	  newHeight = document.body.offsetHeight'.$height.';
	  newWidth = document.body.offsetWidth'.$width.'-20;
	}
      }
      if (newHeight < 200) { newHeight = 200; }
      if (newWidth < 200) { newWidth = 200; }

      textarea_obj.style.height = newHeight + "px";
      textarea_obj.style.width = newWidth + "px";
    }
  }
  
  
  </script>';

    return $header;
  }

  function get_header($name, $width='600px', $height='400px', $rows='0', $pagestyle='', $config_options='', $table_operations=false, $restrict_editor=false)
  {
    global $GO_CONFIG, $htmlarea_language_name;
    $this->width = 0;
    $this->name = $name;
    $this->rows = 0;

    if (!$this->browser_is_supported)
    {
      return $this->get_textarea_header($width, $height);
    }

    $this->pagestyle = $pagestyle == '' ? $this->pagestyle : $pagestyle;

    $editor_url = $GO_CONFIG->control_url.'htmlarea/';
    $header = '
    <script type="text/javascript">
    _editor_url = "'.$editor_url.'";
  _editor_lang = "'.$htmlarea_language_name.'";
  </script>
    <script type="text/javascript" src="'.$GO_CONFIG->control_url.'htmlarea/htmlarea.js"></script>
    <!-- load the plugins -->
    <script type="text/javascript">
    HTMLArea.loadPlugin("CSS");
  ';
    if ($table_operations)
    {
      $header .= 'HTMLArea.loadPlugin("TableOperations");';
    }
    $header .= '
    var editor = null;

  function initEditor()
  {
    editor = new HTMLArea("'.$name.'");
    config = editor.config;
    config.killWordOnPaste=true;
    ';

    // TODO: htmlarea in $restrict_editor mode will not be affected by the hack in patch 372 (which is the following hack)
    if($restrict_editor)
    {
      $header .= 'config.toolbar = ['.
	'["formatblock", "space",'.
	'"bold", "italic", "underline", "strikethrough", "separator",'.
	'"subscript", "superscript", "separator",'.
	'"copy", "cut", "paste", "space", "undo", "redo" ],'.							
	'[ "justifyleft", "justifycenter", "justifyright", "justifyfull", "separator",'.
	'"lefttoright", "righttoleft", "separator",'.
	'"orderedlist", "unorderedlist", "outdent", "indent", "separator",'.
	'"inserthorizontalrule", "createlink", "insertimage", "inserttable", "htmlmode", "separator",'.
	'"popupeditor", "separator", "showhelp", "about" ]'.
	'];';
    }
    else
    {
      // This is a hacking way to solve the layout temporarilly
      // Begin of hacking block
      if (count($this->button_controls) > 0) {
        $header .= 'config.toolbar = [
    	[ "fontname", "space",
    	  "fontsize", "space",
    	  "formatblock", "space",
    	  "bold", "italic", "underline", "strikethrough", "separator",
    	  "subscript", "superscript", "separator",
    	  "copy", "cut", "paste", "space", "undo", "redo" ],
    	[ "justifyleft", "justifycenter", "justifyright", "justifyfull", "separator",
    	  "lefttoright", "righttoleft", "separator",
    	  "orderedlist", "unorderedlist", "outdent", "indent", "separator",
    	  "forecolor", "hilitecolor", "separator",
    	  "inserthorizontalrule", "createlink", "insertimage", "go_image",
              "inserttable", "htmlmode", "separator",
    	  "popupeditor", "separator", "showhelp", "about" ]
    	];';
      }
      foreach ($this->button_controls as $b) {
        $header .= $b['code'];
      }
  // End of hacking block
    }
    
    $header.='config.fontname = {'.
      '"Default":	   \'\','.
      '"Arial":	   \'arial,helvetica,sans-serif\','.
      '"Courier New":	   \'courier new,courier,monospace\','.
      '"Georgia":	   \'georgia,times new roman,times,serif\','.
      '"Tahoma":	   \'tahoma,arial,helvetica,sans-serif\','.
      '"Times New Roman": \'times new roman,times,serif\','.
      '"Verdana":	   \'verdana,arial,helvetica,sans-serif\','.
      '"impact":	   \'impact\','.
      '"WingDings":	   \'wingdings\''.
      '};';

    if ($width <= 0)
    {
      $width=substr($width,1);
      $header .= '

	config.width = 630;
      if (parseInt(navigator.appVersion)>3)
      {
	if (navigator.appName=="Netscape")
	{
	  config.width= window.innerWidth-'.$width.';
	}
	if (navigator.appName.indexOf("Microsoft")!=-1)
	{
	  config.width = document.body.offsetWidth-'.$width.'-20;
	}
	if (config.width < 200)
	{
	  config.width = 200;
	}
      }
      ';
    }else
    {
      $header .= 'config.width = "'.$width.'";';
    }

    if ($height <= 0)
    {
      $height=substr($height,1);
      $header .= '
	config.height = 460;

      if (parseInt(navigator.appVersion)>3)
      {
	if (navigator.appName=="Netscape")
	{
	  config.height = window.innerHeight-'.$height.';
	}
	if (navigator.appName.indexOf("Microsoft")!=-1)
	{
	  config.height = document.body.offsetHeight-'.$height.'-20;
	}

	if (config.height < 200)
	{
	  config.height = 200;
	}
      }
      ';
    }else
    {
      $header .= 'config.height = "'.$height.'";';
    }

    $header .= '
      // enable creation of a status bar?
      config.statusBar = false;

    // the next parameter specifies whether the toolbar should be included
    // in the size or not.
    config.sizeIncludesToolbar = true;

    config.pageStyle = "'.addslashes($this->pagestyle).'";';

    $header .= $config_options;

    //add buttons
    $first=true;
    $push = 'config.toolbar.push([';
    while ($button = array_shift($this->buttons))
    {
      $header .= $button['code'];
      if ($first)
      {
	$first=false;
	$push .= "'".$button['id']."'";
      }else
      {
	$push .= ",'".$button['id']."'";
      }
    }
    $push .= ']);';

    $header .= $push;
    if ($table_operations)
    {
      $header .= 'editor.registerPlugin(TableOperations);';
    }
    if ($combos = $this->generate_combos())
    {
      $header .= 'editor.registerPlugin(CSS, '.$combos.');';
    }

    $header .= '
      setTimeout(function(){editor.generate();}, 500);
    window.onresize = resize_editor;
  }

  function editor_insertHTML(html) {
    editor.insertHTML(html);
  }

  function editor_insertImage(src)
  {
    alert(src);
    var img = new Image();
    img.src=src;
    img.border=0;
    img.align="absmiddle";
    img.vspace=0;
    img.hspace=0;
    editor._insertImage(img);
  }

  function resize_editor()
  {
    var newHeight;
    var newWidth;

    if (parseInt(navigator.appVersion)>3)
    {
      if (navigator.appName=="Netscape")
      {
	newHeight = window.innerHeight-'.$height.'-100;
	newWidth = window.innerWidth-'.$width.'-10;
      }
      if (navigator.appName.indexOf("Microsoft")!=-1)
      {
	newHeight = document.body.offsetHeight - editor._toolbar.offsetHeight - '.$height.'-20;
	newWidth = document.body.offsetWidth-'.$width.'-20;
      }
    }
    if (newHeight < 200) { newHeight = 200; }
    if (newWidth < 200) { newWidth = 200; }

    if (editor.config.statusBar) {
      newHeight -= editor._statusBar.offsetHeight;
    }

    editor._textArea.style.height = editor._iframe.style.height = newHeight + "px";
    editor._textArea.style.width = editor._iframe.style.width = newWidth + "px";
  }
    ';
    $header .= "
  function clean_editor_html() {
		var D = editor.getInnerHTML();
		
		// make one line
		D = D.replace(/\\r\\n/g, ' ').
			replace(/\\n/g, ' ').
			replace(/\\r/g, ' ').
			replace(/\\&nbsp\\;/g,' ');

		// keep tags, strip attributes
		D = D.replace(/ class=[^\s|>]*/gi,'').
			//replace(/<p [^>]*TEXT-ALIGN: justify[^>]*>/gi,'<p align=\"justify\">').
			replace(/ style=\\\"[^>]*\\\"/gi,'').
			replace(/ align=[^\\s|>]*/gi,'');

		//clean up tags
		D = D.replace(/<b [^>]*>/gi,'<b>').
			replace(/<i [^>]*>/gi,'<i>').
			replace(/<li [^>]*>/gi,'<li>').
			replace(/<ul [^>]*>/gi,'<ul>');

		// replace outdated tags
		D = D.replace(/<b>/gi,'<strong>').
			replace(/<\\/b>/gi,'</strong>');

		// mozilla doesn't like <em> tags
		D = D.replace(/<em>/gi,'<i>').
			replace(/<\\/em>/gi,'</i>');

		// kill unwanted tags
		D = D.replace(/<\\?xml:[^>]*>/g, '').       // Word xml
			replace(/<\\/?st1:[^>]*>/g,'').     // Word SmartTags
			replace(/<\\/?[a-z]\:[^>]*>/g,'').  // All other funny Word non-HTML stuff
			replace(/<\\/?font[^>]*>/gi,'').    // Disable if you want to keep font formatting
			replace(/<\\/?span[^>]*>/gi,' ').
			replace(/<\\/?div[^>]*>/gi,' ').
			replace(/<\\/?pre[^>]*>/gi,' ');


		//remove empty tags
		//D = D.replace(/<strong><\/strong>/gi,'').
		//replace(/<i><\\/i>/gi,'').
		//replace(/<P[^>]*><\\/P>/gi,'');

		// nuke double tags
		oldlen = D.length + 1;
		while(oldlen > D.length) {
			oldlen = D.length;
			// join us now and free the tags, we'll be free hackers, we'll be free... ;-)
			D = D.replace(/<([a-z][a-z]*)> *<\\/\\1>/gi,' ').
				replace(/<([a-z][a-z]*)> *<([a-z][^>]*)> *<\\/\\1>/gi,'<$2>');
		}
		D = D.replace(/<([a-z][a-z]*)><\\1>/gi,'<$1>').
			replace(/<\\/([a-z][a-z]*)><\\/\1>/gi,'<\\/$1>');

		// nuke double spaces
		D = D.replace(/  */gi,' ');

		//Convert P to BR
		D = D.replace(/<\\/p>/gi,'<br />').
			replace(/<p[^>]*>/gi, '');

		editor.setHTML(D);
		editor.updateToolbar();
  
  }
  </script>
     ";

    return $header;
  }

  function generate_combos()
  {
    $styles = $this->get_styles($this->pagestyle);

    if (count($styles) > 0)
    {
      $combos = '
    {
combos : [
	 { label: "Syntax:",

options: {"CSS":""';

      for ($i=0;$i<count($styles);$i++)
      {
	$combos .= ', "'.$styles[$i].'":"'.$styles[$i].'"';
      }

      $combos .= '}
	 }
	 ]
    }';
      return $combos;
    }else
    {
      return false;
    }
  }

  function print_htmlarea($value='')
  {
    echo $this->get_htmlarea($value);
  }

  function get_htmlarea($value='')
  {
    return '<textarea id="'.$this->name.'" style="width: '.$this->width.'" name="'.$this->name.'" rows="'.$this->rows.'">'.$value.'</textarea>';
  }

  function browser_is_supported()
  {
    return $this->browser_is_supported;
  }

  function force_textmode()
  {
    $this->browser_is_supported = false;
  }

  function htmlarea()
  {
    $browser = detect_browser();
    if ($browser['name'] == 'MSIE' && $browser['version'] >= 5.5 || $browser['name'] == 'MOZILLA' && $browser['version'] > 4.0 && $browser['subversion'] >= 20030210)
    {
      $this->browser_is_supported = true;
    }
  }
}
?>
