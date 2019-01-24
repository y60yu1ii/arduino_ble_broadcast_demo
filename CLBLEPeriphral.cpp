#include "CLBLEPeriphral.h"

extern "C"
{
#include "utility/ard_ble.h"
#include "utility/ard_bt_builtin_profile.h"
#include "utility/ard_bt_attr_callback.h"
}


void CLBLEAdvertisementData::addManufactureData(int16_t data)
{
    LBLEAdvDataItem item;

    item.adType = BT_GAP_LE_AD_TYPE_MANUFACTURER_SPECIFIC;
    item.adDataLen = 0x08;

    item.adData[0] = 0x01;
    item.adData[1] = 0x00;
    //   byte low = val & 0xff;
    //   byte high = (val >> 8) & 0xff;
    item.adData[2] = data & 0xff;
    item.adData[3] = (data >> 8) & 0xff;


    addAdvertisementData(item);
}


void CLBLEAdvertisementData::configAsConnectableWithPayload(const char* deviceName, const LBLEUuid& uuid, int16_t data)
{
    // //clear all
    LBLEAdvertisementData();

    addFlag(BT_GAP_LE_AD_FLAG_BR_EDR_NOT_SUPPORTED | BT_GAP_LE_AD_FLAG_GENERAL_DISCOVERABLE);

    // UUID
    LBLEAdvDataItem item;
    if(uuid.is16Bit())
    {
        // 16-bit UUID
        item.adType = BT_GAP_LE_AD_TYPE_16_BIT_UUID_COMPLETE;
        item.adDataLen = 2;	// 16 Bit UUID = 2 bytes
        const uint16_t uuid16 = uuid.getUuid16();
        item.adData[0] = uuid16 & 0x00FF;
        item.adData[1] = (uuid16 & 0xFF00)>>8;
    }
    else
    {
        // 128-bit UUID
        item.adType = BT_GAP_LE_AD_TYPE_128_BIT_UUID_COMPLETE;
        item.adDataLen = 16;	// 128 Bit UUID = 16 bytes
        uuid.toRawBuffer(item.adData, item.adDataLen);
    }
    addAdvertisementData(item);

    addManufactureData(data);

    // Usually we need a name for iOS devices to list our peripheral device.
    addName(deviceName);
}

