Hi

<?php 
include_once 'Database.php';
?>

<!DOCTYPE html>
<html>
    <head>
        <title></title>
        <style>
        table {
            border-collapse: collapse;
        }
        
        th, td {
            width:150px;
            border: 3px solid #1090ff;
        }
        </style>
    
    </head>
    <body>

        
        <br>
        <table>
            <thead>
                <tr>
                    <th> anc_id </th> 
                    <th> device_id </th> 
                    <th> RSSI </th> 
                    <th> last_updated </th> 
                </tr>
            </thead>
            <tbody>


        
            
        

        <?php
            $sql = "SELECT * FROM anchor_device;";
            $result = mysqli_query($conn, $sql);
            $resultCheck = mysqli_num_rows($result);

            if ($resultCheck > 0) {
                while ($row = mysqli_fetch_assoc($result)){
                    //echo "<br>  $row[anc_id]  <br>  $row[device_id] <br> $row[RSSI] <br> $row[last_updated]";  
                    //echo " <br> $row[anc_id]  $row[device_id]  $row[RSSI]  $row[last_updated]";  
                    
                    echo "<tr>";
                    echo "<br>";

                        echo"<td> $row[anc_id]</td>";  
                        echo"<td> $row[device_id]</td>";
                        echo"<td> $row[RSSI]</td>";
                        echo"<td> $row[last_updated]</td>";
                    
                    
                    echo "</tr>";
                }

            }
        ?>
            </tbody>
        </table>


    </body>

</html>
