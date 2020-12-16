<?php  // ----- SETUP SECTION -----
// Require the file, that makes the connection to the database
require_once 'database.php';
// Include my php_helper class, to look for values in multidimensional array
require_once 'php_helper.php';

// Check if the GET variables are set in the URL --> make normal variables with them.
if (isset($_GET)) $dev = strtoupper($_GET["dev_mac"]);
if (isset($_GET)) $rssi = $_GET["rssi"];
if (isset($_GET)) $anc = strtoupper($_GET["anchor"]);

// ----- FUNCTION SECTION -----
// -- Update Database Function --
function db_update($stmt) {
  echo "<br>SKRIV TIL DATABASE! <br>";
  // Insert whatever data into the database, that the conditions figured out
  if ($conn->query($stme) === TRUE) {
    echo "Database updated successfully";
  } else {
    echo "Error: " . $stmt . "<br>" . $conn->error;
  }
}

// ----- MAIN API SECTION -----
// If the GET variables has been set, start the update
if (isset($dev) && isset($rssi) && isset($anc)) {
  // First check if the Anchor has been seen before
  $sql = "SELECT *
          FROM anchor
          WHERE id = '$anc'";

  // Start Request
  $result = mysqli_query($conn, $sql);
  echo "<br> Anchor check... <br>";
  var_dump($result);

  // If the Anchor hasn't already been seen
  if (mysqli_num_rows($result) < 1) {
    echo "<br>Anchor ikke set før <br>";
    // Insert the anchor into the Database
    $sql = "INSERT INTO anchor (id)
            VALUES ('$anc')";
    echo "<br> $sql <br>";
    // Insert Anchor
    db_update($sql);
  }


  // Check if the device has already been seen
  $sql = "SELECT *
          FROM anchor_device
          WHERE device_id = '$dev'";
  echo "<br> $sql <br>";

  // Start Request
  $result = mysqli_query($conn, $sql);

  var_dump($result);

  // If the device has already been seen
  if (mysqli_num_rows($result)) {
    $fetch = mysqli_fetch_all($result);
    var_dump($fetch);
    echo "<br> ENHED SET FØR! <br>";

    var_dump(php_helper::in_multi_array($anc, $fetch));
    // Make sure the device has already been seen from that anchor too
    if (php_helper::in_multi_array($anc, $fetch)) {
      echo "<br> OPDATER ANKER! <br>";
      $sql = "UPDATE anchor_device
      SET RSSI = '$rssi'
      WHERE anc_id='$anc' AND device_id = '$dev'";
    }
    // If first time from this anchor, insert instead of update
    else {
      echo "<br> NYT ANKER PÅ KENDT ENHED! <br>";
      $sql = "INSERT INTO anchor_device (anc_id, device_id, RSSI)
              VALUES ('$anc', '$dev', $rssi)";
    }
  }
  // Otherwise insert newly seen device in database     -- NEW DEVICE --
  else {
    echo "<br> NY ENHED! <br>";
    $sql = "INSERT INTO device (id)
            VALUES ('$dev')";

    echo "<br> $sql <br>";
    // Insert device
    db_update($sql);

    $sql = "INSERT INTO anchor_device (anc_id, device_id, RSSI)
            VALUES ( '$anc', '$dev', $rssi)";
  }
  echo "<br> $sql <br>";
  // Insert/Update Device
  db_update($sql);
  // Done, close connection
  $conn->close();
}

 ?>
