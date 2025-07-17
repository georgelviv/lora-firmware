#include "lora.h"

Lora::Lora() : logger("Lora", true), settings(&radio, &logger)  {}

bool Lora::interruptFlag = false;

void Lora::init(LoraSettings initalSettings) {
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
  pinMode(LED_PIN, OUTPUT);

  delay(1500);

  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE) {
    this->logger.log("Module init success");
  } else {
    this->logger.log("Module init failed with code:", state);
    while (true) { delay(10); }
  }

  this->radio.setPacketReceivedAction(Lora::packetReceiveCallback);
  this->radio.setPacketSentAction(Lora::packetSentCallback);

  delay(1000);
  this->settings.updateSettings(initalSettings);
  // this->listen();
}

void Lora::packetReceiveCallback() {
  Lora::interruptFlag = true;
}

void Lora::packetSentCallback() {
  Lora::interruptFlag = true;
}

int Lora::transmit(String msg) {
  LoraSettings settings = this->settings.getSettings();

  if (settings.implicitHeader) {
    if (msg.length() > settings.headerSize) {
      this->logger.log("Message too long for implicit header:", msg);
      return 0;
    }

    int diff = settings.headerSize - msg.length();
    msg.reserve(settings.headerSize);
    for (int i = 0; i < diff; i++) {
      msg += '_';
    }
  }

  this->transmissionState = this->radio.startTransmit(msg);
  this->isTransmitInProgres = true;

  return strlen(msg.c_str());  
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
        this->logger.log("Transmission failed:", transmissionState);
      } else {
        if (this->transmitCallback) {
          this->transmitCallback();
        }
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
    this->logger.log("Listen failed:", state);
    while (true) { delay(10); } 
  } else {
    this->logger.info("Listening");
  }
}

void Lora::handleReceiveMessage() {
  String payload = "0";
  
  int state = radio.readData(payload);
  if (state == RADIOLIB_ERR_NONE) {  
    if (this->receiveCallback) {
      this->receiveCallback(payload);
    }
  }
}

float Lora::getRSSI() {
  return radio.getRSSI();
}

float Lora::getSNR() {
  return radio.getSNR();
}

int Lora::getTOA(int payloadLength) { 
  float time = this->radio.getTimeOnAir(payloadLength) / 1000;
  return static_cast<int>(time); 
}

void Lora::setReceiveCallback(void (* callback)(String msg)) {
  this->receiveCallback = callback;
}

void Lora::setTransmitCallback(std::function<void()> callback) {
  this->transmitCallback = callback;
}