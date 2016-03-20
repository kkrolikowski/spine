var data;
$.ajax({
  url: "/hostinfo.php?uptime",
  method: "GET",
  success: function(response) {
    data = response
  }
});
Chart.defaults.global.legend = {
  enabled: false
};
/*
var data = {
  labels: [
    "Symbian",
    "Blackberry",
    "Other",
    "Android",
    "IOS"
  ],
  datasets: [{
    data: [15, 20, 30, 10, 30],
    backgroundColor: [
      "#BDC3C7",
      "#9B59B6",
      "#455C73",
      "#26B99A",
      "#3498DB"
    ],
    hoverBackgroundColor: [
      "#CFD4D8",
      "#B370CF",
      "#34495E",
      "#36CAAB",
      "#49A9EA"
    ]

  }]
};
*/
var canvasDoughnut = new Chart(document.getElementById("canvas1"), {
  type: 'doughnut',
  tooltipFillColor: "rgba(51, 51, 51, 0.55)",
  data: data
});
