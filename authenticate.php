<?php
  //authenticate.php
  session_start();
  $username = $_POST['username'];
  $password = $_POST['password'];

  if($username && $password){
    // Connect to schema
    /*
    $db = new PDO(
    'mysql:host=127.0.0.1;dbname=elevatorDB',
    'root',
    ''
    );
    // Query the authorizedUsers table
    $authenticated = FALSE;
    $rows = $db->query('SELECT * FROM autherizedUsers ORDER BY id');
    foreach($rows as $row){
      if($username == $row[1] && $password ==$row[2]){
        $authenticated = TRUE;
      }
    }
    */
    //hardcode login
    if($username == 'Josheeee' && $password == 'password'){
      $authenticated = TRUE;
    }

    if($authenticated == True){
      $_SESSION['username']=$username;
      echo "<p>You are now logged in</p>";
      echo "<p>Please click <a href=\"member.php\" here</a> to be taken to the members page</p>";
    }
    else{
      echo "<p>Not a valid username or password</p>";
      echo "<p>Please check username and password and click <a href=\"login.html\">here</a> to log in again";
    }
  }
  else{
    echo "<p>Please enter a username and password</p>";
  }
