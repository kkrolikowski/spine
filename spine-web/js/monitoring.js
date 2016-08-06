var Monitoring;
function getDeadHosts() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if(xhttp.readyState == 4 && xhttp.status == 200) {
      deadHosts.push(xhttp.responseText);
    }
  };
  xhttp.open("GET", "/hostcheck.php", true);
  xhttp.send();
}
function watch() {
  if(typeof(Monitoring) == "undefined")
    Monitoring = new Worker("js/monitoring.js");
    Monitoring.onmessage = function(event) {
      if(typeof(event.data) != "undefined")
        alert(event.data);
    }
    setInterval(function() { getDeadHosts()}, 1000);
}
