// Copyright 2001-2004 Interakt Online. All rights reserved.
	

/**
	class PropertiesLink
*/


function PropertiesLink(owner) {
	this.owner = owner;
	var name = owner.name;
	this.a = 'Properties_a_' + name;
	this.none = 'Properties_none_' + name;
	this.a_name = "Properties_a_name_"+name;
	this.a_href = "Properties_a_href_"+name;
	this.a_target = "Properties_a_target_"+name;
	this.a_title = "Properties_a_title_"+name;

	this.tooperate = '';
	
}

// public methods
PropertiesLink.prototype.showNone = PropertiesLink_showNone;
PropertiesLink.prototype.hideNone = PropertiesLink_hideNone;
PropertiesLink.prototype.update = PropertiesLink_update;
PropertiesLink.prototype.href_changed = PropertiesLink_href_changed;
PropertiesLink.prototype.name_changed = PropertiesLink_name_changed;
PropertiesLink.prototype.target_changed = PropertiesLink_target_changed;
PropertiesLink.prototype.chooseFileLink = PropertiesLink_chooseFileLink;
PropertiesLink.prototype.selectOption = PropertiesLink_selectOption;
PropertiesLink.prototype.clear = PropertiesLink_clear;
PropertiesLink.prototype.hndlr_keyDown = PropertiesLink_hndlr_keyDown;

PropertiesLink.prototype.name_focus = PropertiesLink_name_focus;
PropertiesLink.prototype.href_focus = PropertiesLink_href_focus;
PropertiesLink.prototype.title_changed = PropertiesLink_title_changed;

function PropertiesLink_clear() {
	document.getElementById(this.a).style.display='none';
}

/**
* update properties for a specific tag in property editor
*
* @param
*	obj - ktml object
*/
function PropertiesLink_showNone() {
	document.getElementById(this.none).style.display='block';
}
function PropertiesLink_hideNone() {
	document.getElementById(this.none).style.display='none';
}

function PropertiesLink_update(){
	this.clear();
	if (this.owner.selectableNodes && this.owner.selectableNodes[0]) {
		try {
			if (
				this.owner.selectableNodes[0].tagName == 'A' ||
				(this.owner.useIntrospection && this.owner.inspectedNode != null) 
			) {
				this.hideNone();
				this.tooperate = this.owner.selectableNodes[0];
			} else {
				this.tooperate = null;
				var i=0;
				while ((this.owner.selectableNodes[i].tagName == 'SPAN' ||
						this.owner.selectableNodes[i].tagName == 'FONT')
						&& i < this.owner.selectableNodes.length) {
					i++;
				}
				if(i<this.owner.selectableNodes.length && this.owner.selectableNodes[i].tagName == 'A') {
					this.hideNone();
					this.tooperate = this.owner.selectableNodes[i];
				} else {
					this.showNone();
				}
			}
		} catch (e) {
			this.showNone();
		}	
		
		if (!this.tooperate) 
			return;
			
		this.owner.inspectedNode = this.tooperate;
			
		if (this.tooperate.tagName == 'A') {
			document.getElementById(this.a).style.display='block';
			//this.a_name.value = tooperate.getAttribute("name");
			//this.a_href.value = tooperate.getAttribute("href");
			var tmp = this.tooperate.getAttribute("href");
			if(tmp) {
				tmp = util_removeURLfromHref(tmp);
			} else {
				tmp = '';
			}
			
			util_setInput(document.getElementById(this.a_href), tmp);
			util_setInput(document.getElementById(this.a_name), this.tooperate.getAttribute("name"));
			util_setInput(document.getElementById(this.a_title), this.tooperate.getAttribute("title"));
			this.selectOption(document.getElementById(this.a_target), this.tooperate.getAttribute("target"));
		}
	} else {
		this.showNone();
	}
}

/**
	check if the href has some value. If yes ask the user to confirm the change.
	Called when Name field got focus
	
	@params
	msg - message for the user
	
	@return
		none
*/
function PropertiesLink_name_focus(msg) {

	var hrefNode = document.getElementById(this.a_href);
	
	if (hrefNode.value) {
		var change = false;
		if (hrefNode.value == "#" || hrefNode.value == "http://") {
			change = true;
		} else {
			change = window.confirm(msg);
		}
		if (change) {
			hrefNode.value = '';
		} else {
			hrefNode.focus();
		}
	}
	
}

/**
	check if the name has some value. If yes ask the user to confirm the change.
	Called when href field got focus
	
	@params
	msg - message for the user
	
	@return
		none
*/
function PropertiesLink_href_focus(msg) {
	
	var nameNode = document.getElementById(this.a_name);
	
	if (nameNode.value) {
		if (window.confirm(msg)) {
			nameNode.value = '';
		} else {
			nameNode.focus();
		}
	}
}

/**
* change the href property
*
* @param
*	propValue - property value
*/
function PropertiesLink_href_changed(propValue){

	//var nameNode = document.getElementById(this.a_name);

	
	if (propValue != '' && propValue != 'false') {
		propValue = String(propValue);
		propValue = propValue.replace(/'/g, "\\'");
		if (Ktml_mozilla) {
			this.tooperate.setAttribute("href", propValue);
			this.tooperate.removeAttribute("name");
		} else {
			this.tooperate.setAttribute("href", propValue);
			this.tooperate.setAttribute("name", ''); // work around a bug in IE
			this.tooperate.removeAttribute('name');
			this.tooperate.removeAttribute('NAME');
		}
	} else {
		this.tooperate.removeAttribute('href');
		if ((this.tooperate.getAttribute("name") == undefined) || (this.tooperate.getAttribute("name") == '')) {
			if (this.owner.svdSelection) {
				this.owner.util_restoreSelection();
			} else {
				this.owner.cw.focus();
			}
			//this.owner.util_restoreSelection();
			this.owner.logic_updateDOMHierarchy(true, 0);
			this.tooperate.outerHTML = this.tooperate.innerHTML;
			this.owner.logic_removeTag(1);
			
			this.owner.logic_updateDOMHierarchy(true, 0);
			this.owner.propertieslink.update();

		}

	}
		
		//LA CE FOLOSESTE ASTA?
		//Comentat de MPR ca sa prinda focusul Name-ul.
/*  		if (this.owner.svdSelection) {
			this.owner.util_restoreSelection();
		} else {
			this.owner.cw.focus();
		}
 */				
	if(this.owner.undo) {
		this.owner.undo.addEdit();
	}

}

function PropertiesLink_title_changed(propValue) {
	if (propValue != '' && propValue != 'false') {
		propValue = String(propValue);
		propValue = propValue.replace(/'/g, "\\'");
		this.tooperate.setAttribute("title", propValue);
	} else {
		this.tooperate.removeAttribute('title');
	}
}

/**
* change the name property
*
* @param
*	propValue - property value
*/
function PropertiesLink_name_changed(propValue){
	
	
	if (propValue != '' && propValue != 'false') {
		propValue = String(propValue);
		propValue = propValue.replace(/'/g, "\\'");
		if (Ktml_mozilla) {
			this.tooperate.setAttribute("name", propValue);
			this.tooperate.name = propValue;
			this.tooperate.removeAttribute("href");
		} else {
			this.tooperate.removeAttribute('href');
			this.tooperate.removeAttribute('NAME');
			this.tooperate.removeAttribute('name');
			// dirty trick for a workaround of a IE bug (that doesn't set attribute name)
			this.tooperate.setAttribute('NAME', propValue); // this will set the text
			this.tooperate.setAttribute('name', propValue); // this will set the dom 
		}
	} else {
		this.tooperate.removeAttribute('name');
		this.tooperate.removeAttribute('NAME');
		if (this.tooperate.getAttribute('href') == undefined || this.tooperate.getAttribute('href') == '') {
			if (this.owner.svdSelection) {
				this.owner.util_restoreSelection();
			} else {
				this.owner.cw.focus();
			}
			this.tooperate.outerHTML = this.tooperate.innerHTML;
			this.owner.logic_updateDOMHierarchy(true, 0);
		}
	}
	if(this.owner.undo) {
		this.owner.undo.addEdit();
	}
}

/**
* change the href property
*
* @param
*	propValue - property value
*/
function PropertiesLink_target_changed(propValue){
	propName = 'target';
	if (propValue != '' && propValue != 'false') {
		propValue = String(propValue);
		propValue = propValue.replace(/'/g, "\\'");
		this.tooperate.setAttribute(propName, propValue);
		if (propName == 'name' && tooperate.tagName == 'A') {
			this.tooperate.setAttribute(propName, propValue);
		}
	} else {
		this.tooperate.removeAttribute(propName);
	}
//	if (this.owner.svdSelection) {
//		this.owner.util_restoreSelection();
//	}

//	this.owner.cw.focus();	
	if(this.owner.undo) {
		this.owner.undo.addEdit();
	}
}


function PropertiesLink_chooseFileLink(element, idu, counter) {
	var pth = this.owner.pathToFileDir;
	util_openwnd("uploadImage",NEXT_ROOT+"includes/ktedit/dirbrowser."+Ktml_ext+"?mode=img&submode=file&elname="+idu+"&ktmlid="+this.owner.pageId + '&counter=' + counter, 700, 540);
}

/**
* selects an option in a html dropdown
*
* @param
*	select - dropdown object
*	option - the value to be selected
*/
function PropertiesLink_selectOption(select, option) {
	for (i=0; i<select.options.length; i++) {
		if(select.options[i].value==option) {
			select.options[i].selected = true;
			return;
		}
	}
	// select default option
	select.options[0].selected = true;
}

function PropertiesLink_hndlr_keyDown(e) {
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
