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

      $q = $dbh->prepare("SELECT id FROM db_name WHERE name = '".$_POST['dbname']."'");
      $q->execute();
      $r = $q->fetch();
      $dbid = $r['id'];

      $q = $dbh->prepare("SELECT d.id, d.name AS dbname, CASE d.vhost_id WHEN 1 THEN ".
                        "v.ServerName ELSE 'None' END AS vhost, d.host_id AS serverid ".
                        "FROM db_name d LEFT JOIN www v ON d.vhost_id = v.id WHERE d.id =".$dbid);
      $q->execute();
      $r = $q->fetch();

      $json = array(
        'id' => $r['id'],
        'dbname' => $r['dbname'],
        'vhost' => $r['vhost'],
        'serverid' => $r['serverid']
      );
      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
?>
