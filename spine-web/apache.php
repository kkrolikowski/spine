<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['addvhost'])) {
    if(count($_POST['ServerAlias']) == 0) {
      $ServerALias = "NaN";
    }
    else {
      foreach ($_POST['ServerAlias'] as $value) {
        $ServerALias .= $value . " ";
      }
    }
    if(! isset($_POST['htaccess'])) {
      $htaccess = "NaN";
    }
    else {
      $htaccess = $_POST['htaccess'];
    }
    $q = $dbh->prepare("INSERT INTO www(ServerName, ServerAlias, DocumentRoot, htaccess, user_id, system_id) VALUES('".
    $_POST['ServerName']. "', '". $ServerALias . "', '". $_POST['DocumentRoot']. "', '". $htaccess. "', ". $_POST['account'].
    ", ". $_POST['serverid'] .")");
    $q->execute();
    updateConfigVersion($dbh, $_POST['serverid']);
  }
?>