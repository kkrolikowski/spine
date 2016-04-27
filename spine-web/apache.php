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

    $q = $dbh->prepare("INSERT INTO www(ServerName, ServerAlias, DocumentRoot, htaccess, user_id, system_id, status) VALUES('".
    $_POST['sn']. "', '". $ServerALias . "', '". $DocumentRoot. "', '". $htaccess. "', ". $_POST['account'].
    ", ". $_POST['serverid'] .", 'A')");
    $q->execute();

    // ustalamy ID vhosta
    $q = $dbh->prepare("SELECT id FROM www WHERE ServerName = '".$_POST['sn']."'");
    $q->execute();
    $r = $q->fetch();
    $vhostid = $r['id'];

    foreach ($_POST['vhopts'] as $opt) {
      $q = $dbh->prepare("INSERT INTO www_opts_selected(vhost_id,opt_id) VALUES(".$vhostid.", ".$opt.")");
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
       "group_concat(wo.id separator ' ') as selected_optid, ".
       "group_concat(wo.vhostopt separator ' ') as selected_options ".
       "FROM www w ".
        "JOIN sysusers su ".
       "ON w.user_id =su.id ".
        "JOIN www_opts_selected wos ".
	     "ON w.id = wos.vhost_id ".
        "JOIN www_opts wo ".
       "ON wo.id = wos.opt_id ".
       "WHERE w.id = ". $_GET['vhostid']);
    $q->execute();
    $r = $q->fetch();

    $sa = array_filter(explode(" ", $r['ServerAlias']));

    $opts = array_filter(explode(" ", $r['selected_options']));
    $optids = array_filter(explode(" ", $r['selected_optid']));
    $optsWithID = array_combine($opts, $optids);

    $json = array(
      'id' => $r['id'],
      'ServerName' => $r['ServerName'],
      'ServerAlias' => $sa,
      'DocumentRoot' => $r['DocumentRoot'],
      'htaccess' => $r['htaccess'],
      'user' => $r['login'],
      'vhost_options' => $optsWithID
    );
    header('Content-Type: application/json');
    echo json_encode($json);
  }
?>
