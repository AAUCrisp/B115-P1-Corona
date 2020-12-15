import numpy as np
import math as ma
import mysql.connector


def findD(RSSI):
    # Find the difference between transmitted power and received power at unknown distance d
    Pl_d = 20.5 - RSSI

    # Find the difference between transmitter power and received power at known distance d_0 = 1 meter
    Pl_d0 = 84.16667
    d0 = 1

    # Define the Path Loss Exponent n
    n = 1.8

    # Calculate and return distance d isolated from Log Distance Path Loss model
    return ma.exp(((((Pl_d0 - Pl_d) * ma.log(10)) / (10 * n))) * d0)


def findPosition(id, RSSI1, RSSI2, RSSI3):
    # X and Y coordinates of Anchor Points
    x1 = -7.98
    y1 = 3.74
    x2 = 0.56
    y2 = 3.54
    x3 = -3.98
    y3 = -2.5

    # Find the 3 radii
    r1 = findD(RSSI1)
    r2 = findD(RSSI2)
    r3 = findD(RSSI3)

    # Calculate A and b matrices
    A = 2 * np.array([[x3 - x1, y3 - y1], [x3 - x2, y3 - y2]])
    b = np.array([[(r1 ** 2 - r3 ** 2) - (x1 ** 2 - x3 ** 2) - (y1 ** 2 - y3 ** 2)],
                  [(r2 ** 2 - r3 ** 2) - (x2 ** 2 - x3 ** 2) - (y2 ** 2 - y3 ** 2)]])

    # Calculate the matrix multiplication of the inverse of A and b
    res = np.matmul(np.linalg.inv(A), b)
    res1 = res[0]
    res2 = res[1]
    return [id, round(float(res1), 3), round(float(res2), 3)]

def program():
    # Connect to MySQL database. Copied from https://www.w3schools.com/python/python_mysql_getstarted.asp
    mydb = mysql.connector.connect(
        host="192.168.12.1",
        user="back-end",
        password="heidiIsaHoe",
        database="maindb")
    mycursor = mydb.cursor()
    mycursor.execute("SELECT device_id, anc_id , RSSI FROM anchor_device ORDER BY device_id")
    myresult = mycursor.fetchall()

    # Define array for device id and 3 RSSI values
    rssiArray = [0, 0, 0, 0]

    # Create counter
    counter = 0

    for x in myresult:
        dev_id = x[0]
        if dev_id == x[0]:
            anc_id = int(x[1])
            rssiArray[anc_id] = x[2]
            counter += 1
            rssiArray[0] = int(x[0])
            if counter == 3:
                c = findPosition(rssiArray[0], rssiArray[1], rssiArray[2], rssiArray[3])
                val = (c[0], c[1], c[2], c[1], c[2])
                mycursor.execute("INSERT INTO position (id, x, y) VALUES (%s, %s, %s) ON DUPLICATE KEY UPDATE x=%s, y=%s", val)
                mydb.commit()
                rssiArray = [0, 0, 0, 0]
                counter = 0

program()