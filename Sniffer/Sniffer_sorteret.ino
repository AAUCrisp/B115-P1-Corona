// Original kode lavet af Ray Burnette
// Modificeret af gruppe B115 for kun at få bestemte informationer ud.

#include <ESP8266WiFi.h>
#include "./functions.h"

#define disable 0
#define enable  1
// uint8_t channel = 1;
unsigned int channel = 1;

void setup() {
  Serial.begin(115200);
  //Serial.printf("\n\nSDK version:%s\n\r", system_get_sdk_version());
  //Serial.println(F("ESP8266 mini-sniff by Ray Burnette http://www.hackster.io/rayburne/projects"));
  //Serial.println(F("Type:   /-------MAC------/-----WiFi Access Point SSID-----/  /----MAC---/  Chnl  RSSI"));

  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);
}

void loop() {
  memset(clients_known, 0, sizeof(clients_known)); //Nulstiller clients_known array
  channel = 1;
  wifi_set_channel(channel);
  
  while (true) {
    nothing_new++;                          // Array is not finite, check bounds and adjust if required
    if (nothing_new > 200) {
      nothing_new = 0;
      channel++;
      if (channel == 15) break;             // Only scan channels 1 to 14
      wifi_set_channel(channel);
    }
    delay(1);  // critical processing timeslice for NONOS SDK! No delay(0) yield()
    // Press keyboard ENTER in console with NL active to repaint the screen
    if ((Serial.available() > 0) && (Serial.read() == '\n')) {
      //Serial.println("\n-------------------------------------------------------------------------------------\n");
      for (int u = 0; u < clients_known_count; u++) print_client(clients_known[u]);
    //  for (int u = 0; u < aps_known_count; u++) print_beacon(aps_known[u]);
     // Serial.println("\n-------------------------------------------------------------------------------------\n");
    }
  }
}
