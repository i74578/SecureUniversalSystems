<?php
// Setup a mysqli connection to the database, with predefined credentials
define('DB_SERVER', 'localhost');
define('DB_USERNAME', 'phusr');
define('DB_PASSWORD', '7yLCTYetfQha4k5t');
define('DB_NAME', 'sus');
$db = mysqli_connect(DB_SERVER, DB_USERNAME, DB_PASSWORD, DB_NAME);
if($db === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
?>
