<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['getinfo'])) {
    $now = time();
    $tf = date("H:i:s d-m", $now);
    $hostlist = array();

    $q = $dbh->prepare("SELECT hostname, seen, distro, host_status FROM sysinfo");
    $q->execute();

    while ($r = $q->fetch()) {
      if(($now - $r['seen']) > 20) {
        $q2 = $dbh->prepare("SELECT host_status FROM sysinfo WHERE hostname = '".$r['hostname']."'");
        $q2->execute();
        $r2 = $q2->fetch();

        if($r2['host_status'] != "S") {
          $q3 = $dbh->prepare("UPDATE sysinfo SET host_status = 'U' WHERE hostname = '".$r['hostname']."'");
          $q3->execute();

          $q3 = $dbh->prepare("SELECT timestamp FROM log_host WHERE category = 'host' AND ".
                              "serverid = (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."') ".
                              "AND state = 'U' ORDER BY timestamp DESC LIMIT 1");
          $q3->execute();
          $r3 = $q3->fetch();
          if($q3->rowCount() == 0 || ($now - $r3['timestamp']) > 60) {
            $q4 = $dbh->prepare(
              "INSERT INTO log_host(category, state, `timestamp`, serverid) ".
              "VALUES('host', 'U', ".$now.", (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."'))");
            $q4->execute();
          }
        }
      }
      else {
        $q2 = $dbh->prepare("SELECT host_status FROM sysinfo WHERE hostname = '".$r['hostname']."'");
        $q2->execute();
        $r2 = $q2->fetch();
        if($r2['host_status'] == "U") {
          $q3 = $dbh->prepare("UPDATE sysinfo SET host_status = 'R' WHERE hostname = '".$r['hostname']."'");
          $q3->execute();
        }
        if($r2['host_status'] == "R") {
          $q3 = $dbh->prepare("UPDATE sysinfo SET host_status = 'A' WHERE hostname = '".$r['hostname']."'");
          $q3->execute();
          $q3 = $dbh->prepare(
            "INSERT INTO log_host(category, state, `timestamp`, serverid) ".
            "VALUES('host', 'A', ".$now.", (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."'))");
          $q3->execute();
        }
      }
      $qm = $dbh->prepare("SELECT service, status FROM service_checks WHERE host_id = (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."')");
      $qm->execute();
      while ($rm = $qm->fetch())
        $srv[$rm['service']] = $rm['status'];
      $hostlist[$r['hostname']] = array(
        'os' => $r['distro'],
        'time' => $tf,
        'status' => $r['host_status'],
        'services' => $srv
      );
      foreach ($srv as $sn => $st) {
        if($st == "ERR") {
          $qsl = $dbh->prepare("SELECT timestamp FROM log_host WHERE category = '".$sn."' AND ".
                              "serverid = (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."') ".
                              "AND state = 'U' ORDER BY timestamp DESC LIMIT 1");
          $qsl->execute();
          $rsl = $qsl->fetch();
          if($qsl->rowCount() == 0 || ($now - $rsl['timestamp']) > 60) {
            $qs = $dbh->prepare("INSERT INTO log_host(category, state, `timestamp`, serverid) " .
                                "VALUES('".$sn."', 'U', ".$now.", (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."'))");
            $qs->execute();
          }
        }
        elseif ($st == "OK") {
          $qs = $dbh->prepare("SELECT state FROM log_host WHERE category = '".$sn."' AND serverid = ".
                              "(SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."') ORDER BY id DESC LIMIT 1");
          $qs->execute();
          $qsr = $qs->fetch();
          if ($qsr['state'] == "U") {
            $qsi = $dbh->prepare("INSERT INTO log_host(category, state, `timestamp`, serverid) ".
                                "VALUES('".$sn."', 'A', ".$now.", (SELECT id FROM sysinfo WHERE hostname = '".$r['hostname']."'))");
            $qsi->execute();
          }
        }
      }
    }
    header('Content-Type: application/json', true, 200);
    echo json_encode($hostlist);
  }
  if(isset($_GET['monitor'])) {
    $now = time();
    if ($_POST['state'] == 'true') {
      $query = "UPDATE sysinfo SET host_status = 'A' WHERE id = ". $_GET['monitor'];
      $loginsert = "INSERT INTO log_host(category, state, timestamp, serverid) ".
                  "VALUES ('host', 'M', ".$now.", ".$_GET['monitor'].")";
    }
    else {
      $query = "UPDATE sysinfo SET host_status = 'S' WHERE id = ". $_GET['monitor'];
      $loginsert = "INSERT INTO log_host(category, state, timestamp, serverid) ".
                  "VALUES ('host', 'S', ".$now.", ".$_GET['monitor'].")";
    }
    $q = $dbh->prepare($query);
    $q->execute();
    $q = $dbh->prepare($loginsert);
    $q->execute();

    $q = $dbh->prepare("SELECT distro FROM sysinfo WHERE id = ". $_GET['monitor']);
    $q->execute();
    $r = $q->fetch();

    $json = array('os' => $r['distro']);
    header('Content-Type: application/json', true, 200);
    echo json_encode($json);
  }
?>
