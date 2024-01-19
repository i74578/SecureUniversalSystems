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
                    <li class="nav-item"><a class="nav-link active" href="employees.php" active><i class="far fa-address-card"></i><span>Employees</span></a></li>
                    <li class="nav-item"><a class="nav-link " href="ac.php"><i class="far fa-address-card"></i><span>Access Control</span></a></li>
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
                            <p class="fs-5 text-primary m-0 fw-bold">Employees</p>
                        </div>
                        <div class="card-body" style="padding-top: 0px;">
                            <div class="table-responsive table mt-2" id="dataTable" role="grid" aria-describedby="dataTable_info">
                                <table class="table my-0" id="dataTable">
                                    <thead>
                                        <tr>
                                            <th>Name</th>
                                            <th>PIN</th>
                                            <th>NUID</th>
                                            <th>Actions</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php
                                            // Execute SQL query to get employees and their attributes, and then echo it as a table row
                                            $sql = "SELECT employees.id, employees.name, employees.nuid,employees.pin FROM employees";
                                            $result = $db->query($sql);
                                            if ($result->num_rows > 0) {
                                                while($row = $result->fetch_assoc()) {
                                                    echo "<tr>";
                                                    echo '<td>';
                                                    echo '<input type="hidden" class="employeeIDField" value="'.$row['id'].'">';
                                                    echo '<input value="'.$row['name'].'"type="text" class="form-control employeeNameField" aria-label="PIN" aria-describedby="basic-addon2">';
                                                    echo '</td>';
                                                    echo '<td>';
                                                    echo '<input name="employeePin" value="'.$row['pin'].'"type="text" class="form-control employeePinField" aria-label="PIN" aria-describedby="basic-addon2">';
                                                    echo '</td>';
                                                    echo '<td>';
                                                    echo '<input name="employeeNuid" value="'.$row['nuid'].'"type="text" class="form-control employeeNuidField" aria-label="NUID" aria-describedby="basic-addon2">';
                                                    echo '</td>';
                                                    echo '<td>';
                                                    echo '<button class="btn btn-danger employee-remove" type="button"><i class="far fa-trash-alt"></i></button>';
                                                    echo '<button class="btn btn-success employee-update" type="button" style="margin-left: 10px;"><i class="far fa-save"></i></button>';
                                                    echo "</td>";
                                                    echo "</tr>";
                                                }
                                            }
                                        ?>
                                    </tbody>
                                </table>
                            </div>
                            <button class="btn btn-success" type="button" onclick="addEmployee()">Add employee</button>
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
        
        //Send POST request to add employee
        async function addEmployee(){
            await fetch("https://secureuniversal.systems/setEmployee.php", {
                method: "POST",
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    action: "newEmployee"
                })
            });
            await location.reload();
        }

        //Send POST request to remove employee
        async function removeEmployee(id){
            await fetch("https://secureuniversal.systems/setEmployee.php", {
                method: "POST",
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    action: "removeEmployee",
                    id: id
                })
            });
            await location.reload();
        }

        //Send POST request to update employee
        async function updateEmployee(id,name,pin,nuid){
            await fetch("https://secureuniversal.systems/setEmployee.php", {
                method: "POST",
                headers: {
                    'Accept': 'application/json',
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    action: "updateEmployee",
                    id: id,
                    name: name,
                    pin: pin,
                    nuid: nuid
                })
            });
            await location.reload();
        }

        //Handler for when remove entry button is clicked
        //Extract relevant data from row and send it to removeEmployee()
        removeEmployeeBtns = document.getElementsByClassName('employee-remove');
        for (var i=0;i<removeEmployeeBtns.length;i++) {
            var btn = removeEmployeeBtns[i];
            btn.addEventListener('click', (event) => {
                var employeeRow = event.currentTarget.parentElement.parentElement;
                var id = employeeRow.getElementsByClassName("employeeIDField")[0].value;
                removeEmployee(id);
            });
        }

        //Handler for when remove entry button is clicked
        //Extract relevant data from row and send it to updateEmployee()
        updateEmployeeBtns = document.getElementsByClassName('employee-update');
        for (var i=0;i<updateEmployeeBtns.length;i++) {
            var btn = updateEmployeeBtns[i];
            btn.addEventListener('click', (event) => {
                var employeeRow = event.currentTarget.parentElement.parentElement;
                var id = employeeRow.getElementsByClassName("employeeIDField")[0].value;
                var name = employeeRow.getElementsByClassName("employeeNameField")[0].value;
                var pin = employeeRow.getElementsByClassName("employeePinField")[0].value;
                var nuid = employeeRow.getElementsByClassName("employeeNuidField")[0].value;
                updateEmployee(id,name,pin,nuid);
            });
        }
    </script>
</body>
</html>