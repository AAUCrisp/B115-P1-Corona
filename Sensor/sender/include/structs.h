#ifndef STRUCTS_H
#define STRUCTS_H
#define MAX_DEVICES 10

struct Device {
  int rssi;
  unsigned char dev_mac[6];
};

struct DevicePacket {
  unsigned char sens_mac[6];
  struct Device devices[MAX_DEVICES];
};
#endif
