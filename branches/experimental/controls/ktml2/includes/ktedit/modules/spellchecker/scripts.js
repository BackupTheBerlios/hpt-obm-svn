// Copyright 2001-2003 Interakt Online. All rights reserved.
	
function SpellChecker(owner, defaultLanguage) {
	this.owner = owner;	
	var name = owner.name;
	this.language = defaultLanguage;
	this.location = false;
	this.dialects = document.getElementById(name+"_spellcheck_dialect");
	this.wnd = null;
}	

SpellChecker.prototype.initsp = SpellChecker_initsp;
SpellChecker.prototype.invalidate = SpellChecker_invalidate;
SpellChecker.prototype.setLanguage = SpellChecker_setLanguage;

function SpellChecker_initsp(win, counter) {
	if ((this.wnd != null) && (win == null)) {
		this.wnd.close();
		this.wnd = null;
	}
	if (win && (win != this.wnd)) {
		this.wnd = win;
	}
		this.owner.util_loadBody(hndlr_save(this.owner.edit.body.innerHTML));
	var args="width=350,height=260,status=1,scrollbars=no";
	var urlParam = "";
	if (this.wnd) {
					urlParam = this.location+"?counter="+counter+"&dialect="+this.language;
	} else {
					urlParam = NEXT_ROOT+"includes/ktedit/modules/spellchecker/spellcheck.php?counter="+counter+"&dialect="+this.language;
	}
	if (this.wnd) {
					this.wnd.location = urlParam;
	} else {
					this.wnd = window.open(urlParam, "", args);
					this.location = this.wnd.location.href;
	}
	this.wnd.focus();
	return true;
}

function SpellChecker_invalidate() { 
	return true;
}

function SpellChecker_setLanguage(language,win, counter) {
	this.language = language; 
	if (this.wnd) {
		if(this.location == 'about:blank') {
			this.location = this.wnd.location.href.match(/^(.*spellcheck\.php).*$/)[1];
		}
		this.invalidate();
		this.initsp(win, counter);
	} else {
		this.initsp(win, counter);
	}
}
