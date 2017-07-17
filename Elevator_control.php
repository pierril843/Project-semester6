<!DOCTYPE html>
<html>
  <head>
    <!--[if lt IE 9]>
   		<script src=http://html5shiv.googlecode.com/svn/trunk/html5.js></script>
	  <![endif] -->
    <meta http-equiv="author" content="Josh Perry"  />
    <meta http-equiv="pragma" content="no-cache"  />
    <meta name="description" content="Sign up page for software midterm"/>
    <meta name="robots" content="noindex nofollow"/>
    <meta charset="utf-8" />
    <meta name = "viewport" content="width=device-width, initial-scale=1" />
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" />
    <!-- jQuery library -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <!-- Latest compiled JavaScript -->
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>

    <link href="css/Elevator_control.css" type="text/css" rel="stylesheet" />
  </head>
  <body>

  <nav class="navbar navbar-inverse">
  <div class="container-fluid">
    <ul class="nav navbar-nav">
      <li><a href="HomePage.html">J&hearts;R&hearts;R</a></li>
      <li><a href="plan.html">Project Details</a></li>
      <li><a href="media.html">Media</a></li>
      <li><a href="request_access.html">Request Access</a></li>
      <li class="dropdown">
        <a class="dropdown-toggle" data-toggle="dropdown" href="#">Logbooks
        <span class="caret"></span></a>
        <ul class="dropdown-menu">
          <li><a href="Logbook/Joshlogbook.html">Josh's Logbook</a></li>
          <li><a href="Logbook/Rileylogbook.html">Riley's Logbook</a></li>
          <li><a href="Logbook/Ryanlogbook.html">Ryan's Logbook</a></li>
        </ul>
      </li>
    </ul>
    <ul class="nav navbar-nav navbar-right">
       <li><a href="login.html"></span>Login</a></li>
     </ul>
  </div>
 </nav>
 <?php
   // member.php
   session_start();
   // Members only content
   if(isset($_SESSION['username'])){
     echo "<h2>Welcome to the Members Only Page {$_SESSION['username']}! </h2>";

     // Sign out
     echo "<br /><p>Click to <a href='logout.php'>Logout</a></p>";
   }
   else {
     echo "<p>You are not authorized!</p>";
   }
   $db = new PDO(
   'mysql:host=127.0.0.1;dbname=elevatorDB',
   'root',
   ''
   );
  ?>
  <br />
  <br />
  <?php
  // What was clicked
    print_r($_POST);
    if(isset($_SESSION['username'])){
      $authorized = TRUE;
    }
    else{
      $authorized = FALSE;
    }

    if(isset($_POST['floor'])){
      if(($_POST['floor'])=="Floor 3"){
        echo "<br /><p>Requested Floor 3</p>";
        if($authorized == TRUE){
          $query = 'INSERT INTO Logger (CAN_message,CAN_sender,CAN_receiver,Timestamp,Local_Access,Message_type)
                    VALUES(:message,:sender,:receiver,:time,:localaccess,:message_type)';
          $statement = $db->prepare($query);
          $curr_time_query = $db->query('Select NOW()');
          $curr_time = $curr_time_query->fetch(PDO::FETCH_ASSOC);

          $params = [
            //'date' => $curr_date['CURRENT_DATE()'],
            'time' => $curr_time['NOW()'],
            'message' => 'floor 3 requested the elevator',
            'sender' => 'Web',
            'receiver' => 'SV',
            'localaccess' => 0,
            'message_type' => 'R'
          ];
          $result = $statement->execute($params);
        }
        //Write new command to db here (check for authorized)
      }
      else if(($_POST['floor'])=="Floor 2"){
        echo "<br /><p>Requested Floor 2</p>";
        if($authorized == TRUE){
          $query = 'INSERT INTO Logger (CAN_message,CAN_sender,CAN_receiver,Timestamp,Local_Access,Message_type)
                    VALUES(:message,:sender,:receiver,:time,:localaccess,:message_type)';
          $statement = $db->prepare($query);
          $curr_time_query = $db->query('Select NOW()');
          $curr_time = $curr_time_query->fetch(PDO::FETCH_ASSOC);

          $params = [
            //'date' => $curr_date['CURRENT_DATE()'],
            'time' => $curr_time['NOW()'],
            'message' => 'floor 2 requested the elevator',
            'sender' => 'Web',
            'receiver' => 'SV',
            'localaccess' => 0,
            'message_type' => 'R'
          ];
          $result = $statement->execute($params);
        }
        //Write new command to db here (check for authorized)
      }
      else if(($_POST['floor'])=="Floor 1"){
        echo "<br /><p>Requested Floor 1</p>";
        if($authorized == TRUE){
          $query = 'INSERT INTO Logger (CAN_message,CAN_sender,CAN_receiver,Timestamp,Local_Access,Message_type)
                    VALUES(:message,:sender,:receiver,:time,:localaccess,:message_type)';
          $statement = $db->prepare($query);
          $curr_time_query = $db->query('Select NOW()');
          $curr_time = $curr_time_query->fetch(PDO::FETCH_ASSOC);

          $params = [
            //'date' => $curr_date['CURRENT_DATE()'],
            'time' => $curr_time['NOW()'],
            'message' => 'floor 1 requested the elevator',
            'sender' => 'Web',
            'receiver' => 'SV',
            'localaccess' => 0,
            'message_type' => 'R'
          ];
          $result = $statement->execute($params);
        }
        //Write new command to db here (check for authorized)
      }
    }
    else if (isset($_POST['door'])){
      if(($_POST['door'])=="Open"){
        echo "<br /><p>Requested Door open</p>";
        if($authorized == TRUE){
          $query = 'INSERT INTO Logger (CAN_message,CAN_sender,CAN_receiver,Timestamp,Local_Access,Message_type)
                    VALUES(:message,:sender,:receiver,:time,:localaccess,:message_type)';
          $statement = $db->prepare($query);
          $curr_time_query = $db->query('Select NOW()');
          $curr_time = $curr_time_query->fetch(PDO::FETCH_ASSOC);

          $params = [
            //'date' => $curr_date['CURRENT_DATE()'],
            'time' => $curr_time['NOW()'],
            'message' => 'Door open request',
            'sender' => 'Web',
            'receiver' => 'SV',
            'localaccess' => 0,
            'message_type' => 'R'
          ];
          $result = $statement->execute($params);
        }
        //Write new command to db here (check for authorized)
      }
      else if(($_POST['door'])=="Close"){
        echo "<br /><p>Requested Door closed</p>";
        if($authorized == TRUE){
          $query = 'INSERT INTO Logger (CAN_message,CAN_sender,CAN_receiver,Timestamp,Local_Access,Message_type)
                    VALUES(:message,:sender,:receiver,:time,:localaccess,:message_type)';
          $statement = $db->prepare($query);
          $curr_time_query = $db->query('Select NOW()');
          $curr_time = $curr_time_query->fetch(PDO::FETCH_ASSOC);

          $params = [
            //'date' => $curr_date['CURRENT_DATE()'],
            'time' => $curr_time['NOW()'],
            'message' => 'Door close request',
            'sender' => 'Web',
            'receiver' => 'SV',
            'localaccess' => 0,
            'message_type' => 'R'
          ];
          $result = $statement->execute($params);
        }
        //Write new command to db here (check for authorized)
      }
    }
   ?>
  <div class="col-sm-6 col-lg-4 center-block" id="login-block">
  <form action="Elevator_control.php" class="form-horizontal" method="post" id="Control">
    <fieldset class="login-border">
      <legend>Controls</legend>
      </br>
      <div id="article1">
        <div class="button">
          <button name="floor" type="submit" class="btn btn-default" id="submitButton" value="Floor 3">Floor 3</button>
        </div>
        <div class="button">
          <button name="floor" type="submit" class="btn btn-default" id="submitButton" value="Floor 2">Floor 2</button>
        </div>
        <div class="button">
          <button name="floor" type="submit" class="btn btn-default" id="submitButton" value="Floor 1">Floor 1</button>
        </div>
        <div class="button">
          <button name="door" type="submit" class="btn btn-default" id="submitButton" value="Open">Open</button>
          <button name="door" type="submit" class="btn btn-default" id="submitButton" value="Close">close</button>
        </div>
      </div>
    </fieldset>
  </form>

  <script src="js/login.js"></script>
  </div>
  </body>
</html>
