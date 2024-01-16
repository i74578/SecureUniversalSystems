<?php
// Initialize the session
session_start();

if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: login.php");
    exit;
}

// Include config file
require_once "config.php";
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
echo "client connected";

$doorToCheck = 1;

$sql = "SELECT employees.pin, employees.nuid FROM dooraccess LEFT JOIN employees ON (dooraccess.employeeID = employees.id) WHERE dooraccess.doorID=".$doorToCheck;
$result = $db->query($sql);
$payload = str_pad(strval($result->num_rows), 2, "0", STR_PAD_LEFT);
if ($result->num_rows > 0) {
  while($row = $result->fetch_assoc()) {
    echo "quadro";
    $payload .= $row['nuid'] . $row['pin'];
  }
}
echo $payload;
$mqtt->publish('sus/accessList', $payload, 0);
$mqtt->disconnect();
?>
