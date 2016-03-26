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

  return $r['ver'];
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
?>
