<?php  // Require the file, that makes the connection to the database
require_once 'database.php';

echo "FED PIK";

// Check if the GET variables are set in the URL, and make normal variables with them.
if (isset($_GET)) $dev = strtoupper($_GET["dev_mac"]);
if (isset($_GET)) $rssi = $_GET["rssi"];
if (isset($_GET)) $anc = strtoupper($_GET["anchor"]);

if (isset($dev) && isset($rssi) && isset($anc)) {
  // Check if the device has already been seen
  $sql = "SELECT *
          FROM anchor_device
          WHERE device_id = '$dev'";
  var_dump($sql);

  $result = mysqli_query($conn, $sql);

  var_dump($result);
  $fetch = mysqli_fetch_assoc($result);
  echo "Før DB Kald";
  var_dump($fetch);
  echo "Efter DB Kald";

  // If the device has already been seen
  if (mysqli_num_rows($result)) {
    echo "<br> FUNDET TING! <br>";

    // Make sure the device has already been seen from that anchor too
    if (in_array($anc, $fetch)) {
      echo "<br> OPDATER ANKER! <br>";
      $sql = "UPDATE anchor_device
      SET RSSI = '$rssi'
      WHERE anc_id='$anc' AND device_id = '$dev'";
    }
    // If first time from this anchor, insert instead of update
    else {
      echo "<br> NYT ANKER! <br>";
      $sql = "INSERT INTO anchor_device (anc_id, device_id, RSSI)
              VALUES ('$anc', '$dev', $rssi)";
    }
  }
  // Otherwise insert newly seen device in database
  else {

    echo "<br> NY ENHED! <br>";
    $sql = "INSERT INTO device (id)
            VALUES ('$dev')";

    var_dump($sql);
    // Insert whatever data into the database, that the conditions figured out
    if ($conn->query($sql) === TRUE) {
      echo "Database updated successfully";
    } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
    }

    $sql = "INSERT INTO anchor_device (anc_id, device_id, RSSI)
            VALUES ( '$anc', '$dev', $rssi)";
  }
  var_dump($sql);

  // Insert whatever data into the database, that the conditions figured out
  if ($conn->query($sql) === TRUE) {
    echo "Database updated successfully";
  } else {
    echo "Error: " . $sql . "<br>" . $conn->error;
  }
  // Done, close connection
  $conn->close();
}

 ?>
