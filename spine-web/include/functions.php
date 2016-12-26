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
  $versions = array();
  $q = $dbh->prepare("SELECT version FROM configver WHERE id = ". $serverid);
  $q->execute();
  if($q->rowCount() > 0) {
    while($r = $q->fetch()) {
      array_push($versions, $r['version']);
    }
    return max($versions);
  }
  else
    return 0;
}
function updateConfigVersion($dbh, $serverid, $scope) {
  $newVer = dayVersion();
  $oldVer = checkConfigVer($dbh, $serverid);

  if($newVer > $oldVer) {
    if($oldVer == 0)
      $q = $dbh->prepare("INSERT INTO configver(scope, version, systemid) VALUES('".$scope."', ".$newVer.", ".$serverid.")");
    else
      $q = $dbh->prepare("UPDATE configver SET version = ".$newVer. " WHERE scope = '".$scope."', AND systemid = ".$serverid);
    $q->execute();
  }
  else {
    $oldVer += 1;
    $q = $dbh->prepare("UPDATE configver SET version = ".$oldVer. " WHERE scope = '".$scope."' AND systemid = ".$serverid);
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
function timestring($epoch) {
  return date("Y-m-d H:i:s", $epoch);
}
function lastUID($dbh, $sysid) {
  $q = $dbh->prepare("SELECT id,uid FROM sysusers WHERE system_id = $sysid ORDER BY id DESC LIMIT 1");
  $q->execute();
  $r = $q->fetch();

  return $r['uid'];
}
function sha512_pass($clearPass) {
  $charArr = array('1','2','3','4','5','6','q','w','e','r','t','a','s',
                  'd','f','g','z','x','c','v','b','7','8','9','0','-',
                  'y','u','i','o','p','h','j','k','l','n','m',',','.','/',
                  'Q','W','E','R','T','A','S','D','F','G','Z','X','C','V',
                  'B','Y','U','I','O','P','H','J','K','L','N','?','!');

  for ($i=0; $i < 8; $i++) {
    $charIndex = rand(0, count($charArr));
    $salt .= $charArr[$charIndex];
  }

  define('CRYPT_SHA512', 1);
  return crypt($clearPass, "$6$".$salt);
}
?>
