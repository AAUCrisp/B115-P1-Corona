#include "db2.h"
#include "structs.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFi.h>

// CONSTRUCTOR
DBManager::DBManager() : conn((Client *)&client), server_addr(192, 168, 1, 50) {
  m_user = "sensor";
  m_pass = "KageDagErG0dt";
  m_def_db = "maindb";
  WiFi.macAddress(m_mac);
}

// MEMBER FUNCTIONS
bool DBManager::connect(void) {
  /* Conversion from String to char* */
  char user[m_user.length() + 1];
  char pass[m_pass.length() + 1];
  char def_db[m_def_db.length() + 1];

  strcpy(user, m_user.c_str());
  strcpy(pass, m_pass.c_str());
  strcpy(def_db, m_def_db.c_str());

  if (conn.connect(server_addr, 3306, user, pass, def_db)) {
    delay(1000);
    if (!checkSelf()) {
      Serial.println("Not in db... Adding self");
      sendSelf();
    } else {
      Serial.println("Already in db");
    }
  } else {
    Serial.println("Connection to db failed");
  }
}

bool DBManager::checkDevice(struct Device dev) {
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  row_values *row = NULL;

  /* Prepared SQL statement */

  const char SQL_STATE[] =
      "SELECT * FROM device WHERE id = '%X:%X:%X:%X:%X:%X'";
  char query[128];

  sprintf(query, SQL_STATE,
          /* Device MAC Addr */
          dev.dev_mac[0], dev.dev_mac[1], dev.dev_mac[2], dev.dev_mac[3],
          dev.dev_mac[4], dev.dev_mac[5]);
  Serial.println(query);
  /* Execution of SQL statement */
  cur_mem->execute(query);

  /* NOT USED, BUT REQUIRED */
  column_names *cols = cur_mem->get_columns();

  /* Actual check */
  row = cur_mem->get_next_row();
  if (row != NULL) {
    delete cur_mem;
    Serial.println("Found device");
    return true;
  } else {
    delete cur_mem;
    Serial.println("Device not found");
    return false;
  }
}

void DBManager::insertDevice(struct Device dev) {
  /* MySQL Cursor */
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  /* SQL statement */
  const char SQL_STATE_dev[] =
      "INSERT INTO device (id) VALUES ('%X:%X:%X:%X:%X:%X')";
  const char SQL_STATE_anc_dev[] =
      "INSERT INTO anchor_device (RSSI, anc_id, device_id) VALUES "
      "('%i','%X:%X:%X:%X:%X:%X','%X:%X:%X:%X:%X:%X') ON DUPLICATE KEY UPDATE "
      "RSSI=VALUES (RSSI)";
  char query_dev[128];
  char query_anc_dev[128];

  sprintf(query_dev, SQL_STATE_dev,
          /* Dev MAC */
          dev.dev_mac[0], dev.dev_mac[1], dev.dev_mac[2], dev.dev_mac[3],
          dev.dev_mac[4], dev.dev_mac[5]);
  sprintf(query_anc_dev, SQL_STATE_anc_dev, dev.rssi,
          /* Sensor MAC */
          m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5],
          /* Dev MAC */
          dev.dev_mac[0], dev.dev_mac[1], dev.dev_mac[2], dev.dev_mac[3],
          dev.dev_mac[4], dev.dev_mac[5], dev.rssi);
  cur_mem->execute(query_dev);
  delay(200);
  cur_mem->execute(query_anc_dev);

  delete cur_mem;
}

void DBManager::updateDevice(struct Device dev) {
  /* Cursor */
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  /* SQL statement */
  const char SQL_UPDATE[] = "UPDATE anchor_device SET RSSI = %i WHERE anc_id = "
                            "'%X:%X:%X:%X:%X:%X' AND "
                            "device_id = '%X:%X:%X:%X:%X:%X'";
  char query[128];

  sprintf(query, SQL_UPDATE, dev.rssi,
          /* Sensor MAC */
          m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5],
          /* Device MAC */
          dev.dev_mac[0], dev.dev_mac[1], dev.dev_mac[2], dev.dev_mac[3],
          dev.dev_mac[4], dev.dev_mac[5]);

  Serial.println(query);
  cur_mem->execute(query);

  // /* NOT USED, BUT REQUIRED */
  // column_names *cols = cur_mem->get_columns();

  delete cur_mem;
}

bool DBManager::sendDevice(struct Device dev) {
  if (checkDevice(dev)) {
    Serial.println("Updating found device");
    updateDevice(dev);
  } else {
    Serial.println("Inserting found device");
    insertDevice(dev);
  }
}

bool DBManager::checkSelf(void) {
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
          m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);

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
bool DBManager::sendSelf(void) {
  /* MySQL Cursor */
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);

  /* SQL statement */
  const char SQL_STATE[] =
      "INSERT INTO anchor (id) VALUES ('%X:%X:%X:%X:%X:%X')";
  char query[128];

  sprintf(query, SQL_STATE,
          /* Sensor MAC */
          m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);

  cur_mem->execute(query);
  if (cur_mem->execute(query)) {
    delete cur_mem;
    return true;
  } else {
    delete cur_mem;
    return false;
  }
}
