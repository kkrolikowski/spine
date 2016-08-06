<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();
  $now = time();
  $dead = array();

  $q = $dbh->prepare("SELECT hostname, seen FROM sysinfo");
  $q->execute();

  while ($r = $q->fetch()) {
    if(($now - $r['seen']) > 1)
      array_push($dead, $r['hostname']);
  }
  if(count($dead) > 0) {
    header('Content-Type: application/json');
    echo json_encode($dead);
  }
  else {
    $message = "X-Message: all hosts are alive";
    header($message, true, 406);
  }
?>
