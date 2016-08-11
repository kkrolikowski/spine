var Monitoring;

function watch() {
  if(typeof(Monitoring) == "undefined")
    Monitoring = new Worker("js/hostcheck.js");
    Monitoring.onmessage = function(event) {
        if(typeof(event.data) != "undefined") {
          var json = {};
          var hostpageHeader;
          var sysinfoList;

          json = JSON.parse(event.data);
          $.each(json, function(k, v) {
            if(v.status == "U") {
              alertify.error("HOST: " + k + " is down!");
              var div =   $(".hostList:has(strong:contains('"+ k +"'))");
              hostpageHeader = $(".page-header:contains('"+ k +"')");
              sysinfoList = $("dd:contains('"+ k +"')").parent();

              div.find('.label-success').removeClass('label-success')
              .addClass('label-danger')
              .html('Offline');

              sysinfoList.find('.label-success').removeClass('label-success')
              .addClass('label-danger')
              .html('Offline');

              if(v.os === "Ubuntu") {
                var image = "/images/server-ubuntu_error.png";
              }
              else if (v.os === "Centos6" || v.os == "Centos7") {
                var image = "/images/server-centos_error.png";
              }
              div.prev().find('img').attr('src', image);
              hostpageHeader.find('img').attr('src', image);
            }
            if(v.status == "R") {
              alertify.success("HOST: " + k + " is UP!");
              var div =   $(".hostList:has(strong:contains('"+ k +"'))");
              hostpageHeader = $(".page-header:contains('"+ k +"')");
              sysinfoList = $("dd:contains('"+ k +"')").parent();

              div.find('.label-danger').removeClass('label-danger')
              .addClass('label-success')
              .html('Online');

              sysinfoList.find('.label-danger').removeClass('label-danger')
              .addClass('label-success')
              .html('Offline');

              if(v.os === "Ubuntu") {
                var image = "/images/server-ubuntu_ok.png";
              }
              else if (v.os === "Centos6" || v.os == "Centos7") {
                var image = "/images/server-centos_ok.png";
              }
              div.prev().find('img').attr('src', image);
              hostpageHeader.find('img').attr('src', image);
            }
          });
        }
    }
    Monitoring.postMessage("HI");
}
