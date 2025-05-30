#ifndef MY_SERIAL_H
#define MY_SERIAL_H

#include <Arduino.h>
#include <lora.h>

class MySerial {
  
  public:
    MySerial(Lora *lora);
    void parseSerial();

  private:
    Lora *lora;
    void handleSerialMessage(String type);
    void sendPing();
};

#endif