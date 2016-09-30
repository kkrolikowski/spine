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
            $.each(v.services, function(srv, status) {
              var d = $("dd:contains('"+ k +"')")
                      .parentsUntil('#page-wrapper')
                      .next()
                      .find("strong:contains('"+ srv +"')")
                      .parent();
              if(status == "OK") {
                var image = "/images/led-on.png";
              }
              else {
                var image = "/images/led-off.png";
              }
              d.next().find('img').attr('src', image);
            });
          });
        }
    }
    Monitoring.postMessage("HI");

// Obsluga dynamicznych wykresow pasma.
  Netchart = new Worker("js/netstats.js");
  var json = {};
  var chartContainer = $('.panel-heading:contains("Utylizacja pasma")').next();
  var serverid = chartContainer.attr('data-serverid');
  Netchart.postMessage({"serverid": serverid});
  Netchart.onmessage = function(event) {
    if(typeof(event.data) != "undefined") {
      var json = JSON.parse(event.data);

      var traffic_in =  {label: "Incomming", values: []};
      var traffic_out = {label: "Outgoing",  values: []};
      traffic_in.values.push({time: json.time, y: json.in});
      traffic_out.values.push({time: json.time, y: json.out});
      var mychart = $('#traffic_' + serverid).epoch({
        type: 'time.line',
        data: [traffic_in, traffic_out],
        axes: ['left', 'bottom']
      });
      mychart.push([traffic_in.values[0], traffic_out.values[0]]);

      var inbound = json.in;
      var outbound = json.out;
      var unit = "B/s";
      if(json.in > 1000 && json.out > 1000) {
        unit = "kB/s";
        inbound /= 1024;
        outbound /= 1024;
      }
      inbound = inbound.toFixed(1);
      outbound = outbound.toFixed(1);

      $('.panel-heading:contains("Utylizacja pasma")')
        .html("Utylizacja pasma <strong>Download: " + inbound + " " + unit + ", Upload: " + outbound + " " + unit + "</strong>");
    }
  }
  // Obsluga zegarow.
    sysinfo = new Worker("js/sysstats.js");
    var chartContainer = $('.panel-heading:contains("Utylizacja pasma")').next();
    var serverid = chartContainer.attr('data-serverid');
    sysinfo.postMessage({"serverid": serverid});
    sysinfo.onmessage = function(event) {
      if(typeof(event.data) != "undefined") {
        var sysinfo_json = JSON.parse(event.data);

        // HDD info
        var hdd = $('#hddUsedGauge_' + serverid).epoch({
          type: 'time.gauge',
          value: sysinfo_json.hdd_used
        });
        hdd.update(sysinfo_json.hdd_used);
        $('strong:contains("HDD Free")')
          .html("HDD Free: " + sysinfo_json.hdd_free + " GB");

        // RAM info
        var ram = $('#ramUsedGauge_' + serverid).epoch({
          type: 'time.gauge',
          value: sysinfo_json.ram_used
        });
        ram.update(sysinfo_json.ram_used);
        $('strong:contains("RAM Free")')
          .html("RAM Free: " + sysinfo_json.ram_free + " GB")

        // CPU info
        var ram = $('#CPUGauge_' + serverid).epoch({
          type: 'time.gauge',
          value: sysinfo_json.cpu_usage
        });
        ram.update(sysinfo_json.cpu_usage);
        $('strong:contains("CPU Usage")')
          .html("CPU Usage: " + sysinfo_json.ram_free + " %")
      }
    }
}
