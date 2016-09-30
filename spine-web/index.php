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

  // calkowita liczba stron www
  $websiteCount = allWebsitesCount($dbh);
  $spine->assign('websiteCount', $websiteCount);

  // status wszystkich hostow z z bazy
  $allHostStatus = serverStatus($dbh);
  $spine->assign('allHostStatus', $allHostStatus);

  // tworzymy jsona z opcjami konfiguracyjnymi apacza
  vhostOptionFile($dbh);

  if (isset($_GET['serverid'])) {
      // komplet informacji na temat systemu i zuzycia zasobow
      $q = $dbh->prepare("SELECT ip, ext_ip, hostname, distro, uptime, hdd_total, hdd_free, ram_total, ram_free, host_status, cpu_usage FROM sysinfo WHERE id = ". $_GET['serverid']);
      $q->execute();
      $r = $q->fetch();

      $hdd_used = $r['hdd_total'] - $r['hdd_free'];
      $hdd_percentage_used = $hdd_used / $r['hdd_total'] * 100;
      $hdd_percentage_free = $r['hdd_free'] / $r['hdd_total'] * 100;

      $ram_used = $r['ram_total'] - $r['ram_free'];
      $ram_percentage_used = $ram_used / $r['ram_total'] * 100;
      $ram_percentage_free = $r['ram_free'] / $r['ram_total'] * 100;

      $uptime = secondsToTime($r['uptime']);

      $basicInfo = array(
        'os' => $r['distro'],
        'hostname' => $r['hostname'],
        'status' => $r['host_status']
      );

      $spine->assign('basicInfo', $basicInfo);
      $sysinfo = array(
          'ip' => $r['ip'],
          'ext_ip' => $r['ext_ip'],
          'hostname' =>$r['hostname'],
          'system' => $r['distro'],
          'uptime' => $uptime,
          'hdd_percentage_used' => round($hdd_percentage_used, 0),
          'hdd_percentage_free' => round($hdd_percentage_free, 0),
          'ram_percentage_free' => round($ram_percentage_free, 0),
          'hdd_free' => round($r['hdd_free'] / 1073741824, 2),
          'hdd_used' => round($hdd_used / 1073741824, 2),
          'ram_free' => round($r['ram_free'] / 1073741824, 2),
          'ram_total' => round($r['ram_total'] / 1073741824, 2),
          'cpu_usage' => $r['cpu_usage'],
          'status' => $r['host_status']
      );
      $spine->assign('sysinfo', $sysinfo);

      // lista uslug i ich status na serwerze
      $q = $dbh->prepare("SELECT service,status FROM service_checks WHERE host_id = ".$_GET['serverid']);
      $q->execute();
      while ($r = $q->fetch())
        $services[$r['service']] = $r['status'];
      $spine->assign('hostsrv', $services);

      // Lista kont uzytkownikow w systemie
      $q = $dbh->prepare("SELECT id, login, fullname, email FROM sysusers WHERE system_id = ". $_GET['serverid']);
      $q->execute();
      while ($r = $q->fetch()) {
        $sysuser[$r['id']] = array(
          'login' => $r['login'],
          'fullname' => $r['fullname'],
          'email' => $r['email']
        );
      }
      $spine->assign('sysuser', $sysuser);

      // lista stron WWW na danym serwerze
      $q = $dbh->prepare("SELECT id, ServerName,htpasswd FROM www WHERE system_id = ". $_GET['serverid']." AND status = 'A'");
      $q->execute();
      if($q->rowCount() == 0)
        $spine->assign('EmptySiteList', 1);
      else {
        while ($r = $q->fetch()) {
          $access_type = vhostAccessLevel($dbh, $r['id']);
          $apacheconf[$r['id']] = array(
            'ServerName' => $r['ServerName'],
            'password' => $r['htpasswd'],
            'access_type' => $access_type
          );
        }
        $spine->assign('websites', $apacheconf);
      }

      // lista kont htaccess na danym serwerze
      $q = $dbh->prepare("SELECT id, login FROM www_users WHERE system_id = ". $_GET['serverid']);
      $q->execute();
      if($q->rowCount() == 0)
        $spine->assign('EmptyUserList', 1);
      else {
        while ($r = $q->fetch()) {
          $htusers[$r['id']] = $r['login'];
        }
        $spine->assign('htusers', $htusers);
      }

      // lista uzytkownikow na danym serwerze
      $q = $dbh->prepare("SELECT id, login FROM sysusers WHERE system_id = ". $_GET['serverid']);
      $q->execute();
      while ($r = $q->fetch()) {
        $wwwuser[$r['id']] = $r['login'];
      }
      $spine->assign('wwwuser', $wwwuser);

      // lista uzytkownikow z htpasswd
      $q = $dbh->prepare("SELECT id, login FROM www_users WHERE system_id = ". $_GET['serverid']);
      $q->execute();
      if($q->rowCount() > 0) {
        while ($r = $q->fetch()) {
          $htpasswd[$r['id']] = $r['login'];
        }
        $spine->assign('htpasswd', $htpasswd);
      }
      else {
        $spine->assign('htpasswd', 'NaN');
      }
  }
  if($_GET['show'] == "logs") {
    $q = $dbh->prepare(
    "SELECT log.id, log.category, log.state, log.timestamp, sys.hostname ".
    "FROM log_host log JOIN sysinfo sys ON log.serverid = sys.id ORDER BY timestamp DESC");
    $q->execute();
    $logs = array();
    while ($r = $q->fetch()) {
      $logs[$r['id']] = array(
        'hostname' => $r['hostname'],
        'category' => $r['category'],
        'state' => $r['state'],
        'timestamp' => date("H:i:s d-m", $r['timestamp'])
      );
    }
    $spine->assign('Logs', $logs);
  }
  $spine->display('main.tpl');
?>
