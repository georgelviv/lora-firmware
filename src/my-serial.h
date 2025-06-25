#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <Arduino.h>
#include <Lora.h>
#include <Logger.h>
#include "utils.h"

#define ACK_TIMEOUT 5000

class MySerial {
  
  public:
    MySerial(Lora *lora);
    Logger logger;

    void parseSerial();
    void parseLoraMessage(String msg);
    void checkPending();

  private:
    Lora *lora;
    void handleSerialMessage(String command, String params);
    void sendPing(String params);
    void sendPingBack(String params);
    void sendData(String params);
    void sendConfigSync(String configMsg);
    void updateSettings(String params);
    void syncConfig(String params);
    void updateSettingsAndCheck(String params);
    void printPingSuccess(String params);
    void sendConfigCheckAck();
    void handleIncomingSend(String params);
    void printSendSuccess(String params);
    void printSuccessConfigUpdate(String params);
    String getStatusString(unsigned long* startTime, String params);
    void printConfig();

    unsigned long pingStart = 0;
    String pingPendingId = "";

    unsigned long dataStart = 0;
    String dataPendingId = "";
  
    unsigned long configSyncStart = 0;
    unsigned long configCheckStart = 0;
    bool isConfigPending = false;
    bool isConfigCheckPending = false;
    LoraSettings fallbackConfigSyncSettings;
};

#endif