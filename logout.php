<?php
  //logout.php
  session_start();
  session_destroy();
  echo "<p>Click to return to the <a href='login.html'>Login</a></p>";
?>
