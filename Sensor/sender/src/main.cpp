#include "parser.hpp"
#include "structs.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

// Prototypes
void sendDevice(struct Device dev);

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
    delay(10000);
  }
}

void loop() {
  struct Device device;
  parse(&device);

  if (device.rssi < 0 && device.rssi > -250) {
    sendDevice(device);
  }
}

void sendDevice(struct Device dev) {
  String serverName = "http://192.168.12.1/api.php";
  HTTPClient http;

  char anchor[255];
  sprintf(anchor, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2],
          mac[3], mac[4], mac[5]);

  char dev_mac[255];
  sprintf(dev_mac, "%02X:%02X:%02X:%02X:%02X:%02X", dev.dev_mac[0],
          dev.dev_mac[1], dev.dev_mac[2], dev.dev_mac[3], dev.dev_mac[4],
          dev.dev_mac[5]);

  String serverPath = serverName + "?anchor=" + anchor + "&dev_mac=" + dev_mac +
                      "&rssi=" + dev.rssi;

  http.begin(serverPath.c_str());

  int httpResponse = http.GET();

  if (httpResponse > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponse);
    Serial.print("[QUERY] ");
    Serial.println(serverPath);
  }
}
