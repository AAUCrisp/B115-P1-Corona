#include "db2.h"
#include "parser.hpp"
#include "structs.h"
#include <Arduino.h>
#include <WiFi.h>

/* Connection stuff */
// const char *ssid = "B115-Test-Net";
// const char *pass = "hygeRMitYWCa";

/* HOME TEST CONNECTION */
const char *ssid = "HyggeHytten";
const char *pass = "9B523D55E444";

DBManager dbman;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to TEST NET");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to TEST NET");
  }
  dbman.connect();
  struct Device test;
  test.rssi = -30;

  for (int i = 0; i < 6; i++)
    test.dev_mac[i] = 0xAA;

  // dbman.checkDevice(test);
  dbman.insertDevice(test);

  // dbman.sendDevice(test);
}

void loop() {}
