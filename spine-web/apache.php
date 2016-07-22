<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['addvhost'])) {

    // sprawdzamy czy istnieje juz taki ServerName
    $result = "NaN";
    if(($result = HostExist($dbh, $_POST['sn'], $_POST['sa'], $_POST['serverid'])) != "NaN") {
      $message = "X-Message: Witryna ". $result ." jest juz dodana do bazy.";
      header($message, true, 406);
    }
    else {
      // sklejamy poszczegolne domeny w jeden string
      foreach ($_POST['sa'] as $value) {
        $ServerALias .= $value . " ";
      }
      $ServerAliasClean = substr($ServerALias, 0, -1);

      $htaccess = $_POST['htaccess'];

      // Konfiguracja DocumentRoot w zaleznosci od konta
      $q = $dbh->prepare("SELECT login FROM sysusers WHERE id = ". $_POST['account']);
      $q->execute();
      $r = $q->fetch();
      if($r['login'] == "root") {
        $DocumentRoot = "/var/www/". $_POST['sn'] ."/htdocs";
      }
      else {
        $DocumentRoot = "/home/" .$r['login']. "/public_html/". $_POST['sn'];
      }

      $q = $dbh->prepare("INSERT INTO www(ServerName, ServerAlias, DocumentRoot, htaccess, user_id, system_id, status, access_order) VALUES('".
      $_POST['sn']. "', '". $ServerAliasClean . "', '". $DocumentRoot. "', '". $htaccess. "', ". $_POST['account'].
      ", ". $_POST['serverid'] .", 'A', '".$_POST['access_order']."')");
      $q->execute();

      // ustalamy ID vhosta
      $q = $dbh->prepare("SELECT id FROM www WHERE ServerName = '".$_POST['sn']."'");
      $q->execute();
      $r = $q->fetch();
      $vhostid = $r['id'];

      // opcje, ktore zostaly wybrane
      foreach ($_POST['vhopts'] as $opt) {
        $q = $dbh->prepare("INSERT INTO www_opts_selected(vhost_id,opt_id) VALUES(".$vhostid.", ".$opt.")");
        $q->execute();
      }

      $vhostAccess = array_combine($_POST['fromhost'], $_POST['allow']);
      foreach ($vhostAccess as $host => $allow) {
        $q = $dbh->prepare("INSERT INTO www_access(fromhost,access_permission, vhost_id) " .
                            "VALUES('".$host."', ".$allow.", ".$vhostid.")");
        $q->execute();
      }
      if(isset($_POST['htusers'])) {
        foreach ($_POST['htusers'] as $userid) {
          $q = $dbh->prepare("INSERT INTO www_users_access(user_id, vhost_id, server_id) VALUES(".$userid.", ".$vhostid.", ".$_POST['serverid'].")");
          $q->execute();
        }
        $htpasswdStatus = 1;
      }
      else {
        $q = $dbh->prepare("INSERT INTO www_users_access(user_id, vhost_id, server_id) VALUES(0, ".$vhostid.", ".$_POST['serverid'].")");
        $q->execute();
        $htpasswdStatus = 0;
      }
      $q = $dbh->prepare("UPDATE www SET htpasswd = ".$htpasswdStatus." WHERE id = ".$vhostid);
      $q->execute();
      updateConfigVersion($dbh, $_POST['serverid']);

      $vhostAccess = vhostAccessLevel($dbh, $vhostid);

      $q = $dbh->prepare("SELECT ServerName,htpasswd FROM www WHERE id = ". $vhostid);
      $q->execute();
      $r = $q->fetch();

      $json = array(
        'id' => $vhostid,
        'ServerName' => $r['ServerName'],
        'password_protection' => $r['htpasswd'],
        'vhostAccess' => $vhostAccess
      );
      header('Content-Type: application/json');
      echo json_encode($json);
    }
  }
  if(isset($_GET['addhtuser'])) {
    if($_POST['password'] == $_POST['confirm']) {
      $secret = base64_encode(sha1($_POST['password'], true));
      $pass = "{SHA}" . $secret;

      if(!htuserExist($dbh, $_POST['login'], $_POST['serverid'])) {
        $q = $dbh->prepare("INSERT INTO www_users(login,password,system_id) ".
        "VALUES('".$_POST['login']."', '".$pass."', ".$_POST['serverid'].")");
        $q->execute();

        $q = $dbh->prepare("SELECT id FROM www_users WHERE login = '".$_POST['login']."'");
        $q->execute();
        $r = $q->fetch();
        $id = $r['id'];

        if(isset($id)) {
          $q = $dbh->prepare("SELECT id, login, system_id FROM www_users WHERE id = ".$id);
          $q->execute();
          $r = $q->fetch();

          $json = array(
            'id' => $r['id'],
            'login' => $r['login'],
            'serverid' => $r['system_id']
          );
          header('Content-Type: application/json');
          echo json_encode($json);
        }
      }
      else {
        $message = "X-Message: Konto ". $_POST['login'] ." jest juz dodane do bazy.";
        header($message, true, 406);
      }
    }
  }
  if(isset($_GET['vhostid'])) {
    $q = $dbh->prepare("SELECT w.id, w.ServerName, w.ServerAlias, w.DocumentRoot, w.htaccess, su.login, ".
      "GROUP_CONCAT(DISTINCT wo.id SEPARATOR ' ') AS selected_optid, ".
      "GROUP_CONCAT(DISTINCT wo.vhostopt SEPARATOR ' ') AS selected_options, ".
      "GROUP_CONCAT(DISTINCT CONCAT(wa.fromhost, ':', wa.access_permission) SEPARATOR ',') AS accesslist, " .
      "w.access_order, ".
      "w.htpasswd, ".
      "GROUP_CONCAT(DISTINCT wu.id SEPARATOR ' ') AS htuserid, ".
      "GROUP_CONCAT(DISTINCT wu.login SEPARATOR ' ') AS htuser ".
      "FROM www w ".
        "JOIN sysusers su ON w.user_id = su.id ".
        "JOIN www_opts_selected wos ON w.id = wos.vhost_id ".
        "JOIN www_opts wo ON wo.id = wos.opt_id ".
        "JOIN www_access wa ON wa.vhost_id = w.id ".
        "LEFT JOIN www_users_access wua ON wua.vhost_id = w.id ".
		    "LEFT JOIN www_users wu ON wu.id = wua.user_id ".
      "WHERE w.id = ".$_GET['vhostid']);
    $q->execute();
    $r = $q->fetch();

    $sa = array_filter(explode(" ", $r['ServerAlias']));

    $opts = array_filter(explode(" ", $r['selected_options']));
    $optids = array_filter(explode(" ", $r['selected_optid']));
    $optsWithID = array_combine($optids, $opts);

    $htuser = array_filter(explode(" ", $r['htuser']));
    $htuserid = array_filter(explode(" ", $r['htuserid']));
    $htusers = array_combine($htuserid, $htuser);

    $access = array_filter(explode(",", $r['accesslist']));
    $accessArr = array();
    foreach ($access as $val) {
      $tmp = explode(":", $val);
      $accessArr[$tmp[0]] = $tmp[1];
    }
    $json = array(
      'id' => $r['id'],
      'ServerName' => $r['ServerName'],
      'ServerAlias' => $sa,
      'DocumentRoot' => $r['DocumentRoot'],
      'htaccess' => $r['htaccess'],
      'user' => $r['login'],
      'vhost_options' => $optsWithID,
      'access_order' => $r['access_order'],
      'access_list' => $accessArr,
      'htpasswd' => $r['htpasswd'],
      'htusers' => $htusers
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if(isset($_GET['edit'])) {
    $opts = array();

    foreach ($_POST['sa'] as $value) {
      $sa .= $value . " ";
    }
    $saClean = substr($sa, 0, -1);

    foreach ($_POST['opts'] as $value) {
      array_push($opts, $value);
    }
    $q = $dbh->prepare("UPDATE www SET ServerAlias = '". $saClean ."', htaccess = '". $_POST['htaccess'].
                        "', access_order = '".$_POST['access_order']."', htpasswd = ".$_POST['htpasswd']." WHERE id = ". $_GET['edit']);
    $q->execute();

    $q = $dbh->prepare("DELETE FROM www_opts_selected WHERE vhost_id = ". $_GET['edit']);
    $q->execute();
    foreach ($opts as $value) {
      $q = $dbh->prepare("INSERT INTO www_opts_selected(vhost_id, opt_id) VALUES(".$_GET['edit'].", ".$value.")");
      $q->execute();
    }

    $q = $dbh->prepare("DELETE FROM www_access WHERE vhost_id = ". $_GET['edit']);
    $q->execute();
    $accessList = array_combine($_POST['fromhost'], $_POST['allow']);
    foreach ($accessList as $host => $permission) {
      $q = $dbh->prepare("INSERT INTO www_access(fromhost,access_permission,vhost_id) " .
                        "VALUES('".$host."', ".$permission.", ".$_GET['edit'].")");
      $q->execute();
    }

    $q = $dbh->prepare("DELETE FROM www_users_access WHERE vhost_id = ". $_GET['edit']. " AND server_id = ". $_POST['serverid']);
    $q->execute();
    foreach ($_POST['htusers'] as $htuser) {
      $q = $dbh->prepare("INSERT INTO www_users_access(user_id, vhost_id, server_id) VALUES(".$htuser.", ".$_GET['edit'].", ".$_POST['serverid'].")");
      $q->execute();
    }
    updateConfigVersion($dbh, $_POST['serverid']);

    $vhostid = $_GET['edit'];
    $vhostAccess = vhostAccessLevel($dbh, $vhostid);

    $q = $dbh->prepare("SELECT ServerName,htpasswd FROM www WHERE id = ". $vhostid);
    $q->execute();
    $r = $q->fetch();

    $json = array(
      'id' => $vhostid,
      'ServerName' => $r['ServerName'],
      'password_protection' => $r['htpasswd'],
      'vhostAccess' => $vhostAccess
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if(isset($_GET['htusers'])) {
    $q = $dbh->prepare("SELECT id, login FROM www_users WHERE system_id = ".$_GET['htusers']);
    $q->execute();
    while ($r = $q->fetch()) {
      $json[$r['id']] = $r['login'];
    }
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if(isset($_GET['rmuser'])) {

    // sprawdzamy czy user nie jest uzywany w ktorejs witrynie
    $q = $dbh->prepare("SELECT w.ServerName AS vhost, wu.login ".
                      "FROM www w JOIN www_users_access wua ON w.id = wua.vhost_id JOIN www_users wu ON wu.id = wua.user_id WHERE wua.user_id = ".$_GET['rmuser']);
    $q->execute();
    if($q->rowCount() > 0) {
      while ($r = $q->fetch()) {
        $vhosts_raw .= $r['vhost'] . ", ";
      }
      $vhosts = substr($vhosts_raw, 0, -2);
      $message = "X-Message: Konto " . $r['login']. " jest dodane do witryn: " . $vhosts;
      header($message, true, 406);
    }
    else {
      $q = $dbh->prepare("DELETE FROM www_users_access WHERE user_id = ".$_GET['rmuser']);
      $q->execute();
      $q = $dbh->prepare("DELETE FROM www_users WHERE id = ".$_GET['rmuser']);
      $q->execute();

      updateConfigVersion($dbh, $_POST['serverid']);
    }
  }
  if(isset($_GET['chpass'])) {
    $secret = base64_encode(sha1($_POST['password'], true));
    $pass = "{SHA}" . $secret;

    $q = $dbh->prepare("UPDATE www_users SET password = '".$pass."' WHERE id = ".$_POST['id']);
    $q->execute();

    updateConfigVersion($dbh, $_POST['serverid']);
  }
  if(isset($_GET['vhid'])) {
    $q = $dbh->prepare("SELECT ServerName FROM www WHERE id = ". $_GET['vhid']);
    $q->execute();
    $r = $q->fetch();

    $json = array('vhost' => $r['ServerName']);
    header('Content-Type: application/json');
    echo json_encode($json);
  }
?>
