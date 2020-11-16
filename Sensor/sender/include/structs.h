#define MAX_DEVICES 10

struct Device {
    int rssi;
    byte dev_mac[6];
};

struct DevicePacket {
    byte sens_mac[6];
    struct Device devices[MAX_DEVICES];
};
