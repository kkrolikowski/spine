<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['smtp'])) {
    $q = $dbh->prepare("SELECT count(*) AS cnt FROM settings_smtp");
    $q->execute();
    $r = $q->fetch();
    if($r['cnt'] > 0) {
      $q2 = $dbh->prepare("DELETE FROM settings_smtp");
      $q2->execute();
    }
    if(!empty($_POST['smtplogin'])) {
      if(isset($_POST['use_ssl']) && $_POST['use_ssl'] == "on") {
        $ssl = 1;
      }
      else {
        $ssl = 0;
      }
      $query = "INSERT INTO settings_smtp (host,port,login,password,`ssl`) ".
               "VALUES('".$_POST['smtphost']."', ".$_POST['smtpport'].", '".$_POST['smtplogin']."', ".
               "'".$_POST['smtppass']."', ".$ssl.")";
    }
    else {
      $query = "INSERT INTO settings_smtp (host,port) VALUES('".$_POST['smtphost']."', ".$_POST['smtpport'].")";
    }
    $q = $dbh->prepare($query);
    $q->execute();
  }
?>
