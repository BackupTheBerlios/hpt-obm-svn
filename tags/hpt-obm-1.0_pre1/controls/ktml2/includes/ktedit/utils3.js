// Copyright 2001-2004 Interakt Online. All rights reserved.

ELEMENT_NODE                   = 1;
ATTRIBUTE_NODE                 = 2;
TEXT_NODE                      = 3;
CDATA_SECTION_NODE             = 4;
ENTITY_REFERENCE_NODE          = 5;
ENTITY_NODE                    = 6;
PROCESSING_INSTRUCTION_NODE    = 7;
COMMENT_NODE                   = 8;
DOCUMENT_NODE                  = 9;
DOCUMENT_TYPE_NODE             = 10;
DOCUMENT_FRAGMENT_NODE         = 11;
NOTATION_NODE                  = 12;


RG_AMP = new RegExp('[&]', "g");
RG_GT = new RegExp('[>]', "g");
RG_LT = new RegExp('[<]', "g");
RG_QUOTE = new RegExp('["]', "g");


if (Ktml_mozilla) {
	DECMD_BOLD =                      "bold";
	DECMD_COPY =                      "copy";
	DECMD_CUT =                       "cut";
	DECMD_DELETE =                    5004;
	DECMD_DELETECELLS =               5005;
	DECMD_DELETECOLS =                5006;
	DECMD_DELETEROWS =                5007;
	DECMD_FINDTEXT =                  5008;
	DECMD_FONT =                      "fontname";
	DECMD_GETBACKCOLOR =              "backcolor";
	DECMD_GETBLOCKFMT =               5011;
	DECMD_GETBLOCKFMTNAMES =          5012;
	DECMD_GETFONTNAME =               "fontname";
	DECMD_GETFONTSIZE =               "fontsize";
	DECMD_GETFORECOLOR =              "forecolor";
	DECMD_HYPERLINK =                 "createlink";
	DECMD_ANCHORLINK =                "createbookmark";
	DECMD_IMAGE =                     "insertimage";
	DECMD_INDENT =                    "indent";
	DECMD_INSERTCELL =                5019;
	DECMD_INSERTCOL =                 5020;
	DECMD_INSERTROW =                 5021;
	DECMD_INSERTTABLE =               5022;
	DECMD_INSERTHR =               	  "inserthorizontalrule";
	DECMD_ITALIC =                    "italic";
	DECMD_JUSTIFYCENTER =             "justifycenter";
	DECMD_JUSTIFYLEFT =               "justifyleft";
	DECMD_JUSTIFYRIGHT =              "justifyright";
	DECMD_JUSTIFYFULL  = 			  			"justifyfull";
	DECMD_LOCK_ELEMENT =              5027;
	DECMD_MAKE_ABSOLUTE =             5028;
	DECMD_MERGECELLS =                5029;
	DECMD_ORDERLIST =                 "insertorderedlist";
	DECMD_OUTDENT =                   "outdent";
	DECMD_PASTE =                     "paste";
	DECMD_REDO =                      "redo";
	DECMD_REMOVEFORMAT =              "removeformat";
	DECMD_SELECTALL =                 "selectall";
	DECMD_SEND_BACKWARD =             5036;
	DECMD_BRING_FORWARD =             5037;
	DECMD_SEND_BELOW_TEXT =           5038;
	DECMD_BRING_ABOVE_TEXT =          5039;
	DECMD_SEND_TO_BACK =              5040;
	DECMD_BRING_TO_FRONT =            5041;
	DECMD_SETBACKCOLOR =              "backcolor";
	DECMD_SETBLOCKFMT =               "formatblock";
	DECMD_SETFONTNAME =               "fontname";
	DECMD_SETFONTSIZE =               "fontsize";
	DECMD_SETFORECOLOR =              "forecolor";
	DECMD_SPLITCELL =                 5047;
	DECMD_UNDERLINE =                 "underline";
	DECMD_UNDO =                      "undo";
	DECMD_UNLINK =                    "unlink";
	DECMD_UNANCHOR =                  "unBookmark";
	DECMD_UNORDERLIST =               "insertunorderedlist";
	DECMD_PROPERTIES =                "";
	
	
	
	// OLECMDEXECOPT  
	OLECMDEXECOPT_DODEFAULT =         false;
	OLECMDEXECOPT_PROMPTUSER =        true;
	OLECMDEXECOPT_DONTPROMPTUSER =    false;
	
	// DHTMLEDITCMDF
	DECMDF_NOTSUPPORTED =             false;
	DECMDF_DISABLED =                 false;
	DECMDF_ENABLED =                  true;
	DECMDF_LATCHED =                  7;
	DECMDF_NINCHED =                  11;
	
	// DHTMLEDITAPPEARANCE
	DEAPPEARANCE_FLAT =               0;
	DEAPPEARANCE_3D =                 1;
	
	// OLE_TRISTATE
	OLE_TRISTATE_UNCHECKED =          0;
	OLE_TRISTATE_CHECKED =            1;
	OLE_TRISTATE_GRAY =               2;
}
if (Ktml_ie) {
	DECMD_BOLD =                      "Bold";
	DECMD_COPY =                      "Copy";
	DECMD_CUT =                       "Cut";
	DECMD_DELETE =                    5004;
	DECMD_DELETECELLS =               5005;
	DECMD_DELETECOLS =                5006;
	DECMD_DELETEROWS =                5007;
	DECMD_FINDTEXT =                  5008;
	DECMD_FONT =                      "FontName";
	DECMD_GETBACKCOLOR =              "BackColor";
	DECMD_GETBLOCKFMT =               5011;
	DECMD_GETBLOCKFMTNAMES =          5012;
	DECMD_GETFONTNAME =               "FontName";
	DECMD_GETFONTSIZE =               "FontSize";
	DECMD_GETFORECOLOR =              "ForeColor";
	DECMD_HYPERLINK =                 "CreateLink";
	DECMD_ANCHORLINK =                "CreateBookmark";
	DECMD_IMAGE =                     "InsertImage";
	DECMD_INDENT =                    "Indent";
	DECMD_INSERTCELL =                5019;
	DECMD_INSERTCOL =                 5020;
	DECMD_INSERTROW =                 5021;
	DECMD_INSERTTABLE =               5022;
	DECMD_INSERTHR =               	  "InsertHorizontalRule";
	DECMD_ITALIC =                    "Italic";
	DECMD_JUSTIFYCENTER =             "JustifyCenter";
	DECMD_JUSTIFYLEFT =               "JustifyLeft";
	DECMD_JUSTIFYRIGHT =              "JustifyRight";
	DECMD_JUSTIFYFULL =              "JustifyFull";
	DECMD_LOCK_ELEMENT =              5027;
	DECMD_MAKE_ABSOLUTE =             5028;
	DECMD_MERGECELLS =                5029;
	DECMD_ORDERLIST =                 "InsertOrderedList";
	DECMD_OUTDENT =                   "Outdent";
	DECMD_PASTE =                     "Paste";
	DECMD_REDO =                      "Redo";
	DECMD_REMOVEFORMAT =              "RemoveFormat";
	DECMD_SELECTALL =                 "SelectAll";
	DECMD_SEND_BACKWARD =             5036;
	DECMD_BRING_FORWARD =             5037;
	DECMD_SEND_BELOW_TEXT =           5038;
	DECMD_BRING_ABOVE_TEXT =          5039;
	DECMD_SEND_TO_BACK =              5040;
	DECMD_BRING_TO_FRONT =            5041;
	DECMD_SETBACKCOLOR =              "BackColor";
	DECMD_SETBLOCKFMT =               "FormatBlock";
	DECMD_SETFONTNAME =               "FontName";
	DECMD_SETFONTSIZE =               "FontSize";
	DECMD_SETFORECOLOR =              "ForeColor";
	DECMD_SPLITCELL =                 5047;
	DECMD_UNDERLINE =                 "Underline";
	DECMD_UNDO =                      "Undo";
	DECMD_UNLINK =                    "Unlink";
	DECMD_UNANCHOR =                  "UnBookmark";
	DECMD_UNORDERLIST =               "InsertUnorderedList";
	DECMD_PROPERTIES =                "";
	
	
	
	// OLECMDEXECOPT  
	OLECMDEXECOPT_DODEFAULT =         false;
	OLECMDEXECOPT_PROMPTUSER =        true;
	OLECMDEXECOPT_DONTPROMPTUSER =    false;
	
	// DHTMLEDITCMDF
	DECMDF_NOTSUPPORTED =             false;
	DECMDF_DISABLED =                 false;
	DECMDF_ENABLED =                  true;
	DECMDF_LATCHED =                  7;
	DECMDF_NINCHED =                  11;
	
	// DHTMLEDITAPPEARANCE
	DEAPPEARANCE_FLAT =               0;
	DEAPPEARANCE_3D =                 1;
	
	// OLE_TRISTATE
	OLE_TRISTATE_UNCHECKED =          0;
	OLE_TRISTATE_CHECKED =            1;
	OLE_TRISTATE_GRAY =               2;
}

/**
	Table Edit methods
*/
function mergeDown(ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell) {
		 return;
	}
	//        [  TD   ] [   TR   ] [  TBODY ] [         TRs            ]
	var trs = tableCell.parentNode.parentNode.getElementsByTagName("TR");
	var topRowIndex = tableCell.parentNode.rowIndex;
	if (!trs[topRowIndex + tableCell.rowSpan]) {
		 alert((KT_js_messages["Table nocellbelow"]) ? KT_js_messages["Table nocellbelow"] : "No cell below");
		 return;
	}
	//                   [              TR                 ]
	var tds        =  trs[ topRowIndex + tableCell.rowSpan ].getElementsByTagName("TD");
	var bottomCell = false;
	var idx = tableCell.cellIndex;
	while(!bottomCell && idx>=0) {
		bottomCell = tds[ idx ];
		idx--;
	}
	if (!bottomCell) {
		 alert((KT_js_messages["Table nocellbelowmerge"]) ? KT_js_messages["Table nocellbelowmerge"] : "There is not a cell below this one to merge with.");
		 return;
	}

	// don't allow merging rows with different colspans
	if (tableCell.colSpan != bottomCell.colSpan) {
		 alert((KT_js_messages["Table diffcolspans"]) ? KT_js_messages["Table diffcolspans"] : "Can't merge cells with different colspans."); 
		 return;
	}

	// do the merge
	tableCell.innerHTML += bottomCell.innerHTML;
	tableCell.rowSpan += bottomCell.rowSpan;
	bottomCell.parentNode.removeChild(bottomCell);
	//bottomCell.removeNode(true); 
	if (ktml.undo) {
		ktml.undo.addEdit();
	}
	//ktml.edit.selection.collapse();
	ktml.cw.focus();
}

function unMergeDown(ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		 return;
	if (tableCell.rowSpan <= 1) {
		 alert((KT_js_messages["Table rowspanalready1"]) ? KT_js_messages["Table rowspanalready1"] : "RowSpan is already set to 1.");
		 return;
	}
	var topRowIndex = tableCell.parentNode.rowIndex;
	// add a cell to the beginning of the next row
	var td = tableCell.parentNode.parentNode.getElementsByTagName("TR")[ topRowIndex + tableCell.rowSpan - 1 ].appendChild( ktml.edit.createElement("TD") );
	tableCell.rowSpan -= 1;
	td.innerHTML = "&nbsp;";
	if (ktml.undo) {
		ktml.undo.addEdit();
	}
}
function mergeRight(ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		 return;
	var tds = tableCell.parentElement.getElementsByTagName("TD");
	if (!tds[tableCell.cellIndex+1]) {
		return;
	}
	// don't allow user to merge rows with different rowspans
	if (tableCell.rowSpan != tds[tableCell.cellIndex+1].rowSpan) {
		 alert((KT_js_messages["Table diffrowspans"]) ? KT_js_messages["Table diffrowspans"] : "Can't merge cells with different rowSpans.");
		 return;
	}

	tableCell.innerHTML += tds[tableCell.cellIndex+1].innerHTML;
	tableCell.colSpan += tds[tableCell.cellIndex+1].colSpan;
	
	tableCell.parentNode.removeChild(tds[tableCell.cellIndex+1]);

	tableCell = null;
	ktml.cw.focus();
	ktml.undo.addEdit();

}

function splitCell(ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		 return;
	if (tableCell.colSpan < 2) {
		 alert((KT_js_messages["Table cantdivide"]) ? KT_js_messages["Table cantdivide"] : "Cell can't be divided.  Add another cell instead");
		 return;
	}

	tableCell.colSpan = tableCell.colSpan - 1;
	var newCell = tableCell.parentNode.insertBefore( ktml.edit.createElement("TD"), tableCell);
	newCell.rowSpan = tableCell.rowSpan;
	newCell.innerHTML = "&nbsp;";
	if (ktml.undo) {
		ktml.undo.addEdit();
	}

}

function removeCell(ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		 return;
	// can't remove all cells for a row
	var tds = tableCell.parentElement.getElementsByTagName("TD");
	if (td.length == 1) {
		 alert((KT_js_messages["Table onlycell"]) ? KT_js_messages["Table onlycell"] : "You can't remove the only remaining cell in a row.");
		 return;
	}
	tableCell.parentNode.removeChild(tableCell);
	tableCell = null;
	if (ktml.undo) {
		ktml.undo.addEdit();
	}

} 

function addCell(ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		 return;
	var tds = tableCell.parentElement.getElementsByTagName("TD");
	tableCell.parentElement.insertBefore(ktml.edit.createElement("TD"), tableCell.nextSibling);
	ktml.cw.focus();
	if (ktml.undo) {
		ktml.undo.addEdit();
	}

}

function processRow(action, ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		return;
	// go back to TABLE def and keep track of cell index
	var idx = 0;
	var rowidx = -1;
	var tr = tableCell.parentNode;
	// count number of TD children
	var numcells = tr.getElementsByTagName("TD").length;
	while (tr) {
		if (tr.tagName == "TR")
			rowidx++;
		tr = tr.previousSibling;
	}
	// now we should have a row index indicating where the
	// row should be added / removed
	var tbl = getObjectParentTag("table", tableCell);
	if (!tbl) {
		 alert("Could not " + action + " row.");
		 return;
	}
	if (action == "add") {
		var r = tbl.insertRow(rowidx);
		for (var i = 0; i < numcells; i++) {
			var td = ktml.edit.createElement("TD");
			//td.appendChild(ktml.edit.createElement("BR"));
			td.innerHTML = '&nbsp;';
			var c = r.appendChild(td);
			if (tableCell.parentNode.getElementsByTagName("TD")[i].colSpan)
				c.colSpan = tableCell.parentNode.getElementsByTagName("TD")[i].colSpan;
		}
	} else {
		tbl.deleteRow(rowidx);
		tableCell = null;
	}
	ktml.cw.focus();
	if (ktml.undo) {
		ktml.undo.addEdit();
	}
}

function processColumn(action, ktml) {
	var tableCell = getObjectParentTag("td", ktml.edit.selection.createRange().parentElement());
	if (!tableCell)
		return;
	// store cell index in a var because the cell will be
	// deleted when processing the first row
	var cellidx = tableCell.cellIndex;
	var tbl = getObjectParentTag("table", tableCell);
	if (!tbl) {
		 alert("Could not " + action + " column.");
		 return;
	}
	// now we have the table containing the cell
	__addOrRemoveCols(tbl, cellidx, action, ktml);
	ktml.cw.focus();
	if (ktml.undo) {
		ktml.undo.addEdit();
	}
}

function __addOrRemoveCols(tbl, cellidx, action, ktml) {
	if (!tbl.childNodes.length)
		 return;
	var i;
	for (i = 0; i < tbl.childNodes.length; i++) {
		 if (tbl.childNodes[i].tagName == "TR") {
				var cell = tbl.childNodes[i].getElementsByTagName("TD")[ cellidx ];
				if (!cell)
					 break; // can't add cell after cell that doesn't exist
				if (action == "add") {
					var td = ktml.edit.createElement("TD");
					//td.appendChild(ktml.edit.createElement("BR"));
					td.innerHTML = '&nbsp;';
					cell.parentNode.insertBefore(td, cell);
				} else {
					 // don't delete too many cells because or a rowspan setting
					 if (cell.rowSpan > 1) {
							i += (cell.rowSpan - 1);
					 }
					 cell.parentNode.removeChild(cell);
				}
		 } else {
				__addOrRemoveCols(tbl.childNodes[i], cellidx, action, ktml); 
		 }
	}
}

/*----------------------------------------*/

function ui_openAboutBox() {
	util_openwnd("_about",NEXT_ROOT+"includes/ktedit/popups/about.html", 350, 200);
}

function util_openwnd(u,n,w,h, x) {
	var top, left, top, features;
	left = (screen.width - w) / 2;
	top = (screen.height - h) / 2;
	features = ",left="+left+",top="+top;
	winargs="width="+w+",height="+h+",resizable=no,scrollbars=no,status=0";
	dialogargs = "scrollbars=no,center=yes;dialogHeight="+h+"px;dialogWidth="+w+"px;help=no;status=no;resizable=no";
	if (n.indexOf("?") != -1) {
		n += "&";
	} else {
		n += "?";
	}
	n +="rand="+Math.random().toString().substring(3);
	//alert(n)
	if (window.showModalDialog && !x) {
	//if (window.showModalDialog && 0) {
		//alert("opening modal:\n" + n);
		remote=window.showModelessDialog(n, window, dialogargs + features);
		if (n.match(/dirbrowser/)) {
			window._dlg_ = remote;
		}
	} else {
		var newLocation;
		newLocation = window.location + '';
		newLocation = newLocation.replace(/\/[^\/]*$/, '');
		if(n.indexOf('./') == 0) {
			newLocation += n.replace(/^\./, '');
		} else if (n.indexOf('http://') == 0) { 
			newLocation = n;
		} else {
			newLocation += '/' + n;
		}
		remote=window.open(newLocation, u, winargs + features);
		remote.focus();
	}
  
  if (remote != null) {
    if (remote.opener == null)
      remote.opener = self;
  }
  return remote;
}

function logic_submitKTMLS() {
	for ( var i = 0; i < ktmls.length ; i++) {
		if (ktmls[i].viewinvisibles == true) {
			ktmls[i].logic_toggleInvisibles();
		}
		if(ktmls[i].displayMode == 'RICH') {
			if(ktmls[i].saveXHTML == 'yes') {
				document.getElementById(ktmls[i].name).value = hndlr_save(get_docXHTML(ktmls[i].edit));
			} else {
				document.getElementById(ktmls[i].name).value = hndlr_save(ktmls[i].edit.body.innerHTML);
			}
		} else {
			document.getElementById(ktmls[i].name).value = hndlr_save(ktmls[i].textarea.value);
		}
	}
	return true;
}

function get_docXHTML(doc) {
	var toReturn = innerXHTML(doc, doc.body, new RegExp("contenteditable"));
	return toReturn;
}

function innerXHTMLAttributes(el, ignore) {
	var str = '';
	for (var i = 0; i < el.attributes.length; i++) {
		var attr = el.attributes[i];
		// ignore unset reported attributes
		if (attr.nodeValue && (!ignore || attr.nodeName.toLowerCase().search(ignore) == -1)) {
			if (typeof(attr.nodeValue) == "string" ) {
				if (str.length) str += ' ';
				str += attr.nodeName.toLowerCase(); //lower
				str += '="' + (attr.nodeValue).replace(/[\"]/g, "&quot;") + '"'; //put between quotes, escape quote
			} else if(typeof(attr.nodeValue) == "number") {
				if (str.length) str += ' ';
				str += attr.nodeName.toLowerCase(); //lower
				str += '="' + attr.nodeValue + '"';
			}
		}
	}
	return str;
}

/**
* Construct the innerHTML for a node in a browser independent way
*/
function innerXHTML(doc, el, ignore) {
	var str = ''; 
	for(var i=0; i<el.childNodes.length; i++) {
		if(el.childNodes[i].parentNode.nodeName != el.nodeName) {
			// bug in IE: in case html is broke the DOM is not constructed correctly
			continue;
		}
		if(el.childNodes[i].nodeType == TEXT_NODE) {  
			str += el.childNodes[i].nodeValue.replace(RG_AMP, "&amp;").replace(RG_LT, "&lt;").replace(RG_GT, "&gt;").replace(RG_QUOTE, "&quot;");		
		} else if(el.childNodes[i].nodeType == COMMENT_NODE) {
			str += '<!--' + el.childNodes[i].nodeValue + '-->';
		} else {
			str += outerXHTML(doc, el.childNodes[i], ignore);
		}
	}
	
	return str;
}

function outerXHTML(doc, el, ignore) {
	var attrs = innerXHTMLAttributes(el, ignore);
	var inner = innerXHTML(doc, el, ignore);
	return '<' + el.nodeName.toLowerCase() + (attrs.length ? ' ' + attrs : '') + (inner.length ? '>' + inner + '</' + el.nodeName.toLowerCase() + '>' : '/>');
}

function util_Transform(html) {
	//var html = doc.innerHTML;
	//if (html) html = replaceCharacters(html);
	//if (html) html = util_replaceAbsoluteUrls(html);
	//if (html) html = util_remEmptyTags(html)
	//if (html) html = util_replaceTags([["strong","B"],["em","I"]],html);
	return html;
}

function util_remEmptyTags() {
	var html = activeActiveX.contentWindow.document.body.innerHTML;
	var re = /<[^(>|\/|tr|td)]+>[ |	]*<\/[^>]+>/gi;
	while(re.test(html)) {
		html = html.replace(re,"");
		while(re.test(html)) {
			html = html.replace(re,"");
		}
	}
	return html;
}

function util_replaceAbsoluteUrls(html) {
	var docLoc = document.location.toString();
	docLoc = docLoc.substring(0,docLoc.lastIndexOf("/")+1);
	docLoc = docLoc.replace(/\//gi,"\\\/");
	var re = eval("/"+docLoc+"/gi");
	return html.replace(re, "");
}
// set= [["strong", "b"], ["em", "i"]]
function util_replaceTags(set, html) {
	var re;
	for(var i = 0; i < set.length; i++) {
		re = eval("/(<[\/]{0,1})"+set[i][0]+">/gi");
		html=html.replace(re,"$1"+set[i][1]+">");
	}
	return html;
}

function util_ccParser(html) {
//	html = html.replace(/@/gi,"_AT_");
//	html = html.replace(/#/gi,"_DZ_");
//
	var htmltag = /(&lt;[\w\/]+[ ]*[\w\=\"\'\.\/\;\: \)\(-]*&gt;)/gi;
//	html = html.replace(htmltag,"<span class=ccp_tag>$1</span>");
//	html = html.replace(htmltag,"<span style='color:#0000CC'>$1</span>");
	return html;
}
function sg(a,a2,a3) {
	var r='';
	while (a.indexOf(a2)!=-1) {
		var v1=a.indexOf(a2);
		r+=a.substring(0,v1)+a3;
		a=a.substring(v1+a2.length);
	}
	return r+a;
}

function toSafeString(a) {
//	a = sg(a,'\\','\\\\'); // Backslash backslashes.
//	a = sg(a,'\n','\\n');  // Backslash newlines (Unix, Windows, DOS).
//	a = sg(a,'\r','\\r');  // Backslash newlines (Macintosh).
//	a = sg(a,'\t','\\t');  // Backslash tabs.
//	a = sg(a,'"','\\"');   // Backslash double quotes.
	a = sg(a,'\'','\\\'');
	//return '"' + a + '"';  // Return quoted string.
	return a; //Return nonquoted String
}

function util_srchString(htmlEl, attrMask) {
	var toRet = new String("|");
	for ( var j = 0; j < htmlEl.attributes.length; j++) {
		var attr = htmlEl.attributes[j];
		if (attr.nodeValue && typeof(attr.nodeValue) == "string") {
			if (attr.nodeName.toLowerCase().search(attrMask) >= 0) {
				toRet+= attr.nodeValue+"|";
			}
		}
	}
	for (var i = 0; i< htmlEl.children.length; i++) {
		toRet+=util_srchString(htmlEl.children[i], attrMask);
	}
	return toRet;
}

function util_modAttr(htmlEl, tagNames, attrMask, attrValue) {
	var toRet = new String("|");
	if (htmlEl.nodeType == 1) { //htmlElement
		if (tagNames.join().indexOf(htmlEl.tagName.toLowerCase()+",") > 0) {
			htmlEl.setAttribute(attrMask, attrValue);
		}
	} else if (htmlEl.nodeType == 3) { //Textnode
		return;
	}
	if (htmlEl.nodeType == 1) {
		if (tagNames.join().indexOf(htmlEl.tagName.toLowerCase()+",") > 0) { //is a div or a p , operate only on the children
			if (htmlEl.children.length > 0) {
				for (var i = 0; i< htmlEl.children.length; i++) {
					util_modAttr(htmlEl.children[i], tagNames, attrMask, attrValue);
				}
			} else {
				return;
			}
		} else { //is not a div, p operate on the TextNodes also
			if (htmlEl.childNodes.length > 0) {
				var txtNodes = new Array();
				var htmlNodes = new Array();
				for (var i = 0; i< htmlEl.childNodes.length; i++) {
					if (htmlEl.childNodes[i].nodeType == 3) {//TextNode
						txtNodes[txtNodes.length] = htmlEl.childNodes[i];
					} else {
						htmlNodes[htmlNodes.length] = htmlEl.childNodes[i];
					}
				}
				for (var j = 0; j<txtNodes.length; j++) {
					var tmpstr = txtNodes[j].data;
					if (tmpstr != "" && tmpstr != "&nbsp;") {
						var dv = activeActiveX.document.createElement("DIV");
						dv.align = "justify";
						dv.innerHTML = tmpstr;
						htmlEl.insertBefore(dv, txtNodes[j]);
					}
				}
				for ( var j = 0; j< htmlEl.childNodes.length; j++) {
					if (htmlEl.childNodes[j].nodeType == 3) {
						htmlEl.childNodes[j].removeNode();
					}
				}
				for ( var j = 0; j< htmlNodes.length; j++) {
					util_modAttr(htmlNodes[j], tagNames, attrMask, attrValue);
				}
			} else {
				return;
			}
		}
	}
	return;
}

function util_remSpaces(tmpstr) {
	if(tmpstr == "<p>&nbsp;</p>" || tmpstr == "<div>&nbsp;</div>") {
		tmpstr = "";
	}
	return tmpstr;
}

function util_fix4NS(instr){
	var re  = /STYLE=\"WIDTH\s*:\s*(\d+)px;\s*HEIGHT:\s*(\d+)px;*\s*\"/gi;
        var re1 = /<A\s+href=".*?"\s*>\s*<\/A>/gi; // for taking out blank links
        instr=instr.replace(re1, "");
	return(instr.replace(re, "width=$1 height=$2"));
}	

function util_cleanHTMLContent(tmp) {
	tmp = tmp.replace(/<\?xml:.*?>/ig, "");

	tmp = tmp.replace(/<H[0-9]+\s?([^>]*)>/ig, "<P $1>");
	tmp = tmp.replace(/<\/H[0-9]+>/ig, "</P>");

	tmp = tmp.replace(/<TT([^>]*)>/ig, "<P $1>");
	tmp = tmp.replace(/<\/TT>/ig, "</P>");

	tmp = tmp.replace(/(<[^>]+)lang=[a-z]*([^>]*>)/ig, "$1$2");
	tmp = tmp.replace(/("|\s)MARGIN:.*?(;|")/ig, "$1$2");
	tmp = tmp.replace(/("|\s)TEXT-INDENT:.*?(;|")/ig, "$1$2");
	tmp = tmp.replace(/("|\s)FONT-WEIGHT:.*?(;|")/ig, "$1$2");
	tmp = tmp.replace(/("|\s)tab-stops:.*?(;|")/ig, "$1$2");

	tmp = tmp.replace(/("|\s)mso-.*?:.*?(;|")/ig, "$1$2");
	tmp = tmp.replace(/\sclass=Mso.*?\s/ig, " ");
	tmp = tmp.replace(/<\/?o:p>/ig, "");
	tmp = tmp.replace(/(style="[^"]*)TEXT-ALIGN:\s?([a-z]*)([^"]*")/ig, "align=$2 $1$3");
	tmp = tmp.replace(/(style="[^"]*)BACKGROUND:\s?([a-z0-9#]*)([^"]*")/ig, "bgcolor=$2 $1$3");
	tmp = tmp.replace(/<SPAN style="FONT-FAMILY: Symbol">\s*<FONT\s+size=[0-9]*>\ï¿½<\/FONT>/ig, "<li>");
	tmp = tmp.replace(/(<font[^>]*)>\s*<font/ig, "$1");
	tmp = tmp.replace(/<dir>/ig, "<BLOCKQUOTE>");
	tmp = tmp.replace(/<\/dir>/ig, "</BLOCKQUOTE>");

	tmp = tmp.replace(/\sstyle="[^"]*"/ig, " ");
	tmp = tmp.replace(/\sclass=[A-Z0-9_]+/ig, " ");
	tmp = tmp.replace(/\sclass="[^"]*"/ig, " ");
	//tmp = tmp.replace(/(<td[^>]*)(width|height)=["'a-z0-9_%]*([^>]*>)/ig, '$1$3');
	//tmp = tmp.replace(/(<table[^>]*)(width|height)=["'a-z0-9_%]*([^>]*>)/ig, '$1$3');
	tmp = tmp.replace(/<font[^>]*><\/font>/ig, '');
	tmp = tmp.replace(/<p[^>]*><\/p>/ig, '');

	return tmp;//activeActiveX.document.body.innerHTML = tmp;
}

function util_cleanHTMLContent2(tmp) {
	tmp = tmp.replace(/<H[0-9]+\s?(.*)>/ig, "<P $1>");
	tmp = tmp.replace(/<\/H[0-9]+>/ig, "</P>");

	tmp = tmp.replace(/<TT([^>]*)>/ig, "<P $1>");
	tmp = tmp.replace(/<\/TT>/ig, "</P>");

	tmp = tmp.replace(/style="[^"]*"/ig, "");
	tmp = tmp.replace(/<font[^>]*>/ig, "");
	tmp = tmp.replace(/<\/font>/ig, "");
	tmp = tmp.replace(/\xFE/g, "Å£");
	tmp = tmp.replace(/\xDE/g, "Å¢");

	tmp = tmp.replace(/\xE2/g, "Ã¢");
	tmp = tmp.replace(/\xC2/g, "Ã‚");
	
	tmp = tmp.replace(/\xEE/g, "Ã®");
	tmp = tmp.replace(/\xCE/g, "ÃŽ");
	
	tmp = tmp.replace(/\xBA/g, "ÅŸ");
	tmp = tmp.replace(/\xAA/g, "ï¿½");
	
	tmp = tmp.replace(/\xE3/g, "Äƒ"); 
	tmp = tmp.replace(/\xC3/g, "Ä‚");
	tmp = tmp.replace(/\x92/g, "kk");
	tmp = tmp.replace(/\x96/g, "-");
	tmp = tmp.replace(/â€“/g, "-");
	tmp = tmp.replace(/â€™/g, "-");

	tmp = tmp.replace(/class=[^ >]*/ig, "");
	tmp = tmp.replace(/class="[^"]*"/ig, "");

	//tmp = tmp.replace(/(<td[^>]*)(width|height)=[^ >]*([^>]*>)/ig, '$1$3');
	//tmp = tmp.replace(/(<table[^>]*)(width|height)=[^ >]*([^>]*>)/ig, '$1$3');

	tmp = tmp.replace(/<font[^>]*><\/font>/ig, '');
	//tmp = hndlr_load(tmp);
	return tmp;//activeActiveX.document.body.innerHTML = tmp;
}

/**
* inserts a node in selection
*
* @param
*		win - ktml.edit
*		insertNode - node to be inserted
*/
if (Ktml_mozilla) {
function util_insertNodeSel(win, insertNode) {
	// get current selection
	var sel = win.selection;
	// get the first range of the selection
	// (there's almost always only one range)
	var range = sel.getRangeAt(0);
	// deselect everything
	sel.removeAllRanges();
	// remove content of current selection from document
	range.deleteContents();
	// get location of current selection
	var container = range.startContainer;
	var pos = range.startOffset;
	// make a new range for the new selection
	//range=sel.createRange();
	if (container.nodeType==3 && insertNode.nodeType==3) {
		container.insertData(pos, insertNode.nodeValue);// if we insert text in a textnode, do optimized insertion
		range.setEnd(container, pos+insertNode.length); // put cursor after inserted text
		range.setStart(container, pos+insertNode.length);
	} else {
		var afterNode;
		if (container.nodeType==3) {
			// when inserting into a textnode
			// we create 2 new textnodes
			// and put the insertNode in between
			var textNode = container;
			container = textNode.parentNode;
			var text = textNode.nodeValue;
			// text before the split
			var textBefore = text.substr(0,pos);
			// text after the split
			var textAfter = text.substr(pos);
			var beforeNode = document.createTextNode(textBefore);
			var afterNode = document.createTextNode(textAfter);
			// insert the 3 new nodes before the old one
			container.insertBefore(afterNode, textNode);
			container.insertBefore(insertNode, afterNode);
			container.insertBefore(beforeNode, insertNode);
			// remove the old node
			container.removeChild(textNode);
		} else {
			// else simply insert the node
			afterNode = container.childNodes[pos];
			container.insertBefore(insertNode, afterNode);
		}
		range.selectNode(insertNode);
		range.collapse(false);
		//range.setStart(afterNode, 0);
	}
	sel.addRange(range);
}
}
if (Ktml_ie) {
function util_insertNodeSel(win, insertNode) {
	if (win.selection.type =='Text' || win.selection.type =="None" ) {
		var tr = win.selection.createRange();
		tr.pasteHTML(insertNode.outerHTML);
		return tr.parentElement();
	} else {
		var ctrlRange = win.selection.createRange(); //the text that is selected
		var tmpel = ctrlRange.commonParentElement();
		tmpel.outerHTML = insertNode.outerHTML;
		return tmpel;
		//return tmpel.parentNode.insertBefore(insertNode, tmpel);
	}
}
}


function getObjectParentTag (tname, startel) {
    found = false; error = false;
    toret = null;
    do {
        if (startel.tagName.toLowerCase() == tname) {
            found = true;
            toret = startel;
        }
        if (startel.tagName.toLowerCase() == "body" || !startel.parentElement || startel == null) {
			error = true;
		}
		if (found || error) {
			break;
		}
		startel = startel.parentElement;
	} while (1);                    
	return toret;
}   
     
function hndlr_buttonMouseDown(src) {
	if(src.state==0) return;
	if(src.getAttribute("kttype")=="btn") {
		src.className="toolbaritem_inset";
	}
	return true;
}

function hndlr_buttonMouseUp(src) {
	if(src.state==0) return;
	if(src.state==2) { src.className="toolbaritem_latched"; return; }
	if(src.getAttribute("kttype")=="btn") {
		src.className="toolbaritem_outset";
	}
	return true;
}

function hndlr_buttonMouseOver(src) {
	if(src.state==0) {
		return;
	}
	//if(src.state==2) return;
	if(src.getAttribute("kttype")=="btn") {
		src.className="toolbaritem_outset";
	}
}

function hndlr_buttonMouseOut(src) {
	if(src.state==0) return;	
	if(src.state==2) { 
		src.className="toolbaritem_latched"; return; 
	}
	if(src.getAttribute("kttype")=="btn") {
		src.className="toolbaritem_flat";
	}
}

var orig_window_location = "";
var orig_short_location = "";

function hndlr_load (str, mode) {
	str = str.replace(/<span badword=[^>]*>([^<]+)<\/span>/gi, '$1');
	str = util_removeURLfromHref(str);

	return str;
}


function util_removeURLfromHref(href) {
	if(!Ktml_strip) {
		return href;
	}
	if (orig_window_location == "") {
        orig_window_location = window.location.href;
        orig_short_location  = window.location.href;
        var ix = orig_window_location.lastIndexOf("/");
        if(ix != -1) {
			orig_window_location = orig_window_location.substr(0, ix+1);
        }
        ix = orig_short_location.indexOf("/", 7);
		if(ix != -1) {
            orig_short_location = orig_short_location.substr(0, ix);
		}
	}
 
	if(orig_window_location != "") {
		var href_re = new RegExp(orig_window_location+"[^\"']*#", "ig");
		href = href.replace(href_re, '#');
		 
		href_re = new RegExp(orig_window_location, "ig");
		href = href.replace(href_re, "");
		 
		href_re = new RegExp('(src|href)=("|\'|)' + orig_short_location, "ig");
		href = href.replace(href_re, '$1=$2');
    }
   
    return href;
}
 
function hndlr_save(str) {
	str = util_removeURLfromHref(str);
	return str;
}

function util_setInput(el, vl) {
	if (Ktml_mozilla) {
		el.value = vl;
	} else {
		el.value = vl;
	}
}


function util_checkFld(el, max) {
	var cond = true;
	//
	//if (!el.value.match(/^#[0-9a-fA-F]{6}*$/) && (el.value.match(/^[0-9]*%$/))) {
	if (!el.value.match(/^[0-9]*%?$/)) {
		cond = false;
		text = "Invalid value";
	}
	if ( parseInt(el.value) > max) {
		cond = false;
		text = "Value to big! Must be less then " +  max;
	}
	if (el.value == "") {
	 cond = true;
	}
	if (cond) {
		return true;
	} else {
		el.value = '';
		alert(text);
		return false;
	}
}

if (Ktml_mozilla) {
function util_preventEvent(e) {
	var keycode = e.keyCode;
	if (keycode == 0) {
		keycode = e.charCode;
	}
	if (keycode == 13) {
		e.preventDefault();
		e.stopPropagation();
		return false;
	}
}
function util_preventEvent2(o, e) {
	var keycode = e.keyCode;
	if (keycode == 0) {
		keycode = e.charCode;
	}
	if (keycode == 13) {
		e.preventDefault();
		e.stopPropagation();
		return false;
	}
}
}

if (Ktml_ie) {
	function util_preventEvent(e) {
		if (e.keyCode == 13) {
			return false;
		}
	}
	function util_preventEvent2(o, e) {
		if (e.keyCode == 13) {
			return false;
		}
	}
}


function ktmlAddEvent(oldHandler,newHandler) {
	var me = function () {
		newHandler();
		if (oldHandler) {
			oldHandler();
		}
	}
	return me;
}

var framesLoaded = false;

function util_loadIframe(iframeName, url) {
  if ( window.frames[iframeName] ) {
    window.frames[iframeName].location = url;   
    return false;
  }
  else return true;
}

function loadIframes(){
	if(framesLoaded) {
		return;
	}

	for(i=0;i<window.frames.length;i++){
		ifr = window.frames[i].name;
		if(ifr.match(/_htmlObject$/)){
			hfi = ifr.replace(/_htmlObject$/, "");
			if(document.getElementById(hfi)){
				if(document.getElementById(hfi).value){
					window.frames[i].document.write(document.getElementById(hfi).value);
				}
			}
		}
	}
	framesLoaded = true;
}

function lay_getAbsolutePos(el) {
	var r = new Object();
	r.x = el.offsetLeft;
	r.y = el.offsetTop;
	if (el.offsetParent) {
		var tmp = lay_getAbsolutePos(el.offsetParent);
		r.x += tmp.x;
		r.y += tmp.y;
	}
	return r;
}

function util_srchString(htmlEl, attrMask) {
	var toRet = new String("|");
	var regexpstr = "/"+attrMask+"/";
	for ( var j = 0; j < htmlEl.attributes.length; j++) {
		var attr = htmlEl.attributes[j];
		if (attr.nodeValue && typeof(attr.nodeValue) == "string") {
			if (attr.nodeName.toLowerCase().search(attrMask) >= 0) {
				toRet+= attr.nodeValue+"|";
			}
		}
	}
	for (var i = 0; i< htmlEl.children.length; i++) {
		toRet+=util_srchString(htmlEl.children[i], attrMask);
	}
	return toRet;
}

// CBA 2003.08.21
// x,y,w,h - wanted position (relative to document 0,0) and box size
// return {x: new x, y: new y} (relative to document0,0)
function util_positionOnScreen(x,y,w,h) {
	var bw, bh, sw, sh, d;
	if (Ktml_mozilla) {
		bw = document.width;
		bh = document.height;
		sw = window.pageXOffset;
		sh = window.pageYOffset;
	}
	else {
		d = document.body;
		bw = d.offsetWidth - 20;
		bh = d.offsetHeight;
		sw = d.scrollLeft;
		sh = d.scrollTop;
	}
	if (x + w > bw + sw) {
		x = bw + sw - w;
	}
	if (y + h > bh + sh) {
		y = bh + sh - h;
	}
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	return {x: x, y:y};
}

function util_preserveSession(numSeconds) {
	if (Ktml_ie) {
		xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
	}
	if (Ktml_mozilla) {
		xmlhttp=new XMLHttpRequest();
	}
	xmlhttp.open("GET", NEXT_ROOT+"/includes/ktedit/popups/session.php",true);
	xmlhttp.onreadystatechange=function() {
		if (xmlhttp.readyState==4) {
			setTimeout('util_preserveSession('+numSeconds+')', numSeconds);
		}
	}
	try {
		xmlhttp.send(null);
	} catch (e) {
		//alert('error');
	}
}
 
//xmlhttp_to();


// string extensions

String.prototype.ltrim = function() {
	return this.replace(/^\s+/,'');
}
String.prototype.rtrim = function() {
	return this.replace(/\s+$/,'');
}
String.prototype.trim = function() {
	return this.replace(/^\s+/,'').replace(/\s+$/,'');
}

// zeroPad adds 0 in frotn till length
function util_zeroPad(str, length) {
	if (!str) str = "";
	str = str.toString();
	while (str.length < length) {
		str = "0" + str;
	}
	return str;
}

// decimal to hex - should be redone
function decimalToHex(_decimal)
{
 if (!_decimal) return;
 var inputDecimal = _decimal;
 var inputDecimal1 = parseInt(inputDecimal);
 var outputHex = new String("");
 var outputHex1;
 var decimalLength = inputDecimal.length;
 var decimalDigit;
 var hexDigit;
 var i = 0;
 var j = 0;
 var temp;
 for(j=0 ; Math.pow(16,j)<=inputDecimal ; j++)
     ;
 i = j - 1;
 while(i!=-1) {
     temp = Math.floor(inputDecimal1 / Math.pow(16,i));
     if(temp!=0)
 inputDecimal1 = inputDecimal1 - temp*Math.pow(16,i);
     if(temp<10)
 outputHex1 = temp;
     else if(temp==10)
 outputHex1 = "A";
     else if(temp==11)
 outputHex1 = "B";
     else if(temp==12)
 outputHex1 = "C";
     else if(temp==13)
 outputHex1 = "D";
     else if(temp==14)
 outputHex1 = "E";
     else if(temp==15)
 outputHex1 = "F";
     else
 outputHex1 = "";
     outputHex = outputHex + outputHex1;
     i--;  }
 if(inputDecimal=="0" || inputDecimal=="1")
     outputHex = inputDecimal;
 return outputHex;
	}

// bgr2rgb
function intbgr2hexrgb(a) {
	d = decimalToHex;
	// on mozilla will report rgb(a, b, c) - so the following is not good
	return "#" + util_zeroPad(d(a%256),2) + util_zeroPad(d((a/256)%256),2) + util_zeroPad(d((a/65536)%256),2);
}

function dumpVar(obj, sep) {
	if (sep == undefined) {
		sep = '';
	}
	tm = "";
	if (typeof(obj) == "object") {
		for (i in obj) {
			tm += sep + i + ":{\n" + dumpVar(obj[i], sep + '  ') + "}\n";
		}
		return tm;
	} 
	if (typeof(obj) == "function") return sep + typeof(obj) + "\n";
	return sep + obj + "\n";
}