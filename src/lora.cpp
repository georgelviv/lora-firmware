#include "lora.h"

Lora::Lora(): settings(&radio) {};

bool Lora::interruptFlag = false;

void Lora::init(LoraSettings initalSettings) {
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
  pinMode(LED_PIN, OUTPUT);

  delay(1500);

  int state = radio.begin();
  Serial.print(F("[Lora] Initializing ... "));
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  this->radio.setPacketReceivedAction(Lora::packetReceiveCallback);
  this->radio.setPacketSentAction(Lora::packetSentCallback);

  this->settings.updateSettings(initalSettings);

  delay(1000);

  this->listen();
}

void Lora::packetReceiveCallback() {
  Lora::interruptFlag = true;
}

void Lora::packetSentCallback() {
  Lora::interruptFlag = true;
}

void Lora::transmit(String msg) {
  this->transmissionState = this->radio.startTransmit(msg);
  this->isTransmitInProgres = true;
}


void Lora::flashLedOn() {
  digitalWrite(LED_PIN, HIGH);
  this->lastFlashTime = millis();
}

void Lora::flashLedOff() {
  if ((millis() - this->lastFlashTime) > 50) {
    digitalWrite(LED_PIN, LOW);
  }
}

void Lora::check() {
  if (Lora::interruptFlag) {
    Lora::interruptFlag = false;
    this->flashLedOn();
    if (this->isTransmitInProgres) {
      if (this->transmissionState != RADIOLIB_ERR_NONE) {
        Serial.print(F("failed, code "));
        Serial.println(transmissionState);
      }
      this->isTransmitInProgres = false;
      this->listen();
    } else {
      this->handleReceiveMessage();
    }
  }

  this->flashLedOff();
}

void Lora::listen() {
  int state = radio.startReceive();
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); } 
  }
}

void Lora::handleReceiveMessage() {
  String payload = "0";
  
  int state = radio.readData(payload);
  if (state == RADIOLIB_ERR_NONE) {
    // float rssi = radio.getRSSI();
    // float snr = radio.getSNR();
  
    if (this->receiveCallback) {
      this->receiveCallback(payload);
    }
  }
}

void Lora::setReceiveCallback(void (* callback)(String msg)) {
  this->receiveCallback = callback;
}