#ifndef STRUCTS_H
#define STRUCTS_H
#define MAX_DEVICES 10

struct Device {
  int rssi;
  char dev_mac[6];
};

struct DevicePacket {
  char sens_mac[6];
  struct Device devices[MAX_DEVICES];
};
#endif
