function MM_findObj(n, d) { //v4.0
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && document.getElementById) x=document.getElementById(n); return x;
}

function KT_UniVAL() { //v0.1
  var p,q,nm,test,reg,num,errors='',args=KT_UniVAL.arguments, errVal = '', fval;
  for (i=0; i<(args.length-1); i+=2) {
    test=args[i+1];
    val=MM_findObj(args[i]);
    if (val) {
      p = test.indexOf("|");
      nm = test.substring(1, p);
      if (p+1!=test.length){
        reg = "^"+unescape(test.substring(p+1, test.length))+"$";
      } else {
        reg = ".*";
      }
      if(nm.length == 0){
        nm=val.name;
      }
      if ((fal=val.value)!="") {
        reg = new RegExp(reg);
        if(!reg.test(fal)){
          if(errVal == ''){
            errVal = val;
          }
          errors += KT_UNV_THEF+unescape(nm)+KT_UNV_ISWR;
        }
      } else if (test.charAt(0) == 'R') {
        if(errVal == ''){
          errVal = val;
        }
        errors += KT_UNV_THEF +unescape(nm)+KT_UNV_ISRQ;
      }
    }
  }
  if (errors) {
    alert(KT_UNV_ERR +errors);
	errVal.focus();
  }
  document.MM_returnValue = (errors == '');
}