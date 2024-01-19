<?php
/*  When this file is executed, the server reads the current ACL from 
the database and then publishes it to the sus/accessList MQTT topic. 
This is called after a employee attributes or access is changed  */

// Only allow user to access this page if they are logged in, otherwise redirect to login page
session_start();
if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: login.php");
    exit;
}
//Connect to DB
require_once "config.php";

//Include MQTT library and connect to MQTT broker, with predefined credentials
require('vendor/autoload.php');
use \PhpMqtt\Client\MqttClient;
use \PhpMqtt\Client\ConnectionSettings;
$server   = '127.0.0.1';
$port     = 1883;
$clientId = 'test-publisher';
$username = 'backendServer';
$password = 'mppyuOn7xH0V9xc';
$clean_session = false;
$mqtt_version = MqttClient::MQTT_3_1_1;
$connectionSettings = (new ConnectionSettings)
  ->setUsername($username)
  ->setPassword($password)
  ->setKeepAliveInterval(60)
  ->setLastWillTopic('sus/accessList')
  ->setLastWillMessage('client disconnect')
  ->setLastWillQualityOfService(1);
$mqtt = new MqttClient($server, $port, $clientId, $mqtt_version);
$mqtt->connect($connectionSettings, $clean_session);
echo "Connected to MQTT broker\n";

//Run sql query to get fetch all users with access
$sql = "SELECT employees.pin, employees.nuid FROM dooraccess LEFT JOIN employees ON (dooraccess.employeeID = employees.id)";
$result = $db->query($sql);
$payload = str_pad(strval($result->num_rows), 2, "0", STR_PAD_LEFT);
if ($result->num_rows > 0) {
  while($row = $result->fetch_assoc()) {
    $payload .= $row['nuid'] . $row['pin'];
  }
  echo "ACL to publish: $payload";

  //Publish ACL to MQTT broker
  $mqtt->publish('sus/accessList', $payload, 0);
}
$mqtt->disconnect();
?>
