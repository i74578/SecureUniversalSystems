<?php
/* Database credentials. Assuming you are running MySQL
server with default setting (user 'root' with no password) */
define('DB_SERVER', 'localhost');
define('DB_USERNAME', 'phusr');
define('DB_PASSWORD', 'ENTER_PASSWORD_HERE');
define('DB_NAME', 'sus');


/* Attempt to connect to MySQL database */
$db = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD, DB_NAME);

// Check connection
if($db === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
?>
