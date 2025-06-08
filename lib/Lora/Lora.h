#ifndef LORA_H
#define LORA_H

#include "Arduino.h"
#include <RadioLib.h>
#include "LoraSettings.h"

#define RADIO_SCLK_PIN 5
#define RADIO_MISO_PIN 19
#define RADIO_MOSI_PIN 27

#define RADIO_CS_PIN 18
#define RADIO_DIO0_PIN 26
#define RADIO_DIO1_PIN 33
#define RADIO_RST_PIN 23

#define LED_PIN 25

class Lora {

  public:
    Lora();
    LoraSettingsManager settings;

    void init(LoraSettings initalSettings);
    void transmit(String msg);
    void check();
    float getRSSI();
    float getSNR();

    void setReceiveCallback(void (* callback)(String msg));
    void setTransmitCallback(std::function<void()> callback);

    static void packetReceiveCallback(void);
    static void packetSentCallback(void);
    static bool interruptFlag;

  private:
    SX1276 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_DIO1_PIN);
  
    bool isTransmitInProgres = false;
    uint32_t lastFlashTime = 0;
    int transmissionState = RADIOLIB_ERR_NONE;
    void (* receiveCallback)(String msg);
    std::function<void()> transmitCallback;
  
    void listen();
    void handleReceiveMessage();
    void flashLedOn();
    void flashLedOff();
};

#endif