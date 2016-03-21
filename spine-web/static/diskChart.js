$(document).ready(function() {
  $.ajax({
    url: "/hostinfo.php?uptime",
    method: "GET",
    success: function(response) {
      var data = {response};
      Chart.defaults.global.legend = {
        enabled: false
      };
      var canvasDoughnut = new Chart(document.getElementById("canvas1"), {
        type: 'doughnut',
        tooltipFillColor: "rgba(51, 51, 51, 0.55)",
        data: data
      });

    }
  });
});
