/*** Written by Tran Kien Duc 2003-05                 */
/*** Original by Vnsign  http://www.vnsign.com      */
/*** Release 2004-01-07  R14.0                      */
/****************************************************/

// modules: make sure the URIs are correct!:
var vuspellaURI= "vuspella.js";
var vuspellbURI= "vuspellb.js";
var vumapsURI  = "vumaps.js";
var vuspella = 0;
var vuspellb = 0;
var vumaps = 0;

// interface for HTML:
//

function VNOff() { return 0; }
var disabled = false;
var charmapid = 1;
var typingMode = VNOff;
var theTyper = null;

var spellerror = 0;
var buffer = "";
var restorewd = 1;
var linebreak = 0;
function resetSChecker(rflag){ spellerror=0; buffer=""; restorewd=rflag; }

// for HTML-calls:
//---------------------

telexingVietUC = initTyper;

// Set Vietnamese typing for textboxs on Form:
//
function initTypingForm()
{
	for (j=0;j<document.forms.length;j++)
	{
		frm = document.forms[j];
		for (i=0;i<frm.length;i++)
		{
			if (frm.elements[i].type=="text" || frm.elements[i].type=="textarea")
			{
//							alert(frm.elements[i].name);
				frm.elements[i].onkeyup = function(){initTyper(this)};
			}
		}
	}
}

function setTypingMode(mode) {
  switch (mode) {
     case 1: typingMode = telexTyping; break;
     case 2: typingMode = vniTyping; break;
     case 3: typingMode = viqrTyping; break;
     default: typingMode = VNOff;
  }
  if (theTyper) theTyper.typingMode = typingMode;
  if (disabled) return true;
  if (!document.all && !document.getElementById) {
     alert("Xin loi, trinh duyet web cua ban khong cho phep dung VietTyping.\n");
     disabled = true;  
  }
}

function convertAtOnce(txtarea) {
  if(!txtarea) return;
  if(typingMode==VNOff) {
    var msg = "Bo^. go~ ddang o+? tra.ng tha'i ta('t.\n Ba.n pha?i ddu+a ve^` "+
    "kie^?u dda~ du`ng dde^? vie^'t ba`i truo+'c khi soa't da^'u";
    return alert(msg);
  }
  if(!theTyper) theTyper = new CVietString("");
  if(vuspella) txtarea.value = theTyper.scConvert(txtarea.value);
  else txtarea.value = theTyper.doConvertIt(txtarea.value);
}

function loadSpellA (lflag) {
  if (!lflag) return (vuspella= 0);
  if (theTyper && theTyper.checkSpell) return (vuspella=1);
  loadModule(vuspellaURI, "vuspella");
}

function loadSpellB (txtarea, search) {
  if (!txtarea) return;
  if(!vuspellb) {
     loadModule(vuspellbURI, "vuspellb");
     if(vuspellb) document.body.insertAdjacentHTML('beforeEnd', vuspellb);
  }
  if (!theTyper) return; else theTyper.txtarea = txtarea; 
  if (theTyper.theSChecker) theTyper.theSChecker.startCS(search);
}

function setCharMap(mapID) { 
  charmapid = mapID+1; 
  if (!vumaps) loadModule(vumapsURI, "vumaps");
  if (theTyper) theTyper.charmap = initCharMap();
} 

function convertTo(txtarea, destmap) {
  if (!txtarea) return 0;
  if (!vumaps) loadModule(vumapsURI, "vumaps");
  if (!vumaps) return 0;
  var srcmap = initCharMap();
  txtarea.value=srcmap.convertTxtTo(txtarea.value,initCharMap(destmap+1));
  return 1;
}

function detectMap(txtarea) { 
  if (!txtarea) return 0;
  if (!vumaps) loadModule(vumapsURI, "vumaps");
  if (!vumaps) return 0;
  var cm = detectFormat(txtarea.value, 1);
  if (cm) setCharMap(cm-1);
  return cm;
} 

// end from HTML


// init
initCharMap = function() { return new CVietUniCodeMap(); }

function initTyper(txtarea,evt) {
  txtarea.onkeydown = endWordKEvt;
  txtarea.onmousedown = function(evt) { resetSChecker(0); return true; }
  if (document.all) txtarea.onkeypress = ieTyping; 
  else if (document.getElementById) txtarea.onkeypress = ns6Typing;
  else telexingVietUC = VNOff;
  txtarea.onkeyup = null;
}

function loadModule(mURI,idstr) {
  if (!document.all) return alert("Sorry, only IE4,5,6 support this feature.");
  var ls="&nbsp;<script defer type='text/javascript' src='"+mURI+"'></script>";
  document.body.insertAdjacentHTML('beforeEnd', ls);
  if (!eval(idstr)) alert( errormsg);
}
var errormsg="Module chu+a na.p xong, co' the^? ke^'t no^'i cha^.m...\n"+
"Ba.n ha~y thu+.c hie^.n thao ta'c mo^.t la^`n nu+~a!";

// end init


// event & typing:
//------------------------

function endWordKEvt(evt) {
  var c = document.all? event.keyCode: (evt.which? evt.which: 0);
  if ((c==10) || (c==13)) { resetSChecker(1); linebreak= 1; }
  else if ((c<49) && (c!=16) && (c!=20)) { linebreak= 0; resetSChecker(c==32); }
  return true;
}

function ieTyping() {
  var c = event.keyCode;
  if(!theTyper) theTyper = new CVietString(getCurrentWord(this));
  else theTyper.value = getCurrentWord(this);
  if ((c > 32) && theTyper.typing(c)) {
     replaceWord(this, theTyper.value);
     return false;
  }
  return true;
}

function ns6Typing(evt) {
  var c = (evt && evt.which)? evt.which : 0;
  if(!theTyper) theTyper = new CVietString(this.value);
  else theTyper.value = this.value;
  if ((c > 32) && theTyper.typing(c)) {
     this.value = theTyper.value;
     return false;
  }
  return true;
}

// for IE4+,5+: 
function getCurrentWord(txtarea) {
  var caret = txtarea.document.selection.createRange();
  var backward = -17;
  do {
    var caret2 = caret.duplicate();
    caret2.moveStart("character", backward++);
  } while (caret2.parentElement() != txtarea && backward <0);
  txtarea.curword = caret2.duplicate();
  return caret2.text;
}

function replaceWord(txtarea, newword) {
  txtarea.curword.text = newword;
  txtarea.curword.collapse(false);
}
// end IE-special

//////////////////////////////
// end interface


// "class": CVietString
//
function CVietString(str) {
  this.value = str;
  this.charmap = initCharMap();
  this.ctrlchar = '-';
  this.changed = 0;

  this.typing = typing;
  this.typingMode = typingMode;
  this.Dau = Dau;
  this.AEOWD = AEOWD;
  this.findCharToChange = findCharToChange;
  this.doConvertIt = doConvertIt;
  this.checkSpell = null;
  return this;
}

function typing(ctrl) {
  this.changed = 0;
  this.ctrlchar = String.fromCharCode(ctrl);
  buffer += this.ctrlchar;
  if (spellerror) return 0;
  if (linebreak) {
    linebreak= 0;
    if ("fFzZwWjJ".indexOf(this.ctrlchar) >= 0) spellerror= 1;
  }
  else this.typingMode();
  return this.changed;
}

function telexTyping() {
  switch (this.ctrlchar.toLowerCase()) {
    case 's': this.Dau(1); break;  // s, S: sac
    case 'f': this.Dau(2); break;  // f, F: huyen
    case 'j': this.Dau(3); break;  // j, J: nang
    case 'r': this.Dau(4); break;  // r, R: hoi
    case 'x': this.Dau(5); break;  // x, X: nga
    case 'a': this.AEOWD(1); break;
    case 'e': this.AEOWD(2); break;
    case 'o': this.AEOWD(3); break;
    case 'w': this.AEOWD(4); break;
    case 'd': this.AEOWD(5); break;
    case 'z': this.Dau(6); break;  // z, Z: xo'a da^'u
  }
  if ((!this.changed || spellerror) && vuspella) this.checkSpell();
}

function vniTyping() {
  switch (this.ctrlchar) {
    case '1': this.Dau(1); break;  // 1 -> '
    case '2': this.Dau(2); break;  // 2 -> `
    case '5': this.Dau(3); break;  // 5 -> .
    case '3': this.Dau(4); break;  // 3 -> ?
    case '4': this.Dau(5); break;  // 4 -> ~
    case '7': 
    case '8': this.AEOWD(4); break;  // 7,8 -> a(, o+, u+
    case '9': this.AEOWD(5); break;  // 9 -> dd
    case '6': this.AEOWD(1);         // 6 -> a^,e^ or  o^
              if(!this.changed) this.AEOWD(2);
              if(!this.changed) this.AEOWD(3); break;
    case '0': this.Dau(6); break;  // 0 -> xoa dau
  }
}

function viqrTyping() {
  var lc = this.value.charAt(this.value.length-1);
  if ((lc=='\\') && ((this.ctrlchar=='?') || (this.ctrlchar=='.'))) {
    this.value= this.value.substring(0, this.value.length-1)+ this.ctrlchar;
    return (this.changed = 1);
  }
  switch (this.ctrlchar) {
    case '/' : case "'": case String.fromCharCode(180): this.Dau(1); break;
    case '`': this.Dau(2); break;
    case '.': this.Dau(3); break;
    case '?': this.Dau(4); break;
    case '~': this.Dau(5); break;
    case '^': this.AEOWD(1);
              if(!this.changed) this.AEOWD(2);
              if(!this.changed) this.AEOWD(3); break;
    case '(': case '+': case '*': this.AEOWD(4); break;
    case 'd': case 'D': this.AEOWD(5); break;
    case '-': this.Dau(6); break;
  }
}

function doConvertIt(txt) {
  var i = 1, len = txt.length;
  this.value = txt.charAt(0);
  while (i < len) {
    this.ctrlchar = txt.charAt(i++);
    this.changed = 0;
    this.typingMode();
    if (!this.changed) this.value+= this.ctrlchar;
  }
  return this.value;
}

//-------------------

function AEOWD(type) {
  var lc = this.charmap.lastCharsOf(this.value);
  var telex = this.charmap.getAEOWD (lc[0], type);
  if (!(this.changed = telex[0])) return;
  this.value= this.value.replaceAt(this.value.length-lc[1],telex[1],lc[1]);
  if (!telex[2]) { spellerror= 1; this.value+= this.ctrlchar; }
}

function Dau(type) {
  var info = this.findCharToChange(type);
  if (!info) return;
  var telex= this.charmap.getDau(info[0],type);
  if (!(this.changed = telex[0])) return;
  this.value = this.value.replaceAt(info[1],telex[1],info[2]);
  if (!telex[2]) { spellerror= 1; this.value+= this.ctrlchar; }
}

function findCharToChange(type) {
  var i = 0;
  var lastchars = this.charmap.lastCharsOf(this.value, 3);
  var c = lastchars[0][0];
  while( !this.charmap.isVowel(c) ) {
     if ((c < 'A') || (i >= 2)) return null;
     if (!(c = lastchars[++i][0])) return null;
  }
  c = lastchars[0][0].toLowerCase();
  var prevc = lastchars[1][0].toLowerCase();
  if (i == 2) {
    var tmp = prevc + c;
    if ((tmp!="ng") && (tmp!="ch") && (tmp!="nh")) return null;
    if (tmp=="ch" && type!=1 && type!=3) return null; 
  }
  else if (i == 1) {
    if((c>='v') || ("bdfghjklqrs".indexOf(c) >= 0)) return null;
    else if ("cpt".indexOf(c) >=0 && type!=1 && type!=3) return null; 
  }
  else if (this.charmap.isVowel(lastchars[1][0],0) && ("uyoia".indexOf(c)>=0)){
    ++i;
    if (((prevc=='o') && (c=='a')) || ((prevc=='u') && (c=='y'))) --i;
    c = prevc;  prevc = lastchars[2][0].toLowerCase();
    if (((prevc=='q') && (c=='u')) || ((prevc=='g') && (c=='i'))) --i;
  }
  var pos = this.value.length;
  for (var j=0; j<= i; j++) pos -= lastchars[j][1];
  var info = new Array(lastchars[i][0], pos, lastchars[i][1]);
  return info;  // [char, pos, charlength (for vni, viqr...)]
}
// end vietString Obj


////////////////////////////

// character-map template
// optimized code, not easy to read :-)
//
function CVietCharMap() {
  this.vietchars = null;
  this.length = 149;
  return this; 
}

CVietCharMap.prototype.charAt = function(ind) { 
  var chrcode = this.vietchars[ind];
  return chrcode ? String.fromCharCode(chrcode) : null; 
}

CVietCharMap.prototype.regExpAt = function (i) {
  var c=this.charAt(i);
  return c? new RegExp(c,'g') : 0;
}

CVietCharMap.prototype.lowerCaseOf = function (chr, ind) {
  var i = ind? ind: this.isVowel(chr);
  if(i) return (i && ((i-1)%24 >= 12))? this.charAt(i-12): this.charAt(i); 
}

CVietCharMap.prototype.isVowel = function (chr,isnumber) {
  var c = isnumber? String.fromcharCode(chr): chr;
  var ind = this.length-5;
  while ((c != this.charAt(ind)) && (ind > 0)) --ind;
  return ind;
}

CVietCharMap.prototype.indexOf = function (chr,isnumber) {
  var c = isnumber? String.fromcharCode(chr) : chr;
  var ind = this.length-1;
  while ((c != this.charAt(ind)) && (ind > 0)) --ind;
  return ind;
}

CVietCharMap.prototype.getDau = function (vchr, type) {
  var result = new Array(0,0,0);
  var ind = this.isVowel(vchr);
  if (!ind) return result;
  var accented = (ind < 25)? 0: 1;
  var ind_i = (ind-1) % 24 +1;
  var charset = (type == 6)? 0 : type;

  if ((type == 6) && !accented) return result;   
  if (accented & (charset*24 + ind_i == ind)) charset = 0;
  else result[2] = 1;

  result[1] = this.charAt(charset*24 + ind_i); 
  if(!result[1]) result[1]= this.lowerCaseOf(0,charset*24 + ind_i); 
  result[0] = 1;
  return result;
}

CVietCharMap.prototype.getAEOWD = function (vchr, type) {
  var result = new Array(0,0,0);
  var ind = 1, len = this.length; 
  if (type==5) while (this.charAt(len-ind)!=vchr) {if (++ind>4) return result;}
  else while ((ind < 25) && (vchr != this.charAt(ind))) { ind++; }
  if (ind >= 25) return result;
  var newind = ind;
  var vc = (type==5)? (ind-1)%2: (ind-1)%12;
  switch (type) {
    case 1: newind += (vc==0)? 1 : (vc==1)? -1: 0; break;
    case 2: newind += (vc==3)? 1 : (vc==4)? -1: 0; break;
    case 3: newind += (vc==6)? 1 : (vc==7)? -1: 0; break;
    case 5: newind += (vc==0)? 1 : (vc==1)? -1: 0; 
            newind=len-newind; ind=len-ind; break;
    case 4:
      if ((vc==0) || (vc==6)) newind += 2;
      else if ((vc==2) || (vc==8)) newind -= 2;
      else if (vc==9) ++newind; else if (vc==10) --newind;
  }
  if(newind != ind) {
    result[0] = 1;
    result[1] = this.charAt(newind);
    if(newind > ind) { result[2] = 1; }
  }
  return result;
}

CVietCharMap.prototype.convertTxtTo = function (txt, newmap) {
  var i, regexp, res;
  for (i=this.length-1; i>0; i--) {
    if(regexp=this.regExpAt(i)) txt= txt.replace(regexp, "::"+i+"::");
  }
  while (res = /::(\d+)::/gi.exec(txt)) {
    regexp = new RegExp("::"+res[1]+"::",'g');
    txt= txt.replace(regexp, newmap.charAt(parseInt(res[1],10)));
  }
  return txt;
}

CVietCharMap.prototype.lastCharsOf = function (str, num) {
  if (!num) return new Array(str.charAt(str.length-1),1);
  var vchars = new Array(num);
  for (var i=0; i< num; i++) vchars[i]= [str.charAt(str.length-i-1),1];
  return vchars;
}
// end CVietCharMap prototype

///////////////////////////


String.prototype.replaceAt= function(i,newchr,clen) {
  return this.substring(0,i)+ newchr + this.substring(i+clen);
}

// class: CVietUniCodeMap (child of CVietCharMap)
//
function CVietUniCodeMap() {
  var map = new CVietCharMap();
  // vietchars: [name, a,a^,a(,e,e^,i,o,o^,o+,u,u+,y,A*Y,a'*a`*a.*a?*a~,d,dd,D,DD]
  map.vietchars = new Array(
    "UNICODE",
    97, 226, 259, 101, 234, 105, 111, 244, 417, 117, 432, 121,
    65, 194, 258, 69, 202, 73, 79, 212, 416, 85, 431, 89,
    225, 7845, 7855, 233, 7871, 237, 243, 7889, 7899, 250, 7913, 253,
    193, 7844, 7854, 201, 7870, 205, 211, 7888, 7898, 218, 7912, 221,
    224, 7847, 7857, 232, 7873, 236, 242, 7891, 7901, 249, 7915, 7923,
    192, 7846, 7856, 200, 7872, 204, 210, 7890, 7900, 217, 7914, 7922,
    7841, 7853, 7863, 7865, 7879, 7883, 7885, 7897, 7907, 7909, 7921, 7925,
    7840, 7852, 7862, 7864, 7878, 7882, 7884, 7896, 7906, 7908, 7920, 7924,
    7843, 7849, 7859, 7867, 7875, 7881, 7887, 7893, 7903, 7911, 7917, 7927,
    7842, 7848, 7858, 7866, 7874, 7880, 7886, 7892, 7902, 7910, 7916, 7926,
    227, 7851, 7861, 7869, 7877, 297, 245, 7895, 7905, 361, 7919, 7929,
    195, 7850, 7860, 7868, 7876, 296, 213, 7894, 7904, 360, 7918, 7928,
    100, 273, 68, 272);
  return map;
}

// end vietuni.js
