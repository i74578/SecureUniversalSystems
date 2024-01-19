<?php
// Only allow user to access this page if they are logged in, otherwise redirect to login page
session_start();
if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: login.php");
    exit;
}
// Load input data as JSON
$data = json_decode(file_get_contents("php://input"),true);
 
// If all required parameters are present, run relevant SQL query
if ($data !== null && isset($data['employeeID']) && isset($data['access'])) {
    require_once "config.php";
    $employeeID = mysqli_real_escape_string($db, $data['employeeID']);
    $access = mysqli_real_escape_string($db, $data['access']);
    $sql = "";
    if ($access){
        echo "Access added";
        $sql = "INSERT INTO dooraccess (employeeID) VALUES ($employeeID);";

    }
    else {
        echo "Access removed";
        $sql = "DELETE FROM dooraccess WHERE employeeID=$employeeID";
    }
    if ($db->query($sql) === TRUE) {
        echo "Success";
        include 'sendUpdatesACL.php';
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
    $db->close();
}
?>
