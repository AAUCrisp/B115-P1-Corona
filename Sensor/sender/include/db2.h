#ifndef DB2_H
#define DB2_H

/* NOTE:
 * This is a rework of the prior db library
 * It wil be object oriented for easier use
 */

///////////////
// LIBRARIES //
///////////////
#include "structs.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <WiFi.h>

class DBManager {
  ///////////////
  // VARIABLES //
  ///////////////
  /*  General  */
  unsigned char m_mac[6];
  String m_def_db;
  String m_user;
  String m_pass;

  MySQL_Connection conn;
  IPAddress server_addr;
  WiFiClient client;

  ///////////////
  // FUNCTIONS //
  ///////////////
  bool checkSelf(void);
  bool sendSelf(void);

public:
  DBManager();

  bool connect(void);
  void updateDevice(struct Device dev);
  void insertDevice(struct Device dev);
  bool checkDevice(struct Device dev);

  bool sendDevice(struct Device dev);
};
#endif // __DB2_H_
