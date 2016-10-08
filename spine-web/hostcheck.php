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
        if($st == "ERR" && $hostlist[$r['hostname']]['status'] != 'S') {
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
  if (isset($_GET['netinfo'])) {
    $q = $dbh->prepare("SELECT * FROM netif_stats WHERE system_id = ".$_GET['sid']." ORDER BY time_stamp DESC LIMIT 2");
    $q->execute();

    $time_arr = array();
    $out_arr = array();
    $in_arr = array();

    while ($r = $q->fetch()) {
      array_push($time_arr, $r['time_stamp']);
      array_push($out_arr, $r['out']);
      array_push($in_arr, $r['in']);
    }

    $sec = $time_arr[0] - $time_arr[1];
    $bytes_out = $out_arr[0] - $out_arr[1];
    $bytes_in = $in_arr[0] - $in_arr[1];

    if($sec != 0 && $bytes_out != 0)
      $bytes_out_per_sec = $bytes_out / $sec;
    else
      $bytes_out_per_sec = 0;

    if($sec != 0 && $bytes_in != 0)
      $bytes_in_per_sec = $bytes_in / $sec;
    else
      $bytes_in_per_sec = 0;

      $json = array(
        'time' => $time_arr[0],
        'out' => $bytes_out_per_sec,
        'in' => $bytes_in_per_sec
      );

    // kasujemy starsze niz doba wpisy
    $q = $dbh->prepare("DELETE FROM netif_stats WHERE time_stamp < (UNIX_TIMESTAMP() - 86400)");
    $q->execute();

    header('Content-Type: application/json', true, 200);
    echo json_encode($json, JSON_PRETTY_PRINT);
  }
  if(isset($_GET['sysinfo'])) {
    $q = $dbh->prepare("SELECT hdd_total, hdd_free, ram_total, ram_free, cpu_usage FROM sysinfo WHERE id = ".$_GET['sid']);
    $q->execute();
    $r = $q->fetch();

    // procent uzycia dysku
    $hdd_used = $r['hdd_total'] - $r['hdd_free'];
    $hdd_used_percent = $hdd_used / $r['hdd_total'];

    // procent uzycia ramu
    $ram_used = $r['ram_total'] - $r['ram_free'];
    $ram_used_percent = $ram_used / $r['ram_total'];

    //$cpu = $r['cpu_usage'] - 0.0;

    $json = array(
      'ram_free' => round($r['ram_free'] / 1073741824, 2),
      'ram_used' => $ram_used_percent,
      'hdd_free' => round($r['hdd_free'] / 1073741824, 2),
      'hdd_used' => $hdd_used_percent,
      'cpu_usage' => (float) $r['cpu_usage'] / 100
    );

    header('Content-Type: application/json', true, 200);
    echo json_encode($json);
  }
?>
