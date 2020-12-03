#ifndef PARSER_H
#define PARSER_H

/* Includes */
#include "structs.h"
#include <WiFi.h>

/* Prototypes */
struct DevicePacket parse(char *buffer);

//============
// Functions
//============
struct DevicePacket parse() {
  struct DevicePacket packet;
  WiFi.macAddress(packet.sens_mac);

  char buffer[255];

  char terminator = "\n";

  for (int i = 0; i < MAX_DEVICES; i++) {
    struct Device dev;
    buffer = Serial.readStringUntil(terminator);
    int rssi;
    char mac[6];

    sscanf(buffer, "%i %X %X %X %X %X %X", rssi, mac[0], mac[1], mac[2], mac[3],
           mac[4], mac[5]);

    dev.rssi = rssi;
    for (int i = 0; i < 6; i++) {
      dev.dev_mac[i] = mac[i];
    }
    packet.devices[i] = dev;
  }

  return packet;
}

#endif
