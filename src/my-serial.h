#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <Arduino.h>
#include <lora.h>
#include "utils.h"

class MySerial {
  
  public:
    MySerial(Lora *lora);
    void parseSerial();
    void parseLoraMessage(String msg);

  private:
    Lora *lora;
    void handleSerialMessage(String command, String params);
    void sendPing();
    void sendPingBack();
    void sendConfigSync(String configMsg);
    void updateSettings(String params);
    void syncConfig(String params);
    void updateSettingsAndCheck(String params);
    void printPingSuccess();
    void sendConfigCheckAck();
    void printSuccessConfigUpdate();
    String getStatusString(unsigned long* startTime);

    unsigned long pingStart = 0;
    unsigned long configSyncStart = 0;
};

#endif