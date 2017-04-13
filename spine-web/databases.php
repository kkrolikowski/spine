<?php
  include_once './include/config.php';
  include_once './include/functions.php';
  $dbh = DBconnect();

  if(defined($_GET['add'])) {
    echo $_POST;
  }
?>
