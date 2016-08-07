<?php
  include_once './include/functions.php';
  $toGB = 1073741824;       // przelicznik z bajtow na GB

  function serverList($dbh) {
    $q = $dbh->prepare("SELECT id,hostname FROM sysinfo");
    $q->execute();
    $HostMenu = array();
    while($r = $q->fetch()) {
      $HostMenu[$r['id']] = $r['hostname'];
    }

    return $HostMenu;
  }
  function serverCount($dbh) {
    $q = $dbh->prepare("SELECT id FROM sysinfo");
    $q->execute();

    return $q->rowCount();
  }
  function hddFreeTotal($dbh) {
    global $toGB;
    $FreeTotalBytes = 0;
    $q = $dbh->prepare("SELECT hdd_free FROM sysinfo");
    $q->execute();
    while ($r = $q->fetch()) {
      $FreeTotalBytes += $r['hdd_free'];
    }

    return $FreeTotalBytes / $toGB;
  }
  function hddFreePerServer($dbh) {
    global $toGB;
    $q = $dbh->prepare("SELECT hostname, distro, hdd_free, hdd_total FROM sysinfo");
    $q->execute();

    $SrvHDDFree = array();
    while ($r = $q->fetch()) {
      $percent_free = $r['hdd_free'] / $r['hdd_total'] * 100;
      $SrvHDDFree[$r['hostname']] = array(
        'ostype' => $r['distro'],
        'pfree' => round($percent_free, 0)
      );
    }
    return $SrvHDDFree;
  }
  function secondsToTime($seconds) {
      $dtF = new DateTime("@0");
      $dtT = new DateTime("@$seconds");
      return $dtF->diff($dtT)->format('%ad%hh%im%ss');
  }
  function uptimePerServer($dbh) {
    $q = $dbh->prepare("SELECT hostname, uptime FROM sysinfo");
    $q->execute();

    $uptime = array();
    while ($r = $q->fetch()) {
      $uptimeString = secondsToTime($r['uptime']);
      $uptime[$r['hostname']][] = array(
        'seconds' => $r['uptime'],
        'time' => $uptimeString
      );
    }
    return $uptime;
  }
  function allWebsitesCount($dbh) {
    $q = $dbh->prepare("SELECT count(ServerName) AS websiteCount FROM www WHERE status = 'A'");
    $q->execute();
    $r = $q->fetch();

    return $r['websiteCount'];
  }
  function serverStatus($dbh) {
    $q = $dbh->prepare("SELECT hostname,distro,uptime,seen,host_status FROM sysinfo");
    $q->execute();
    $sysStat = array();
    while ($r = $q->fetch()) {
      $uptimeString = secondsToTime($r['uptime']);
      $lastSeen = timestring($r['seen']);
      $sysStat[$r['hostname']][] = array(
        'os' => $r['distro'],
        'uptime' => $uptimeString,
        'lastSeen' => $lastSeen,
        'status' => $r['host_status']
      );
    }
    return $sysStat;
  }
?>
