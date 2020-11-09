import numpy as np
import math as ma
import mysql.connector
import time

def findD(RSSI):
    Pl_d = 20.5 - RSSI
    Pl_d0 = 84.16667
    n = 1.8
    d0 = 1

    return ma.exp(((((Pl_d0 - Pl_d)*ma.log(10))/(10*n)))*d0)

def findPosition(id, RSSI1, RSSI2, RSSI3):
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
    res = np.matmul(np.linalg.inv(A),b)
    res1 = res[0]
    res2 = res[1]
    return [id,round(float(res1),3), round(float(res2),3)]

#Kopieret fra https://www.w3schools.com/python/python_mysql_getstarted.asp
def program():
    mydb = mysql.connector.connect(
        host="192.168.12.1",
        user="back-end",
        password="heidiIsaHoe",
        database="maindb")
    mycursor = mydb.cursor()
    mycursor.execute("SELECT device_id, anc_id , RSSI FROM anchor_device ORDER BY device_id")
    myresult = mycursor.fetchall()

    xsave = [0 ,0 ,0, 0]
    z = 0

    for x in myresult:
        y = x[0]
        if (y == x[0]):
            u = int(x[1])
            xsave[u] = x[2]
            z +=1
            xsave[0] = int(x[0])
            if (z==3):
                c = findPosition(xsave[0], xsave[1], xsave[2], xsave[3])
                val = (c[0],c[1],c[2], c[1], c[2])
                mycursor.execute("INSERT INTO position (id, x, y) VALUES (%s, %s, %s) ON DUPLICATE KEY UPDATE x=%s, y=%s", val)
                mydb.commit()
                z=0
                xsave = [0,0,0, 0]

    
program()


