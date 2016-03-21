<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  include_once './include/dashboard.php';
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
  if (isset($_GET['uptime'])) {
    $dbh = DBconnect();
    $uptimeData = uptimePerServer($dbh);
    $chartDataLabels = array();
    $chartDataSet = array();
    $chartDataSetColors = array();

    foreach ($uptimeData as $key => $value) {
      array_push($chartDataLabels, $key);
      $sec[0] = $value;
      $color = "#" . substr(md5(rand()), 0, 6);
      array_push($chartDataSet, $sec[0][0]['seconds']);
      array_push($chartDataSetColors, $color);
    }
    $json = "labels: [";
    foreach ($chartDataLabels as $value) {
      $json .= "\"". $value ."\",";
    }
    $json .= "], datasets: [{data: [";
    foreach ($chartDataSet as $value) {
      $json .= $value. ",";
    }
    $json .= "],backgroundColor: [";
    foreach ($chartDataSetColors as $value) {
      $json .= "\"". $value ."\",";
    }
    $json .= "],hoverBackgroundColor: [";
    foreach ($chartDataSetColors as $value) {
      $json .= "\"". $value ."\",";
    }
    $json .= "]}]";
    $chartDataLabels = array('Labels' => $chartDataLabels);
    $chartDataSets = array(
        'datasets' => array('data' => $chartDataSet));
    header('Content-Type: application/json');
    echo $json;
  }
?>
