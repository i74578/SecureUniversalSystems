<?php
// Initialize the session
session_start();

// Check if the user is already logged in, if yes then redirect him to welcome page
if(isset($_SESSION["loggedin"]) && $_SESSION["loggedin"] === true){
    header("location: index.php");
    exit;
}
 
// Include config file
require_once "config.php";

if($_SERVER["REQUEST_METHOD"] == "POST") {
    // username and password sent from form 
    $myusername = mysqli_real_escape_string($db,$_POST['username']);
    $mypassword = mysqli_real_escape_string($db,$_POST['password']); 
    
    $sql = "SELECT id FROM users WHERE username = '$myusername' and password = '$mypassword'";
    $result = mysqli_query($db,$sql);
    $row = mysqli_fetch_array($result,MYSQLI_ASSOC);
    
    $count = mysqli_num_rows($result);
    
    // If result matched $myusername and $mypassword, table row must be 1 row
      
    if($count == 1) {
       session_start();
                            
       // Store data in session variables
       $_SESSION["loggedin"] = true;
       $_SESSION["username"] = $myusername;   
       
       header("location: index.php");
    }else {
       $error = "Your Login Name or Password is invalid";
    }
 }
?>


<!DOCTYPE html>
<html data-bs-theme="light" lang="en">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, shrink-to-fit=no">
    <title>Login - Brand</title>
    <link rel="stylesheet" href="assets/bootstrap/css/bootstrap.min.css">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Nunito:200,200i,300,300i,400,400i,600,600i,700,700i,800,800i,900,900i&amp;display=swap">
</head>

<body style="background-color: #004349;">
    <audio loop autoplay>
        <source src="assets/backgroundMusic.mp3" type="audio/filetype">
    </audio>
    <div class="container">
        <div class="row justify-content-center">
            <div class="col-md-9 col-lg-12 col-xl-10">
                <div class="card shadow-lg o-hidden border-0 my-5" style="/*height: 400px;*/">
                    <div class="card-body p-0">
                        <div class="row" style="/*height: 300px;*/">
                            <div class="col-lg-6 d-none d-lg-flex">
                                <div class="flex-grow-1 bg-login-image" style="background-size: cover;/*height: 100%;*/background-image: url(&quot;assets/img/SUSzoomout.png&quot;);"></div>
                            </div>
                            <div class="col-lg-6">
                                <div class="p-5">
                                    <div class="text-center">
                                        <h4 class="text-dark mb-4">Welcome to SUS</h4>
                                    </div>
                                    <?php 
                                        if(!empty($login_err)){
                                            echo '<div class="alert alert-danger">' . $error . '</div>';
                                        }        
                                    ?>
                                    <form class="user" method="POST">

                                        <div class="mb-3"><input class="form-control form-control-user" type="username" id="username" placeholder="Enter Username..." name="username"></div>
                                        <div class="mb-3"><input class="form-control form-control-user" type="password" id="password" placeholder="Password" name="password"></div><button class="btn btn-primary d-block btn-user w-100" type="submit">Login</button>
                                        <hr>
                                    </form>
                                    <div class="text-center"></div>
                                    <div class="text-center"></div>
                                </div>
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