// JavaScript Document
var prevId='';
var prevFontWeight = 'normal';
var prevFontSize=10;
function Bold_Text(id, url){
	parent.main.document.location=url; 	
	if (document.getElementById){
		if (prevId != ''){
			document.getElementById(prevId).style.fontWeight = prevFontWeight;
			document.getElementById(prevId).style.fontSize = prevFontSize;
		}
		prevFontWeight = document.getElementById(id).style.fontWeight;
		prevFontSize = document.getElementById(id).style.fontSize;
		document.getElementById(id).style.fontWeight = 'bold';
		document.getElementById(id).style.fontSize = 11;
		
	}
	else if (document.all){
		if (prevId != ''){
			document.all[prevId].style.fontWeight = prevFontWeight;
			document.all[prevId].style.fontSize = prevFontSize;
		}
		prevFontWeight = document.all[id].style.fontWeight;
		prevFontSize = document.all[id].style.fontSize;
		document.all[id].style.fontWeight = 'bold';
		document.all[id].style.fontSize = 11;		
	}
	prevId = id;
}
