// Copyright 2001-2004 Interakt Online. All rights reserved.
	

/**
	class PropertiesUI
*/


function PropertiesUI(owner) {
	this.owner = owner;
	
	this.name = owner.name;
	this.tooperate = '';
	
	this.boundSize = false;
	this.ratio = 1;
	
	this.isLoaded = false;
	
	this.none = 'Properties_none_' + this.name;
	this.td = 'Properties_td_' + this.name;
	this.tr = 'Properties_tr_' + this.name;
	this.table = 'Properties_table_' + this.name;
	this.img = 'Properties_img_' + this.name;

	this.td_width = "Properties_td_width_"+this.name;
	this.td_height = "Properties_td_height_"+this.name;
	this.td_align = "Properties_td_align_"+this.name;
	this.td_valign = "Properties_td_valign_"+this.name;
	this.td_bgcolor = "Properties_td_bgcolor_"+this.name;
	this.td_brdcolor = "Properties_td_brdcolor_"+this.name;
	this.td_nowrap = "Properties_td_nowrap_"+this.name;

	this.tr_align = "Properties_tr_align_"+this.name;
	this.tr_valign = "Properties_tr_valign_"+this.name;
	this.tr_bgcolor = "Properties_tr_bgcolor_"+this.name;
	this.tr_brdcolor = "Properties_tr_brdcolor_"+this.name;
	
	this.table_width = "Properties_table_width_"+this.name;
	this.table_height = "Properties_table_height_"+this.name;
	this.table_cellpadding = "Properties_table_cellpadding_"+this.name;
	this.table_cellspacing = "Properties_table_cellspacing_"+this.name;
	this.table_border = "Properties_table_border_"+this.name;
	this.table_align = "Properties_table_align_"+this.name;
	this.table_brdcolor = "Properties_table_brdcolor_"+this.name;
	this.table_valign = "Properties_table_valign_"+this.name;
	this.table_bgcolor = "Properties_table_bgcolor_"+this.name;
	
	this.img_width = "Properties_img_width_"+this.name;
	this.img_hspace = "Properties_img_hspace_"+this.name;
	this.img_align = "Properties_img_align_"+this.name;
	this.img_border = "Properties_img_border_"+this.name;
	this.img_height = "Properties_img_height_"+this.name;
	this.img_vspace = "Properties_img_vspace_"+this.name;
	this.img_alt = "Properties_img_alt_"+this.name;
		
}

// static methods
function PropertiesUI_intValue(el) {
	try {
		var val = parseInt(el.value);
		if(isNaN(val)) {
			el.value = '';
		} else {
			el.value = val;
		}
	} catch (e) {
		el.value = '';
	}
}

function PropertiesUI_colorValue(el) {
	try {
		var val = el.value;
		if(val== '' || val.search(/^#[0-9abcdef]{0,6}$/i) != -1) {
			el.goodValue = val;
		} else {
			if(el.goodValue) {
				el.value = el.goodValue;
			} else {
				el.value = '';
			}
		}
	} catch (e) {
		el.value = '';	
	}
}

// public methods
PropertiesUI.prototype.update = PropertiesUI_update;
PropertiesUI.prototype.prop_changed = PropertiesUI_prop_changed;
PropertiesUI.prototype.chooseBgColor = PropertiesUI_chooseBgColor;
PropertiesUI.prototype.chooseBrdColor = PropertiesUI_chooseBrdColor;
PropertiesUI.prototype.selectOption = PropertiesUI_selectOption;
PropertiesUI.prototype.clear = PropertiesUI_clear;
PropertiesUI.prototype.prop_getDimensions = PropertiesUI_prop_getDimensions;
PropertiesUI.prototype.boundImgSize = PropertiesUI_boundImgSize;
PropertiesUI.prototype.syncHSize = PropertiesUI_syncHSize;
PropertiesUI.prototype.syncWSize = PropertiesUI_syncWSize;

function PropertiesUI_clear() {
      var hideArr = [this.td, this.tr, this.table, this.img];
      var i, tmp;
      for (i=0;i<hideArr.length;i++) {
            tmp = document.getElementById(hideArr[i]);
            if (tmp) {
                  tmp.style.display='none';
            }
      }
}

/**
* update properties for a specific tag in property editor
*
* @param
*	obj - ktml object
*/
function PropertiesUI_update(){
	if(!this.isLoaded) {
		util_loadIframe(this.name+'_buffer', NEXT_ROOT+"includes/ktedit/modules/introspection/ui."+Ktml_ext+'?ktmlname='+this.name+'&counter='+this.owner.pageId+'&dirDepth='+NEXT_ROOT+'&language='+this.owner.UILanguage);
		return;
	}
	this.clear();
	
	if (this.owner.selectableNodes && this.owner.selectableNodes[0]) {
		try {
			if (   this.owner.selectableNodes[0].tagName == 'TD' 
					|| this.owner.selectableNodes[0].tagName == 'TR' 
					|| this.owner.selectableNodes[0].tagName == 'TABLE' 
					|| this.owner.selectableNodes[0].tagName == 'IMG' 
				) {
					this.tooperate = this.owner.selectableNodes[0];
				} else {
					this.tooperate = null;
					var i=0;
					while (this.owner.selectableNodes[i].tagName != 'TD' &&
							this.owner.selectableNodes[i].tagName != 'A' &&
							i < this.owner.selectableNodes.length) {
						i++;
					}
					if(i<this.owner.selectableNodes.length && this.owner.selectableNodes[i].tagName == 'TD') {
						this.tooperate = this.owner.selectableNodes[i];
					}
				}
		} catch (e) {
		}
		
		this.owner.inspectedNode = this.tooperate;
		
		if (!this.tooperate) 
			return;
			
		if (this.tooperate.tagName == 'TD') {
			this.prop_getDimensions();

			util_setInput(document.getElementById(this.td_width), this.tooperate.getAttribute("width"));
			util_setInput(document.getElementById(this.td_height), this.tooperate.getAttribute("height"));
			this.selectOption(document.getElementById(this.td_align), this.tooperate.getAttribute("align"));
			this.selectOption(document.getElementById(this.td_valign), this.tooperate.getAttribute("vAlign"));
			util_setInput(document.getElementById(this.td_bgcolor), this.tooperate.getAttribute("bgColor"));
			util_setInput(document.getElementById(this.td_brdcolor), this.tooperate.getAttribute("borderColor"));
			if (this.tooperate.noWrap) {
				document.getElementById(this.td_nowrap).checked = true;
			} else {
				document.getElementById(this.td_nowrap).checked = false;
			}
			document.getElementById(this.td).style.display='block';
		}

		if (this.tooperate.tagName == 'TR') {
			this.selectOption(document.getElementById(this.tr_align), this.tooperate.getAttribute("align"));
			this.selectOption(document.getElementById(this.tr_valign), this.tooperate.getAttribute("vAlign"));
			util_setInput(document.getElementById(this.tr_bgcolor), this.tooperate.getAttribute("bgColor"));
			util_setInput(document.getElementById(this.tr_brdcolor), this.tooperate.getAttribute("borderColor"));
			document.getElementById(this.tr).style.display='block';
		}

		if (this.tooperate.tagName == 'TABLE') {
			this.prop_getDimensions();
			
			util_setInput(document.getElementById(this.table_width), this.tooperate.getAttribute("width"));
			util_setInput(document.getElementById(this.table_height), this.tooperate.getAttribute("height"));
			util_setInput(document.getElementById(this.table_cellpadding), this.tooperate.getAttribute("cellPadding"));
			util_setInput(document.getElementById(this.table_cellspacing), this.tooperate.getAttribute("cellSpacing"));
			util_setInput(document.getElementById(this.table_border), this.tooperate.getAttribute("border"));
			this.selectOption(document.getElementById(this.table_align), this.tooperate.getAttribute("align"));
			util_setInput(document.getElementById(this.table_brdcolor), this.tooperate.getAttribute("borderColor"));
			util_setInput(document.getElementById(this.table_bgcolor), this.tooperate.getAttribute("bgColor"));

			document.getElementById(this.table).style.display='block';
		}

		if (this.tooperate.tagName == 'IMG') {
			this.prop_getDimensions();
			
			this.ratio = this.tooperate.height/this.tooperate.width;

			util_setInput(document.getElementById(this.img_width), this.tooperate.getAttribute("width"));
			util_setInput(document.getElementById(this.img_hspace), this.tooperate.getAttribute("hspace"));
			document.getElementById(this.img_align).selectedIndex = this.tooperate.getAttribute("align");
			this.selectOption(document.getElementById(this.img_align), this.tooperate.getAttribute("align"));			
			util_setInput(document.getElementById(this.img_border), this.tooperate.getAttribute("border"));
			util_setInput(document.getElementById(this.img_height), this.tooperate.getAttribute("height"));
			util_setInput(document.getElementById(this.img_vspace), this.tooperate.getAttribute("vspace"));
			util_setInput(document.getElementById(this.img_alt), this.tooperate.getAttribute("alt"));

			document.getElementById(this.img).style.display='block';
		}
	}
}

/**
* move dimensions from style to width and heigh attributes
*
*/
function PropertiesUI_prop_getDimensions() {
	if(this.owner.mozilla) {
		//return;
	}
	if (this.tooperate.style.width) {
		if(this.tooperate.style.pixelWidth) {
			this.tooperate.width = this.tooperate.style.pixelWidth;
		} else {
			var str = this.tooperate.style.width;
			this.tooperate.width = str.substring(0, str.length-2);
		}
		this.tooperate.style.width = '';
	}
	if (this.tooperate.style.height) {
		if(this.tooperate.style.pixelHeight) {
			this.tooperate.height = this.tooperate.style.pixelHeight;
		} else {
			var str = this.tooperate.style.height;
			this.tooperate.height = str.substring(0, str.length-2);			
		}
		this.tooperate.style.height = '';
	}
}

/**
* change a tag property
*
* @param
*	propName - property name
*	propValue - property value
*/
function PropertiesUI_prop_changed(propName, propValue){
	if (this.owner.selectableNodes && this.owner.selectableNodes[0]) {
		if (propValue != '' && propValue != 'false') {
			propValue = String(propValue);
			propValue = propValue.replace(/'/g, "\\'");
			this.tooperate.setAttribute(propName, propValue);
		} else {
			this.tooperate.removeAttribute(propName);
		}

		if (this.tooperate.tagName == 'IMG') {
			if(this.boundsize && (propName == 'height' || propName == 'width')) {
				if(propName == 'height') {
					if(propValue == '') {
						this.tooperate.removeAttribute('width');
					} else {
						this.tooperate.width = this.tooperate.height/this.ratio;
					}
					//util_setInput(document.getElementById(this.img_width), this.tooperate.getAttribute("width"));
				} else {
					if(propValue == '') {
						this.tooperate.removeAttribute('height');
					} else {
						this.tooperate.height = this.tooperate.width*this.ratio;
					}
					//util_setInput(document.getElementById(this.img_height), this.tooperate.getAttribute("height"));
				}
			} else if(propName == 'alt') {
				// set also the 'title' attribute
				if (propValue != '') {
					this.tooperate.setAttribute('title', propValue);
				} else {
					this.tooperate.removeAttribute('title');
				}
			}
			
		} else {
			if (
				this.tooperate.tagName == 'TABLE' && 
				propName == 'border'
			) {
				if (this.owner.viewinvisibles == true) {
					if (propValue == '' || propValue == 0) {
						this.owner.utils_makeTableOutline(this.tooperate);
					} else {
						this.owner.utils_unmakeTableOutline(this.tooperate);
					}
				} else {
					
				}
			}
		}
		// RST - removed as after one change the focus is lost from inspector
		// and when entering the new value -  0003834
		//this.owner.cw.focus();
		if(this.owner.undo) {
			this.owner.undo.addEdit();
		}
	}
}

function PropertiesUI_chooseBgColor(element, idu) {
	if (this.owner.cpalette.isVisible) {
		this.owner.cpalette.setVisible(false);
	} else {
		var p = lay_getAbsolutePos(element);
		this.owner.cpalette.setAction("util_setInput(document.getElementById('"+idu+"'), selectedColor);this.owner.properties.prop_changed('bgColor', document.getElementById('"+idu+"').value);");
		this.owner.cpalette.setSelected(document.getElementById(idu).value);
		this.owner.cpalette.showAt(p.x, p.y + element.offsetHeight - 271);
		this.owner.cpalette.setVisible(true);
		//this.owner.cpalette.setSelected();
	}
	return false;
}

function PropertiesUI_chooseBrdColor(element, idu) {
	if (this.owner.cpalette.isVisible) {
		this.owner.cpalette.setVisible(false);
	} else {
		var p = lay_getAbsolutePos(element);
		this.owner.cpalette.setAction("util_setInput(document.getElementById('"+idu+"'), selectedColor);this.owner.properties.prop_changed('borderColor', document.getElementById('"+idu+"').value);");
		this.owner.cpalette.setSelected(document.getElementById(idu).value);
		this.owner.cpalette.showAt(p.x, p.y + element.offsetHeight - 271);
		this.owner.cpalette.setVisible(true);
	}
	return false;	
}

/**
* set constrains on image size
*
* @param
*	element - dom element - image on inspector 
*	relpath - string - relative path to images
*/
function PropertiesUI_boundImgSize(element, relpath) {
	var obj = this;
	
	if(!this.syh) {
		this.syh = function (e) { obj.syncHSize(e); };
		this.syw = function (e) { obj.syncWSize(e); };
	}
	
	var iw = document.getElementById(this.img_width);
	var ih = document.getElementById(this.img_height)
	if(this.boundsize) {
		this.boundsize = false;
		element.setAttribute('src', relpath+'/ubound.gif');
		if(iw.removeEventListener) {
			iw.removeEventListener('keyup', this.syh, true);
			ih.removeEventListener('keyup', this.syw, true);
		} else {
			iw.detachEvent('onkeyup', this.syh);
			ih.detachEvent('onkeyup', this.syw);
		}
	} else {
		this.boundsize = true;
		element.setAttribute('src', relpath+'/bound.gif');
		this.ratio = this.tooperate.height/this.tooperate.width;
		// event
		if(iw.addEventListener) {
			iw.addEventListener('keyup', this.syh, true);
			ih.addEventListener('keyup', this.syw, true);
		} else {
			iw.attachEvent('onkeyup', this.syh);
			ih.attachEvent('onkeyup', this.syw);
		}
	}
}

var di_LEFT = 37;
var di_RIGHT = 39;

/**
* Synchronize width with height in real time
*
* @param
*	e - event
*/
function PropertiesUI_syncWSize(e) {
	if (e.keyCode == di_LEFT || e.keyCode == di_RIGHT) {
		return;
	}

	var ih = document.getElementById(this.img_height);
	try {
		ih.value = parseInt(ih.value);
		var val = Math.floor(ih.value/this.ratio);
		if(isNaN(val)) {
			val = '';
			ih.value = '';
		}
		util_setInput(document.getElementById(this.img_width), val);
	} catch (e) {
		ih.value = '';
	}
}

/**
* Synchronize width with height in real time
*
* @param
*	e - event
*/
function PropertiesUI_syncHSize(e) {
	if (e.keyCode == di_LEFT || e.keyCode == di_RIGHT) {
		return;
	}
	var iw = document.getElementById(this.img_width);
	try {
		iw.value = parseInt(iw.value);
		var val = Math.floor(iw.value*this.ratio);
		if(isNaN(val)) {
			val = '';
			iw.value = '';
		}
		util_setInput(document.getElementById(this.img_height), val);
	} catch (e) {
		iw.value = '';
	}
}

/**
* selects an option in a html dropdown
*
* @param
*	select - dropdown object
*	option - the value to be selected
*/
function PropertiesUI_selectOption(select, option) {
	for (i=0; i<select.options.length; i++) {
		if(select.options[i].value==option) {
			select.options[i].selected = true;
			return;
		}
	}
	// select default option
	select.options[0].selected = true;
}
