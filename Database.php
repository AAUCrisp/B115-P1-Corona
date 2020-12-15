<?php
$servername = "192.168.12.1";
$username = "marge";
$userpassword = "overrask_mig";
$dbname = "maindb";

$conn = new mysqli($servername, $username, $userpassword,$dbname);



if ($conn -> connect_error) {
	die("Connection failed" . $conn -> connect_error);
}
//echo "connected successfully";







/*
if ($result->num_rows > 0) {
  // output data of each row
  while($row = $result->fetch_assoc())
  	{
	  echo "got it";
	}
	else
	{
	  echo "0 results";
	}
}
$conn->close();

*/
?>
