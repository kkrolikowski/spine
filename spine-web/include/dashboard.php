<?php

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
    $q = $dbh->prepare("SELECT hostname, hdd_free FROM sysinfo");
    $q->execute();

    $SrvHDDFree = array();
    while ($r = $q->fetch()) {
      $freeGB = $r['hdd_free'] / $toGB;
      $SrvHDDFree[$r['hostname']] = round($freeGB, 1);
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
?>
