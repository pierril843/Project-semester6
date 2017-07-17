<html>
  <head><title>Members Only Page</title></head>
  <body>
    <h1>Members Only Page</h1>
    <p>Here you will be able to view and edit the content of the elevatorNetwork database</p>
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
        echo "<br /><p>Click to <a href='logout.php'>Logout</a></p>";
        // Sign out
        echo "<br /><p>Click to go to <a href='Elevator_control.php'>Elevator Control Panel</a></p>";
        // Sign out
        echo "<br /><p>Click to go to <a href='Diagnostic.php'>Diagnostic Panel</a></p>";
      }
      else {
        echo "<p>You are not authorized!</p>";
      }
    ?>

    <h2>Input NEW data to the database using the form below</h2>
    <form action="member.php" method="POST">
      status: <input type="text" name="status" /><br />
      currentFloor: <input type="text" name="currentFloor" /><br />
      requestedFloor: <input type="text" name ="requestedFloor" /><br />
      <input type="submit" value="Add to database" />
    </form>
    <?php
      //Insert into db

      if(isset($_SESSION['username']) && ((isset($_POST['nodeID']) == FALSE) && (isset($_POST['status'])) && (isset($_POST['currentFloor'])) && (isset($_POST['requestedFloor'])))) {
        $query = 'INSERT INTO elevatorStates (date,time,status,currentFloor,requestedFloor)
                  VALUES(:date,:time,:status,:currentFloor,:requestedFloor)';
        $statement = $db->prepare($query);
        $curr_date_query = $db->query('Select CURRENT_DATE()');
        $curr_date = $curr_date_query->fetch(PDO::FETCH_ASSOC);
        $curr_time_query = $db->query('SELECT CURRENT_TIME()');
        $curr_time = $curr_time_query->fetch(PDO::FETCH_ASSOC);
        $status = $_POST['status'];
        $currentFloor = $_POST['currentFloor'];
        $requestedFloor = $_POST['requestedFloor'];

        $params = [
          'date' => $curr_date['CURRENT_DATE()'],
          'time' => $curr_time['CURRENT_TIME()'],
          'status' => $status,
          'currentFloor' => $currentFloor,
          'requestedFloor' => $requestedFloor,
        ];
        $result = $statement->execute($params);
      }

      $rows = $db->query('SELECT * FROM elevatorStates ORDER BY nodeID');
    ?>

    <h2>Update data in the database using the form below</h2>
    <form action="member.php" method="POST">
      nodeID: <input type="text" name="nodeID" /><br />
      status: <input type="text" name="status" /><br />
      currentFloor: <input type="text" name="currentFloor" /><br />
      requestedFloor: <input type="text" name ="requestedFloor" /><br />
      <input type="submit" value="Update database" />
    </form>
    <?php
      /*function update_elevatorStates(int $nodeID, int $status, int $currentFloor, int $requestedFloor): void {
        $db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

        $query = 'UPDATE elevatorStates
                  SET status = :stat
                  currentFloor = :curr
                  requestedFloor = :req
                  WHERE nodeID = :id';
        $statement = $db->prepare($query);
        $statement->bindValue('stat',$status);
        $statement->bindValue('curr',$currentFloor);
        $statement->bindValue('req',$requestedFloor);
        $statement->bindValue('id',$nodeID);
        $statement->execute();
      }
      */

      if(isset($_SESSION['username']) && (isset($_POST['nodeID']) && (isset($_POST['status'])) && (isset($_POST['currentFloor'])) && (isset($_POST['requestedFloor'])))){
      $nodeID = $_POST['nodeID'];
      $status = $_POST['status'];
      $currentFloor = $_POST['currentFloor'];
      $requestedFloor = $_POST['requestedFloor'];
      //update_elevatorStates($nodeID, $status, $currentFloor, $requestedFloor);
      //$db->setAttribute(PDO::ATTR_DEFAULT_FETCH_MODE, PDO::FETCH_ASSOC);

      $query = 'UPDATE elevatorStates
                SET status = :stat,
                currentFloor = :curr,
                requestedFloor = :req
                WHERE nodeID = :id';
      $statement = $db->prepare($query);
      $statement->bindValue('stat',$status);
      $statement->bindValue('curr',$currentFloor);
      $statement->bindValue('req',$requestedFloor);
      $statement->bindValue('id',$nodeID);
      $statement->execute();
      echo "<p>Finished Executing</p>";
      }
    ?>
    <?php
      echo "<h3>Entire content of the elevatorStates table</h3>";
      $rows = $db->query('SELECT * FROM elevatorStates ORDER BY nodeID');
      echo "   DATE    |   TIME   | ID | Status | Current Floor | Requested Floor | ";
      echo "<br />";
      foreach($rows as $row){
        for($i=0;$i < sizeof($row)/2;$i++){
         echo $row[$i] . " | ";
        }
        echo "<br />";
      }
    ?>
  </body>
</html>
