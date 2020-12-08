#include "db.hpp"
#include "parser.hpp"
#include "structs.h"
#include <Arduino.h>
#include <WiFi.h>

/* Connection stuff */
const char *ssid = "B115-Test-Net";
const char *pass = "hygeRMitYWCa";

/* Structs */
struct DevicePacket base;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  WiFi.macAddress(base.sens_mac);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to TEST NET");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to TEST NET");
  }
  dbInit(&base);
  Serial.println("Initialization complete");
}

void loop() {
  /* Parse incoming data */
  struct DevicePacket data;

  data = parse();

  dbSendDevices(&data);
}
