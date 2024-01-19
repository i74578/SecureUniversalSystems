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
                    <li class="nav-item"><a class="nav-link active" href="ac.php"><i class="far fa-address-card"></i><span>Access Control</span></a></li>
                    <li class="nav-item"><a class="nav-link" href="log.php"><i class="fas fa-table"></i><span>Access log</span></a></li>
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
                            <p class="fs-5 text-primary m-0 fw-bold">Entrance Door Access</p>
                        </div>
                        <div class="card-body" style="padding-top: 0px;">
                            <div class="table-responsive table mt-2" id="dataTable" role="grid" aria-describedby="dataTable_info">
                                <table class="table my-0" id="dataTable">
                                    <thead>
                                        <tr>
                                            <th>Name</th>
                                            <th>Access</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php
                                            // Get all employees and their access status, and echo them as table rows with checkboxes
                                            $sql = "SELECT employees.id, employees.name, IF(dooraccess.employeeID IS NULL, FALSE, TRUE) as hasAccess FROM employees LEFT JOIN dooraccess ON (employees.id = dooraccess.employeeID)";
                                            $result = $db->query($sql);
                                            if ($result->num_rows > 0) {
                                                while($row = $result->fetch_assoc()) {
                                                    echo "<tr><td>" . $row["name"] . "</td>";
                                                    echo '<td><div class="form-check form-switch"><input class="form-check-input access-checkbox" type="checkbox" name="'.$row['id'].'" ';
                                                    if ($row["hasAccess"] == '1') echo "checked";
                                                    echo "></div></td>";
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
    <script>
        // Send HTTP POST request to server with employeeID and access value
        function updateAccess(employeeID_input,access_input){
            fetch("https://secureuniversal.systems/setAccess.php", {
                method: "POST",
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    employeeID: employeeID_input,
                    access: access_input
                })
            });
        }

        // Handler to all checkboxes
        // Extract access value and employeeID, and send to updateAccess
        checkboxes = document.getElementsByClassName('access-checkbox');
        for (var i=0;i<checkboxes.length;i++) {
            var checkbox = checkboxes[i];
            checkbox.addEventListener('change', (event) => {
                console.log(event.target.name + ", checked:" + event.target.checked);
                employeeID = event.target.name;
                updateAccess(parseInt(employeeID),event.target.checked)
            });
        }
    </script>
</body>
</html>