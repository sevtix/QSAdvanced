#include <Wire.h>
#include <EEPROM.h>

#include <M5AtomS3.h>
#include "M5_ADS1115.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// VMETER UNIT
ADS1115 GEARVMETER;

// BLE SERVER & CHARACTERISTICS
BLEServer *BLE_SERVER = NULL;
//BLECharacteristic *BLE_LOG_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_ADC_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_CUT_CHARACTERISTIC = NULL;

// IS DEVICE CONNECT VIA BLE?
bool DEVICECONNECTED = false;

// IS POWER BEING CUT?
uint8_t CUT = 0;

// IS CUT RELAIS ON?
bool RELAIS_CUT = 0;

// WHICH GEAR IS ENGAGED?
uint8_t GEAR = 0;

// GEAR DEFINITIONS (DEFAULTS)
uint16_t GEAR_1_START = 612;
uint16_t GEAR_1_END = 616;
uint16_t GEAR_N_START = 800;
uint16_t GEAR_N_END = 1100;
uint16_t GEAR_2_START = 1355;
uint16_t GEAR_2_END = 1362;
uint16_t GEAR_3_START = 2104;
uint16_t GEAR_3_END = 2112;
uint16_t GEAR_4_START = 2862;
uint16_t GEAR_4_END = 2974;
uint16_t GEAR_5_START = 3612;
uint16_t GEAR_5_END = 3620;
uint16_t GEAR_6_START = 4362;
uint16_t GEAR_6_END = 4370;

// GEAR MARGIN
// IS ADDED TO EACH _END AND SUBSTRACTED FROM EACH _START
uint16_t GEAR_RANGE_MARGIN = 10;

// BLE CHARACTERISTIC
BLECharacteristic *BLE_GEAR_1_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_1_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_N_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_N_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_2_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_2_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_3_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_3_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_4_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_4_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_5_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_5_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_6_START_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_6_END_CHARACTERISTIC = NULL;
BLECharacteristic *BLE_GEAR_RANGE_MARGIN_CHARACTERISTIC = NULL;

class BLE_SERVER_CALLBACK : public BLEServerCallbacks {
  void onConnect(BLEServer *BLE_SERVER) {
    DEVICECONNECTED = true;
    AtomS3.Lcd.setTextColor(GREEN);
    AtomS3.Lcd.drawString("BLE", 64, 64, 4);
  }

  void onDisconnect(BLEServer *BLE_SERVER) {
    BLEDevice::startAdvertising();
    DEVICECONNECTED = false;
    AtomS3.Lcd.setTextColor(RED);
    AtomS3.Lcd.drawString("BLE", 64, 64, 4);
  }
};

class BLE_GEAR_1_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_1_START & 0xFF;
    dataBytes[1] = (GEAR_1_START >> 8) & 0xFF;
    BLE_GEAR_1_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_1_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(0, GEAR_1_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_1_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_1_END & 0xFF;
    dataBytes[1] = (GEAR_1_END >> 8) & 0xFF;
    BLE_GEAR_1_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_1_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(2, GEAR_1_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_N_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_N_START & 0xFF;
    dataBytes[1] = (GEAR_N_START >> 8) & 0xFF;
    BLE_GEAR_N_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_N_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(4, GEAR_N_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_N_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_N_END & 0xFF;
    dataBytes[1] = (GEAR_N_END >> 8) & 0xFF;
    BLE_GEAR_N_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_N_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(6, GEAR_N_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_2_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_2_START & 0xFF;
    dataBytes[1] = (GEAR_2_START >> 8) & 0xFF;
    BLE_GEAR_2_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_2_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(8, GEAR_2_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_2_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_2_END & 0xFF;
    dataBytes[1] = (GEAR_2_END >> 8) & 0xFF;
    BLE_GEAR_2_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_2_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(10, GEAR_2_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_3_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_3_START & 0xFF;
    dataBytes[1] = (GEAR_3_START >> 8) & 0xFF;
    BLE_GEAR_3_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_3_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(12, GEAR_3_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_3_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_3_END & 0xFF;
    dataBytes[1] = (GEAR_3_END >> 8) & 0xFF;
    BLE_GEAR_3_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_3_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(14, GEAR_3_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_4_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_4_START & 0xFF;
    dataBytes[1] = (GEAR_4_START >> 8) & 0xFF;
    BLE_GEAR_4_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_4_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(16, GEAR_4_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_4_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_4_END & 0xFF;
    dataBytes[1] = (GEAR_4_END >> 8) & 0xFF;
    BLE_GEAR_4_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_4_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(18, GEAR_4_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_5_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_5_START & 0xFF;
    dataBytes[1] = (GEAR_5_START >> 8) & 0xFF;
    BLE_GEAR_5_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_5_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(20, GEAR_5_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_5_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_5_END & 0xFF;
    dataBytes[1] = (GEAR_5_END >> 8) & 0xFF;
    BLE_GEAR_5_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_5_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(22, GEAR_5_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_6_START_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_6_START & 0xFF;
    dataBytes[1] = (GEAR_6_START >> 8) & 0xFF;
    BLE_GEAR_6_START_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_6_START = *(uint16_t *)characteristicValue.data();
      EEPROM.put(24, GEAR_6_START);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_6_END_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_6_END & 0xFF;
    dataBytes[1] = (GEAR_6_END >> 8) & 0xFF;
    BLE_GEAR_6_END_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_6_END = *(uint16_t *)characteristicValue.data();
      EEPROM.put(26, GEAR_6_END);
      EEPROM.commit();
    }
  }
};

class BLE_GEAR_RANGE_MARGIN_CALLBACK : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    uint8_t dataBytes[2];
    dataBytes[0] = GEAR_RANGE_MARGIN & 0xFF;
    dataBytes[1] = (GEAR_RANGE_MARGIN >> 8) & 0xFF;
    BLE_GEAR_RANGE_MARGIN_CHARACTERISTIC->setValue(dataBytes, sizeof(dataBytes));
  }
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string characteristicValue = pCharacteristic->getValue();
    if (characteristicValue.length() == sizeof(uint16_t)) {
      GEAR_RANGE_MARGIN = *(uint16_t *)characteristicValue.data();
      EEPROM.put(28, GEAR_RANGE_MARGIN);
      EEPROM.commit();
    }
  }
};

void setup() {
  AtomS3.begin();

  EEPROM.begin(32);

  pinMode(7, OUTPUT);

  // LOAD FROM EEPROM
  EEPROM.get(0, GEAR_1_START);
  EEPROM.get(2, GEAR_1_END);
  EEPROM.get(4, GEAR_N_START);
  EEPROM.get(6, GEAR_N_END);
  EEPROM.get(8, GEAR_2_START);
  EEPROM.get(10, GEAR_2_END);
  EEPROM.get(12, GEAR_3_START);
  EEPROM.get(14, GEAR_3_END);
  EEPROM.get(16, GEAR_4_START);
  EEPROM.get(18, GEAR_4_END);
  EEPROM.get(20, GEAR_5_START);
  EEPROM.get(22, GEAR_5_END);
  EEPROM.get(24, GEAR_6_START);
  EEPROM.get(26, GEAR_6_END);
  EEPROM.get(28, GEAR_RANGE_MARGIN);

  Wire.begin(38, 39, 100000L);
  Serial.begin(115200);

  AtomS3.Lcd.clear();
  AtomS3.Lcd.setTextDatum(MC_DATUM);
  AtomS3.Lcd.setTextColor(RED);
  AtomS3.Lcd.drawString("BLE", 64, 64, 4);

  BLEDevice::init("QSTest3");
  BLE_SERVER = BLEDevice::createServer();
  BLE_SERVER->setCallbacks(new BLE_SERVER_CALLBACK());
  BLEService *BLE_SERVICE = BLE_SERVER->createService(BLEUUID("798D0A29-DD0B-42A9-A3BC-2DF18819D5DF"), 50);

  //BLE_LOG_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-000000000001"), BLECharacteristic::PROPERTY_NOTIFY);
  //BLE_LOG_CHARACTERISTIC->addDescriptor(new BLE2902());

  BLE_GEAR_ADC_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-000000000001"), BLECharacteristic::PROPERTY_NOTIFY);
  BLE_GEAR_ADC_CHARACTERISTIC->addDescriptor(new BLE2902());

  BLE_GEAR_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-000000000002"), BLECharacteristic::PROPERTY_NOTIFY);
  BLE_GEAR_CHARACTERISTIC->addDescriptor(new BLE2902());

  BLE_CUT_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-000000000003"), BLECharacteristic::PROPERTY_NOTIFY);
  BLE_CUT_CHARACTERISTIC->addDescriptor(new BLE2902());

  BLE_GEAR_1_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000001"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_1_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_1_START_CALLBACK());

  BLE_GEAR_1_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000002"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_1_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_1_END_CALLBACK());

  BLE_GEAR_N_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000003"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_N_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_N_START_CALLBACK());
  BLE_GEAR_N_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000004"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_N_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_N_END_CALLBACK());

  BLE_GEAR_2_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000005"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_2_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_2_START_CALLBACK());
  BLE_GEAR_2_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000006"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_2_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_2_END_CALLBACK());

  BLE_GEAR_3_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000007"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_3_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_3_START_CALLBACK());
  BLE_GEAR_3_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000008"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_3_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_3_END_CALLBACK());

  BLE_GEAR_4_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000009"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_4_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_4_START_CALLBACK());
  BLE_GEAR_4_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000010"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_4_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_4_END_CALLBACK());

  BLE_GEAR_5_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000011"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_5_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_5_START_CALLBACK());
  BLE_GEAR_5_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000012"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_5_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_5_END_CALLBACK());

  BLE_GEAR_6_START_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000013"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_6_START_CHARACTERISTIC->setCallbacks(new BLE_GEAR_6_START_CALLBACK());
  BLE_GEAR_6_END_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000014"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_6_END_CHARACTERISTIC->setCallbacks(new BLE_GEAR_6_END_CALLBACK());

  BLE_GEAR_RANGE_MARGIN_CHARACTERISTIC = BLE_SERVICE->createCharacteristic(BLEUUID("00000000-0000-0000-0000-100000000015"), BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  BLE_GEAR_RANGE_MARGIN_CHARACTERISTIC->setCallbacks(new BLE_GEAR_RANGE_MARGIN_CALLBACK());

  BLE_SERVICE->start();

  BLEAdvertising *monitorAdvertising = BLEDevice::getAdvertising();
  monitorAdvertising->addServiceUUID(BLE_SERVICE->getUUID());
  monitorAdvertising->setScanResponse(true);
  BLEDevice::startAdvertising();

  GEARVMETER.setRate(RATE_250);
}

void loop() {

  // GET ADC DATA
  float rawadc = GEARVMETER.getValue();
  uint16_t ADCVAL = 0;

  if (rawadc >= 0) {
    ADCVAL = (uint16_t)round(GEARVMETER.getValue());
  } else {
    ADCVAL = 0;
  }

  // FALLBACK
  GEAR = 0;
  CUT = 1;

  // GEAR 1 RANGE
  if (ADCVAL >= (GEAR_1_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_1_END + GEAR_RANGE_MARGIN)) {
    GEAR = 1;
    CUT = 0;
  }

  // GEAR N RANGE
  if (ADCVAL >= (GEAR_N_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_N_END + GEAR_RANGE_MARGIN)) {
    GEAR = 0;
    CUT = 0;
  }

  // GEAR 2 RANGE
  if (ADCVAL >= (GEAR_2_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_2_END + GEAR_RANGE_MARGIN)) {
    GEAR = 2;
    CUT = 0;
  }

  // GEAR 3 RANGE
  if (ADCVAL >= (GEAR_3_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_3_END + GEAR_RANGE_MARGIN)) {
    GEAR = 3;
    CUT = 0;
  }

  // GEAR 4 RANGE
  if (ADCVAL >= (GEAR_4_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_4_END + GEAR_RANGE_MARGIN)) {
    GEAR = 4;
    CUT = 0;
  }

  // GEAR 5 RANGE
  if (ADCVAL >= (GEAR_5_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_5_END + GEAR_RANGE_MARGIN)) {
    GEAR = 5;
    CUT = 0;
  }

  // GEAR 6 RANGE
  if (ADCVAL >= (GEAR_6_START - GEAR_RANGE_MARGIN) && ADCVAL <= (GEAR_6_END + GEAR_RANGE_MARGIN)) {
    GEAR = 6;
    CUT = 0;
  }

  if (CUT == 1) {
    CutInjection();
  }

  if (CUT == 0) {
    UncutInjection();
  }

  if (DEVICECONNECTED) {
    //sendLog(";" + String(millis()) + ";" + String(ADCVAL) + ";" + String(GEAR) + ";" + String(CUT) + ";");

    // Send GEAR
    uint8_t gearBytes[1];
    gearBytes[0] = GEAR;
    BLE_GEAR_CHARACTERISTIC->setValue(gearBytes, sizeof(gearBytes));

    // Send GEAR ADC
    uint8_t gearADCBytes[2];
    gearADCBytes[0] = ADCVAL & 0xFF;
    gearADCBytes[1] = (ADCVAL >> 8) & 0xFF;
    BLE_GEAR_ADC_CHARACTERISTIC->setValue(gearADCBytes, sizeof(gearADCBytes));

    // Send CUT
    uint8_t cutBytes[1];
    cutBytes[0] = CUT;
    BLE_CUT_CHARACTERISTIC->setValue(cutBytes, sizeof(cutBytes));

    BLE_GEAR_CHARACTERISTIC->notify();
    BLE_GEAR_ADC_CHARACTERISTIC->notify();
    BLE_CUT_CHARACTERISTIC->notify();
  }
}

void CutInjection() {
  // Save unneccesary gpio operations
  if (!RELAIS_CUT) {
    digitalWrite(7, HIGH);
    RELAIS_CUT = true;
  }
}

void UncutInjection() {
  // Save unneccesary gpio operations
  if (RELAIS_CUT) {
    digitalWrite(7, LOW);
    RELAIS_CUT = false;
  }
}