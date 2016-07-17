<?php
  include_once './include/config.php';

  function DBconnect() {
    $dsn = 'mysql:host='.DBHOST.';dbname='.DBNAME;
    $dbh = new PDO($dsn, DBUSER, DBPASS);
    return $dbh;
  }
  function dayVersion() {
  $version = strftime("%G%m%d", time());
  $version .= "00";

  return $version;
}
function checkConfigVer($dbh, $serverid) {
  $q = $dbh->prepare("SELECT config_ver FROM sysinfo WHERE id = ". $serverid);
  $q->execute();
  $r = $q->fetch();

  return $r['config_ver'];
}
function updateConfigVersion($dbh, $serverid) {
  $newVer = dayVersion();
  $oldVer = checkConfigVer($dbh, $serverid);

  if($newVer > $oldVer) {
    $q = $dbh->prepare("UPDATE sysinfo SET config_ver = ".$newVer." WHERE id = ". $serverid);
    $q->execute();
  }
  else {
    $oldVer += 1;
    $q = $dbh->prepare("UPDATE sysinfo SET config_ver = ".$oldVer." WHERE id = ". $serverid);
    $q->execute();
  }
}
function vhostOptionFile($dbh) {
  $q = $dbh->prepare("SELECT id, vhostopt FROM www_opts");
  $q->execute();
  $str = '[';
  while($r = $q->fetch(PDO::FETCH_ASSOC)) {
    $str .= (json_encode($r)) . ',';
  }
  $json = substr($str, 0, -1);
  $json .= ']';
  $fd = fopen('data/vhost_options.json', 'w');
  fwrite($fd, $json);
  fclose($fd);
}
function checkHost($dbh, $name, $server_id) {
  $status = "NaN";
  $q = $dbh->prepare("SELECT count(*) AS n FROM www WHERE ServerName = '".$name."' AND system_id = ".$server_id);
  $q->execute();
  $r = $q->fetch();

  if($r['n'] > 0) {
    $status = $name;
  }
  else {
    $q = $dbh->prepare("SELECT count(*) AS n FROM www WHERE ServerAlias like '%".$name."' AND system_id = ".$server_id);
    $q->execute();
    $r = $q->fetch();

    if($r['n'] > 0) {
      $status = $name;
    }
    else {
      $status = "NaN";
    }
  }

  return $status;
}
function checkServerName($dbh, $sname, $sid) {
  return checkHost($dbh, $sname, $sid);
}
function checkServerAlias($dbh, $saname, $sid) {
  $status = "NaN";
  foreach ($saname as $host) {
    if(($status = checkHost($dbh, $host, $sid)) != "NaN")
      return $status;
    }
    return $status;
}
function HostExist($dbh, $sname, $saname, $sid) {
  if(checkServerName($dbh, $sname, $sid) != "NaN")
    return $sname;
  else if($saname != "NaN") {
    if(($saname_item = checkServerAlias($dbh, $saname, $sid)) != "NaN")
      return $saname_item;
    else
      return "NaN";
  }
}
function htuserExist($dbh, $htuser, $sid) {
  $q = $dbh->prepare("SELECT count(*) as n FROM www_users WHERE login = '".$htuser."' AND system_id = ".$sid);
  $q->execute();
  $r = $q->fetch();

  return $r['n'];
}
function vhostAccessLevel($dbh, $vhost_id) {
  $level = 0;
  $count = 0;
  $q = $dbh->prepare("SELECT access_permission FROM www_access WHERE vhost_id = ".$vhost_id);
  $q->execute();
  while ($r = $q->fetch()) {
    if($r['access_permission'] == 1)
      $level++;
    $count++;
  }
  if($count > 1)
    $level++;
  return $level;
}
?>
