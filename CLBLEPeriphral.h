#include <LBLE.h>
#include <constants.h>
#include "LBLEPeriphral.h"
#include <Arduino.h>
class CLBLEAdvertisementData:public LBLEAdvertisementData{
  public:
    void configAsConnectableWithPayload(const char* deviceName, const LBLEUuid& uuid, int16_t data);
    void addManufactureData(int16_t data);
};
