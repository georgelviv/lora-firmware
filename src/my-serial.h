#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <Arduino.h>
#include <Lora.h>
#include "utils.h"

class MySerial {
  
  public:
    MySerial(Lora *lora);
    void parseSerial();
    void parseLoraMessage(String msg);

  private:
    Lora *lora;
    void handleSerialMessage(String command, String params);
    void sendPing(String params);
    void sendPingBack(String params);
    void sendConfigSync(String configMsg);
    void updateSettings(String params);
    void syncConfig(String params);
    void updateSettingsAndCheck(String params);
    void printPingSuccess(String params);
    void sendConfigCheckAck();
    void printSuccessConfigUpdate(String params);
    String getStatusString(unsigned long* startTime, String params);
    void printConfig();

    unsigned long pingStart = 0;
    unsigned long configSyncStart = 0;
};

#endif