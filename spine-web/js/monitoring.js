var Monitoring;
function watch() {
  if(typeof(Monitoring) == "undefined")
    Monitoring = new Worker("js/hostcheck.js");
    Monitoring.onmessage = function(event) {
        if(typeof(event.data) != "undefined") {
          var json = {};
          json = JSON.parse(event.data);
          $.each(json, function(k, v) {
            alertify.error("HOST: " + k + " is down!");
            $('#alertBox-body > .list-group').append(
              '<a href="#" class="list-group-item">' +
                '<i class="fa fa-warning fa-fw"></i> Host: ' + k + ' is down.' +
                '<span class="pull-right text-muted small"><em>4 minutes ago</em></span>' +
              '</a>'
            );
            var div =   $(".hostList:has(strong:contains('"+ k +"'))");
            div.find('.label-success').removeClass('label-success')
            .addClass('label-danger')
            .html('Offline');
            if(v === "Ubuntu") {
              var image = "/images/server-ubuntu_error.png";
            }
            else if (v === "Centos6" || v == "Centos7") {
              var image = "/images/server-centos_error.png";
            }
            div.prev().find('img').attr('src', image);
          });
        }
    }
    Monitoring.postMessage("HI");
}
