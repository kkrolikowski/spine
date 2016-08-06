var Monitoring;
function watch() {
  if(typeof(Monitoring) == "undefined")
    Monitoring = new Worker("js/hostcheck.js");
    Monitoring.onmessage = function(event) {
        //console.log(event.data);
        if(typeof(event.data) != "undefined") {
          var json = {};
          json = JSON.parse(event.data);
          for(var i = 0; i < json.length; i++) {
              alertify.error(json[i]);
          }
        }
    }
    Monitoring.postMessage("HI");
}
