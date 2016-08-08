var Monitoring;
function watch() {
  if(typeof(Monitoring) == "undefined")
    Monitoring = new Worker("js/hostcheck.js");
    Monitoring.onmessage = function(event) {
        if(typeof(event.data) != "undefined") {
          var json = {};
          json = JSON.parse(event.data);
          for(var i = 0; i < json.length; i++) {
            alertify.error("HOST: " + json[i] + " is down!");
            $('#alertBox-body > .list-group').append(
              '<a href="#" class="list-group-item">' +
                '<i class="fa fa-warning fa-fw"></i> Host: ' + json[i] + ' is down.' +
                '<span class="pull-right text-muted small"><em>4 minutes ago</em></span>' +
              '</a>'
            );
            $(".hostList:has(strong:contains('"+ json[i] +"'))")
              .find('.label-success').removeClass('label-success')
              .addClass('label-danger')
              .html('Offline');
          }
        }
    }
    Monitoring.postMessage("HI");
}