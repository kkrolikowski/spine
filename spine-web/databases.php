<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['add'])) {
    $q = $dbh->prepare("SELECT count(*) AS cnt FROM db_name WHERE name = '".$_POST['dbname']."' AND host_id = ".$_POST['serverid']);
    $q->execute();
    $r = $q->fetch();
    if($r['cnt'] == 1) {
      $message = "X-Message: Baza ". $_POST['dbname'] ." juz istnieje.";
      header($message, true, 406);
    }
    else {
      $q = $dbh->prepare("INSERT INTO db_name(name, status, host_id, vhost_id) ".
                        "VALUES('".$_POST['dbname']."', 'N', ".$_POST['serverid'].
                        ", ".$_POST['vhostid'].")");
      $q->execute();

      $q = $dbh->prepare("SELECT id FROM db_name WHERE name = '".$_POST['dbname']."' AND host_id = ".$_POST['serverid']);
      $q->execute();
      $r = $q->fetch();
      $dbid = $r['id'];

      $q = $dbh->prepare("SELECT d.id, d.name AS dbname, CASE d.vhost_id WHEN 0 THEN ".
                        "'None' ELSE v.ServerName END AS vhost, d.host_id AS serverid ".
                        "FROM db_name d LEFT JOIN www v ON d.vhost_id = v.id WHERE d.id =".$dbid);
      $q->execute();
      $r = $q->fetch();

      $json = array(
        'id' => $r['id'],
        'dbname' => $r['dbname'],
        'vhost' => $r['vhost'],
        'serverid' => $r['serverid']
      );
      updateConfigVersion($dbh, $_POST['serverid'], "db_name");

      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
  if (isset($_GET['adduser'])) {
    $q = $dbh->prepare("SELECT count(*) AS cnt FROM db_user WHERE host_id = ".$_POST['serverid'].
                      " AND login = '".$_POST['dblogin']."'");
    $q->execute();
    $r = $q->fetch();
    if ($r['cnt'] == 1) {
      $message = "X-Message: User ". $_POST['dblogin'] ." exists.";
      header($message, true, 406);
    }
    else {
      $hash = "*" . strtoupper(sha1(sha1($_POST['dbpass'], TRUE)));
      $q = $dbh->prepare("INSERT INTO db_user(login,pass,status,host_id) VALUES".
                        "('".$_POST['dblogin']."', '".$hash."', 'N', ".$_POST['serverid'].")");
      $q->execute();

      $q = $dbh->prepare("SELECT id,login,host_id FROM db_user WHERE login = '".$_POST['dblogin']."' AND host_id = ".$_POST['serverid']);
      $q->execute();
      $r = $q->fetch();

      $json = array(
        'id' => $r['id'],
        'login' => $r['login'],
        'serverid' => $r['host_id']
      );
      updateConfigVersion($dbh, $_POST['serverid'], "db_user");
      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
  if (isset($_GET['addperms'])) {
    foreach ($_POST['dbperms'] as $g)
      $grants .= "$g,";
    // check if there are some privileges granted to particular user on particular DB
    $q = $dbh->prepare("SELECT id FROM db_privs WHERE db_id = ".$_POST['dbname']. " AND user_id = ".$_POST['dbuser']);
    $q->execute();
    $r = $q->fetch();

    if($q->rowCount() > 0) {
      $id = $r['id'];
      $q = $dbh->prepare("UPDATE db_privs SET grants = '".rtrim($grants, ',')."', status = 'U' WHERE id = ". $id);
    }
    else {
      $q = $dbh->prepare("INSERT INTO db_privs(grants, status, user_id, db_id) VALUES ".
                        "('".rtrim($grants, ',')."', 'N', ".$_POST['dbuser'].", ".$_POST['dbname'].")");
    }
    $q->execute();
    // We need ID of last record
    $q = $dbh->prepare("SELECT id FROM db_privs WHERE db_id = ".$_POST['dbname']. " AND user_id = ".$_POST['dbuser']);
    $q->execute();
    $r = $q->fetch();
    $id = $r['id'];
    // to build and send json to javascript
    $q = $dbh->prepare("SELECT dp.id, du.login, dp.user_id, dn.name, dp.db_id, dp.grants FROM db_privs dp JOIN db_name dn ON dp.db_id = dn.id ".
                      "JOIN db_user du ON dp.user_id = du.id WHERE dp.status NOT LIKE 'D' AND du.host_id = ".$_POST['serverid']);
    $q->execute();
    while($r = $q->fetch()) {
      $json[$r['id']] = array(
        'dbuser' => $r['login'],
        'user_id' => $r['user_id'],
        'dbid' => $r['db_id'],
        'dbname' => $r['name'],
        'grants' => explode(",", $r['grants'])
      );
    }
    updateConfigVersion($dbh, $_POST['serverid'], "db_privs");
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if (isset($_GET['getperm'])) {
    $q = $dbh->prepare("SELECT id, db_id, user_id, grants FROM db_privs WHERE id = ".$_GET['getperm']);
    $q->execute();
    $r = $q->fetch();

    $json = array(
      'id' => $r['id'],
      'dblogin' => $r['user_id'],
      'dbname' => $r['db_id'],
      'grants' => explode(",", rtrim($r['grants']))
    );

    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if (isset($_GET['rmperm'])) {
    $q = $dbh->prepare("UPDATE db_privs SET status = 'D' WHERE id = ".$_GET['rmperm']);
    $q->execute();

    $q = $dbh->prepare("SELECT du.host_id FROM db_privs dp JOIN db_user du ON dp.user_id = du.id WHERE dp.id = ". $_GET['rmperm']);
    $q->execute();
    $r = $q->fetch();

    updateConfigVersion($dbh, $r['host_id'], "db_privs");
  }
  if (isset($_GET['chuserpass'])) {
    $hash = "*" . strtoupper(sha1(sha1($_POST['dbpass'], TRUE)));
    $q = $dbh->prepare("UPDATE db_user SET pass = '".$hash."', status = 'U' WHERE id = ".$_POST['dbid']);
    $q->execute();
    updateConfigVersion($dbh, $_POST['serverid'], "db_user");
  }
  if (isset($_GET['rmdbuser'])) {
    $q = $dbh->prepare("SELECT host_id FROM db_user WHERE id = ".$_GET['rmdbuser']);
    $q->execute();
    $r = $q->fetch();

    $q = $dbh->prepare("UPDATE db_user SET status = 'D' WHERE id = ". $_GET['rmdbuser']);
    $q->execute();

    $q2 = $dbh->prepare("SELECT count(*) AS rowcnt FROM db_privs WHERE user_id = ".$_GET['rmdbuser']);
    $q2->execute();
    $r2 = $q2->fetch();
    if($r2['rowcnt'] > 0) {
      $q = $dbh->prepare("UPDATE db_privs SET status = 'D' WHERE user_id = ". $_GET['rmdbuser']);
      $q->execute();
      updateConfigVersion($dbh, $r['host_id'], "db_privs");
    }

    updateConfigVersion($dbh, $r['host_id'], "db_user");
  }
  if (isset($_GET['rmdb'])) {
    $q = $dbh->prepare("SELECT host_id FROM db_name WHERE id = ". $_GET['rmdb']);
    $q->execute();
    $r = $q->fetch();

    $q = $dbh->prepare("UPDATE db_name SET status = 'D' WHERE id = ". $_GET['rmdb']);
    $q->execute();

    $q2 = $dbh->prepare("SELECT count(*) AS rows FROM db_privs WHERE db_id = ". $_GET['rmdb']);
    $q2->execute();
    $r2 = $q2->fetch();
    if($r2['rows'] > 0) {
      $q = $dbh->prepare("UPDATE db_privs SET status = 'D' WHERE db_id = ". $_GET['rmdb']);
      $q->execute();
      updateConfigVersion($dbh, $r['host_id'], "db_privs");
    }

    updateConfigVersion($dbh, $r['host_id'], "db_name");
  }
?>
