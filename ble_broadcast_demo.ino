#include <LBLE.h>
#include "Arduino.h"
#include "CLBLEPeriphral.h"

const int sensor = 14;//use photosensor as example, intput at pin14(analog)
const int usr_btn = 6;
const int SAMPLE_RATE = 1000;
String data;
char* devName = "LumosBLE-B";

char* serviceUUID = "00001802-0000-1000-8000-00805f9b34fb";
LBLEUuid uuid(serviceUUID);

LBLEService uartService(serviceUUID);
LBLECharacteristicString txCharacteristic("00002a05-0000-1000-8000-00805f9b34fb", LBLE_READ | LBLE_WRITE);//Rx

void setup() {  

  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // Initialize BLE subsystem
  LBLE.begin();
  while (!LBLE.ready()) {
    delay(100);
  }

  String address = LBLE.getDeviceAddress().toString();

  Serial.println("=== BLE ready ===");
  Serial.print(devName);
  Serial.print(" Address = [");
  Serial.print(LBLE.getDeviceAddress());
  Serial.println("]");

  
  CLBLEAdvertisementData advertisement;
  advertisement.configAsConnectableWithPayload(devName, serviceUUID, 0);

  LBLEPeripheral.setName(devName);
  
  uartService.addAttribute(txCharacteristic);
  LBLEPeripheral.addService(uartService);
  

  LBLEPeripheral.advertise(advertisement);

  LBLEPeripheral.begin();
  
  pinMode(sensor, INPUT);

}

void loop() { 
  delay(SAMPLE_RATE);
  int r = analogRead(sensor);
  Serial.println(r);
//  Serial.print("conected=");
//  Serial.println(LBLEPeripheral.connected());
  if(LBLEPeripheral.connected())
  {
    String result = r;
    txCharacteristic.setValue(result);
    LBLEPeripheral.notifyAll(txCharacteristic);
  }else{
    CLBLEAdvertisementData advertisement;
    advertisement.configAsConnectableWithPayload(devName, serviceUUID, r);
    LBLEPeripheral.stopAdvertise();
    LBLEPeripheral.advertise(advertisement);
    Serial.println(r);
  }
}
