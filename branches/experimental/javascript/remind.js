var timer;
var count = 0;

function jiggleit()
{
  obj = document.getElementById("notification_area");
  // obj.style.left=parseInt(obj.style.left)+2;
  //obj.innerHTML = obj.innerHTML + obj.style.display + '.';
  count ++;
  if (obj.style.display == 'none') {
    obj.style.display='';
  } else {
    if (count >= 4) {
      count = 0;
      obj.style.display='none';
    }
  }
}

function SetStatus(text)
{
  obj = document.getElementById("user_area");
  obj.style.display= 'none';
  obj = document.getElementById("notification_area");
  obj.innerHTML = text;
  timer=setInterval("jiggleit()",100);
  window.setTimeout("Stop()",4000);
  window.setTimeout("ResetStatus()",10000);
}
function Stop()
{
  clearInterval(timer);
  obj = document.getElementById("notification_area");
  obj.style.display='';
}
function ResetStatus()
{
  obj = document.getElementById("user_area");
  obj.style.display= '';
  obj = document.getElementById("notification_area");
  obj.innerHTML = '&nbsp;';
}
