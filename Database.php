<?php
$servername = "192.168.12.1";
$username = "back-end";
$userpassword = "heidiIsaHoe";
$dbname = "maindb";

$conn = new mysqli($servername, $username, $userpassword,$dbname);



if ($conn -> connect_error) {
	die("Connection failed" . $conn -> connect_error);
}
echo "Connected to Database successfully";



?>
