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
                    <li class="nav-item"><a class="nav-link active" href="index.php"><i class="fas fa-tachometer-alt"></i><span>Dashboard</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="employees.php"><i class="far fa-address-card"></i><span>Employees</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="ac.php"><i class="far fa-address-card"></i><span>Access Control</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="log.php"><i class="fas fa-table"></i><span>Access log</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="logout.php"><i class="fas fa-lock"></i><span>Logout</span></a></li>
                </ul>
                <div class="text-center d-none d-md-inline"></div>
            </div>
        </nav>
        <div class="d-flex flex-column" id="content-wrapper" style="padding-top: 32px;">
            <div id="content">
                <div class="container-fluid">
                    <div class="d-sm-flex justify-content-between align-items-center mb-4">
                        <h3 class="text-dark mb-0">Dashboard</h3>
                    </div>
                    <div class="row">
                        <div class="col-md-6 col-xl-3 mb-4">
                            <div class="card shadow border-start-primary py-2">
                                <div class="card-body">
                                    <div class="row align-items-center no-gutters">
                                        <div class="col me-2">
                                            <div class="text-uppercase text-primary fw-bold text-xs mb-1"><span>Last unlocked</span></div>
                                            <div class="text-dark fw-bold h5 mb-0"><span> 
                                                <?php
                                                    //Run sql query to get time of lastest log entry, and echo it
                                                    $sql = "SELECT time FROM logs ORDER BY id DESC LIMIT 1";
                                                    $result = $db->query($sql);
                                                    echo $result->fetch_assoc()['time'];
                                                ?>
                                            </span></div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-6 col-xl-3 mb-4">
                            <div class="card shadow border-start-success py-2">
                                <div class="card-body">
                                    <div class="row align-items-center no-gutters">
                                        <div class="col me-2">
                                            <div class="text-uppercase text-success fw-bold text-xs mb-1"><span>unlocked today(count)</span></div>
                                            <div class="text-dark fw-bold h5 mb-0"><span>
                                                <?php
                                                    //Run sql query to get the count of log entries today, and echo it
                                                    $sql = "SELECT COUNT(*) FROM logs WHERE time >= CURDATE()";
                                                    $result = $db->query($sql);
                                                    echo $result->fetch_assoc()['COUNT(*)'];
                                                ?>
                                            </span></div>
                                        </div>
                                        <div class="col-auto"><i class="fas fa-unlock-alt fa-2x text-gray-300"></i></div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-6 col-xl-3 mb-4">
                            <div class="card shadow border-start-info py-2">
                                <div class="card-body">
                                    <div class="row align-items-center no-gutters">
                                        <div class="col me-2">
                                            <div class="text-uppercase text-info fw-bold text-xs mb-1"><span>unlocked this month</span></div>
                                            <div class="row g-0 align-items-center">
                                                <div class="col-auto">
                                                    <div class="text-dark fw-bold h5 mb-0 me-3"><span>
                                                        <?php
                                                            //Run sql query to get the count of log entries this month, and echo it
                                                            $sql = "SELECT COUNT(*) FROM logs WHERE time BETWEEN NOW() - INTERVAL 30 DAY AND NOW();";
                                                            $result = $db->query($sql);
                                                            echo $result->fetch_assoc()['COUNT(*)'];
                                                        ?>
                                                    </span></div>
                                                </div>
                                            </div>
                                        </div>
                                        <div class="col-auto"><i class="fas fa-unlock fa-2x text-gray-300"></i></div>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="col-md-6 col-xl-3 mb-4">
                            <div class="card shadow border-start-warning py-2">
                                <div class="card-body">
                                    <div class="row align-items-center no-gutters">
                                        <div class="col me-2">
                                            <div class="text-uppercase text-warning fw-bold text-xs mb-1"><span>Registered employees</span></div>
                                            <div class="text-dark fw-bold h5 mb-0"><span>
                                                <?php
                                                    //Run sql query to get the count of rows in employees table, and echo it
                                                    $sql = "SELECT COUNT(*) FROM employees";
                                                    $result = $db->query($sql);
                                                    $employeeCount = $result->fetch_assoc()['COUNT(*)'];
                                                    echo $employeeCount;
                                                ?>
                                            </span></div>
                                        </div>
                                        <div class="col-auto"><i class="fas fa-users fa-2x text-gray-300"></i></div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-lg-7 col-xl-8">
                            <div class="card shadow mb-4">
                                <div class="card-header py-3">
                                    <h6 class="text-primary fw-bold m-0">Latest 6 log entries</h6>
                                </div>
                                <ul class="list-group list-group-flush">
                                    <?php
                                        //Run sql query to get the latest 6 log entries, and echo them as table rows
                                        $sql = "SELECT employees.name as employeeName,logs.time,logs.success FROM logs LEFT OUTER JOIN employees ON logs.employeeID = employees.id ORDER BY time DESC LIMIT 6";
                                        $result = $db->query($sql);
                                        
                                        if ($result->num_rows > 0) {
                                            while($row = $result->fetch_assoc()) {
                                                echo '<li class="list-group-item"><div class="row align-items-center no-gutters"><div class="col me-2"><h6 class="mb-0"><strong>';
                                                if ($row["success"] == "1"){
                                                    echo $row["employeeName"] . " unlocked the door";
                                                }
                                                else {
                                                    echo $row["employeeName"] . " failed to unlock the door";
                                                }
                                                echo '</strong></h6><span class="text-xs">';
                                                echo $row["time"];
                                                echo '</span></div></div></li>';
                                            }
                                        }
                                    ?>
                                </ul>
                            </div>
                        </div>
                        <div class="col-lg-5 col-xl-4">
                            <div class="card shadow mb-4">
                                <div class="card-header d-flex justify-content-between align-items-center">
                                    <h6 class="text-primary fw-bold m-0">Employees with door access</h6>
                                </div>
                                <div class="card-body">
                                    <?php
                                        //Run sql query to get the count of emplyoees with and without access
                                        $sql = "SELECT COUNT(*) FROM dooraccess";
                                        $result = $db->query($sql);
                                        $accessCount = $result->fetch_assoc()['COUNT(*)'];
                                        $nonAccessCount = $employeeCount-$accessCount;
                                    ?>
                                    <div class="chart-area"><canvas data-bss-chart="{&quot;type&quot;:&quot;doughnut&quot;,&quot;data&quot;:{&quot;labels&quot;:[&quot;Access&quot;,&quot;Not access&quot;],&quot;datasets&quot;:[{&quot;label&quot;:&quot;&quot;,&quot;backgroundColor&quot;:[&quot;#4e73df&quot;,&quot;#1cc88a&quot;],&quot;borderColor&quot;:[&quot;#ffffff&quot;,&quot;#ffffff&quot;],&quot;data&quot;:[&quot;<?php echo $accessCount; ?>&quot;,&quot;<?php echo $nonAccessCount; ?>&quot;]}]},&quot;options&quot;:{&quot;maintainAspectRatio&quot;:false,&quot;legend&quot;:{&quot;display&quot;:false,&quot;labels&quot;:{&quot;fontStyle&quot;:&quot;normal&quot;}},&quot;title&quot;:{&quot;fontStyle&quot;:&quot;normal&quot;}}}"></canvas></div>
                                    <div class="text-center small mt-4"><span class="me-2"><i class="fas fa-circle text-primary"></i>&nbsp;Access</span><span class="me-2"><i class="fas fa-circle text-success"></i>&nbsp;Not access</span></div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script src="assets/bootstrap/js/bootstrap.min.js"></script>
    <script src="assets/js/chart.min.js"></script>
    <script src="assets/js/bs-init.js"></script>
    <script src="assets/js/theme.js"></script>
</body>
</html>
