// Copyright 2001-2004 Interakt Online. All rights reserved.
	
function KT_attachProperties(obj) {

	//obj is the initial iframe
	obj_wnd = obj.contentWindow;
	obj_doc = obj.contentWindow.document;
	obj_doc.readyState = "complete";
	//define references to prototypes
	obj_wnd_proto = obj.contentWindow.__proto__; //Window prototype
	obj_wndsel_proto = obj.contentWindow.getSelection().__proto__; //Selection prototype

	obj_doc_proto = obj.contentWindow.document.__proto__; //HTMLDocument prototype	
	obj_docbody_proto = obj.contentWindow.document.body.__proto__; //HTMLBodyElement prototype	
	obj_docrange_proto = obj.contentWindow.document.createRange().__proto__; //HTMLDocument prototype	
	obj_doc_htmlel_proto = obj.contentWindow.document.createElement("P").__proto__.__proto__; //HTMLElement prototype
	obj_doc_event_proto = obj.contentWindow.document.createEvent("MouseEvents").__proto__; //Event prototype
	obj_doc_style_proto = obj.contentWindow.document.createElement("STYLE").__proto__; //HTMLStyleElement prototype
	
	//attach attachEvent to window, document and element

	obj_doc_proto.__defineGetter__("KT_Window", function () {
		return obj.contentWindow;
	});
	obj_doc_proto.__defineGetter__("selection", function () {
		if (this.KT_Window.getSelection() == null)
			return null;
		return this.KT_Window.getSelection();
	});

	obj_wnd_proto.__defineGetter__("selection", _selection);
	obj_wndsel_proto.createRange = _createRange;
	obj_wndsel_proto.__defineGetter__("type", _getType);


	// Create a new stylesheet, or load a .css file
	obj_doc_proto.createStyleSheet = _createStyleSheet;

	// Range methods & properties	
	obj_docrange_proto.isEqual = _isEqual;
	obj_docrange_proto.__defineGetter__("KT_Window", function () {
		return obj.contentWindow;
	});

	obj_docrange_proto.parentElement = _rngparentElement;
	obj_docrange_proto.moveToElementText = _moveToElementText;
	obj_docrange_proto.addElement = _addElement;

	obj_docrange_proto.select = function () {
		var sel = this.KT_Window.getSelection();
		sel.removeAllRanges();
		sel.addRange(this);
	};
	
	//obj_docrange_proto.pasteHTML = function (text) {
	//};

	obj_docrange_proto.findFirstTextNode = function (node) {
		if(!node) {
			return false;
		}
		switch (node.nodeType) {
    case 1:
    case 9:
    case 11:
      if (node.childNodes.length > 0)
        return this.findFirstTextNode(node.firstChild);
      else
        return this.findFirstTextNode(node.nextSibling);
    case 3:
      return node;
    default:
      alert("Not implemented:  type=" + node.nodeType);
  	}
	};
	obj_docrange_proto.findLastTextNode = function (node) {
		if(!node) {
			return false;
		}
		switch (node.nodeType) {
    case 1:
    case 9:
    case 11:
      if (node.childNodes.length > 0)
        return this.findLastTextNode(node.lastChild);
      else
        return this.findLastTextNode(node.previousSibling);
    case 3:
      return node;
    default:
      alert("Not implemented:  type=" + node.nodeType);
  	}
	};

	obj_docrange_proto.__defineGetter__("htmlText", function () {
		return KT_getHTML(this.cloneContents(), false);
	});
	obj_docrange_proto.__defineGetter__("text", function () {
		return this.toString();//this.cloneContents().text;
	});

	obj_docbody_proto.createTextRange = function () {
  	if (obj.contentWindow.getSelection().rangeCount > 0) {
    	return obj.contentWindow.getSelection().getRangeAt(0);
  	} else
    	return null;
	};

	obj_docbody_proto.createControlRange = function () {
  	if (obj.contentWindow.getSelection().rangeCount > 0) {
    	return obj.contentWindow.getSelection().getRangeAt(0);
  	} else
    	return null;
	};


	// add the specific methods to the events
	obj_doc_event_proto.__defineSetter__("returnValue", _returnValue);
	obj_doc_event_proto.__defineSetter__("cancelBubble", _cancelBubble);
	obj_doc_event_proto.__defineGetter__("srcElement", _srcElement);
	obj_doc_event_proto.__defineGetter__("fromElement",_fromElement);
	obj_doc_event_proto.__defineGetter__("toElement",_toElement);
	obj_doc_event_proto.__defineGetter__("offsetX", _offsetX);
	obj_doc_event_proto.__defineGetter__("offsetY", _offsetY);
	//parentElement
	obj_doc_htmlel_proto.__defineGetter__("parentElement", _parentElement);
	// el.children()	
	obj_doc_htmlel_proto.__defineGetter__("children", _children);
	//contains()	
	obj_doc_htmlel_proto.contains =_contains();

	obj_doc_htmlel_proto.__defineSetter__("outerHTML", function (sHTML) {
	   var r = this.ownerDocument.createRange();
	   r.setStartBefore(this);
	   var df = r.createContextualFragment(sHTML);
	   this.parentNode.replaceChild(df, this);
	   
	   return sHTML;
	});
	obj_doc_htmlel_proto.__defineGetter__("outerHTML", function () {
		var attr, attrs = this.attributes;
		var str = "<" + this.tagName;
		for (var i = 0; i < attrs.length; i++) {
			attr = attrs[i];
			if (attr.specified)
				str += " " + attr.name + '="' + attr.value + '"';
		}
		if (!this.canHaveChildren)
			return str + ">";
		
		return str + ">" + this.innerHTML + "</" + this.tagName + ">";
	});


	// Add a css rule
	//obj_doc_proto.addRule = _addRule;
	//obj_doc_htmlel_proto.addRule = _addRule;
	//obj_doc_style_proto.addRule = _addRule();
}

/*
mozilla stylesheet stufy

document.getElementById('tssxyz').sheet.insertRule('P { fontSize: 25pt }', 0);
document.getElementById('tssxyz').sheet.cssRules.length
*/

function _createStyleSheet(xmlURI) {
  // load the xml
	var theHeadNode = this.getElementsByTagName("head")[0];
	var theStyleNode = this.createElement('style');
	theStyleNode.type ="text/css"
	theStyleNode.rules = new Array();

	theHeadNode.appendChild(theStyleNode);

	if (xmlURI != "") {
		var xmlHttp = new XMLHttpRequest();
		xmlHttp.open("GET", xmlURI, false);
		xmlHttp.send(null);
		var theTextNode = this.createTextNode(xmlHttp.responseText);
		theStyleNode.appendChild(theTextNode);
		
		var re = /\s*\{[^\}]*\}\s*/;
		nameList = xmlHttp.responseText.split (re);
		for(var i=0; i<nameList.length; i++) {
			var rul = new Object();
			rul.selectorText = nameList[i];
			theStyleNode.rules.push(rul);
		}
		
	} else {
		var theTextNode = this.createTextNode('u');
		theStyleNode.appendChild(theTextNode);
	}
	return theStyleNode;
}

function _addRule(itemName, itemStyle) {
	var theStyleNode = this.getElementsByTagName("style")[0];
	var theTextNode = this.createTextNode(itemName + " { " + itemStyle + " }");
	theStyleNode.appendChild(theTextNode);
}

function _rngparentElement () {
	var parent = this.commonAncestorContainer;
	if (parent.nodeType == 3) {
		return parent.parentNode;
	} else {
		return parent;
	}
}

function _addElement(node) {
	var kids = node.parentNode.childNodes;
	var ix = 0;
	for(var ix=0; ix<kids.length, kids[ix]!=node; ix++);
	this.setStart(node.parentNode, ix);
	this.setEnd(node.parentNode, ix+1);
}

function _moveToElementText(node) {
  var first = this.findFirstTextNode(node);
  if(!first) {
  	first = node;
  }
  var last = this.findLastTextNode(node);
  if(!last) {
  	last = first;
  }

  //this = document.createRange();
  this.setEnd(last, last.nodeValue.length);
  this.setStart(first, 0);
}

function _isEqual(rng) {
  return this.startContainer == rng.startContainer &&
         this.startOffset == rng.startOffset &&
         this.endContainer == rng.endContainer &&
         this.endOffset == rng.endOffset;
}


function _returnValue (b) {
	if (!b) this.preventDefault();
	return b;
}

function _getType() {
	if (this.rangeCount == 0) 
		return "None";
	else
		return "Text";
}


function _cancelBubble () {
	var node = this.target;
	while (node.nodeType != 1) node = node.parentNode;
	return node;
}

function _srcElement () {
	var node = this.target;
	while (node.nodeType != 1) node = node.parentNode;
	return node;
}

function _fromElement () {
	var node;
	if (this.type == "mouseover")
		node = this.relatedTarget;
	else if (this.type == "mouseout")
		node = this.target;
	if (!node) return;
	while (node.nodeType != 1) node = node.parentNode;
	return node;
}

function _toElement () {
	var node;
	if (this.type == "mouseout")
		node = this.relatedTarget;
	else if (this.type == "mouseover")
		node = this.target;
	if (!node) return;
	while (node.nodeType != 1) node = node.parentNode;
	return node;
}

function _offsetX() {
	return this.layerX;
}

function _offsetY() {
	return this.layerY;
}

function _parentElement() {
	if (this.parentNode == this.ownerDocument) 
		return null;
	return this.parentNode;
}

function _selection() {
	if (this.getSelection() == null)
		return null;
	return this.getSelection();
}

function _createRange() {
	var sel = this;
	if(sel.rangeCount == 0) {
		alert(_createRange.caller.name);
	}
	if (sel) {
		return this.getRangeAt(0);
	} else {
		return this.ownerDocument.createRange();
	}	
}


function _children () {
	var tmp = [];
	var j = 0;
	var n;
	for (var i = 0; i < this.childNodes.length; i++) {
		n = this.childNodes[i];
		if (n.nodeType == 1) {
			tmp[j++] = n;
			if (n.name) {	// named children
				if (!tmp[n.name])
					tmp[n.name] = [];
				tmp[n.name][tmp[n.name].length] = n;
			}
			if (n.id)		// child with id
				tmp[n.id] = n
		}
	}
	return tmp;
}

function _contains (oEl) {
	if (oEl == this) return true;
	if (oEl == null) return false;
	return this.contains(oEl.parentNode);		
}

Window.prototype.__defineGetter__("selection", _selection);
Window.prototype.createRange = _createRange;

/*

// Create a new stylesheet, or load a .css file
HTMLDocument.prototype.createStyleSheet = _createStyleSheet;
// Add a css rule
//HTMLStyleElement.prototype.addRule = _addRule();

// add the specific methods to the events
Event.prototype.__defineSetter__("returnValue", _returnValue);
Event.prototype.__defineSetter__("cancelBubble", _cancelBubble);
Event.prototype.__defineGetter__("srcElement", _srcElement);
Event.prototype.__defineGetter__("fromElement",_fromElement);
Event.prototype.__defineGetter__("toElement",_toElement);
Event.prototype.__defineGetter__("offsetX", _offsetX);
Event.prototype.__defineGetter__("offsetY", _offsetY);
// attachEvent
HTMLDocument.prototype.attachEvent = 
HTMLElement.prototype.attachEvent = _attachEvent;
// detachEvent
HTMLDocument.prototype.detachEvent = 
HTMLElement.prototype.detachEvent = _detachEvent;
//parentElement
*/
HTMLElement.prototype.__defineGetter__("parentElement", _parentElement);
// el.children()	
HTMLElement.prototype.__defineGetter__("children", _children);
//contains()	
HTMLElement.prototype.contains =_contains();

function KT_getHTML(root, outputRoot) {
	function encode(str) {
		// we don't need regexp for that, but.. so be it for now.
		str = str.replace(/&/ig, "&amp;");
		str = str.replace(/</ig, "&lt;");
		str = str.replace(/>/ig, "&gt;");
		str = str.replace(/\"/ig, "&quot;");
		return str;
	};
	var html = "";
	switch (root.nodeType) {
	    case 1: // Node.ELEMENT_NODE
	    case 11: // Node.DOCUMENT_FRAGMENT_NODE
		var closed;
		var i;
		if (outputRoot) {
			closed = (!(root.hasChildNodes() || (" script style div span ".indexOf(" " + root.tagName.toLowerCase() + " ") != -1)));
			html = "<" + root.tagName.toLowerCase();
			var attrs = root.attributes;
			for (i = 0; i < attrs.length; ++i) {
				var a = attrs.item(i);
				if (!a.specified) {
					continue;
				}
				var name = a.name.toLowerCase();
				if (name.substr(0, 4) == "_moz") {
					continue;
				}
				var value;
				if (name != 'style') {
					value = a.value;
				} else {
					value = root.style.cssText.toLowerCase();
				}
				if (value.substr(0, 4) == "_moz") {
					continue;
				}
				html += " " + name + '="' + value + '"';
			}
			html += closed ? " />" : ">";
		}
		for (i = root.firstChild; i; i = i.nextSibling) {
			html += KT_getHTML(i, true);
		}
		if (outputRoot && !closed) {
			html += "</" + root.tagName.toLowerCase() + ">";
		}
		break;
	    case 3: // Node.TEXT_NODE
		html = encode(root.data);
		break;
	    case 8: // Node.COMMENT_NODE
		html = "<!--" + root.data + "-->";
		break;		// skip comments, for now.
	}
	return html;
}; 

