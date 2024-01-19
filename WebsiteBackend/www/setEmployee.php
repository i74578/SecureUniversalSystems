<?php
// Only allow user to access this page if they are logged in, otherwise redirect to login page
session_start();
if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: login.php");
    exit;
}
// Load input data as JSON
$data = json_decode(file_get_contents("php://input"),true);
 
// Connect to DB and run relevant SQL query based on action parameter if present
$sql_error = "";
if ($data !== null && isset($data['action'])) {
    require_once "config.php";

    $sql = "";
    if ($data['action'] == "newEmployee"){
        $sql = "INSERT INTO employees (name,pin,nuid) VALUES ('New Employee','9999','FFFFFFFF');";

    }
    else if ($data['action'] == "removeEmployee"){
        if (isset($data['id']) && is_numeric($data['id'])){
            $employeeId = mysqli_real_escape_string($db, $data['id']);
            $sql = "DELETE FROM employees WHERE id=$employeeId;";
        }
        else{
            $sql_error = "Invalid ID";
        }
    }
    else if ($data['action'] == "updateEmployee"){
        if (isset($data['id']) && isset($data['id']) && isset($data['id']) && isset($data['id'])){
            $employeeId = mysqli_real_escape_string($db, $data['id']);
            $employeeName = mysqli_real_escape_string($db, $data['name']);
            $employeePin = mysqli_real_escape_string($db, $data['pin']);
            $employeeNuid = mysqli_real_escape_string($db, $data['nuid']);

            if(ctype_xdigit($employeeNuid) && strlen($employeeNuid) == 8 && is_numeric($employeePin) && strlen($employeePin) == 4){
                $sql = "UPDATE employees SET name='$employeeName', pin='$employeePin', NUID='$employeeNuid' WHERE id=$employeeId;";
            }
            else{
                $sql_error = "Invalid PIN or NUID";
            }
        }
        else{
            $sql_error = "Invalid Parameters";
        }
    }
    else{
        $sql_error = "Invalid action";
    }
    if ($sql != ""){
        if ($db->query($sql) === TRUE) {
            echo "Success";
            include 'sendUpdatesACL.php';
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    }
    $db->close();
}
else{
    $sql_error = "Invalid data";
}
?>
