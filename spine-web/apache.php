<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(isset($_GET['addvhost'])) {

    // sklejamy poszczegolne domeny w jeden string
    foreach ($_POST['sa'] as $value) {
      $ServerALias .= $value . " ";
    }

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
    $_POST['sn']. "', '". $ServerALias . "', '". $DocumentRoot. "', '". $htaccess. "', ". $_POST['account'].
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
    updateConfigVersion($dbh, $_POST['serverid']);

    $q = $dbh->prepare("SELECT ServerName FROM www WHERE id = ". $vhostid);
    $q->execute();
    $r = $q->fetch();

    $json = array(
      'id' => $vhostid,
      'ServerName' => $r['ServerName']
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if(isset($_GET['vhostid'])) {
    $q = $dbh->prepare("SELECT w.id, w.ServerName, w.ServerAlias, w.DocumentRoot, w.htaccess, su.login, ".
      "GROUP_CONCAT(DISTINCT wo.id SEPARATOR ' ') AS selected_optid, ".
      "GROUP_CONCAT(DISTINCT wo.vhostopt SEPARATOR ' ') AS selected_options, ".
      "GROUP_CONCAT(DISTINCT CONCAT(wa.fromhost, ':', wa.access_permission) SEPARATOR ',') AS accesslist, " .
      "w.access_order ".
      "FROM www w ".
        "JOIN sysusers su ON w.user_id = su.id ".
        "JOIN www_opts_selected wos ON w.id = wos.vhost_id ".
        "JOIN www_opts wo ON wo.id = wos.opt_id ".
        "JOIN www_access wa ON wa.vhost_id = w.id ".
      "WHERE w.id = ".$_GET['vhostid']);
    $q->execute();
    $r = $q->fetch();

    $sa = array_filter(explode(" ", $r['ServerAlias']));

    $opts = array_filter(explode(" ", $r['selected_options']));
    $optids = array_filter(explode(" ", $r['selected_optid']));
    $optsWithID = array_combine($optids, $opts);

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
      'access_list' => $accessArr
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
  if(isset($_GET['edit'])) {
    $opts = array();

    if($_POST['sa'][0] == "NaN") {
      $sa = "NaN";
    }
    else {
      foreach ($_POST['sa'] as $value) {
        $sa .= $value . " ";
      }
    }
    foreach ($_POST['opts'] as $value) {
      array_push($opts, $value);
    }
    $q = $dbh->prepare("UPDATE www SET ServerAlias = '". $sa ."', htaccess = '". $_POST['htaccess']. "' WHERE id = ". $_GET['edit']);
    $q->execute();

    $q = $dbh->prepare("DELETE FROM www_opts_selected WHERE vhost_id = ". $_GET['edit']);
    $q->execute();
    foreach ($opts as $value) {
      $q = $dbh->prepare("INSERT INTO www_opts_selected(vhost_id, opt_id) VALUES(".$_GET['edit'].", ".$value.")");
      $q->execute();
    }
    updateConfigVersion($dbh, $_POST['serverid']);
  }
?>
