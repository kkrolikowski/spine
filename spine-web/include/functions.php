<?php
  include_once './include/config.php';

  function DBconnect() {
    $dsn = 'mysql:host='.DBHOST.';dbname='.DBNAME;
    $dbh = new PDO($dsn, DBUSER, DBPASS);
    return $dbh;
  }
?>
