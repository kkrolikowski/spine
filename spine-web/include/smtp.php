<?php
  require 'Mail.php';

  function sendEmail($from, $to, $subject, $message, $params) {
    $host   = $params[0];
    $port   = $params[1];
    $login  = $params[2];
    $pass   = $params[3];
    $ssl    = $params[4];

    $headers = array(
      "From"    => "Spine <". $from .">",
      "To"      => $to,
      "Subject" => $subject
    );

    if($ssl) {
      $server = "ssl://".$host;
    }
    else {
      $server = $host;
    }

    if ($login != "none") {
      $smtp_params = array(
        "host"      => $server,
        "username"  => $login,
        "password"  => $pass,
        "auth"      => true,
        "port"      => $port
      );
    }
    else {
      $smtp_params = array(
        "host" => $server,
        "port" => $port
      );
    }
    $smtp = Mail::factory('smtp', $smtp_params);
    $mail = $smtp->send($to, $headers, $message);

    if(PEAR::isError($mail)) {
      echo $mail->getMessage();
    }
    else {
      echo "Message sent to " . $to . " successfully";
    }
  }
?>
