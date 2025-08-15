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
}

void Lora::packetReceiveCallback() {
  Lora::interruptFlag = true;
}

void Lora::packetSentCallback() {
  Lora::interruptFlag = true;
}

std::pair<int, int> Lora::transmit(String msg) {
  this->splitTransmitMessage(msg);

  this->transmitChunk();

  int byteCount = strlen(msg.c_str());
  int chunkCount = this->chunksToTransmit.size();

  return std::make_pair(byteCount, chunkCount); 
}

void Lora::transmitChunk() {
  String currentChunk = this->chunksToTransmit[this->currentChunkIndex];
  this->transmissionState = this->radio.startTransmit(currentChunk);
  this->isTransmitInProgres = true;
}

void Lora::splitTransmitMessage(String msg) {
  this->currentChunkIndex = 0;
  this->chunksToTransmit.clear();

  LoraSettings settings = this->settings.getSettings();

  int chunkSize = settings.implicitHeader ? settings.headerSize : 200;
  int totalChunks = ceil((float)msg.length() / chunkSize);

  for (int i = 0; i < totalChunks; i++) {
    int start = i * chunkSize;
    String header = "[" + String(i + 1) + "/" + String(totalChunks) + "]";
    int bodySize = chunkSize - header.length();
    String body = msg.substring(start, start + bodySize);
    String framedChunk = header + body;

    if (settings.implicitHeader) {
      int diff = settings.headerSize - framedChunk.length();
      framedChunk.reserve(settings.headerSize);
      for (int j = 0; j < diff; j++) {
        framedChunk += '_';
      }
    }

    this->chunksToTransmit.push_back(framedChunk);
  }
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
        this->onTransmitDone();
      }
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
  if (state != RADIOLIB_ERR_NONE) {
    this->logger.log("Error receive", state);
    return;
  }

  int startBracket = payload.indexOf('[');
  int endBracket = payload.indexOf(']');

  if (startBracket != -1 && endBracket != -1 && endBracket > startBracket) {
    String header = payload.substring(startBracket + 1, endBracket);
    int slashIndex = header.indexOf('/');

    if (slashIndex != -1) {
      int partNum = header.substring(0, slashIndex).toInt();
      int totalParts = header.substring(slashIndex + 1).toInt();

      String messageContent = payload.substring(endBracket + 1);
      this->receivedChunks.push_back(messageContent);

      if (partNum == totalParts) {
        String fullString = "";
        for (int i = 0; i < this->receivedChunks.size(); i++) {
          String chunk = this->receivedChunks[i];
          fullString += chunk;
        }

        if (this->receiveCallback) {
          this->receiveCallback(fullString);
        }
        
        this->receivedChunks.clear();
      }
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

void Lora::onTransmitDone() {
  this->isTransmitInProgres = false;
  if (this->currentChunkIndex + 1 >= this->chunksToTransmit.size()) {
    this->listen();
    if (this->transmitCallback) {
      this->transmitCallback();
    }
  } else {
    this->currentChunkIndex = this->currentChunkIndex + 1;
    this->transmitChunk();
  }
}