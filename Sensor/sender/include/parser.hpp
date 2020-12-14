#ifndef PARSER_H
#define PARSER_H

/* Includes */
#include "structs.h"
#include <Arduino.h>
#include <WiFi.h>

/* Prototypes */
// struct Device parse();

//============
// Functions
//============
//

struct Device parse_sec() {
  struct Device dev;
  String buffer;

  char terminator = 0x0a;

  buffer = Serial.readStringUntil(terminator);

  const char *c = buffer.c_str();

  sscanf(c, "%02X %02X %02X %02X %02X %02X %i", dev.dev_mac[0], dev.dev_mac[1],
         dev.dev_mac[2], dev.dev_mac[3], dev.dev_mac[4], dev.dev_mac[5],
         dev.rssi);

  return dev;
}
void parse(struct Device *dev) {

  String buffer;

  char terminator = 0x0a;

  buffer = Serial.readStringUntil(terminator);

  const char *c = buffer.c_str();

  sscanf(c, "%02X %02X %02X %02X %02X %02X %i", &dev->dev_mac[0],
         &dev->dev_mac[1], &dev->dev_mac[2], &dev->dev_mac[3], &dev->dev_mac[4],
         &dev->dev_mac[5], &dev->rssi);
}

#endif
