#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <Arduino.h>
#include <lora.h>

class MySerial {
  
  public:
    MySerial(Lora *lora);
    void parseSerial();
    void parseLoraMessage(String msg);

  private:
    Lora *lora;
    void handleSerialMessage(String command, String params);
    void sendPing();
    void updateSettings(String params);
    int parseParams(String str, String paramsPairs[10][2]);
};

#endif