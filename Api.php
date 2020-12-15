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
          WHERE dev_id = $dev";

  $result = mysqli_query($conn, $sql);

  // If the device has already been seen
  if (mysqli_num_rows($result)) {
    $fetch = mysqli_fetch_assoc($result);

    var_dump($fetch);

    // Make sure the device has already been seen from that anchor too
    if (in_array($anc, $fetch['anc_id'])) {
      $sql = "UPDATE anchor_device
      SET RSSI = '$rssi'
      WHERE anc_id='$anc' AND dev_id = '$dev'";
    }
    // If first time from this anchor, insert instead of update
    else {
      $sql = "INSERT INTO anchor_device (anc_id, dev_id, RSSI)
              VALUES ($anc, $dev, $rssi)";
    }
  }
  // Otherwise insert newly seen device in database
  else {
    $sql = "BEGIN;";
    $sql .= "INSERT INTO device (id)
            VALUES ('$dev');
            INSERT INTO anchor_device (anc_id, dev_id, rssi)
            VALUES ( '$anchor', '$dev', '$rssi' );
            COMMIT;";
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
