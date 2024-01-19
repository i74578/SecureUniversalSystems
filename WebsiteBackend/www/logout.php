<?php
// Initialize session and then destroy session variables
session_start();
$_SESSION = array();
session_destroy();
//Redirect to login page
header("location: login.php");
exit;
?>
