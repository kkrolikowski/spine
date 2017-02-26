<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if (isset($_GET['add'])) {
    // sprawdzamy, czy takie konto juz istnieje w bazie
    $q = $dbh->prepare("SELECT count(*) AS usercnt FROM sysusers WHERE login = '".$_POST['login']."' AND status != 'D' AND system_id = ".$_POST['serverid']);
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
      isset($_POST['sudo'])             ? $sudo   = 1 : $sudo   = 0;

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

      $q = $dbh->prepare("INSERT INTO sysusers(login,pass,fullname,email,system_id,gecos,uid,gid,active,expiration, shell, sshkeys, status, sudo) ".
                          "VALUES('".$_POST['login']."', '".$sha512."', '".$_POST['fullname'].
                          "', '".$_POST['email']."', ".$_POST['serverid'].", '".$gecos."', ".$uid.", ".$uid.
                          ", ".$active.", '".$exptime."', ".$shell.", ".$usekey.", 'N', ".$sudo.")");
      $q->execute();

      $q = $dbh->prepare("SELECT id,login,fullname,email,active FROM sysusers WHERE login = '".$_POST['login']."' AND system_id = ".$_POST['serverid']);
      $q->execute();
      $r = $q->fetch();

      if($usekey) {
        foreach ($_POST['sshkey'] as $sshkey) {
          $q2 = $dbh->prepare("INSERT INTO sysusers_sshkeys(sshkey,user_id) VALUES('".$sshkey."', ".$r['id'].")");
          $q2->execute();
        }
      }
      updateConfigVersion($dbh, $_POST['serverid'], "sysusers");
      $json = array(
        'id'        => $r['id'],
        'login'     => $r['login'],
        'fullname'  => $r['fullname'],
        'email'     => $r['email'],
        'isactive'  => $r['active']
      );
      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
  if(isset($_GET['edit'])) {
    $q = $dbh->prepare("SELECT su.id, su.fullname, su.email, su.login, su.active, su.expiration, su.shell, ".
                       "su.status, su.sudo,	CASE su.sshkeys WHEN 1 THEN GROUP_CONCAT(s.sshkey SEPARATOR ',') ELSE 'NaN' ".
                       "END AS sshkeys FROM sysusers su LEFT JOIN sysusers_sshkeys s ON s.user_id = su.id ".
                       "WHERE	su.id = ". $_GET['edit']);
    $q->execute();
    $r = $q->fetch();
    $json = array(
      'id'        => $r['id'],
      'fullname'  => $r['fullname'],
      'email'     => $r['email'],
      'login'     => $r['login'],
      'active'    => $r['active'],
      'expire'    => $r['expiration'],
      'shell'     => $r['shell'],
      'status'    => $r['status'],
      'sudo'      => $r['sudo'],
      'sshkeys'   => $r['sshkeys']
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if (isset($_GET['update'])) {
    setlocale(LC_ALL, 'pl_PL.utf8');
    $gecos = iconv('UTF-8', 'ASCII//TRANSLIT//IGNORE', $_POST['fullname_edit']);

    isset($_POST['isActive_edit'])        ? $active = 1 : $active = 0;
    isset($_POST['shell_edit'])           ? $shell  = 1 : $shell  = 0;
    isset($_POST['sudo_edit'])            ? $sudo   = 1 : $sudo   = 0;
    isset($_POST['sshkey_enable_edit'])   ? $sshkey = 1 : $sshkey = 0;

    if (isset($_POST['expEnable_edit'])) {
      $timeArr = explode("/", $_POST['expdate_edit']);
      $timeStr .= $timeArr[2] . "-" . $timeArr[1] . "-" . $timeArr[0];
      $time = new DateTime($timeStr);
      $exptime = $time->format('U');
    }
    else {
      $exptime = "Never";
    }
    if(strlen($_POST['password_edit'])) {
      $pass = sha512_pass($_POST['password_edit']);
    }
    else {
      $q = $dbh->prepare("SELECT pass FROM sysusers WHERE id = ".$_GET['update']);
      $q->execute();
      $r = $q->fetch();
      $pass = $r['pass'];
    }
    $q = $dbh->prepare("UPDATE sysusers SET fullname = '".$_POST['fullname_edit']."', email = '".$_POST['email_edit']."', pass = '".$pass."', ".
                      "gecos = '".$gecos."', login = '".$_POST['login_edit']."', active = ".$active.", expiration = '".$exptime."', ".
                      "shell = '".$shell."', sshkeys = ".$sshkey.", sudo = ".$sudo.", status = 'U' WHERE id = ".$_GET['update']);
    $q->execute();
    if ($sshkey) {
      $q = $dbh->prepare("DELETE FROM sysusers_sshkeys WHERE user_id = ".$_GET['update']);
      $q->execute();
      foreach ($_POST['sshkey_edit'] as $key) {
        $q = $dbh->prepare("INSERT INTO sysusers_sshkeys (sshkey, user_id) VALUES('".$key."', ".$_GET['update'].")");
        $q->execute();
      }
    }
    else {
      $q = $dbh->prepare("SELECT count(*) AS count FROM sysusers_sshkeys WHERE user_id = ".$_GET['update']);
      $q->execute();
      $r = $q->fetch();
      if($r['count'] > 0) {
        $q = $dbh->prepare("DELETE FROM sysusers_sshkeys WHERE user_id = ".$_GET['update']);
        $q->execute();
      }
    }
    updateConfigVersion($dbh, $_POST['sid'], "sysusers");
    $q = $dbh->prepare("SELECT id,login,fullname,email,active FROM sysusers WHERE login = '".$_POST['login_edit']."' AND system_id = ".$_POST['sid']);
    $q->execute();
    $r = $q->fetch();
    $json = array(
      'id'        => $r['id'],
      'login'     => $r['login'],
      'fullname'  => $r['fullname'],
      'email'     => $r['email'],
      'isactive'  => $r['active']
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if(isset($_GET['rmuser'])) {
    $q = $dbh->prepare("UPDATE sysusers SET status = 'D' WHERE id = ".$_GET['rmuser']);
    $q->execute();
    updateConfigVersion($dbh, $_POST['serverid'], "sysusers");
  }
  if(isset($_GET['lock'])) {
    if($_GET['lock'] == 1) {
      $query = "UPDATE sysusers SET active = 0 WHERE id = ". $_GET['userid'];
    }
    else {
      $query = "UPDATE sysusers SET active = 1 WHERE id = ". $_GET['userid'];
    }
    $q = $dbh->prepare($query);
    $q->execute();
  }
?>
