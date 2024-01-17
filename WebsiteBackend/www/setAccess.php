<?php

// Initialize the session
session_start();

if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: login.php");
    exit;
}

$data = json_decode(file_get_contents("php://input"),true);
 
if ($data !== null && isset($data['doorID']) && isset($data['employeeID']) && isset($data['access'])) {
    require_once "config.php";
    $doorID = mysqli_real_escape_string($db, $data['doorID']);
    $employeeID = mysqli_real_escape_string($db, $data['employeeID']);
    $access = mysqli_real_escape_string($db, $data['access']);

    $sql = "";
    if ($access){
        echo "Access added";
        $sql = "INSERT INTO dooraccess (doorID,employeeID) VALUES ($doorID,$employeeID);";

    }
    else {
        echo "Access removed";
        $sql = "DELETE FROM dooraccess WHERE doorID=$doorID AND employeeID=$employeeID";
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
