#ifndef DB_H
#define DB_H
// Includes
#include "structs.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFi.h>

// Prototypes
/* void functions */
void dbInit(struct DevicePacket *packet);
void dbSendDevices(struct DevicePacket *packet);
void dbSendSelf(struct DevicePacket *packet);

/* bool functions */
bool dbCheckSelf();
bool dbCheckDevice(struct Device *device);

// Variables
/* SQL related */
IPAddress server_addr(192, 168, 12, 1); /* Server ip */
char user[] = "sensor";                 /* Database user */
char password[] = "KageDagErG0dt";      /* Database pass */
char def_db[] = "maindb";
WiFiClient client;                        /* WiFi Client */
MySQL_Connection conn((Client *)&client); /* Database connector */

// Code

/*
 * Info: Function for checking if sensor already has connected to database
 * Return:
 *    true, if sensor is in database
 *    false, if sensor is not in database
 */
bool dbCheckSelf(struct DevicePacket *packet) {
  /* MySQL Cursor */
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  /* SQL row */
  row_values *row = NULL;

  /* Prepared SQL statement */
  const char SQL_STATE[] =
      "SELECT * FROM anchor WHERE id = '%X:%X:%X:%X:%X:%X'";
  char query[128];

  sprintf(query, SQL_STATE,
          /* Sender module MAC Addr */
          packet->sens_mac[0], packet->sens_mac[1], packet->sens_mac[2],
          packet->sens_mac[3], packet->sens_mac[4], packet->sens_mac[5]);

  /* Execution of SQL statement */
  cur_mem->execute(query);

  /* NOT USED, BUT REQUIRED */
  column_names *cols = cur_mem->get_columns();

  /* Actual check */
  row = cur_mem->get_next_row();
  if (row != NULL) {
    delete cur_mem;
    return true;
  } else {
    delete cur_mem;
    return false;
  }
}

/*
 * Info: Function for checking if device already has been seen by sensors
 * Params: struct Device *device
 * Return:
 *    true, if device is in database
 *    false, if device is not in database
 */
bool dbCheckDevice(struct Device *device) {
  /* MySQL Cursor */
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  /* SQL row */
  row_values *row = NULL;

  /* Prepared SQL statement */
  const char SQL_STATE[] =
      "SELECT * FROM device WHERE id = '%X:%X:%X:%X:%X:%X'";
  char query[128];

  sprintf(query, SQL_STATE,
          /* Device MAC Addr */
          device->dev_mac[0], device->dev_mac[1], device->dev_mac[2],
          device->dev_mac[3], device->dev_mac[4], device->dev_mac[5]);

  /* Execution of SQL statement */
  cur_mem->execute(query);

  /* NOT USED, BUT REQUIRED */
  column_names *cols = cur_mem->get_columns();

  /* Actual check */
  row = cur_mem->get_next_row();
  if (row != NULL) {
    delete cur_mem;
    return true;
  } else {
    delete cur_mem;
    return false;
  }
}

/*
 * Info: Function for inserting devices into database
 * Params: struct DevicePacket *packet
 */
void dbSendDevices(struct DevicePacket *packet) {
  if (conn.connect(server_addr, 3306, user, password, def_db)) {
    /* MySQL Cursor */
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

    /* SQL statements */
    const char SQL_INSERT[] =
        "INSERT INTO anchor_device (RSSI, anc_id, device_id) VALUES ('%i', "
        "'%X:%X:%X:%X:%X:%X', '%X:%X:%X:%X:%X:%X')";
    const char SQL_UPDATE[] =
        "UPDATE anchor_device SET RSSI = %i WHERE anc_id = "
        "'%X:%X:%X:%X:%X:%X' AND "
        "device_id = '%X:%X:%X:%X:%X:%X'";
    char query[128];

    for (int i = 0; i < MAX_DEVICES; i++) {
      if (dbCheckDevice(&packet->devices[i])) {
        sprintf(query, SQL_UPDATE, packet->devices[i].rssi,
                /* Sensor MAC */
                packet->sens_mac[0], packet->sens_mac[1], packet->sens_mac[2],
                packet->sens_mac[3], packet->sens_mac[4], packet->sens_mac[5],
                /* Device MAC */
                packet->devices[i].dev_mac[0], packet->devices[i].dev_mac[1],
                packet->devices[i].dev_mac[2], packet->devices[i].dev_mac[3],
                packet->devices[i].dev_mac[4], packet->devices[i].dev_mac[5]);

        cur_mem->execute(query);
      } else {
        sprintf(query, SQL_INSERT, packet->devices[i].rssi,
                /* Sensor MAC */
                packet->sens_mac[0], packet->sens_mac[1], packet->sens_mac[2],
                packet->sens_mac[3], packet->sens_mac[4], packet->sens_mac[5],
                /* Device MAC */
                packet->devices[i].dev_mac[0], packet->devices[i].dev_mac[1],
                packet->devices[i].dev_mac[2], packet->devices[i].dev_mac[3],
                packet->devices[i].dev_mac[4], packet->devices[i].dev_mac[5]);

        cur_mem->execute(query);
      }
    }
    delete cur_mem;
  }
}

/*
 * Info: Function for inserting sensor into database
 * Params: struct DevicePacket *packet
 */
void dbSendSelf(struct DevicePacket *packet) {
  /* MySQL Cursor */
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  /* SQL statement */
  const char SQL_STATE[] =
      "INSERT INTO anchor (id) VALUES ('%X:%X:%X:%X:%X:%X')";
  char query[128];

  sprintf(query, SQL_STATE,
          /* Sensor MAC */
          packet->sens_mac[0], packet->sens_mac[1], packet->sens_mac[2],
          packet->sens_mac[3], packet->sens_mac[4], packet->sens_mac[5]);

  cur_mem->execute(query);
  delete cur_mem;
}

/*
 * Info: Function for initializing connection to database
 * Params: struct DevicePacket *packet
 */
void dbInit(struct DevicePacket *packet) {
  if (conn.connect(server_addr, 3306, user, password, def_db)) {
    delay(1000);
    if (!dbCheckSelf(packet)) {
      Serial.println("Not in db... Adding self");
      dbSendSelf(packet);
    } else {
      Serial.println("Already in db");
    }
  }
}

#endif
