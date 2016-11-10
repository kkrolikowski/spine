<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if (isset($_GET['add'])) {
    // sprawdzamy, czy takie konto juz istnieje w bazie
    $q = $dbh->prepare("SELECT count(*) AS usercnt FROM sysusers WHERE login = '".$_POST['login']."' AND system_id = ".$_POST['serverid']);
    $q->execute();
    $r = $q->fetch();
    if ($r['usercnt'] > 0) {
      $message = "X-Message: Konto ". $_POST['login'] ." jest juz dodane do bazy.";
      header($message, true, 406);
    }
    else {
      // ustalamy nastepny wolny UID w systemie.
      // Jesli jest tylko konto roota, ustawiamy UID na 10000
      // w przeciwnym wypadku bierzemy nastepny wolny
      $uid_last = lastUID($dbh, $_POST['serverid']);
      if ($uid_last == 0) {
        $uid = 10000;
      }
      else {
        $uid = $uid_last + 1;
      }
      $sha512 = sha512_pass($_POST['userpass']);

      isset($_POST['isActive'])         ? $active = 1 : $active = 0;
      isset($_POST['shell'])            ? $shell  = 1 : $shell  = 0;
      isset($_POST['sshkey_enable'])    ? $usekey = 1 : $usekey = 0;

      if (isset($_POST['expEnable'])) {
        $timeArr = explode("/", $_POST['expdate']);
        $timeStr .= $timeArr[2] . "-" . $timeArr[1] . "-" . $timeArr[0];
        $time = new DateTime($timeStr);
        $exptime = $time->format('U');
      }
      else {
        $exptime = "Never";
      }
      setlocale(LC_ALL, 'pl_PL.utf8');
      $gecos = iconv('UTF-8', 'ASCII//TRANSLIT//IGNORE', $_POST['fullname']);

      $q = $dbh->prepare("INSERT INTO sysusers(login,pass,fullname,email,system_id,gecos,uid,gid,active,expiration, shell, sshkeys) ".
                          "VALUES('".$_POST['login']."', '".$sha512."', '".$_POST['fullname'].
                          "', '".$_POST['email']."', ".$_POST['serverid'].", '".$gecos."', ".$uid.", ".$uid.
                          ", ".$active.", '".$exptime."', ".$shell.", ".$usekey.")");
      $q->execute();

      $q = $dbh->prepare("SELECT id,login,fullname,email FROM sysusers WHERE login = '".$_POST['login']."' AND system_id = ".$_POST['serverid']);
      $q->execute();
      $r = $q->fetch();

      if($usekey) {
        foreach ($_POST['sshkey'] as $sshkey) {
          $q2 = $dbh->prepare("INSERT INTO sysusers_sshkeys(sshkey,user_id) VALUES('".$sshkey."', ".$r['id'].")");
          $q2->execute();
        }
      }
      updateConfigVersion($dbh, $_POST['serverid']);
      $json = array(
        'login' => $r['login'],
        'fullname' => $r['fullname'],
        'email' => $r['email']
      );
      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
?>
