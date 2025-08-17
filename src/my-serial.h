#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <Arduino.h>
#include <Lora.h>
#include <Logger.h>
#include "utils.h"

#define ACK_TIMEOUT 10000

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
    void handlePing(String params);
    void sendData(String params);
    void sendConfigSync(String configMsg);
    void updateSettings(String params);
    void handleIncomingConfigSync(String params);
    void handleConfigSyncAck(String params);
    void handlePingAck(String params);
    void handleConfigSyncCheck(String params);
    void handleIncomingSend(String params);
    void handleSendAck(String params);
    void handleIncomingConfigSyncCheck(String params);
    String getStatusString(unsigned long* startTime, String params);
    void printConfig();

    void sendLora(String msg);
    void prepareTransmit(String params, String command);
    void handleAck(String params, String command);
    void sendAck(String params, String command);

    unsigned long pendingTimeoutStart = 0;
    unsigned long pendingStart = 0;
    String pendingId = "";
    String pendingTimeoutMsg = "";
    LoraSettings fallbackConfigSyncSettings;
    String settingsToUpdateOnTransmit = "";

    int payloadSize = 0;
    int chunksCount = 0;
};

#endif