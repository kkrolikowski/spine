<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  include_once './include/dashboard.php';
  include(SMARTY_LIB);

  $spine = new Smarty;
  $dbh = DBconnect();

  // pobieramy liste serwerow z bazy
  $HostMenu = serverList($dbh);
  $spine->assign('HostMenu', $HostMenu);
  $HostTotalCount = serverCount($dbh);
  $spine->assign('HostTotalCount', $HostTotalCount);

  // calkowita liczba wolnego miejsca na serwerach w GB
  $FreeTotalGB = hddFreeTotal($dbh);
  $spine->assign('FreeTotalGB', round($FreeTotalGB, 1));

  // ilosc wolnego miejsca na poszczegolnych serwerach
  $SrvHDDFree = hddFreePerServer($dbh);
  $spine->assign('SrvHDDFree', $SrvHDDFree);

  // uptime na poszczegolnych serwerach
  $uptimePerHost = uptimePerServer($dbh);
  $spine->assign('uptimePerHost', $uptimePerHost);

  if (isset($_GET['serverid'])) {
      $q = $dbh->prepare("SELECT ip, hostname, uptime, hdd_total, hdd_free FROM sysinfo WHERE id = ". $_GET['serverid']);
      $q->execute();
      $r = $q->fetch();

      $hdd_used = $r['hdd_total'] - $r['hdd_free'];
      $hdd_percentage_used = $hdd_used / $r['hdd_total'] * 100;
      $hdd_percentage_free = $r['hdd_free'] / $r['hdd_total'] * 100;
      $uptime = secondsToTime($r['uptime']);

      $spine->assign('HostName', $r['hostname']);
      $sysinfo = array(
          'ip' => $r['ip'],
          'hostname' =>$r['hostname'],
          'uptime' => $uptime,
          'hdd_percentage_used' => round($hdd_percentage_used, 0),
          'hdd_percentage_free' => round($hdd_percentage_free, 0),
          'hdd_free' => round($r['hdd_free'] / 1073741824, 2),
          'hdd_used' =>round($hdd_used / 1073741824, 2)
      );
      $spine->assign('sysinfo', $sysinfo);
  }

  $spine->display('main.tpl');
?>
