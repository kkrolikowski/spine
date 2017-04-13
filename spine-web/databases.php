<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['add'])) {
    $q = $dbh->prepare("INSERT INTO db_name(name, status, host_id, vhost_id) ".
                      "VALUES('".$_POST['dbname']."', 'N', ".$_POST['serverid'].
                      ", ".$_POST['vhostid'].")");
    $q->execute();

    $q = $dbh->prepare("SELECT id FROM db_name WHERE name = '".$_POST['dbname']."'");
    $q->execute();
    $r = $q->fetch();
    $dbid = $r['id'];

    $q = $dbh->prepare("SELECT d.id, d.name AS dbname, v.ServerName AS vhost FROM db_name d ".
                      "JOIN www v ON d.vhost_id = v.id WHERE d.id = ".$dbid);
    $q->execute();
    while ($r = $q->fetch()) {
      $json[$r['id']] = array(
        'dbname' => $r['dbname'],
        'vhost' => $r['vhost']
      );
    }
    header('Content-Type: application/json');
    echo json_encode($json);
  }
?>
