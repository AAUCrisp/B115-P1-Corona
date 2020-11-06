import numpy as np
import math as ma
import mysql.connector

def findD(RSSI):
    Pl_d = 20.5 - RSSI
    Pl_d0 = 84.16667
    n = 1.8
    d0 = 1

    return ma.exp(((((Pl_d0 - Pl_d)*ma.log(10))/(10*n)))*d0)

def findPosition(RSSI1, RSSI2, RSSI3):
    x1 = -7.98
    x2 =  0.56
    x3 = -3.98

    y1 = 3.74
    y2 = 3.54
    y3 = -2.5

    r1 = findD(RSSI1)
    r2 = findD(RSSI2)
    r3 = findD(RSSI3)

    A = 2*np.array([[x3-x1, y3-y1], [x3-x2, y3-y2]])
    b = np.array([[(r1**2 - r3**2)-(x1**2-x3**2)-(y1**2-y3**2)],[(r2**2-r3**2)-(x2**2-x3**2)-(y2**2-y3**2)]])
    return np.matmul(np.linalg.inv(A),b)

mydb = mysql.connector.connect(
  host="192.168.12.1",
  user="back-end",
  password="heidiIsaHoe",
  database="maindb"
)
mycursor = mydb.cursor()
mycursor.execute("SELECT device_id, RSSI FROM anchor_device")
myresult = mycursor.fetchall()

for device_id in myresult:

    for RSSI in device_id
    findPosition()



