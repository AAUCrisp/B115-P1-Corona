#ifndef PARSER_H
#define PARSER_H

/* Includes */
#include "structs.h"
#include <Arduino.h>
#include <WiFi.h>

/* Prototypes */
struct DevicePacket parse();

//============
// Functions
//============
struct DevicePacket parse() {
  struct DevicePacket packet;
  WiFi.macAddress(packet.sens_mac);

  String buffer;

  char terminator = 0x0a;

  for (int i = 0; i < MAX_DEVICES; i++) {
    struct Device dev;
    buffer = Serial.readStringUntil(terminator);

    const char c * = buffer.c_str();

    int rssi;

    sscanf(c, "%X %X %X %X %X %X %i", dev.dev_mac[0], dev.dev_mac[1],
           dev.dev_mac[2], dev.dev_mac[3], dev.dev_mac[4], dev.dev_mac[5],
           rssi);

    dev.rssi = rssi;
    packet.devices[i] = dev;
  }

  return packet;
}

#endif
