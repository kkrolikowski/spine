var hostlist = new Array();

self.addEventListener('message', function(e) {
  sid = e.data.serverid;
  getNetInfo(sid);
}, false);

function getNetInfo(sid) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function(sid) {
    if(xhttp.readyState == 4 && xhttp.status == 200) {
      hostlist.push(xhttp.responseText);
    }
  };
  xhttp.open("GET", "/hostcheck.php?netinfo&sid=" + sid, true);
  xhttp.send();
  if(hostlist.length) {
    postMessage(hostlist);
    hostlist = [];
  }
}
setInterval(function() { getNetInfo(sid)}, 3000);
