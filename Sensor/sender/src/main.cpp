#include "parser.hpp"
#include "structs.h"
#include <Arduino.h>
#include <WiFi.h>

// Prototypes
bool sendSelf();
bool sendDevice(struct Device dev);

/* Variables */
char ssid[] = "B115-Test-Net";
char password[] = "hygeRMitYWCa";
unsigned char mac[6];
IPAddress server_addr(192, 168, 12, 1);
WiFiClient client;
int status = WL_IDLE_STATUS;

void setup() {
  WiFi.macAddress(mac);
  Serial.begin(115200);
  while (status != WL_CONNECTED) {
    Serial.println("Connecting to network");
    status = WiFi.begin(ssid, password);
    delay(500);
  }

  while (!sendSelf()) {
  }
}

void loop() {
  struct Device device;
  parse(&device);

  // device = parse_sec();

  if (device.rssi != 0) {
    sendDevice(device);
  }
}

bool sendSelf() {
  client.stop();
  char req[] = "GET /api?anchor=%02X:%02X:%02X:%02X:%02X:%02X HTTP/1.1\n"
               "User-Agent: ArduinoWiFi/1.1\n"
               "Connection: close\n";

  char buf[255];

  sprintf(buf, req, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  if (client.connect(server_addr, 80)) {
    client.println(buf);
    Serial.println("Sensor sent");
    return true;
  } else {
    return false;
  }
}

bool sendDevice(struct Device dev) {
  client.stop();

  char req[] = "GET "
               "/api"
               "?anchor=%02X:%02X:%02X:%02X:%02X:%02X&dev_mac=%02X:%"
               "02X:%02X:%02X:%02X:%02X&rssi=%i HTTP/1.1\n"
               "User-Agent: ArduinoWiFi/1.1\n"
               "Connection: close\n";

  char buf[255];

  sprintf(buf, req, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
          dev.dev_mac[0], dev.dev_mac[1], dev.dev_mac[2], dev.dev_mac[3],
          dev.dev_mac[4], dev.dev_mac[5], dev.rssi);

  if (client.connect(server_addr, 80)) {
    client.println(buf);
    Serial.println("Device sent");
    return true;
  } else {
    return false;
  }
}
