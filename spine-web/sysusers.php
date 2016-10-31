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
      $uid_last = lastUID($dbh);
      if ($uid_last == 0) {
        $uid = 10000;
      }
      else {
        $uid = $uid_last + 1;
      }
      $sha512 = sha512_pass($_POST['userpass']);

      if ($_POST['isActive'] == "on") {
        $active = 1;
      }
      else {
        $active = 0;
      }
      if ($_POST['expEnable'] == "on") {
        $timeArr = explode("/", $_POST['expdate']);
        $timeStr .= $timeArr[2] . "-" . $timeArr[1] . "-" . $timeArr[0];
        $time = new DateTime($timeStr);
        $unixtime = $time->format('U');
      }
      $q = $dbh->prepare("INSERT INTO sysusers(login,pass,fullname,email,system_id,uid,gid,active,expiration) ".
                          "VALUES('".$_POST['login']."', '".$sha512."', '".$_POST['fullname'].
                          "', '".$_POST['email']."', ".$_POST['serverid'].", ".$uid.", ".$uid.
                          ", ".$active.", ".$unixtime.")");
      $q->execute();

      $q = $dbh->prepare("SELECT id,login,fullname,email FROM sysusers WHERE login = '".$_POST['login']."'");
      $q->execute();
      $r = $q->fetch();

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
