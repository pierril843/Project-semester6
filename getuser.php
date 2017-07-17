<!DOCTYPE html>
<html>
<head>
<style>
table {
    width: 100%;
    border-collapse: collapse;
}

table, td, th {
    border: 1px solid black;
    padding: 5px;
}

th {text-align: left;}
</style>
</head>
<body>

<?php
	//session_start();

	$q = intval($_POST['q']);

	//if(isset($_SESSION['username'])){
        //echo "<h2>Welcome to the Members Only Page {$_SESSION['username']}! </h2>";
        $db = new PDO(
        'mysql:host=127.0.0.1;dbname=elevatorDB',
        'root',
        ''
        );
        // Sign out
        echo "<p>Click to <a href='logout.php'>Logout</a></p>";
      //}
	//else {
        //echo "<p>You are not authorized!</p>";
      //}

//mysqli_select_db($con,"ajax_demo");
$loggerRows = $db->query('SELECT * FROM Logger ORDER BY Timestamp');
//$result = mysqli_query($con,$sql);

echo "<table>
<tr>
<th>Can Message</th>
<th>Can Sender</th>
<th>Can Receiver</th>
<th>Can Raw</th>
<th>Timestamp</th>
<th>Local Access Flag</th>
<th>Message Type</th>
</tr>";
foreach($loggerRows as $loggerRow) {
    echo "<tr>";
    echo "<td>" . $loggerRow[0] . "</td>";
    echo "<td>" . $loggerRow[1] . "</td>";
    echo "<td>" . $loggerRow[2] . "</td>";
    echo "<td>" . $loggerRow[3] . "</td>";
    echo "<td>" . $loggerRow[4] . "</td>";
    echo "<td>" . $loggerRow[4] . "</td>";
    echo "<td>" . $loggerRow[4] . "</td>";
    echo "</tr>";
}
echo "</table>";
//mysqli_close($con);
?>
</body>
</html>
