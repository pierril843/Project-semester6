<html>
  <head><title>Elevator Diagnostic Screen</title></head>
  <body>
    <h1>Elevator Diagnostic</h1>
    <p>Current state of the elevator along with log messages are displayed on this page</p>
    <?php
      // member.php
      session_start();

      // Members only content
      if(isset($_SESSION['username'])){
        echo "<h2>Welcome to the Members Only Page {$_SESSION['username']}! </h2>";
        $db = new PDO(
        'mysql:host=127.0.0.1;dbname=elevatorDB',
        'root',
        ''
        );
        // Sign out
        echo "<p>Click to <a href='login.html'>Logout</a></p>";
      }
      else {
        echo "<p>You are not authorized!</p>";
      }	
    	?>

	<?php      
      $rows = $db->query('SELECT * FROM State ORDER BY Timestamp DESC LIMIT 1');	

	foreach($rows as $row)
	{
	echo "<h2>Current Elevator State</h2>";
	echo "<div>";
	
	if ($row[0] == 0)
	{
		echo "<p id=carStateDisplay>Car is currently at floor $row[1]</p>";
	}
	else
	{
		echo "<p id=carStateDisplay>Car is currently in transit</p>";
	}
	
	if ($row[2] == 1)
	{
		echo "<p id=doorStateDisplay>Door is open</p>";
	}
	else
	{
		echo "<p id=doorStateDisplay>Door is closed</p>";
	}
	
	echo "<p id=timeStampDisplay>Timestamp of message is $row[3]</p>";
	echo "</div>";
	}        
	?>

	<?php
	$loggerRows = $db->query('SELECT * FROM Logger ORDER BY Timestamp');

	echo "<h2>All Elevator Logs</h2>";
	echo "<div>";
	echo "CAN_Message | Can_Sender | Can_Receiver | CAN_raw | Timestamp | local_Access | Message_type |";
	echo "<br />";	
	foreach($loggerRows as $loggerRow)
	{
	for($i=0;$i < sizeof($loggerRow)/2;$i++){
         echo $loggerRow[$i]	 . " | ";
        }
        echo "<br />";
        }
	echo "</div>";
	?>

  </body>
</html>
