<?php
// Only allow user to access this page if they are logged in, otherwise redirect to login page
session_start();
if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: login.php");
    exit;
}
//Connect to DB
require_once "config.php";
?>
<!DOCTYPE html>
<html data-bs-theme="light" lang="en">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
    <title>Secure Universal Systems</title>
    <link rel="stylesheet" href="assets/bootstrap/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Nunito:200,200i,300,300i,400,400i,600,600i,700,700i,800,800i,900,900i&amp;display=swap">
    <link rel="stylesheet" href="assets/fonts/fontawesome-all.min.css">
</head>
<body id="page-top">
    <div id="wrapper">
        <nav class="navbar align-items-start sidebar sidebar-dark accordion p-0 navbar-dark" style="background-color: #006972;">
            <div class="container-fluid d-flex flex-column p-0"><a class="navbar-brand d-flex justify-content-center align-items-center sidebar-brand m-0" href="index.php" style="padding-top: 40px;padding-bottom: 40px;">
                    <div class="sidebar-brand-icon"><img src="assets/img/SUS.png" style="height: 62px;"></div>
                    <div class="sidebar-brand-text mx-3"><span>Secure <br>Universal <br>Systems</span></div>
                </a>
                <hr class="sidebar-divider my-0">
                <ul class="navbar-nav text-light" id="accordionSidebar">
                    <li class="nav-item"><a class="nav-link" href="index.php"><i class="fas fa-tachometer-alt"></i><span>Dashboard</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="employees.php"><i class="far fa-address-card"></i><span>Employees</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="ac.php"><i class="far fa-address-card"></i><span>Access Control</span></a></li>
                    <li class="nav-item"><a class="nav-link active" href="log.php"><i class="fas fa-table"></i><span>Access log</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="logout.php"><i class="fas fa-lock"></i><span>Logout</span></a></li>
                </ul>
                <div class="text-center d-none d-md-inline"></div>
            </div>
        </nav>
        <div class="d-flex flex-column" id="content-wrapper">
            <div id="content">
                
                <div class="container-fluid" style="padding-top: 24px;">
                    <div class="card shadow">
                        <div class="card-header py-3">
                            <p class="fs-4 text-primary m-0 fw-bold">Access Log</p>
                        </div>
                        <div class="card-body">
                            <div class="table-responsive table mt-2" id="dataTable" role="grid" aria-describedby="dataTable_info">
                                <table class="table my-0" id="dataTable">
                                    <thead>
                                        <tr>
                                            <th>Name</th>
                                            <th>Access State</th>
                                            <th>Time</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php
                                        //Fetches all logs from the database and echos them as table rows
                                        $sql = "SELECT employees.name as employeeName,logs.time,logs.success FROM logs LEFT OUTER JOIN employees ON logs.employeeID = employees.id ORDER BY time DESC";
                                        $result = $db->query($sql);
                                        
                                        if ($result->num_rows > 0) {
                                            while($row = $result->fetch_assoc()) {
                                                $accessStateText = $row["success"] == 1 ? "Access Granted" : "Access Denied";
                                                $employeeName = $row["employeeName"] == null ? "Removed User" : $row["employeeName"];
                                                echo "<tr><td>" . $employeeName . "</td><td>" . $accessStateText . "</td><td>" . $row["time"] . "</td></tr>";
                                            }
                                        }
                                        ?>
                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script src="assets/bootstrap/js/bootstrap.min.js"></script>
    <script src="assets/js/bs-init.js"></script>
    <script src="assets/js/theme.js"></script>
</body>
</html>