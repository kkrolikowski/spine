<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  include_once './include/dashboard.php';

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
    $chartData = array(
      'labels' => $chartDataLabels,
      'datasets' => array(
        'data' => $chartDataSet,
        'backgroundColor' => $chartDataSetColors,
        'hoverBackgroundColor' => $chartDataSetColors
      )
    );
    $color = substr(md5(rand()), 0, 6);
    echo "#$color";
    header('Content-Type: application/json');
    echo json_encode($chartData);
  }
?>
