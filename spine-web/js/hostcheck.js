var hostlist = new Array();

function getHostStatus() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if(xhttp.readyState == 4 && xhttp.status == 200) {
      hostlist.push(xhttp.responseText);
    }
  };
  xhttp.open("GET", "/hostcheck.php?getinfo", true);
  xhttp.send();
  if(hostlist.length) {
    postMessage(hostlist);
    hostlist = [];
  }
}
setInterval(function() { getHostStatus()}, 3000);
