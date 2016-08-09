<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();
  $now = time();
  $dead = array();

  $q = $dbh->prepare("SELECT hostname, seen, distro FROM sysinfo");
  $q->execute();

  while ($r = $q->fetch()) {
    if(($now - $r['seen']) > 10) {
      $tf = date("H:i:s d-m", $now);
      $dead[$r['hostname']] = array(
        'os' => $r['distro'],
        'time' => $tf
      );
      $q2 = $dbh->prepare("UPDATE sysinfo SET host_status = 'U' WHERE hostname = '".$r['hostname']."'");
      $q2->execute();

      $message = "Host ". $r['hostname']. " is down";
      $q2 = $dbh->prepare("INSERT INTO log_host(message, state, `timestamp`) VALUES('".$message."', 'U', ".$now.")");
      $q2->execute();

      // czyscimy logi starsze niz ostatnie 24h
      $q2 = $dbh->prepare("DELETE FROM log_host WHERE `timestamp` < ".$now." - 86400");
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
    header($message, true, 206);
  }
?>
