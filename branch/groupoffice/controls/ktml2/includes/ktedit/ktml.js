// Copyright 2001-2004 Interakt Online. All rights reserved.


/*******************************
	STATIC
*/	
if (!ktmls) {
	var ktmls = new Array();
	Ktml_strip = true;
}

/**
	class ColorPalette 
*/
var ColorPaletteDivHead = 
'<table border="0" width="100%" style="border: solid 1px #000000" class="ktml_bg">'+
'  <tr class="ktml_bg">'+
'    <td valign="top"><fieldset class="ktml_fieldset" style="height: 98%;">'+
'<legend class="ktml_legend">@@picker@@</legend>'+
'<center>'+
'			<table cellspacing="1" border="0" width="100%" class="cc_innerTable">'+
'				<tr>'+
'					<td align="left" style="border: 0px;padding:1px;">'+
'						<label style="vertical-align:middle">@@enter@@</label>'+
'						<input class="toolbaritem_txt" style="vertical-align:middle" type="text" name="ktml_@@ktml@@_cctext" id="ktml_@@ktml@@_cctext" value=""'+
'							onkeydown="return ktml_@@ktml@@.cpalette.keyDown(this, event);"'+
'							onkeyup="return ktml_@@ktml@@.cpalette.keyUp(this, event);"'+
'						/>'+
'					</td>'+
'					<td style="width:70px" id="ktml_@@ktml@@_ccpreview">&#160;</td>'+
'				</tr>'+
'			</table>'+
'			<table align="right" cellspacing="1" border="0">' +
'				<tr>'+
'					<td colspan="12"></td>';
var ColorPaletteDivFooter = 
'			</table>'+
'</center>'+
'</fieldset>'+
'</td>'+
'	<td style="width:80px" align="center" valign="top">'+
'		<div style="position:relative;margin-top: 10px;">'+
'			<label style="vertical-align:middle;margin-left:-7px;">@@selected@@</label>'+
'			<div style="margin-top: 3px;border: solid 1px #000000; width: 60px; height: 20px" id="ktml_@@ktml@@_ccsel">&#160;</div>'+
'		</div>'+
'		<input type="button" class="ktml_button" style="width: 60px;margin-top:20px" value="OK" onclick="ktml_@@ktml@@.cpalette.pickColor(ktml_@@ktml@@.cpalette.selcolor.style.backgroundColor)"/>'+
'		<input type="button" class="ktml_button" style="width: 60px;margin-top:10px" value="Cancel" onclick="ktml_@@ktml@@.cpalette.setVisible(false)"/>'+
'	</td>'+
'  </tr>'+
'</table>';

function ColorPalette(owner) {
	var d;
	this.owner = owner;
	this.div = document.getElementById('ktml_'+this.owner.name+'_ccdiv');
	this.original = "#000000";
	this.preview = 'ktml_'+this.owner.name+'_ccpreview';
	this.isVisible = false;
	this.isDrawn = false;
}
ColorPalette.prototype.initializeUI = ColorPalette_initializeUI;
ColorPalette.prototype.drawPicker = ColorPalette_drawPicker;
ColorPalette.prototype.drawTable =ColorPalette_drawTable;
ColorPalette.prototype.writeColBtn =ColorPalette_writeColBtn;
ColorPalette.prototype.dechex = ColorPalette_dechex;
ColorPalette.prototype.setAction = ColorPalette_setAction;
ColorPalette.prototype.setSelected = ColorPalette_setSelected;
ColorPalette.prototype.pickColor = ColorPalette_pickColor;
ColorPalette.prototype.mOver = ColorPalette_mOver;
ColorPalette.prototype.mClick = ColorPalette_mClick;
ColorPalette.prototype.keyDown = ColorPalette_keyDown;
ColorPalette.prototype.keyUp = ColorPalette_keyUp;
ColorPalette.prototype.keyPress = ColorPalette_keyUp;
ColorPalette.prototype.setVisible = ColorPalette_setVisible;
ColorPalette.prototype.showAt = ColorPalette_showAt;

function ColorPalette_dechex(val) {
var dxarr = new Array();
dxarr[10] = 'A';
dxarr[11] = 'B';
dxarr[12] = 'C';
dxarr[13] = 'D';
dxarr[14] = 'E';
dxarr[15] = 'F';
	if (val < 10) return val;
	else return dxarr[val];
}
function ColorPalette_writeColBtn(col) {
	return '<td style="background-color:#'+col+'; width:10px; height:12px;"'+
		'onmouseover="ktml_' + this.owner.name +
		'.cpalette.mOver(this, event, false)" onclick="ktml_'+this.owner.name+
		'.cpalette.mOver(this, event, true)" ondblclick="ktml_'+this.owner.name+
		'.cpalette.mClick(this, event)"></td>';
}

function ColorPalette_initializeUI() {
	this.drawPicker();
	var oname = this.owner.name;
	this.textfld = document.getElementById('ktml_'+oname+'_cctext');
	this.selcolor = document.getElementById('ktml_'+oname+'_ccsel');
	this.isDrawn = true;
}

function ColorPalette_drawTable() {
	var text = '';
	var kt_a, kt_b, kt_c, kt_hex_a, kt_hex_b, kt_hex_c;
	//var tm1 = new Date().getTime();
	for (var kt_j=0;kt_j<12;kt_j++) {
		kt_b = (kt_j%6)*3;
		text+="<tr>\n";
		//code for generating the color selector
		for (var kt_i=0;kt_i<18;kt_i++) {
			kt_a = (kt_i%6)*3;
			kt_c = parseInt(kt_i/6)*3 + parseInt(kt_j/6)*9;
		
			kt_hex_a = this.dechex(kt_a);
			kt_hex_b = this.dechex(kt_b);
			kt_hex_c = this.dechex(kt_c);

			kt_hex_a += '' + kt_hex_a;
			kt_hex_b += '' + kt_hex_b;
			kt_hex_c += '' + kt_hex_c;
			text+=this.writeColBtn(kt_hex_c +''+ kt_hex_a +''+ kt_hex_b);
			//document.write('<td style="background-color:#' + kt_hex_c +''+ kt_hex_a +''+ kt_hex_b + '" width="10" onmouseover="ktml_<?php echo $objectName?>.cpalette.mOver(this, event, false)" onclick="ktml_<?php echo $objectName?>.cpalette.mOver(this, event, true)" ondblclick="ktml_<?php echo $objectName?>.cpalette.mClick(this, event);">&#160;</td>\n');
		}
		text+="</tr>\n";
	}
	return text;
}

function ColorPalette_drawPicker() {
	var text = '';
	var header = ColorPaletteDivHead.replace(/@@ktml@@/g, this.owner.name);
	header = header.replace(/@@enter@@/g, KT_js_messages["Enter a color:"]);
	header = header.replace(/@@picker@@/g, KT_js_messages["Color picker"]);
	text+=header;
	text+=this.writeColBtn('000000');
	text+=this.writeColBtn('333333');
	text+=this.writeColBtn('666666');
	text+=this.writeColBtn('999999');
	text+=this.writeColBtn('cccccc');
	text+=this.writeColBtn('ffffff');
	text+='</tr>';
	text+=this.drawTable();
	var footer = ColorPaletteDivFooter.replace(/@@ktml@@/g, this.owner.name);
	footer = footer.replace(/@@selected@@/g, KT_js_messages["Selected"]);
	text+=footer;
	this.div.innerHTML = text;
}
function ColorPalette_mOut(o) {
	//this.tdobj.style.border = "outset 1px #cccccc";
}
function ColorPalette_mOver(o, e, t) {
	
	if (typeof(o.onmouseout) != "function") {
		o.tdobj = o;
		o.cc = this;
		o.tdbrd = o.style.border;
		o.onmouseout = ColorPalette_mOut;
		o.ondblclick = ColorPalette_mClick;
	}
	document.getElementById(this.preview).style.background = ColorPalette_transformColor(o.style.backgroundColor);
	document.getElementById(this.preview).innerHTML = ColorPalette_transformColor(o.style.backgroundColor);
	//o.style.border = "solid 1px #000000";
	if (t) {
		this.textfld.value = ColorPalette_transformColor(o.style.backgroundColor);
		this.selcolor.style.backgroundColor = ColorPalette_transformColor(o.style.backgroundColor);
	}
}

function ColorPalette_setAction(str) {
	this.actionStr = str;
}
function ColorPalette_setSelected(str) {
	if (!str) {
		str = "#000000";
	}
	this.original = str;
}

function ColorPalette_transformColor(c) {
	if(!Ktml_mozilla) {
		return c;
	}
	if (arr = c.match(/^rgb\(([0-9]+),\s*([0-9]+),\s*([0-9]+)\)/i)) {
		var ret = '';

		var tmp = decimalToHex(arr[1]);
		while (tmp.length<2) {
			tmp = "0" + tmp;
		}
		ret += tmp;

		var tmp = decimalToHex(arr[2]);
		while (tmp.length<2) {
			tmp = "0" + tmp;
	}
		ret += tmp;

		var tmp = decimalToHex(arr[3]);
		while (tmp.length<2) {
			tmp = "0" + tmp;
		}
		ret += tmp;
		
		return "#" + ret;
	} else {
		return c;
	}
}

function ColorPalette_transformColor2(c) {
	c = c;
	return c;
}
function ColorPalette_pickColor(selectedColor) {
	selectedColor = ColorPalette_transformColor(selectedColor);
	if (this.owner.util_restoreSelection) {
		this.owner.util_restoreSelection();
	}
	selectedColor = ColorPalette_transformColor(selectedColor);
	this.textfld.value = selectedColor;
	if (typeof(this.actionStr) == "function") {
		this.actionStr();
	} else {
		eval(this.actionStr);
	}
	this.setVisible(false);
	this.owner.undo.addEdit();
}
function ColorPalette_mClick(o, e) {
	this.cc.pickColor(this.tdobj.style.backgroundColor);
}

function ColorPalette_keyUp(o, e) {
try {
	this.selcolor.style.backgroundColor = this.textfld.value;
}
catch(e) {
	this.selcolor.style.backgroundColor = "#000000";
}
}

function ColorPalette_keyDown(o, e) {
if(Ktml_mozilla) {
	if (e.keyCode == 13) { // ENTER
		if (true) {
			this.pickColor(this.textfld.value);
		}
		e.preventDefault();
		e.stopPropagation();
	return false;
}
	//this.cw.focus();
	return true;
}
if (Ktml_ie) {
	if (e.keyCode == 13) { // ENTER
		if (true) {
			this.pickColor(this.textfld.value);
		}
		e.cancelBubble = true;
		e.returnValue = false;
		return false;
	}
	return true;
}
}

function ColorPalette_setVisible(par) {
	if(this.isVisible == par) {
		return;
	}
	if (par) {
		if (!this.original) {
			this.original = "#000000";
		}
		this.textfld.value = this.original;
		this.selcolor.style.backgroundColor = this.original;
		this.div.style.display = 'block';
		this.isVisible = true;
	} else {
		this.original = "#000000";
		this.div.style.display= 'none';
		this.isVisible = false;
	}
}

function ColorPalette_showAt(x, y) {
	if(!this.isDrawn) {
		this.initializeUI();
	}
	var d;
	d = util_positionOnScreen(x,y,340,300);
	this.div.style.left = d.x+'px';
	this.div.style.top = d.y+'px';
}

/**
	class UndoItem
*/
function UndoItem(undoManager, text) {
	this.logMeString = true;
	this.parent = undoManager;
	this.b = this.parent.hasBookmarks; // hasBookmarks
	this.text = text; // item text = this.parent.edit.body.innerHTML

	this.add(); // add 1st position
}

UndoItem.prototype.toString = UndoItem_toString;
UndoItem.prototype.add = UndoItem_add;
UndoItem.prototype.moveTo = UndoItem_moveTo;
UndoItem.prototype.undo = UndoItem_undo;
UndoItem.prototype.redo = UndoItem_redo;
UndoItem.prototype.getSelection = UndoItem_getSelection;
UndoItem.prototype.getBookmark = UndoItem_getBookmark;
UndoItem.prototype.getTrueSelection = UndoItem_getTrueSelection;
UndoItem.prototype.getNodeForPath = UndoItem_getNodeForPath;

function UndoItem_toString() {
	var str = "";
	return str;
}

function UndoItem_add() {
	// selection may not be ready
	var obj = this;
	if (this.parent.owner.edit.readyState != 'complete') {
		setTimeout(function () { return obj.add(); }, 100);
		return;
  }

	// gather bookmark and selection 
	var book, sel;
	sel = this.getSelection();
	book = this.getBookmark();
	if(!sel) {
		return;
	}
	
	this.selection_start = sel;
	this.bookmark_start = book;

	// exit if there's no difference from previous entry
	if (!this.selection_end) {
		this.selection_end = sel;
		this.bookmark_end = book;
	}
}

function UndoItem_getBookmark() {
	var selected = this.parent.owner.edit.selection;
	var tr = selected.createRange();

	if(this.b) {
		if (selected.type != "Control") {
			return tr.getBookmark();
		} else {
			return tr(0);
		}
	} else {
		return null;
	}
}

/**
* Compose a serialized, dom independent selection for mozilla
*
* @return serialized selection
*/
function UndoItem_getSelection() {

	var selected = this.parent.owner.edit.selection;
	var tr = selected.createRange();

	if(this.b) {
		return tr;
	} else {
		var sel = new Object;
		try {
			sel.startOffset = tr.startOffset;
			sel.endOffset = tr.endOffset;
			sel.startContainer = static_UndoItem_getNumericPath(tr.startContainer);
			sel.endContainer = static_UndoItem_getNumericPath(tr.endContainer);
		} catch(e) {
			//alert('getSelection ' + e);
			return null;
		}
		return sel;
	}
}

/**
* Deserialize a selection
*/
function UndoItem_getTrueSelection(sel) {
	if(this.b) {
		return sel;
	} else {
		var selected = this.parent.owner.edit.selection;
		var tr = selected.createRange();
		
		var node = this.getNodeForPath(sel.endContainer);
		try {
			tr.setEnd(node, sel.endOffset);
		} catch(e) {
			if(node.nodeValue) {
				//alert('value ' + node.nodeValue.length + ' ' + sel.endOffset);
			} else {
				//alert('kids ' + node.nodeName + ' ' + node.childNodes.length + ' ' + sel.endOffset);
				//window.status = "HACK";
				tr.setEnd(node.previousSibling, sel.endNode);
			}
		}
		node = this.getNodeForPath(sel.startContainer);
		tr.setStart(node, sel.startOffset);
		
		return tr;
	}
}

/**
*
* Compute a numeric (child index) path for a node.
*
* @static
*	@param
*		node - dom element
* @return 
*		array - numeric path;
*/
function static_UndoItem_getNumericPath(node) {
	var path = new Array();
	var root = node.ownerDocument.documentElement;
	while(node != root)  {
		var first = node.parentNode.firstChild;
		var i=0;
		while(node != first) {
			node = node.previousSibling;
			i++;
		}
		path.push(i);
		node = node.parentNode;
	}
	return path.reverse();
}

/**
* Find the dom node for coresponding path
*
* @param 
*		path - array - numeric path (children index list)
* @return 
*		dom element - node for path
*/
function UndoItem_getNodeForPath(path) {
	var node = this.parent.owner.edit.documentElement;
	var kids;
	for(var i=0; i<path.length; i++) {
		kids = node.childNodes;
		//alert(kids.length + ' ' + path[i]);
		node = kids[path[i]];
	}
	return node;
}

function UndoItem_moveTo(sel, book) {
	// move cursor and try to move to specified position
	var tr;
	this.parent.owner.cw.focus();
	if(this.b) {
		try {
			tr = this.parent.owner.edit.body.createTextRange();
			tr.moveToBookmark(book);
			tr.collapse();
			tr.select();
		} catch(e) {
			//alert(e.toString());
		}
	} else {
		try {
			tr = this.getTrueSelection(sel);
			tr.collapse(false);
		} catch(e) {
			//alert(e.toString());
		}
	}
}

function UndoItem_undo() {
	this.moveTo(this.selection_start, this.bookmark_start);
}

function UndoItem_redo() {
	this.moveTo(this.selection_end, this.bookmark_end);
}

/**
	class UndoManager
*/
function UndoManager(ktml) {
	this.owner = ktml;
	this.logMeString = true;
	this.limit = 50;
	this.edits = new Array();
	if (this.owner.edit.body.createTextRange) {
		this.hasBookmarks = 1;
	} else {
		this.hasBookmarks = 0;
	}
}

UndoManager.prototype.toString = Undo_toString;
UndoManager.prototype.init = Undo_init;
UndoManager.prototype.atStart = Undo_atStart;
UndoManager.prototype.atEnd = Undo_atEnd;
UndoManager.prototype.addEdit = Undo_addEdit;
UndoManager.prototype.undo = Undo_undo;
UndoManager.prototype.redo = Undo_redo;

function Undo_toString() {
	var i, str="";
	for (i=0; i<this.edits.length; i++) {
		str += this.edits[i].toString();
	}
	return str;
}

function Undo_init() {
	
	var tr;
	if (Ktml_ie) { // move caret at top of document
		this.owner.cw.focus();
		tr = this.owner.edit.selection.createRange();
		tr.collapse(true);
	}
	// add first entry - this one will never be changed
	this.cursor = 0;
	this.edits[0] = new UndoItem(this, this.owner.edit.body.innerHTML);
}

function Undo_atStart() {
	if (this.cursor < 1) {
		return true;
	} else {
		return false;
	}
}
function Undo_atEnd() {
	if (this.cursor >= this.edits.length - 1) {
		return true;
	} else {
		return false;
	}
}

function Undo_addEdit() {
	// get current position data
	var text;
	text = this.owner.edit.body.innerHTML; // text
	
	// if there's no difference from previous entry 
	// just exit (rst - modify to lenght - less restrictive )
	if (this.edits[this.cursor].text == text) {
		//this.edits[this.cursor].add();
		return;
	}
	
	// if we reached the limit remove edits[1] and add at end
	if (this.cursor == this.limit - 1) {
		//this.edits = this.edits.slice(1,1);
		for (var j=2; j<this.edits.length; j++) {
			this.edits[j-1] = this.edits[j];
		}
		//this.edits.length--;
	} else {
		this.cursor++; // move cursor to next position
	}
	// erase redos
	this.edits.length = this.cursor;
	// add new entry
	this.edits[this.cursor] = new UndoItem(this, text);
}

function Undo_undo() {
	if (this.atStart()) return;
	if (!this.edits[this.cursor]) return;
	this.cursor--;
	// restore text (exactly as it were)
	this.owner.edit.body.innerHTML = this.edits[this.cursor].text;
	//this.owner.util_loadBody(this.edits[this.cursor].text);
	this.edits[this.cursor].undo();
	this.owner.cw.focus();
}

function Undo_redo() {
	if (this.atEnd()) return;
	if (!this.edits[this.cursor]) return;
	this.cursor++;
	// restore text
	this.owner.edit.body.innerHTML = this.edits[this.cursor].text;
	//this.owner.util_loadBody(this.edits[this.cursor].text);
	this.edits[this.cursor].redo();
	this.owner.cw.focus();
}

/**
	class Toolbar
*/

function Toolbar(element, owner) {
	this.owner = owner;
	this.base = element;
	
	this.buttons = new Array();

	this.helpMode = false;
}

Toolbar.prototype.initializeUI = Toolbar_initializeUI;
Toolbar.prototype.getValueFromActiveForm = Toolbar_getValueFromActiveForm;
Toolbar.prototype.setHelpMode = Toolbar_setHelpMode;
Toolbar.prototype.getHelpMode = Toolbar_getHelpMode;
Toolbar.prototype.checkHelp = Toolbar_checkHelp;
Toolbar.prototype.showHelp = Toolbar_showHelp;

function Toolbar_initializeUI() {
	if (typeof(window.KTML_ROOT) == "undefined") {
		var str = window.location.href;
		var res, index;
		str = str.substring(0, str.lastIndexOf("/")+1);
		index = NEXT_ROOT.indexOf("./")
		if (index == 0) { //relative
			res = str + NEXT_ROOT.substring(2);
		} else {
			index = NEXT_ROOT.indexOf("/");
			if (index == 0) {
				res = window.location.host + NEXT_ROOT.substring(1);
			} else {
				res = str + NEXT_ROOT;
			}
		}
		window.KTML_ROOT = res;
	}
	this.url = this.getValueFromActiveForm("property_url", "TEXT", "all");
	this.font_type = this.getValueFromActiveForm("property_font_type", "SELECT", "all");
	this.font_size = this.getValueFromActiveForm("property_size", "SELECT", "all");
	//this.blockformat = this.getValueFromActiveForm("property_blockformat", "SELECT", "all");
	this.styler = this.getValueFromActiveForm("property_styler", "SELECT", "all");
	this.blockformat = this.getValueFromActiveForm("property_heading", "SELECT", "all");
	this.targets = this.getValueFromActiveForm("property_targets", "SELECT", "all");
			
	var kids = this.base.getElementsByTagName('img');
	var pbtn;
	for(var i=0;i<kids.length;i++) {
		pbtn = kids[i];
		//if(pbtn.getAttribute('cid') != DECMD_HYPERLINK && pbtn.getAttribute('kttype') == "btn") {
		if(pbtn.getAttribute('kttype') == "btn") {
			this.buttons.push(pbtn);
		}
		if(pbtn.getAttribute('id') == 'html') {
			this.swap = pbtn;
		}
		if(pbtn.getAttribute('id') == 'fgcolor') {
			this.fgcolor = pbtn;
		}
		if(pbtn.getAttribute('id') == 'bgcolor') {
			this.bgcolor = pbtn;
		}
	}
}

function Toolbar_getValueFromActiveForm(fieldName, fieldType, returnType) {
  switch (fieldType) {
    case "SELECT": var containers = this.base.getElementsByTagName("SELECT");
      break;
    case "TEXT": var containers   = this.base.getElementsByTagName("INPUT");
      break;
    case "CHECKBOX": var containers = this.base.getElementsByTagName("INPUT");
      break;
  }
  var valToReturn = false;
  for (var i=0;i<containers.length;i++) {
    if (containers[i].name == fieldName || containers[i].id == fieldName ) {
      if (fieldType == "TEXT") {
      	if (returnType == "value") {
        	valToReturn = containers[i].value;
        } else if (returnType == "all") {
        	valToReturn = containers[i];
        }
      } else if (fieldType == "SELECT") {
    		if (returnType == "value") {
      		valToReturn = containers[i].options[containers[i].selectedIndex].value;
      	} else if (returnType == "all") {
      		valToReturn = containers[i];
      	}
      } else if (fieldType == "CHECKBOX") {
    		if (returnType == "value") {
      		valToReturn = containers[i].checked;
      	} else if (returnType == "all") {
      		valToReturn = containers[i];
      	}
      }
    }
  }
  return valToReturn;
}

function Toolbar_setHelpMode(hMode) {

	function massCursorElems(el, tag, how) {
		var a, i;
		a = el.getElementsByTagName(tag);
		for (i=0; i<a.length; i++) {
			a[i].style.cursor = how;
		}
	}
	
	function massCursorToChildren(el, how) {
		el = document.getElementById(el);
		if(el) { // due to lazy instanciation this could not exist yet
			massCursorElems(el, "INPUT", how);
			massCursorElems(el, "SELECT", how);
			massCursorElems(el, "IMG", how);
		}
	}
	
	function massCursor(k, how) {
		if(k.properties) {
			massCursorToChildren(k.properties.td, how);
			massCursorToChildren(k.properties.tr, how);
			massCursorToChildren(k.properties.table, how);
			massCursorToChildren(k.properties.img, how);
		}
		massCursorToChildren(k.propertieslink.a, how);
	}

	this.helpMode = hMode;
	if (hMode) {
		for (i = 0; i< this.buttons.length; i++) {
			this.buttons[i].style.cursor = 'help';
		}
		massCursor(this.owner, 'help');
		document.getElementById('ktml_'+this.owner.name+'_help').className = "toolbaritem_latched";
		document.getElementById('ktml_'+this.owner.name+'_help').state = 2;
	} else {
		for (i = 0; i< this.buttons.length; i++) {
			this.buttons[i].style.cursor = 'default';
		}
		massCursor(this.owner, '');
		document.getElementById('ktml_'+this.owner.name+'_help').className = "toolbaritem_flat";
		document.getElementById('ktml_'+this.owner.name+'_help').state = 1;
	}
}
function Toolbar_getHelpMode() {
	return this.helpMode;
	
}
function Toolbar_checkHelp(obj, e) {
	if (this.helpMode) {
		if (Ktml_ie) {
			e.cancelBubble = true;
			e.returnValue = false;
		} else {
			e.preventDefault();
			e.stopPropagation();
		}
		return true;
	} else {
		return false;
	}
}

function Toolbar_showHelp(fname) {
	var r;
	r = Math.random().toString().substring(3);
	if (arguments[1]) {
		strToOpen = KTML_ROOT+"includes/ktedit/" +arguments[2] +'/help/' +this.owner.UILanguage+"/"+fname+".html?ktmlid="+this.base.pageId;
	} else {
		strToOpen = KTML_ROOT+"includes/ktedit/help/"+this.owner.UILanguage+"/"+fname+".html?ktmlid="+this.base.pageId;
	}
	util_openwnd("_help", strToOpen, 550, 300, 1);
}

/**
	class Ktml
*/


function Ktml(name) {
	this.name = name;
	this.setButtons = true;
	this.displayMode = 'RICH';
	this.displayShouldChange = true;
	this.viewinvisibles = false;

	this.selectableNodes = new Array();
	this.svdSelection = null;	
	this.mozilla = Ktml_mozilla;	
	
	this.inspectedNode = null;

	ktmls.push(this); 
	this.pageId = ktmls.length-1;
	this.keepAliveTime = -1;
	
	this.defaultFontFace = '';
	this.defaultFontSize = '';
	this.defautlStyle = '';
	this.stripServer = true;
	
	this.autoFocus = true;
	this.saveXHTML = false;
}


// public methods
Ktml.prototype.initializeUI = Ktml_initializeUI;

// API calls
Ktml.prototype.getContent = Ktml_getContent;
Ktml.prototype.setContent = Ktml_setContent;
Ktml.prototype.setFontFace = Ktml_setFontFace;
Ktml.prototype.setFontSize = Ktml_setFontSize;
Ktml.prototype.setStyle = Ktml_setStyle;
Ktml.prototype.stripServerLocation = Ktml_stripServerLocation;

//--------------------------------------------------
Ktml.prototype.focus = Ktml_focus;
Ktml.prototype.getName = Ktml_getName;
Ktml.prototype.setPathToStyle = Ktml_setPathToStyle;
Ktml.prototype.setPathToImageDir = Ktml_setPathToImageDir;
Ktml.prototype.setPathToFileDir = Ktml_setPathToFileDir;
Ktml.prototype.setUseTemplates = Ktml_setUseTemplates;
Ktml.prototype.setUseIntrospection = Ktml_setUseIntrospection;
Ktml.prototype.setUseLinkIntrospection = Ktml_setUseLinkIntrospection;
Ktml.prototype.setUseSpellcheck = Ktml_setUseSpellcheck;
Ktml.prototype.setKeepAliveTime = Ktml_setKeepAliveTime;
Ktml.prototype.setDefaultLanguage = Ktml_setDefaultLanguage; 
Ktml.prototype.setUILanguage = Ktml_setUILanguage; 
Ktml.prototype.setAutoFocus = Ktml_setAutoFocus;
Ktml.prototype.setXHTML = Ktml_setXHTML;

// private methods - do not call as they are subject to changes
Ktml.prototype.p_registerEvents = Ktml_p_registerEvents;
Ktml.prototype.p_wait_displayChanged = Ktml_p_wait_displayChanged;
Ktml.prototype.p_displayChanged = Ktml_p_displayChanged;
Ktml.prototype.p_wait_undo = Ktml_p_wait_undo;

//Ktml.prototype.hndlr_onBlurUrl = Ktml_hndlr_onBlurUrl;
Ktml.prototype.hndlr_onBlur = Ktml_hndlr_onBlur;
Ktml.prototype.hndlr_onFocus = Ktml_hndlr_onFocus;
Ktml.prototype.hndlr_onkeypress = Ktml_hndlr_onkeypress;

Ktml.prototype.util_expandSelection_saveSel = Ktml_util_expandSelection_saveSel;
Ktml.prototype.util_expandSelection = Ktml_util_expandSelection;
Ktml.prototype.util_expandSelection_enclose = Ktml_util_expandSelection_enclose;
Ktml.prototype.util_insertRawHtml = Ktml_util_insertRawHtml;
Ktml.prototype.util_insertNodeHtml = Ktml_util_insertNodeHtml;
Ktml.prototype.util_cleanHTMLContent = Ktml_util_cleanHTMLContent;
Ktml.prototype.util_cleanHTMLContent2 = Ktml_util_cleanHTMLContent2;
Ktml.prototype.util_checkFocus = Ktml_util_checkFocus;
Ktml.prototype.util_loadBody = Ktml_util_loadBody;
//ie behavoir workaround
if (Ktml_ie) {
	Ktml.prototype.util_saveSelection = Ktml_util_saveSelection;
	Ktml.prototype.util_restoreSelection = Ktml_util_restoreSelection;
}

Ktml.prototype.ui_setButtonState = Ktml_ui_setButtonState;
Ktml.prototype.ui_updateFontFace = Ktml_ui_updateFontFace;
Ktml.prototype.ui_updateFontSize = Ktml_ui_updateFontSize;
Ktml.prototype.ui_updateBlockFormat = Ktml_ui_updateBlockFormat;
Ktml.prototype.ui_updateStyle = Ktml_ui_updateStyle;
//Ktml.prototype.ui_getLink = Ktml_ui_getLink;

Ktml.prototype.logic_domSelect = Ktml_logic_domSelect;
Ktml.prototype.logic_isEditable = Ktml_logic_isEditable;
Ktml.prototype.logic_doFormat = Ktml_logic_doFormat;
Ktml.prototype.logic_InsertTable = Ktml_logic_InsertTable;
Ktml.prototype.logic_openInsertImage = Ktml_logic_openInsertImage;
Ktml.prototype.logic_openInsertLink = Ktml_logic_openInsertLink;
Ktml.prototype.logic_InsertImage = Ktml_logic_InsertImage;
Ktml.prototype.logic_setDisplayMode = Ktml_logic_setDisplayMode;
Ktml.prototype.logic_openPalette = Ktml_logic_openPalette;
Ktml.prototype.logic_InsertLink = Ktml_logic_InsertLink;
Ktml.prototype.logic_InsertHeading = Ktml_logic_InsertHeading;
//Ktml.prototype.logic_InsertLinkToFile = Ktml_logic_InsertLinkToFile;
//Ktml.prototype.logic_putTargetTag = Ktml_logic_putTargetTag;

Ktml.prototype.init_fillCssClasses = Ktml_init_fillCssClasses;
Ktml.prototype.init_createStyleSelect = Ktml_init_createStyleSelect;

if (Ktml_ie) {
	Ktml.prototype.logic_recCommand = Ktml_logic_recCommand;
}

/***************************************
*	PUBLIC API
*/

/**
* Gets the ktml's content with or without alteration
* Three alteration levels are possible (defaults to exact):
*	exact - returns the exact actual content of ktml
*	stripped - all URLs are stripped to / location of the site
*	optimized - not available
*
* @param
*	mode - string - one of {exact, stripped, optimized}
* @return 
*	string - ktml content
*/
function Ktml_getContent(mode) {
	if(!mode || mode == 'exact') {
		return this.edit.body.innerHTML;
	} else if(mode == 'stripped') {
		return hndlr_save(this.edit.body.innerHTML);
	} else if(mode == 'optimized') {
		return 'optimized mode is not yet supported';
	} else {
		return false;
	}
}

/**
* Sets the ktml's content
*
* @param
*	text - string - the new content
*/
function Ktml_setContent(text) {
	this.util_loadBody(hndlr_load(text, "RICH"));
}

/**
* Sets the default KTML's font type. Default value is ''.
*
* @param
*	fontface - string - the font type (HTML compliant)
*/
function Ktml_setFontFace(fontface) {
	this.defaultFontFace = fontface;
}

/**
* Sets the defaut KTML's font size. Default value is ''.
*
* @param
*	fotnsize - string - the font size (HTML compliant) 
*/
function Ktml_setFontSize(fontsize) {
	this.defaultFontSize = fontsize;
}

/**
* Sets the default KTML's style. Default value is ''.
*
* @param
*	style - string - the style (HTML compliant) 
*/
function Ktml_setStyle(style) {
	this.defaultStyle = style;
}

/**
* Sets the strip flag for KTML. If this flag is set to true
* then the KTML will strip server location from all URLs to 
* a relative path. Default value is true.
*
* @param 
*	strip - boolean - strip flag
*/
function Ktml_stripServerLocation(strip) {
	Ktml_strip = strip
}

/**
* Insert a text at the cursor position
*
* @param 
*	text - string - the text to be inserted (can be html) 
*/
function Ktml_insertAtCursor(text) {
	
}

/*******************************************
*	PROTECTED API
*/

/**
* initialise ktml ui
*/
function Ktml_initializeUI() {
	//if (!document.getElementById("Properties_a_href_"+this.name)) {
		//window.location.reload(true);
	//}
	if(!document.getElementById(this.name)) {
		// basic requirement - hidden field
		alert(KT_js_messages["hidden missing"] ? KT_js_messages["hidden missing"] : "1The hidden field related to this KTML control has no ID set!\n Check http://www.interakt.ro/products/technical_17.html for details.");
		return;
	}
		
	if (this.keepAliveTime > 0) {
		util_preserveSession(this.keepAliveTime * 1000);
	}
	this.iframe = document.getElementById(this.name+'_htmlObject'); // activeActiveX
	this.textarea = document.getElementById(this.name+"_textObject");
	this.edit = this.iframe.contentWindow.document;
	this.cw = this.iframe.contentWindow;
	this.frm = document.getElementById(this.name).form;
	if(!this.frm) {
		alert(KT_js_messages["No form"] ? KT_js_messages["No form"] : "KTML is not inside a form! You will not be able to sumbit your work.");
	}
	this.DOMProperty = document.getElementById("DOMProperty_"+this.name); // ~ status bar

	// set default style
		if(this.defaultFontFace != '') {
			this.edit.body.style.fontFamily = this.defaultFontFace;
		}
		if(this.defaultFontSize != '') {
			this.edit.body.style.fontSize = this.defaultFontSize;
		}
		if(this.defaultStyle != '') {
			this.edit.body.setAttribute('class', this.defaultStyle);
		}
	//

	// make it editable
	if(Ktml_mozilla) {
		this.edit.designMode='on';
		this.undo = new UndoManager(this);
	} else {
		this.edit.body.contentEditable=true;
		this.undo = new UndoManager(this);
	}
	
	if(document.getElementById(this.name).value){
		this.util_loadBody(hndlr_load(document.getElementById(this.name).value, "RICH"));
		if (Ktml_mozilla) {
			this.edit.designMode='on';
		}
	}

	//initialize undo manager
	if(this.undo) {
		this.undo.init();
	}
	
	// mozilla-IE compatibility
	if(Ktml_mozilla) {
		KT_attachProperties(this.iframe);
	}
	
	this.toolbar = new Toolbar(document.getElementById("Property_"+this.name), this); // toolbar - activeFormName	
	this.toolbar.initializeUI();
	if (this.useIntrospection) {
		this.properties = new PropertiesUI(this);
	}
	if (this.useLinkIntrospection) {
		this.propertieslink = new PropertiesLink(this);
	}

	this.cpalette = new ColorPalette(this);
	if(typeof SpellChecker != 'undefined') {
		this.spellcheck = new SpellChecker(this, this.defaultLanguage);
	} 
	if (this.useTemplates) {
		this.templateeditor = new TemplateEditor(this);
	}
	this.p_registerEvents(this);
	if (this.toolbar.styler) {
		this.init_createStyleSelect(this.useTemplates);
	}
	
}


function Ktml_getName() {
	return this.name;
}

/** 
* KTML environment setting
*/
function Ktml_setPathToStyle(pathToStyle) {
	this.pathToStyle = pathToStyle;
}
function Ktml_setPathToImageDir(pathToImageDir) {
	this.pathToImageDir = pathToImageDir;
}
function Ktml_setPathToFileDir(pathToFileDir) {
	this.pathToFileDir = pathToFileDir;
}
function Ktml_setUseTemplates(useTemplates) {
	this.useTemplates = useTemplates;
}
function Ktml_setUseIntrospection(useIntrospection) {
	this.useIntrospection = useIntrospection;
}
function Ktml_setUseLinkIntrospection(useLinkIntrospection) {
	this.useLinkIntrospection = useLinkIntrospection;
}
function Ktml_setUseSpellcheck(useSpellcheck) {
	this.useSpellcheck = useSpellcheck;
}
function Ktml_setKeepAliveTime(keepAlive) {
	this.keepAliveTime = keepAlive;
}
function Ktml_setDefaultLanguage(defLang) {
	this.defaultLanguage = defLang;
}
function Ktml_setUILanguage(uiLang) {
	this.UILanguage = uiLang;
}
function Ktml_setAutoFocus(autoF) {
	this.autoFocus = autoF;
}
function Ktml_setXHTML(saveXhtml) {
	this.saveXHTML = saveXhtml;
}

//---------------------------------------------------------
// init methods
//---------------------------------------------------------

function Ktml_focus() {
	var obj = this;
	if(this.autoFocus == 'yes') {
		setTimeout(function () { obj.cw.focus(); }, 400);
	} else {
		var node = null;
		var i=0;
		while(node == null && i < this.frm.elements.length) {
			if(this.frm.elements[i].type != 'hidden' && !this.frm.elements[i].disabled) {
				node = this.frm.elements[i];
			}
			i++;
		}
		if(node) {
			setTimeout(function () { node.focus(); }, 400);
		}
	}
}

function Ktml_init_fillCssClasses() {
	var obj = this;
	if (this.edit.readyState != "complete") {
	  setTimeout(function () { obj.init_fillCssClasses(); }, 400);
		return;
	} else {
	  if (!this.edit.stilu || this.edit.stilu.rules.length == 0) {
	    setTimeout(function () { obj.init_fillCssClasses(); }, 400);
			return;
	  } else {
	  	cssuri = this.toolbar.styler;
	    if (cssuri.options.length==1) {
				var tmparr = Array();
				for(var i=0;i<this.edit.stilu.rules.length;i++){
					tmparr[tmparr.length] = this.edit.stilu.rules[i].selectorText;
				}
				tmparr.sort();
	      for(i=0; i<tmparr.length; i++){
	        v = tmparr[i];
	        if(v.substring(0, 1)=="."){
	          o = new Option(v.substring(1), v.substring(1));
	          cssuri.options[cssuri.options.length]=o;
	        }
	      }
	    }
	  }
	}
}

function Ktml_init_createStyleSelect(useTemplates) {
	if (this.edit.stilu) {
		return;
	}

	var obj = this;
	if (this.edit.readyState != 'complete') {
		setTimeout(function () { obj.init_createStyleSelect(); }, 100);
		return;
  } 
	tmpstr = new String;
	try {
		var ssh = this.edit.createStyleSheet('');
	} catch(e) {
			//error action
	}
	docpath = document.location.pathname;
	str = docpath.split('/');
	if (str[str.length-1] != "") {		
		tmpstr = docpath.replace(str[str.length-1], this.pathToStyle);
	} else {
		tmpstr = docpath + this.pathToStyle;
	}
	cale = document.location.protocol + '//' + document.location.hostname + ((document.location.port != "") ? ":"+document.location.port : "") + tmpstr;
	if (useTemplates) {
	 	cale_template = cale.replace(/\/[a-zA-Z0-9_]+\.css/, "/template.css");
	}
	//ssh.addRule("OBJECT","border-color: black; border-width: 1px; border-style: dashed;");
	this.edit.ssh = ssh;
	this.edit.stilu = this.edit.createStyleSheet(cale);
	if (useTemplates) {
		this.edit.admin_stilu = this.edit.createStyleSheet(cale_template);
	}
	this.init_fillCssClasses();
	//}
}


//---------------------------------------------------------
// private methods
//---------------------------------------------------------

/**
* register events on editor
*/
function Ktml_p_registerEvents(obj) {
	if (Ktml_ie) {
		this.edit.attachEvent('onkeyup', function (e) {obj.hndlr_onkeyup(e);obj.p_wait_displayChanged(obj, e); });
		this.edit.attachEvent('onclick', function (e) {obj.cw.focus() });
		this.edit.attachEvent('onkeydown', function(e) { obj.hndlr_onkeydown(e);});
		this.edit.attachEvent('onmouseup', function () {if (obj.undo) { obj.undo.addEdit(); } obj.p_displayChanged(); });
		this.edit.attachEvent('onblur', function () { obj.hndlr_onBlur() });
		//this.edit.attachEvent('onfocus', function () { obj.hndlr_onFocus() });
		this.edit.attachEvent('onbeforedeactivate', function () { obj.hndlr_onBlur() });
		if(this.frm) {
			this.frm.attachEvent('onsubmit', function () { logic_submitKTMLS(); });
		}
	} else {
		this.edit.addEventListener('keyup', function (e) { obj.hndlr_onkeyup(e); obj.p_wait_displayChanged(obj); }, true);
		this.edit.addEventListener('click', function (e) {obj.cw.focus(); }, true);
		this.edit.addEventListener('keydown',  function(e) { obj.hndlr_onkeydown(e);}, true);
		this.edit.addEventListener('keypress',  function(e) { obj.hndlr_onkeypress(e);}, true);
		this.edit.addEventListener('mouseup', function () { if (obj.undo) { obj.undo.addEdit(); } obj.p_displayChanged(); }, true);
		this.edit.addEventListener('blur', function () { obj.hndlr_onBlur() }, true);
		if(this.frm) {
			this.frm.addEventListener('submit', function () { logic_submitKTMLS(); }, true );
		}
	}
}


/**
* prevent mozilla from getting the keys
*/
function Ktml_hndlr_onkeypress(e) {
	var keycode = e.keyCode;
	if (keycode == 0) {
		keycode = e.charCode;
	}
	if(e.ctrlKey || keycode == 17 || keycode == 9) {
		if ( ((keycode != 86) && (keycode != 88) && (keycode != 67)) && ((keycode != 120) && (keycode != 99) && (keycode != 118)) && (keycode != 70)  && (keycode != 102) ) { // if not Ctrl-X, Ctrl-C, Ctrl-V, dont propagate the event
			e.preventDefault();
			e.stopPropagation();
		}
	}
	return true;
}

if(Ktml_mozilla) {
function Ktml_hndlr_onkeydown(e) {
}
}

if (Ktml_mozilla) {
function Ktml_hndlr_onkeyup(e) {
	if (e.ctrlKey && e.keyCode == 90) { // Ctrl + Z UNDO
		var tmp = this.undo.edits[this.undo.cursor].text;
		tmp = tmp.replace(/<[^>]*>/g, '');
		var tmp2 = this.edit.body.innerHTML;
		tmp2 = tmp2.replace(/<[^>]*>/g, '');
		if (tmp != tmp2) {
			this.undo.addEdit();
		}
		this.undo.undo();
	} else if (e.ctrlKey && e.keyCode == 89) { // Ctrl + Y REDO
		this.undo.redo();
	} else if (e.ctrlKey && e.keyCode == 66) { // Ctrl + B Bold
		this.logic_doFormat('Bold');
	} else if (e.ctrlKey && e.keyCode == 73) { // Ctrl + I Italic
		this.logic_doFormat('Italic');
	} else if (e.ctrlKey && e.keyCode == 85) { // Ctrl + U Underline
		this.logic_doFormat('Underline');
	} else if (e.ctrlKey && e.keyCode == 35) { // Ctrl + End 
		this.logic_goToEnd();
	} else if (e.ctrlKey && e.keyCode == 36) { // Ctrl + Home 
		this.logic_goToHome();
	} else if (e.keyCode == 9 && !e.altKey) { // TAB
		if (e.shiftKey == true) {
			this.logic_doFormat('Outdent');
		} else {
			this.logic_doFormat('Indent');
		}
	} else if (e.keyCode == 32 || e.keyCode == 13 ||
			// arrows
			e.keyCode == 37 || e.keyCode == 38 || e.keyCode == 39 || e.keyCode == 40
		) {
		this.undo.addEdit();		
	} else if (e.keyCode == 46 || e.keyCode == 8) { // delete, backspace
		this.undo.addEdit();
		if (this.selectableNodes[0] && this.selectableNodes[0].tagName == "TABLE") {
			this.logic_removeTag();
		}
	} else {
		if(e.keyCode != 17) { // CTRL
			//this.p_wait_undo(this);
		}
		return true;
	}
	util_stopEvent(e);
	//this.cw.focus();
	return true;
}
}

if (Ktml_ie) {
function Ktml_hndlr_onkeyup(e) {
	if (this.edit && !this.edit.hasFocus()) {
		util_stopEvent(e);
		return false;
	}
	if (e.ctrlKey && e.keyCode == 90) { // Ctrl + Z UNDO
		this.undo.addEdit();
		this.undo.undo();
	} else if (e.ctrlKey && e.keyCode == 89) { // Ctrl + Y REDO
		this.undo.redo();
	} else if (e.ctrlKey && e.keyCode == 66) { // Ctrl + B Bold
		this.logic_doFormat(DECMD_BOLD);
	} else if (e.ctrlKey && e.keyCode == 73) { // Ctrl + I Italic
		this.logic_doFormat(DECMD_ITALIC);
	} else if (e.ctrlKey && e.keyCode == 85) { // Ctrl + U Underline
		this.logic_doFormat(DECMD_UNDERLINE);
	} else {
		// on writting, trigger undo on following keys
		if (!e.ctrlKey && 
			  // space, enter
			(e.keyCode == 32 || e.keyCode == 13 ||
				// arrows
				e.keyCode == 37 || e.keyCode == 38 || e.keyCode == 39 || e.keyCode == 40 ||
				// delete, backspace
				e.keyCode == 46 || e.keyCode == 8
			)
		) {
			this.undo.addEdit();
		}
		if (!e.ctrlKey && e.keyCode == 46) {
			//this.logic_removeTag();
		}
		//must add word and enter
		if(e.keyCode != 17) { // CTRL
			//this.p_wait_undo(this);
		}		
		this.cw.focus();
		return true;
	}
	// never get here
}
}

if (Ktml_ie) {
function Ktml_hndlr_onkeydown(e) {
	if (this.edit && !this.edit.hasFocus()) {
		util_stopEvent(e);
		return false;
	}
	
	if (e.keyCode == 9 && !e.altKey) { // TAB
		if (e.shiftKey == true) {
			this.logic_doFormat('Outdent');
		} else {
			this.logic_doFormat('Indent');
		}
		util_stopEvent(e);
	} else 	if (e.ctrlKey && (
				e.keyCode == 90 || // Z
				e.keyCode == 89 || // Y
				e.keyCode == 66 || // B
				e.keyCode == 73 || // I
				e.keyCode == 85 // U
		)) {
		util_stopEvent(e);
	} else if (e.keyCode == 8 || e.keyCode == 46) {// Backspace
		if (this.selectableNodes[0] && (
			this.selectableNodes[0].tagName == "TABLE" ||
			this.selectableNodes[0].tagName == "IMG")
		) {
			this.logic_removeTag();
			util_stopEvent(e);
		}
	}		
}
}


/**
* wait for user to stop typing and than fire event
*
* @param
*		obj - this
*/
function Ktml_p_wait_undo(obj) {
	if(window.toundo) {
		clearTimeout(window.toundo);
	}
	window.toundo = setTimeout(function () { obj.undo.addEdit(); }, 200);
}


/**
* wait for user to stop typing and than fire event
*/
function Ktml_p_wait_displayChanged(obj, event) {
	if(window.to) {
		clearTimeout(window.to);
	}
	window.to = setTimeout(function () { obj.p_displayChanged(); }, 200);
}

/**
*	display has changed so take action
*/
function Ktml_p_displayChanged() {
	this.displayShouldChange = false;
 	this.cpalette.setVisible(false);
	if (this.useSpellcheck) {
		this.spellcheck.invalidate();
	}
	var txtRange = this.edit.selection.createRange();
	if (this.edit.selection.type == "Text" || this.edit.selection.type == "None" ) {
		var tmpprs = txtRange.parentElement();
	} else {
		var tmpprs = txtRange.commonParentElement();
	}
	if (tmpprs.getAttribute("ktedit") == "2" || tmpprs.getAttribute("ktedit") == "3") {
		this._edit = false;
	} else {
		this._edit = true;
	}
	if (this.displayMode != "CODE") {
		if (this.setButtons) {
			this.ui_setButtonState();
		} 
	} else {
		this.DOMProperty.innerHTML = "";
	}
}

/**
* save the state in the hidden field
*/

function Ktml_hndlr_onBlur() {
	if(Ktml_mozilla) {
		document.getElementById(this.name).value = hndlr_save(this.edit.body.innerHTML);
	} else {
		document.getElementById(this.name).value = hndlr_save(this.edit.body.innerHTML);
		this.util_saveSelection();
	}
}

function Ktml_hndlr_onFocus() {
	if(Ktml_mozilla) {
	} else {
		//document.getElementById(this.name).value = hndlr_save(this.edit.body.innerHTML);
		this.util_restoreSelection();
	}
}


//----------------------------------------------------------------------------------------------
//		Application UI
//----------------------------------------------------------------------------------------------


/**
*	update the buttons state depending on context
*/
function Ktml_ui_setButtonState() {
	var pbtn;
	var qstate;
	
	this.displayShouldChange = true;
	

	//this is very expensive
	for(var i=0;i<this.toolbar.buttons.length;i++) {
		pbtn = this.toolbar.buttons[i];
		try {
			if (!pbtn.id.match(/_help/)) { // && !pbtn.id == "togglevisible"
				if (Ktml_mozilla) {
					qstate = this.edit.queryCommandState(pbtn.getAttribute('cid').toLowerCase()); 
				}
				if (Ktml_ie) {
					qstate = this.edit.queryCommandSupported(pbtn.getAttribute('cid'));
					qvalue = this.edit.queryCommandState(pbtn.getAttribute('cid'));
				}
				//var qvalue = activeActiveX.contentWindow.document.queryCommandValue(pbtn.getAttribute('cid').toLowerCase());	
				if (Ktml_ie) {
					if (qstate == false) {
						pbtn.className = "toolbaritem_flat"; 
						pbtn.state = 0;
					} else {
						pbtn.className = "toolbaritem_flat"; 
						pbtn.state = 1;
					}
					if (qvalue == true) {
						pbtn.className = "toolbaritem_latched"; 
						pbtn.state = 2;
					}
				} else {
					if (qstate == false) {
						pbtn.className = "toolbaritem_flat"; 
						pbtn.state = 0;
					} else {
						pbtn.className = "toolbaritem_latched"; 
						pbtn.state = 2;
					}
				}
			} else {
				if (this.toolbar.getHelpMode()) {
					pbtn.className = "toolbaritem_latched";
				} else {
					pbtn.className = "toolbaritem_flat";
				}
			}
		} catch (e) {
			if (pbtn.id == 'ktml_'+this.name+'_help') {
				if (this.toolbar.getHelpMode()) {
					pbtn.className = "toolbaritem_latched";
				} else {
					pbtn.className = "toolbaritem_flat";
				}
			} else if (pbtn.id == "togglevisible") {
				if (this.viewinvisibles) {
					pbtn.state = 2;
					pbtn.className = "toolbaritem_latched";
				} else {
					pbtn.state = 1;
					pbtn.className = "toolbaritem_flat";
				}
			} else {
				pbtn.className = "toolbaritem_flat"; 
				pbtn.state = 1;
			}
			// do nothing go to next button
		}
	}


	this.ui_updateFontFace();
	this.ui_updateFontSize();
	this.ui_updateBlockFormat();
	// this fixes 0002833 ... fast switching to another form element while typing breaks tag hierarchy 
	// window.to is set if there is a pending setTimeout, we should wait for it to finish
	if (!window.to && Ktml_ie) {
		//~ window.setTimeout("ktml_" + this.name + ".logic_updateDOMHierarchy(true, 0);", 200);
	this.logic_updateDOMHierarchy(true, 0); // not sure if 0 or 1 // expensive 60ms
	} else {
		this.logic_updateDOMHierarchy(true, 0); // not sure if 0 or 1 // expensive 60ms
	}
	this.ui_updateStyle();
	//this.ui_getLink();
	if (this.useIntrospection) {
		this.properties.update();
	}
	if (this.useLinkIntrospection) {
		this.propertieslink.update();
	}
}

/**
* update fonts in selection list from toolbar
*/
function Ktml_ui_updateFontFace() {
	var osel;
	if (osel = this.toolbar.font_type) {
		if (osel.disabled) {
			osel.disabled=false;
		}
		try {
			var fontname = this.edit.queryCommandValue(DECMD_GETFONTNAME).toLowerCase();
		} catch(e) {
		}
		if ((fontname !="") && (fontname != null)) {
			fcreatenew=true;
			var str = "/"+fontname+"/i";

			for (i=0; i<osel.options.length; i++) {
				var nm = new String(osel.options[i].value);
				if(nm==fontname || nm.match(str) || (nm.toLowerCase().indexOf(fontname.toLowerCase())!=-1)) {
					if(!osel.options[i].selected) {
						osel.options[i].selected = true;
					}
					fcreatenew = false;
				}
			}
			if(fcreatenew) {
				var ooption = document.createElement("OPTION");
				osel.options.add(ooption);
				ooption.text = fontname;
				ooption.name = fontname;
				ooption.value = fontname;					
				ooption.selected=true;
			}
		} else {
			osel.selectedIndex=0;
		}
	}
	return true;
}

/**
* updete font size in toolbar
*/
function Ktml_ui_updateFontSize() {
	var fssel;
	if(fssel = this.toolbar.font_size )  { //font size updating
		if(fssel.disabled) {
			fssel.disabled=false; 
		}
		try {
			var fs=this.edit.queryCommandValue(DECMD_GETFONTSIZE);
		} catch (e) {
			var fs = "";
		}
		if(fs!=null) {
			fscreatenew = true;
			for(i=0;i<fssel.options.length;i++) {
				if(fssel.options[i].value == fs ) {
					fssel.options[i].selected=true;
				}
				fscreatenew = false;
			}
			if(fscreatenew) {
				var ooption = document.createElement("OPTION");
				fssel.options.add(fsOption,0);
				fsoption.text = fs;
				fsoption.name = fs;
				fsoption.value = fs;
				fsoption.selected=true;
			}				
		} else {
			fssel.selectedIndex=0;
		}
	}
	return true;
}

function Ktml_ui_updateBlockFormat() {
	var fsblc;
	if(fsblc = this.toolbar.blockformat)  { //heading updating
		if(fsblc.disabled) {
			fsblc.disabled=false; 
		}
		try {
			fs=this.edit.queryCommandValue(DECMD_SETBLOCKFMT);
		} catch (e) {
			fs = "";
		}
		if(fs!=null && fs!="") {
			fsval = '<' + fs + '>';
			for(i=0;i<fsblc.options.length;i++) {
				if(fsblc.options[i].value == fsval || fsblc.options[i].innerHTML == fs) {
					fsblc.options[i].selected=true;
				} 
				
			}		
		} else {
			fsblc.selectedIndex=0;
		}
	}
	return true;
}

/**
*	Updates the style dropdown from toolbar with the current node style
*/
function Ktml_ui_updateStyle() {	
	toA = this.DOMProperty;
	var tmptst = false;
	var alsoUpd = this.toolbar.styler;
	if(!alsoUpd) {
		return;
	}
	if (toA.innerHTML!="") {
		for (var h=0;h<this.selectableNodes.length;h++) {
			if (this.selectableNodes[h].className != "") {
				for (var hh=0;hh<alsoUpd.options.length;hh++) {
					if (alsoUpd.options[hh].value == this.selectableNodes[h].className) {
						alsoUpd.selectedIndex = hh;
						tmptst = true;
						break;
					}
				}
			}
			if (tmptst) {
				break;
			}
		}
		if (!tmptst) {
			try {
				if (Ktml_ie) {
					var temp = this.selectableNodes[0].childNodes(0);
					if (temp.tagName == "SPAN" && temp.className != "") {
						for (var hh=0;hh<alsoUpd.options.length;hh++) {
							if (alsoUpd.options[hh].value == temp.className) {
								alsoUpd.selectedIndex = hh;
								tmptst = true;
								break;
							}
						}
					}
				}
			}
			catch(e) {
				//
			}
		}
		if (!tmptst) {
			alsoUpd.selectedIndex = 0;
		}
	} else {
		alsoUpd.selectedIndex = 0;
	}
}

//----------------------------------------------------------------------------------------------
//		Utilities
//----------------------------------------------------------------------------------------------

function Ktml_util_expandSelection_saveSel() {
	this.savedSel = this.edit.selection.createRange();
}
function Ktml_util_expandSelection() {
	var range, text, pe;
	this.savedSel.select();
	if (this.edit.selection.type != "Control") {
		range = this.edit.selection.createRange();
		text = range.htmlText;
		pe = range.parentElement();
		if (!text) {
			if (pe && pe.tagName == "FONT") {
				range.moveToElementText(pe);
				range.select();
			}
		}
	}
}

function Ktml_util_expandSelection_enclose(tag, attributes) { // mozilla only
	var range, text, pe, i;
	this.savedSel.select();
	if (this.edit.selection.type != "Control") {
		range = this.edit.selection.createRange();
		text = range.htmlText;
		pe = range.parentElement();
		if (!text) { // if no selection
			if (pe && pe.tagName == "FONT") { // modify an existing container font tag
				for (i=0; i<attributes.length; i+=2) {
					pe.setAttribute(attributes[i], attributes[i+1]);
				}
			} else { // or enclose parent''s contents in a font
				//range.collapse(true);
				var tmp = "<" + tag + " ";
				for (i=0; i<attributes.length; i+=2) {
					tmp += attributes[i] + "='" + attributes[i+1].replace("'", "\\'") + "'";
				}
				tmp += '>';
				tmp += pe.innerHTML;
				tmp += "</" + tag + ">";
				pe.innerHTML = tmp;
			}
		} else { // if selection
			if (Ktml_mozilla) { // like surroundContents, but works
				var fnt = this.edit.createElement("FONT");
				for (i=0; i<attributes.length; i+=2) {
					fnt.setAttribute(attributes[i], attributes[i+1]);
				}
				fnt.appendChild(range.extractContents());
				range.insertNode(fnt);
				this.cw.focus();
			}
		}
	}
}

function Ktml_util_insertRawHtml(sHTML){ 
	if(this.edit.selection.type == "Control") { 
		this.edit.selection.clear();
	}  
	this.edit.selection.createRange();
}

/** not used */
function Ktml_util_insertNodeHtml(nodeName) {
	this.edit.createElement(nodeName);
}

function Ktml_util_loadBody(text) {
	if (Ktml_mozilla) {
		this.edit.body.innerHTML = text;
	} else {
		this.edit.body.innerHTML = text.replace(/^<p/i, '<div');
	}
}

function Ktml_util_cleanHTMLContent() {
	if( this.displayMode == "CODE" || this.viewinvisibles ) {
		return false;
	}	
	var tmp = util_cleanHTMLContent(this.edit.body.innerHTML);
	this.toolbar.font_type.selectedIndex = 0;
	this.toolbar.font_size.selectedIndex = 0;
	this.toolbar.styler.selectedIndex = 0;
	this.util_loadBody(tmp);
	if(this.undo) {
		this.undo.addEdit();
	}
	
}

function Ktml_util_cleanHTMLContent2() {
	if( this.displayMode == "CODE" ) {
		return false;
	}	
	var tmp = util_cleanHTMLContent2(this.edit.body.innerHTML);
	this.util_loadBody(tmp);
}

/**
* check if ktml has the focus
*
* @param
*		obj - caller element
* 	mandatory - 
*/
function Ktml_util_checkFocus(obj, mandatory) {
	if(obj.className == "toolbaritem_disabled") {
		return false;
	}
	propname = obj.parentElement.id.replace(/Property_/, "");

	if (propname != this.name) {
		if (mandatory) {
			alert(KT_js_messages["not active context"] ? KT_js_messages["not active context"] : "This is not the active editor context! ");
			return false;
		}
	}
	this.cw.focus();
	return true;
}

if (Ktml_mozilla) {
function Ktml_util_saveSelection() {
}
}
if (Ktml_ie) {
function Ktml_util_saveSelection() {
	if (this.edit.selection.type != 'None') {
		this.svdSelection = this.edit.selection.createRange();
		return true;
	}
}
}

if (Ktml_mozilla) {
function Ktml_util_restoreSelection() {
}
}
if (Ktml_ie) {
function Ktml_util_restoreSelection() {
try {
	if (this.svdSelection) {
		this.svdSelection.select();
		this.svdSelection = null;
	}
}
catch(e) {
}
}
}
//----------------------------------------------------------------------------------------------
//		Application logic
//----------------------------------------------------------------------------------------------

function Ktml_logic_isEditable(node) {
	switch (node.nodeName) {
    	case 'TD':
		case 'TR':
		case 'TABLE':
		case 'IMG':
			return true;
		default: 
			return false;
	}
}

/**
* DOM path introspection
* 
* @param 
*	toDraw - to draw the navbar
*	toCollapse - collapse selection
*/
if (Ktml_mozilla) {
function Ktml_logic_updateDOMHierarchy(toDraw, toCollapse) {
		
	var toUpd = this.DOMProperty;
	toUpd.innerHTML = "";
	var _Range = this.edit.selection.createRange();

	if (toCollapse) {
		if (this.edit.selection.type == "Text") {
		  _Range.collapse(true);
		}
	}
		
	// empty array
	while (this.selectableNodes.length) {
		this.selectableNodes.pop();
	}

	var startElement = null;
	var cnt = 0;
	if (this.edit.selection.type == "Text" || this.edit.selection.type == "None") {

		var tmpprs = this.logic_getSelectedNode();
		
		var cElement = "&nbsp;";
		if (tmpprs == this.edit.body || tmpprs.nodeType == 3) {
			return; 
		}
		while (tmpprs && (tmpprs.tagName != "HTML") && tmpprs != this.edit.body) {
		//	if (this.logic_isEditable(tmpprs)) {
				cElement = (' <a class="tagitem" href="javascript:ktml_'+this.name+'.logic_domSelect('+cnt+', 2, 0);">'+tmpprs.tagName+'</a>&nbsp;>&nbsp;') + cElement;
				this.selectableNodes.push(tmpprs); 
				cnt++ ;
		//	}
			tmpprs = tmpprs.parentNode;
		}
		if(toDraw) {
			toUpd.innerHTML = cElement.replace(/>&nbsp;&nbsp;$/, "");
			toUpd.innerHTML+='&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:ktml_'+this.name+'.logic_removeTag(0);">' + (KT_js_messages["Remove Tag"]?KT_js_messages["Remove Tag"]:"Remove Tag") + '</a>';
		}
	}
}
}

if (Ktml_ie) {
function Ktml_logic_updateDOMHierarchy(toDraw, toCollapse) {
		var toUpd = this.DOMProperty;
		toUpd.innerHTML = "";
		//this.util_restoreSelection();
		var _Range = this.edit.selection.createRange();
		if (toCollapse) {
			if (this.edit.selection.type == "Text") {
				_Range.collapse();
			}
		}
		
		var tmpprs = this.logic_getSelectedNode();
		while (this.selectableNodes.length) {
			this.selectableNodes.pop();
		}
		
		var cnt = 0;
		if (this.edit.selection.type == 'Text' || this.edit.selection.type == 'None') {
			var cElement = "&nbsp;";
			if (tmpprs == this.edit.body) {
				return; 
			}
			if (tmpprs.tagName == 'TD') {
				//tmpprs.style.width="10px";
			}
			while (tmpprs && (tmpprs.tagName != "HTML") && tmpprs != this.edit.body) {
				if (tmpprs.tagName!="BODY") {
					if (tmpprs.tagName == "TABLE" || tmpprs.tagName == "IMG") {
						cElement = (' <A class=tagitem href="javascript:ktml_'+this.name+'.logic_domSelect('+cnt+', 2, 0);">'+tmpprs.tagName+'</A>&nbsp;>&nbsp;') + cElement;
					} else {
						cElement = (' <A class=tagitem href="javascript:ktml_'+this.name+'.logic_domSelect('+cnt+', 1, 0);">'+tmpprs.tagName+'</A>&nbsp;>&nbsp;') + cElement;
					}
				}
				this.selectableNodes.push(tmpprs); cnt++ ;
				tmpprs = tmpprs.parentElement;
			}
			if(toDraw) {
				tmp = cElement.replace(/>&nbsp;&nbsp;$/, "");
				if (tmp.indexOf("logic_domSelect") > 0) {
					tmp += '&nbsp;&nbsp;&nbsp;&nbsp; <A href="javascript:ktml_'+this.name+'.logic_removeTag(0);">'+(KT_js_messages["Remove Tag"]?KT_js_messages["Remove Tag"]:"Remove Tag")+'</A>';
				}
				if (toUpd.innerHTML != tmp) {
					//toUpd.contentEditable = true;
					toUpd.innerHTML = tmp;
					//toUpd.contentEditable = false;
				}
			}
			return; 
		} else if (this.edit.selection.type == "Control" ) {
			var cElement = "&nbsp;";
			if (tmpprs == this.edit) {
				return; 
			}
			while (tmpprs && (tmpprs.tagName != "HTML") && tmpprs != this.edit.body) {
				if (tmpprs.tagName!="BODY") {
					if (tmpprs.tagName == "TABLE" || tmpprs.tagName == "IMG") {
						cElement = (' <a class="tagitem" href="javascript:ktml_'+this.name+'.logic_domSelect('+cnt+', 2, 0);">'+tmpprs.tagName+'</a>&nbsp;>&nbsp;') + cElement;
					} else {
						cElement = (' <a class="tagitem" href="javascript:ktml_'+this.name+'.logic_domSelect('+cnt+', 1, 0);">'+tmpprs.tagName+'</a>&nbsp;>&nbsp;') + cElement;
					}
				}
				this.selectableNodes.push(tmpprs); cnt++ ;
				tmpprs = tmpprs.parentElement;
			}
			if(toDraw) {
				tmp = cElement.replace(/>&nbsp;&nbsp;$/, "");
				tmp += '&nbsp;&nbsp;&nbsp;&nbsp; <A href="javascript:ktml_'+this.name+'.logic_removeTag(0);">Remove Tag</A>';
				if (toUpd.innerHTML != tmp) {
					//toUpd.contentEditable = true;
					toUpd.innerHTML = tmp;
					//toUpd.removeAttribute('contentEditable');
					//toUpd.contentEditable = false;
				}
			}
		}
		return true;
}
}


/**
* returns the selected node non text
*/
if (Ktml_mozilla) {
function Ktml_logic_getSelectedNode() {
		var tmpprs = this.edit.selection.focusNode; // in this node selection starts
		if(tmpprs.nodeType == 3) {  // if is text
			tmpprs = tmpprs.parentNode;
		} else {
			tmpprs = tmpprs.childNodes[this.edit.selection.anchorOffset];
			if(!tmpprs || tmpprs.nodeType == 3) { // textnode
				tmpprs = this.edit.selection.anchorNode;
			}
		}
		
		return tmpprs;
}
}

if (Ktml_ie) {
function Ktml_logic_getSelectedNode() {
	var _Range = this.edit.selection.createRange();
		
	startElement = null;
	if (this.edit.selection.type == "Text" || this.edit.selection.type == "None") {
		for( var i=this.selectableNodes.length-1; i>=0; i--) {
			var tmpelement = this.selectableNodes[i];
			if (tmpelement && tmpelement.innerHTML != "") {
				var tmprng = this.edit.body.createTextRange();
				try {
					tmprng.moveToElementText(tmpelement);
				} catch(e) {
				};
				if (_Range.isEqual(tmprng)) {
					startElement = tmpelement;
				}
			}
		}				
	}
	
	var cnt = 0;
	var tmpprs;
	if (this.edit.selection.type == 'Text' || this.edit.selection.type == 'None') {
		if (startElement == null) {
			tmpprs = _Range.parentElement();
		} else {
			tmpprs = startElement;
		}
	} else if (this.edit.selection.type == "Control" ) {
		if (startElement == null) {
			tmpprs = _Range.commonParentElement();
		} else {
			tmpprs = startElement;
		}
	}
	return tmpprs;
}
}

/**
* Selects a node into the dom (programatic)
*
* @param
*		o -	index in selectableNodes array (the path to root from the current selection)
*		ind -	k
*		collapse - to collapse selection after
*/
function Ktml_logic_domSelect(o,ind, collapse) {

	this.selectableNodes = this.selectableNodes.reverse();
	while(o){
		this.selectableNodes.pop();
		o--;
	}
	this.selectableNodes = this.selectableNodes.reverse();
	
	if (ind == 1) {
		try {
			tmp1 = this.edit.body.createTextRange();
			tmp1.moveToElementText(this.selectableNodes[o]);
			tmp1.select();
			//this.util_saveSelection();
			this.logic_updateDOMHierarchy(true, 0);
			if (this.useIntrospection) {
				this.properties.update();
			}
			if (this.useLinkIntrospection) {
				this.propertieslink.update();
			}
		} catch(e) {
		}
	} else if (ind == 2) { 
		tmp1 = this.edit.body.createControlRange();
		tmp1.addElement(this.selectableNodes[o]);
		tmp1.select();
		//this.util_saveSelection();
		this.logic_updateDOMHierarchy(true, 0);
		if (this.useIntrospection) {
			this.properties.update();
		}
		if (this.useLinkIntrospection) {
			this.propertieslink.update();
		}
	}
	this.cw.focus();	
}

function Ktml_logic_doFormat(what) {
	var toundo = true;
  switch (what) {
    case 'Cut':
			try {
				this.edit.execCommand(DECMD_CUT,OLECMDEXECOPT_DODEFAULT, null);
			} catch(e) {
				if (confirm('Unprivileged scripts cannot invoke the Cut, Copy, and Paste commands!\nPress OK to open a technical note window with possible explanations to your problem. Press Cancel to return to the editor.')) {
					var wnd = window.open('http://mozilla.org/editor/midasdemo/securityprefs.html', 'securityprefs', '');
					wnd.focus();
				}
			}
			break;
			
    case 'Copy':
			try {
				this.edit.execCommand(DECMD_COPY,OLECMDEXECOPT_DODEFAULT, null);
			} catch(e) {
				if (confirm('Unprivileged scripts cannot invoke the Cut, Copy, and Paste commands!\nPress OK to open a technical note window with possible explanations to your problem. Press Cancel to return to the editor.')) {
					var wnd = window.open('http://mozilla.org/editor/midasdemo/securityprefs.html', 'securityprefs', '');
					wnd.focus();
				}
			}
			break;
    case 'Paste':
			try {
				this.edit.execCommand(DECMD_PASTE,OLECMDEXECOPT_DODEFAULT, null);
			} catch(e) {
				if (confirm('Unprivileged scripts cannot invoke the Cut, Copy, and Paste commands!\nPress OK to open a technical note window with possible explanations to your problem. Press Cancel to return to the editor.')) {
					var wnd = window.open('http://mozilla.org/editor/midasdemo/securityprefs.html', 'securityprefs', '');
					wnd.focus();
				}
			}
			break;
    case 'toggleInvisibles': 
			this.logic_toggleInvisibles();	
			break;
	case 'toggleEditMode':
			if (this.displayMode=='CODE') {
				this.swapToMode='RICH';
			} else {
				this.swapToMode='CODE';
			}
			this.logic_setDisplayMode();
			return;
	case 'FontName':
    		if(Ktml_ie) {
    			if(this.edit.selection.type == "Control" && this.selectableNodes[0].nodeName == 'TABLE') {
    				this.selectableNodes[0].style.fontFamily = arguments[1];
    				break;
    			}
    		} else {
    			var rng = this.edit.selection.getRangeAt(0);
    			if(rng.collapsed && rng.startContainer.nodeType == 3) {
    				var st = rng.startContainer;
    				var offset = rng.startOffset;
    				var i=offset;
    				while(i>0 && st.nodeValue.charAt(i) != ' ') i--;
    				rng.setStart(st, i);
    				i = offset;
    				while(i<st.length && st.nodeValue.charAt(i) != ' ') i++;
    				rng.setEnd(st, i);
    			}
    		}
			this.edit.execCommand(DECMD_SETFONTNAME, OLECMDEXECOPT_DODEFAULT, arguments[1]);
			this.ui_updateFontFace();
			break;
    case 'SelectAll':  this.edit.execCommand(DECMD_SELECTALL,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'FontSize': 
    		if (Ktml_mozilla) {
    			var selNode = this.logic_getSelectedNode();
    			if(selNode.nodeName == 'SPAN') {
    				selNode.innerHTML = '<font size="' + parseInt(arguments[1]) + 
    						'">' + selNode.innerHTML + '</font>';
    				var sel = this.edit.selection;
    				sel.collapse(selNode, 0);
    				sel.extend(selNode, 1);
    				break;
    			} else if(selNode.nodeName == 'FONT') {
    				selNode.setAttribute('size', parseInt(arguments[1]));
    				break;
    			}
	    	}
			var tmp = arguments[1];
			if(tmp == '') {
				//RST:  IE hack
				tmp = 3;
			}
    		this.edit.execCommand(DECMD_SETFONTSIZE, OLECMDEXECOPT_DODEFAULT, parseInt(arguments[1]));
    		break;
    case 'Bold': this.edit.execCommand(DECMD_BOLD,OLECMDEXECOPT_DODEFAULT, null);
			break;
    case 'Italic': this.edit.execCommand(DECMD_ITALIC,OLECMDEXECOPT_DODEFAULT, null);
			break;
    case 'Underline': this.edit.execCommand(DECMD_UNDERLINE,OLECMDEXECOPT_DODEFAULT, null);
			break;
    case 'JustifyLeft': this.edit.execCommand(DECMD_JUSTIFYLEFT,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'JustifyCenter': this.edit.execCommand(DECMD_JUSTIFYCENTER,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'JustifyRight': this.edit.execCommand(DECMD_JUSTIFYRIGHT,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'JustifyFull': this.edit.execCommand(DECMD_JUSTIFYFULL,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'InsertOrderedList':  this.edit.execCommand(DECMD_ORDERLIST,OLECMDEXECOPT_DODEFAULT, null);
			this.logic_cleanList();
			break;
    case 'InsertUnorderedList': this.edit.execCommand(DECMD_UNORDERLIST,OLECMDEXECOPT_DODEFAULT, null);
			this.logic_cleanList();
			break;
    case 'Indent': this.edit.execCommand(DECMD_INDENT,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'Outdent': this.edit.execCommand(DECMD_OUTDENT,OLECMDEXECOPT_DODEFAULT, null);break;
    case 'InsertHorizontalRule':this.edit.execCommand(DECMD_INSERTHR,OLECMDEXECOPT_DODEFAULT, null);
			//this.util_insertNodeHtml("HR");
			break;
    case 'InsertImage': this.edit.execCommand(DECMD_IMAGE,OLECMDEXECOPT_DONTPROMPTUSER,arguments[1]);break;
    case 'FindText':
			if (Ktml_mozilla) {
				//window.find();
			}
			//this.edit.execCommand(DECMD_FINDTEXT, OLECMDEXECOPT_DODEFAULT, arguments[1]);
			break;
    case 'CleanHTML':
    		this.util_cleanHTMLContent();
			return;
    case 'RemoveLink':
    		this.logic_removeTag();
			break;
    case 'InsertLink':
    		this.logic_InsertLink('href');
			break;
    case 'BackColor': 
			this.logic_openPalette(this.toolbar.bgcolor, 'b')
			return;
	case "ForeColor":
			//this.edit.execCommand("forecolor", false, document.getElementById('SelColor').value);
			this.logic_openPalette(this.toolbar.fgcolor, 'f')
			break;
    case 'Undo':
    	if(this.undo) {
    		this.undo.undo();
    		toundo = false;
    	} else {
		this.logic_recCommand(DECMD_UNDO);
	}
    	break;
    case 'Redo':
    	if(this.undo) {
    		this.undo.redo();
    		toundo = false;
    	} else {
		this.logic_recCommand(DECMD_REDO);
	}
	break;
    case 'FontAdvanced':this.edit.execCommand(DECMD_FONT,OLECMDEXECOPT_PROMPTUSER);break;
    case 'InsertStyle': this.logic_InsertStyle(arguments[1]); break;
	case 'InsertHeading': this.logic_InsertHeading(arguments[1]); break;
  }
	if(this.undo && toundo) {
		this.undo.addEdit();
	}
	this.cw.focus();
	this.ui_setButtonState();
}

function Ktml_logic_InsertTable(counter) {
	if(this.displayMode == "CODE" ) {
		return false;
	}	
	this.undo.addEdit();
	util_openwnd("_instable", NEXT_ROOT+"includes/ktedit/popups/instable."+Ktml_ext+"?ktmlid="+this.pageId + '&counter=' + counter, 250, 150);
	this.undo.addEdit();
}

function Ktml_logic_openInsertLink(){
	if(this.displayMode == "CODE" ) {
		return false;
	}	
	var pth = this.pathToFileDir;
}

/**
*	Opens the insert image dialog. If there is an image selected, 
* its path or url is sent to dirbrowser.
*
*	@param 
*		counter ?
*/
function Ktml_logic_openInsertImage(counter){
	if(this.displayMode == "CODE" ) {
		return false;
	}	
	
	var imgPath = '';
	var imgBorder = '';
	var imgAlign = '';
	var imgAlt = '';
	if (this.selectableNodes[0] && this.selectableNodes[0].nodeName == 'IMG') {
		imgPath = this.selectableNodes[0].getAttribute('src');
		imgBorder = this.selectableNodes[0].getAttribute('border');
		imgAlign = this.selectableNodes[0].getAttribute('align');
		imgAlt = this.selectableNodes[0].getAttribute('alt');
		var ix = imgPath.indexOf('?');
		if(ix != -1) {
			imgPath = imgPath.substr(0, ix);
		}
	}
//	var pth = this.pathToImageDir;
	util_openwnd("uploadImage",
			NEXT_ROOT+"includes/ktedit/dirbrowser."+Ktml_ext+"?mode=img&submode=img&ktmlid="+this.pageId 
			+ "&counter=" + counter 
			+ "&imgpath=" + imgPath
			+ "&imgborder=" + imgBorder
			+ "&imgalign=" + imgAlign
			+ "&imgalt=" + imgAlt
			, 710, 580);
	if(this.undo) {
		this.undo.addEdit();
	}
}

function Ktml_logic_openPalette(o, cmd){	
	if(this.displayMode == "CODE" ) {
		return false;
	}
	if (this.cpalette.isVisible) {
		this.cpalette.setVisible(false);
	} else {
		var p = lay_getAbsolutePos(o), col;
		this.cpalette.showAt(p.x, p.y + o.offsetHeight);
		this.util_expandSelection_saveSel();
		if (cmd == 'b') {
			col = intbgr2hexrgb(this.edit.queryCommandValue(DECMD_GETBACKCOLOR));
			this.cpalette.setSelected(col);
			this.cpalette.setAction("if(Ktml_ie){this.owner.util_expandSelection();this.owner.edit.execCommand(DECMD_GETBACKCOLOR, false, this.textfld.value)} else{this.owner.util_expandSelection_enclose('FONT',['style', 'background-color:' + ColorPalette_transformColor(this.textfld.value) + ';'])}");
		} else if (cmd == 'f') { // this one doesn''t need the hack
			col = intbgr2hexrgb(this.edit.queryCommandValue('forecolor'));
			this.cpalette.setSelected(col);
			this.cpalette.setAction("this.owner.edit.execCommand(DECMD_GETFORECOLOR, false, this.textfld.value)");
		}
		this.cpalette.setVisible(true);
	}
	//this.undo.addEdit();
}

function Ktml_logic_InsertHeading(hName) {
	if(hName != '') {
		this.edit.execCommand(DECMD_SETBLOCKFMT,OLECMDEXECOPT_DONTPROMPTUSER,hName);
	} else {
		this.edit.execCommand(DECMD_REMOVEFORMAT,OLECMDEXECOPT_DONTPROMPTUSER,null);	
	}
}

/**
* Adds a style attribute to the selected node or surrounds the selection with a
* styled span (if it is a text). Text selection must start and end into the same 
* container.
*
* undoable in do_format
*
* @param
*	stName - style name
*/
if(Ktml_mozilla) {
function Ktml_logic_InsertStyle(stName){
	var sel = this.edit.selection.createRange();
	if(sel.startContainer == sel.endContainer) {
		var container = sel.startContainer;
		if(container.nodeType == 3) { // text node
			if(stName != '') {
				if(sel.startOffset < sel.endOffset) { // selection - set new style
					var span = this.edit.createElement('span');
					span.setAttribute('class', stName);
			
					span.appendChild(sel.extractContents());
					sel.insertNode(span);
				} else { // no selection so search for a styled span and try to change it
					var pparent = container.parentNode;
					if(pparent.nodeName == 'SPAN') {
						if(sel.endOffset != pparent.innerHTML.length) {
							pparent.setAttribute('class', stName);
						} else {
							// if the caret is at the end of span create another with the new style
							var span = this.edit.createElement('span');
							span.setAttribute('class', stName);	
							var ppparent = pparent.parentNode;
							if( ppparent.lastChild == pparent) {
								ppparent.appendChild(span);
							} else {
								ppparent.insertBefore(span, pparent.nextSibling);
							}
							sel.setEnd(span, 0);
							sel.setStart(span, 0);
							sel.collapse(false);
						}
					} else { // select the container and apply the style to it
						container = this.logic_getSelectedNode();
						// set new content
						container.innerHTML = "<span class='" + stName + "'>" + container.innerHTML + "</span>";
						sel.collapse(false);
					}
				}
			} else { // stName == ''
				// get span parent
				var pparent = this.utils_getParent(container, 'SPAN');
				if(pparent != null) {
					pparent.outerHTML = pparent.innerHTML;
				} else {
					pparent = this.utils_getParent(container, 'FONT');
					if(pparent != null) {
						pparent.outerHTML = pparent.innerHTML;
					}
				}
			}
		} else { // not a text node
			container = this.logic_getSelectedNode();
			if(stName != '') {
				// RST remove 
				//if (container.tagName == "SPAN") {
				container.setAttribute('class', stName);
				//}
			} else {
				container.removeAttribute('class');
			}
		}
		
	} else { // startContainer != endContainer
		var pparent = null;
		if(sel.startContainer.childNodes[sel.startOffset] == 'SPAN') {
			pparent = sel.startContainer.parentNode;
		}
		if(pparent) {
			if(stName != '') {
				pparent.setAttribute('class', stName);
			} else {
				pparent.outerHTML = pparent.innerHTML;
			}
		} else if(stName != '') {
			var span = this.edit.createElement("span");
			span.setAttribute("class", stName);

			if(sel.startContainer.nodeType == 3 && 
				sel.startOffset == 0 &&
				sel.startContainer.parentNode.nodeName != 'BODY'
			) {
				// include the parent of this node or it will get messy
				sel.setStartBefore(sel.startContainer.parentNode);
			} else if(sel.endContainer.nodeType == 3 && 
				sel.endOffset == sel.endContainer.length &&
				sel.endContainer.parentNode.nodeName != 'BODY'
			) {
				sel.setEndAfter(sel.endContainer.parentNode);
			}
			this.edit.selection.collapseToStart();
			sel.surroundContents(span);
			sel.select();
			this.cw.focus();
		}
	}
	return true;
}
}

if (Ktml_ie) {
// prove IE is varza
function Ktml_logic_InsertStyle(stName){
	var dbg = false;
	if(dbg)alert(stName);
	
	if (this.edit.selection.type == "Control") {
		this.edit.selection.createRange().item(0).className = stName;
		return;
	}

	var done, applyStyleTo = null, sel, text, text2, pe;
	sel = this.edit.selection.createRange();
	text = sel.htmlText;
	pe = sel.parentElement();
	
	trim_text = text.replace(/\s\s*/gim, " ").trim();
	trim_pe = pe.outerHTML.replace(/\s\s*/gim, " ").trim();
	
	//                               [   tag  ]   [attrs] [in]   [tag ]
	match1_text = trim_text.match(/^<([^>|\s]*)\s*([^>]*)>(.*)<\/([^>]*)>$/i);
	match2_text = trim_text.match(/^<span class=([^>]*)>(.*)<\/span>$/i);
	match1_pe = trim_pe.match(/^<([^>|\s]*)\s*([^>]*)>(.*)<\/([^>]*)>$/i);
	match2_pe = trim_pe.match(/^<span class=([^>]*)>(.*)<\/span>$/i);
	
	if(dbg)alert("X" + text + "X\n\nX" + pe.outerHTML + "X");
	if(dbg)alert(match1_text + "\n\n" + match2_text);
	if(dbg)alert(match1_pe + "\n\n" + match2_pe);

	// a bug in TextRange.parentElement()
	// RST - euristic
	if (trim_pe.length < trim_text.length) {
		if(dbg)alert("wrong parent element retrival");
		if(stName) {
			if(match2_text) {
				pe.parentNode.outerHTML = 
					"<span class=" + stName + ">" + match1_text[3] + "</span>";
			} else if(match2_pe) {
				pe.outerHTML = "<span class=" + stName + ">" + pe.innerHTML + "</span>";
			} else {
				pe.parentNode.outerHTML = 
					"<" + match1_text[1] + " " + match1_text[2] + ">" +
					"<span class=" + stName + ">" + match1_text[3] + "</span>" +
					"</" + match1_text[1] + ">";
			}
		} else {
			if(match2_text) {
				pe.parentNode.outerHTML =  pe.parentNode.innerHTML;
			} else if(match2_pe) {
				pe.outerHTML = pe.innerHTML;
			}
		}
		return;
	}
	
	// a bug in pasteHTML() 
	// RST - euristic
	if(trim_pe.length == trim_text.length && match1_text[1] == match1_pe[1]) {
		if(dbg)alert("avoid pasteHTML");
		if(stName) {
			pe.innerHTML = "<span class=" + stName + ">" + match1_text[3] + "</span>";
		} else {
			
		}
		return;
	}

	// a tag - not span class=
	if (match1_text && match1_text[1] == match1_text[4] && !match2_text) {
		if(dbg)alert("a tag - not span class=");
		if(dbg)alert(match1_text[1] + "\n" + match1_text[3] + "\n" + sel.htmlText);
		if (!stName) {
		} else {
			try {
				// RST: hope to be an improvement
				var toPaste = "<span class=" + stName + ">" + text + "</span>";
				// use the same retival method
				var allText = pe.innerHTML.replace(/\s\s*/gim, " ");
				allText = allText.replace(match1_text[3], toPaste);	
				pe.innerHTML = allText;

				/*
				sel.pasteHTML(
					"<" + match1_text[1] + " " + match1_text[2] + ">" +
					"<span class=" + stName + ">" + match1_text[3] + "</span>" +
					"</" + match1_text[1] + ">"
				);
				*/
			} catch (e) {
				// after paste will be a valid doc no more
			}
		}
		return;
	}
	// span class=
	if (match2_text && match2_pe) {
		if(dbg)alert("span class=");
		if (!stName) {
			pe.outerHTML = match2_text[2];
		} else {
			pe.className = stName;
		}
		return;
	}
	// something selected - buggy
	if (!match1_text && !match2_text && text) {
		if(dbg)alert("something selected - buggy");
		if (!stName) {
		} else {
			sel.pasteHTML(
				"<span class=" + stName + ">" + text + "</span>"
			);
		}
		return;
	}
	// nothing selected contained in a tag - not span class=
	if (!text && match1_pe && match1_pe[1] == match1_pe[4] && !match2_pe) {
		if(dbg)alert("nothing selected contained in a tag - not span class=");
		if (!stName) {
			if(pe.nodeName == 'FONT') {
				if(pe.parentNode.nodeName == 'SPAN') {
					pe.parentNode.outerHTML = pe.parentNode.innerHTML;
				}
			}
		} else {
			if(pe.nodeName == 'FONT') {
				if(pe.parentNode.nodeName == 'SPAN') {
					pe.parentNode.outerHTML = "<span class=" + stName + ">" + pe.outerHTML + "</span>";
				} else {
					pe.outerHTML = 
						"<span class=" + stName + ">" + pe.outerHTML + "</span>";
				}
			} else {
				pe.innerHTML = 
					"<span class=" + stName + ">" + match1_pe[3] + "</span>";
			}
		}
		return;
	}
	// nothing selected contained in a span class=
	if (!text && match2_pe) {
		if(dbg)alert("nothing selected contained in a span class=");
		if (!stName) {
			pe.outerHTML = match2_pe[2];
		} else {
			pe.className = stName;
		}
		return;
	}
}
}
/**
* Create an object to be inserted into the page ( like img, mp3, ...)
* Called from dirbrowser.Ktml_ext
*
* @param 
*		arr - array of parameters
*/
function Ktml_logic_InsertImage(arr) {

	function addInit(doc) {
		if (Ktml_mozilla) {
			d = doc.createElement("OBJECT");
		} else {
			d = "<OBJECT ";
		}
		return d;
	}
	function addParam(doc, d, n, v) {
		if (!n) n = "";
		if (!v) v = "";
		var t;
		if (Ktml_mozilla) {
			t = doc.createElement("param");
			t.setAttribute("name", n);
			t.setAttribute("value", v);
			d.appendChild(t);
		} else {
			d += "<PARAM NAME='" + n + "' VALUE='" + v + "'/>"
		}
		return d;
	}
	function addAttr(d, n, v) {
		if (Ktml_mozilla) {
			d.setAttribute(n.toUpperCase(), v);
		} else {
			d += n + "=' " + v + "' ";
		}
		return d;
	}
	function addCC(d, cls, cb) {
		if (Ktml_mozilla) {
			d.setAttribute("CLASSID", cls);
			d.setAttribute("CODEBASE", cb);
		} else {
			if (cls) {
				d += " CLASSID='" + cls + "' ";
			}
			if (cb) {
				d += " CODEBASE='" + cb + "' "
			}
			d += ">";
		}
		return d;
	}
	function addParams(doc, d, p) {
		var de;
		if (Ktml_mozilla) {
			de = doc.createElement("embed");
			de.setAttribute("PLUGINSPAGE", p);
		} else {
			de = "<EMBED PLUGINSPAGE='" + p + "' ";
		}
		for (var i=3; i<arguments.length; i+=2) {
			arguments[i] = arguments[i].toLowerCase();
			d = addParam(doc, d, arguments[i], arguments[i+1]);
			de = addAttr(de, arguments[i], arguments[i+1]);
		}
		if (Ktml_mozilla) {
			d.appendChild(de);
		} else {
			d += de + "</EMBED></OBJECT>";
		}
		return d;
	}


var args=new Array();
var d = addInit(this.edit);
if (arr != null) {
  	strWidth = '';
  	strHeight = '';
	sHTML = '';
  	if( arr["ImgWidth"] != "" && arr["ImgWidth"] != "0" ) {
  		strWidth = 'width="' + arr["ImgWidth"] +'"';
  	}
  	if( arr["ImgHeight"] != "" && arr["ImgHeight"] != "0" ) {
  		strHeight = ' height="' + arr["ImgHeight"] +'"';
  	}
	if( arr["ImgWidth"] == "" || arr["ImgWidth"] == "0" ) {
		arr["ImgWidth"] = "";
	}
	if( arr["ImgHeight"] == "" || arr["ImgHeight"] == "0" ) {
		arr["ImgHeight"] = "";
	}
	if (arr["ImgUrl"].match(/.mov$/i) || arr["ImgUrl"].match(/.qt$/i)) {
		d = addCC(d, "clsid:02BF25D5-8C17-4B23-BC80-D3488ABDDC6B", "http://www.apple.com/qtactivex/qtplugin.cab#version=6,0,2,0");
		d = addParams(this.edit, d, "http://www.apple.com/quicktime/download/index.html", 
			"width", arr["ImgWidth"],
			"height", arr["ImgHeight"],
			"src", arr["ImgUrl"],
			"autoplay", "TRUE",
			"CONTROLLER", "TRUE",
			"TARGET", "MYSELF",
			"CACHE", "TRUE"
		);
		sHTML += '<OBJECT CLASSID="clsid:02BF25D5-8C17-4B23-BC80-D3488ABDDC6B" WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '" CODEBASE="http://www.apple.com/qtactivex/qtplugin.cab#version=6,0,2,0">\n';
		sHTML += '<PARAM NAME="SRC" VALUE="' + arr["ImgUrl"] + '">\n';
		sHTML += '<PARAM NAME="AUTOPLAY" VALUE="true">\n';
		sHTML += '<PARAM NAME="CONTROLLER" VALUE="TRUE">\n';
		sHTML += '<PARAM NAME="target" VALUE="myself">\n';
		sHTML += '<EMBED WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '" TARGET=MYSELF SRC="' + arr["ImgUrl"] + '" PLUGINSPAGE="http://www.apple.com/quicktime/download/index.html" CONTROLLER=TRUE CACHE="true">\n';
		sHTML += '</OBJECT>\n';
	} else if (
			arr["ImgUrl"].match(/.rm$/i) || 
			arr["ImgUrl"].match(/.ram$/i) || 
			arr["ImgUrl"].match(/.ra$/i) 
		) {
		d = addCC(d, "clsid:CFCDAA03-8BE4-11cf-B84B-0020AFBBCCFA", "");
		d = addParams(this.edit, d, "http://www.real.com/player/index.html", 
			"WIDTH", arr["ImgWidth"],
			"HEIGHT", arr["ImgHeight"],
			"SRC", arr["ImgUrl"],
			"AUTOSTART", "FALSE",
			"CONTROLS", "ImageWindow"
		);
		sHTML += '<OBJECT CLASSID="clsid:CFCDAA03-8BE4-11cf-B84B-0020AFBBCCFA" WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '">\n';
		sHTML += '<PARAM NAME="src" VALUE="' + arr["ImgUrl"] + '">\n';
		sHTML += '<PARAM NAME="autostart" VALUE="false">\n';
		sHTML += '<PARAM NAME="controls" VALUE="ImageWindow">\n';
		sHTML += '<EMBED WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '" SRC="' + arr["ImgUrl"] + '" PLUGINSPAGE="http://www.real.com/player/index.html" CONTROLS="ImageWindow" AutoStart="false">\n';
		sHTML += '</OBJECT>\n';
	} else if (arr["ImgUrl"].match(/.swf$/i)) {
		d = addCC(d, "clsid:D27CDB6E-AE6D-11cf-96B8-444553540000", "http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#4,0,0,0");
		d = addParams(this.edit, d, "http://www.macromedia.com/go/getflashplayer", 
			"width", arr["ImgWidth"],
			"height", arr["ImgHeight"],
			"src", arr["ImgUrl"],
			"quality", "high"
		);
		sHTML += '<OBJECT CLASSID="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#4,0,0,0">\n';
		sHTML += '<PARAM NAME="SRC" VALUE="' + arr["ImgUrl"] + '">\n';
		sHTML += '</OBJECT>\n';
	} else if (
			arr["ImgUrl"].match(/.wmv$/i) || 
			arr["ImgUrl"].match(/.avi$/i) ||
			arr["ImgUrl"].match(/.mpe?g$/i)
		) {
		d = addCC(d, "clsid:22D6F312-B0F6-11D0-94AB-0080C74C7E95", "");
		d = addParams(this.edit, d, "http://www.microsoft.com/isapi/redir.dll?prd=windows&sbp=mediaplayer&ar=Media&sba=Plugin&", 
			"WIDTH", arr["ImgWidth"],
			"HEIGHT", arr["ImgHeight"],
			"SRC", arr["ImgUrl"],
			"FILENAME", arr["ImgUrl"],
			"AUTOSTART", "FALSE",
			"ANIMATIONATSTART", "FALSE",
			"SHOWCONTROLS", "FALSE"
		);
		sHTML += '<OBJECT CLASSID="clsid:22D6F312-B0F6-11D0-94AB-0080C74C7E95" WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '">\n';
		sHTML += '<PARAM NAME="FileName" VALUE="' + arr["ImgUrl"] + '">\n';
		sHTML += '<PARAM NAME="AutoStart" VALUE="false">';
		sHTML += '<EMBED WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '" SRC="' + arr["ImgUrl"] + '" PLUGINSPAGE="http://www.microsoft.com/isapi/redir.dll?prd=windows&sbp=mediaplayer&ar=Media&sba=Plugin&" SHOWCONTROLS="1" AutoStart="0" AnimationatStart="0"></EMBED>\n';
		sHTML += '</OBJECT>\n';
	} else if (arr["ImgUrl"].match(/.mp3$/i)) {
		d = addCC(d, "clsid:22D6F312-B0F6-11D0-94AB-0080C74C7E95", "");
		d = addParams(this.edit, d, "http://www.microsoft.com/isapi/redir.dll?prd=windows&sbp=mediaplayer&ar=Media&sba=Plugin&", 
			"WIDTH", arr["ImgWidth"],
			"HEIGHT", arr["ImgHeight"],
			"SRC", arr["ImgUrl"],
			"FILENAME", arr["ImgUrl"],
			"SHOWCONTROLS", "FALSE"
		);
		sHTML = '<OBJECT CLASSID="clsid:22D6F312-B0F6-11D0-94AB-0080C74C7E95" WIDTH="' + arr["ImgWidth"] + '" HEIGHT="' + arr["ImgHeight"] + '">\n';
		sHTML += '<PARAM NAME="FileName" VALUE="' + arr["ImgUrl"] + '">\n';
		sHTML += '<embed type="audio/mpeg" src="' + arr["ImgUrl"] + '" width="100" height="40" autostart="0" showcontrols="0"></embed>';
		sHTML += '</OBJECT>\n';			
	} else {
		sHTML +='<img src="' + arr["ImgUrl"] + '" alt="' + arr["AltText"] + '" '+ strWidth + strHeight +' vspace="' + arr["VerSpace"] + '" hspace="' + arr["HorSpace"] +  '"align="' + arr["ImgAlign"] +'"border="' + arr["ImgBorder"]+ '">';
		d = this.edit.createElement("IMG");
		//add random to the end of the image to force reload
		d.src = arr['ImgUrl'] + '?' + Math.random();
		if (arr["ImgWidth"]) {
			d.width = arr["ImgWidth"];
		} 
		if (arr["ImgHeight"]) {
			d.height = arr["ImgHeight"];
		}
		d.setAttribute("BORDER", arr["ImgBorder"]);
		d.vspace = arr["VerSpace"];
		d.align = arr["ImgAlign"];
		d.hspace = arr["HorSpace"];
		d.alt = arr['AltText'];
		
		if (this.displayMode == 'RICH') {
			var tmNd = util_insertNodeSel(this.edit, d); // was cw
			this.cw.focus();
			this.undo.addEdit();

			//This is a trick to remove the random parameter at the end of the source
			//ufortunatly it works only in IE
			// RST this trik is to be revised as it does not work on IE if there is a series of images
			/*
			if (tmNd) {
				if (tmNd.type != 'IMG') {
					tmNd = tmNd.getElementsByTagName('IMG')[0];
				}
				tmNd.src = arr['ImgUrl'];
			} */
			
			return;
		}
	}
	if (this.displayMode == 'RICH'){
		if (Ktml_mozilla) {
			util_insertNodeSel(this.edit, d); // was cw
		} else {
			var dd, tr;
			//dd = this.edit.createElement(d);
			tr = this.edit.selection.createRange();
			tr.pasteHTML(d);
		}
	}
}
this.undo.addEdit();
this.cw.focus();
}

/**
* switch display mode between code and wysiwyg
*/
function Ktml_logic_setDisplayMode(){
	var mode = this.swapToMode;
	
	if(mode == 'CODE'){
		//~ mozilla on windows 98, me and probably 95 crashes when returning from view source
		//~ so, we disable the button and give a warning for those unfortunate ones
		var a, i, s="", ua;
		ua = navigator.userAgent;
		if (Ktml_mozilla && 
			(ua.indexOf("Win95") != -1 || ua.indexOf("Win98") != -1 || ua.indexOf("Win 9x 4.90") != -1)
		) {
			a = this.selectableNodes;
			for (i=0; i<a.length; i++) {
				if (
					a[i].tagName == "TABLE" || a[i].tagName == "IMG" || a[i].tagName == "OBJECT"
				) {
					alert(KT_sprintf(KT_js_messages["move pointer"]?KT_js_messages["move pointer"]:"Please move the selection pointer outside the %s element to be able to switch to code view.", a[i].tagName));
					return;
				}
				s+= a[i].tagName + ", ";
			}
			//alert(s);
		}
		
		this.toolbar.swap.className = "toolbaritem_latched"; this.toolbar.swap.state = 2;
		this.textarea.style.display = "block";
		this.iframe.style.display = "none";
		if(this.viewinvisibles) {
			this.utils_setInvisibles(true);
		}
		if(this.edit.body.innerHTML != "") {
			this.textarea.value = hndlr_load(this.edit.body.innerHTML, mode);
		} else {
			this.textarea.value = "";
		}
		this.displayMode = "CODE";
		
		for(var i=0; i<this.toolbar.buttons.length; i++) {
			var pbtn = this.toolbar.buttons[i];
			if (pbtn.id != "html") {
				switch(pbtn.getAttribute('kttype')) {
					case "btn":
						if (this.viewinvisibles && pbtn.id == "toggleinvisible") {
							pbtn.classNmae = "toolbaritem_latched";
							pbtn.state = 1;
						} else {
							pbtn.className = "toolbaritem_disabled";
							pbtn.state = 0;
						}
						break;
					case "slc":
						pbtn.disabled = true;
						break;
					case "text":
						pbtn.disabled = true;
						break;
					case "checkbox":
						pbtn.disabled = true;
						break;
				}
			}
		}
		this.toolbar.font_type.disabled = true ;
		this.toolbar.font_size.disabled = true ;
		this.toolbar.blockformat.disabled = true ;
		this.toolbar.styler.disabled = true ;

		//this.DOMProperty.contentEditable = true;
		this.DOMProperty.innerHTML = "n/a in Source Mode";
		//this.DOMProperty.contentEditable = false;
		if (this.useIntrospection) {
			this.properties.clear();
		}
		if (this.useLinkIntrospection) {
			this.propertieslink.clear();
			this.propertieslink.showNone();
		}
		this.textarea.focus();
	} else { // mode = html
		this.toolbar.swap.className = "toolbaritem_flat"; this.toolbar.swap.state = 0;
		this.textarea.style.display = "none";
		this.iframe.style.display = "block";

		var toundo = false;
		if(this.textarea.value != this.edit.body.innerHTML && this.undo) {
			toundo = true;
		}

		if(this.textarea.value != "") {
			this.util_loadBody(hndlr_load(this.textarea.value, mode));
		} else {
			this.edit.body.innerHTML = "";
		}

		if(Ktml_mozilla) {
			this.edit.designMode='on';
		}
		
		// redo invisibles if selected
		if(this.viewinvisibles) {
			this.utils_setInvisibles(false);
		}
		
		if(toundo) {
			this.undo.addEdit();
		}
		
		this.cw.focus();
		this.displayMode = "RICH";

		
		for(var i=0;i<this.toolbar.buttons.length;i++) {
			var pbtn = this.toolbar.buttons[i];
			if (pbtn.id != "html") {
				switch(pbtn.getAttribute('kttype')) {
					case "btn":
						pbtn.className = "toolbaritem_flat"; pbtn.state = 1;
						break;
					case "slc":
						pbtn.disabled = false;
						break;
					case "text":
						pbtn.disabled = false;
						break;
					case "checkbox":
						pbtn.disabled = false;
						break;
				}
			}
		}
		this.toolbar.font_type.disabled = false ;
		this.toolbar.font_size.disabled = false ;
		this.toolbar.blockformat.disabled = false ;
		this.toolbar.styler.disabled = false ;
		
		//this.DOMProperty.contentEditable = true;
		this.DOMProperty.innerHTML = "";
		//this.DOMProperty.contentEditable = false;
		this.p_displayChanged();
	}
}

/**
*	Insert a link into document
*
* @param
*		prm - 
*/
function Ktml_logic_InsertLink(prm) {

	if (this.displayMode == "CODE") {
		return false;
	}
	
	this.cw.focus();
	var range = this.edit.selection.createRange();
	var usesecond = 0;	
	var inserted = 0;

	if(this.selectableNodes && this.selectableNodes[0]) {
		var tmpel = this.selectableNodes[0];
	} else {
		var tmpel = this.logic_getSelectedNode();
	}
	
	if(range.htmlText != "" && tmpel.tagName != 'HR') {
		this.undo.addEdit();
		if(Ktml_mozilla) {
				if(range.endContainer.nodeType == 3) {
					// link is made over a TEXT
					var text = range.startContainer.nodeValue;
					var i = range.startOffset;
					// ltrim
					while (text.charAt(i) == ' ') {
						i++;
					}
					range.setStart(range.startContainer, i);
					
					var i = range.endOffset;
					// rtrim
					while (text.charAt(i-1) == ' ') {
						i--;
					}
					range.setEnd(range.endContainer, i);
				} else {
					var nt = this.logic_getSelectedNode();
					if(nt.nodeName == 'A' && nt.getAttribute('name') && nt.getAttribute('name') != '') {
						if (confirm(KT_js_messages["Anchor2Link"] ? KT_js_messages["Anchor2Link"] : "Do you want to transform this anchor into a link? (the old Anchor will be deleted).")) {
							nt.removeAttribute('name');
						} else {
							return;
						}
					}
				}
			this.edit.execCommand(DECMD_HYPERLINK,OLECMDEXECOPT_DONTPROMPTUSER,'http://');
			this.undo.addEdit();
			usesecond = 0;
			inserted = 1;
		} else { //IE
			// leading and trailing spaces must be removed if selection is text		
			if(this.edit.selection.type == "Text" || this.edit.selection.type == "None") {
				var text = range.htmlText;
				if(text.match(/^[^<]*<hr.*$/i)) {
					// don't make link over hr 
					return;
				}
				
				var nt = this.logic_getSelectedNode();
				if(nt.nodeName == 'A' && nt.getAttribute('name') && nt.getAttribute('name') != '') {
					if (confirm(KT_js_messages["Anchor2Link"] ? KT_js_messages["Anchor2Link"] : "Do you want to transform this anchor into a link? (the old Anchor will be deleted).")) {
						nt.removeAttribute('name');
						nt.removeAttribute('NAME');
						nt.name = '';
					} else {
						return;
					}
				} else {
	
					var beginspaces, endspaces, mstart, mend, arr, arr1, arr2;
					arr1 = text.match(/^\s*<[^>]*>((?:&nbsp;| )*)([^<]*)<\/[^>]*>$/i);
					arr2 = text.match(/^\s*((?:&nbsp;| )*)([^<]*)$/i);
					if (arr1) {
						arr = arr1;
					} else if (arr2) {
						arr = arr2;
					} else {
						arr = null;
					}
					if (arr) { // special hack for selecting a whole line
						beginspaces = arr[1];
						endspaces = arr[2].match(/((?:&nbsp;| )*)$/i)[1];
						mstart = 0;
						while (beginspaces != "") {
							if (beginspaces.indexOf("&nbsp;") == 0) {
								beginspaces = beginspaces.substr(6);
								mstart++;
							} else if (beginspaces.indexOf(" ") == 0) {
								beginspaces = beginspaces.substr(1);
								mstart++;
							}
						}
						mend = 0;
						while (endspaces != "") {
							if (endspaces.indexOf("&nbsp;") == 0) {
								endspaces = endspaces.substr(6);
								mend--;
							} else if (endspaces.indexOf(" ") == 0) {
								endspaces = endspaces.substr(1);
								mend--;
							}
						}
						range.moveStart('character', mstart);
						range.moveEnd('character', mend);
						text = range.htmlText;
						// remake selection
						range.select();
					}
				} 
				this.edit.execCommand(DECMD_HYPERLINK,OLECMDEXECOPT_DONTPROMPTUSER,'http://');
				var tr, trp, trpc, el;
				tr = this.edit.selection.createRange();
				trp = tr.parentElement();
				try {
					if (trp.tagName == "A") {
						el = trp;
					} else if (trp.getElementsByTagName("A")[0]) {
						el = trp.getElementsByTagName("A")[0];
					}
				} catch (e) {
					el = trp;
				}
				if (el) {
					/*this is  HACK added by MPR
					 it tricks an IE bug that for a TextRange the textHTML is different (contains) the outer HTML of the
					 parentElement.
					 
					 also for an unknown reason moveToElementText doesn't work all the time, especially with the last 
					 <P> (container in the text).
					 
					 
					*/
					//Move the end of the selection in order to put the selection inside of the A tag
					tr = this.edit.selection.createRange();
					tr.moveEnd("character",-1);
					tr.select();
					
					//I don't know exactly wich part of this function updates the DOM tre or does something good
					// but without this call moveToElementText doesn't work all the time
					this.p_displayChanged();
					//finally call the moveToElementText to select only the A tag
					tr = this.edit.selection.createRange();
					tr.moveToElementText(el);
					tr.select();
				}
				usesecond = 0;
				inserted = 1;
			} else { // CONTROL range
				var ctrlRange = this.edit.selection.createRange(); //the text that is selected

				this.edit.execCommand(DECMD_HYPERLINK,OLECMDEXECOPT_DONTPROMPTUSER,'http://');
				
				//I don't know exactly wich part of this function updates the DOM tree or does something good
				// but without this call moveToElementText doesn't work all the time
				this.p_displayChanged();
				
				//move the selection around the A tag (parent node);
				var tmpel = ctrlRange.commonParentElement();
				var parentel = tmpel.parentNode;
				var txtRange = this.edit.body.createTextRange(); //the text that is selected
				txtRange.moveToElementText(parentel);
				txtRange.select();
				
				usesecond = 1;	
				inserted = 1;
			}
		}
		this.undo.addEdit();
	} else {
		inserted = 0;
		alert(KT_js_messages["select something"]?KT_js_messages["select something"]:"Please select some text or an image.");
	}
	
	this.p_displayChanged();
	//this.logic_domSelect(usesecond,1,0);
	
	return false;
}

/**
* adds the target attribute to a link
*/
function Ktml_logic_putTargetTag(){
	var str=new String();
	var strnou = new String();
	
	sel = this.edit.selection;
	propertyTargets = this.toolbar.targets.value;
	if ("None" != sel.type) {
		if (propertyTargets!="") {
			if (sel.type == "Control") {
				var range = this.edit.body.createTextRange(); 
				var tmp1 = sel.createRange();
				range.moveToElementText(tmp1.item(0));
			} else {
				range = sel.createRange();
			}
			
			if(range.startContainer.nodeName == 'A') {
				// we are inside an anchor so set the target
				range.startContainer.setAttribute('target', propertyTargets);
			} else {
			
				if(range.pasteHTML) { // on IE
					str = range.htmlText;
					strnou = str.replace("<A","<A target=\""+propertyTargets+"\"");
					range.pasteHTML(strnou);
				} else if(range.startContainer.nodeType != 3) { 
					range.startContainer.childNodes[range.startOffset].setAttribute('target', propertyTargets);
				} else {
					// link inside text
					if(range.startContainer.parentNode.nodeName == 'A') {
						// clicked inside anchor
						range.startContainer.parentNode.setAttribute('target', propertyTargets);
					} else {
						// the selection includes anchor
						range.startContainer.nextSibling.setAttribute('target', propertyTargets);
					}
				}
			}
		}	
	} else {
		alert(KT_js_messages["No text selected"]?KT_js_messages["No text selected"]:"No text selected");
	}		
}

/**
* Remove a tag from html with exception of body (for obvious reasons),
* td and tr to not make the html invalid.
* For td adn tr removal, table introspector should be used.
*/
function Ktml_logic_removeTag() {
	// for some cases of bad html resulted from style insertion on 
	// IE an exception is thrown - promlem must be rezolved through 
	// style insertion's improvment
	
	try {
		if(this.selectableNodes && this.selectableNodes[0]) {
			var tmpel = this.selectableNodes[0];
		} else {
			var tmpel = this.logic_getSelectedNode();
		}
		if (tmpel.tagName != "BODY" 
			&& tmpel.tagName != 'TD' 
			&& tmpel.tagName != 'TR' 
			&& tmpel.tagName != 'HR'
			&& tmpel.tagName != 'UL'
			&& tmpel.tagName != 'OL'
			&& tmpel.tagName != "TABLE" 
			&& tmpel.tagName != "TBODY" 
			&& tmpel.tagName != "IMG") {
			tmpel.outerHTML = tmpel.innerHTML;
		} else if (tmpel.tagName == "TABLE" 
			|| tmpel.tagName == "IMG"
			|| tmpel.tagName == 'HR') {
			var parentel, t;
			parentel = tmpel.parentElement;
			parentel.removeChild(tmpel);
		} else if(tmpel.tagName == 'OL' || tmpel.tagName == 'UL') {
			var lis = tmpel.getElementsByTagName('LI');
			for (var i=lis.length-1; i>=0; i--) {
				lis[i].outerHTML = lis[i].innerHTML + '<br/>';
			}
			tmpel.outerHTML = tmpel.innerHTML;
		} else {
			return;
		}
		this.cw.focus();
		this.p_displayChanged();
		this.undo.addEdit();
	} catch (e) {
		// do nothig with the error - sweep it under the rug
	}
}



/**
* make border for tables that don't have one
*/
function Ktml_utils_makeTableOutline(t) {
	var tdarr, j;
	this.utils_makeOutline(t);
	tdarr = t.getElementsByTagName("TD");
	for (j=0; j<tdarr.length; j++) {
		if(this.utils_getParent(tdarr[j], 'TABLE') == t) {
			this.utils_makeOutline(tdarr[j]);
		}
	}
}

function Ktml_utils_unmakeTableOutline(t) {
	var tdarr, j;
	this.utils_unmakeOutline(t);
	tdarr = t.getElementsByTagName("TD");
	for (j=0; j<tdarr.length; j++) {
		if(this.utils_getParent(tdarr[j], 'TABLE') == t) {
			this.utils_unmakeOutline(tdarr[j]);
		}
	}
}

function Ktml_utils_makeOutline(t) {
//	if ((!t.getAttribute("KT_modified") || t.getAttribute("KT_modified") == "0") && 
//		(!t.border || t.border == 0 || t.border == null)) { // on mozilla is null
		t.setAttribute("KT_modified", "1");
		t.setAttribute("KT_exclass", t.className);
		try {
			t.setAttribute("KT_exstyle", t.style.cssText);
		} catch(e) {
			t.setAttribute("KT_exstyle", "");
		}
		t.removeAttribute("class");
		t.style.cssText = "border: dashed 1px black;";
//	}
}
function Ktml_utils_unmakeOutline(t) {
	if (t.getAttribute("KT_modified") && (t.getAttribute("KT_modified") == 1)) { //has been modified
		t.removeAttribute("KT_modified");
		//put old things back and remove the attributes
		t.removeAttribute("style");
		if(t.getAttribute("KT_exstyle") != '') {
			t.style.cssText = t.getAttribute("KT_exstyle");
		}
		if(t.getAttribute("KT_exclass") != '') {
			t.className = t.getAttribute("KT_exclass");
		}
		t.removeAttribute("KT_exstyle");
		t.removeAttribute("KT_exclass");
	}
}


function Ktml_utils_setInvisibles(flag) { // show if flag == false
	var tarr = this.edit.getElementsByTagName("TABLE");
	for ( var i = 0; i< tarr.length; i++) {
		if (!flag && 
			(!tarr[i].getAttribute("KT_modified") || tarr[i].getAttribute("KT_modified") == "0") && 
			(!tarr[i].border || tarr[i].border == 0 || tarr[i].border == null)) 
		{
			this.utils_makeTableOutline(tarr[i]);
		} else if (flag) {
			this.utils_unmakeTableOutline(tarr[i]);
		}
	}
}

function Ktml_logic_toggleInvisibles() {
	//var kttime = new Date();
	var tdarr, j;
	var sw = false;
	if (this.viewinvisibles != null) {
		sw = this.viewinvisibles;
	}
	this.utils_setInvisibles(sw);
	this.viewinvisibles = !sw;
	//window.status = new Date() - kttime;
}

/**
* on mozill after removing a list some garbage remain
*/
function Ktml_logic_cleanList() {
	this.logic_cleanTag('OL');
	this.logic_cleanTag('UL');
}

/**
* cleans tags that are empty
* used for font, span, ol, ul
*
* @param
*		name - tag name
*/
function Ktml_logic_cleanTag(name) {
	if (Ktml_mozilla)return;
	var rem = true;
	var lists = this.edit.getElementsByTagName(name);
	
	for (var i = 0; i< lists.length; i++) {
		var kids = lists[i].childNodes;
		for(var j=0; j<kids.length; j++) {
			// this should never throw anything yet it does
			try {
				rem = rem & (kids[i].nodeType == 3);
			}
			catch(e) {
				rem = rem & false;
			}
		} 
		if (rem) {
			lists[i].parentNode.removeChild(lists[i]);
		}
		rem = true;
	}

}

/**
* programaticly execute Ctrl+End
*/
function Ktml_logic_goToEnd() {
	this.edit.selection.collapse();
	var range = this.edit.selection.createRange();
	//var end = this.edit.body.lastChild;
	range.setEnd(this.edit.body, this.edit.body.childNodes.length);
	range.collapse(false);
}

/**
* programaticly execute Ctrl+Home
*/
function Ktml_logic_goToHome() {
	this.edit.selection.collapse();
	var range = this.edit.selection.createRange();
	//var end = this.edit.body.lastChild;
	range.setStart(this.edit.body, 0);
	range.collapse(true);
}

function Ktml_logic_recCommand(cmd) {
	var tmpcont = this.edit.body.innerHTML;
	var i =0;
	do { 
		if (this.edit.execCommand(cmd)) {
			i++;
		} else {
			break;
		}
		var str = this.edit.body.innerHTML;
		if (str != tmpcont || i >= 101) {
			break;
		}
	} while (1);
}

function Ktml_utils_getParent(t, parentName) {
	if(t.nodeName == parentName) {
		return t;
	}
	while(t.parentNode && t.parentNode.nodeName != parentName && t.parentNode.nodeName != 'BODY') {
		t = t.parentNode;
	}
	
	if(t.parentNode && t.parentNode.nodeName == parentName) {
		return t.parentNode;
	} else {
		return null;
	}
}

Ktml.prototype.utils_getParent = Ktml_utils_getParent;
Ktml.prototype.utils_makeTableOutline = Ktml_utils_makeTableOutline;
Ktml.prototype.utils_unmakeTableOutline = Ktml_utils_unmakeTableOutline;
Ktml.prototype.utils_makeOutline = Ktml_utils_makeOutline;
Ktml.prototype.utils_unmakeOutline = Ktml_utils_unmakeOutline;

Ktml.prototype.logic_updateDOMHierarchy = Ktml_logic_updateDOMHierarchy;
Ktml.prototype.logic_removeTag = Ktml_logic_removeTag;
Ktml.prototype.logic_InsertStyle = Ktml_logic_InsertStyle;
Ktml.prototype.logic_toggleInvisibles = Ktml_logic_toggleInvisibles;
Ktml.prototype.logic_cleanList = Ktml_logic_cleanList;
Ktml.prototype.logic_cleanTag = Ktml_logic_cleanTag;
Ktml.prototype.logic_goToEnd =  Ktml_logic_goToEnd;
Ktml.prototype.logic_goToHome = Ktml_logic_goToHome;
Ktml.prototype.utils_setInvisibles = Ktml_utils_setInvisibles;
Ktml.prototype.logic_getSelectedNode = Ktml_logic_getSelectedNode;

Ktml.prototype.hndlr_onkeydown = Ktml_hndlr_onkeydown;
Ktml.prototype.hndlr_onkeyup = Ktml_hndlr_onkeyup;

function util_stopEvent(e) {
	if (Ktml_ie) {
		e.cancelBubble = true;
		e.returnValue = false;		
	} else {
		e.preventDefault();
		e.stopPropagation();
	}
}


function dump(o) {
	var i, a='';
	for(i in o) {
		a += i + ' = ' + o[i] + '   ';
	}
	alert(a);
}
