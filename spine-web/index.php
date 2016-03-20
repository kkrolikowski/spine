<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  include(SMARTY_LIB);

  $spine = new Smarty;
  $dbh = DBconnect();

  // pobieramy liste serwerow z bazy
  $q = $dbh->prepare("SELECT id,hostname FROM sysinfo");
  $q->execute();
  $HostMenu = array();
  while($r = $q->fetch()) {
    $HostMenu[$r['id']] = $r['hostname'];
  }
  $spine->assign('HostMenu', $HostMenu);
  $spine->display('main.tpl');
?>
