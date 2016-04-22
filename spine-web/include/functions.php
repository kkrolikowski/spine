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
?>
