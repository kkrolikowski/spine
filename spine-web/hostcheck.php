<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();
  $now = time();
  $dead = array();

  $q = $dbh->prepare("SELECT hostname, seen FROM sysinfo");
  $q->execute();

  while ($r = $q->fetch()) {
    if(($now - $r['seen']) > 10) {
      array_push($dead, $r['hostname']);
      $q2 = $dbh->prepare("UPDATE sysinfo SET host_status = 'U' WHERE hostname = '".$r['hostname']."'");
      $q2->execute();
    }
    else {
      $q2 = $dbh->prepare("UPDATE sysinfo SET host_status = 'A' WHERE hostname = '".$r['hostname']."'");
      $q2->execute();
    }
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
