var deadHosts = new Array();
function getDeadHosts() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if(xhttp.readyState == 4 && xhttp.status == 200) {
      deadHosts.push(xhttp.responseText);
    }
  };
  xhttp.open("GET", "/hostcheck.php", true);
  xhttp.send();
  if(deadHosts.length) {
    postMessage(deadHosts);
    deadHosts = [];
  }
}
setInterval(function() { getDeadHosts()}, 5000);
