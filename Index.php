<!-- Linje 4-6: Her forbinder inkludere jeg en anden fil, hvor der oprettes en forbindelse til Databasen
når man skal skrive i PHP, så skal man lave PHP tags  -->

<?php
include_once 'Database.php';
?>

<!-- Herunder står koden for det der skal stå på front-enden. for at koden skal kunne skrives på et website
skal det være inde i et HTML tag: <html> alt der står herinde er kode til website </html> -->


<!DOCTYPE html>
<html>
    <head>

       <!--  inde i "script tagget står koden der automatisk genindlæser siden, således at ny data
        som tilføjes til databasen kommer op på websitet. det der står i script tagget er javascript
        da html blot er et "skrive-redskab", kan det ikke anvendes til at skrive kode, som funktioner
        variabler, statements. alt dette skal gøres i PHP eller Javascript. script tag anvendes når
        man vil skrive i javascript-->
        <script>
        function timedRefresh(timeoutPeriod) {
            setTimeout("location.reload(true);", timeoutPeriod);
        }

        window.onload = timedRefresh(5000);
        </script>


        <title></title>
        <!-- herunder bliver der indenfor <style> tagget skrevet i CSS. Disse få linjer bruges på at ændre
        designet af tabellerne hvordan tabellerne skal se ud, og hvilken farve skal de have. -->
        <style>
        table {
            border-collapse: collapse; /* design af tabellens kant */
        }

        th, td {
            width:150px;
            border: 3px solid #1090ff;    /* tabellens farve */
        }

        /* h1    {

            border: 5px solid purple;
            width: 10%;

        } */

        </style>
    </head>
    <!-- en hjemme side af delt op i blokke. Lige PT kender jeg kun til Head blokken, og body blokken.
    her ender alt det som skal stå i head blokken -->

    <body>
         <!--    Herunder bliver der hentet Data fra databasen. For at kunne interagere med databasen
            skal man skrive SQL strenge. men for at kunne gøre det i HTML, så skal SQL-koden være inde i
            PHP tags.

            select distinct = vælg kolonner, og sørg for ikke at værdierne i kolonnerne ikke går igen.
            from og join = her udvælges to tabeller, hvor 1 kolonne fra hver tabel slås sammen med den anden.
            on = angiver hvilke kolonner som skal slås sammen.  -->

            <?php
                $sql = "SELECT DISTINCT device.id AS hamster, device.Last_updated , position.x, position.y
                        FROM device
                        JOIN anchor_device
                        ON anchor_device.device_id = device.id
                        JOIN position
                        ON anchor_device.device_id = position.id";
                $result = mysqli_query($conn, $sql);        /* hentes data fra databasen, */
                $resultCheck = mysqli_num_rows($result);    /* oprettes en variable, som er ligmed antal af rækker */

                /* herunder opsættes et if statement. antallet af rækker = antal af enheder. hvis der er mindre end 20
                skal boxen på hjemmesiden være grøn. hvis den ikke er under 20, bliver den rød.
                Burde nok trække det her tal ud fra databasen, fra en tabel som fortæller hvad rummet må rumme */
            ?>
            <?php
                if ($resultCheck < 20) {
                    $color = "green";
                } else {
                    $color = "red";
                }

            ?>

        <!-- <h1> laver en overskrift, hvor "style" definere farven for boxen, at den skal have værdien color, som er afhængig af antal enheder -->
            <h1 style = "border: 10px solid <?php echo "$color"?>">

            <?php
                echo $resultCheck;         //print antal enheder
            ?>

            </h1>


        <br>
        <!-- Oprettelse af Tabel i HTML, og derefter indsættelse af tabel-headeren så man kan se hvad dataen er til -->
       <table>
            <thead>
                <tr>
                    <th> ID </th>
                  <th> Last Seen </th>
                    <th> X-Coordinate </th>
                    <th> Y-Coordinate </th>
                </tr>
            </thead>
            <tbody>

                <?php
                    /* her hentes værdierne fra databasen og de bliver printet ud på hjemmesiden. */
                    if ($resultCheck > 0) {
                        while ($row = mysqli_fetch_assoc($result)){
                            echo "<tr>";
                            echo "<br>";
                            echo"<td> $row[device.id]</td>";
                            echo"<td> $row[Last_updated]</td>";
                            echo"<td> $row[x]</td>";
                            echo"<td> $row[y]</td>";
                            echo "</tr>";
                        }
                    } else {
                        echo "no rows";
                    }
                ?>
            </tbody>
        </table>
    </body>
</html>
