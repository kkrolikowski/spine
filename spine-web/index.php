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
      $q = $dbh->prepare("SELECT hostname FROM sysinfo WHERE id = ". $_GET['serverid']);
      $q->execute();
      $r = $q->fetch();
      
      $spine->assign('HostName', $r['hostname']);
  }

  $spine->display('main.tpl');
?>
